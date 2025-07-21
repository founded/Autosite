<?php
/* model*/
namespace translator;
class TranslateOverview_model extends \translator\Translate_model{
    private $lastQ = '';
    public function saveOne($locale,$key ,$value){
        $t = \autosystem\DBConn::getTableNameFor('translations');
        $sql = 'INSERT INTO '.$t.'(`key`,`locale`,`value`) VALUES(:key,:locale,:value)';
        $msql[$sql]=[':key'=>$key,':locale'=>$locale,':value'=>$value];
        return is_array($this->transaction($msql)); 
    }
    public function exist($locale,$key){
        $t = \autosystem\DBConn::getTableNameFor('translations');
        $sql ='SELECT * FROM '.$t.' WHERE `key`=:key AND `locale`=:locale ';//
        $param = [':key'=>$key,':locale'=>$locale];
        return $this->isQuery($sql,$param);
    }
    public function isInstalled(){        return parent::isInstalled();    }
    public function __upgrade($version){  return true;    }
    public function getModelInfo(){       return 'Translate.Overview.model.info';    }
    public function getVersionNr(){       return (float)5.4;    }
    public function GET_Summary($templateLocation,$templateName,&$view,$filter,$src = 'en',$dest = 'nl',$onlyempty=false,$pagenr=0){
            $t = \autosystem\DBConn::getTableNameFor('translations');
            $sql = 'SELECT `a`.`locale` AS `localesrc`,IF(`b`.`locale` IS NULL ,\''.$dest.'\',`b`.`locale`) AS `localedest`, `a`.`key`,`a`.`value` AS `translatefrom`,`b`.`value` AS `translateto` FROM'.
            ' `'.$t.'` AS `a` LEFT JOIN'.' `'.$t.'` AS `b` '.
            'ON `a`.`locale` = \''.$src.'\' AND `b`.`locale` =\''.$dest.'\' AND (`a`.`key` = `b`.`key` OR `a`.`key` = null)'.
            ' WHERE a.locale = \''.$src.'\' ';
            if($onlyempty){
                $sql.='AND `b`.`locale` IS NULL ';
            }
            $sql.='';
            $this->paginator->setSort(' order by `a`.`value`');
            $this->paginator->setRange($this->defaultPageRange);
            $this->paginator->setlistlength($this->GET_COUNT($this->get_CountSQL($src,$dest,$onlyempty)));
            $this->paginator->setpagenr($pagenr);
            $this->paginator->setpaginatorreload($templateLocation,$templateName);
            parent::GET_Summary($templateLocation,$templateName,$view,$sql);
            $param = ($onlyempty)?'&onlyempty':'&summary';
            $view->getpaginator($this->paginator->getdata(),$param);
    }
    public function GET_NotfoundSummary($templateLocation,$templateName,&$view,$locale = 'en',$pagenr=0){
            $t = \autosystem\DBConn::getTableNameFor('translations_notfound');
            $sql = 'SELECT * FROM `'.$t.'` WHERE `locale` = \''.$locale.'\' ';
            $this->paginator->setSort('ORDER BY `count` DESC');
            $this->paginator->setRange($this->defaultPageRange);
            $this->paginator->setlistlength($this->get_notfoundCount($locale));
            $this->paginator->setpagenr($pagenr);
            $this->paginator->setpaginatorreload($templateLocation,$templateName);
            parent::GET_Summary($templateLocation,$templateName,$view,$sql);
            $param = '&notfound';
            $view->getpaginator($this->paginator->getdata(),$param);
    }
    public function GET_ALLNotfoundSummary($templateLocation,$templateName,&$view,$pagenr=0){
            $t = \autosystem\DBConn::getTableNameFor('translations_notfound');
            $sql = 'SELECT * FROM `'.$t.'` ';
            $this->paginator->setSort('ORDER BY `count` DESC');
            $this->paginator->setRange($this->defaultPageRange);
            $this->paginator->setlistlength($this->GET_COUNT('SELECT count(*) AS `count` FROM `'.$t.'` '));
            $this->paginator->setpagenr($pagenr);
            $this->paginator->setpaginatorreload($templateLocation,$templateName);
            parent::GET_Summary($templateLocation,$templateName,$view,$sql);
            $param = '&allnotfound';
            $view->getpaginator($this->paginator->getdata(),$param);
    }
    public function GET_NotfoundClean(){
            $tn = \autosystem\DBConn::getTableNameFor('translations_notfound');
            $t = \autosystem\DBConn::getTableNameFor('translations');
            $sql = 'DELETE `translations_notfound`'.
                    'FROM `translations_notfound`'.
                    'INNER JOIN `translations`'.
                    'ON `translations_notfound`.`locale` = `translations`.`locale`'.
                    'AND `translations_notfound`.`key` = `translations`.`key`';
            print $sql;
            return $this->isQuery($sql,$param);
            $param = '&notfound';
    }
    public function get_notfoundCount($locale){
            $t = \autosystem\DBConn::getTableNameFor('translations_notfound');
            $sql = 'SELECT count(*) AS `count` FROM `'.$t.'` WHERE `locale` = \''.$locale.'\' ';
            return $this->GET_COUNT($sql);
    }
    private function get_CountSQL($src,$dest,$onlyempty){
            $t = \autosystem\DBConn::getTableNameFor('translations');
            $sql = 'SELECT count(*) AS `count` WHERE `a`.`locale` = \''.$src.'\'';
            if($onlyempty){
                $sql.='AND `b`.`locale` IS NULL';
            }
            return $sql;
    }
    public function getRuleDefault(){    return 'TRANSLATE';   }
}
?>