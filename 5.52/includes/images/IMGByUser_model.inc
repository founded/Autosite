<?php
namespace images;
class IMGByUser_model extends \images\Gallery_model{
    public function getModelInfo(){   return 'gallery.by.user.model.info'; }
    public function getVersionNr(){   return (float)5.4; }
    public function __upgrade($version){  return true;   }
    public function BIND_pathimg($bind_name,$dest_id,$accound_id,$image_id){
        $ic = \autosystem\DBConn::getTableNameFor('images_category');
        $ib = \autosystem\DBConn::getTableNameFor('images_bind');
         if($this->gas_ACCESS_forbindings($bind_name,$accound_id,$dest_id)){
             $key ='SELECT `id` AS \'category_id\' FROM `'.$ic.'`WHERE `label`=:bind_name RETURNING `category_id`';
             $value = [':bind_name'=>$bind_name ];
             $sql[$key]= $value;
             $key ='INSERT INTO `'.$ib.'`(`id`,`category_id`,`dest_id`,`accound_id`,`images_id`,`PATHIMG`) '.
                    'VALUES(NULL,:category_id ,:dest_id ,:accound_id ,:images_id,\'TRUE\') RETURNING `bind_id`';
            $value = [':category_id'=>'',':dest_id'=>$dest_id,':accound_id'=>$accound_id,':images_id'=>$image_id ];
            $sql[$key]= $value;
            return $this->transaction($sql);
         }else{
            return false;
         }
    }
    public function UNBIND_pathimg($UNbind_name,$dest_id,$accound_id,$image_id){
         $key ='SELECT `id` AS \'category_id\' FROM `'.\autosystem\DBConn::getTableNameFor('images_category').'`WHERE `label`=:bind_name RETURNING `category_id`';
         $value = [':bind_name'=>$UNbind_name ];
         $sql[$key]= $value;
         $key ='DELETE FROM `'.\autosystem\DBConn::getTableNameFor('images_bind').'` '.
                     'WHERE `category_id`=:category_id AND `dest_id`=:dest_id AND '.
                            '`accound_id`=:accound_id AND `images_id`=:images_id ';
        $value = [':category_id'=>'',':dest_id'=>$dest_id,':accound_id'=>$accound_id,':images_id'=>$image_id];
        $sql[$key]= $value;
        return $this->transaction($sql);
    }
    public function GET_ImagesPath_OverView($location,$templatename,$view,$category_id='',$accound_id=''){
        parent::ImagesPath_OverView($location,$templatename,$view,$this->getRuleDefault(),$category_id,$accound_id);
    }
    public function GET_Images_OverViewToBind($location,$templatename,$view,$category_name,$accound_id='',$dest_id=''){
        $category_id = $this->GET_Category_id($category_name);
        $view->setENUM_TRANSLATIONS(['TRUE'=>' checked','FALSE'=>' ']);//TODO WATCH overrull text
        $im = \autosystem\DBConn::getTableNameFor('images_path');
        $ib = \autosystem\DBConn::getTableNameFor('images_bind');
        $sql='SELECT DISTINCT(`'.$im.'`.`id`),`urlto`,`image_time`,`namekey`,IF(images_id>0,\'TRUE\',\'FALSE\')AS `imgexist` '.
        'FROM `'.$im.'` LEFT JOIN `'.$ib.'` ON `'.$ib.'`.`images_id`=`'.$im.'`.`id`';
        if($dest_id!=''){
            $sql.= 'AND `dest_id`=\''.$dest_id.'\'';
        }
        $sql.= 'WHERE `'.$im.'`.`category_id`=\''.$category_id.'\' AND `'.$im.'`.`accound_id`=\''.$accound_id.'\'';
        //print $sql;
        parent::GET_Summary($location,$templatename,$view,$sql);
    }
    public function save_asPathimage($name,$url,$category_id,$accound_id,$ext){
        if($accound_id === 0){throw new Exception('INVALID ACCESSS TO SAVE IMAGE');}          
        $ip = parent::getIp();
        $ENABLED = 'TRUE';
        $key ='INSERT INTO `'.\autosystem\DBConn::getTableNameFor('images_path').
                     '`(`id`,`namekey`,`category_id`,`access`,`ext`,`image_time`,`accound_id`,`ip`,`urlto`) '.
                'VALUES(NULL,:namekey,:category_id,\'USER\',:ext,:image_time,:accound_id,:ip,:urlto) RETURNING `id`';//
        $value = [':namekey'=>$name,
        ':category_id'=>$category_id,
        ':ext'=>$ext,
        ':image_time'=>$this->nowDateTime(),
        ':accound_id'=>$accound_id,
        ':ip'=>$ip,
        ':urlto'=>$url
        ];
        $sql[$key]= $value;
        return $this->transaction($sql);                           
    }
    public function delete_Pathimage($id,$category_id,$accound_id){
        if($accound_id === 0){throw new Exception('INVALID ACCESSS TO DELETE IMAGE');}
        $im = \autosystem\DBConn::getTableNameFor('images_path');
        if($url=$this->GetUrl($id)){
            if($this->removePathimg($url)){
                $key ='DELETE FROM `'.$im.'` WHERE `'.$im.'`.`id`=:id AND `'.$im.'`.`accound_id`=:accound_id';
                $value = [':id'=>$id,':accound_id'=>$accound_id];
                $sql[$key]= $value;
            }else{
                $key ='UPDATE `'.$im.'` SET `ENABLED`=\'FALSE\',`access`=\'QUARANTINE\' '.
                  'WHERE `'.$im.'`.`id`=:id AND `'.$im.'`.`accound_id`=:accound_id';
                 $value = [':id'=>$id,':accound_id'=>$accound_id];
                $sql[$key]= $value;
            }          
            return $this->transaction($sql);
        }else{
            return false;
            throw Exception('not found');
        }
    }
    private function GetUrl($id){//unsave to make public
        $im = \autosystem\DBConn::getTableNameFor('images_path');
        $sql ='SELECT `urlto` FROM `'.$im.'` WHERE `'.$im.'`.`id`=:id';//
        $value = [':id'=>$id];
        $arr = $this->GET_Stmt($sql,$value);
        if(key_exists('urlto',$arr)){   
            return $arr['urlto'];
        }else{
            return '';
        }
    }
    private function removePathimg($url){
        return unlink('./'.$url);
    }
    public function save_asEmbeddedimage($namekey ,$category_id ,$access ,$ext ,$accound_id,$data){
        if($accound_id === 0){throw new Exception('INVALID ACCESSS TO SAVE IMAGE');} 
        $key ='INSERT INTO '.\autosystem\DBConn::getTableNameFor('images_embedded').
                '(`id`,`namekey`,`category_id`,`access`,`ext`,`image_time`,`accound_id`,`ENABLED`,`data`)'.
         ' VALUES(NULL,:namekey,:category_id,\'USER\',:ext,:image_time,:accound_id,\'TRUE\',:data) RETURNING images_id';
        $value = [  ':namekey'=>$namekey,
                    ':category_id'=>$category_id,
                    ':ext'=>$ext,
                    ':image_time'=>time(),
                    ':accound_id'=>$accound_id,
                    ':data'=>$data];
        $sql[$key]= $value;
        return $this->transaction($sql);                  
    }     
    public function GET_Categorys($rulles=['USER']){
        $sql='SELECT `id` AS `key` , `title` AS `value` FROM `'.\autosystem\DBConn::getTableNameFor('images_category').'`' ;
        return parent::GET_Array($sql);
    }
    public function GET_Category_id($name){
        $sql='SELECT `id` FROM `'.\autosystem\DBConn::getTableNameFor('images_category').'` WHERE `label`=\''.$name.'\'' ;
        //print $sql;
        $array = parent::GET_Stmt($sql,[]);
        if(array_key_exists('id',$array)){
            return $array['id'];
        }else{
            return 0;
        }
    }
    public function getRuleDefault(){            return 'USER';        }
    public function GET_PathImages($collection_id='',$accound_id=''){
        $sql=parent::SQL_ImagePath('`id` AS \'key\',`urlto` AS \'value\'',$this->getRuleDefault(),$collection_id,$accound_id);
        return  parent::GET_Array($sql);
    }
    public function GET_BIND($binder,$as){
        switch($binder){ 
        	case'auction':
                return 'auctions '.$binder;
        	break;           
        	default :
                throw new Exception('BINDER :'.$binder.'NOT EXIST');
        }
    }
    /*public function GET_UNBIND($binder,$as){
    }*/
    private function gas_ACCESS_forbindings($bind_name,$accound_id,$dest_id){        return true;    }
}
?>