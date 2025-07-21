<?php
/**
* @author Lieven Roegiers
* @copyright 
* @CMS autosite
*/
    namespace users;
	class UserStats_model extends \users\Profile_model{
	    protected $isinstalled =false;
	    public function __construct(){
            parent::__construct();
            $this->isinstalled = self::isInstalled();
	    }
        public function __install(){
            if(parent::__install()&&!$this->existTableUser_stats()){
                $sql = 'CREATE TABLE IF NOT EXISTS `'.\autosystem\DBConn::getTableNameFor('user_stats').'`(
                        `id`    INT(15) NOT NULL AUTO_INCREMENT,
                        `date`  date NOT NULL,
                        `users`  INT(7) DEFAULT 0,
                        `visitors`  INT(7) DEFAULT 0,
                        `crimes`  INT(7) DEFAULT 0,
                        `validlogins`  INT(7) DEFAULT 0,
                        `invalidlogins`  INT(7) DEFAULT 0,
                        `pages`  INT(7) DEFAULT 0,
                         PRIMARY KEY (`id`)
                    )ENGINE = InnoDB DEFAULT CHARSET='.\autosystem\DBConn::getCHARSET().' COLLATE='.\autosystem\DBConn::getCOLLATE().';';
                $result1 = $this->DB->query($sql);
                $return1 = $result1->rowCount()==1;
            }else{
                $return1 = true;
            }
            return $return1;
        }
        public function upgrade01(){
            //ALTER TABLE `user_stats` ADD `validlogins` INT( 7 ) NOT NULL DEFAULT '0';
        }
        public function __upgrade($version){      return true;       }
        public function isInstalled(){            return $this->isinstalled||$this->existTableUser_stats();  }
        private function existTableUser_stats(){  return $this->existTableLabel('user_stats');    }
        public function save(){          if(!$this->isinstalled){return false;}
        }
        public function getModelInfo(){     return 'Users.stads.model.info'; }
        public function getVersionNr(){     return (float)5.4;        }
        public function getRuleDefault(){   return 'ADMINISTRATION';  }
        public function GET_logins($year,$month =0,$accoundid =0){
           if($month>0){     return $this->GET_stat('users',$year,$month);
           }else{            return $this->GET_yearstat('users',$year);
           }
        }
        public function GET_crimes($year,$month =0,$accoundid =0){
           if($month>0){     return $this->GET_stat('crimes',$year,$month);
           }else{            return $this->GET_yearstat('crimes',$year);
           }
        }
        public function GET_visitors($year,$month =0,$accoundid =0){
           if($month>0){     return $this->GET_stat('visitors',$year,$month);
           }else{            return $this->GET_yearstat('visitors',$year);
           }
        }
        public function GET_pages($year,$month =0,$accoundid =0){
           if($month>0){     return $this->GET_stat('pages',$year,$month);
           }else{            return $this->GET_yearstat('pages',$year);
           }
        }
        public function GET_invalidlogins($year,$month =0,$accoundid =0){
           if($month>0){     return $this->GET_stat('invalidlogins',$year,$month);
           }else{            return $this->GET_yearstat('invalidlogins',$year);
           }
        }
        private function GET_stat($F,$Y,$M){
           $ul = \autosystem\DBConn::getTableNameFor('user_stats');
           $sql='SELECT `'.$F.'` AS `value`, DAY(`date`) AS `key` FROM `'.$ul.'` WHERE YEAR(`date`)=\''.$Y.'\' AND MONTH(`date`)=\''.$M.'\' ';
           //print $sql;
           return parent::GET_Array($sql);
        }
        private function GET_Browserstat($F,$Y,$M){
           $ul = \autosystem\DBConn::getTableNameFor('user_logins');
           $sql='SELECT `'.$F.'` AS `value`, DAY(`time`) AS `key` FROM `'.$ul.'` WHERE YEAR(`time`)=\''.$Y.'\' AND MONTH(`time`)=\''.$M.'\' ';
           return parent::GET_Array($sql);
        }
        public function GET_yearstat($F,$Y){
           $ul = \autosystem\DBConn::getTableNameFor('user_stats');
           $sql='SELECT sum(`'.$F.'`) AS `value`, MONTH(`date`) AS `key` FROM `'.$ul.'` WHERE YEAR(`date`)=\''.$Y.'\' GROUP BY MONTH(`date`)';
           //print $sql;
           return parent::GET_Array($sql);
        }
        public function GET_loginamount($year,$month,$accoundid =0){
           $ul = \autosystem\DBConn::getTableNameFor('user_logins');
           $sql='SELECT count(`id`) AS `value`, DAY(`time`) AS `key` FROM `'.$ul.
           '` WHERE YEAR(`time`)=\''.$year.'\' AND MONTH(`time`)=\''.$month.'\' ';
            $where = '';
            $link ='AND';
            if(!empty($accound_id)){
                $where.= $link.' `accound_id`=\''.$accoundid.'\' ';
                $link =' && ';
            }
           $sql.= $where . 'GROUP BY `key`';
           return parent::GET_Array($sql);
        }
        public function updatemonth($year,$month){
           $ul = \autosystem\DBConn::getTableNameFor('user_logins');
           $us = \autosystem\DBConn::getTableNameFor('user_stats');
           $k='REPLACE INTO `'.$us.'` (`date`,`users`) '.
           'SELECT DATE(`time`) AS `dateb`,count(`user_logins`.`id`) AS `usersb` '.
           'FROM `'.$ul.'` LEFT JOIN `user_stats` ON `user_stats`. `date`= DATE(`time`)'.
           ' WHERE YEAR(`time`)=:year AND MONTH(`time`)=:month  AND  user_stats.`date`is NULL ';
            $k.= 'GROUP BY DAY(`time`)';
            $sql[$k]= [ ':year'=>$year ,':month'=>$month];
            //DEBUG::printTransaction($sql);
            return $this->transaction($sql);
        }
        public function updateday($year,$month,$day){
           $ul = \autosystem\DBConn::getTableNameFor('user_logins');
           $us = \autosystem\DBConn::getTableNameFor('user_stats');
           $sql='REPLACE INTO `'.$us.'` (`date`,`users`) '.
           '(SELECT COUNT(`id`),DATE(`time`) AS `date` FROM `'.$ul.
           '` WHERE YEAR(`time`)=\''.$year.'\' AND MONTH(`time`)=\''.$month.'\' AND DAY(`time`)=\''.$day.'\' ';
           $sql.= ') WHERE '.$us.'.`date`';
           print $sql .'<br><br>';
        }
    }
?>