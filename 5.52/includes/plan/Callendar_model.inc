<?php
    namespace plan;
	class Callendar_model extends \autosystem\Model{
	    protected $isinstalled = false;
        protected $monthval = ['01'=>'Jan','02'=>'Feb','03'=>'Mar','04'=>'Apr','05'=>'May','06'=>'Jun','07'=>'Jul','08'=>'Aug','09'=>'Sep','10'=>'Oct','11'=>'Nov','12'=>'Dec'];
	    public function __install(){
            if(!$this->existTableCallendar_category()){
                $sql = 'CREATE TABLE IF NOT EXISTS `'.\autosystem\DBConn::getTableNameFor('callendar_category').'`(
                        `id`    INT(5) NOT NULL AUTO_INCREMENT,
                        `label`  varchar(230) COLLATE '.\autosystem\DBConn::getCOLLATE()." NOT NULL,
                        `ENABLED` ENUM('TRUE','FALSE')NOT NULL DEFAULT 'TRUE',
                        `TYPE` ENUM('".implode('\',\'',array_keys($this->modeltypes)).'\') default \'NULL\',
                    PRIMARY KEY (`id`)
                )ENGINE = InnoDB DEFAULT CHARSET='.\autosystem\DBConn::getCHARSET().' COLLATE='.\autosystem\DBConn::getCOLLATE().' ;';
                $result1 = $this->DB->query($sql);
                $return1 = $result1->rowCount()==1;
            }else{
                $return1 = true;
            }
            if(!$this->existTableCallendar()){
                $dbSql = 'CREATE TABLE IF NOT EXISTS `'.\autosystem\DBConn::getTableNameFor('callendar').'`(
                         `id` int(12) NOT NULL AUTO_INCREMENT,
                         `namekey` varchar(50) COLLATE '.\autosystem\DBConn::getCOLLATE().' NOT NULL,
                         `from`  DATE,
                         `to`  DATE,
                         `accound_owner_id`  INT(20) ZEROFILL NOT NULL,
                         `ENABLED` ENUM(\'TRUE\',\'FALSE\')NOT NULL DEFAULT \'TRUE\',
                         `ip`  varchar(50) COLLATE '.\autosystem\DBConn::getCOLLATE()." NOT NULL,
                         `TYPE` ENUM('".implode('\',\'',array_keys($this->modeltypes))."') default 'NULL',
                         `priority`    INT(4) DEFAULT 0,
                         `category_id` INT(5)  NOT NULL,
                        PRIMARY KEY  (`id`)
                        )ENGINE = InnoDB DEFAULT CHARSET=".\autosystem\DBConn::getCHARSET().' COLLATE='.\autosystem\DBConn::getCOLLATE().' ;';
                $result2 = $this->DB->query($dbSql);
                $return2 = $result2->rowCount()==1;
                if(!$resultx =$this->DB->query('ALTER TABLE `'.\autosystem\DBConn::getTableNameFor('callendar')."` COMMENT='this is not callendars, it is callendar_items'")){
                   //print_r($resultx);
                }
            }else{
                $return2 = true;
            }
            if(!$this->existTableCallendar_bind()){
                $dbSql = 'CREATE TABLE IF NOT EXISTS `'.\autosystem\DBConn::getTableNameFor('callendar_bind')."`(
                         `callendar_id` INT(12),
                         `event_id` INT(12) DEFAULT NULL,
                         `task_id` INT(12) DEFAULT NULL,
                         `project_id` INT(20) DEFAULT NULL,
                         `accound_id`  INT(20)  NOT NULL,
                         `accound_access` ENUM('TRUE','FALSE')NOT NULL DEFAULT 'TRUE',
                         `dest_id`  INT(50)  NOT NULL,
                         `EVENT` ENUM('TRUE','FALSE')NOT NULL DEFAULT 'FALSE',
                         `TASK` ENUM('TRUE','FALSE')NOT NULL DEFAULT 'FALSE',
                         `HAVE_KEYS` ENUM('TRUE','FALSE')NOT NULL DEFAULT 'FALSE',
                         `time` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
                         `ip`  varchar(50) COLLATE ".\autosystem\DBConn::getCOLLATE().' NOT NULL,
                        PRIMARY KEY  (`callendar_id`,`accound_id`)
                        )ENGINE = InnoDB DEFAULT CHARSET='.\autosystem\DBConn::getCHARSET().' COLLATE='.\autosystem\DBConn::getCOLLATE().' ;';
                $result5 = $this->DB->query($dbSql);
                $return5 = $result5->rowCount()==1;
            }else{
                $return5 = true;
            }
            if(!$this->existTableCallendar_location()){
                $sql = 'CREATE TABLE IF NOT EXISTS `'.\autosystem\DBConn::getTableNameFor('callendar_location').'`(
                        `callendar_id` INT(12),
                        `adresdetail_id`   INT(10),
                        `location_id`    INT(9) NOT NULL,
                        `VISIBLE` ENUM(\'TRUE\',\'FALSE\')NOT NULL DEFAULT \'TRUE\',
                         PRIMARY KEY (`callendar_id`)
                )ENGINE = InnoDB DEFAULT CHARSET='.\autosystem\DBConn::getCHARSET().' COLLATE='.\autosystem\DBConn::getCOLLATE().' ;';
                $result6 = $this->DB->query($sql);
                $return6 = $result6->rowCount()==1;
            }else{
                $return6 = true;
            }
            if(!$this->existTableCallendar_location()){
                $sql = 'CREATE TABLE IF NOT EXISTS `'.\autosystem\DBConn::getTableNameFor('callendar_contact').'`(
                        `contact_accound_id`  INT(20)UNSIGNED ZEROFILL,
                        `contact_id`  INT(20) default 0,
                        `adresdetail_id`   INT(10),
                        `location_id`    INT(9) NOT NULL,
                        `PUBLIC` ENUM(\'TRUE\',\'FALSE\')NOT NULL DEFAULT \'TRUE\',
                         PRIMARY KEY (`callendar_id`)
                )ENGINE = InnoDB DEFAULT CHARSET='.\autosystem\DBConn::getCHARSET().' COLLATE='.\autosystem\DBConn::getCOLLATE().' ;';
                $result6 = $this->DB->query($sql);
                $return6 = $result6->rowCount()==1;
            }else{
                $return6 = true;
            }
            return $return1 && $return2 &&   $return5 &&   $return6;
            
        }
        public function __upgrade($version){         return true;  }
        public function isInstalled(){
            return $this->isinstalled|| $this->existTableCallendar_category()&& $this->existTableCallendar()&& $this->existTableCallendar_bind() && $this->isinstalled=true;
        }
        public function existTableCallendar_category(){   return $this->existTableLabel('callendar_category');    }
        public function existTableCallendar(){            return $this->existTableLabel('callendar');        }
        public function existTableCallendar_bind(){       return $this->existTableLabel('callendar_bind');        }
        public function existTableCallendar_location(){   return $this->existTableLabel('callendar_location');    }
        public function GET_Items($year,$month,$accoundid){
            $ids = $this->GET_ids($year,$month,$accoundid);
            if(isset($ids)){
                $val = 'WHERE id=\''.implode('\' OR id=\'',array_keys($ids)).'\'';
                $c = \autosystem\DBConn::getTableNameFor('callendar');
                $sql='SELECT `'.$c.'`.*, DAY(`from`) AS from_day ,DAY(`to`) AS to_day FROM `'.$c.'` '.$val;
                $data = $this->GET_Stmt_key($sql,[],0,'id');
                $return = [];
                $translatekeys = [];
                foreach($ids as $key=>$val){
                    $translatekeys[]= $data[$key]['namekey'];
                    for ($daynr = $data[$key]['from_day'], $max = $data[$key]['to_day']+1; $daynr < $max; $daynr++){
                        $return[$daynr][]=$data[$key];
                    }
                }
                return $return;
            }else{
                return [];
            }
            return [];
        }
        private function GET_ids($year,$month,$accoundid){
           $c = \autosystem\DBConn::getTableNameFor('callendar');
           $sql='SELECT `id` AS \'key\' , DAY(`from`) AS \'value\' FROM `'.$c.'` '.
           "WHERE (YEAR(`from`)='$year' AND MONTH(`from`)='$month')||(YEAR(`to`)='$year' AND MONTH(`to`)='$month') AND `accound_owner_id`='$accoundid' ";
           return parent::GET_Array($sql);
        }
        public function summary($location,$templatename,&$view,$year,$month,$accoundid,$rolls){
            $c = \autosystem\DBConn::getTableNameFor('callendar');
           $sql='SELECT `id` AS \'key\' , DAY(`from`) AS \'value\' FROM `'.$c.'` '.
           'WHERE (YEAR(`from`)=\''.$year.'\' AND MONTH(`from`)=\''.$month.'\' )'.
           '||(YEAR(`to`)=\''.$year.'\' AND MONTH(`to`)=\''.$month.'\') '.
           'AND (`accound_owner_id`=\''.$accoundid.'\' ';
           foreach($rolls as $roll){
                if($roll !='USER'){
                    $sql .= '|| `TYPE`=\''.$accoundid.'\'';
                }
           }
           $sql .= ')';
           $a = \autosystem\DBConn::getTableNameFor('accound');
           $exploreoverwrite['accound']='SELECT `username` FROM `'.$a.'` WHERE `id`=:accound_owner_id';
           parent::GET_ExploreR($location,$templatename,$view,$sql,['accound'=>'accound_owner_id'],$exploreoverwrite,null,true,$accoundid,'');
           /*
        $this->paginator->setlistlength($len);
        $this->paginator->setpagenr($pagenr);
        $this->paginator->setpaginatorreload($location,$templatename);
        $view->getpaginator($this->paginator->getdata());
            */
        }
        public function save($namekey,$from,$to,$accound_owner_id,$TYPE,$category_id=0){
            $ip = parent::getIp();
            $ENABLED = 'TRUE';
            $c=\autosystem\DBConn::getTableNameFor('callendar');
            $key ='INSERT INTO `'.$c.'`(`id`,`namekey`,`from`, `to`, `accound_owner_id`, `ip`, `TYPE`,`category_id`) '.
                      'VALUES(NULL,:key,:from,:to,:accound_owner_id,:ip,:TYPE,:catid) RETURNING `id`';//
            $value = [':key'=>$namekey,':from'=>$from,':to'=>$to,':accound_owner_id'=>$accound_owner_id, ':ip'=>$ip,':TYPE'=>$TYPE,':catid'=>$category_id];
            $sql[$key]= $value;
            return $this->transaction($sql);
        }
        public function delete($id,$accound_id){
            $ip = parent::getIp();
            $ENABLED = 'TRUE';
            $c=\autosystem\DBConn::getTableNameFor('callendar');
            $key ='DELETE FROM `'.$c.'` WHERE `id`=:id AND `accound_owner_id`=:accound_id';//
            $value = [':id'=>$id,':accound_id'=>$accound_id];
            $sql[$key]= $value;
            return $this->transaction($sql);
        }
        public function Update($id,$namekey,$from,$to,$accound_owner_id,$TYPE,$category_id=0){
            $ENABLED = 'TRUE';
            $c=\autosystem\DBConn::getTableNameFor('callendar');
            $key ='UPDATE `'.$c.'` SET `namekey`=:key,`from`=:from, `to`=:to, `accound_owner_id`=:accound_owner_id,`ENABLED`=:ENABLED, `ip`=:ip, `TYPE`=:TYPE, `category_id`=:catid  where id=:id';
            $value = [':id'=>$id,':key'=>$namedkey,':from'=>$from,':to'=>$to,':accound_owner_id'=>$accound_owner_id,':ENABLED'=>$ENABLED,':ip'=>$ip,':TYPE'=>$TYPE,':catid'=>$category_id ];
            $sql[$key]= $value;
            return $this->transaction($sql);
        }
        public function getModelInfo(){      return 'callendar.model.info';    }
        public function getVersionNr(){      return (float)5.4;    }
        public function getRuleDefault(){    return 'USER';        }
    }
?>