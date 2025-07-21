<?php
    namespace adres;
	class ModLocation_model extends \adres\Location_model{
        public $directions = array('NULL' => 'not availeble','NORTH' => 'north','SOUD' => 'soud','MID' => 'central','EAST' => 'east-side','WEST' => 'west-side');
        public function getModelInfo(){        return 'adres.modlocation.model.info';     }
        public function getVersionNr(){        return (float)5.4;       }
        public function __upgrade($version){   return true;        }
        public function GET_toMergeSummary($location,$name,$view,$accound_id,$filter){  
        }
        public function GET_EqualsSummary($location,$name,$view,$accound_id,$filter){
            $l = \autosystem\DBConn::getTableNameFor('adres_location');
            parent::findDublicates($location,$name,$view ,$l,'postcode',['town']);
        }
        public function GET_NOTchecked($location,$templatename,$view,$accound_id,$pagenr,$filter){
            $sql=$this->GET_SQL('','',false,$filter);
            $this->paginator->setlistlength($this->GET_COUNT($sql));
            $this->paginator->setpagenr($pagenr);
            $this->paginator->setpaginatorreload($location,$templatename);
            parent::GET_Summary($location,$templatename,$view,$sql,null,true,$accound_id);
            $view->getpaginator($this->paginator->getdata(),'&notchecked');
        }
        public function GET_ASummary($location,$name,$view,$filter){
            $l = \autosystem\DBConn::getTableNameFor('adres_location');
            $sql='SELECT * FROM `'.$l.'` '.((isset($filter)&&$filter!='')?' WHERE TYPE=\''.$filter.'\'':'');
            parent::GET_Summary($location,'item',$view,$sql);
        }
        public function GET_LocalesSummary($location,$name,$view,$filter){
            $l = \autosystem\DBConn::getTableNameFor('adres_locales');
            $sql='SELECT * FROM `'.$l.'` '.((isset($filter)&&$filter!='')?' WHERE TYPE=\''.$filter.'\'':'');
            parent::GET_Summary($location,'item',$view,$sql);
        }
        public function setChecked($item_id,$bool){
            $Checked = ($bool=='true')?'TRUE':'FALSE';
            $l = \autosystem\DBConn::getTableNameFor('adres_location');
            $key= 'UPDATE `'.$l.'` SET `isChecked`=:Checked WHERE `id`=:id';
            $value = [':Checked'=>$Checked,':id'=>$item_id];
            $sql[$key]= $value;
            print \DEBUG::printPDO($key, $value);
            return $this->transaction($sql);   
        }
        public function Update(){
            $key ='';
            $value = [];
            $sql[$key]= $value;
            return $this->transaction($sql);
        }
        public function getRuleDefault(){            return 'MODERATOR';        }
        public function GET_Locations(){
            $sql = 'SELECT id,city,state,locale,country,DIRECTION,isChecked FROM `'.\autosystem\DBConn::getTableNameFor('adres_location').'`';
        }                
    }
    
?>