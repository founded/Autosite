<?php
    namespace adres;
	class Location_model extends \autosystem\Model{
	    public $directions = ['NULL' => 'not availeble','NORTH' => 'north','SOUD' => 'soud','MID' => 'central','EAST' => 'east-side','WEST' => 'west-side'];
        public $types = ['NULL' => 'not availeble','BILL_ADRES'=>'','DELIVERY_ADRES'=>'','MEET_ADRES'=>'','EVENT_ADRES'=>'','CONTACT_ADRES'=>''];
        public $section = ['postcode','town','city','state','province','region','locale','country'];
        protected $isinstalled;
        public $states = ['ACTIVE' => 'active','INACTIVE' => 'inactive','ERROR' => 'on error'];
        public function getModelInfo(){      return 'adres.location.model.info';    }
        public function getVersionNr(){      return (float)5.4;      }
        public function GET_location($location,$name,$view,$filter,$id){        }
        public function __install(){
            if(!$this->existTableLocation()){
                $sql = 'CREATE TABLE IF NOT EXISTS `'.\autosystem\DBConn::getTableNameFor('adres_location').'`(
                        `id`    INT(9) NOT NULL AUTO_INCREMENT,
                        `postcode` varchar(10) COLLATE '.\autosystem\DBConn::getCOLLATE().' DEFAULT NULL,
                        `town` varchar(100) COLLATE '.\autosystem\DBConn::getCOLLATE().' DEFAULT NULL,
                        `city` varchar(100) COLLATE '.\autosystem\DBConn::getCOLLATE().' DEFAULT NULL,
                        `state` varchar(50) COLLATE '.\autosystem\DBConn::getCOLLATE().' NULL,
                        `province` varchar(100) COLLATE '.\autosystem\DBConn::getCOLLATE().' DEFAULT NULL,
                        `region` varchar(100) COLLATE '.\autosystem\DBConn::getCOLLATE().' DEFAULT NULL,
                        `locale` varchar(3) COLLATE '.\autosystem\DBConn::getCOLLATE().' DEFAULT NULL,
                        `country` varchar(50) COLLATE '.\autosystem\DBConn::getCOLLATE().' DEFAULT NULL,
                        `DIRECTION` ENUM(\''.implode('\',\'',array_keys($this->directions)).'\') DEFAULT \'NULL\',
                        `gpslat` DECIMAL(10,8) COLLATE '.\autosystem\DBConn::getCOLLATE().' DEFAULT NULL,
                        `gpslng` DECIMAL(10,8) COLLATE '.\autosystem\DBConn::getCOLLATE().' DEFAULT NULL,
                        `imgspec` varchar(100) COLLATE '.\autosystem\DBConn::getCOLLATE().' DEFAULT NULL,
                        `isChecked` ENUM(\'TRUE\',\'FALSE\')NOT NULL DEFAULT \'FALSE\',
                        `isEnabled` ENUM(\'TRUE\',\'FALSE\')NOT NULL DEFAULT \'TRUE\',
                        PRIMARY KEY (`id`)
                )ENGINE = InnoDB DEFAULT CHARSET='.\autosystem\DBConn::getCHARSET().' COLLATE='.\autosystem\DBConn::getCOLLATE().' ;';
                $return1 = $this->DB->query($sql);
                $return1 = $return1->rowCount()==1;
            }else{
                $return1 = true;
            }
            if(!$this->existTableLocationMerges()){
                $sql = 'CREATE TABLE IF NOT EXISTS `'.\autosystem\DBConn::getTableNameFor('adres_location_merges').'`(
                        `location_id`    INT(9) NOT NULL AUTO_INCREMENT,
                        `location_merge_id` varchar(10) COLLATE '.\autosystem\DBConn::getCOLLATE().' DEFAULT NULL,
                        `isChecked` ENUM(\'TRUE\',\'FALSE\')NOT NULL DEFAULT \'FALSE\',
                        PRIMARY KEY (`location_id`,`location_merge_id`)
                )ENGINE = InnoDB DEFAULT CHARSET='.\autosystem\DBConn::getCHARSET().' COLLATE='.\autosystem\DBConn::getCOLLATE().' ;';
                $return1 = $this->DB->query($sql);
                $return1 = $return1->rowCount()==1;
            }else{
                $return1 = true;
            }
            if(!$this->existTableFullLocation()){
                $sql = 'CREATE TABLE IF NOT EXISTS `'.\autosystem\DBConn::getTableNameFor('adres_detail').'`(
                        `id`    INT(15) NOT NULL AUTO_INCREMENT,
                        `language` varchar(2) COLLATE '.\autosystem\DBConn::getCOLLATE().' DEFAULT NULL,
                        `adres` varchar(100) COLLATE '.\autosystem\DBConn::getCOLLATE().' DEFAULT NULL,
                        `adres_E` varchar(100) COLLATE '.\autosystem\DBConn::getCOLLATE().' DEFAULT NULL,
                        `postcode` varchar(10) COLLATE '.\autosystem\DBConn::getCOLLATE().' NOT NULL,
                        `street` varchar(100) COLLATE '.\autosystem\DBConn::getCOLLATE().' DEFAULT NULL,
                        `houseNumber` varchar(50) COLLATE '.\autosystem\DBConn::getCOLLATE().' DEFAULT NULL,
                        `bus` varchar(5) COLLATE '.\autosystem\DBConn::getCOLLATE().' DEFAULT NULL,
                        `gpslat` DECIMAL(10,8) COLLATE '.\autosystem\DBConn::getCOLLATE().' DEFAULT NULL,
                        `gpslng` DECIMAL(10,8) COLLATE '.\autosystem\DBConn::getCOLLATE().' DEFAULT NULL,
                        `isChecked` ENUM(\'TRUE\',\'FALSE\')NOT NULL DEFAULT \'FALSE\',
                        `hasTrueResive` ENUM(\'TRUE\',\'FALSE\')NOT NULL DEFAULT \'FALSE\',
                        `STATUS` ENUM(\''.implode('\',\'',array_keys($this->states)).'\')NOT NULL  default \'ACTIVE\',
                    PRIMARY KEY (`id`)
                )ENGINE = InnoDB DEFAULT CHARSET='.\autosystem\DBConn::getCHARSET().' COLLATE='.\autosystem\DBConn::getCOLLATE().' ;';
                $return2 = $this->DB->query($sql);
                $return2 = $return2->rowCount()==1;
            }else{
                $return2 = true;
            }
            if(!$this->existTableUser__Adreses()){                         
                $sql= 'CREATE TABLE IF NOT EXISTS `'.\autosystem\DBConn::getTableNameFor('adres_accound').'`(
                    `adresdetail_id`   INT(10),
                    `location_id`    INT(9) NOT NULL,
                    `accound_id`  INT(50)  NOT NULL,
                    `VISIBLE` ENUM(\'TRUE\',\'FALSE\')NOT NULL DEFAULT \'TRUE\',
                    `ENABLED` ENUM(\'TRUE\',\'FALSE\')NOT NULL DEFAULT \'TRUE\',
                    `imputDate` datetime NOT NULL,
                    `TYPE` ENUM(\''.implode('\',\'',array_keys($this->states)).'\') NOT NULL,
                     PRIMARY KEY (`accound_id`,`adresdetail_id`,`TYPE`)
                )ENGINE = InnoDB DEFAULT CHARSET='.\autosystem\DBConn::getCHARSET().' COLLATE='.\autosystem\DBConn::getCOLLATE().';';
                $result3 = $this->DB->query($sql);
                $return3 = $result3->rowCount()==1;
            }else{
                $return3 = true;
            }
            if(!$this->existTableLocales()){
                $sql = 'CREATE TABLE IF NOT EXISTS `'.\autosystem\DBConn::getTableNameFor('adres_locales').'`(
                        `id`    INT(9) NOT NULL AUTO_INCREMENT,
                        `locale` varchar(3) COLLATE '.\autosystem\DBConn::getCOLLATE().' NOT NULL,
                        `country` varchar(50) COLLATE '.\autosystem\DBConn::getCOLLATE().' NOT NULL,
                        `translatekey` varchar(15) COLLATE '.\autosystem\DBConn::getCOLLATE().' DEFAULT NULL,
                        `phonecountrycode_id` INT(10),
                        `urltoIMG` varchar(150) COLLATE '.\autosystem\DBConn::getCOLLATE().' NOT NULL,
                        `IMG_id` int(9) COLLATE '.\autosystem\DBConn::getCOLLATE().' NOT NULL,
                        PRIMARY KEY (`id`)
                )ENGINE = InnoDB DEFAULT CHARSET='.\autosystem\DBConn::getCHARSET().' COLLATE='.\autosystem\DBConn::getCOLLATE().' ;';
                $return4 = $this->DB->query($sql);
                $return4 = $return4->rowCount()==1;
            }else{
                $return4 = true;
            }
            return $return1 && $return2 && $return3 && $return4 ;
        }
        public function isInstalled(){
            return $this->isinstalled||$this->existTableLocation()&& $this->existTableLocationMerges()&& $this->existTableFullLocation() && $this->existTableUser__Adreses() && $this->existTableLocales()&& $this->isinstalled=true;
        }
        public function __upgrade($version){       return true;    }
        public function existTableLocation(){      return $this->existTableLabel('adres_location');       }
        public function existTableLocationMerges(){return $this->existTableLabel('adres_location_merges');       }
        public function existTableLocales(){       return $this->existTableLabel('adres_locales');    }
        public function existTableFullLocation(){  return $this->existTableLabel('adres_detail');     }
        public function existTableUser__Adreses(){ return $this->existTableLabel('adres_accound');      }
        public function getRuleDefault(){       return 'USER';     }
        public function GET_COMPLETE($country){
            return ['belgium','nederland'];
        }
        public function GET_ALocation($location,$templatename,$view,$id,$accound_id=''){
            $l = \autosystem\DBConn::getTableNameFor('adres_location');
            $sql = 'SELECT * FROM `'.$l.'` WHERE `id`=\''.$id.'\'';
            return parent::GET($location,$templatename,$view,$sql,null,true,$accound_id);
        }
        public function FIND($location,$name,$view,$term,$in){
            $l = \autosystem\DBConn::getTableNameFor('adres_location');
            $sql = 'SELECT * FROM `'.$l.'`'.' WHERE `'.$in.'`=\''.$term.'\'';
            parent::GET_Summary($location,$name,$view,$sql);
        }
        protected function GET_SQL($postcode,$accound_id,$isChecked,$filter){
            $l = \autosystem\DBConn::getTableNameFor('adres_location');
            $al = \autosystem\DBConn::getTableNameFor('');//TODO s
            $sql='SELECT *, IF(`'.$l.'`.`isChecked`=\'TRUE\',\' checked=checked \',\' \') AS `isChecked` FROM `'.$l.'`';
            $where = '';
            $link =' WHERE';
            if(!empty($postcode)){
                $where.= $link.' `'.$l.'`.`postcode`=\''.$postcode.'\'';
                $link =' && ';
            }
            if(!empty($accound_id)){
                $where.= $link.' `'.$al.'`.`accound_id`=\''.$accound_id.'\'';
                $link =' && ';
            }
            if(isset($isChecked)){
                $where.= $link.' `'.$l.'`.`isChecked`=\''.(($isChecked)? 'TRUE':'FALSE').'\'';
                $link =' && '; 
            }
            $sql.= $where;
            $this->paginator->setSort(' GROUP BY `'.$l.'`.`postcode`,`'.$l.'`.`id` ');
            //$this->paginator->setSort(' GROUP BY `'.$l.'`.`postcode`,`'.$l.'`.`id` ');
			$this->paginator->setRange(25);
            //print $sql;
            return $sql;
        } 
        private function get_CountSQL($filter,$postcode){
            $l = \autosystem\DBConn::getTableNameFor('adres_location');
            $sql='SELECT COUNT(*) as `count` FROM `'.$l.'` '.
            ((isset($filter)&&$filter!='')?' WHERE TYPE=\''.$filter.'\'':'').
            ((isset($postcode)&&$postcode!='')?' WHERE `postcode`=\''.$postcode.'\'':'');
            return $sql;
        }
        public function GET_LocationSummary($location,$templatename,$view,$filter=null,$postcode=null,$pagenr=0){
            $this->paginator->setSort('');
            $this->paginator->setRange(13);
            $l = \autosystem\DBConn::getTableNameFor('adres_location');
            $sql='SELECT * FROM `'.$l.'`'.
            ((isset($filter)&&$filter!='')?' WHERE TYPE=\''.$filter.'\'':'').
            ((isset($postcode)&&$postcode!='')?' WHERE `postcode`=\''.$postcode.'\'':'');
            $len =$this->GET_COUNT($this->get_CountSQL($filter,$postcode));
            $this->paginator->setlistlength($len);
            $this->paginator->setpagenr($pagenr);
            $this->paginator->setpaginatorreload($location,$templatename);
            parent::GET_Summary($location,$templatename,$view,$sql);
            $view->getpaginator($this->paginator->getdata());
        } 
        public function getLocationId($postcode,$town ,$city ,$state ,$country){
            $sql =' SELECT * FROM '.\autosystem\DBConn::getTableNameFor('adres_location').'';//
            if(!empty($postcode)){
                $sql .=' WHERE `postcode`=:postcode';
                $param[':postcode']=$postcode;
                $array = $this->GET_Stmt($sql,$param);
                //print \DEBUG::printPDO($sql,$param);
                if(is_array($array)){
                    if($array['isChecked']=='TRUE'){
                        return (array_key_exists('id',$array))?$array['id']:false;
                    }else{
                        if(strtolower($array['country'])!=strtolower($country)){    return;}
                        if(strtolower($array['city'])!=strtolower($city)){          return;}
                        if(strtolower($array['state'])!=strtolower($state)){        return;}
                        if(strtolower($array['postcode'])!=strtolower($postcode)){  return;}
                        if(strtolower($array['town'])!=strtolower($town)){      return;}
                        return (array_key_exists('id',$array))?$array['id']:false;
                    }
                }else{
                    return;
                }
            }            
            if(!empty($country)&&!empty($state)||!empty($city)){     
                $sql .=' WHERE `country`=:country && `state`=:state OR `city`=:city OR `town`=:town';
                $param[':country']=$country;
                $param[':state']=$state;
                $param[':city']=$city;
                $param[':town']=$town;
                $array = $this->GET_Stmt($sql,$param);
                return (is_array($array)&& array_key_exists('id',$array))?$array['id']:false;
            }
        }
        private function GET_list($field,$startwith){
            $c = \autosystem\DBConn::getTableNameFor('adres_location'); 
            $sql='SELECT `id` AS \'key\' ,`'.$field.'` AS \'value\' FROM `'.$c.'` WHERE `'.$field.'` LIKE \''.$startwith.'%\'' ;
            return parent::GET_Array($sql);
        }
        private function GET_full_list($startwith){
            $c = \autosystem\DBConn::getTableNameFor('adres_location');
            $fields = ['postcode','town','city','state','province','region','country'];
            $a=[];
            foreach($fields as $field){
              $sql='SELECT CONCAT(\''.$field.'_\',`id`) AS \'key\' ,CONCAT(\''.$field.'_\',`'.$field.'`) AS \'value\' FROM `'.$c.'` WHERE `'.$field.'` LIKE \''.$startwith.'%\' GROUP BY `'.$field.'`' ;
              $a = $a + parent::GET_Array($sql);
            }
            return $a;
        }
        private function GET_countrylist($field,$startwith){
            $c = \autosystem\DBConn::getTableNameFor('adres_locales'); 
            $sql='SELECT `id` AS \'key\' ,`country` AS \'value\' FROM `'.$c.'` WHERE `country` LIKE \''.$startwith.'%\'' ;
            return parent::GET_Array($sql);
        }
        public function AcompleteLocation($startwith,$field=null){
            if(isset($field)){
                if($field=='country'){
                    $return['Acompletelist']=$this->GET_countrylist($field,$startwith);
                }else{
                    $return['Acompletelist']=$this->GET_list($field,$startwith);
                }
            }else{
                $return['Acompletelist']=$this->GET_full_list($startwith);
            }
            return $return;
        }
        public function save_Location($postcode ,$town,$city ,$state ,$province ,$region ,$locale ,$country ,$DIRECTION){
            $ENABLED = 'TRUE';
            $al = \autosystem\DBConn::getTableNameFor('adres_location');
            $key ='INSERT INTO `'.$al.'`(`id`,`postcode`,`town`,`city`,`state`,`province`,`region`,`locale`,`country`,`DIRECTION`) '.
                      'VALUES(NULL ,:postcode ,:town ,:city ,:state ,:province ,:region ,:locale ,:country ,:DIRECTION ) RETURNING `id`';//
            $value = [':postcode'=>$postcode,':town'=>$town,':city'=>$city,':state'=>$state,':province'=>$province,
                      ':region'=>$region,':locale'=>$locale,':country'=>$country,':DIRECTION'=>$DIRECTION ];
            $sql[$key]= $value;
            $return = $this->transaction($sql);
            print \DEBUG::printPDO($key,$value)."\n".print_r($return,true);
            if(is_array($return)){
                return $return[':id'];
            }else{
                return 0;
            }
        }
        public function set_CheckedLocation_byPostcode($postcode){
            $key =' UPDATE `adres_location` set  `isChecked`=\'TRUE\' WHERE `postcode`=:postcode ';
            $sql[$key]=[':postcode'=>$postcode];
            return is_array($this->transaction($sql));           
        }
        public function set_CheckedLocation_byID($id){
            $key =' UPDATE `adres_location` set  `isChecked`=\'TRUE\' WHERE `id`=:id ';
            $sql[$key]=[':id'=>$id];
            return is_array($this->transaction($sql));
        }
        public function existLocation($postcode,$town){
            $l = \autosystem\DBConn::getTableNameFor('adres_location');
            $sql = ' SELECT * FROM `'.$l.'` WHERE `postcode`=:postcode AND ( `town`=:town OR `city`=:town )';
            $value = [':postcode'=>$postcode,':town'=>$town];
            return $this->isQuery($sql,$value); 
        }
    }
    
?>