<?php
namespace start;
class SiteMapSummary_model extends \autosystem\Model{
    static private $isinstalled = false;
    public function getModelInfo(){     return 'SiteMapSummary.model.info';   }
    public function getVersionNr(){     return (float)4.7;   }
    public function __install(){        return true;    }
    public function __upgrade($version){return true;    }
    public function isInstalled(){      return self::$isinstalled||self::$isinstalled =true  ;  }
    public function getRuleDefault(){   return 'USER';  }
    public function GET_AOverView($location,$templatename,$view,$type){
        $i = \autosystem\DBConn::getTableNameFor('installs');
           $sql='SELECT `'.$i."`.* , IF(`MENU`='TRUE',' checked=checked ',' ') AS `inmenu`".
           'FROM `'.$i.'`WHERE `TYPE`=\''.$type.'\' ORDER BY `location`,`key` ';
           //print $sql;
           return parent::GET_Array($sql);
    }
    public function GET_SumArray($type){
        $i = \autosystem\DBConn::getTableNameFor('installs');
           $sql='SELECT `'.$i."`.* , IF(`MENU`='TRUE',' checked=checked ',' ') AS `inmenu`".
           'FROM `'.$i.'`WHERE `TYPE`=\''.$type.'\' ORDER BY `location`,`key` ';
           //print $sql;
           return parent::GET_Stmt_key($sql,[],0,'key');
    }
    public function GET_KingSumArray(){
        $i = \autosystem\DBConn::getTableNameFor('installs');
           $sql='SELECT `'.$i."`.* , IF(`MENU`='TRUE',' checked=checked ',' ') AS `isinmenu`".
           'FROM `'.$i.'`ORDER BY `location`,`key` ';
           print $sql;
           return parent::GET_Stmt_key($sql,[],0,'key');
    }
    public function GET_keysToTranslate($category_id){
           $sql='SELECT id as \'key\' , label as \'value\' FROM `'.\autosystem\DBConn::getTableNameFor('auction_category').'` '.((isset($category_id))?" where `category_id`='$category_id'":'') ;
           return parent::GET_Array($sql);
    }
}
?>