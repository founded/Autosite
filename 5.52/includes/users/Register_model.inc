<?php
    namespace users;
	class Register_model extends \users\Profile_model{
	    public function __construct() {
            parent::__construct();
            $this->isinstalled = self::isInstalled();
	    }
        public function __install(){
            if(!parent::isInstalled()){
                parent::__install();
                print 'parrent not installed';
            }
            if(!$this->existTableEmail()){
                $dbSql = 'CREATE TABLE `'.\autosystem\DBConn::getTableNameFor('email').'`(
                         `id`     INT(20)ZEROFILL NOT NULL AUTO_INCREMENT,
                         `email`  VARCHAR(150) COLLATE '.\autosystem\DBConn::getCOLLATE()." NOT NULL,
                         `emailchecked` ENUM('TRUE','FALSE')NOT NULL DEFAULT 'FALSE',
                         `emailsubscribed` ENUM('TRUE','FALSE')NOT NULL DEFAULT 'FALSE',
                         `ENABLED` ENUM('TRUE','FALSE')NOT NULL DEFAULT 'TRUE',
                         `VISIBLE` ENUM('TRUE','FALSE')NOT NULL DEFAULT 'TRUE',
                         PRIMARY KEY (`id`,`email`)
                    )ENGINE = InnoDB DEFAULT CHARSET=".\autosystem\DBConn::getCHARSET().' COLLATE='.\autosystem\DBConn::getCOLLATE().';';
                $result1 = $this->DB->query($dbSql);
                $return1 = $result1->rowCount()==1;                                                  
                if(!$resultx =$this->DB->query('alter table '.\autosystem\DBConn::getTableNameFor('email').' AUTO_INCREMENT=1654001001;')){
                   //print_r($resultx);
                }
            }else{
                $return1 = true;
            }
            if(!$this->existTableAccound()){
                $dbSql = 'CREATE TABLE IF NOT EXISTS `'.\autosystem\DBConn::getTableNameFor('accound').'`(
                        `id`  INT(20)ZEROFILL NOT NULL AUTO_INCREMENT,
                        `username`  varchar(100) COLLATE '.\autosystem\DBConn::getCOLLATE().' NOT NULL,
                        `pwdx`  varchar(100) COLLATE '.\autosystem\DBConn::getCOLLATE()." NOT NULL,
                        `ACTIVATED` ENUM('TRUE','FALSE')NOT NULL DEFAULT 'FALSE',
                        `ENABLED` ENUM('TRUE','FALSE')NOT NULL DEFAULT 'TRUE',
                        `email_id` INT(20)ZEROFILL NOT NULL,
                        `key`  varchar(50) COLLATE ".\autosystem\DBConn::getCOLLATE().' NOT NULL,
                        `updateDate` datetime DEFAULT NULL,
                        `imputDate` datetime NOT NULL,
                         PRIMARY KEY (`id`)
                    )ENGINE = InnoDB DEFAULT CHARSET='.\autosystem\DBConn::getCHARSET().' COLLATE='.\autosystem\DBConn::getCOLLATE().';'
                    ;
                $result2 = $this->DB->query($dbSql);
                $return2 = $result2->rowCount()==1;
                if(!$resultx = $this->DB->query('alter table '.\autosystem\DBConn::getTableNameFor('accound').' AUTO_INCREMENT=1654001001;')){
                  // print_r($resultx);
                }
            }else{
                $return2 = true;
            }
            return $return2 && $return1;
        }
        public function isInstalled(){
            return $this->isinstalled||parent::isInstalled()&& $this->existTableEmail()&& $this->existTableAccound();
        }
        private function existTableEmail(){        return $this->existTableLabel('email');      }
        private function existTableAccound(){      return $this->existTableLabel('accound');    }
        public function save($username,$password,$email,$Akey,$activatemail = true){
            $e = \autosystem\DBConn::getTableNameFor('email');
            $a = \autosystem\DBConn::getTableNameFor('accound');
            $key ='INSERT INTO `'.$e.'`(`id`,`email`) '.
                  'VALUES(NULL,:email) RETURNING email_id';
            $value = [':email'=>$email];
            $sql[$key]= $value;             
            $key ='INSERT INTO `'.$a.'`(`id`,`username`,`pwdx`,`ENABLED`,`email_id`,`key`,imputDate) '.
                  'VALUES(NULL,:username,:pwdx,:ENABLED,:email_id,:key,:imputDate) RETURNING accound_id';
            $value = [':username'=>$username,':pwdx'=>$password,':ENABLED'=>'FALSE',':email_id'=>'',':key'=>$Akey,':imputDate'=>$this->nowDateTime()];
            $sql[$key]= $value;
            if($arr = $this->transaction($sql)){
              if(array_key_exists('accound_id',$arr) ){
                   if($activatemail){
                     $newmailer = new \Amail\CreateAEmail();
                     $newmailer->activationmail($arr['accound_id']);
                   }
              }
              return true;
            }
            return false;
        }
        public function getModelInfo(){       return 'Register.model.info';       }
        public function getVersionNr(){       return (float)5.4;        }
        public function getRuleDefault(){     return 'PUBLIC';        }
    }
?>