<?php
namespace search;
class Term_model extends \autosystem\Model{
    protected $states =[ 'ACTIVE'=>'active','STOPED' => 'stoped','ATTEND' => 'attend','DELETED' => 'deleted'];
    protected $isinstalled = false;
    private $viewtaged=false;
    public function __install(){
        $database = \autosystem\DBConn::GET();
        if(!$this->existTableSearchterm()){
            $sql = 'CREATE TABLE IF NOT EXISTS `'.\autosystem\DBConn::getTableNameFor('search_term').'`('.
                '`id` INT(20) NOT NULL AUTO_INCREMENT,'.
                '`locale` varchar(5) COLLATE '.\autosystem\DBConn::getCOLLATE().' NOT NULL,'.
                '`term` varchar(230) COLLATE '.\autosystem\DBConn::getCOLLATE().' NOT NULL,'.
                '`term_id` INT(20) DEFAULT 0,'.
                '`search_stars` INT(10) DEFAULT 0,'.
                '`CHECKED` ENUM(\'TRUE\',\'FALSE\')NOT NULL DEFAULT \'FALSE\','.
                '`ENABLED` ENUM(\'TRUE\',\'FALSE\')NOT NULL DEFAULT \'FALSE\','.
                '`PUBLIC` ENUM(\'TRUE\',\'FALSE\')NOT NULL DEFAULT \'FALSE\','.
                '`PROMOTE` ENUM(\'TRUE\',\'FALSE\')NOT NULL DEFAULT \'FALSE\','.//TODO
                '`lastscan` datetime DEFAULT NULL,'.
                '`stars` INT(10) DEFAULT 0,'.
                'FULLTEXT (`term`),'.
                'PRIMARY KEY (`id`,`locale`)'.
            ')ENGINE = MyISAM DEFAULT CHARSET='.\autosystem\DBConn::getCHARSET().' COLLATE='.\autosystem\DBConn::getCOLLATE().' ;';
            $result1 = $database->query($sql);
            $return1 = $result1->rowCount()==1;
            
        }else{
            $return1 = true;
        }
        if(!$this->existTableSearchTermsDetail()){
            $sql = 'CREATE TABLE IF NOT EXISTS `'.\autosystem\DBConn::getTableNameFor('search_term_detail').'`('.
                '`term_id` INT(20) NOT NULL,'.
                '`imputDate` datetime NOT NULL,'.
                '`ip` varchar(50) COLLATE '.\autosystem\DBConn::getCOLLATE().' NOT NULL,'.
                'PRIMARY KEY (`term_id`)'.
            ')ENGINE = InnoDB DEFAULT CHARSET='.\autosystem\DBConn::getCHARSET().' COLLATE='.\autosystem\DBConn::getCOLLATE().' ;';
            $result2 = $database->query($sql);
            $return2 = $result2->rowCount()==1;
        }else{
            $return2 = true;
        }
        if(!$this->existTableSearchTermsPages()){
            $sql = 'CREATE TABLE IF NOT EXISTS `'.\autosystem\DBConn::getTableNameFor('search_terms_pages').'`('.
                '`term_id` INT(20) NOT NULL,'.
                '`page_id` INT(20) NOT NULL,'.
                '`incontent` ENUM(\'TRUE\',\'FALSE\')NOT NULL DEFAULT \'FALSE\','.
                '`inkeywords` ENUM(\'TRUE\',\'FALSE\')NOT NULL DEFAULT \'FALSE\','.
                '`intitleH` ENUM(\'TRUE\',\'FALSE\')NOT NULL DEFAULT \'FALSE\','.
                '`amount` INT(3) DEFAULT 0,'.
                'PRIMARY KEY (`term_id`,`page_id`)'.
            ')ENGINE = InnoDB DEFAULT CHARSET='.\autosystem\DBConn::getCHARSET().' COLLATE='.\autosystem\DBConn::getCOLLATE().' ;';
            $result3 = $database->query($sql);
            $return3 = $result3->rowCount()==1;
        }else{
            $return3 = true;
        }
        if(!$this->existTableSearchPagesAccess()){
            $sql = 'CREATE TABLE IF NOT EXISTS `'.\autosystem\DBConn::getTableNameFor('search_pages_access').'`('.
                '`page_id` INT(20) NOT NULL,'.
                '`PUBLIC` ENUM(\'TRUE\',\'FALSE\')NOT NULL DEFAULT \'TRUE\','.
                'PRIMARY KEY (`page_id`)'.
            ')ENGINE = InnoDB DEFAULT CHARSET='.\autosystem\DBConn::getCHARSET().' COLLATE='.\autosystem\DBConn::getCOLLATE().' ;';
            $result4 = $database->query($sql);
            $return4 = $result4->rowCount()==1;
            
        }else{
            $return4 = true;
        }
        if(!$this->existTableSearchModuleAccess()){
            $sql = 'CREATE TABLE IF NOT EXISTS `'.\autosystem\DBConn::getTableNameFor('search_Modules_access').'`('.
                '`action` varchar(230) COLLATE '.\autosystem\DBConn::getCOLLATE().' NOT NULL,'.
                '`loc` varchar(230) COLLATE '.\autosystem\DBConn::getCOLLATE().' NOT NULL,'.
                '`HASRULLS` ENUM(\'TRUE\',\'FALSE\')NOT NULL DEFAULT \'TRUE\','.
                '`HASMODULESEARCH` ENUM(\'TRUE\',\'FALSE\')NOT NULL DEFAULT \'TRUE\','.
                '`COMBINE` ENUM(\'TRUE\',\'FALSE\')NOT NULL DEFAULT \'TRUE\','.
                'PRIMARY KEY (`action`,`loc`)'.
            ')ENGINE = InnoDB DEFAULT CHARSET='.\autosystem\DBConn::getCHARSET().' COLLATE='.\autosystem\DBConn::getCOLLATE().' ;';
            $result8 = $database->query($sql);
            $return8 = $result8->rowCount()==1;
            
        }else{
            $return8 = true;
        }
        if(!$this->existTableSearchPages()){
            $sql = 'CREATE TABLE IF NOT EXISTS `'.\autosystem\DBConn::getTableNameFor('search_pages').'`('.
                '`id` INT(20) NOT NULL AUTO_INCREMENT,'.
                '`locale` varchar(5) COLLATE '.\autosystem\DBConn::getCOLLATE().' DEFAULT NULL,'.
                '`url` varchar(1000) COLLATE '.\autosystem\DBConn::getCOLLATE().' NOT NULL,'.
                '`view_stars` INT(10) DEFAULT 0,'.
                '`INTERNAL` ENUM(\'TRUE\',\'FALSE\')NOT NULL DEFAULT \'TRUE\','.
                '`ENABLED` ENUM(\'TRUE\',\'FALSE\')NOT NULL DEFAULT \'TRUE\','.
                '`lastvisite` datetime DEFAULT NULL,'.
                '`stars` INT(10) DEFAULT 0,'.
                '`domain_id` INT(20) DEFAULT 0,'.
                'PRIMARY KEY (`id`,`locale`)'.
            ')ENGINE = InnoDB DEFAULT CHARSET='.\autosystem\DBConn::getCHARSET().' COLLATE='.\autosystem\DBConn::getCOLLATE().' ;';
            $result5 = $database->query($sql);
            $return5 = $result5->rowCount()==1;
        }else{
            $return5 = true;
        }
        //IPv6 (39 bytes) :              ABCD:ABCD:ABCD:ABCD:ABCD:ABCD:ABCD:ABCD
        //IPv4-mapped IPv6 (45 bytes) :(chars46)  ABCD:ABCD:ABCD:ABCD:ABCD:ABCD:192.168.158.190
        if(!$this->existTableSearchSites()){
            $sql = 'CREATE TABLE IF NOT EXISTS `'.\autosystem\DBConn::getTableNameFor('search_sites').'`('.
                '`id` INT(20) NOT NULL AUTO_INCREMENT,'.
                '`title` varchar(230) COLLATE '.\autosystem\DBConn::getCOLLATE().' NOT NULL,'.
                '`shortcomment` varchar(1000) COLLATE '.\autosystem\DBConn::getCOLLATE().' NOT NULL,'.
                '`accoundowner_id` INT(50) ZEROFILL NOT NULL,'.
                '`accoundinput_id` INT(50) ZEROFILL NOT NULL,'.
                '`location_id` INT(50) DEFAULT 0,'.
                '`category_id` INT(4) NOT NULL,'.
                '`STATUS` ENUM(\''.implode('\',\'',array_keys($this->states)).'\')NOT NULL ,'.
                '`ENABLED` ENUM(\'TRUE\',\'FALSE\')NOT NULL DEFAULT \'TRUE\','.
                '`imputDate` datetime NOT NULL,'.
                '`domain` varchar(230) COLLATE '.\autosystem\DBConn::getCOLLATE().' NOT NULL,'.
                '`ip` varchar(50) COLLATE '.\autosystem\DBConn::getCOLLATE().' NOT NULL,'.
                '`stars` INT(4) DEFAULT 0,'.
                'PRIMARY KEY (`id`)'.
            ')ENGINE = InnoDB DEFAULT CHARSET='.\autosystem\DBConn::getCHARSET().' COLLATE='.\autosystem\DBConn::getCOLLATE().' ;';
            $result6 = $database->query($sql);
            $return6 = $result6->rowCount()==1;
        }else{
            $return6 = true;
        }
        if(!$this->existTableSearchterminfo()){
            $sql = 'CREATE TABLE IF NOT EXISTS `'.\autosystem\DBConn::getTableNameFor('search_terminfo').'`('.
                '`term_id` INT(20) NOT NULL,'.
                '`locale` varchar(5) COLLATE '.\autosystem\DBConn::getCOLLATE().' NOT NULL,'.
                '`info` TEXT COLLATE '.\autosystem\DBConn::getCOLLATE().' NOT NULL,'.
                'PRIMARY KEY (`locale`,`term_id`)'.
            ')ENGINE = InnoDB DEFAULT CHARSET='.\autosystem\DBConn::getCHARSET().' COLLATE='.\autosystem\DBConn::getCOLLATE().' ;';
            $result7 = $database->query($sql);
            $return7 = $result7->rowCount()==1;
        }else{
            $return7 = true;
        }
        if(!$this->existTableSearchterminfo()){
            $sql = 'CREATE TABLE IF NOT EXISTS `'.\autosystem\DBConn::getTableNameFor('search_excludes').'`('.
                '`id` INT(20) NOT NULL AUTO_INCREMENT,'.
                '`regex` INT(20) NOT NULL,'.
                '`info` TEXT COLLATE '.\autosystem\DBConn::getCOLLATE().' NOT NULL,'.
                'PRIMARY KEY (`locale`,`term_id`)'.
            ')ENGINE = InnoDB DEFAULT CHARSET='.\autosystem\DBConn::getCHARSET().' COLLATE='.\autosystem\DBConn::getCOLLATE().' ;';
            $result7 = $database->query($sql);
            $return7 = $result7->rowCount()==1;
        }else{
            $return7 = true;
        }
        return  $return1 && $return2 && $return3 && $return4 && $return5 && $return6 && $return7;
    }
    public function isInstalled(){
        return $this->isinstalled||
        $this->existTableSearchterm()&&
        $this->existTableSearchTermsDetail()&&
        $this->existTableSearchTermsPages()&&
        $this->existTableSearchPages()&&
        $this->existTableSearchPagesAccess()&&
        $this->existTableSearchSites()&&
        $this->existTableSearchterminfo()&&
        $this->isinstalled=true;
    }
    public function existTableSearchterm(){        return $this->existTableLabel('search_term');      }
    public function existTableSearchTermsDetail(){ return $this->existTableLabel('search_term_detail');}
    public function existTableSearchTermsPages(){  return $this->existTableLabel('search_terms_pages');}
    public function existTableSearchPagesAccess(){ return $this->existTableLabel('search_pages_access');}
    public function existTableSearchPages(){       return $this->existTableLabel('search_pages');    }
    public function existTableSearchModuleAccess(){return $this->existTableLabel('search_pages');    }
    public function existTableSearchSites(){       return $this->existTableLabel('search_sites');    }
    public function existTableSearchterminfo(){    return $this->existTableLabel('search_terminfo'); }
    public function checkinstall(){
        
    }
    public function getModelInfo(){        return 'search.term.model.info';    }
    public function getVersionNr(){        return (float)4.7;    }
    public function __upgrade($version){         return true;  }
    public function GET_Site($location,$templatename,$view,$id,$accound_id=''){
        $t = $this->existTableLabel('search_term');
        $s = $this->existTableLabel('search_sites');
        $sql='SELECT * FROM `'.$t.'` LEFT JOIN `'.$s.'` ON `'.$t.'`.`id`=`'.$i.'`.`term_id` WHERE `'.$a.'`.`id`=\''.$id.'\' ' ;
        return parent::GET($location,$templatename,$view,$sql,null,true,$accound_id);
    }
    public function existTerm($locale,$term){
        if(!$this->isInstalled()){return false;}
        $t = \autosystem\DBConn::getTableNameFor('search_term');
        $sql ='SELECT `id` FROM `'.$t.'` WHERE `locale`=:locale AND `term`=:term ';
        $value = [':locale'=>$locale,':term'=>$term];
        return $this->isQuery($sql,$value);
    }
    public function existTermALL($term){
        if(!$this->isInstalled()){return false;}
        $t = \autosystem\DBConn::getTableNameFor('search_term');
        $sql ='SELECT `id` FROM `'.$t.'` WHERE `term`=:term ';
        $value = [':term'=>$term];
        return $this->isQuery($sql,$value);
    }
    public function registrateTerm($locale,$term){
        if(!$this->isInstalled()){return false;}
        $this->saveTerm($locale,$term);
    }
    public function IsUsedTerm($locale,$term){
        if(!$this->isInstalled()){return false;}
        $t = \autosystem\DBConn::getTableNameFor('search_term');
        $sql ='SELECT `id` FROM `'.$t.'` WHERE `locale`=:locale AND `term`=:term AND `CHECKED`=\'TRUE\' AND `ENABLED`=\'FALSE\' ';
        $value = [':locale'=>$locale,':term'=>$term];
        return $this->isQuery($sql,$value);
    }
    public function Delete_Aterm($locale,$term){
       $wcp= \autosystem\DBConn::getTableNameFor('search_term');
       $key ='DELETE FROM `'.$wcp.'` WHERE `locale`=:locale AND `term`=:term';
       $value = [':locale'=>$locale,':term'=>$term];
       $sql[$key]= $value;
       return $this->transaction($sql);
   }
   public function Delete_Unusedterms($locale,$term){
       $t= \autosystem\DBConn::getTableNameFor('search_term');
       $p= \autosystem\DBConn::getTableNameFor('search_terms_pages');
       $key ='DELETE `'.$p.'` FROM `'.$p.'` LEFT JOIN `'.$t.'` ON `'.$t.'`.`id`=`'.$p.'`.`term_id` WHERE `term` IS NULL OR `term`=:term AND `CHECKED`=\'TRUE\' AND `ENABLED`=\'FALSE\'';
       /*DELETE `search_terms_pages` FROM `search_terms_pages`
LEFT JOIN `search_term` ON `search_term`.`id` = `search_terms_pages`.`term_id`
WHERE */
       
       $value = [':term'=>$term];
       $sql[$key]= $value;
       $key ='DELETE `'.$t.'` FROM `'.$t.'` WHERE `term`=:term AND `CHECKED`=\'TRUE\' AND `ENABLED`=\'FALSE\'';
       $value = [':term'=>$term];
       $sql[$key]= $value;
       return $this->transaction($sql);
   }
    public function hasModuleSearch($action,$location){
        //HASMODULESEARCH
        return false;
    }
    private function GET_Thermlist($startwith){
        $t = \autosystem\DBConn::getTableNameFor('search_term');
        $sql='SELECT `id` AS \'key\' ,`term` AS \'value\' FROM `'.$t.'` WHERE `term` LIKE \''.$startwith.'%\' AND `CHECKED`=\'TRUE\' GROUP BY `term`' ;
        //print $sql;
        return parent::GET_Array($sql);
    }
    public function Acomplete($startwith){
        $return['Athermlist']=$this->GET_Thermlist($startwith);
        return $return;
    }
    /*public function AcompleteTerm($startwith){
        $return['Athermlist']=$this->GET_Thermlist($startwith);
        return $return;
    }*/
    protected function saveTerm($locale,$term){
        //if($term->existTerm($locale,$aterm)){return false;}
        $ip = parent::getIp();
        $t = \autosystem\DBConn::getTableNameFor('search_term');
        $key ='INSERT INTO `'.$t.'`(`id`,`locale`,`term`) VALUES(NULL,:locale,:term) RETURNING `term_id`';
        $value = [':locale'=>$locale,':term'=>$term];
        $sql[$key]= $value;
        $d = \autosystem\DBConn::getTableNameFor('search_term_detail');
        $key ='INSERT INTO `'.$d.'`(`term_id`,`imputDate`,`ip`) VALUES(:term_id,:imputDate,:ip)';
        $value = [':term_id'=>'',':imputDate'=>$this->nowDateTime(),':ip'=>$ip];
        $sql[$key]= $value;       
        return $this->transaction($sql);
    }
    public function saveUrl($locale,$url,$isIntern){
        $INTERN = ($isIntern=='true')?'TRUE':'FALSE';
        $p = \autosystem\DBConn::getTableNameFor('search_pages');
        $key ='INSERT INTO `'.$p.'`(`id`,`locale`,`url`,`INTERNAL`) VALUES(NULL,:locale,:url,:in) RETURNING `page_id`';
        $value = [':locale'=>$locale,':url'=>$url,':in'=>$INTERN];
        $sql[$key]= $value;     
        return $this->transaction($sql);
    }
    public function existUrl($locale,$url){
        $p = \autosystem\DBConn::getTableNameFor('search_pages');
        $sql ='SELECT `id` FROM `'.$p.'` WHERE `locale`=:locale AND `url`=:url';
        $value = [':locale'=>$locale,':url'=>$url];
        return $this->isQuery($sql,$value);
    }
    public function hasUrlTerm($url,$term){
        $tp = \autosystem\DBConn::getTableNameFor('search_terms_pages');
        $p = \autosystem\DBConn::getTableNameFor('search_pages');
        $t = \autosystem\DBConn::getTableNameFor('search_term');
        $sql ='SELECT `'.$tp.'`.`term_id`,`'.$tp.'`.`page_id` FROM `'.$tp.'` '.
               'LEFT JOIN `'.$t.'` ON `'.$tp.'`.`term_id` = `'.$t.'`.`id` '.
               'LEFT JOIN `'.$p.'` ON `'.$tp.'`.`page_id` = `'.$p.'`.`id`';
        $sql .=' WHERE `term`=:term AND `url`=:url';
        print \autosystem\DEBUG::printTransaction($sql,$value);
        $value = [':term'=>$term,':url'=>$url];
        return $this->isQuery($sql,$value);
         /*
        'CREATE TABLE IF NOT EXISTS `'.\autosystem\DBConn::getTableNameFor('search_terms_pages').'`('.
                '`term_id` INT(20) NOT NULL,'.
                '`page_id` INT(20) NOT NULL,'.
                '`incontent` ENUM(\'TRUE\',\'FALSE\')NOT NULL DEFAULT \'FALSE\','.
                '`inkeywords` ENUM(\'TRUE\',\'FALSE\')NOT NULL DEFAULT \'FALSE\','.
                '`intitleH` ENUM(\'TRUE\',\'FALSE\')NOT NULL DEFAULT \'FALSE\','.
                '`amount` INT(3) DEFAULT 0,'.
                'PRIMARY KEY (`term_id`,`page_id`)'.
            ')ENGINE = InnoDB DEFAULT CHARSET='.\autosystem\DBConn::getCHARSET().' COLLATE='.\autosystem\DBConn::getCOLLATE().' ;';
        */
        return true;
    }
    public function saveUrlTerm($url,$term,$locale){
        //print "<br><br>SaveURLTerm $url,$term";
        $t = \autosystem\DBConn::getTableNameFor('search_term');
        $key = 'SELECT `term` AS \'key\', `id` AS \'value\' FROM `'.$t.'` WHERE `term`=:term';
        $arr = parent::GET_Array($key,[':term'=>$term]);
        //print_r($arr);
        $term_id = 0;
        if(key_exists($term,$arr))
        $term_id =$arr[$term];
        if(key_exists(strtolower($term),$arr));
        $term_id = $arr[strtolower($term)];
        if($term_id == 0)return -1;
        $p = \autosystem\DBConn::getTableNameFor('search_pages');
        $key = 'SELECT `url` AS \'key\',`id` AS \'value\' FROM `'.$p.'` WHERE `url`=:url';
        if(!empty($locale)){
             $key .= 'AND `locale`=:locale ' ;
             $arr = parent::GET_Array($key,[':url'=>$url,':locale'=>$locale]);
        }else{
             $arr = parent::GET_Array($key,[':url'=>$url]);
        }
        if(!key_exists($url,$arr))return -2;
        $page_id = $arr[$url];
        $tp = \autosystem\DBConn::getTableNameFor('search_terms_pages');
        $key ='INSERT INTO `'.$tp.'`(`term_id`,`page_id`) VALUES(:term_id,:page_id)';
        $value = [':term_id'=>$term_id,':page_id'=>$page_id];
        $sql[$key]= $value;
        return $this->transaction($sql);
    }
    private function getIdsUrlTerm($url,$term){
        
    }
    public function Seachedcount($locale,$term){
        if(!$this->isInstalled()){return false;}
        $p = \autosystem\DBConn::getTableNameFor('search_term');
        $key = 'UPDATE '.$p.' SET `search_stars` = `search_stars` + 1 WHERE `locale`=:locale AND `term`=:term ';
        $value = [':locale'=>$locale,':term'=>$term];
        $sql[$key]= $value;       
        return $this->transaction($sql);
    }
    protected function GET_SQL($enabled,$idlist=null){
        $t = \autosystem\DBConn::getTableNameFor('search_term');
        $s = \autosystem\DBConn::getTableNameFor('search_sites');
        $sql='SELECT * FROM `'.$t.'` LEFT JOIN `'.$s.'` ON `'.$t.'`.`id`=`'.$i.'`.`term_id` ';
        $link = 'WHERE';
        if(!empty($enabled)&&$enabled==true){
            $where.= $link.' `'.$a.'`.`ENABLED`=\'TRUE\'';
            $link =' &&'; 
        }
        if(!empty($idlist)){
            if(is_array($idlist)){
                $where.= $link.'(';
                $link = '';
                foreach($idlist as $id){
                    if(is_numeric($id)){
                        $where.= $link.' `'.$a.'`.`id`=\''.$id.'\'';
                        $link =' OR ';
                    }
                }
                $where.= ')' ;
                $link =' && ';
            }else{
                $where.= $link.' `'.$a.'`.`id`=\''.$idlist.'\'';
                $link =' && '; 
            }
        }
        $sql.= $where;
        if(!$this->viewtaged){
             $sql.= $link.'(`'.$a.'`.`TAG`=\'NULL\' OR `'.$a.'`.`TAG` =\'\')';
        }
        $this->paginator->setSort(' ORDER BY `stars`,`view_stars`,`search_stars` DESC ');
        $this->paginator->setRange(13);
        return $sql;
    }
    public function FIND($location,$name,$view,$term,$in,$accound_id){
        /*$a = \autosystem\DBConn::getTableNameFor('auction');
        $sql = 'SELECT `id` as `key`, `STATUS` as `value` FROM `'.$a.'`'.' WHERE `'.$in.'` LIKE \'%'.$term.'%\'  AND `ENABLED`=\'TRUE\'';
        $array = parent::GET_Array($sql);
        $sql = $this->GET_SQL('','','',array_keys($array));
        parent::GET_Summary($location,$name,$view,$sql,null,true,$accound_id);*/
    }
    private function get_CountSQL($category_id,$accound_id,$enabled){
        /*$a = \autosystem\DBConn::getTableNameFor('auction');
        $c = \autosystem\DBConn::getTableNameFor('auction_category');
        $sql='SELECT count(*) as `count` FROM `'.$a.'` LEFT JOIN `'.$c.'` ON  `'.$a.'`.`category_id`= `'.$c.'`.`id`';
        $link =' WHERE';
        $where='';
        if(!empty($category_id)){
            $where.= $link.' (`'.$a.'`.`category_id`=\''.$category_id.'\'';
            $where.= ' OR `'.$c.'`.`category_id`=\''.$category_id.'\' )' ;
            $link =' && ';    
        }
        if(!empty($accound_id)){
            $where.= $link.' `'.$a.'`.`accound_id`=\''.$accound_id.'\'';
            $link =' && ';
        }
        if(!empty($enabled)&&$enabled==true){
            $where.= $link.' `'.$a.'`.`ENABLED`=\'TRUE\'';
            $link =' && '; 
        }
        $sql.= $where;
        return $sql;*/
    }
    public function GET_AOverView($location,$templatename,$view,$category_id='',$accound_id='',$enabled = null,$ids='',$pagenr=0){ 
        $sql= $this->GET_SQL($category_id,$accound_id,$enabled,$ids);
        $len =$this->GET_COUNT($this->get_CountSQL($category_id,$accound_id,$enabled));
        $this->paginator->setlistlength($len);
        $this->paginator->setpagenr($pagenr);
        $this->paginator->setpaginatorreload($location,$templatename);
        parent::GET_Summary($location,$templatename,$view,$sql,null,true,$accound_id);
        $view->getpaginator($this->paginator->getdata());
    }
    public function GET_Locations($accound_id,$mustvisible=true){
        /*$visible = ($mustvisible)?'TRUE':'FALSE';
        $d =\autosystem\DBConn::getTableNameFor('adres_detail');
        $a =\autosystem\DBConn::getTableNameFor('adres_accound');
        $l =\autosystem\DBConn::getTableNameFor('adres_detail');
        $sql ='SELECT id as \'key\' , CONCAT_WS(\' - \',`postcode`,`street`,`houseNumber`) as \'value\' FROM `'.$a.'`'.
        ' LEFT JOIN `'.$d.'` ON `'.$a.'`.`adresdetail_id` =`'.$d.'`.`id` and `'.$a.'`.`accound_id` ='.$accound_id.' WHERE `'.$a.'`.`VISIBLE` =\''.$visible.'\'' ;
        return parent::GET_Array($sql);*/
    }
    public function getRuleDefault(){
        return 'PUBLIC';
    }
    public function setReportTherm($id,$tag,$msg,$url,$location,$accound_id=0,$location_id=0){
        /*$key =' UPDATE '.\autosystem\DBConn::getTableNameFor('auction').' SET `TAG`=:tag WHERE `id`=:id ';
        $sql[$key]= [':id'=>$id,':tag'=>$tag];
        $ticket = new \support\Ticket();
        $nr = $ticket->create($id,$id.'error:'.$tag,'',$msg,$url,$location,$accound_id,$location_id);
        return $this->transaction($sql);*/
    }
    private function canReportItem($accound_id){
        return true;
    }
    private function canSetTAG($accound_id){
        return true;
    }
    public function existcach($location,$name,$additionaldir=''){
       $lang = $this->properties->getlocale();
       return file_exists($this->cashdir.''.$lang.'/'.$additionaldir.$location.'_'.$name.'_'.$lang.'.cache');
    }
    public function makecach($location,$name,$html,$additionaldir=''){
        $lang = $this->properties->getlocale();
        if($this->makenotexistlangcachdir($lang)){
            return file_put_contents($this->cashdir.''.$lang.'/'.$additionaldir.$location.'_'.$name.'_'.$lang.'.cache',$html);
        }
        return false;
    }
    public function getcach($location,$name,$additionaldir=''){
        $lang = $this->properties->getlocale();
        return file_get_contents($this->cashdir.''.$lang.'/'.$additionaldir.$location.'_'.$name.'_'.$lang.'.cache');
    }
    private function makenotexistlangcachdir($lang){
        if(!is_dir($this->cashdir.''.$lang.'/')){
           return mkdir($this->cashdir.''.$lang.'/', 0755);
        }else{
           return true;
        }
    }
    private function iscachSelect($location,$name){
        return true;
    }
}
?>