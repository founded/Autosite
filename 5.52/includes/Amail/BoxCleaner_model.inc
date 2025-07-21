<?php
/* model*/
namespace Amail;
class BoxCleaner_model extends \autosystem\Model{
  protected $isinstalled = false;
  public $filtertypes = ['IP'=>'ip','REGEX' => 'regex','DOMAIN' => 'domain','EMAIL' => 'email','STRING' => 'string'];
  public $filterlocations = ['BODY'=>'body','HEADER' => 'header','GLOBAL' => 'global','ATTACHMENT' => 'attachment','MIME' => 'mime'];
  public function __install(){
  	//$installed = parent::__install();
    if(!$this->existTableAmailboxFilter()){
     $sql = 'CREATE TABLE IF NOT EXISTS `'.\autosystem\DBConn::getTableNameFor('amailbox_filter').'`('.
       '`id` INT (7) NOT NULL AUTO_INCREMENT,'.
       '`filtervalue` VARCHAR (255) COLLATE '.\autosystem\DBConn::getCOLLATE().' NOT NULL,'.
       '`REMOVE` ENUM(\'TRUE\',\'FALSE\')NOT NULL DEFAULT \'FALSE\','.
       '`TYPE` VARCHAR (60) COLLATE '.\autosystem\DBConn::getCOLLATE().' NOT NULL,'.
       '`LOCATION` VARCHAR (60) COLLATE '.\autosystem\DBConn::getCOLLATE().' NOT NULL,'.
       '`counter` INT (7),'.
       '`cattegory_id` INT (3),'.
       '`delaydays` INT (6),'.
      ' PRIMARY KEY ( `id` )'.
     ')ENGINE = InnoDB DEFAULT CHARSET='.\autosystem\DBConn::getCHARSET().' COLLATE='.\autosystem\DBConn::getCOLLATE().' ;';
     $result1 = $this->DB->query($sql);
     $return1 = $result1->rowCount()==1;
    }else{
     $return1 = true;
    }

    if(!$this->existTableAmailboxAccound()){
     $sql = 'CREATE TABLE IF NOT EXISTS `'.\autosystem\DBConn::getTableNameFor('amailbox_accound').'`('.
       '`id` INT (7) NOT NULL AUTO_INCREMENT,'.
       '`email` VARCHAR (255) NOT NULL,'.
       '`CLEANON` ENUM(\'TRUE\',\'FALSE\')NOT NULL DEFAULT \'FALSE\','.
       '`AUTOCLEAN` ENUM(\'TRUE\',\'FALSE\')NOT NULL DEFAULT \'FALSE\','.
       '`accound_id` INT (20)NOT NULL,'.
      ' PRIMARY KEY ( `id` )'.
     ')ENGINE = InnoDB DEFAULT CHARSET='.\autosystem\DBConn::getCHARSET().' COLLATE='.\autosystem\DBConn::getCOLLATE().' ;';
     $result2 = $this->DB->query($sql);
     $return2 = $result2->rowCount()==1;
    }else{
     $return2 = true;
    }

    if(!$this->existTableAmailboxBindFilter()){
     $sql = 'CREATE TABLE IF NOT EXISTS `'.\autosystem\DBConn::getTableNameFor('amailbox_bind_filter').'`('.
       '`id` INT (15) NOT NULL AUTO_INCREMENT,'.
       '`amailbox_id` INT (7),'.
       '`amailfilter_id` INT (6),'.
       '`type` VARCHAR (100) COLLATE '.\autosystem\DBConn::getCOLLATE().' NOT NULL,'.
       '`PLACE` ENUM(\''.'\')NOT NULL DEFAULT \'\','.//TODO check DEFAULT parameter
       '`DECODE` ENUM(\'TRUE\',\'FALSE\')NOT NULL DEFAULT \'FALSE\','.
       '`ENABLED` ENUM(\'TRUE\',\'FALSE\')NOT NULL DEFAULT \'FALSE\','.
      ' PRIMARY KEY ( `id` )'.
     ')ENGINE = InnoDB DEFAULT CHARSET='.\autosystem\DBConn::getCHARSET().' COLLATE='.\autosystem\DBConn::getCOLLATE().' ;';
     $result3 = $this->DB->query($sql);
     $return3 = $result3->rowCount()==1;
    }else{
     $return3 = true;
    }

    if(!$this->existTableAmailboxLog()){
     $sql = 'CREATE TABLE IF NOT EXISTS `'.\autosystem\DBConn::getTableNameFor('amailbox_log').'`('.
       '`id` INT(7) NOT NULL AUTO_INCREMENT,'.
       '`info` VARCHAR (5000) COLLATE '.\autosystem\DBConn::getCOLLATE().' NOT NULL,'.
       '`amailbox_acound_id` INT(20),'.
       '`importantlevel` INT(3),'.
      ' PRIMARY KEY(`id`)'.
     ')ENGINE = InnoDB DEFAULT CHARSET='.\autosystem\DBConn::getCHARSET().' COLLATE='.\autosystem\DBConn::getCOLLATE().' ;';
     $result4 = $this->DB->query($sql);
     $return4 = $result4->rowCount()==1;
    }else{
     $return4 = true;
    }
   return $return1  && $return2  && $return3  && $return4  ; 
  }
  // isset()&&
  public function isInstalled(){
	return $this->isinstalled|| $this->existTableAmailboxFilter()&& $this->existTableAmailboxAccound()&& $this->existTableAmailboxBindFilter()&& $this->existTableAmailboxLog()&& $this->isinstalled=true;
  }
 /*
 * options 
*/
  public function InstallData($sqlfile=''){

		
  }	
	public function save_BoxCleanderAmailboxfilter($filtervalue,$REMOVE,$TYPE,$LOCATION,$counter,$cattegory_id,$delaydays){ 
       $is_REMOVE= ( $isREMOVE)?'TRUE':'FALSE';
       $af= \autosystem\DBConn::getTableNameFor('amailbox_filter');
       $key ='INSERT INTO `'.$af.'`(`id`,`filtervalue`,`REMOVE`,`TYPE`,`LOCATION`,`counter`,`cattegory_id`,`delaydays`) '.
                            'VALUES(NULL, :filtervalue, :REMOVE, :TYPE, :LOCATION, :counter, :cattegory_id, :delaydays) RETURNING amailbox_filter_id';
       $sql[$key] = [':filtervalue'=>$filtervalue,':REMOVE'=>$isREMOVE,':TYPE'=>$TYPE,':LOCATION'=>$LOCATION,':counter'=>$counter,':cattegory_id'=>$cattegory_id,':delaydays'=>$delaydays];//TODO CHECK insert save_BoxCleanderAmailboxfilter
       return $this->transaction($sql);
    }
    public function update_BoxCleanderAmailboxfilter($id,$filtervalue,$REMOVE,$TYPE,$LOCATION,$counter,$cattegory_id,$delaydays){ 
       $is_REMOVE= ( $isREMOVE)?'TRUE':'FALSE';
       $af= \autosystem\DBConn::getTableNameFor('amailbox_filter');
       $key ='UPDATE `'.$af.'` SET  `filtervalue`=:filtervalue,`REMOVE`=:REMOVE,`TYPE`=:TYPE,`LOCATION`=:LOCATION,`counter`=:counter,`cattegory_id`=:cattegory_id,`delaydays`=:delaydays` WHERE id=:id';
       
       $sql[$key]= [':id'=>$id,':filtervalue'=>$filtervalue,':REMOVE'=>$isREMOVE,':TYPE'=>$TYPE,':LOCATION'=>$LOCATION,':counter'=>$counter,':cattegory_id'=>$cattegory_id,':delaydays'=>$delaydays];//TODO CHECK update update_BoxCleanderAmailboxfilter
       return is_array($this->transaction($sql));      
    }
    public function delete_BoxCleanderAmailboxfilter($id){
       $af= \autosystem\DBConn::getTableNameFor('amailbox_filter');
       $key ='DELETE FROM `'.$af.'` WHERE `id`=:id';//
       $value = [':id'=>$id];//TODO check if there need more checks before use delete_BoxCleanderAmailboxfilter
       $sql[$key]= $value;
       return $this->transaction($sql);
    }
    public function exist_BoxCleanderAmailboxfilter($id){
       $af= \autosystem\DBConn::getTableNameFor('amailbox_filter');
       $key ='SELECT * FROM `'.$af.'` WHERE `id`=:id ;';//
       $value = [':id'=>$id];//TODO check if there are more parameters to use exist_BoxCleanderAmailboxfilter
       return $this->isQuery($key,$value);
    }
    public function GET_amailbox_filter($location,$templatename,$view,$accountid,$id){
       $af= \autosystem\DBConn::getTableNameFor('amailbox_filter');
       $sql='SELECT `'.$af.'`.* FROM `'.$af.'` ';
        $where = '';//
        $link ='WHERE';
    }
    private function GET_amailbox_filterSQL($accountid,$mailbox,$reverse){//
       $af= \autosystem\DBConn::getTableNameFor('amailbox_filter');
       $abf= \autosystem\DBConn::getTableNameFor('amailbox_bind_filter');
       $aa= \autosystem\DBConn::getTableNameFor('amailbox_accound');
       $sql='SELECT `'.$af.'`.* ,IF(`'.$af.'`.`REMOVE`=\'TRUE\',\' checked=checked \',\' \') AS `isREMOVE`';
       if($reverse){
       		$sql.=',IF(`'.$af.'`.`id` IS NOT NULL,\' \',\' \') AS `isbind`'; //uncheck not empty
       	    $sql.=' FROM `'.$af.'`';
			$where = 'WHERE NOT exists';//TODO check it out
			$where .= '(SELECT * FROM `'.$abf.'` AS a LEFT JOIN `'.$aa.'` ON `a`.`amailbox_id` = `'.$aa.'`.`id` WHERE `a`.`amailfilter_id`=`'.$af.'`.`id` AND `'.$aa.'`.`email`=\''.$mailbox.'\')';//TODO check it out  
	   }else{
	   		$sql.=',IF(`'.$aa.'`.`email` IS NOT NULL,\' checked=checked \',\' \') AS `isbind`';
	   		$sql.=',`'.$abf.'`.*';
	   		$sql.=',`'.$aa.'`.* FROM `'.$af.'` ';
	   	    $sql.='LEFT JOIN `'.$abf.'` ON `'.$abf.'`.`amailfilter_id` = `'.$af.'`.`id`';
       		$sql.='RIGHT JOIN `'.$aa.'` ON `'.$abf.'`.`amailbox_id` = `'.$aa.'`.`id`';
			$where = 'WHERE `'.$aa.'`.`accound_id`=\''.$accountid.'\'';
			$where .='AND `'.$aa.'`.`email`=\''.$mailbox.'\'';//TODO check it out
	   }   
       $sql.= $where;       
       $this->paginator->setSort(' GROUP BY `'.$af.'`.`id`');
       $this->paginator->setRange(20);
       return $sql;
    }
    public function GET_filters($location,$templatename,$view,$accound_id,$mailbox,$pagenr,$reverse=false){// TODO: check params
        $sql= $this->GET_amailbox_filterSQL($accound_id,$mailbox,$reverse);
        $len =$this->GET_COUNT($this->get_amailbox_filterCountSQL($accound_id,$mailbox,$reverse));
        $this->paginator->setlistlength($len);
        $this->paginator->setpagenr($pagenr);
        $this->paginator->setpaginatorreload($location,$templatename);
        parent::GET_Summary($location,$templatename,$view,$sql,null,true,$accound_id);
        $view->getpaginator($this->paginator->getdata(),'&filters&mailbox='.$mailbox);
    }
    private function get_amailbox_filterCountSQL($accound_id,$accountid,$id){
       $af= \autosystem\DBConn::getTableNameFor('amailbox_filter');
       $abf= \autosystem\DBConn::getTableNameFor('amailbox_bind_filter');
       $aa= \autosystem\DBConn::getTableNameFor('amailbox_accound');
       $sql='SELECT `'.$af.'`.* FROM `'.$af.'` ';
       $sql.='LEFT JOIN `'.$abf.'` ON `'.$abf.'`.`amailfilter_id` = `'.$af.'`.`id`';
       $sql.='LEFT JOIN `'.$aa.'` ON `'.$abf.'`.`amailfilter_id` = `'.$abf.'`.`id`';
       if($reverse){
			$where = 'WHERE `'.$aa.'`.`email`!=\''.$mailbox.'\'';
        	$where .='OR `amailbox_accound`.`email` IS NULL ';//TODO check it out
	   }else{
			$where = 'WHERE `'.$aa.'`.`accound_id`=\''.$accountid.'\'';
			$where .='AND `'.$aa.'`.`email`=\''.$mailbox.'\'';//TODO check it out
	   }
       return $sql;
    }
    public function add_mailbox($email,$accound_id,$isONCLEAN=false){
       $is_ONCLEAN= ( $isONCLEAN)?'TRUE':'FALSE';
       $ab= \autosystem\DBConn::getTableNameFor('amail_box');
       $key ='INSERT INTO `'.$ab.'`(`id`,`email`,`accound_id`) '.
                            'VALUES(NULL, :email, :accound_id) RETURNING amail_box_id';
       $value = [':email'=>$email,':accound_id'=>$accound_id];
       $sql[$key]=$value;
       return $this->transaction($sql);
    }
    public function update_BoxCleanderAmailboxaccound($id,$email,$CLEANON,$AUTOCLEAN,$accound_id){ 
       $is_AUTOCLEAN= ( $isAUTOCLEAN)?'TRUE':'FALSE';
       $aa= \autosystem\DBConn::getTableNameFor('amailbox_accound');
       $key ='UPDATE `'.$aa.'` SET  `id`=:id,`email`=:email,`CLEANON`=:CLEANON,`AUTOCLEAN`=:AUTOCLEAN,`accound_id`=:accound_id` WHERE id=:id';
       
       $sql[$key]= [':id'=>$id,':id'=>$id,':email'=>$email,':CLEANON'=>$isCLEANON,':AUTOCLEAN'=>$isAUTOCLEAN,':accound_id'=>$accound_id];//TODO CHECK update update_BoxCleanderAmailboxaccound
       return is_array($this->transaction($sql));      
    }
    public function delete_BoxCleanderAmailboxaccound($id){
       $aa= \autosystem\DBConn::getTableNameFor('amailbox_accound');
       $key ='DELETE FROM `'.$aa.'` WHERE `id`=:id';//
       $value = [':id'=>$id];//TODO check if there need more checks before use delete_BoxCleanderAmailboxaccound
       $sql[$key]= $value;
       return $this->transaction($sql);
    }
    public function exist_mailbox($email){
       $ab= \autosystem\DBConn::getTableNameFor('amailbox_accound');
       $key ='SELECT * FROM `'.$ab.'` WHERE `email`=:email ;';//
       $value = [':email'=>$email];
       return $this->isQuery($key,$value);
    }
    public function GET_amailbox_accound($location,$templatename,$view,$accountid,$id){
       $aa= \autosystem\DBConn::getTableNameFor('amailbox_accound');
       $sql='SELECT `'.$aa.'`.* FROM `'.$aa.'` ';
        $where = '';//
        $link ='WHERE';
    }
    private function GET_amailbox_accoundSQL($accountid,$id){
       $aa= \autosystem\DBConn::getTableNameFor('amailbox_accound');
       $sql='SELECT `'.$aa.'`.* FROM `'.$aa.'` ';
//' IF(`'.$a.'`.`ENABLED`='TRUE',' checked=checked ',' ') AS `isVisable`,
        $where = '';
        $link ='WHERE';
        if(!empty($accountid)){// TODO: field not exist
            $where.= $link.' `???`.`accound_id`=\''.$accountid.'\';';//TODO check it out
        }
        if(!empty($id)){// TODO: field not exist
            $where.= $link.' `???`.`accound_id`=\''.$id.'\';';//TODO check it out
        }
        $sql.= $where;
        $this->paginator->setSort('  ');//TODO ORDER BY `???`.`inputDate` DESC
        $this->paginator->setRange(20);
        return $sql;
    }
    public function GET_amailbox_accoundsSummary($location,$templatename,$view,$accountid,$id){// TODO: check params
        $sql= $this->GET_amailbox_accoundSQL($accound_id,$accountid,$id);
        $len =$this->GET_COUNT($this->get_amailbox_accoundCountSQL($accound_id,$accountid,$id));
        $this->paginator->setlistlength($len);
        $this->paginator->setpagenr($pagenr);
        $this->paginator->setpaginatorreload($location,$templatename);
        // TODO: check next line and uncomment
        //parent::GET_Summary($location,$templatename,$view,$sql,null,true,$accound_id);
        $view->getpaginator($this->paginator->getdata());
        print 'NOT IMPLEMENTED';
    }
    private function get_amailbox_accoundCountSQL($accound_id,$accountid,$id){
       $aa= \autosystem\DBConn::getTableNameFor('amailbox_accound');
       $sql='SELECT COUNT(*) AS `count`FROM `'.$aa.'` ';
       $sql.= $where;
       return $sql;
    }
    public function save_BoxCleanderAmailboxbindfilter($amailbox_id,$amailfilter_id,$type,$PLACE,$DECODE,$ENABLED){ 
       $is_ENABLED= ( $isENABLED)?'TRUE':'FALSE';
       $abf= \autosystem\DBConn::getTableNameFor('amailbox_bind_filter');
       $key ='INSERT INTO `'.$abf.'`(`id`,`amailbox_id`,`amailfilter_id`,`type`,`PLACE`,`DECODE`,`ENABLED`) '.
                            'VALUES(NULL, :amailbox_id, :amailfilter_id, :type, :PLACE, :DECODE, :ENABLED) RETURNING amailbox_bind_filter_id';
       $sql[$key] = [':amailbox_id'=>$amailbox_id,':amailfilter_id'=>$amailfilter_id,':type'=>$type,':PLACE'=>$PLACE,':DECODE'=>$isDECODE,':ENABLED'=>$isENABLED];//TODO CHECK insert save_BoxCleanderAmailboxbindfilter
       return $this->transaction($sql);
    }
    public function update_BoxCleanderAmailboxbindfilter($id,$amailbox_id,$amailfilter_id,$type,$PLACE,$DECODE,$ENABLED){ 
       $is_ENABLED= ( $isENABLED)?'TRUE':'FALSE';
       $abf= \autosystem\DBConn::getTableNameFor('amailbox_bind_filter');
       $key ='UPDATE `'.$abf.'` SET  `amailbox_id`=:amailbox_id,`amailfilter_id`=:amailfilter_id,`type`=:type,`PLACE`=:PLACE,`DECODE`=:DECODE,`ENABLED`=:ENABLED` WHERE id=:id';
       
       $sql[$key]= [':id'=>$id,':amailbox_id'=>$amailbox_id,':amailfilter_id'=>$amailfilter_id,':type'=>$type,':PLACE'=>$PLACE,':DECODE'=>$isDECODE,':ENABLED'=>$isENABLED];//TODO CHECK update update_BoxCleanderAmailboxbindfilter
       return is_array($this->transaction($sql));      
    }
    public function delete_BoxCleanderAmailboxbindfilter($id){
       $abf= \autosystem\DBConn::getTableNameFor('amailbox_bind_filter');
       $key ='DELETE FROM `'.$abf.'` WHERE `id`=:id';//
       $value = [':id'=>$id];//TODO check if there need more checks before use delete_BoxCleanderAmailboxbindfilter
       $sql[$key]= $value;
       return $this->transaction($sql);
    }
    public function exist_BoxCleanderAmailboxbindfilter($id){
       $abf= \autosystem\DBConn::getTableNameFor('amailbox_bind_filter');
       $key ='SELECT * FROM `'.$abf.'` WHERE `id`=:id ;';//
       $value = [':id'=>$id];//TODO check if there are more parameters to use exist_BoxCleanderAmailboxbindfilter
       return $this->isQuery($key,$value);
    }
    public function GET_amailbox_bind_filter($location,$templatename,$view,$accountid,$id){
       $abf= \autosystem\DBConn::getTableNameFor('amailbox_bind_filter');
       $sql='SELECT `'.$abf.'`.* FROM `'.$abf.'` ';
        $where = '';//
        $link ='WHERE';
    }
    private function GET_amailbox_bind_filterSQL($accountid,$id){
       $abf= \autosystem\DBConn::getTableNameFor('amailbox_bind_filter');
       $sql='SELECT `'.$abf.'`.* FROM `'.$abf.'` ';
//' IF(`'.$a.'`.`ENABLED`='TRUE',' checked=checked ',' ') AS `isVisable`,
        $where = '';
        $link ='WHERE';
        if(!empty($accountid)){// TODO: field not exist
            $where.= $link.' `???`.`accound_id`=\''.$accountid.'\';';//TODO check it out
        }
        if(!empty($id)){// TODO: field not exist
            $where.= $link.' `???`.`accound_id`=\''.$id.'\';';//TODO check it out
        }
        $sql.= $where;
        $this->paginator->setSort('  ');//TODO ORDER BY `???`.`inputDate` DESC
        $this->paginator->setRange(20);
        return $sql;
    }
    public function GET_amailbox_bind_filtersSummary($location,$templatename,$view,$accountid,$id){// TODO: check params
        $sql= $this->GET_amailbox_bind_filterSQL($accound_id,$accountid,$id);
        $len =$this->GET_COUNT($this->get_amailbox_bind_filterCountSQL($accound_id,$accountid,$id));
        $this->paginator->setlistlength($len);
        $this->paginator->setpagenr($pagenr);
        $this->paginator->setpaginatorreload($location,$templatename);
        // TODO: check next line and uncomment
        //parent::GET_Summary($location,$templatename,$view,$sql,null,true,$accound_id);
        $view->getpaginator($this->paginator->getdata());
        print 'NOT IMPLEMENTED';
    }
    private function get_amailbox_bind_filterCountSQL($accound_id,$accountid,$id){
       $abf= \autosystem\DBConn::getTableNameFor('amailbox_bind_filter');
       $sql='SELECT COUNT(*) AS `count`FROM `'.$abf.'` ';
       $sql.= $where;
       return $sql;
    }
    public function save_BoxCleanderAmailboxlog($info,$amailbox_acound_id,$importantlevel){ 
       $Al= \autosystem\DBConn::getTableNameFor('amailbox_log');
       $key ='INSERT INTO `'.$Al.'`(`id`,`info`,`amailbox_acound_id`,`importantlevel`) '.
                            'VALUES(NULL, :info, :amailbox_acound_id, :importantlevel) RETURNING amailbox_log_id';
       $sql[$key] = [':info'=>$info,':amailbox_acound_id'=>$amailbox_acound_id,':importantlevel'=>$importantlevel];//TODO CHECK insert save_BoxCleanderAmailboxlog
       return $this->transaction($sql);
    }
    public function update_BoxCleanderAmailboxlog($id,$info,$amailbox_acound_id,$importantlevel){ 
       $Al= \autosystem\DBConn::getTableNameFor('amailbox_log');
       $key ='UPDATE `'.$Al.'` SET  `info`=:info,`amailbox_acound_id`=:amailbox_acound_id,`importantlevel`=:importantlevel` WHERE id=:id';
       
       $sql[$key]= [':id'=>$id,':info'=>$info,':amailbox_acound_id'=>$amailbox_acound_id,':importantlevel'=>$importantlevel];//TODO CHECK update update_BoxCleanderAmailboxlog
       return is_array($this->transaction($sql));      
    }
    public function delete_BoxCleanderAmailboxlog($id){
       $Al= \autosystem\DBConn::getTableNameFor('amailbox_log');
       $key ='DELETE FROM `'.$Al.'` WHERE `id`=:id';//
       $value = [':id'=>$id];//TODO check if there need more checks before use delete_BoxCleanderAmailboxlog
       $sql[$key]= $value;
       return $this->transaction($sql);
    }
    private function GET_Amailbox_logSQL($accountid,$id){
       $Al= \autosystem\DBConn::getTableNameFor('amailbox_log');
       $sql='SELECT `'.$Al.'`.* FROM `'.$Al.'` ';
//' IF(`'.$a.'`.`ENABLED`='TRUE',' checked=checked ',' ') AS `isVisable`,
        $where = '';
        $link ='WHERE';
        if(!empty($accountid)){// TODO: field not exist
            $where.= $link.' `???`.`accound_id`=\''.$accountid.'\';';//TODO check it out
        }
        if(!empty($id)){// TODO: field not exist
            $where.= $link.' `???`.`accound_id`=\''.$id.'\';';//TODO check it out
        }
        $sql.= $where;
        $this->paginator->setSort('  ');//TODO ORDER BY `???`.`inputDate` DESC
        $this->paginator->setRange(20);
        return $sql;
    }
    public function GET_Amailbox_logsSummary($location,$templatename,$view,$accountid,$id){// TODO: check params
        $sql= $this->GET_Amailbox_logSQL($accound_id,$accountid,$id);
        $len =$this->GET_COUNT($this->get_Amailbox_logCountSQL($accound_id,$accountid,$id));
        $this->paginator->setlistlength($len);
        $this->paginator->setpagenr($pagenr);
        $this->paginator->setpaginatorreload($location,$templatename);
        // TODO: check next line and uncomment
        //parent::GET_Summary($location,$templatename,$view,$sql,null,true,$accound_id);
        $view->getpaginator($this->paginator->getdata());
        print 'NOT IMPLEMENTED';
    }
    private function get_Amailbox_logCountSQL($accound_id,$accountid,$id){
       $Al= \autosystem\DBConn::getTableNameFor('amailbox_log');
       $sql='SELECT COUNT(*) AS `count`FROM `'.$Al.'` ';
       $sql.= $where;
       return $sql;
    }
    public function GET_BoxCleander($location,$templatename,$view,$account_id,$id){
       $af= \autosystem\DBConn::getTableNameFor('amailbox_filter');
       $aa= \autosystem\DBConn::getTableNameFor('amailbox_accound');
       $abf= \autosystem\DBConn::getTableNameFor('amailbox_bind_filter');
       $Al= \autosystem\DBConn::getTableNameFor('amailbox_log');
       $sql='SELECT `'.$Al.'`.* ,`'.$abf.'`.* ,`'.$aa.'`.* ,`'.$af.'`.* FROM `'.$af.'` LEFT JOIN `'.$aa.'`.ON .`'.$af.'`.`id=`'.$aa.'`.`amailbox_accound_id`LEFT JOIN `'.$abf.'`.ON .`'.$af.'`.`id=`'.$abf.'`.`amailbox_bind_filter_id`LEFT JOIN `'.$Al.'`.ON .`'.$af.'`.`id=`'.$Al.'`.`amailbox_log_id`';
        $where = '';//
        $link ='WHERE';
    }
    private function GET_SQL($account_id,$id){
       $af= \autosystem\DBConn::getTableNameFor('amailbox_filter');
       $aa= \autosystem\DBConn::getTableNameFor('amailbox_accound');
       $abf= \autosystem\DBConn::getTableNameFor('amailbox_bind_filter');
       $Al= \autosystem\DBConn::getTableNameFor('amailbox_log');
       $sql='SELECT `'.$Al.'`.* ,`'.$abf.'`.* ,`'.$aa.'`.* ,`'.$af.'`.* FROM `'.$af.'` LEFT JOIN `'.$aa.'`.ON .`'.$af.'`.`id=`'.$aa.'`.`amailbox_accound_id`LEFT JOIN `'.$abf.'`.ON .`'.$af.'`.`id=`'.$abf.'`.`amailbox_bind_filter_id`LEFT JOIN `'.$Al.'`.ON .`'.$af.'`.`id=`'.$Al.'`.`amailbox_log_id`';
//' IF(`'.$a.'`.`ENABLED`='TRUE',' checked=checked ',' ') AS `isVisable`,
        $where = '';
        $link ='WHERE';
        if(!empty($account_id)){// TODO: field not exist
            $where.= $link.' `???`.`accound_id`=\''.$account_id.'\';';//TODO check it out
        }
        if(!empty($id)){// TODO: field not exist
            $where.= $link.' `???`.`accound_id`=\''.$id.'\';';//TODO check it out
        }
        $sql.= $where;
        $this->paginator->setSort('  ');//TODO ORDER BY `???`.`inputDate` DESC
        $this->paginator->setRange(20);
        return $sql;
    }
    private function GET_filterSQL($accountid,$box,$type){
       $afr= \autosystem\DBConn::getTableNameFor('amailbox_filter');
       $abbf= \autosystem\DBConn::getTableNameFor('amailbox_bind_filter');
       $ab= \autosystem\DBConn::getTableNameFor('amailbox_accound');
       $sql='SELECT * FROM `'.$afr.'` '.
	   'LEFT JOIN `'.$abbf.'` ON `'.$abbf.'`.`amailfilter_id` = `'.$afr.'`.`id` '.
	   'LEFT JOIN `'.$ab.'` ON `'.$ab.'`.`id` = `'.$abbf.'`.`amailbox_id`';
        $where = ' ';
        $link ='WHERE';
        if(!empty($accountid)){   $where.= $link.' `'.$ab.'`.`accound_id`=\''.$accountid.'\''; }
        $link ='AND';
        if(!empty($box)){   $where.= $link.' `'.$ab.'`.`email`=\''.$box.'\''; }
        $link ='AND';
        if(!empty($type)){          $where.= $link.' `'.$afr.'`.`TYPE`=\''.$type.'\'';      }
        $sql.= $where;
        //print($sql);
        return $sql;
    }
    public function GET_filtersList($accountid,$box,$type){
		$sql= $this->GET_filterSQL($accountid,$box,$type);
		return parent::GET_Array($sql);
	}
    public function GET_BoxCleandersSummary($location,$templatename,$view,$account_id,$id){// TODO: check params
        $sql= $this->GET_SQL($accound_id,$account_id,$id);
        $len =$this->GET_COUNT($this->get_CountSQL($accound_id,$account_id,$id));
        $this->paginator->setlistlength($len);
        $this->paginator->setpagenr($pagenr);
        $this->paginator->setpaginatorreload($location,$templatename);
        // TODO: check next line and uncomment
        //parent::GET_Summary($location,$templatename,$view,$sql,null,true,$accound_id);
        $view->getpaginator($this->paginator->getdata());
        print 'NOT IMPLEMENTED';
    }
    private function get_CountSQL($accound_id,$account_id,$id){
       $af= \autosystem\DBConn::getTableNameFor('amailbox_filter');
       $aa= \autosystem\DBConn::getTableNameFor('amailbox_accound');
       $abf= \autosystem\DBConn::getTableNameFor('amailbox_bind_filter');
       $Al= \autosystem\DBConn::getTableNameFor('amailbox_log');
       $sql='SELECT COUNT(*) AS `count`FROM `'.$af.'` LEFT JOIN `'.$aa.'`.ON .`'.$af.'`.`id=`'.$aa.'`.`amailbox_accound_id`LEFT JOIN `'.$abf.'`.ON .`'.$af.'`.`id=`'.$abf.'`.`amailbox_bind_filter_id`LEFT JOIN `'.$Al.'`.ON .`'.$af.'`.`id=`'.$Al.'`.`amailbox_log_id`';
       $sql.= $where;
       return $sql;
    }
  public function existTableAmailboxFilter(){     return $this->existTableLabel('amailbox_filter');   }
  public function existTableAmailboxAccound(){     return $this->existTableLabel('amailbox_accound');   }
  public function existTableAmailboxBindFilter(){     return $this->existTableLabel('amailbox_bind_filter');   }
  public function existTableAmailboxLog(){     return $this->existTableLabel('amailbox_log');   }
  public function getModelInfo(){  return 'amail.boxcleaner.model.info';  }
  public function __upgrade($version){ return true;  }
  public function getVersionNr(){  return (float)5.4;  }
  public function getRuleDefault(){return 'MODERATOR';} //'PUBLIC','FINANCIAL','TRANSLATE','MANAGE','ADMINISTRATION','KING','USER','MODERATOR','CHAIRMAN' }
}
?>