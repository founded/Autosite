<?php
namespace images;
class Gallery_model extends \autosystem\Model{
    public $acceses = array( 'PUBLIC'=>'public','PRIVATE' => 'private','PARTNER' => 'partner','USER' => 'user','QUARANTINE' => 'Quarantine');
    protected $isinstalled;
    public function __install(){
        if(!$this->existTableCategory()){
            $sql = 'CREATE TABLE IF NOT EXISTS `'.\autosystem\DBConn::getTableNameFor('images_category').'`(
                `id` INT(20) NOT NULL AUTO_INCREMENT,
                `title` varchar(230) COLLATE '.\autosystem\DBConn::getCOLLATE().' NOT NULL,
                `label` varchar(230) COLLATE '.\autosystem\DBConn::getCOLLATE().' NOT NULL,
                `TYPE` ENUM(\''.implode('\',\'',array_keys($this->modeltypes)).'\') default \'NULL\',
                `category_id` INT(4),
                `user_id` INT(10) NOT NULL,
                PRIMARY KEY (`id`)
            )ENGINE = InnoDB DEFAULT CHARSET='.\autosystem\DBConn::getCHARSET()." COLLATE=".\autosystem\DBConn::getCOLLATE()." ;";
            $result1 = $this->DB->query($sql);
            $return1 = $result1->rowCount()==1;
        }else{    $return1 = true;   }
        if(!$this->existTableEmbeddedImages()){
            $dbSql = 'CREATE TABLE IF NOT EXISTS `'.\autosystem\DBConn::getTableNameFor('images_embedded').'`(
                     `id` int(9) NOT NULL AUTO_INCREMENT,
                     `namekey` varchar(50) COLLATE '.\autosystem\DBConn::getCOLLATE().' NOT NULL,
                     `category_id` varchar(50) COLLATE '.\autosystem\DBConn::getCOLLATE().' NOT NULL,
                     `access` ENUM(\''.implode('\',\'',array_keys($this->acceses )).'\')NOT NULL,
                     `ext` varchar(8) COLLATE '.\autosystem\DBConn::getCOLLATE().' NOT NULL,
                     `image_time` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
                     `accound_id` INT(50)  NOT NULL,
                     `ENABLED` ENUM(\'TRUE\',\'FALSE\')NOT NULL DEFAULT \'TRUE\',
                     `ip` varchar(50) COLLATE '.\autosystem\DBConn::getCOLLATE().' NOT NULL,
                     `data` mediumblob NOT NULL,
                    PRIMARY KEY (`id`)
                    )ENGINE = InnoDB DEFAULT CHARSET='.\autosystem\DBConn::getCHARSET().' COLLATE='.\autosystem\DBConn::getCOLLATE().' ;';
            $result2 = $this->DB->query($dbSql);
            $return2 = $result2->rowCount()==1;
        }else{   $return2 = true;   }
        if(!$this->existTablePathImages()){
            $dbSql = 'CREATE TABLE IF NOT EXISTS `'.\autosystem\DBConn::getTableNameFor('images_path').'`(
                     `id` int(9) NOT NULL AUTO_INCREMENT,
                     `namekey` varchar(50) COLLATE '.\autosystem\DBConn::getCOLLATE().' NOT NULL,
                     `category_id` varchar(50) COLLATE '.\autosystem\DBConn::getCOLLATE().' NOT NULL,
                     `access` ENUM(\''.implode('\',\'',array_keys($this->acceses )).'\')NOT NULL,
                     `ext` varchar(8) COLLATE '.\autosystem\DBConn::getCOLLATE().' NOT NULL,
                     `image_time` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
                     `accound_id` INT(50)  NOT NULL,
                     `ENABLED` ENUM(\'TRUE\',\'FALSE\')NOT NULL DEFAULT \'TRUE\',
                     `ip` varchar(50) COLLATE '.\autosystem\DBConn::getCOLLATE().' NOT NULL,
                     `urlto` varchar(150) COLLATE '.\autosystem\DBConn::getCOLLATE().' NOT NULL,
                    PRIMARY KEY (`id`)
                    )ENGINE = InnoDB DEFAULT CHARSET='.\autosystem\DBConn::getCHARSET().' COLLATE='.\autosystem\DBConn::getCOLLATE().' ;';
        $result3 = $this->DB->query($dbSql);
            $return3 = $result3->rowCount()==1;
        }else{    $return3 = true;   }
        if(!$this->existTableRel()){//$bind_name,$id,$accound_id,$image_id
            $dbSql = 'CREATE TABLE IF NOT EXISTS `'.\autosystem\DBConn::getTableNameFor('images_bind').'`(
                         `id` INT(50) NOT NULL AUTO_INCREMENT,
                         `category_id` INT(50)  NOT NULL,
                         `images_id` INT(20) NOT NULL,
                         `accound_id`  INT(50)  NOT NULL,
                         `dest_id`  INT(50)  NOT NULL,
                         `PATHIMG` ENUM(\'TRUE\',\'FALSE\')NOT NULL DEFAULT \'FALSE\',
                         `EMBEDDED` ENUM(\'TRUE\',\'FALSE\')NOT NULL DEFAULT \'FALSE\',
                         `HAVE_KEYS` ENUM(\'TRUE\',\'FALSE\')NOT NULL DEFAULT \'FALSE\',
                         `time` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
                        PRIMARY KEY  (`id`)
                        )ENGINE = InnoDB DEFAULT CHARSET='.\autosystem\DBConn::getCHARSET().' COLLATE='.\autosystem\DBConn::getCOLLATE().' ;';
            $result4 = $this->DB->query($dbSql);
            $return4 = $result4->rowCount()==1;
        }else{    $return4 = true;  }
        if(!$resultx =$this->DB->query('ALTER TABLE `'.\autosystem\DBConn::getTableNameFor('images_bind').'` COMMENT=\'there is a connection by category id and dest_id\'')){
               //print_r($resultx);
        }
        return $return1 && $return2 && $return3 && $return4;
    }
    public function isInstalled(){
        return $this->isinstalled||$this->existTableCategory()&& $this->existTablePathImages()&&$this->existTableEmbeddedImages()&&$this->existTableRel() &&$this->isinstalled=true;
    }
    public function existTablePathImages(){     return $this->existTableLabel('images_path'); }
    public function existTableEmbeddedImages(){ return $this->existTableLabel('images_embedded'); }
    public function existTableCategory(){       return $this->existTableLabel('images_category');}
    public function existTableRel(){            return $this->existTableLabel('images_bind'); }
    public function getModelInfo(){             return 'gallery.model.info'; }
    public function getVersionNr(){             return (float)5.4; }
    public function getRuleDefault(){           return 'USER'; }
    public function GET_item($location,$name,&$view,$id,$accound_id){
    }
    public function GET_ImageAsUrl($id,$accound_id){
        $sql = $this->SQL_ImagePath('*','','',$accound_id,$id);
        return parent::GET_Stmt($sql,[]);
    }
    protected function SQL_ImagePath($select,$access,$category_id,$accound_id,$image_id=null){
        $sql='SELECT '.$select.' FROM `'.\autosystem\DBConn::getTableNameFor('images_path').'`';
        $where  ='';
        $link ='';
        if(!empty($category_id)){$where.= $link.'`category_id`=\''.$category_id.'\''; $link =' AND '; }
        if(!empty($accound_id)){ $where.= $link.'`accound_id`=\''.$accound_id.'\'';   $link =' AND '; }
        if(!empty($image_id)){   $where.= $link.'`id`=\''.$image_id.'\'';             $link =' AND '; }
        if($where!=''){
            $sql .= ' WHERE '.$where;
        }
        return $sql;
    }
    protected function ImagesPath_OverView($location,$templatename,$view,$access,$category_id='',$accound_id=''){
        $sql=$this->SQL_ImagePath('*',$access,$category_id,$accound_id);
        parent::GET_Summary($location,$templatename,$view,$sql);
    }
    public function GET_Categorys(){
        $sql='SELECT id AS \'key\' , title AS \'value\' FROM `'.\autosystem\DBConn::getTableNameFor('images_category').'`' ;
        return parent::GET_Array($sql);
    }
    public function GET_Title($id){
        $i = \autosystem\DBConn::getTableNameFor('images_category');
        $sql='SELECT id AS \'key\' ,label AS \'value\' FROM `'.$i.'` WHERE `id`=\''.$id.'\'' ;
        $arr = parent::GET_Array($sql);
        if(key_exists($id,$arr)){
            return $arr[$id];
        }
        return '';
    }
    public function __upgrade($version){
            return true;
    }
}
?>