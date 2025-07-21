<?php
/**
* @author Lieven Roegiers
* @copyright 
* @CMS autosite
*/
    namespace users;
	class ModUsers_model extends \autosystem\Model{
	    private $isinstalled =false;
	    public function __construct() {
            parent::__construct();
            $this->isinstalled = self::isInstalled();
	    }
        public function __install(){
           if(!$this->existTableAccound_modsettings()){
                $dbSql = 'CREATE TABLE IF NOT EXISTS `'.\autosystem\DBConn::getTableNameFor('accound_modsettings').'`(
                        `accound_id` INT(20),
                        `VISIBLE` ENUM(\'TRUE\',\'FALSE\')NOT NULL DEFAULT \'TRUE\',
                        `FOLLOW` ENUM(\'TRUE\',\'FALSE\')NOT NULL DEFAULT \'FALSE\',
                        `CHANGELOOKUP` ENUM(\'TRUE\',\'FALSE\')NOT NULL DEFAULT \'FALSE\',
                        `LOG` ENUM(\'TRUE\',\'FALSE\')NOT NULL DEFAULT \'FALSE\',
                        `inf` varchar(230) COLLATE '.\autosystem\DBConn::getCOLLATE().' NOT NULL,
                        `risklevel` INT(4)ZEROFILL DEFAULT 0,
                         PRIMARY KEY (`accound_id`)
                    )ENGINE = InnoDB DEFAULT CHARSET='.\autosystem\DBConn::getCHARSET().' COLLATE='.\autosystem\DBConn::getCOLLATE().';';
                $result2 = $this->DB->query($dbSql);
                $return2 = $result2->rowCount()==1;
            }else{
                $return2 = true;
            }
            return $return2;
        }
        public function isInstalled(){
            return $this->isinstalled||$this->existTableAccound()&&$this->existTableAccound_modsettings()&& $this->isinstalled=true;
        }
        public function __upgrade($version){    return true; }
        private function existTableAccound(){   return $this->existTableLabel('accound');       }
        private function existTableAccound_modsettings(){ return $this->existTableLabel('accound_modsettings');        }        
        public function save(){           if(!$this->isinstalled){return false;}        }
        public function getModelInfo(){         return 'Users.modusers.model.info';       }
        public function getVersionNr(){         return (float)5.4;    }
        public function getRuleDefault(){       return 'KING';        }
        public function GET_AreportsSummary($location,$templatename,&$view,$accound_id='',$enabled = null,$activated = null,$date=null){ 
            if(!$this->existTableLabel('support_tickets')){return;}
            $a = \autosystem\DBConn::getTableNameFor('support_tickets');
            $c = \autosystem\DBConn::getTableNameFor('support_category');
            $sql='SELECT `'.$c.'`.*,`'.$a.'`.* FROM `'.$a.'`'.' LEFT JOIN `'.$c.'` ON `'.$a.'`.`category_id`= `'.$c.'`.`id`'
                //.' GROUP BY `'.$a.'`.`id` ' 
				.'ORDER BY `'.$a.'`.`inputDate` ';
            $array = [];
            //print $sql.'------------------';
            parent::GET_Summary($location,$templatename,$view,$sql,$array,true,$accound_id);
        }
        //his->_model->GET_AProsessions($this->_loc,'session_item',$this->_view,$this->_values['page'],$this->getAccound_id(),'', true,true
        public function GET_AProsessions($location,$templatename,&$view,$pagenr,$account_id){
            $p = \autosystem\DBConn::getTableNameFor('accound_prosessions');
            $array = [];
            $a = \autosystem\DBConn::getTableNameFor('accound');
            $w='';
            $selection='FROM `'. $p.'` ';
            $len = $this->GET_COUNT('SELECT count(*) AS `count` FROM `'. $p.'`');
            $sql='SELECT * '.$selection.$w ;
            //print $sql;
            $this->paginator->setRange(100);
            $this->paginator->setlistlength($len);
            $this->paginator->setpagenr($pagenr); 
            $this->paginator->setpaginatorreload($location,$templatename);
            $exploreoverwrite['accounds']='SELECT * FROM `'.$a.'` WHERE `username`=:uname)';       
            $l = \autosystem\DBConn::getTableNameFor('adres_location');
            $exploreoverwrite['location']='SELECT * FROM `'.$l.'` WHERE `id`=:location_id';
            parent::GET_ExploreR($location,$templatename,$view,$sql,['name'=>'uname','location'=>'location_id'],$exploreoverwrite,null,true,$account_id,'');    

        }
        public function delete_AsessionById($session_id){
            $key ='DELETE FROM `'.\autosystem\DBConn::getTableNameFor('accound_prosessions').'` WHERE `session_id`=:id';
            $sql[$key]= [':id'=>$session_id];
            return $this->transaction($sql);
        }
        public function delete_AsessionByAccound($session_id){
            $key ='DELETE FROM `'.\autosystem\DBConn::getTableNameFor('accound_prosessions').'` WHERE `session_id`=:id';
            $sql[$key]= [':id'=>$session_id];
            return $this->transaction($sql);
        }        
        public function delete_AsessionByIp($ip){
            $key ='DELETE FROM `'.\autosystem\DBConn::getTableNameFor('accound_prosessions').'` WHERE `session_id`=:id';
            $sql[$key]= [':id'=>$session_id];
            return $this->transaction($sql);
        }        
                
        public function GET_ASummary($location,$templatename,&$view,$pagenr=0,$accound_id='',$param ='',$enabled = null,$activated = null,$visible = null){ 
            $len =$this->GET_COUNT($this->get_CountSQL($accound_id,$enabled,$visible,$activated));
            $sql = $this->GET_SQL($accound_id,$enabled,$activated);
            //print $sql.' GROUP BY `accound`.`id`,`user_rullset`.`rule_id` '; ;
            $this->paginator->setlistlength($len);
            $this->paginator->setpagenr($pagenr);
            $this->paginator->setpaginatorreload($location,$templatename);
            $array = [];
            $view->getpaginator($this->paginator->getdata(),'&'.$param);
			
            /*$u = \autosystem\DBConn::getTableNameFor('user');
            $s = \autosystem\DBConn::getTableNameFor('user_rullset');
			$exploreoverwrite['accound_id']='SELECT *';
			foreach($this->modeltypes AS $enum => $namekey){
               $exploreoverwrite['accound_id'] .= ', IF(`'.$s.'`.`TYPE` = \''.$enum.'\', \' checked=checked \', \'\' ) `is'.$enum.'` ';
            }
            $exploreoverwrite['accound_id'].=',`'.$u.'`.`id` AS `user_id` FROM `'. $u.'`'; 
			$exploreoverwrite['accound_id'].=' LEFT JOIN `'.$s.'` ON `'.$u.'`.`rule_id`=`'. $s.'`.`rule_id` '.
			'WHERE `'.$u.'`.`accound_id`=:accound_id ';*/
	        //print_r($exploreoverwrite);
			$this->debug=true;
			//see overrull by model ['accound_id'=>'modeltypes']
			print parent::GET_Explore($location,$templatename,$view,$sql,['accound_id'=>'modeltypes'],$exploreoverwrite,$array,true,$accound_id);
            $view->getpaginator($this->paginator->getdata(),'&'.$param);
        }
        protected function GET_PROSESSIONS(){
            $p = \autosystem\DBConn::getTableNameFor('accound_prosessions');
            $array = [];
            $w='';
            $selection='`session_id`,`host`,`created`,`STATUS`,`lastonline`,`accound_id`';
            $selection.=',`write_try`,`name`,`HASLOGIN`,`SUPERVISION`,`language`,`location_id`';
            $selection.=', IF(`'.$a.'`.`HASLOGIN`=\'TRUE\',\' checked=checked \',\' \') AS `islogin`,';
            $selection.=', IF(`'.$a.'`.`SUPERVISION`=\'TRUE\',\' checked=checked \',\' \') AS `undersupervision`,';
            $sql='SELECT '.$selection.' FROM `'. $p.'` '.$w ;
            //print $sql;
            $this->paginator->setRange(100);
            $this->paginator->setlistlength($len);
            $this->paginator->setpagenr($pagenr); 
            $this->paginator->setpaginatorreload($location,$templatename);
            $exploreoverwrite['accounds']='SELECT * FROM `'.$c.'`.`columns` WHERE (`table_schema`=\''.$dbname.'\' AND `TABLE_NAME`=:Name)';       
            $l = \autosystem\DBConn::getTableNameFor('adres_location');
            $exploreoverwrite['location']='SELECT * FROM `'.$l.'` WHERE `id`=:location_id';
            parent::GET_ExploreR($location,$templatename,$view,$sql,['collums'=>'Name','location'=>'location_id'],$exploreoverwrite,null,true,$account_id,'');    

        }
        protected function GET_SQL($accound_id='',$enabled = false,$activated = false,$visible=false,$idlist=false){
            $VISIBLE = ($visible=='true')?'TRUE':'FALSE';
            $u = \autosystem\DBConn::getTableNameFor('user');
            $a = \autosystem\DBConn::getTableNameFor('accound');
            $l = \autosystem\DBConn::getTableNameFor('adres_location');
            $m = \autosystem\DBConn::getTableNameFor('accound_modsettings');
            $e = \autosystem\DBConn::getTableNameFor('email');
            $sql='SELECT `'.$u.'`.`id` AS `id`'.
			',`'.$e.'`.`id` AS `emailid`, `'.$e.'`.`email` ,`'.$e.'`.`emailchecked` ,`'.$e.'`.`emailsubscribed` ,`'.$e.'`.`ENABLED` AS `emailENABLED` ,`'.$e.'`.`VISIBLE` AS `emailVISIBLE`'.
			', IF(`'.$a.'`.`ENABLED`=\'TRUE\',\' checked=checked \',\' \') AS `isaccessibility`,';
            $sql.='`accound`.`id` AS `accound_id`,`key`,`city`,`country`,`imputDate`,`lastonline`,`'.$u.'`.`TYPE`,`username`,`'.$u.'`.`name`,`'.$u.'`.`rule_id`';
            $sql.=',IF(`'.$m.'`.`VISIBLE`=\'TRUE\' OR `'.$m.'`.`VISIBLE` IS NULL,\' checked=checked \',\' \') AS `isvisible`,';
            $sql.='IF(`'.$m.'`.`FOLLOW`=\'TRUE\',\' checked=checked \',\' \') AS `isfollow`,';
            $sql.='IF(`'.$m.'`.`CHANGELOOKUP`=\'TRUE\',\' checked=checked \',\' \') AS `ischangelookup`,';
            $sql.='IF(`'.$m.'`.`LOG`=\'TRUE\',\' checked=checked \',\' \') AS `islog`';   
            $sql.=' FROM `'.$u.'` JOIN `'.$a.'` ON `'.$a.'`.`id`=`'.$u.'`.`accound_id` '.
                 'LEFT JOIN `'.$l.'` ON `'.$l.'`.`id`=`'. $u.'`.`location_id` '.
                 'LEFT JOIN `'.$m.'` ON `'.$a.'`.`id`=`'.$m.'`.`accound_id` '.
                 'LEFT JOIN `'.$e.'` ON `'.$a.'`.`email_id`= `'.$e.'`.`id` '
				;
            $where = 'WHERE';//.'(`'.$m.'`.`VISIBLE`=\''.$VISIBLE.'\' OR `'.$m.'`.`VISIBLE` IS NULL )';
            $link ='';//'AND';
            if($enabled !==false){    $where.= $link.$this->getENUM($a.'`.`ENABLED',$enabled);     $link ='AND';  }
            if($activated!==false){   $where.= $link.$this->getENUM($a.'`.`ACTIVATED',$activated); $link ='AND';  }
            $sql.= $where;
            $this->paginator->setSort(' ORDER BY `lastonline`,`imputDate` ');
            $this->paginator->setRange(45);
            return $sql;
        }
        protected function GET_accoundSQL($accound_id='',$enabled = false,$visible=false,$activate=true){
            $VISIBLE = ($visible=='true')?'TRUE':'FALSE';
            $a = \autosystem\DBConn::getTableNameFor('accound');
            $m = \autosystem\DBConn::getTableNameFor('accound_modsettings');
            $e = \autosystem\DBConn::getTableNameFor('email');
            $sql='SELECT `'.$a .'`.*,`'.$e .'`.*'.
			//',`'.$e.'`.`id` AS `emailid`, `'.$e.'`.`email` ,`'.$e.'`.`emailchecked` ,`'.$e.'`.`emailsubscribed` ,`'.$e.'`.`ENABLED` AS `emailENABLED` ,`'.$e.'`.`VISIBLE` AS `emailVISIBLE`'.
			', IF(`'.$a.'`.`ENABLED`=\'TRUE\',\' checked=checked \',\' \') AS `isaccessibility`,';
            $sql.='`accound`.`id` AS `accound_id`,`key`,`imputDate`,`username`';
            $sql.=',IF(`'.$m.'`.`VISIBLE`=\'TRUE\' OR `'.$m.'`.`VISIBLE` IS NULL,\' checked=checked \',\' \') AS `isvisible`,';
            $sql.='IF(`'.$m.'`.`FOLLOW`=\'TRUE\',\' checked=checked \',\' \') AS `isfollow`,';
            $sql.='IF(`'.$m.'`.`CHANGELOOKUP`=\'TRUE\',\' checked=checked \',\' \') AS `ischangelookup`,';
            $sql.='IF(`'.$m.'`.`LOG`=\'TRUE\',\' checked=checked \',\' \') AS `islog`';   
            $sql.=' FROM `'.$a.'` '.
                 'LEFT JOIN `'.$m.'` ON `'.$a.'`.`id`=`'.$m.'`.`accound_id` '.
                 'LEFT JOIN `'.$e.'` ON `'.$a.'`.`email_id`= `'.$e.'`.`id` '
				;
            $where = '';
            $link ='WHERE';
			if($enabled !==false){    $where.= $link.$this->getENUM($a.'`.`ENABLED',$enabled);     $link ='AND';  }            
            if($visible !==false){    //$where.= $link.'`'.$m.'`.`VISIBLE` != \'TRUE\'';  
			$where.= $link.'(`'.$m.'`.`VISIBLE`=\'TRUE\' OR `'.$m.'`.`VISIBLE` IS NULL ) ';  
			$link ='AND';  }
            $where.= $link.$this->getENUM($a.'`.`ACTIVATED',$activate);     $link ='AND';
            $sql.= $where;
            $this->paginator->setSort('ORDER BY `imputDate` DESC');
            $this->paginator->setRange(40);
            return $sql;
        }
        public function GET_NotActiefSummary($location,$templatename,&$view,$accound_id='',$enabled = false,$activated = false,$pagenr=0){ 
			$sql = $this->GET_accoundSQL($accound_id,$enabled,$activated,false);
            $array = [];
            $len =$this->GET_COUNT($this->get_CountSQL($accound_id,$enabled,false,false));
            $this->paginator->setlistlength($len);
            $this->paginator->setpagenr($pagenr);
            $this->paginator->setpaginatorreload($location,$templatename);
            parent::GET_Summary($location,$templatename,$view,$sql,$array,true,$accound_id);
            $view->getpaginator($this->paginator->getdata(),'&notActivated');
        }
        private function get_AccoundCountSQL($accound_id,$enabled,$activated,$visible){
            $VISIBLE = ($visible=='true')?'TRUE':'FALSE';
            $a = \autosystem\DBConn::getTableNameFor('accound');
            $m = \autosystem\DBConn::getTableNameFor('accound_modsettings');
            $e = \autosystem\DBConn::getTableNameFor('email');
            $sql='SELECT count(*) AS `count` FROM `'.$a.'` '.
                 'LEFT JOIN `'.$m.'` ON `'.$a.'`.`id`=`'.$m.'`.`accound_id` '.
                 'LEFT JOIN `'.$e.'` ON `'.$a.'`.`email_id`= `'.$e.'`.`id` '
				;
            $where = 'WHERE (`'.$m.'`.`VISIBLE`=\''.$VISIBLE.'\' OR `'.$m.'`.`VISIBLE` IS NULL )';
            $link ='AND';
            if($enabled !==false){    $where.= $link.$this->getENUM($a.'`.`ENABLED',$enabled);     $link ='AND';  }
            $sql.= $where;
            return $sql;
        } 
        private function get_CountSQL($accound_id,$enabled,$visible=false,$activated=true){
            $u = \autosystem\DBConn::getTableNameFor('user');
            $a = \autosystem\DBConn::getTableNameFor('accound');
            $l = \autosystem\DBConn::getTableNameFor('adres_location');
            $m = \autosystem\DBConn::getTableNameFor('accound_modsettings');
            $e = \autosystem\DBConn::getTableNameFor('email');
            $sql='SELECT count(*) AS `count` FROM `'.$a.'` LEFT JOIN `'.$u.'` ON `'.$a.'`.`id`=`'.$u.'`.`accound_id` '.
                 'LEFT JOIN `'.$l.'` ON `'.$l.'`.`id`=`'. $u.'`.`location_id` '.
                 'LEFT JOIN `'.$m.'` ON `'.$a.'`.`id`=`'.$m.'`.`accound_id` '
                 //.'LEFT JOIN `'.$e.'` ON  `'.$a.'`.`email_id`= `'.$e.'`.`id` '
				 ;
            $where = '';
            $link ='WHERE';
			if($enabled !==false){    $where.= $link.$this->getENUM($a.'`.`ENABLED',$enabled);     $link ='AND';  }            
            if($visible !==false){    //$where.= $link.'`'.$m.'`.`VISIBLE` != \'TRUE\'';  
			$where.= $link.'(`'.$m.'`.`VISIBLE`=\'TRUE\' OR `'.$m.'`.`VISIBLE` IS NULL ) ';  
			$link ='AND';  }
            $where.= $link.$this->getENUM($a.'`.`ACTIVATED',$activated);     $link ='AND';
            
            $sql.= $where;
            return $sql;
        }                
        public function addRules($user_id,$rules){
            $r = \autosystem\DBConn::getTableNameFor('user_rull');
            $u = \autosystem\DBConn::getTableNameFor('user');
            $s = \autosystem\DBConn::getTableNameFor('user_rullset');
            if(!$this->hasRulles($user_id)){
                $key =' INSERT INTO '.$r.'(`id`,`comment`) VALUES(NULL,:comment) RETURNING `rule_id` ';
                $sql[$key]= [':comment'=>''];
                $key =' UPDATE '.$u.' SET `rule_id`=:rule_id WHERE `id`=:id ';
                $sql[$key]= [':rule_id'=>'',':id'=>$user_id];
            }else{
                 $key ='SELECT `rule_id` FROM '.$u.' WHERE `id`=:id '.'RETURNING `rule_id`';
                 $sql[$key]= [':id'=>$user_id];
            }
            foreach($rules as $rull => $access){
				if($this->hasRull($user_id,$rull)){
                    if($access == 'false'){
                        $key ='DELETE FROM `'.$s.'` WHERE `rule_id`=:rule_id AND `TYPE`=:type';//
                        $sql[$key]= [':rule_id'=>'',':type'=>$rull];
                    } else{
                        $key =' delete do nothing '.$access;
                        $sql[$key]= [':rule_id'=>''];
                    }
                }else{
                    if($access == 'true'){
                        $key =' INSERT INTO '.$s.'(`rule_id`,`TYPE`) VALUES(:rule_id,\''.$rull.'\')';
                        $sql[$key]= [':rule_id'=>''];
                    }else{
                        $key =' insert do nothing ';
                        $sql[$key]= [':rule_id'=>''];
                    }
                }
            }
            return is_array($this->transaction($sql));
        }
        public function insertRules($user_id,$rules,$comment=''){
            $r = \autosystem\DBConn::getTableNameFor('user_rull');
            $u = \autosystem\DBConn::getTableNameFor('user');
            $s = \autosystem\DBConn::getTableNameFor('user_rullset');
            $key =' INSERT INTO '.$r.'(`id`,`comment`) VALUES(NULL,:comment) RETURNING `rule_id` ';
            $sql[$key]= [':comment'=>$comment];
            $key =' UPDATE '.$u.' SET `rule_id`=:rule_id WHERE `id`=:id ';
            $sql[$key]= [':rule_id'=>'',':id'=>$user_id];
            foreach($rules as $rull => $access){
				if($access == 'true'){
                     $key =' INSERT INTO '.$s.'(`rule_id`,`TYPE`) VALUES(:rule_id,\''.$rull.'\')';
                     $sql[$key]= [':rule_id'=>''];
                }else{
                     $key =' insert do nothing ';
                     $sql[$key]= [':rule_id'=>''];
                }
            }
            return is_array($this->transaction($sql,true));
        }
        private function hasRulles($user_id){
            $sql ='SELECT `accound_id`,`rule_id` FROM '.\autosystem\DBConn::getTableNameFor('user').' WHERE `id`=:id ';
            $param = [':id'=>$user_id];
            $array = $this->GET_Stmt($sql,$param);
            print $sql;
            return is_array($array)&& array_key_exists('rule_id',$array)&& intval($array['rule_id'])>0;
        }
        private function hasRull($user_id,$TYPE){
            $s = \autosystem\DBConn::getTableNameFor('user_rullset');
            $u = \autosystem\DBConn::getTableNameFor('user');
            $sql = 'SELECT `'.$u.'`.`rule_id` ,`user_rullset`.`TYPE` FROM `'.$u.'` LEFT JOIN `'.$s.'` ON `'.$u.'`.`rule_id` = `'.$s.'`.`rule_id`';
            $sql .=' WHERE `id`=:id AND `user_rullset`.`TYPE`=:type ';
            $param = [':id'=>$user_id , ':type'=>$TYPE];
            $array = $this->GET_Stmt($sql,$param);
            //print_r($array);
            return  is_array($array)&& array_key_exists('TYPE',$array)&& $array['TYPE']==$TYPE;
        }
        public function userEnabled($user_id,$bool){
            $ENABLED = ($bool=='true')?'TRUE':'FALSE';
            $a = \autosystem\DBConn::getTableNameFor('accound');
            $u = \autosystem\DBConn::getTableNameFor('user');
            $key ='UPDATE `'.$a.'` left join `'.$u.'` ON `'.$u.'`.`accound_id`=`'.$a.'`.`id` SET ENABLED=:ENABLED WHERE `'.$u.'`.`id`=:id';
            $sql[$key]= [':ENABLED'=>$ENABLED,':id'=>$user_id];
            return is_array($this->transaction($sql));
        }
        public function accoundEnabled($activatekey,$bool){
            $ENABLED = ($bool=='true')?'TRUE':'FALSE';
            $a = \autosystem\DBConn::getTableNameFor('accound');
            $key ='UPDATE `'.$a.'` SET ENABLED=:ENABLED WHERE `key`=:key';
            $sql[$key]= [':ENABLED'=>$ENABLED,':key'=>$activatekey];
            return is_array($this->transaction($sql));
        }
        public function accoundEnabledId($id,$bool){
            $ENABLED = ($bool=='true')?'TRUE':'FALSE';
            $a = \autosystem\DBConn::getTableNameFor('accound');
            $key ='UPDATE `'.$a.'` SET ENABLED=:ENABLED WHERE `id`=:id';
            $sql[$key]= [':ENABLED'=>$ENABLED,':id'=>$id];
            //print \autosystem\DEBUG::printTransaction($sql);
            return is_array($this->transaction($sql));
        }
        public function getUserDetail($location,$templatename,&$view,$user_id,$accound_id){            
            $visible =true;
            $VISIBLE = ($visible=='true')?'TRUE':'FALSE';
            $u = \autosystem\DBConn::getTableNameFor('user');
            $a = \autosystem\DBConn::getTableNameFor('accound');
            $l = \autosystem\DBConn::getTableNameFor('adres_location');
            $m = \autosystem\DBConn::getTableNameFor('accound_modsettings');
            $e = \autosystem\DBConn::getTableNameFor('email');
            $sql='SELECT IF(`'.$a.'`.`ENABLED`=\'TRUE\',\' checked=checked \',\' \') AS `isaccessibility`,';
            $sql.='`'.$u.'`.*,`key`,`imputDate`,`lastonline`,`'.$u.'`.`TYPE`,`username`,`name`,`'. $u.'`.`id` AS `id` ';
            $sql.=', emailchecked, emailsubscribed, email';
            $sql.=',IF(`'.$m.'`.`VISIBLE`=\'TRUE\' OR `'.$m.'`.`VISIBLE` IS NULL,\' checked=checked \',\' \') AS `isvisible`,';
            $sql.='IF(`'.$m.'`.`FOLLOW`=\'TRUE\',\' checked=checked \',\' \') AS `isfollow`,';
            $sql.='IF(`'.$m.'`.`CHANGELOOKUP`=\'TRUE\',\' checked=checked \',\' \') AS `ischangelookup`,';
            $sql.='IF(`'.$m.'`.`LOG`=\'TRUE\',\' checked=checked \',\' \') AS `islog`';                   
            $sql.=' FROM `'.$u.'` JOIN `'.$a.'` ON `'.$a.'`.`id`=`'.$u.'`.`accound_id` '.
                 'LEFT JOIN `'.$l.'` ON `'.$l.'`.`id`=`'. $u.'`.`location_id` '.
                 'LEFT JOIN `'.$m.'` ON `'.$a.'`.`id`=`'.$m.'`.`accound_id` '.
                 'LEFT JOIN `'.$e.'` ON `'.$a.'`.`email_id`= `'.$e.'`.`id` ';
			$array=[];
            $where = 'WHERE (`'.$m.'`.`VISIBLE`=\''.$VISIBLE.'\' OR `'.$m.'`.`VISIBLE` IS NULL )';
            if(isset($user_id)){       $sql.= 'WHERE `'.$u.'`.`id` =:id';                  $array = [':id'=>$user_id];
            }else{
              if(isset($accound_id)){  $sql.= 'WHERE `'.$a.'`.`id` =:daccound_id';  $array = [':daccound_id'=>$accound_id];
              }else{                   return false;
              }  
            }
			$this->debug=true;
            $exploreoverwrite=[];
			print parent::GET_Explore($location,$templatename,$view,$sql,['accound_id'=>'modeltypes'],$exploreoverwrite,$array,true,$accound_id);
        }
        public function FIND($string,$type=''){
            switch($type){
            	case 'accound':           return $this->FIND_Accound($string);                 
            	   break;
            	default :                 $this->FIND_ALL($string); 
            }
        }
        private function FIND_ALL($string){
            return 0;
        }
        private function FIND_Accound($string){
            $u = \autosystem\DBConn::getTableNameFor('user');
            $a = \autosystem\DBConn::getTableNameFor('accound');
            $sql ='SELECT `'.$a.'`.`id` FROM `'.$a.'` LEFT JOIN `'.$u.'` ON `'.$u.'`.`accound_id`=`'.$a.'`.`id` WHERE `username`=:string';//
            $param = [':string'=>$string];
            $array = $this->GET_Stmt($sql,$param,1);
            return (is_array($array)&& array_key_exists('id',$array))?$array['id']:false;
        }
        public function GET_Accound($location,$templatename,&$view,$accound_id){
            $a = \autosystem\DBConn::getTableNameFor('accound');
            $e = \autosystem\DBConn::getTableNameFor('email');
            $sql='SELECT IF(`'.$a.'`.`ENABLED`=\'TRUE\',\' checked=checked \',\' \') AS `isaccessibility`,';
            $sql.=' `'.$a.'`.`id` AS `accound_id`,`key`,`imputDate`,`username`,`ACTIVATED`,`updateDate` ,`email_id` ,`'.$e.'`.*';
            $sql.=' FROM `'.$a.'` LEFT JOIN `'.$e.'` ON `'.$a.'`.`email_id`= `'.$e.'`.`id`'.
			' WHERE `'.$a.'`.`id` =:accound_id';  
            $array = [':accound_id'=>$accound_id];
            print parent::AGET($location,$templatename,$view,$sql,$array);
        }
        public function GET_LoginSummary($location,$templatename,&$view,$user_id,$accound_id,$pagenr = 0){
            $u = \autosystem\DBConn::getTableNameFor('user');
            $a = \autosystem\DBConn::getTableNameFor('accound');
            $lo = \autosystem\DBConn::getTableNameFor('user_logins');
            $array = [];
            $w='';
            $selection='FROM `'.$lo.'` LEFT JOIN `'.$a.'` ON `'.$lo.'`.`accound_id` = `'.$a.'`.`id` LEFT JOIN `'.$u.'` ON `'.$lo.'`.`accound_id` = `'.$u.'`.`accound_id`';
            if(isset($user_id)){        $w.= 'WHERE `'.$u.'`.`id` =:id';                  $array = [':id'=>$user_id];
            }else{
              if(isset($accound_id)){   $w.= 'WHERE `'.$u.'`.`accound_id` =:accound_id';  $array = [':accound_id'=>$accound_id];
              }
            }
            $len =$this->GET_COUNT('SELECT count(*) AS `count` '.$selection);
            $sql='SELECT `'.$a.'`.*,`'.$lo.'`.* , YEAR(`'.$lo.'`.`time`) AS `activitiyear` '.$selection.$w ;
            //print $sql;
            $this->paginator->setRange(100);
            $this->paginator->setlistlength($len);
            $this->paginator->setpagenr($pagenr); 
            $this->paginator->setpaginatorreload($location,$templatename);
            //print $this->paginator->getsql();
            $view->getpaginator($this->paginator->getdata(),'&lastactions');
            parent::GET_Summary($location,$templatename,$view,$sql,$array);
            $view->getpaginator($this->paginator->getdata(),'&lastactions');
        }
        public function delete_login($id){
            $key ='DELETE FROM `'.\autosystem\DBConn::getTableNameFor('user_logins').'` WHERE `id`=:id';
            $sql[$key]= [':id'=>$id];
            return $this->transaction($sql);
        }
        public function delete_loginByIp($ip){
            $key ='DELETE FROM `'.\autosystem\DBConn::getTableNameFor('user_logins').'` WHERE `ip`=:ip';
            $sql[$key]= [':ip'=>$ip];
            return $this->transaction($sql);
        }
        public function delete_loginByYear($year){
            $key ='DELETE FROM `'.\autosystem\DBConn::getTableNameFor('user_logins').'` WHERE YEAR(`time`)=:time';
            $sql[$key]= [':time'=>$year];
            return $this->transaction($sql);
        }
        public function mod_accoundvisible($accound_id,$bool){
            $VISIBLE = ($bool=='true')?'TRUE':'FALSE';
            if(!$this->existmodsettings($accound_id)){$this->save_modsettings($accound_id,true,false,false,false,'--',1);}
            $a = \autosystem\DBConn::getTableNameFor('accound_modsettings');
            $key ='UPDATE `'.$a.'` SET VISIBLE=:VISIBLE WHERE `accound_id`=:accound_id ';
            $sql[$key]= [':VISIBLE'=>$VISIBLE,':accound_id'=>$accound_id];
            return is_array($this->transaction($sql,true));
        }
        public function  mod_accoundfollow($accound_id,$bool){
            $FOLLOW = ($bool=='true')?'TRUE':'FALSE';
            $a = \autosystem\DBConn::getTableNameFor('accound_modsettings');
            $key ='UPDATE `'.$a.'` SET FOLLOW=:FOLLOW WHERE `accound_id`=:accound_id ';
            $sql[$key]= [':FOLLOW'=>$FOLLOW,':accound_id'=>$accound_id];
            return is_array($this->transaction($sql));
        }
        public function mod_accoundseeonchange($accound_id,$bool){
            $CHANGELOOKUP = ($bool=='true')?'TRUE':'FALSE';
            if(!$this->existmodsettings($accound_id)){$this->save_modsettings($accound_id,false,false,true,false,'',0);}
            $a = \autosystem\DBConn::getTableNameFor('accound_modsettings');
            $key ='UPDATE `'.$a.'` SET CHANGELOOKUP=:CHANGELOOKUP WHERE `accound_id`=:accound_id ';
            $sql[$key]= [':CHANGELOOKUP'=>$CHANGELOOKUP,':accound_id'=>$accound_id];
            return is_array($this->transaction($sql));
        }
        public function  mod_accoundlog($accound_id,$bool){
            $LOG = ($bool=='true')?'TRUE':'FALSE';
            if(!$this->existmodsettings($accound_id)){$this->save_modsettings($accound_id,false,true,true,true,'',10);}
            $a = \autosystem\DBConn::getTableNameFor('accound_modsettings');
            $key ='UPDATE `'.$a.'` SET LOG=:LOG WHERE `accound_id`=:accound_id ';
            $sql[$key]= [':LOG'=>$LOG,':accound_id'=>$accound_id];
            return is_array($this->transaction($sql));
        }
        public function existmodsettings($accound_id){
            $a = \autosystem\DBConn::getTableNameFor('accound_modsettings');
            $sql ='SELECT * FROM '.$a.' WHERE `accound_id`=:accound_id ';
            $param= [':accound_id'=>$accound_id];
            return $this->isQuery($sql,$param);
        }
        public function save_modsettings($accound_id,$VISIBLE=false,$FOLLOW=false,$CHANGELOOKUP=false,$LOG=false,$inf='',$risklevel=0){
            $VISIBLE = ($VISIBLE=='true')?'TRUE':'FALSE';
            $FOLLOW = ($FOLLOW=='true')?'TRUE':'FALSE';
            $CHANGELOOKUP = ($CHANGELOOKUP=='true')?'TRUE':'FALSE';
            $LOG = ($LOG=='true')?'TRUE':'FALSE';
            $a = \autosystem\DBConn::getTableNameFor('accound_modsettings');
            $key =' INSERT INTO '.$a.'(`accound_id`, `VISIBLE`, `FOLLOW`, `CHANGELOOKUP`, `LOG`, `inf`, `risklevel`)'.
            ' VALUES(:accound_id,:VISIBLE,:FOLLOW,:CHANGELOOKUP,:LOG,:inf,:risklevel)';
            $sql[$key]= [':accound_id'=>$accound_id,':VISIBLE'=>$VISIBLE,':FOLLOW'=>$FOLLOW,':CHANGELOOKUP'=>$CHANGELOOKUP,':LOG'=>$LOG,':inf'=>$inf,':risklevel'=>$risklevel];
            return is_array($this->transaction($sql));
        }
        private function getAutocompletetypes(){
            return ['users','accound','country','postcode','town','city'];
        }
        private function IsInautocompletelist($wanted){
            return in_array($wanted ,$this->getAutocompletetypes());
        }
        public function AcompleteSearch($startwith,$type=''){
            if($this->IsInautocompletelist($type)){
                $return['Acompletelist']=$this->GET_full_list_accound($startwith);
            }else{
                //$return['Acompletelist']=$this->GET_full_list_accound($startwith);
            }
            return $return;
        }
        private function GET_full_list_users($startwith){
            //print 'user';
            $c = \autosystem\DBConn::getTableNameFor('user');          
            $sql='SELECT `id` AS \'key\' ,`name` AS \'value\' FROM `'.$c.'` WHERE `name` LIKE \''.$startwith.'%\' ' ;
            //print $sql;
            return parent::GET_Array($sql);
        }
        private function GET_full_list_accound($startwith){
            $c = \autosystem\DBConn::getTableNameFor('accound');            
            $sql='SELECT `id` AS \'key\' ,`username` AS \'value\' FROM `'.$c.'` WHERE `username` LIKE \''.$startwith.'%\' ' ;
            return parent::GET_Array($sql);
        }
        private function GETresult(){
            $a = \autosystem\DBConn::getTableNameFor('accound');
            $sql='SELECT IF(`'.$a.'`.`ENABLED`=\'TRUE\',\' checked=checked \',\' \') AS `isaccessibility`,';
            $sql.=' id AS`accound_id`,`key`,`imputDate`,`username`';
            $sql.=' FROM `'.$a.'` WHERE `'.$a.'`.`id` =:accound_id';  
            $array = [':accound_id'=>$accound_id];
            parent::GET($location,$templatename,$view,$sql,$array);
        }
    }
?>