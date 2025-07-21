<?php
namespace translator;
class Translate_model extends \autosystem\Model{
    private $lastQ = '';
    public function save($locale,$key ,$value){
        if($this->exist($locale,$key)){
            return $this->update($locale,$key ,$value);
        }else{
            return $this->insert($locale,$key ,$value);
        }
    }
    public function update($locale,$key ,$value){
        $k = 'UPDATE `'.\autosystem\DBConn::getTableNameFor('translations').'` SET `value`=:value WHERE `locale`=:locale AND `key`=:key';
        $value = [ ':key'=>$key ,':locale'=>$locale ,':value'=>$value ];
        $sql[$k]= $value;
        return $this->transaction($sql);
    }
    public function insert($locale,$key ,$value){
        $k = ' INSERT INTO `'.\autosystem\DBConn::getTableNameFor('translations').'`(`key`,`locale`,`value`)'.
               'VALUES(:key,:locale,:value)';
        $value = [ ':key'=>$key ,':locale'=>$locale ,':value'=>$value ];
        $sql[$k]= $value;
        return $this->transaction($sql);
    }
    public function exist($locale,$key){
        $sql = 'SELECT * FROM `'.\autosystem\DBConn::getTableNameFor('translations').'` WHERE `key`=:key AND `locale`=:locale ';
        $value = [ ':key'=>$key ,':locale'=>$locale];
        return $this->isQuery($sql,$value);
    }
    public function __install(){
        $database = \autosystem\DBConn::GET();
        $dbSql = 'CREATE TABLE IF NOT EXISTS `'.\autosystem\DBConn::getTableNameFor('translations').'` (
                      `key` varchar(50) COLLATE '.\autosystem\DBConn::getCOLLATE().' NOT NULL,
                      `locale` varchar(3) COLLATE '.\autosystem\DBConn::getCOLLATE().' NOT NULL,
                      `value` varchar(250) COLLATE '.\autosystem\DBConn::getCOLLATE().' NOT NULL,
                      PRIMARY KEY (`key`,`locale`)
                    ) ENGINE=InnoDB DEFAULT CHARSET='.\autosystem\DBConn::getCHARSET().' COLLATE='.\autosystem\DBConn::getCOLLATE().';';
        $dbResultaat = $database->query($dbSql);        
        $dbSql = 'CREATE TABLE IF NOT EXISTS `'.\autosystem\DBConn::getTableNameFor('translations_notfound').'` ('.
                      '`key` varchar(50) COLLATE '.\autosystem\DBConn::getCOLLATE().' NOT NULL,'.
                      '`locale` varchar(3) COLLATE '.\autosystem\DBConn::getCOLLATE().' NOT NULL,'.
                      '`prior` ENUM(\'HIGH\',\'LOW\',\'NORMAL\') DEFAULT \'NORMAL\' ,'.
                      '`count`  INT(7) DEFAULT 0,'.
                      'PRIMARY KEY (`key`,`locale`)'.
                    ') ENGINE=InnoDB DEFAULT CHARSET='.\autosystem\DBConn::getCHARSET().' COLLATE='.\autosystem\DBConn::getCOLLATE().';';
        $dbResultaat = $database->query($dbSql);
    }
    public function __upgrade($version){        return true;    }
    public function GET_RelatedView($templateLocation,$templateName,&$view,$localesrc,$locale,$translateText){
        $t=\autosystem\DBConn::getTableNameFor('translations');
        $sql = 'SELECT a.`locale` AS localesrc, IF(b.`locale`=\'\',\''.$locale.'\',\''.$locale.'\') AS `localedest`, a.`key`,a.value AS translatefrom,b.value AS translateto FROM'.
            ' `'.$t.'` AS a LEFT JOIN `'.$t.'` AS b ON a.locale =:src AND b.locale=:dest AND (a.`key` = b.`key` OR a.`key` is null)'.
            ' WHERE a.locale=:src AND a.value=:val ORDER BY a.`key`';
        $param =[':src'=>$localesrc ,':dest'=>$locale ,':val'=>$translateText];
        parent::GET_Summary($templateLocation,$templateName,$view,$sql,$param);
    }
    public function findkey($templateLocation,$templateName,&$view,$key){
        $this->paginator->setSort('');
        $this->paginator->setRange(13);
        $t = \autosystem\DBConn::getTableNameFor('translations');
        $sql = 'SELECT * FROM `'.$t."` WHERE `key` LIKE '%$key%' ";
        parent::GET_Summary($templateLocation,$templateName,$view,$sql);
    }
    public function findvalue($value){    }
    public function isInstalled(){      return $this->existTableLabel('translations')&& $this->existTableLabel('translations_notfound');
    }
    public function getModelInfo(){     return 'Translate.model.info';    }
    public function getVersionNr(){     return (float)5.4;    }
    public function getRuleDefault(){   return 'TRANSLATE';    }
}
?>