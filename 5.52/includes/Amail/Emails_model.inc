<?php
/**
* @author Lieven Roegiers
* @copyright 
* @CMS autosite
*/
namespace Amail;
class Emails_model extends \autosystem\Model{
    protected $isinstalled = false;
    public $states = ['PROGRESS'=>'progress','ERROR' => 'error','CLOSED' => 'closed' ,'OPEN' => 'open','CANCELED' => 'canceled'];
    public $contenttypes = ['text/plain'=>'plain','text/html' => 'html','image/gif' => 'logo','text/x-vcard'=>'vcf','text/calendar'=>'ics'];
    public $parttypes = ['PLAIN'=>'plain','HTML' => 'html','B2BCARD'=>'B2BCARD','LOGO' => 'logo','IMAGE' => 'image','ATTACHMENT' => 'ATTACHMENT','NOTAVAILEBLE' => 'nota','AUTHENTIFICATION' => 'authent','NA' => 'notavaileble','INBOUNDIMG' => 'image','vcf'=>'visitecard','ics'=>'afspraakjes'];
    public $sendstates = array('SENDIBLE'=>'sendible','SENDED'=>'sended','ERROR' => 'error','WAIT' => 'wait','CANCELED' => 'canceled');
    protected $mailextendor;
    protected $presendstatus = 'SENDIBLE'; //or WAIT
    protected $antispamdetection = 'autosite-detectorkey'; //or WAIT
    public function __upgrade($version){ 
        if(!$this->existTableAmailContentpartCid()){
             $sql = 'CREATE TABLE IF NOT EXISTS `'.\autosystem\DBConn::getTableNameFor('amail_contentpart_cid').'`('.
               '`id` INT (4) NOT NULL AUTO_INCREMENT,'.
               '`cid` VARCHAR (230) COLLATE '.\autosystem\DBConn::getCOLLATE().' NOT NULL,'.
               '`typeencoding` VARCHAR (230) COLLATE '.\autosystem\DBConn::getCOLLATE().' NOT NULL,'.
               '`amail_contentpart_id` INT(7) NOT NULL,'.
              ' PRIMARY KEY ( `id` )'.
             ')ENGINE = InnoDB DEFAULT CHARSET='.\autosystem\DBConn::getCHARSET().' COLLATE='.\autosystem\DBConn::getCOLLATE().' ;';
             $result1 = $this->DB->query($sql);
             $return1 = $result1->rowCount()==1;
            }else{     $return1 = true;    }
        if(!$this->existTableAmailContentpartTracking()){
             $sql = 'CREATE TABLE IF NOT EXISTS `'.\autosystem\DBConn::getTableNameFor('amail_contentpart_tracking').'`('.
               '`amail_contentpart_id` INT(7) NOT NULL,'.
               '`TRACKING` ENUM(\'TRUE\',\'FALSE\')NOT NULL DEFAULT \'FALSE\','.
               '`SESSIONBIND` ENUM(\'TRUE\',\'FALSE\')NOT NULL DEFAULT \'FALSE\','.
              ' PRIMARY KEY ( `amail_contentpart_id` )'.
             ')ENGINE = InnoDB DEFAULT CHARSET='.\autosystem\DBConn::getCHARSET().' COLLATE='.\autosystem\DBConn::getCOLLATE().' ;';
             $result1 = $this->DB->query($sql);
             $return1 = $result1->rowCount()==1;
            }else{     $return1 = true;    }
        if(!$this->existTableAmailContentToContact()){
             $sql = 'CREATE TABLE IF NOT EXISTS `'.\autosystem\DBConn::getTableNameFor('amail_contentto_contact').'`('.
               '`id` INT(4) NOT NULL AUTO_INCREMENT,'.
               '`msg` TEXT NOT NULL,'.
               '`hypermsg` TEXT NOT NULL,'.
               '`amail_contacts_id` INT(7) NOT NULL,'.
               '`accound_id` INT(20),'.
               '`STATUS` ENUM(\''.implode('\',\'',array_keys($this->sendstates)).'\')NOT NULL DEFAULT \'SENDIBLE\','.//TODO check DEFAULT parameter
               '`SENDIBLE` ENUM(\'TRUE\',\'FALSE\')NOT NULL DEFAULT \'FALSE\','.
              ' PRIMARY KEY ( `id` )'.
             ')ENGINE = InnoDB DEFAULT CHARSET='.\autosystem\DBConn::getCHARSET().' COLLATE='.\autosystem\DBConn::getCOLLATE().' ;';
             $result5 = $this->DB->query($sql);
             $return5 = $result5->rowCount()==1;
            }else{     $return5 = true;    }    
        if(!$this->existTableAmailContentpartKey()){
             $sql = 'CREATE TABLE IF NOT EXISTS `'.\autosystem\DBConn::getTableNameFor('amail_response_key').'`('.
               '`id` INT (25) NOT NULL AUTO_INCREMENT,'.
               '`KEY` VARCHAR (200) COLLATE '.\autosystem\DBConn::getCOLLATE().' NOT NULL,'.
               '`accound_id` INT (20),'.
               '`emailstblname` VARCHAR (200) COLLATE '.\autosystem\DBConn::getCOLLATE().' NOT NULL,'.
               '`emails_id` INT (25),'.
               '`responseproces_id` INT (25),'.
               '`readcount` INT (25),'.
               '`imputdate` timestamp NOT NULL,'.
              ' PRIMARY KEY ( `id` )'.
             ')ENGINE = InnoDB DEFAULT CHARSET='.\autosystem\DBConn::getCHARSET().' COLLATE='.\autosystem\DBConn::getCOLLATE().' ;';
             $result2 = $this->DB->query($sql);
             $return2 = $result2->rowCount()==1;
        }else{     $return2 = true;    }
        if(!$this->existTableAmailContentpartLog()){
             $sql = 'CREATE TABLE IF NOT EXISTS `'.\autosystem\DBConn::getTableNameFor('amail_response_log').'`('.
               '`KEY` VARCHAR (200) COLLATE '.\autosystem\DBConn::getCOLLATE().' NOT NULL,'.
               '`accound_id` INT (20),'.
               '`session_id` VARCHAR (200) COLLATE '.\autosystem\DBConn::getCOLLATE().' NOT NULL,'.
               '`imputdate` timestamp NOT NULL,'.
              ' PRIMARY KEY ( `KEY` )'.
             ')ENGINE = InnoDB DEFAULT CHARSET='.\autosystem\DBConn::getCHARSET().' COLLATE='.\autosystem\DBConn::getCOLLATE().' ;';
             $result3 = $this->DB->query($sql);
             $return3 = $result3->rowCount()==1;
        }else{     $return3 = true;    }

        if(!$this->existTableAmailDatasetParam()){
             $sql = 'CREATE TABLE IF NOT EXISTS `'.\autosystem\DBConn::getTableNameFor('amail_dataset_param').'`('.
               '`id` INT (7),'.
               '`paramname` VARCHAR (200) COLLATE '.\autosystem\DBConn::getCOLLATE().' NOT NULL,'.
               '`TYPE` ENUM(\'INPUT\',\'OUTPUT\',\'CONST\')NOT NULL DEFAULT \'OUTPUT\','.
               '`value` VARCHAR (200) COLLATE '.\autosystem\DBConn::getCOLLATE().' DEFAULT \'\','.
              ' PRIMARY KEY (`id` )'.
             ')ENGINE = InnoDB DEFAULT CHARSET='.\autosystem\DBConn::getCHARSET().' COLLATE='.\autosystem\DBConn::getCOLLATE().' ;';
             $result4 = $this->DB->query($sql);
             $return4 = $result4->rowCount()==1;
        }else{     $return4 = true;    }
    return  $return1  && $return2  && $return3&& $return4&& $return5;  
    }
  public function __install(){
    if(!$this->existTableAmailCategory()){
     $sql = 'CREATE TABLE IF NOT EXISTS `'.\autosystem\DBConn::getTableNameFor('amail_category').'`('.
       '`id` INT (4) NOT NULL AUTO_INCREMENT,'.
       '`name` VARCHAR (230) COLLATE '.\autosystem\DBConn::getCOLLATE().' NOT NULL,'.
       '`type_id` INT (4),'.
       '`mailadminlog_id` INT (7),'.
       '`menuindex` INT (2),'.
      ' PRIMARY KEY ( `id` )'.
     ')ENGINE = InnoDB DEFAULT CHARSET='.\autosystem\DBConn::getCHARSET().' COLLATE='.\autosystem\DBConn::getCOLLATE().' ;';
     $result1 = $this->DB->query($sql);
     $return1 = $result1->rowCount()==1;
    }else{     $return1 = true;    }
    
    if(!$this->existTableAmailConceptmail()){
     $sql = 'CREATE TABLE IF NOT EXISTS `'.\autosystem\DBConn::getTableNameFor('amail_conceptmail').'`('.
       '`id` INT (5),'.
       '`NAMEDKEY` VARCHAR (20) COLLATE '.\autosystem\DBConn::getCOLLATE().' NOT NULL,'.
       '`STATUS` ENUM(\''.implode('\',\'',array_keys($this->states)).'\')NOT NULL DEFAULT \'\','.//TODO check DEFAULT parameter
       '`SENDIBLE` ENUM(\'TRUE\',\'FALSE\')NOT NULL DEFAULT \'FALSE\','.
       '`amail_category_id` INT (4),'.
       '`imputdate` timestamp NOT NULL,'.
      ' PRIMARY KEY ( `id` )'.
     ')ENGINE = InnoDB DEFAULT CHARSET='.\autosystem\DBConn::getCHARSET().' COLLATE='.\autosystem\DBConn::getCOLLATE().' ;';
     $result2 = $this->DB->query($sql);
     $return2 = $result2->rowCount()==1;
    }else{     $return2 = true;    }
    
    if(!$this->existTableAmailStyle()){
     $sql = 'CREATE TABLE IF NOT EXISTS `'.\autosystem\DBConn::getTableNameFor('amail_style').'`('.
       '`amail_conceptmail_id` INT (5),'.
       '`key` VARCHAR (230) COLLATE '.\autosystem\DBConn::getCOLLATE().' NOT NULL,'.
       '`value` VARCHAR (230) COLLATE '.\autosystem\DBConn::getCOLLATE().' NOT NULL,'.
      ' PRIMARY KEY ()'.
     ')ENGINE = InnoDB DEFAULT CHARSET='.\autosystem\DBConn::getCHARSET().' COLLATE='.\autosystem\DBConn::getCOLLATE().' ;';
     $result3 = $this->DB->query($sql);
     $return3 = $result3->rowCount()==1;
    }else{     $return3 = true;   }

    if(!$this->existTableAmailContentpart()){
     $sql = 'CREATE TABLE IF NOT EXISTS `'.\autosystem\DBConn::getTableNameFor('amail_contentpart').'`('.
       '`id` INT (7) NOT NULL AUTO_INCREMENT,'.
       '`contenttype` VARCHAR (50) COLLATE '.\autosystem\DBConn::getCOLLATE().' NOT NULL,'.
       '`TYPE` ENUM(\''.implode('\',\'',array_keys($this->parttypes)).'\')NOT NULL DEFAULT \'NA\','.//TODO check DEFAULT parameter
       '`content` TEXT NOT NULL,'.
      ' PRIMARY KEY ( `id` )'.
     ')ENGINE = InnoDB DEFAULT CHARSET='.\autosystem\DBConn::getCHARSET().' COLLATE='.\autosystem\DBConn::getCOLLATE().' ;';
     $result4 = $this->DB->query($sql);
     $return4 = $result4->rowCount()==1;
    }else{     $return4 = true;    }

    if(!$this->existTableAmailDataset()){
     $sql = 'CREATE TABLE IF NOT EXISTS `'.\autosystem\DBConn::getTableNameFor('amail_dataset').'`('.
       '`id` INT (7),'.
       '`name` VARCHAR (50) COLLATE '.\autosystem\DBConn::getCOLLATE().' NOT NULL,'.
       '`selectconverters` VARCHAR (1000) COLLATE '.\autosystem\DBConn::getCOLLATE().' NOT NULL,'.
       '`query` VARCHAR (5000) COLLATE '.\autosystem\DBConn::getCOLLATE().' NOT NULL,'.
      ' PRIMARY KEY ( `id` )'.
     ')ENGINE = InnoDB DEFAULT CHARSET='.\autosystem\DBConn::getCHARSET().' COLLATE='.\autosystem\DBConn::getCOLLATE().' ;';
     $result5 = $this->DB->query($sql);
     $return5 = $result5->rowCount()==1;
    }else{     $return5 = true;    }

    if(!$this->existTableAmailAdminlog()){
     $sql = 'CREATE TABLE IF NOT EXISTS `'.\autosystem\DBConn::getTableNameFor('amail_adminlog').'`('.
       '`id` INT (10),'.
       '`accound_id` INT (20),'.
       '`info` VARCHAR (5000) COLLATE '.\autosystem\DBConn::getCOLLATE().' NOT NULL,'.
       '`imputtime` timestamp NOT NULL,'.
      ' PRIMARY KEY ( `id` )'.
     ')ENGINE = InnoDB DEFAULT CHARSET='.\autosystem\DBConn::getCHARSET().' COLLATE='.\autosystem\DBConn::getCOLLATE().' ;';
     $result6 = $this->DB->query($sql);
     $return6 = $result6->rowCount()==1;
    }else{     $return6 = true;    }

    if(!$this->existTableAmailContenttopartSequence()){
     $sql = 'CREATE TABLE IF NOT EXISTS `'.\autosystem\DBConn::getTableNameFor('amail_contenttopart_sequence').'`('.
       '`amail_conceptmail_id` INT (5),'.
       '`amail_contentpart_id` INT (7),'.
       '`seqranking` INT (4),'.
       '`STATUS` ENUM(\''.implode('\',\'',array_keys($this->states)).'\')NOT NULL DEFAULT \'\','.//TODO check DEFAULT parameter
       '`type` ENUM(\''.implode('\',\'',array_keys($this->parttypes)).'\')NOT NULL DEFAULT \'\','.//TODO check DEFAULT parameter
      ' PRIMARY KEY(`amail_conceptmail_id`,`amail_contentpart_id`)'.
     ')ENGINE = InnoDB DEFAULT CHARSET='.\autosystem\DBConn::getCHARSET().' COLLATE='.\autosystem\DBConn::getCOLLATE().' ;';
     $result7 = $this->DB->query($sql);
     $return7 = $result7->rowCount()==1;
    }else{     $return7 = true;    }

    if(!$this->existTableAmailContentpartDatasetParameter()){
     $sql = 'CREATE TABLE IF NOT EXISTS `'.\autosystem\DBConn::getTableNameFor('amail_contentpart_dataset_parameter').'`('.
       '`id` INT (10),'.
       '`amail_dataset_id` INT (7),'.
       '`amail_parameter_id` INT (7),'.
       '`seqranking` INT (4),'.
       '`imputtime` timestamp NOT NULL,'.
      ' PRIMARY KEY ( `id` )'.
     ')ENGINE = InnoDB DEFAULT CHARSET='.\autosystem\DBConn::getCHARSET().' COLLATE='.\autosystem\DBConn::getCOLLATE().' ;';
     $result8 = $this->DB->query($sql);
     $return8 = $result8->rowCount()==1;
    }else{     $return8 = true;    }

    if(!$this->existTableAmailContentpartLang()){
     $sql = 'CREATE TABLE IF NOT EXISTS `'.\autosystem\DBConn::getTableNameFor('amail_contentpart_lang').'`('.
       '`id` INT (7) NOT NULL AUTO_INCREMENT,'.
       '`amail_contentpart_id` INT (7),'.
       '`locale` VARCHAR (5) COLLATE '.\autosystem\DBConn::getCOLLATE().' NOT NULL,'.
      ' PRIMARY KEY ( `id` )'.
     ')ENGINE = InnoDB DEFAULT CHARSET='.\autosystem\DBConn::getCHARSET().' COLLATE='.\autosystem\DBConn::getCOLLATE().' ;';
     $result9 = $this->DB->query($sql);
     $return9 = $result9->rowCount()==1;
    }else{     $return9 = true;    }
//* isset($this->DB)&&$lnb'*/ 
        return $return1  && $return2  && $return3  && $return4  && $return5  && $return6  && $return7  && $return8  && $return9  && $this->__upgrade($this->getVersionNr())  ; 
    }
    public function isInstalled(){
        return $this->isinstalled|| $this->__installEmails()&& $this->existTableAmailCategory()&& $this->existTableAmailConceptmail()&& $this->existTableAmailStyle()&& $this->existTableAmailContentpart()&& $this->existTableAmailDataset()&& $this->existTableAmailAdminlog()&& $this->existTableAmailContenttopartSequence()&& $this->existTableAmailContentpartDatasetParameter()&& $this->existTableAmailContentpartLang()&& $this->isinstalled=true;
    }
    public function isUpgrade($version){
        $isupgrade = true;
        if($version > 3.31){
             $isupgrade = $this->existTableAmailContentpartCid() && $this->existTableAmailContentpartTracking() && $this->existTableAmailContentpartKey() && $this->existTableAmailContentpartLog();
        }
        return $isupgrade && $this->isinstalled=true;
    }
    private function GET_amail_conceptmailSQL($accountid,$id){
       $ac= \autosystem\DBConn::getTableNameFor('amail_conceptmail');
       $sql='SELECT `'.$ac.'`.* FROM `'.$ac.'` ';
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
        return $sql;
    }
    public function getConceptById($concept_id,$forAccound_id,$view,$content=''){
        $return['from']='noreplay'.$this->antispamdetection;
        $return['values']=$this->getEValues($concept_id,$forAccound_id);
        $return['values']['content']=$content;
        $email = $this->GET_EmailAdress($forAccound_id,true);        
        $return['to'] = $email['email']; 
        $return['values']['to']=$email['email'];
        $return['plain']=$view->pushTemplate($this->getParts($concept_id,'PLAIN'),$return['values']);
        $return['html']=$view->pushTemplate($this->getParts($concept_id,'HTML'),$return['values']);
        $return['name']=$return['values']['username'];        
        $return['cashname']=date('Y').'-'.date('m').'-'.$return['to'].'emailcach';
        $return['activebackup']=false;
        $return['parts']=$this->get_OTHERParts($concept_id);
        return $return;
    }
    public function GET_EmailAdress($formAccound_id,$checkCanSend=false){
        if(!$this->isinstalled){return false;}
        $a= \autosystem\DBConn::getTableNameFor('accound');
        $ac= \autosystem\DBConn::getTableNameFor('email');
        $sql = 'SELECT `'.$ac.'`.* FROM `'.$ac.'` LEFT JOIN `'.$a.'` ON `'.$a.'`.`email_id`=`'.$ac.'`.`id`  WHERE `'.$a.'`.`id`=:acc';
        if($checkCanSend){
            $sql .= ' AND `emailsubscribed`=\'FALSE\' AND `'.$ac.'`.`ENABLED`=\'TRUE\' ';
        }      
        return $this->GET_Stmt($sql,[':acc'=>$formAccound_id]);
    }
    public function getSignature($formAccound_id=0){
        if($formAccound_id>0){
            $return['from']=$this->GET_EmailAdress($formAccound_id,$checkCanSend=false);
        }
        $return['from'] = 'noreplay.mailpool@'.$_SERVER['host'].'';
        return $return;
    }
    private function getParts($concept_id,$type){//not tested
       $parts = $this->getPartsArray($concept_id,$type);
       $return = '' ;    
       foreach($parts as $part){       $return .= $part;     }
       return $return;
    }
    private function getPartsArray($concept_id,$type){
       $cp= \autosystem\DBConn::getTableNameFor('amail_contentpart');
       $mcs= \autosystem\DBConn::getTableNameFor('amail_contenttopart_sequence');
       $sql='SELECT `id` AS `key` , `content` AS `value` FROM `'.$cp.'` ';       
       $sql .= 'LEFT JOIN `'.$mcs.'` ON `'.$mcs.'`.`amail_contentpart_id`= `'.$cp.'`.`id`';
       $sql .= 'WHERE `'.$mcs.'`.`amail_conceptmail_id`=:id AND `amail_contentpart`.`TYPE`=:TYPE ORDER BY `seqranking` ';
       $parts = $this->GET_array($sql,[':id'=>$concept_id,':TYPE'=>$type],0,'id');
       if(!is_array($parts)){return [];}
       return $parts;
    }
    private function get_OTHERParts($concept_id){
       $cp= \autosystem\DBConn::getTableNameFor('amail_contentpart');
       $mcs= \autosystem\DBConn::getTableNameFor('amail_contenttopart_sequence');
       $cc= \autosystem\DBConn::getTableNameFor('amail_contentpart_cid');
       $sql='SELECT * FROM `'.$cp.'` ';
       $sql .= 'LEFT JOIN `'.$cc.'` ON `'.$cp.'`.`id`= `'.$cc.'`.`amail_contentpart_id`';       
       $sql .= 'LEFT JOIN `'.$mcs.'` ON `'.$mcs.'`.`amail_contentpart_id`= `'.$cp.'`.`id`';
       $sql .= 'WHERE `'.$mcs.'`.`amail_conceptmail_id`=:id AND `'.$cp.'`.`TYPE` !=\'PLAIN\' AND `'.$cp.'`.`TYPE` !=\'HTML\'  ORDER BY `seqranking` ';
       //print $sql;
       $parts = $this->GET_Array_FULL($sql,[':id'=>$concept_id],0,'id');
       
       if(!is_array($parts)){return [];}
       return $parts;
    }
    private function getEValues($concept_id,$accound_id){
       if($account_id > 0){return ''; }
       $a= \autosystem\DBConn::getTableNameFor('accound');
       $sql = 'SELECT `username` ,`key` FROM `'.$a.'` WHERE id = :id ';
       $return = $this->GET_Stmt($sql,[':id'=>$accound_id]);
       $user = $this->getUser($accound_id);
       if(is_array($user)&& key_exists('id',$user)){
            $return = $return + $user;
       }
       $param = $this->getConceptParam($concept_id);
       if(is_array($param)){
            $return = $return + $param;
       }
       return $return; 
    }
    private function getUser($accound_id){
        $u= \autosystem\DBConn::getTableNameFor('user');
        $sql = 'SELECT * FROM `'.$u.'` WHERE accound_id = :accound_id ';
       $return = $this->GET_Stmt($sql,[':accound_id'=>$accound_id]);
        return $return;
    }
    private function getConceptParam($concept_id){
       $p= \autosystem\DBConn::getTableNameFor('amail_dataset_param');
       $cp=\autosystem\DBConn::getTableNameFor('amail_contentpart_dataset_parameter');
       $sql = 'SELECT * FROM `'.$cp.'` ';       
       $sql .= 'LEFT JOIN `'.$p.'` ON `'.$p.'`.`amail_contentpart_id`= `'.$cp.'`.`id`';
       $return = $this->GET_Stmt($sql,[':accound_id'=>$accound_id]);
      // $return['url'] = 'http://autosite.gsucks/5.0/users/Activate/';
        return $return;
    }
    
