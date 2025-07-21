<?php
    namespace users;
	class ContactInfo_model extends \users\Profile_model{
	    public $phonetypes = array('FIXED_LINE'=>'fixed','MOBILE'=> 'mobile','FIXED_LINE_OR_MOBILE'=>'notspecificedphone','TOLL_FREE'=> 'payfree','PREMIUM_RATE'=> 'premium','SHARED_COST'=> 'shared','VOIP'=> 'voip','PERSONAL_NUMBER'=> 'prive','PAGER'=> 'page','UAN'=> '','UNKNOWN'=> '');
        public $states = array('ACTIVE' => 'active','INACTIVE' => 'inactive','ERROR' => 'on error');
        protected $isinstalled;
        public function __install(){
            if(!$this->existTableUser_contact_Phone()){
                $sql = 'CREATE TABLE IF NOT EXISTS `'.\autosystem\DBConn::getTableNameFor('user_contact_telephone').'`(
                        `id`    INT(10) NOT NULL AUTO_INCREMENT,
                        `Phonenr`  VARCHAR(50) COLLATE '.\autosystem\DBConn::getCOLLATE().' NOT NULL,
                        `Phonetype` ENUM(\''.implode('\',\'',array_keys($this->phonetypes)).'\') NOT NULL,
                        `ip`  varchar(50) COLLATE '.\autosystem\DBConn::getCOLLATE().' NOT NULL,
                        PRIMARY KEY(`id`)
                    )ENGINE = InnoDB DEFAULT CHARSET='.\autosystem\DBConn::getCHARSET().' COLLATE='.\autosystem\DBConn::getCOLLATE().';';
                $result1 = $this->DB->query($sql);
                $return1 = $result1->rowCount()==1;
            }else{
                $return1 = true;
            }
            if(!$this->existTableUser__phones()){                         
                $sql= 'CREATE TABLE IF NOT EXISTS `'.\autosystem\DBConn::getTableNameFor('user_telephones').'`(
                    `phones_id`   INT(10) NOT NULL,
                    `landcode_id` INT(10) DEFAULT 0,
                    `accound_id`  INT(12)  NOT NULL,
                    `VISIBLE` ENUM(\'TRUE\',\'FALSE\')NOT NULL DEFAULT \'TRUE\',
                    `ENABLED` ENUM(\'TRUE\',\'FALSE\')NOT NULL DEFAULT \'TRUE\',
                    `imputDate` datetime NOT NULL,
                    `STATUS` ENUM(\''.implode('\',\'',array_keys($this->states)).'\') NOT NULL,
                     PRIMARY KEY (`phones_id`,`accound_id`)
                )ENGINE = InnoDB DEFAULT CHARSET='.\autosystem\DBConn::getCHARSET().' COLLATE='.\autosystem\DBConn::getCOLLATE().';';
                $result2 = $this->DB->query($sql);
                $return2 = $result2->rowCount()==1;
            }else{
                $return2 = true;
            }
            if(!$this->existTableUser_contact_adds()){
                $sql= 'CREATE TABLE IF NOT EXISTS `'.\autosystem\DBConn::getTableNameFor('user_contact_adds').'`(
                    `id` INT(10) UNSIGNED ZEROFILL NOT NULL AUTO_INCREMENT,
                    `companyname` varchar(100) COLLATE '.\autosystem\DBConn::getCOLLATE().' DEFAULT NULL,
                     PRIMARY KEY (`id`)
                )ENGINE = InnoDB DEFAULT CHARSET='.\autosystem\DBConn::getCHARSET().' COLLATE='.\autosystem\DBConn::getCOLLATE().';';
                $result3 = $this->DB->query($sql);
                $return3 = $result3->rowCount()==1;
            }else{
                $return3 = true;
            }
            if(!$this->existTabletelephones_landcode()){
                $sql= 'CREATE TABLE IF NOT EXISTS `'.\autosystem\DBConn::getTableNameFor('telephones_landcode').'`(
                    `id` INT(10) UNSIGNED ZEROFILL NOT NULL AUTO_INCREMENT,
                    `telefooncode` varchar(100) COLLATE '.\autosystem\DBConn::getCOLLATE().' NOT NULL,
                    `landcode` varchar(5) COLLATE '.\autosystem\DBConn::getCOLLATE().' NOT NULL,
                     PRIMARY KEY (`id`)
                )ENGINE = InnoDB DEFAULT CHARSET='.\autosystem\DBConn::getCHARSET().' COLLATE='.\autosystem\DBConn::getCOLLATE().';';
                $result4 = $this->DB->query($sql);
                $return4 = $result4->rowCount()==1;
            }else{
                $return4 = true;
            }
            return $return2 && $return1&& $return3 &&$return4 ;
        }
        public function isInstalled(){
            return $this->isinstalled||$this->existTableUser_contact_Phone()&& $this->existTableUser__phones() && $this->existTableUser_contact_adds() && $this->isinstalled=true;
        }
        public function existTableUser_contact_Phone(){      return $this->existTableLabel('user_contact_telephone');   }
        public function existTableUser__phones(){            return $this->existTableLabel('user_telephones');          }
        public function existTableUser_contact_adds(){       return $this->existTableLabel('user_contact_adds');        }
        public function existTabletelephones_landcode(){     return $this->existTableLabel('telephones_landcode');      }
        public function getModelInfo(){        return 'Users.ContactInfo.model.info';   }
        public function __upgrade($version){   return true;        }
        public function getVersionNr(){        return (float)4.7;    }
        public function getRuleDefault(){      return 'USER';        }
        public function save_telPhone($phonenr,$phonetype,$accound_id){
            $ip = parent::getIp();
            $key ='INSERT INTO `'.\autosystem\DBConn::getTableNameFor('user_contact_telephone').
                            '`(`id`,`phonenr`,`phonetype`,`ip`) '.
                       'VALUES(NULL,:phonenr,:phonetype,:ip) RETURNING `phones_id`';//
            $value = [':phonenr'=>$phonenr,':phonetype'=>$phonetype,':ip'=>$ip];
            $sql[$key]= $value;
            $key ='INSERT INTO `'.\autosystem\DBConn::getTableNameFor('user_telephones').
                            '`(`phones_id`,`accound_id`,`VISIBLE`,`STATUS`,`imputDate`) '.
                       'VALUES(:phones_id ,:accound_id,TRUE,\'ACTIVE\',:imputDate)';
            $value = [':phones_id'=>'',':accound_id'=>$accound_id,':imputDate'=>$this->nowDateTime()];
            $sql[$key]= $value;
            return $this->transaction($sql);
        }
        public function delete_telPhone($id,$accound_id){
            $ut = \autosystem\DBConn::getTableNameFor('user_contact_telephone');
            $key ='SELECT phones_id AS id FROM `'.\autosystem\DBConn::getTableNameFor('user_telephones').'` WHERE `phones_id`=:phones_id AND `accound_id`=:accound_id RETURNING `id` ';
            $value = [':phones_id'=>$id,':accound_id'=>$accound_id];
            $sql[$key]= $value;
            $key ='DELETE FROM `'.\autosystem\DBConn::getTableNameFor('user_contact_telephone').'`WHERE `id`=:id ';//
            $value = [':id'=>''];
            $sql[$key]= $value;
            return $this->transaction($sql);
        }
        public function Update_telPhone($id,$phonenr,$phonetype,$accound_id){
            $ip = parent::getIp();
            $ut = \autosystem\DBConn::getTableNameFor('user_contact_telephone');
            $key ='UPDATE `'.$ut.'` left `join user_telephones` `phones_id`'.
                  'SET `phonenr`=:phonenr ,`phonetype`=:phonetype,`imputDate`=:imputDate ,`ip`=:ip where id=:id';
            $value = [':phonenr'=>$phonenr,':phonetype'=>$phonetype,':imputDate'=>$this->nowDateTime(),':ip'=>$ip];
            $sql[$key]= $value;
            return $this->transaction($sql);
        }
    }
?>