<?php
/**
* @author Lieven Roegiers
* @copyright 2009
* refactor 2014 
* @CMS autosite
* @return new properties()
*/
namespace autosystem;
class Properties{
    private $thispath;
    private $locale ;
    private $database;
    private $i_am = null;
    private $default='NL';
    private $enabled = true;
    private $istranslator = false;
    public function __construct($locale){
       $this->locale =($this->is_locale($locale))? strtolower($locale): strtolower($this->default);
       $this->enabled=($this->database = \autosystem\DBConn::GET());
    }
	public function getproperties($tofind_keys,$prefix=''){
	    if(!$this->enabled){return false;}
	    if(!is_array($tofind_keys))throw new exception('variable tofind_keys is not array');
		$asoarr=[];
        $questionmarks = implode('?,',$tofind_keys).'?';//@TODO translations to combi locale
        $t = \autosystem\DBConn::getTableNameFor('translations');        
        $dbSql = 'SELECT `value` FROM `'.$t.'` WHERE `locale`=\''.$this->locale.'\' AND `key`=:key';         
        $dbStmt = $this->database->prepare($dbSql);
        $return = [];
        foreach($tofind_keys as $key){
           $dbStmt->bindValue(':key',$key,\PDO::PARAM_STR);
           $dbStmt->execute();
           $dbRij = $dbStmt->fetch(\PDO::FETCH_ASSOC);
           if(!isset($dbRij['value'])){
               if(!key_exists($key,$return)){
                   $return[$key] =($this->istranslator)?$this->geturl($key,$this->locale):'-?-'.$key.'@'.$this->locale.'-?-';
                   $this->propertienotfoundregistrator($this->locale,$key);
               }
           }else{
                   $return[$key]= $dbRij['value'];
           }
        }
		return $return;	
	}
    public function getlocale(){            return $this->locale;        }
    /**
                         '`key` varchar(50) COLLATE '.\autosystem\DBConn::getCOLLATE().' NOT NULL,'.
                      '`locale` varchar(3) COLLATE '.\autosystem\DBConn::getCOLLATE().' NOT NULL,'.
                      '`prior` ENUM(\'HIGH\',\'LOW\',\'NORMAL\') DEFAULT \'NORMAL\' ,'.
                      '`count`  INT(7) DEFAULT 0,'.
    */
    public function propertienotfoundregistrator($locale,$key,$prior = 'NORMAL'){
        //if(!$this->enabled){return false;}
        $tn = \autosystem\DBConn::getTableNameFor('translations_notfound');
        if($this->ispropertieRegistrateAsNotFound($locale,$key)){       
            $dbSql = 'UPDATE `'.$tn.'` SET  `count` = `count` + 1  WHERE `locale`=:locale AND `key`=:key'.(($prior!='NORMAL')?' `PRIOR`='.$prior.'':'');         
            $dbStmt = $this->database->prepare($dbSql);
            $dbStmt->execute([':key'=>$key,':locale'=>$locale ]);
            return ;
        }else{
            $dbSql = 'INSERT INTO `'.$tn.'`(`key`,`locale`)'.'VALUES(:key,:locale)';
            $dbStmt = $this->database->prepare($dbSql);
            $dbStmt->execute([':key'=>$key ,':locale'=>$locale]);
        }
    }
    public function ispropertieRegistrateAsNotFound($locale,$key){
       // if(!$this->enabled){return false;}
        $tn = \autosystem\DBConn::getTableNameFor('translations_notfound');        
        $dbSql = 'SELECT `key` FROM `'.$tn.'` WHERE `locale`=\''.$locale.'\' AND `key`=:key';
        //print $dbSql;          
        $dbStmt = $this->database->prepare($dbSql);
        $dbStmt->execute([':key'=>$key]);
        $return = $dbRij = $dbStmt->fetch(\PDO::FETCH_ASSOC);
        //print ($return)?'true':'false';
        return ($return);//&& count($dbRij)>0
    }
    function set_IAMtranslator(){           $this->istranslator = true;  }
    public function getPropertie($key){     return $this->getATranslation($key,$this->locale,true);   }
    public function getATranslation($key,$locale,$url=false){
        if(!$this->enabled){return false;}//no database
        $t = \autosystem\DBConn::getTableNameFor('translations');  
        $dbSql = 'SELECT value FROM `'.$t.'` WHERE `locale`=\''.$locale.'\' AND `key`=:key';  
        $dbStmt = $this->database->prepare($dbSql);
        $dbStmt->execute([':key'=>$key]);
        if($dbRij = $dbStmt->fetch(\PDO::FETCH_ASSOC)){
            return  $dbRij['value'];
        }
        return $url?$this->geturl($key,$locale):false;
    }
    public function is_locale($locale){    return preg_match('/^([a-z]{2})$/',strtolower($locale));        }
    static public function get_templatelocUrl($location,$name){
        return implode(DIRECTORY_SEPARATOR,['layout','views',$location,$name.'.Qtemplate']);
    }
    static function exist_location($location){
       return is_dir(implode(DIRECTORY_SEPARATOR,['layout','views',$location]));
    }
    static public function get_Qtemplate($location,$name,$cancreate = false){
        $view = self::get_templatelocUrl($location,$name);
        return (is_ViewExist($view))?(string)file_get_contents($view): false;
        throw new \exception((($cancreate)?self::getUrlToCreateTemplate($location,$name):'').'Qtemplate NOT FOUND @');
    }
    static public function getUrlToCreateTemplate($location,$name,$param =''){
        return '<a href="'.self::getpath().'/template/TemplateSummary/?create&amp;location='.$location.'&amp;name='.$name.(($param=='')?'':'&amp;param='.$param).'" target="_blank" >-?-'.$name.'-?-</a>';
    }
    static private function getUrlToEditTemplate($location,$name){
        return '<a href="'.self::getpath().'/template/TemplateSummary/?edit&amp;location='.$location.'&amp;name='.$name.'" target="_blank" >-?-'.$name.'-?-</a>';
    }
    public function __toString(){    return '=>class: Properties locale=>"'.$this->locale.'"';  }
    public function __destruct(){   $this->database = null;        }
    private function geturl($key,$locale,$params = ''){
        return '>"" ></span><a href="'.self::getpath().'/translator/Translate?key='.$key.'&amp;locale='.$locale.$params.'" target="_blank" >-?-'.$key.'-?-</a><-""';
    }
    static private function getpath(){
        $path = explode('/',htmlspecialchars($_SERVER['PHP_SELF']));
        array_pop($path);
        return implode('/',$path);
    }
}
?>