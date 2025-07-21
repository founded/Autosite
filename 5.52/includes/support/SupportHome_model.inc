<?php
/* model*/
namespace support;
class SupportHome_model extends \autosystem\Model{
  public $states = array( 'PROGRESS'=>'progress','ERROR' => 'error','CLOSED' => 'closed','OPEN' => 'open','CANCELED' => 'canceled');
  public $priority = array('NULL' => 'normal','URGENT'=>'urgent','NOTURGENT' => 'not.urgent','IMPORTANT' => 'important','NOTIMPORTANT' => 'notimportant');
  protected $isinstalled = false;
  public function __install(){
    if(!$this->existTableSupportcategory()){
     $sql = 'CREATE TABLE IF NOT EXISTS `'.\autosystem\DBConn::getTableNameFor('support_category').'`('.
       '`id` INT (3) NOT NULL AUTO_INCREMENT,'.
       '`label` VARCHAR (230) COLLATE '.\autosystem\DBConn::getCOLLATE().' NOT NULL,'.
       '`ENABLED` ENUM(\'TRUE\',\'FALSE\')NOT NULL DEFAULT \'FALSE\','.
       '`category_id` INT (3),'.
       '`user_id` INT (10),'.
       '`stars` INT (4),'.
      ' PRIMARY KEY (`id`)'.
     ')ENGINE = InnoDB DEFAULT CHARSET='.\autosystem\DBConn::getCHARSET().' COLLATE='.\autosystem\DBConn::getCOLLATE().' ;';
     $result1 = $this->DB->query($sql);
     $return1 = $result1->rowCount()==1;
    }else{
     $return1 = true;
    }
    if(!$this->existTableSupporttickets()){
     $sql = 'CREATE TABLE IF NOT EXISTS `'.\autosystem\DBConn::getTableNameFor('support_tickets').'`('.
       '`id` INT (12) NOT NULL AUTO_INCREMENT,'.
       '`url` VARCHAR (230) COLLATE '.\autosystem\DBConn::getCOLLATE().' NOT NULL,'.
       '`location` VARCHAR (230) COLLATE '.\autosystem\DBConn::getCOLLATE().' NOT NULL,'.
       '`title` VARCHAR (230) COLLATE '.\autosystem\DBConn::getCOLLATE().' NOT NULL,'.
       '`shortcomment` VARCHAR (1000) COLLATE '.\autosystem\DBConn::getCOLLATE().' NOT NULL,'.
       '`detail` TEXT NOT NULL,'.
       '`accound_id` INT (20),'.
       '`support_id` INT (12),'.
       '`location_id` INT (9),'.
       '`category_id` INT (3),'.
       '`STATUS` ENUM(\''.implode('\',\'',array_keys($this->states)).'\')NOT NULL ,'.
       '`PRIORITY` ENUM(\''.implode('\',\'',array_keys($this->priority)).'\')NOT NULL ,'.
       '`ENABLED` ENUM(\'TRUE\',\'FALSE\')NOT NULL DEFAULT \'FALSE\','.
       '`inputdate` date NOT NULL,'.
       '`ip` VARCHAR (50) COLLATE '.\autosystem\DBConn::getCOLLATE().' NOT NULL,'.
       '`stars` INT (4),'.
      ' PRIMARY KEY(`id`)'.
     ')ENGINE = InnoDB DEFAULT CHARSET='.\autosystem\DBConn::getCHARSET().' COLLATE='.\autosystem\DBConn::getCOLLATE().' ;';
     $result2 = $this->DB->query($sql);
     $return2 = $result2->rowCount()==1;
    }else{
     $return2 = true;
    }
    if(!$this->existTableSupportticketreactions()){
     $sql = 'CREATE TABLE IF NOT EXISTS `'.\autosystem\DBConn::getTableNameFor('support_ticket_reactions').'`('.
       '`id` INT (20) NOT NULL AUTO_INCREMENT,'.
       '`detail` TEXT NOT NULL,'.
       '`accound_id` INT (20),'.
       '`support_ticket_id` INT (12),'.
       '`inputdate` timestamp NOT NULL,'.
       '`ip` VARCHAR (50) COLLATE '.\autosystem\DBConn::getCOLLATE().' NOT NULL,'.
       '`stars` INT (4),'.
      ' PRIMARY KEY (`id`)'.
     ')ENGINE = InnoDB DEFAULT CHARSET='.\autosystem\DBConn::getCHARSET().' COLLATE='.\autosystem\DBConn::getCOLLATE().' ;';
     $result3 = $this->DB->query($sql);
     $return3 = $result3->rowCount()==1;
    }else{
     $return3 = true;
    }
    if(!$this->existTableSupportworkspace()){
     $sql = 'CREATE TABLE IF NOT EXISTS `'.\autosystem\DBConn::getTableNameFor('support_workspace').'`('.
     '`support_id`  INT(20) NOT NULL AUTO_INCREMENT,'.
     '`accound_id`  INT(50)  NOT NULL,'.
     'PRIMARY KEY (`support_id`,`accound_id`)'.
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
    return $this->isinstalled|| $this->existTableSupportcategory()
    && $this->existTableSupporttickets()&& $this->existTableSupportticketreactions()
    && $this->existTableSupportworkspace()&& $this->existTableLocation()
    && $this->isinstalled=true;
  }
  public function __upgrade($version){
    switch($version){ 
	case 4.1:
        print 'support_ticket_reactions'.'`inputdate` timestamp NOT NULL,';
        // imputdate is changed to inputdate
        $sql = 'ALTER TABLE';
        return true;
	    break;
	case 5.0:
	    break;
    }
  }
 /*
 * options 
*/
    public function save_SupportHomeSupportcategory($label,$ENABLED,$category_id,$user_id,$stars){ 
       $is_ENABLED= ( $isENABLED)?'TRUE':'FALSE';
       $sc= \autosystem\DBConn::getTableNameFor('support_category');
       $key ='INSERT INTO `'.$sc.'`(`id`,`label`,`ENABLED`,`category_id`,`user_id`,`stars`) '.
                            'VALUES(NULL, :label, :ENABLED, :category_id, :user_id, :stars) RETURNING support_category_id';
       $value = [':label'=>$label,':ENABLED'=>$isENABLED,':category_id'=>$category_id,':user_id'=>$user_id,':stars'=>$stars];
       return $this->transaction($sql);
    }
    public function update_SupportHomeSupportcategory($id,$label,$ENABLED,$category_id,$user_id,$stars){ 
       $is_ENABLED= ( $isENABLED)?'TRUE':'FALSE';
       $sc= \autosystem\DBConn::getTableNameFor('support_category');
       $key ='UPDATE `'.$sc.'` SET  `label`=:label,`ENABLED`=:ENABLED,`category_id`=:category_id,`user_id`=:user_id,`stars`=:stars` WHERE id=:id';
       
       $sql[$key]= [':id'=>$id,':label'=>$label,':ENABLED'=>$isENABLED,':category_id'=>$category_id,':user_id'=>$user_id,':stars'=>$stars];
       return is_array($this->transaction($sql));      
    }
    public function delete_SupportHomeSupportcategory($id){
       $sc= \autosystem\DBConn::getTableNameFor('support_category');
       $key ='DELETE FROM `'.$sc.'` WHERE `id`=:id';//
       $value = [':id'=>$id];//TODO check if there need more checks before use delete_SupportHomeSupportcategory
       $sql[$key]= $value;
       return $this->transaction($sql);
    }
    public function exist_SupportHomeSupportcategory($id){
       $sc= \autosystem\DBConn::getTableNameFor('support_category');
       $key ='SELECT * FROM `'.$sc.'` WHERE `id`=:id ;';//
       $value = [':id'=>$id];//TODO check if there are more parameters to use exist_SupportHomeSupportcategory
       return $this->isQuery($key,$value);
    }
    public function GET_categorysSummary($location,$templatename,$view,$category_id='',$accound_id=''){// TODO: check params
        $sc= \autosystem\DBConn::getTableNameFor('support_category');
        $sql='SELECT * FROM `'.$sc.'` '.((isset($category_id)&&$category_id!='')?' WHERE category_id=\''.$category_id.'\'':'');
        parent::GET_Summary($location,$templatename,$view,$sql);
    }
    public function GET_Categorys($category_id=''){
        $sc= \autosystem\DBConn::getTableNameFor('support_category');
        $sql='SELECT id AS \'key\' , label AS \'value\' FROM `'.$sc.'` '.((isset($category_id)&&$category_id!='')?'WHERE category_id=\''.$category_id.'\'':'');
        //print $sql;
		return parent::GET_Array($sql);
    } 
    /* --- */
    public function newTicket($url,$location,$title,$shortcomment,$detail,$status,$accound_id,$category,$location_id=0){
            $ip = parent::getIp();
            $ENABLED = 'TRUE';
            $key ='INSERT INTO `'.\autosystem\DBConn::getTableNameFor('support_tickets').
                            '`(`id`,`url`,`location`,`title`,`shortcomment`,`detail`,`accound_id`,`location_id`,`category_id`,`STATUS`,`ENABLED`,`inputDate`,`ip`) '.
                       'VALUES(NULL,:url ,:location ,:title ,:shortcomment ,:detail ,:accound_id ,:location_id ,:category_id ,:STATUS ,:ENABLED ,:inputDate ,:ip ) RETURNING `id`';//
            $value = [
            ':url'=>$url,
            ':location'=>$location,
            ':title'=>$title,
            ':shortcomment'=>$shortcomment,
            ':detail'=>$detail,
            ':accound_id'=>$accound_id,
            ':location_id'=>$location_id,
            ':category_id'=>$category,
            ':PRIORITY'=>$isPRIORITY,
            ':inputdate'=>$inputdate,
            ':STATUS'=>$status,
            ':ENABLED'=>$ENABLED,
            ':inputDate'=>$this->nowDateTime(),
            ':ip'=>$ip];
            $sql[$key]= $value;
            return $this->transaction($sql);
        }
    
    public function save_ticket($url,$location,$title,$shortcomment,$detail,$accound_id,$support_id,$location_id,$STATUS,$PRIORITY,$ENABLED,$inputdate,$ip,$stars,$category_id=0){ 
       $is_ENABLED=($ENABLED)?'TRUE':'FALSE';
       
       $_PRIORITY=(in_array($PRIORITY,$this->priority))?$PRIORITY:'NOTURGENT';
       $st= \autosystem\DBConn::getTableNameFor('support_tickets');
       $key ='INSERT INTO `'.$st.'`(`id`,`url`,`location`,`title`,`shortcomment`,`detail`,`accound_id`,`support_id`,`location_id`,`category_id`,`STATUS`,`PRIORITY`,`ENABLED`,`inputdate`,`ip`,`stars`) '.
                            'VALUES(NULL, :url, :location, :title, :shortcomment, :detail, :accound_id, :support_id, :location_id, :category_id, :STATUS, :PRIORITY, :ENABLED, :inputdate, :ip, :stars) RETURNING support_tickets_id';
       $value = [':url'=>$url,':location'=>$location,':title'=>$title,':shortcomment'=>$shortcomment,':detail'=>$detail,':accound_id'=>$accound_id,':support_id'=>$support_id,':location_id'=>$location_id,
       ':category_id'=>$category_id,':STATUS'=>$STATUS,':PRIORITY'=>$_PRIORITY,':ENABLED'=>$is_ENABLED,':ip'=>$ip,':stars'=>$stars,':inputdate'=>$this->nowDateTime()];
       $sql[$key]= $value;
       //print \autosystem\DEBUG::printPDO($key,$value);
       return $this->transaction($sql);
    }
    public function update_SupportHomeSupporttickets($id,$url,$location,$title,$shortcomment,$detail,$accound_id,$support_id,$location_id,$category_id,$STATUS,$PRIORITY,$ENABLED,$inputdate,$ip,$stars){ 
       $is_ENABLED=($isENABLED)?'TRUE':'FALSE';
       $st= \autosystem\DBConn::getTableNameFor('support_tickets');
       $key ='UPDATE `'.$st.'` SET  `id`=:id,`url`=:url,`location`=:location,`title`=:title,`shortcomment`=:shortcomment,`detail`=:detail,`accound_id`=:accound_id,`support_id`=:support_id,`location_id`=:location_id,`category_id`=:category_id,`STATUS`=:STATUS,`PRIORITY`=:PRIORITY,`ENABLED`=:ENABLED,`inputdate`=:inputdate,`ip`=:ip,`stars`=:stars` WHERE id=:id';
       
       $sql[$key]= [':id'=>$id,':url'=>$url,':location'=>$location,':title'=>$title,':shortcomment'=>$shortcomment,':detail'=>$detail,':accound_id'=>$accound_id,':support_id'=>$support_id,':location_id'=>$location_id,':category_id'=>$category_id,':STATUS'=>$STATUS,':PRIORITY'=>$isPRIORITY,':ENABLED'=>$isENABLED,':inputdate'=>$inputdate,':ip'=>$ip,':stars'=>$stars];
       return is_array($this->transaction($sql));      
    }
    public function delete_ticket($id){
       $st= \autosystem\DBConn::getTableNameFor('support_tickets');
       $key ='DELETE FROM `'.$st.'` WHERE `id`=:id';//
       $value = [':id'=>$id];
       $sql[$key]= $value;
       $str= \autosystem\DBConn::getTableNameFor('support_ticket_reactions');
       $key ='DELETE FROM `'.$str.'` WHERE `support_ticket_id`=:id';//
       $sql[$key]= $value;
       return $this->transaction($sql);
    }
    public function exist_SupportHomeSupporttickets($ticket_id){
       $st= \autosystem\DBConn::getTableNameFor('support_tickets');
       $key ='SELECT * FROM `'.$st.'` WHERE `id`=:id ;';//
       $value = [':id'=>$ticket_id];
       return $this->isQuery($key,$value);
    }
    public function isOwnerOfticket($accound_id,$ticket_id){
       $st= \autosystem\DBConn::getTableNameFor('support_tickets');
       $key ='SELECT * FROM `'.$st.'` WHERE `id`=:id AND `accound_id`=\''.$accound_id.'\' ;';//
       $value = [':id'=>$ticket_id];
       return $this->isQuery($key,$value);
    }  
    public function GET_ticket($location,$templatename,$view,$id,$accound_id){
            $a = \autosystem\DBConn::getTableNameFor('support_tickets');
            $c = \autosystem\DBConn::getTableNameFor('support_category');
            $l = \autosystem\DBConn::getTableNameFor('adres_location');
            $aa = \autosystem\DBConn::getTableNameFor('adres_accound');
            $p = \autosystem\DBConn::getTableNameFor('user_telephones');
            $tp = \autosystem\DBConn::getTableNameFor('user_contact_telephone');
            $e = \autosystem\DBConn::getTableNameFor('email');
            $ac = \autosystem\DBConn::getTableNameFor('accound');
            $sql='SELECT '.
            '`'.$tp.'`.*,`'.$ac.'`.`username`,`'.$e.'`.*,`'.$l.'`.*, `'.$aa.'`.*, `'.$a.'`.*,`'.$c.'`.`label` AS `categorylabel`'.
            ' FROM `'.$a.'` '.
            //'LEFT JOIN `'.$p.'` ON `'.$p.'`.`accound_id`=`'.$a.'`.`accound_id` '.
            'LEFT JOIN `'.$p.'` ON `'.$p.'`.`phones_id` = (SELECT `'.$p.'`.phones_id FROM `'.$p.'` WHERE `'.$p.'`.`accound_id`=`'.$a.'`.`accound_id` LIMIT 1 )'.  
            'LEFT JOIN `'.$aa.'` ON `'.$aa.'`.`accound_id`=`'.$a.'`.`accound_id` '.'AND `'.$aa.'`.`VISIBLE`=\'TRUE\' '.
            'LEFT JOIN `'.$l.'` ON `'.$aa.'`.`location_id`=`'.$l.'`.`id` '.
            'LEFT JOIN `'.$tp.'` ON `phones_id`=`'.$tp.'`.`id` '.
            'LEFT JOIN `'.$ac.'` ON `'.$ac.'`.`id`=`'.$a.'`.`accound_id` '.
            'LEFT JOIN `'.$e.'` ON `'.$ac.'`.`email_id`= `'.$e.'`.`id` '.'AND `'.$e.'`.`VISIBLE`=\'TRUE\' '.
            'LEFT JOIN `'.$c.'` ON `'.$a.'`.`category_id`= `'.$c.'`.`id`';
            $sql.= ' WHERE `'.$a.'`.`id`=\''.$id.'\'';
            if(isset($accound_id)){
                $sql.='`'.$a.'`.`accound_id` = \''.$id.'\''; 
            }
            return parent::GET($location,$templatename,$view,$sql,null,true,$accound_id); 
    }
    private function GET_support_ticketsSQL($accoundid,$id){
       $st= \autosystem\DBConn::getTableNameFor('support_tickets');
       $sql='SELECT `'.$st.'`.* FROM `'.$st.'` ';
        $where = '';
        $link ='WHERE';
        if(!empty($accoundid)){// TODO: field not exist
            $where.= $link.' `'.$st.'`.`accound_id`=\''.$accoundid.'\';';//TODO check it out
        }
        if(!empty($id)){// TODO: field not exist
            $where.= $link.' `'.$st.'`.`id`=\''.$id.'\';';//TODO check it out
        }
        $sql.= $where;
        $this->paginator->setSort('  ');//TODO ORDER BY `???`.`inputDate` DESC
        $this->paginator->setRange(20);
        return $sql;
    }
    public function getTicketAsArray($Ticketid ,$accound_id=null,$filter=null){
        $l= \autosystem\DBConn::getTableNameFor('adres_location');
        $a= \autosystem\DBConn::getTableNameFor('support_tickets');
        $c= \autosystem\DBConn::getTableNameFor('support_category');
        $ac = \autosystem\DBConn::getTableNameFor('accound');
        $sql='SELECT `'.$c.'`.* ,`support_category`.`label` AS `categorylabel`,`'.$l.'`.* ,`'.$ac.'`.* ,`'.$a.'`.* FROM `'.$a.'` LEFT JOIN `'.$l.'` ON `location_id`=`'.$l.'`.`id` '.
        'LEFT JOIN `'.$c.'` ON `'.$a.'`.`category_id`=`'.$c.'`.`id` '.
        'LEFT JOIN `'.$ac.'` ON `'.$ac.'`.`id`=`'.$a.'`.`accound_id` '.
        'WHERE `'.$a.'`.`id`=:ticketid ORDER BY `'.$a.'`.`inputDate` DESC';
        return $this->GET_Stmt($sql,[':ticketid'=>$Ticketid]);
    }
    public function GET_support_ticketssSummary($location,$templatename,$view,$accound_id,$id,$pagenr=0){// TODO: check params
        $sql= $this->GET_support_ticketsSQL($accound_id,$id);
        $len =$this->GET_COUNT($this->get_support_ticketsCountSQL($accound_id,$id));
        $this->paginator->setlistlength($len);
        $this->paginator->setpagenr($pagenr);
        $this->paginator->setpaginatorreload($location,$templatename);
    }
    private function get_support_ticketsCountSQL($accound_id,$id){
       $st= \autosystem\DBConn::getTableNameFor('support_tickets');
       $sql='SELECT COUNT(*) AS `count`FROM `'.$st.'` ';
       $sql.= $where;
       return $sql;
    }               
    public function save_ticketReaction($detail,$accound_id,$support_ticket_id){ 
       $str= \autosystem\DBConn::getTableNameFor('support_ticket_reactions');
       $ip = $this->getIp();
       $inputdate = $this->nowDateTime();
       $key ='INSERT INTO `'.$str.'`(`id`,`detail`,`accound_id`,`support_ticket_id`,`inputdate`,`ip`,`stars`) '.
                            'VALUES(NULL, :detail, :accound_id, :support_ticket_id, :inputdate, :ip, :stars) RETURNING support_ticket_reactions_id';
       $value = [':detail'=>$detail,':accound_id'=>$accound_id,':support_ticket_id'=>$support_ticket_id,':inputdate'=>$this->nowDateTime(),':ip'=>$ip,':stars'=>$stars];
       $sql[$key]= $value;
       return $this->transaction($sql);
    }
    public function update_ticketReaction($id,$detail,$accound_id,$support_ticket_id,$inputdate,$ip,$stars){ 
       $str= \autosystem\DBConn::getTableNameFor('support_ticket_reactions');
       $key ='UPDATE `'.$str.'` SET  `detail`=:detail,`accound_id`=:accound_id,`support_ticket_id`=:support_ticket_id,`inputdate`=:inputdate,`ip`=:ip,`stars`=:stars` WHERE id=:id';
       
       $sql[$key]= [':id'=>$id,':detail'=>$detail,':accound_id'=>$accound_id,':support_ticket_id'=>$support_ticket_id,':inputdate'=>$inputdate,':ip'=>$ip,':stars'=>$stars];
       return is_array($this->transaction($sql));      
    }
    public function delete_ticketReactions($id){
       $str= \autosystem\DBConn::getTableNameFor('support_ticket_reactions');
       $key ='DELETE FROM `'.$str.'` WHERE `id`=:id';//
       $value = [':id'=>$id];//TODO check if there need more checks before use delete_SupportHomeSupportticketreactions
       $sql[$key]= $value;
       return $this->transaction($sql);
    }
    public function exist_ticketreaction($ticketid){
       $str= \autosystem\DBConn::getTableNameFor('support_ticket_reactions');
       $key ='SELECT * FROM `'.$str.'` WHERE `id`=:id ;';//
       $value = [':id'=>$id];//TODO check if there are more parameters to use exist_SupportHomeSupportticketreactions
       return $this->isQuery($key,$value);
    }
    private function GET_reactionsSQL($accountid,$ticketid){
       $r= \autosystem\DBConn::getTableNameFor('support_ticket_reactions');
       $sql='SELECT `'.$r.'`.*,'.
       'IF(`'.$r.'`.`accound_id`=\''.$accountid.'\',\' chattxtL\',\' chattxtR\') AS `chattxt` FROM `'.$r.'` ';
       $sql.='WHERE `'.$r.'`.`support_ticket_id`=\''.$ticketid.'\'';//TODO check it out
       return $sql;
    }
    public function GET_reactionsSummary($location,$templatename,$view,$account_id,$id){// TODO: check params
        $sql= $this->GET_reactionsSQL($account_id,$id);
        parent::GET_Summary($location,$templatename,$view,$sql);
    }
    public function GET_reactionsAsArray($location,$templatename,$view,$accountid,$id){// TODO: check params
        $sql= $this->GET_reactionsSQL($accound_id,$accountid,$id);
        $len =$this->GET_COUNT($this->get_support_ticket_reactionsCountSQL($accound_id,$accountid,$id));
        $this->paginator->setlistlength($len);
    }
    public function save_Toworkspace($ticket_id,$account_id){ 
       $sw= \autosystem\DBConn::getTableNameFor('support_workspace');
       $key ='INSERT INTO `'.$sw.'`(`ticket_id`,`account_id`) '.
                            'VALUES( :ticket_id, :account_id) RETURNING support_workspace_id';
       $value = [':ticket_id'=>$ticket_id,':account_id'=>$account_id];
       return $this->transaction($sql);
    }
    public function update_Supportworkspace($id,$ticket_id,$account_id){ 
       $sw= \autosystem\DBConn::getTableNameFor('support_workspace');
       $key ='UPDATE `'.$sw.'` SET `ticket_id`=:ticket_id,`account_id`=:account_id` WHERE id=:id';
       
       $sql[$key]= [':id'=>$id,':ticket_id'=>$ticket_id,':account_id'=>$account_id];
       return is_array($this->transaction($sql));      
    }
    public function delete_Supportworkspace($id){
       $sw= \autosystem\DBConn::getTableNameFor('support_workspace');
       $key ='DELETE FROM `'.$sw.'` WHERE `id`=:id';//
       $value = [':id'=>$id];//TODO check if there need more checks before use delete_SupportHomeSupportworkspace
       $sql[$key]= $value;
       return $this->transaction($sql);
    }
    public function exist_SupportHomeSupportworkspace($id){
       $sw= \autosystem\DBConn::getTableNameFor('support_workspace');
       $key ='SELECT * FROM `'.$sw.'` WHERE `id`=:id ;';//
       $value = [':id'=>$id];//TODO check if there are more parameters to use exist_SupportHomeSupportworkspace
       return $this->isQuery($key,$value);
    }
    public function GET_support_workspace($location,$templatename,$view,$accountid,$id){
       $sw= \autosystem\DBConn::getTableNameFor('support_workspace');
       $sql='SELECT `'.$sw.'`.* FROM `'.$sw.'` ';
        $where = '';//
        $link ='WHERE';
    }
    private function GET_support_workspaceSQL($accountid,$id){
       $sw= \autosystem\DBConn::getTableNameFor('support_workspace');
       $sql='SELECT `'.$sw.'`.* FROM `'.$sw.'` ';
//' IF(`'.$a.'`.`ENABLED`='TRUE',' checked=checked ',' ') AS `isVisable`,w
        $where = '';
        $link ='WHERE';
        if(!empty($accountid)){ $where.= $link.' `???`.`accound_id`=\''.$accountid.'\';';}
        return $sql;
    }
    public function GET_support_workspacesSummary($location,$templatename,$view,$accountid,$id){// TODO: check params
        $sql= $this->GET_support_workspaceSQL($accound_id,$accountid,$id);
        $len =$this->GET_COUNT($this->get_support_workspaceCountSQL($accound_id,$accountid,$id));
        $this->paginator->setlistlength($len);
        $this->paginator->setpagenr($pagenr);
        $this->paginator->setpaginatorreload($location,$templatename);
    }
    private function get_support_workspaceCountSQL($accound_id,$accountid,$id){
       $sw= \autosystem\DBConn::getTableNameFor('support_workspace');
       $sql='SELECT COUNT(*) AS `count`FROM `'.$sw.'` ';
       $sql.= $where;
       return $sql;
    }
    public function GET_SupportHome($location,$templatename,$view,$accountid,$id){
       $sc= \autosystem\DBConn::getTableNameFor('support_category');
       $st= \autosystem\DBConn::getTableNameFor('support_tickets');
       $str= \autosystem\DBConn::getTableNameFor('support_ticket_reactions');
       $sw= \autosystem\DBConn::getTableNameFor('support_workspace');
       $sql='SELECT `'.$sw.'`.* ,`'.$str.'`.* ,`'.$st.'`.* ,`'.$sc.'`.* FROM `'.$sc.'` LEFT JOIN `'.$st.'`.ON .`'.$sc.'`.`id=`'.$st.'`.`support_tickets_id`LEFT JOIN `'.$str.'`.ON .`'.$sc.'`.`id=`'.$str.'`.`support_ticket_reactions_id`LEFT JOIN `'.$sw.'`.ON .`'.$sc.'`.`id=`'.$sw.'`.`support_workspace_id`';
       $where = '';//
       $link ='WHERE';
    }
    private function GET_SQL($accound_id,$enabled){
        $a = \autosystem\DBConn::getTableNameFor('support_tickets');
        $c = \autosystem\DBConn::getTableNameFor('support_category');
        $ac = \autosystem\DBConn::getTableNameFor('accound');
        $sql='SELECT `'.$a.'`.*,`'.$c.'`.`label` AS `categorylabel`, IF(`'.$a.'`.`ENABLED`=\'TRUE\',\' checked=checked \',\'\') AS `isVisable`'.
        'FROM `'.$a.'` '.
        'LEFT JOIN `'.$ac.'` ON `'.$ac.'`.`id`=`'.$a.'`.`accound_id` '.
        'LEFT JOIN `'.$c.'` ON `'.$a.'`.`category_id`= `'.$c.'`.`id` ';
        $where = 'WHERE `'.$a.'`.`accound_id`=\''.$accound_id.'\'';
        $link =' && '; 
        if(!empty($category_id)){
            $where.= $link.' (`'.$a.'`.`category_id`=\''.$category_id.'\'';
            $where.= ' OR `'.$c.'`.`category_id`=\''.$category_id.'\' )';
            $link =' && ';    
        }
        if(!empty($enabled)&&$enabled==true){
            $where.= $link.' `'.$a.'`.`ENABLED`=\'TRUE\'';
            $link =' && '; 
        }
        if(!empty($idlist)){
            if(is_array($idlist)){
                $where.= $link.'(';
                $link = '';
                foreach($idlist as $id){
                    if(is_numeric($id)){
                        $where.= $link.'`'.$a.'`.`id`=\''.$id.'\'';
                        $link =' OR ';
                    }
                }
                $where.= ')' ;$link =' && ';
            }else{
                $where.= $link.' `'.$a.'`.`id`=\''.$idlist.'\'';
                $link =' && '; 
            }
        }
        $sql.= $where;
        return $sql;
    }
  public function GET_MyTicketSummary($location,$templatename,$view,$accound_id){
     $sql= $this->GET_SQL($accound_id,$ids);
     parent::GET_Summary($location,$templatename,$view,$sql);
  }
  public function existTableSupportcategory(){     return $this->existTableLabel('support_category');   }
  public function existTableSupporttickets(){     return $this->existTableLabel('support_tickets');   }
  public function existTableSupportticketreactions(){     return $this->existTableLabel('support_ticket_reactions');   }
  public function existTableSupportworkspace(){     return $this->existTableLabel('support_workspace');   }
  public function existTableLocation(){     return $this->existTableLabel('adres_location');      }
  public function getModelInfo(){  return 'support.home.model.info';  }
  public function getVersionNr(){  return (float)5.4;  }
  public function getRuleDefault(){return 'USER';  }
}



?>