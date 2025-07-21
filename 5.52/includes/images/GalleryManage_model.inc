<?php
 /**
 * @author Lieven Roegiers
 * @copyright 
 * @CMS autosite
 */
namespace images;
class GalleryManage_model extends \images\Gallery_model{
    public function __install(){      return parent::__install(); }
    public function isInstalled(){    return parent::isInstalled(); }
    public function getModelInfo(){   return 'gallery.Manage.model.info';  }
    public function getVersionNr(){   return (float)5.4; }
    public function __upgrade($version){
        return true;
    }
    public function save($categoryname,$categorylabel,$access,$user_id,$sub_Category='NULL'){
        if(!$this->existCategory($categoryname,$categorylabel)){
            $c=\autosystem\DBConn::getTableNameFor('images_category');
            $key ='INSERT INTO `'.$c.'`(`id`,`title`, `label`,`TYPE`,`category_id`,`user_id`)'.
            'VALUES(NULL,:title,:label,:type,:category_id,:user_id);';
    $value = [':title'=>$categoryname,':label'=>$categorylabel,':type'=>$access,
              ':category_id'=>$sub_Category,':user_id'=>$user_id];
            $sql[$key]= $value;
            return $this->transaction($sql);
        }
    }
    public function existCategory($categoryname,$categorylabel){
        $c=\autosystem\DBConn::getTableNameFor('images_category');
        $key ='SELECT * FROM `'.$c.'` WHERE `title`=:title OR `label`=:label ;';
        $value = [':title'=>$categoryname,':label'=>$categorylabel];
        return $this->isQuery($key,$value);
    }
    public function getRuleDefault(){  return 'ADMINISTRATION'; }
}
?>