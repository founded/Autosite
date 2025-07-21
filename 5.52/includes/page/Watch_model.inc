<?php
namespace page;
class Watch_model extends \page\Locales_model{
    public $states = array( 'PROGRESS'=>'progress','ERROR' => 'error','CLOSED' => 'closed','OPEN' => 'open','CANCELED' => 'canceled');
    protected $isinstalled;
    public function getModelInfo(){   return 'watch.model.info';}
    public function getVersionNr(){   return (float)5.4;}  
    public function __install(){
            $database = \autosystem\DBConn::GET();
            if(!$this->existTableCategory()){
                $sql = 'CREATE TABLE IF NOT EXISTS `'.\autosystem\DBConn::getTableNameFor('page_category').'`('.
                    '`id`    INT(20) NOT NULL AUTO_INCREMENT,'.
                    '`title`  varchar(230) COLLATE '.\autosystem\DBConn::getCOLLATE().' NOT NULL,'.
                    '`label`  varchar(230) COLLATE '.\autosystem\DBConn::getCOLLATE().' NOT NULL,'.
                    '`category_id`    INT(4) DEFAULT 0,'.
                    '`user_id`    INT(10) NOT NULL,'.
                    '`stars`    INT(4) DEFAULT 0,'.
                    'PRIMARY KEY (`id`)'.
                ')ENGINE = InnoDB DEFAULT CHARSET='.\autosystem\DBConn::getCHARSET().' COLLATE='.\autosystem\DBConn::getCOLLATE().' ;';
                $result1 = $database->query($sql);
                $return1 = $result1->rowCount()==1;
            }else{
                $return1 = true;
            }
            //IPv6 (39 bytes) :              ABCD:ABCD:ABCD:ABCD:ABCD:ABCD:ABCD:ABCD
            //IPv4-mapped IPv6 (45 bytes) :(chars46)  ABCD:ABCD:ABCD:ABCD:ABCD:ABCD:192.168.158.190
            if(!$this->existTablePage()){
                $sql = 'CREATE TABLE IF NOT EXISTS `'.\autosystem\DBConn::getTableNameFor('page').'`(
                    `id`    INT(20) NOT NULL AUTO_INCREMENT,
                    `title.key`  varchar(510) COLLATE '.\autosystem\DBConn::getCOLLATE().' NOT NULL,
                    `title`  varchar(510) COLLATE '.\autosystem\DBConn::getCOLLATE().' NOT NULL,
                    `backcollor`  varchar(6) COLLATE '.\autosystem\DBConn::getCOLLATE()." DEFAULT 'FFFFFF',
                    `shortcomment`  varchar(1000) COLLATE ".\autosystem\DBConn::getCOLLATE().' NOT NULL,
                    `content`  TEXT COLLATE '.\autosystem\DBConn::getCOLLATE().' NOT NULL,
                    `accound_id`  INT(50) ZEROFILL NOT NULL,
                    `category_id`    INT(4) DEFAULT \'0\',
                    `STATUS` ENUM(\''.implode('\',\'',array_keys($this->states)).'\')NOT NULL ,
                    `access` ENUM(\''.implode('\',\'',array_keys($this->modeltypes)).'\')NOT NULL,
                    `VISIBLE` ENUM(\'TRUE\',\'FALSE\')NOT NULL DEFAULT \'TRUE\',
                    `imputDate` datetime NOT NULL,
                    `ip`  varchar(50) COLLATE '.\autosystem\DBConn::getCOLLATE().' NOT NULL,
                    `page_langlocale` varchar(5) COLLATE '.\autosystem\DBConn::getCOLLATE().' NOT NULL,
                    `view_stars`    INT(4) DEFAULT 0,
                    `search_stars`    INT(4) DEFAULT 0,
                    PRIMARY KEY (`id`)
                )ENGINE = InnoDB DEFAULT CHARSET='.\autosystem\DBConn::getCHARSET().' COLLATE='.\autosystem\DBConn::getCOLLATE().' ;';
                $result2 = $database->query($sql);
                $return2 = $result2->rowCount()==1;
            }else{
                $return2 = true;
            }
            if(!$this->existTablePageFavorites()){
                $sql = 'CREATE TABLE IF NOT EXISTS `'.\autosystem\DBConn::getTableNameFor('page_favorites').'`(
                    `page_id`  INT(20) NOT NULL ,
                    `accound_id`  INT(50)  NOT NULL,
                    PRIMARY KEY (`page_id`,`accound_id`)
                )ENGINE = InnoDB DEFAULT CHARSET='.\autosystem\DBConn::getCHARSET().' COLLATE='.\autosystem\DBConn::getCOLLATE().' ;';
                $result3 = $database->query($sql);
                $return3 = $result3->rowCount()==1;
            }else{
                $return3= true;
            }
            if(!$this->existTablePageMeta()){
                $sql = 'CREATE TABLE IF NOT EXISTS `'.\autosystem\DBConn::getTableNameFor('page_meta').'`(
                    `page_id` INT(20) NOT NULL ,
                    `meta_id` INT(20) NOT NULL ,
                    `accound_id` INT(50)  NOT NULL,
                    PRIMARY KEY (`page_id`,`meta_id`)
                )ENGINE = InnoDB DEFAULT CHARSET='.\autosystem\DBConn::getCHARSET().' COLLATE='.\autosystem\DBConn::getCOLLATE().' ;';
                $result4 = $database->query($sql);
                $return4 = $result4->rowCount()==1;
            }else{
                $return4= true;
            }
            if(!$this->existTableUpdates()){
                $sql = 'CREATE TABLE IF NOT EXISTS `'.\autosystem\DBConn::getTableNameFor('page_updates').'`(
                    `page_id` INT(20) NOT NULL ,
                    `accound_id` INT(50)  NOT NULL,
                    `updateDate` datetime NOT NULL,
                    `ip`  varchar(50) COLLATE '.\autosystem\DBConn::getCOLLATE().' NOT NULL,
                    PRIMARY KEY (`page_id`,`accound_id`)
                )ENGINE = InnoDB DEFAULT CHARSET='.\autosystem\DBConn::getCHARSET().' COLLATE='.\autosystem\DBConn::getCOLLATE().' ;';
                $result5 = $database->query($sql);
                $return5 = $result5->rowCount()==1;
            }else{
                $return5= true;
            }
            return $return2 && $return1 && $return3 && $return4&& $return5;
    }
    public function __upgrade($version){
            return true;
    }
    public function GETPage($location,$templatename,$view,$id,$accound_id=0){
        $p = \autosystem\DBConn::getTableNameFor('page');
        $c = \autosystem\DBConn::getTableNameFor('page_category');
        $m = \autosystem\DBConn::getTableNameFor('page_meta');
        $sql='SELECT `'.$p.'`.* FROM `'.$p.'` LEFT JOIN `'.$c.'` ON `'.$p.'`.`id`=`'.$c.'`.`category_id`'.
             'LEFT JOIN `'.$m.'` ON `'.$m.'`.`page_id`=`'.$p.'`.`id` WHERE `'.$p.'`.`id`=\''.$id.'\'';
        //print $sql;
        return parent::GET($location,$templatename,$view,$sql,null,false,$accound_id);
    }
    public function GETPageByTitle($location,$templatename,$view,$title,$accound_id=0){
        $p = \autosystem\DBConn::getTableNameFor('page');
        $c = \autosystem\DBConn::getTableNameFor('page_category');
        $m = \autosystem\DBConn::getTableNameFor('page_meta');
        $sql='SELECT `'.$p.'`.* FROM `'.$p.'` LEFT JOIN `'.$c.'` ON `'.$p.'`.`id`=`'.$c.'`.`category_id`'.
             'LEFT JOIN `'.$m.'` ON `'.$m.'`.`page_id`=`'.$p.'`.`id` WHERE `'.$p.'`.`title`=:title';
        $param = [':title'=>$title];
        //DEBUG::printPDO($sql,) $sql;
        return parent::GET($location,$templatename,$view,$sql,$param,false,$accound_id);
    }
    public function GETArray($id,$accound_id=''){
        $p = \autosystem\DBConn::getTableNameFor('page');
        $c = \autosystem\DBConn::getTableNameFor('page_category');
        $m = \autosystem\DBConn::getTableNameFor('page_meta');
        $sql = 'SELECT `'.$p.'`.* FROM `'.$p.'` LEFT JOIN `'.$c.'` ON `'.$p.'`.`id`=`'.$c.'`.`category_id`'.
               'LEFT JOIN `'.$m.'` ON `'.$m.'`.`page_id`= `'.$p.'`.`id` WHERE `'.$p.'`.`id`=\''.$id.'\'';
        return $this->GET_Stmt($sql,[]);
    }
    /* --------------image start---------------*/
	public function GET_PageImages(){
		$c =\autosystem\DBConn::getTableNameFor('images_category');
		$im=\autosystem\DBConn::getTableNameFor('images_path');
		$sql='SELECT `id` FROM `'.$c.'` WHERE `label`=\'page\'' ;
        $array = parent::GET_Stmt($sql,[]);
        if(array_key_exists('id',$array)){
            $sql='SELECT `id` AS \'key\',`urlto` AS \'value\' FROM `'.$im.'` WHERE '.'`category_id`=\''.$array['id'].'\''; 
			return $this->GET_Array($sql,[]);			
        }
        return [];   
    }
    /* --------------image end---------------*/
    public function existId($id){
        if(!$this->isinstalled){return false;}
        $sql = 'SELECT `id` FROM '.\autosystem\DBConn::getTableNameFor('page').' WHERE `id`=:id ' ;
        $value = [':id'=>$id];
        return $this->isQuery($sql,$value);
    }
    public function isPageVisibled($id){
        if(!$this->isinstalled){return false;}
        $sql = 'SELECT `id` FROM '.\autosystem\DBConn::getTableNameFor('page').' WHERE `id`=:id AND `VISIBLE`=\'TRUE\'' ;
        $value = [':id'=>$id];
        return $this->isQuery($sql,$value);
    }
    public function existTableCategory(){       return $this->existTableLabel('page_category'); }
    public function existTablePage(){           return $this->existTableLabel('page'); }
    public function existTablePageFavorites(){  return $this->existTableLabel('page_favorites'); }
    public function existTablePageMeta(){       return $this->existTableLabel('page_meta');}
    public function existTableUpdates(){        return $this->existTableLabel('page_updates');}
    public function isInstalled(){
        return $this->isinstalled||$this->existTableCategory()&& $this->existTablePage()&& $this->existTablePageFavorites()&& $this->existTablePageMeta()&&$this->isinstalled=true;
    }
    public function getRuleDefault(){           return 'PUBLIC';}
}
?>