    public function GET_amail_conceptmailsSummary($location,$templatename,$view,$accountid,$id){// TODO: check params
        $sql= $this->GET_amail_conceptmailSQL($accound_id,$accountid,$id);
        $len =$this->GET_COUNT($this->get_amail_conceptmailCountSQL($accound_id,$accountid,$id));
        $this->paginator->setlistlength($len);
        $this->paginator->setpagenr($pagenr);
        $this->paginator->setpaginatorreload($location,$templatename);
        // TODO: check next line and uncomment
        //parent::GET_Summary($location,$templatename,$view,$sql,null,true,$accound_id);
        $view->getpaginator($this->paginator->getdata());
        print 'NOT IMPLEMENTED';
    }
    private function get_amail_conceptmailCountSQL($accound_id,$accountid,$id){
       $ac= \autosystem\DBConn::getTableNameFor('amail_conceptmail');
       $sql='SELECT COUNT(*) AS `count`FROM `'.$ac.'` ';
       $sql.= $where;
       return $sql;
    }
    private function GET_amail_styleSQL($accountid,$id){
       $as= \autosystem\DBConn::getTableNameFor('amail_style');
       $sql='SELECT `'.$as.'`.* FROM `'.$as.'` ';
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
    public function GET_amail_stylesSummary($location,$templatename,$view,$accountid,$id){// TODO: check params
        $sql= $this->GET_amail_styleSQL($accound_id,$accountid,$id);
        $len =$this->GET_COUNT($this->get_amail_styleCountSQL($accound_id,$accountid,$id));
        $this->paginator->setlistlength($len);
        $this->paginator->setpagenr($pagenr);
        $this->paginator->setpaginatorreload($location,$templatename);
        // TODO: check next line and uncomment
        //parent::GET_Summary($location,$templatename,$view,$sql,null,true,$accound_id);
        $view->getpaginator($this->paginator->getdata());
        print 'NOT IMPLEMENTED';
    }
    public function GET_EmailConceptArrayList($conceptid){
       $cp= \autosystem\DBConn::getTableNameFor('amail_contentpart');
       $mcs= \autosystem\DBConn::getTableNameFor('amail_contenttopart_sequence');
       $cid= \autosystem\DBConn::getTableNameFor('amail_contentpart_cid');
       $sql='SELECT `'.$cp.'`.*,`'.$cid.'`.`cid` FROM `'.$mcs.'` ';       
       $sql .= 'LEFT JOIN `'.$cp.'` ON `'.$mcs.'`.`amail_contentpart_id`= `'.$cp.'`.`id`';
       $sql .= 'LEFT JOIN `'.$cid.'` ON `'.$cid.'`.`amail_contentpart_id`= `'.$mcs.'`.`amail_contentpart_id`' ;

       $sql .= 'WHERE `'.$mcs.'`.`amail_conceptmail_id`=:id ORDER BY `seqranking`;';
       $param['id']=$conceptid;
       print$sql;
       return $this->GET_Stmt_key($sql ,$param,0,'id');
    }
    private function get_amail_styleCountSQL($accound_id,$accountid,$id){
       $as= \autosystem\DBConn::getTableNameFor('amail_style');
       $sql='SELECT COUNT(*) AS `count`FROM `'.$as.'` ';
       $sql.= $where;
       return $sql;
    }
    private function GET_amail_contentpartSQL($accountid,$id){
       $ac= \autosystem\DBConn::getTableNameFor('amail_contentpart');
       $sql='SELECT `'.$ac.'`.* FROM `'.$ac.'` ';
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
    public function GET_amail_contentpartsSummary($location,$templatename,$view,$accountid,$id){// TODO: check params
        $sql= $this->GET_amail_contentpartSQL($accound_id,$accountid,$id);
        $len =$this->GET_COUNT($this->get_amail_contentpartCountSQL($accound_id,$accountid,$id));
        $this->paginator->setlistlength($len);
        $this->paginator->setpagenr($pagenr);
        $this->paginator->setpaginatorreload($location,$templatename);
        // TODO: check next line and uncomment
        //parent::GET_Summary($location,$templatename,$view,$sql,null,true,$accound_id);
        $view->getpaginator($this->paginator->getdata());
        print 'NOT IMPLEMENTED';
    }
    private function get_amail_contentpartCountSQL($accound_id,$accountid,$id){
       $ac= \autosystem\DBConn::getTableNameFor('amail_contentpart');
       $sql='SELECT COUNT(*) AS `count`FROM `'.$ac.'` ';
       $sql.= $where;
       return $sql;
    }
    public function FIND($string,$type=''){
        switch($type){
        	case 'concept':       return $this->FIND_Title($string);                 
        	   break;
               
        	   break;
        	default :                 $this->FIND_ALL($string); 
        }
    }
    private function FIND_ALL($string){
        return 0;
    }
    public function GET_Concepts_LIST(){
        $a = \autosystem\DBConn::getTableNameFor('amail_conceptmail');
        $sql ='SELECT `id` AS `key` , `namedkey` AS `value` FROM `'.$a.'`';
        //print  $sql;
        $array = $this->GET_Array($sql,[]);
        $array[0]='none';       
        return (is_array($array))?$array:false;
    }
    private function GET_amail_datasetSQL($accountid,$id){
       $ad= \autosystem\DBConn::getTableNameFor('amail_dataset');
       $sql='SELECT `'.$ad.'`.* FROM `'.$ad.'` ';
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
    public function GET_amail_datasetsSummary($location,$templatename,$view,$accountid,$id){// TODO: check params
        $sql= $this->GET_amail_datasetSQL($accound_id,$accountid,$id);
        $len =$this->GET_COUNT($this->get_amail_datasetCountSQL($accound_id,$accountid,$id));
        $this->paginator->setlistlength($len);
        $this->paginator->setpagenr($pagenr);
        $this->paginator->setpaginatorreload($location,$templatename);
        // TODO: check next line and uncomment
        //parent::GET_Summary($location,$templatename,$view,$sql,null,true,$accound_id);
        $view->getpaginator($this->paginator->getdata());
        print 'NOT IMPLEMENTED';
    }
    private function get_amail_datasetCountSQL($accound_id,$accountid,$id){
       $ad= \autosystem\DBConn::getTableNameFor('amail_dataset');
       $sql='SELECT COUNT(*) AS `count`FROM `'.$ad.'` ';
       $sql.= $where;
       return $sql;
    }
    private function GET_amail_adminlogSQL($accountid,$id){
       $aa= \autosystem\DBConn::getTableNameFor('amail_adminlog');
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
    public function GET_amail_adminlogsSummary($location,$templatename,$view,$accountid,$id){// TODO: check params
        $sql= $this->GET_amail_adminlogSQL($accound_id,$accountid,$id);
        $len =$this->GET_COUNT($this->get_amail_adminlogCountSQL($accound_id,$accountid,$id));
        $this->paginator->setlistlength($len);
        $this->paginator->setpagenr($pagenr);
        $this->paginator->setpaginatorreload($location,$templatename);
        // TODO: check next line and uncomment
        //parent::GET_Summary($location,$templatename,$view,$sql,null,true,$accound_id);
        $view->getpaginator($this->paginator->getdata());
        print 'NOT IMPLEMENTED';
    }
    private function get_amail_adminlogCountSQL($accound_id,$accountid,$id){
       $aa= \autosystem\DBConn::getTableNameFor('amail_adminlog');
       $sql='SELECT COUNT(*) AS `count`FROM `'.$aa.'` ';
       $sql.= $where;
       return $sql;
    }
    private function GET_amail_contenttopart_sequenceSQL($accountid,$id){
       $acs= \autosystem\DBConn::getTableNameFor('amail_contenttopart_sequence');
       $sql='SELECT `'.$acs.'`.* FROM `'.$acs.'` ';
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
    public function GET_amailsToSend($xamount=10){// TODO: check params
        $sql= 'SELECT `id`,`issend`,`SENDIBLE`,`STATUS`,`statuscomment`,'.
        '`conceptmail_id`, `accound_id`, `imputDate`, `subject` '.
        'FROM '.$this->getEmailtbl().'WHERE `issend`= \'FALSE\''.(($xamount>0)?'LIMIT '.$xamount:'');
        return $this->GET_Stmt_key($sql,[],0,'id');
        //parent::GET_Summary($location,$templatename,$view,$sql,null,true,$accound_id);
    }
    public function GET_testToSendByID($id){// TODO: check params
        $sql= 'SELECT `id`,`issend`,`SENDIBLE`,`STATUS`,`statuscomment`,'.
        '`conceptmail_id`, `accound_id`, `imputDate`, `subject` '.
        'FROM '.$this->getEmailtbl().' WHERE `id`= '.$id;
        return $this->GET_Stmt_key($sql,[],0,'id');
    }
    private function get_amail_contenttopart_sequenceCountSQL($accound_id,$accountid,$id){
       $acs= \autosystem\DBConn::getTableNameFor('amail_contenttopart_sequence');
       $sql='SELECT COUNT(*) AS `count`FROM `'.$acs.'` ';
       $sql.= $where;
       return $sql;
    }
    private function GET_amail_contentpart_dataset_parameterSQL($accountid,$id){
       $acdp= \autosystem\DBConn::getTableNameFor('amail_contentpart_dataset_parameter');
       $sql='SELECT `'.$acdp.'`.* FROM `'.$acdp.'` ';
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
    public function GET_amail_contentpart_dataset_parametersSummary($location,$templatename,$view,$accountid,$id){// TODO: check params
        $sql= $this->GET_amail_contentpart_dataset_parameterSQL($accound_id,$accountid,$id);
        $len =$this->GET_COUNT($this->get_amail_contentpart_dataset_parameterCountSQL($accound_id,$accountid,$id));
        $this->paginator->setlistlength($len);
        $this->paginator->setpagenr($pagenr);
        $this->paginator->setpaginatorreload($location,$templatename);
        // TODO: check next line and uncomment
        //parent::GET_Summary($location,$templatename,$view,$sql,null,true,$accound_id);
        $view->getpaginator($this->paginator->getdata());
        print 'NOT IMPLEMENTED';
    }
    private function get_amail_contentpart_dataset_parameterCountSQL($accound_id,$accountid,$id){
       $acdp= \autosystem\DBConn::getTableNameFor('amail_contentpart_dataset_parameter');
       $sql='SELECT COUNT(*) AS `count`FROM `'.$acdp.'` ';
       $sql.= $where;
       return $sql;
    }

//no options
    private function GET_SQL($account_id,$id){
       $ac= \autosystem\DBConn::getTableNameFor('amail_category');
       $ac= \autosystem\DBConn::getTableNameFor('amail_conceptmail');
       $as= \autosystem\DBConn::getTableNameFor('amail_style');
       $ac= \autosystem\DBConn::getTableNameFor('amail_contentpart');
       $ad= \autosystem\DBConn::getTableNameFor('amail_dataset');
       $aa= \autosystem\DBConn::getTableNameFor('amail_adminlog');
       $acs= \autosystem\DBConn::getTableNameFor('amail_contenttopart_sequence');
       $acdp= \autosystem\DBConn::getTableNameFor('amail_contentpart_dataset_parameter');
       $acl= \autosystem\DBConn::getTableNameFor('amail_contentpart_lang');
       $sql='SELECT `'.$acl.'`.* ,`'.$acdp.'`.* ,`'.$acs.'`.* ,`'.$aa.'`.* ,`'.$ad.'`.* ,`'.$ac.'`.* ,`'.$as.'`.* ,`'.$ac.'`.* ,`'.$ac.'`.* FROM `'.$ac.'` LEFT JOIN `'.$ac.'`.ON .`'.$ac.'`.`id=`'.$ac.'`.`amail_conceptmail_id`LEFT JOIN `'.$as.'`.ON .`'.$ac.'`.`id=`'.$as.'`.`amail_style_id`LEFT JOIN `'.$ac.'`.ON .`'.$ac.'`.`id=`'.$ac.'`.`amail_contentpart_id`LEFT JOIN `'.$ad.'`.ON .`'.$ac.'`.`id=`'.$ad.'`.`amail_dataset_id`LEFT JOIN `'.$aa.'`.ON .`'.$ac.'`.`id=`'.$aa.'`.`amail_adminlog_id`LEFT JOIN `'.$acs.'`.ON .`'.$ac.'`.`id=`'.$acs.'`.`amail_contenttopart_sequence_id`LEFT JOIN `'.$acdp.'`.ON .`'.$ac.'`.`id=`'.$acdp.'`.`amail_contentpart_dataset_parameter_id`LEFT JOIN `'.$acl.'`.ON .`'.$ac.'`.`id=`'.$acl.'`.`amail_contentpart_lang_id`';
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
    public function GET_EmailssSummary($location,$templatename,$view,$account_id,$id){// TODO: check params
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
       $ac= \autosystem\DBConn::getTableNameFor('amail_category');
       $ac= \autosystem\DBConn::getTableNameFor('amail_conceptmail');
       $as= \autosystem\DBConn::getTableNameFor('amail_style');
       $ac= \autosystem\DBConn::getTableNameFor('amail_contentpart');
       $ad= \autosystem\DBConn::getTableNameFor('amail_dataset');
       $aa= \autosystem\DBConn::getTableNameFor('amail_adminlog');
       $acs= \autosystem\DBConn::getTableNameFor('amail_contenttopart_sequence');
       $acdp= \autosystem\DBConn::getTableNameFor('amail_contentpart_dataset_parameter');
       $acl= \autosystem\DBConn::getTableNameFor('amail_contentpart_lang');
       $sql='SELECT COUNT(*) AS `count`FROM `'.$ac.'` LEFT JOIN `'.$ac.'`.ON .`'.$ac.'`.`id=`'.$ac.'`.`amail_conceptmail_id`LEFT JOIN `'.$as.'`.ON .`'.$ac.'`.`id=`'.$as.'`.`amail_style_id`LEFT JOIN `'.$ac.'`.ON .`'.$ac.'`.`id=`'.$ac.'`.`amail_contentpart_id`LEFT JOIN `'.$ad.'`.ON .`'.$ac.'`.`id=`'.$ad.'`.`amail_dataset_id`LEFT JOIN `'.$aa.'`.ON .`'.$ac.'`.`id=`'.$aa.'`.`amail_adminlog_id`LEFT JOIN `'.$acs.'`.ON .`'.$ac.'`.`id=`'.$acs.'`.`amail_contenttopart_sequence_id`LEFT JOIN `'.$acdp.'`.ON .`'.$ac.'`.`id=`'.$acdp.'`.`amail_contentpart_dataset_parameter_id`LEFT JOIN `'.$acl.'`.ON .`'.$ac.'`.`id=`'.$acl.'`.`amail_contentpart_lang_id`';
       $sql.= $where;
       return $sql;
    }
  public function existTableAmailCategory(){     return $this->existTableLabel('amail_category');   }
  public function existTableAmailConceptmail(){     return $this->existTableLabel('amail_conceptmail');   }
  public function existTableAmailStyle(){     return $this->existTableLabel('amail_style');   }
  public function existTableAmailContentpart(){     return $this->existTableLabel('amail_contentpart');   }
  public function existTableAmailDataset(){     return $this->existTableLabel('amail_dataset');   }
  public function existTableAmailAdminlog(){     return $this->existTableLabel('amail_adminlog');   }
  public function existTableAmailContenttopartSequence(){     return $this->existTableLabel('amail_contenttopart_sequence');   }
  public function existTableAmailContentpartDatasetParameter(){     return $this->existTableLabel('amail_contentpart_dataset_parameter');   }
  public function existTableAmailContentpartLang(){     return $this->existTableLabel('amail_contentpart_lang');   }
  /**
  __upgrade
  */
  public function existTableAmailContentpartCid(){     return $this->existTableLabel('amail_contentpart_cid');   }
  public function existTableAmailContentpartTracking(){     return $this->existTableLabel('amail_contentpart_tracking');   }
  public function existTableAmailContentpartKey(){     return $this->existTableLabel('amail_response_key');   }
  public function existTableAmailContentpartLog(){     return $this->existTableLabel('amail_response_log');   }
  public function existTableAmailDatasetParam(){     return $this->existTableLabel('amail_dataset_param');   }
    
  protected function getEmailtbl(){
        $this->mailextendor =date('Y');
        return 'amail_emails'.$this->mailextendor;
  }
  public function existTableAmail_emails(){      return $this->existTableLabel($this->getEmailtbl());  }
  public function getModelInfo(){  return 'Amail.Emails.model.info';}

  public function getVersionNr(){  return (float)5.4;  }
  public function getRuleDefault(){return 'KING';} //'PUBLIC','FINANCIAL','TRANSLATE','MANAGE','ADMINISTRATION','KING','USER','MODERATOR','CHAIRMAN' }
  protected function __installEmails(){
        if(!$this->existTableAmail_emails()){
            $sql = 'CREATE TABLE IF NOT EXISTS `'.\autosystem\DBConn::getTableNameFor('amail_emails').$this->mailextendor.'`('.
                '`id` INT(25) NOT NULL AUTO_INCREMENT,'.
                '`issend` ENUM(\'TRUE\',\'FALSE\')NOT NULL DEFAULT \'FALSE\','.
                '`SENDIBLE` ENUM(\'TRUE\',\'FALSE\')NOT NULL DEFAULT \'FALSE\','.
                '`STATUS` ENUM(\''.implode('\',\'',array_keys($this->sendstates)).'\') NOT NULL ,'.
                '`statuscomment` varchar(1000) COLLATE '.\autosystem\DBConn::getCOLLATE().' NOT NULL,'.
                '`conceptmail_id` INT(20) NOT NULL ,'.
                '`accound_id` INT(20)ZEROFILL NOT NULL,'.
                '`imputDate` datetime NOT NULL,'.
                '`subject` varchar(1000) COLLATE '.\autosystem\DBConn::getCOLLATE().' NOT NULL DEFAULT \' \','.
                'PRIMARY KEY (`id`)'.
            ')ENGINE = InnoDB DEFAULT CHARSET='.\autosystem\DBConn::getCHARSET().' COLLATE='.\autosystem\DBConn::getCOLLATE().' ;';
            //print $sql;
            $result5 = $this->DB->query($sql);
            return $result5->rowCount()!==false;
        }else{
            return true;
        }
   }
}
?>