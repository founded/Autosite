<?php
    namespace adres;
	class LocationDetail_model extends \adres\Location_model{
        public function getModelInfo(){        return 'adres.Detail.model.info';       }
        public function getVersionNr(){        return (float)5.4;        }
        public function __upgrade($version){   return true;      }
        public function save_adres($accound_id,$language,$street,$houseNumber,$bus,$postcode,$town,$adres,$city,$state,$country,$gpslat,$gpslng,$adres_Extra = null ){
            $ip = parent::getIp();
            if($locationId =$this->getLocationId($postcode,$town,$city,$state ,$country)){
                print 'LOCATIONID:'.$locationId ."\n";
            }else{
               $locationId = $this->save_Location($postcode,$town,$city ,$state ,'NULL' ,'NULL' ,'' ,$country ,'NULL'); 
            }
            $ENABLED = 'TRUE';
            $key ='INSERT INTO `'.\autosystem\DBConn::getTableNameFor('adres_detail').
                            '`(`id`,`language`,`street`,`houseNumber`,`bus`,`postcode`,`adres`,`adres_E`,`gpslat`,`gpslng`) '.
                      'VALUES(NULL ,:language ,:street ,:houseNumber ,:bus ,:postcode ,:adres ,:adres_E ,:gpslat ,:gpslng ) RETURNING `adresdetail_id`';//
            $value = [':language'=>$language,
                      ':street'=>$street,':houseNumber'=>$houseNumber,':bus'=>$bus,
                      ':postcode'=>$postcode,
                      ':adres'=>$adres,
                      ':adres_E'=>$adres_Extra,
                      ':gpslat'=>$gpslat,
                      ':gpslng'=>$gpslng
                      ];
            $sql[$key]= $value;

            $key ='INSERT INTO `'.\autosystem\DBConn::getTableNameFor('adres_accound').
                           '`(`adresdetail_id`,`location_id`,`accound_id`,`VISIBLE`,`imputDate`,`TYPE`) '.
                      'VALUES(:adresdetail_id ,:location_id ,:accound_id ,:VISIBLE ,:imputDate ,:TYPE ) ';//
            $value = [':adresdetail_id'=>'',
                      ':location_id'=>$locationId,
                      ':accound_id'=>$accound_id,
                      ':VISIBLE'=>'FALSE',
                      ':imputDate'=>$this->nowDateTime(),
                      ':TYPE'=>'NULL'
                      ];
            $sql[$key]= $value;
            return $this->transaction($sql);
        }
        public function delete($adresdetail_id,$accound_id){
            $ip = parent::getIp();
            $ENABLED = 'TRUE';
            $key ='DELETE FROM `'.\autosystem\DBConn::getTableNameFor('adres_accound').'` WHERE `adresdetail_id`=:adresdetail_id AND `accound_id`=:accound_id';//
            $value = [':adresdetail_id'=>$adresdetail_id,':accound_id'=>$accound_id];
            $sql[$key]= $value;
            return $this->transaction($sql);
        }
        public function Update_adresDetail($id,$TYPE,$category,$title,$condition,$shortcomment,$detail,$status,$weblocation,$indicationprice,$location_id=0){
            $key ='UPDATE `'.\autosystem\DBConn::getTableNameFor('adres_detail').'` '.
                   'SET `title`=:title ,`shortcomment`=:shortcomment , `detail`=:detail ,`location_id`=:location_id ,'.
                    '`category_id`=:category_id , `STATUS`=:STATUS , `TAG`=:TAG , `CONDITION`=:CONDITION , '.
                    '`TYPE`=:TYPE ,`weblocation`=:weblocation ,`indicationprice`=:indicationprice  where id=:id';
            $value = [':title'=>$title,
            ':shortcomment'=>$shortcomment,
            ':detail'=>$detail,
            ':location_id'=>$location_id,
            ':category_id'=>$category,
            ':STATUS'=>$status,
            ':TAG'=>'',
            ':CONDITION'=>$condition,
            ':TYPE'=>$TYPE,
            ':weblocation'=>$weblocation,
            ':indicationprice'=>$indicationprice,
            ':id'=>$id,
            ];
            $sql[$key]= $value;
            return $this->transaction($sql);
        } 
        public function Exist_adresDetail($street,$houseNumber,$bus,$locationId){
            $d =\autosystem\DBConn::getTableNameFor('adres_detail');
            $a =\autosystem\DBConn::getTableNameFor('adres_accound'); 
            $sql ='SELECT * FROM '.$d.' LEFT JOIN `'.$a.'` ON `'.$a.'`.`images_id` =`'.$d.'`.`id`';
            $value = [':language'=>$language,
                      ':street'=>$street,
                      ':houseNumber'=>$houseNumber,
                      ':bus'=>$bus,
                      ':postcode'=>$postcode,
                      ];
            $sql[$key]= $value;         
        }
        public function GET_Adres($location,$templatename,$view,$accound_id,$id){
            $d = \autosystem\DBConn::getTableNameFor('adres_detail');
            $l = \autosystem\DBConn::getTableNameFor('adres_location');
            $a = \autosystem\DBConn::getTableNameFor('adres_accound');
            $sql='SELECT * FROM `'.$a.'` LEFT JOIN `'.$d.'` ON `adresdetail_id`=`'.$d.'`.`id` AND `accound_id`=:accound_id '.
            ' LEFT JOIN `'.$l.'` ON `location_id`=`'.$l.'`.`id` WHERE `adresdetail_id`=:id';
            $param = [':accound_id'=>$accound_id ,':id'=>$id];
            return parent::GET($location,$templatename,$view,$sql,$param);
        } 
    }
?>