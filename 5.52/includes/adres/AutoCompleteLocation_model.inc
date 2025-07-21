<?php
    namespace adres;
	class AutoCompleteLocation_model extends \adres\Location_model{
	    private $cashdir = './oncache/adres_complete/';
        private $usecache=true;
        public function getModelInfo(){
            return 'adres.AutoComplete.model.info';
        }
        public function getVersionNr(){
            return (float)5.31;
        }
        public function __upgrade($version){
            return true;
        }
        public function result($accound_id,$language,$street,$postcode,$town,$city,$state,$country){
            $ip = parent::getIp();
            if($locationId =$this->getLocationId($postcode,$town,$city,$state ,$country)){
                print 'LOCATIONID:'.$locationId ."\n";
            }else{
               $locationId = $this->save_Location($postcode,$town,$city ,$state ,'NULL' ,'NULL' ,$language ,$country ,'NULL'); 
            }
            $ENABLED = 'TRUE';
            $key ='';//
            $value = [':language'=>$language,
                      ':street'=>$street,':houseNumber'=>$houseNumber,':bus'=>$bus,
                      ':postcode'=>$postcode,
                      ':adres'=>$adres,
                      ];
            $sql[$key]= $value;
            return $this->transaction($sql);
        }

        public function FIND($location,$name,$view,$term,$in){ 
            $l = \autosystem\DBConn::getTableNameFor('adres_location');
            $sql = 'SELECT * FROM `'.$l.'`'.' WHERE `'.$in.'`=\''.$term.'\'';
            parent::GET_Summary($location,$name,$view,$sql);
        }

    }
?>