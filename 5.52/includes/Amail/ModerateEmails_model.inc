<?php
/* model*/
namespace Amail;
class ModerateEmails_model extends \Amail\Emails_model{
    protected $isinstalled = false;
    public function __install(){         return parent::__install();    }
    public function isInstalled(){       return parent::isInstalled();  }
    public function __upgrade($version){ 
        parent::__upgrade($version);
        //ALTER TABLE `amail_conceptmail`CHANGE COLUMN `mail_content_id` `amail_content_id` INT(6) NULL DEFAULT NULL AFTER `amail_category_id`;  
    }
    public function save_category($name,$type_id,$mailadminlog_id,$menuindex){ 
       $ac= \autosystem\DBConn::getTableNameFor('amail_category');
       $key ='INSERT INTO `'.$ac.'`(`id`,`name`,`type_id`,`mailadminlog_id`,`menuindex`) '.
                            'VALUES(NULL, :name, :type_id, :mailadminlog_id, :menuindex) RETURNING amail_category_id';
       $sql[$key] = [':name'=>$name,':type_id'=>$type_id,':mailadminlog_id'=>$mailadminlog_id,':menuindex'=>$menuindex];//TODO CHECK insert save_ModerateEmailsAmailcategory
       return $this->transaction($sql);
    }
    public function update_ModerateEmailsAmailcategory($id,$name,$type_id,$mailadminlog_id,$menuindex){ 
       $ac= \autosystem\DBConn::getTableNameFor('amail_category');
       $key ='UPDATE `'.$ac.'` SET  `name`=:name,`type_id`=:type_id,`mailadminlog_id`=:mailadminlog_id,`menuindex`=:menuindex` WHERE id=:id';
       $sql[$key]= [':id'=>$id,':name'=>$name,':type_id'=>$type_id,':mailadminlog_id'=>$mailadminlog_id,':menuindex'=>$menuindex];//TODO CHECK update update_ModerateEmailsAmailcategory
       return is_array($this->transaction($sql));      
    }
    public function delete_category($id){
       $ac= \autosystem\DBConn::getTableNameFor('amail_category');
       $key ='DELETE FROM `'.$ac.'` WHERE `id`=:id';//
       $value = [':id'=>$id];
       $sql[$key]= $value;
       return $this->transaction($sql);
    }
    public function exist_ModerateEmailsAmailcategory($name){
       $ac= \autosystem\DBConn::getTableNameFor('amail_category');
       $key ='SELECT * FROM `'.$ac.'` WHERE `name`=:name';//
       $value = [':name'=>$name];
       return $this->isQuery($key,$value);
    }
    public function GET_amail_category($location,$templatename,$view,$accountid,$id){
       $ac= \autosystem\DBConn::getTableNameFor('amail_category');
       $sql='SELECT `'.$ac.'`.* FROM `'.$ac.'` ';
        $where = '';//
        $link ='WHERE';
    }
    public function GET_Categorys(){
       $afc= \autosystem\DBConn::getTableNameFor('amail_category'); 
       $sql='SELECT `id` AS \'key\' ,`name` AS \'value\' FROM `'.$afc.'`';
       //print $sql;
       return parent::GET_Array($sql);
    }
    private function GET_amail_categorySQL($Categoryid){
       $ac= \autosystem\DBConn::getTableNameFor('amail_category');
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
    public function GETcategorysSummary($location,$templatename,$view,$Categoryid=0){// TODO: check params
        $sql= $this->GET_amail_categorySQL($Categoryid);
        $len =$this->GET_COUNT($this->get_amail_categoryCountSQL($Categoryid));
        $this->paginator->setlistlength($len);
        $this->paginator->setpagenr($pagenr);
        $this->paginator->setpaginatorreload($location,$templatename);
        // TODO: check next line and uncomment
        parent::GET_Summary($location,$templatename,$view,$sql);
        $view->getpaginator($this->paginator->getdata());
        print 'NOT IMPLEMENTED';
    }
    private function get_amail_categoryCountSQL($Categoryid){
       $ac= \autosystem\DBConn::getTableNameFor('amail_category');
       $sql='SELECT COUNT(*) AS `count`FROM `'.$ac.'` ';
       $sql.= $where;
       return $sql;
    }
    public function save_conceptmail($NAMEDKEY,$STATUS,$SENDIBLE,$amail_category_id,$content,$accound_id,$locale='en',$contenttype='text/plain'){
       $imputdate = $this->nowDateTime();
       $version = 1;
       $is_SENDIBLE= ( $SENDIBLE)?'TRUE':'FALSE';
       $ac= \autosystem\DBConn::getTableNameFor('amail_conceptmail');
       $cp= \autosystem\DBConn::getTableNameFor('amail_contentpart');
       $mcs= \autosystem\DBConn::getTableNameFor('amail_contenttopart_sequence');
       $sql=[];
       $key ='INSERT INTO `'.$cp.'`(`id`,`contenttype`,`TYPE`,`content`) '.
                            'VALUES(NULL, :contenttype,:type, :content) RETURNING `amail_contentpart_id`';
       $sql[$key] = [':contenttype'=>$contenttype,':content'=>$content,':type'=>'PLAIN'];     
       $key ='INSERT INTO `'.$ac.'`(`id`,`NAMEDKEY`,`STATUS`,`SENDIBLE`,`amail_category_id`,`imputdate`) '.
                            'VALUES(NULL, :NAMEDKEY, :STATUS, :SENDIBLE, :amail_category_id, :imputdate) RETURNING `amail_conceptmail_id`';
       $sql[$key] = [':NAMEDKEY'=>$NAMEDKEY,':STATUS'=>$STATUS,':SENDIBLE'=>$is_SENDIBLE,
       ':amail_category_id'=>$amail_category_id,':imputdate'=>$imputdate];
       $key ='INSERT INTO `'.$mcs.'`(`amail_contentpart_id`,`amail_conceptmail_id`,`seqranking`,`type`) '.
                            'VALUES( :amail_contentpart_id, :amail_conceptmail_id, :seqranking, :type)';
       $sql[$key] = [':amail_conceptmail_id'=> '',':amail_contentpart_id'=> '',':seqranking'=>1,':type'=>'PLAIN'];
	   return $this->transaction($sql,true);
    }
    public function Log_Admin($accound_id,$info){
    	$al= \autosystem\DBConn::getTableNameFor('amail_adminlog');
		$imputdate = $this->nowDateTime();
	 	$key ='INSERT INTO `'.$al.'`(`id`,`accound_id`,`info`,`imputtime`) '.
                            'VALUES(NULL, :accound_id, :info, :imputtime)';
		$sql[$key] = [':accound_id'=>$accound_id,':info'=>$info,':imputtime'=>$imputdate];
		return $this->transaction($sql);
	}
    public function update_ModerateEmailsAmailconceptmail($id,$NAMEDKEY,$STATUS,$SENDIBLE,$amail_category_id,$mail_content_id,$imputdate){ 
       $is_SENDIBLE= ( $isSENDIBLE)?'TRUE':'FALSE';
       $ac= \autosystem\DBConn::getTableNameFor('amail_conceptmail');
       $key ='UPDATE `'.$ac.'` SET  `NAMEDKEY`=:NAMEDKEY,`STATUS`=:STATUS,`SENDIBLE`=:SENDIBLE,`amail_category_id`=:amail_category_id,`amail_content_id`=:mail_content_id,`imputdate`=:imputdate` WHERE id=:id';
       $sql[$key]= [':id'=>$id,':NAMEDKEY'=>$NAMEDKEY,':STATUS'=>$STATUS,':SENDIBLE'=>$isSENDIBLE,':amail_category_id'=>$amail_category_id,':mail_content_id'=>$mail_content_id,':imputdate'=>$imputdate];
       return is_array($this->transaction($sql));      
    }
    public function delete_ModerateEmailsAmailconceptmail($id){
       $ac= \autosystem\DBConn::getTableNameFor('amail_conceptmail');
       $key ='DELETE FROM `'.$ac.'` WHERE `id`=:id';//
       $value = [':id'=>$id];//TODO check if there need more checks before use delete_ModerateEmailsAmailconceptmail
       $sql[$key]= $value;
       return $this->transaction($sql);
    }
    public function exist_conceptmail($NAMEDKEY){
       $ac= \autosystem\DBConn::getTableNameFor('amail_conceptmail');
       $key ='SELECT * FROM `'.$ac.'` WHERE `NAMEDKEY`=:NAMEDKEY ;';
       $value = [':NAMEDKEY'=>$NAMEDKEY];
	   //print \autosystem\DEBUG::printPDO($key,$value);
	   return $this->isQuery($key,$value);
    }
    public function GET_conceptmail($location,$templatename,&$view,$id){
       $ac= \autosystem\DBConn::getTableNameFor('amail_conceptmail');
       //$c= \autosystem\DBConn::getTableNameFor('amail_content');
       $cp= \autosystem\DBConn::getTableNameFor('amail_contentpart');
       $mcs= \autosystem\DBConn::getTableNameFor('amail_contenttopart_sequence');
       $sql='SELECT * FROM `'.$ac.'` ';
       //$sql .= 'LEFT JOIN `'.$c.'` ON `'.$c.'`.`id`= `'.$ac.'`.`amail_content_id`';
       $sql .= 'LEFT JOIN `'.$mcs.'` ON `'.$mcs.'`.`amail_conceptmail_id`= `'.$ac.'`.`id`';
       $sql .= 'LEFT JOIN `'.$cp.'` ON `'.$cp.'`.`id`= `'.$mcs.'`.`amail_contentpart_id`';
       $sql .= 'WHERE `'.$ac.'`.`id`=:id ;';//amail_contentpart_id
       /**
       1$key ='INSERT INTO `'.$ac.'`(`id`,`NAMEDKEY`,`STATUS`,`SENDIBLE`,`amail_category_id`,`amail_content_id`,`imputdate`) '.
       //2$key ='INSERT INTO `'.$c.'`(`id`,`STATUS`,`SENDIBLE`,`content_namedkey`) '.
       3$key ='INSERT INTO `'.$mcs.'`(`amail_contentpart_id`,`amail_content_id`,`seqranking`,`type`) '.      
       4$key ='INSERT INTO `'.$cp.'`(`id`,`contenttype`,`content`,`amail_dataset_id`) '.
       */
       //print $sql;
       $param['id']=$id;
       $this->GET($location,$templatename,$view,$sql,$param);
    }

    private function GET_amail_conceptmailSQL($accountid){
       $ac= \autosystem\DBConn::getTableNameFor('amail_conceptmail');
       $sql='SELECT `'.$ac.'`.* , IF(`'.$ac.'`.`SENDIBLE`=\'TRUE\',\' checked=checked \',\' \') AS `issendible` FROM `'.$ac.'` ';
        $this->paginator->setSort('');
        $this->paginator->setRange(20);
        
        return $sql;
    }
    public function GET_conceptmailsSummary($location,$templatename,$view,$accountid){// TODO: check params
        $sql= $this->GET_amail_conceptmailSQL($accountid);
		$len =$this->GET_COUNT($this->get_amail_conceptmailCountSQL($accountid));
        $this->paginator->setlistlength($len);
        $this->paginator->setpagenr($pagenr);
        $this->paginator->setpaginatorreload($location,$templatename);
        parent::GET_Summary($location,$templatename,$view,$sql,null,true,$accound_id);
        $view->getpaginator($this->paginator->getdata());
    }
    private function get_amail_conceptmailCountSQL($accountid){
       $ac= \autosystem\DBConn::getTableNameFor('amail_conceptmail');
       $sql='SELECT COUNT(*) AS `count`FROM `'.$ac.'` ';
       $sql.= $where;
       return $sql;
    }
    public function  mod_ConceptMailSendible($conceptmail_id,$bool){
        $SENDIBLE = ($bool=='true')?'TRUE':'FALSE';
        $ac = \autosystem\DBConn::getTableNameFor('amail_conceptmail');
        $key ='UPDATE `'.$ac.'` SET SENDIBLE=:SENDIBLE WHERE `id`=:id';
        $sql[$key]= [':SENDIBLE'=>$SENDIBLE,':id'=>$conceptmail_id];
        return is_array($this->transaction($sql));
    }
    public function save_ModerateEmailsAmailstyle($amail_conceptmail_id,$key,$value){ 
       $as= \autosystem\DBConn::getTableNameFor('amail_style');
       $key ='INSERT INTO `'.$as.'`(`amail_conceptmail_id`,`key`,`value`) '.
                            'VALUES( :amail_conceptmail_id, :key, :value) RETURNING amail_style_id';
       $sql[$key] = [':amail_conceptmail_id'=>$amail_conceptmail_id,':key'=>$key,':value'=>$value];//TODO CHECK insert save_ModerateEmailsAmailstyle
       return $this->transaction($sql);
    }
    public function update_ModerateEmailsAmailstyle($id,$amail_conceptmail_id,$key,$value){ 
       $as= \autosystem\DBConn::getTableNameFor('amail_style');
       $key ='UPDATE `'.$as.'` SET  `amail_conceptmail_id`=:amail_conceptmail_id,`key`=:key,`value`=:value` WHERE id=:id';
       
       $sql[$key]= [':id'=>$id,':amail_conceptmail_id'=>$amail_conceptmail_id,':key'=>$key,':value'=>$value];//TODO CHECK update update_ModerateEmailsAmailstyle
       return is_array($this->transaction($sql));      
    }
    public function delete_ModerateEmailsAmailstyle($id){
       $as= \autosystem\DBConn::getTableNameFor('amail_style');
       $key ='DELETE FROM `'.$as.'` WHERE `id`=:id';//
       $value = [':id'=>$id];//TODO check if there need more checks before use delete_ModerateEmailsAmailstyle
       $sql[$key]= $value;
       return $this->transaction($sql);
    }
    public function exist_ModerateEmailsAmailstyle($id){
       $as= \autosystem\DBConn::getTableNameFor('amail_style');
       $key ='SELECT * FROM `'.$as.'` WHERE `id`=:id ;';//
       $value = [':id'=>$id];//TODO check if there are more parameters to use exist_ModerateEmailsAmailstyle
       return $this->isQuery($key,$value);
    }
    public function GET_amail_style($location,$templatename,$view,$accountid,$id){
       $as= \autosystem\DBConn::getTableNameFor('amail_style');
       $sql='SELECT `'.$as.'`.* FROM `'.$as.'` ';
        $where = '';//
        $link ='WHERE';
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
    private function get_amail_styleCountSQL($accound_id,$accountid,$id){
       $as= \autosystem\DBConn::getTableNameFor('amail_style');
       $sql='SELECT COUNT(*) AS `count`FROM `'.$as.'` ';
       $sql.= $where;
       return $sql;
    }
    public function save_content($STATUS,$content_namedkey){
       $ac= \autosystem\DBConn::getTableNameFor('amail_content');
       $key ='INSERT INTO `'.$ac.'`(`id`,`STATUS`,`content_namedkey`) '.
                            'VALUES(NULL, :STATUS,  :content_namedkey) RETURNING amail_content_id';
       $sql[$key] = [':STATUS'=>$STATUS,':s'=>$iss,':content_namedkey'=>$content_namedkey];
       return $this->transaction($sql);
    }
    public function update_content($id,$STATUS,$s,$content_namedkey){ 
       $is_s= ( $iss)?'TRUE':'FALSE';
       $ac= \autosystem\DBConn::getTableNameFor('amail_content');
       $key ='UPDATE `'.$ac.'` SET  `STATUS`=:STATUS,`s`=:s,`content_namedkey`=:content_namedkey` WHERE id=:id';
       
       $sql[$key]= [':id'=>$id,':STATUS'=>$STATUS,':s'=>$iss,':content_namedkey'=>$content_namedkey];//TODO CHECK update update_ModerateEmailsAmailcontent
       return is_array($this->transaction($sql));      
    }
    public function delete_ModerateEmailsAmailcontent($id){
       $ac= \autosystem\DBConn::getTableNameFor('amail_content');
       $key ='DELETE FROM `'.$ac.'` WHERE `id`=:id';//
       $value = [':id'=>$id];//TODO check if there need more checks before use delete_ModerateEmailsAmailcontent
       $sql[$key]= $value;
       return $this->transaction($sql);
    }
    public function exist_ModerateEmailsAmailcontent($id){
       $ac= \autosystem\DBConn::getTableNameFor('amail_content');
       $key ='SELECT * FROM `'.$ac.'` WHERE `id`=:id ;';//
       $value = [':id'=>$id];//TODO check if there are more parameters to use exist_ModerateEmailsAmailcontent
       return $this->isQuery($key,$value);
    }
    public function GET_amail_content($location,$templatename,$view,$accountid,$id){
       $ac= \autosystem\DBConn::getTableNameFor('amail_content');
       $sql='SELECT `'.$ac.'`.* FROM `'.$ac.'` ';
        $where = '';//
        $link ='WHERE';
    }
 
    public function save_ModerateEmailsAmailcontentpart($id,$contenttype,$content,$amail_dataset_id){ 
       $ac= \autosystem\DBConn::getTableNameFor('amail_contentpart');
       $key ='INSERT INTO `'.$ac.'`(`id`,`contenttype`,`content`,`amail_dataset_id`) '.
                            'VALUES( :id, :contenttype, :content, :amail_dataset_id)';
       $sql[$key] = [':id'=>$id,':contenttype'=>$contenttype,':content'=>$content,':amail_dataset_id'=>$amail_dataset_id];//TODO CHECK insert save_ModerateEmailsAmailcontentpart
       return $this->transaction($sql);
    }
    public function save_contentpartto($contenttype,$TYPE,$content,$amail_conceptmail_id){ 
       $ac= \autosystem\DBConn::getTableNameFor('amail_contentpart');
       $key ='INSERT INTO `'.$ac.'`(`id`,`contenttype`,`content`,`TYPE`) '.
                            'VALUES(NULL, :contenttype, :content, :TYPE) RETURNING amail_contentpart_id';
                            //print $key;
       $sql[$key] = [':contenttype'=>$contenttype,':content'=>$content,':TYPE'=>$TYPE];
       
       $mcs= \autosystem\DBConn::getTableNameFor('amail_contenttopart_sequence');
       $key ='INSERT INTO `'.$mcs.'`(`amail_conceptmail_id`,`amail_contentpart_id`,`seqranking`,`TYPE`) '.
                            'VALUES(:amail_conceptmail_id,:amail_contentpart_id,:seqranking, :TYPE)';
       $sql[$key] = [':amail_conceptmail_id'=>$amail_conceptmail_id,':amail_contentpart_id'=>'',':seqranking'=>'2',':TYPE'=>$TYPE];
       return $this->transaction($sql,true);
    }
    public function update_ModerateEmailsAmailcontentpart($id,$contenttype,$content,$amail_dataset_id){ 
       $ac= \autosystem\DBConn::getTableNameFor('amail_contentpart');
       $key ='UPDATE `'.$ac.'` SET `contenttype`=:contenttype,`content`=:content,`amail_dataset_id`=:amail_dataset_id` WHERE id=:id';
       
       $sql[$key]= [':id'=>$id,':contenttype'=>$contenttype,':content'=>$content,':amail_dataset_id'=>$amail_dataset_id];//TODO CHECK update update_ModerateEmailsAmailcontentpart
       return is_array($this->transaction($sql));      
    }
    public function delete_ModerateEmailsAmailcontentpart($id){
       $ac= \autosystem\DBConn::getTableNameFor('amail_contentpart');
       $key ='DELETE FROM `'.$ac.'` WHERE `id`=:id';//
       $value = [':id'=>$id];//TODO check if there need more checks before use delete_ModerateEmailsAmailcontentpart
       $sql[$key]= $value;
       return $this->transaction($sql);
    }
    public function exist_Amailcontentpart($NAMEDKEY){
       $ac= \autosystem\DBConn::getTableNameFor('amail_contentpart');
       $key ='SELECT * FROM `'.$ac.'` WHERE `NAMEDKEY`=:NAMEDKEY ;';//
       $value = [':NAMEDKEY'=>$NAMEDKEY];//TODO check if there are more parameters to use exist_ModerateEmailsAmailcontentpart
       return $this->isQuery($key,$value);
    }
    public function GET_amail_contentpart($location,$templatename,$view,$accountid,$id){
       $ac= \autosystem\DBConn::getTableNameFor('amail_contentpart');
       $sql='SELECT `'.$ac.'`.* FROM `'.$ac.'` ';
        $where = '';//
        $link ='WHERE';
    }
    private function GET_amail_contentpartSQL($accountid,$id){
       $ac= \autosystem\DBConn::getTableNameFor('amail_contentpart');
       $acs= \autosystem\DBConn::getTableNameFor('amail_contenttopart_sequence');
       //$acl= \autosystem\DBConn::getTableNameFor('amail_contentpart_lang');
       $img = 'CON'.'CAT( \'<img src="data:\' , `'.$ac.'`.`contenttype` , \';base64,\' , `'.$ac.'`.`content` , \'" >\')';
       $sql='SELECT '.
       '`'.$ac.'`.contenttype,`'.$ac.'`.`TYPE`,IF(`'.$ac.'`.`TYPE`=\'IMAGE\','.$img.',`'.$ac.'`.`content`) AS `content`,'.
       '`'.$acs.'`.* FROM `'.$ac.'` ';
       $sql.= 'LEFT JOIN `'.$acs.'`ON `'.$ac.'`.`id`=`'.$acs.'`.`amail_contentpart_id`';
      // $sql.= 'LEFT JOIN `'.$acl.'`ON `'.$ac.'`.`id`=`'.$acl.'`.`amail_contentpart_id`';
//' IF(`'.$a.'`.`ENABLED`='TRUE',' checked=checked ',' ') AS `isVisable`,
        $where = '';
        $link ='WHERE';
        if(!empty($id)){// TODO: field not exist
            $where.= $link.' `'.$acs.'`.`amail_conceptmail_id`=\''.$id.'\';';//TODO check it out
        }
        $sql.= $where;
        print $sql;
        return $sql;
    }
    public function GET_amail_contentpartsSummary($location,$templatename,$view,$accountid,$conceptid){// TODO: check params
        $sql= $this->GET_amail_contentpartSQL($accountid,$conceptid);
        parent::GET_Summary($location,$templatename,$view,$sql,null,true,$accountid);
    }
    public function save_dataset($selectconverters,$query){
       $ad= \autosystem\DBConn::getTableNameFor('amail_dataset');
       $key ='INSERT INTO `'.$ad.'`(`id`,`selectconverters`,`query`) '.
                           'VALUES(NULL, :selectconverters, :query)';
       $sql[$key] = [':selectconverters'=>$selectconverters,':query'=>$query];//TODO CHECK insert save_ModerateEmailsAmaildataset
       return $this->transaction($sql);
    }
    public function update_ModerateEmailsAmaildataset($id,$selectconverters,$query){
       $ad= \autosystem\DBConn::getTableNameFor('amail_dataset');
       $key ='UPDATE `'.$ad.'` SET `selectconverters`=:selectconverters,`query`=:query` WHERE id=:id';
       $sql[$key]= [':id'=>$id,':selectconverters'=>$selectconverters,':query'=>$query];//TODO CHECK update update_ModerateEmailsAmaildataset
       return is_array($this->transaction($sql));      
    }
    public function delete_ModerateEmailsAmaildataset($id){
       $ad= \autosystem\DBConn::getTableNameFor('amail_dataset');
       $key ='DELETE FROM `'.$ad.'` WHERE `id`=:id';//
       $value = [':id'=>$id];//TODO check if there need more checks before use delete_ModerateEmailsAmaildataset
       $sql[$key]= $value;
       return $this->transaction($sql);
    }
    public function exist_ModerateEmailsAmaildataset($id){
       $ad= \autosystem\DBConn::getTableNameFor('amail_dataset');
       $key ='SELECT * FROM `'.$ad.'` WHERE `id`=:id ;';//
       $value = [':id'=>$id];//TODO check if there are more parameters to use exist_ModerateEmailsAmaildataset
       return $this->isQuery($key,$value);
    }
    public function getAmail_dataset($location,$templatename,$view,$reflector_accountid,$id){
       $ad= \autosystem\DBConn::getTableNameFor('amail_dataset');
       $sql='SELECT `'.$ad.'`.* FROM `'.$ad.'` ';
       $sql.='WHERE `amail_dataset`.`id`='.$id;
       $param['id']= $id;
       $datasets = $this->GET_Stmt_key($sql ,$param,0,'id');
       print_r($datasets);
       return false; //$this->GET_Stmt_key($sql ,$param,0,'id');
    }
    private function GET_amail_datasetSQL($accountid,$id){
       $ad= \autosystem\DBConn::getTableNameFor('amail_dataset');
       $sql='SELECT `'.$ad.'`.* FROM `'.$ad.'` ';     
        $this->paginator->setSort('  ');
        $this->paginator->setRange(20);
        return $sql;
    }
    public function GET_datasetsSummary($location,$templatename,$view,$accountid){// TODO: check params
        $sql= $this->GET_amail_datasetSQL($accound_id,$accountid);
        $len =$this->GET_COUNT($this->get_amail_datasetCountSQL($accound_id,$accountid));
        $this->paginator->setlistlength($len);
        $this->paginator->setpagenr($pagenr);
        $this->paginator->setpaginatorreload($location,$templatename);
        // TODO: check next line and uncomment
        parent::GET_Summary($location,$templatename,$view,$sql,null,true,$accound_id);
        $view->getpaginator($this->paginator->getdata());
    }
    private function get_amail_datasetCountSQL($accound_id,$accountid){
       $ad= \autosystem\DBConn::getTableNameFor('amail_dataset');
       $sql='SELECT COUNT(*) AS `count`FROM `'.$ad.'` ';
       return $sql;
    }
    public function GETdatasets_arr(){
       $d = \autosystem\DBConn::getTableNameFor('amail_dataset'); 
       $sql='SELECT `id` AS \'key\' ,`selectconverters` AS \'value\' FROM `'.$d.'`' ;
       //print $sql;
	   return parent::GET_Array($sql);
    }
    public function save_cidimg($contentpart_id,$contenttype,$Cid,$imgbase64,$conceptId){ 
       $cc= \autosystem\DBConn::getTableNameFor('amail_contentpart_cid');
       $cp= \autosystem\DBConn::getTableNameFor('amail_contentpart');
       $key ='INSERT INTO `'.$cp.'`(`id`,`contenttype`,`TYPE`,`content`) '.
                           'VALUES( NULL, :contenttype, \'IMAGE\', :content) RETURNING amail_contentpart_id';
       $sql[$key] = [':contenttype'=>$contenttype,':content'=>$imgbase64];
       $key ='INSERT INTO `'.$cc.'`(`id`,`cid`,`typeencoding`,`amail_contentpart_id`) '.
                           'VALUES( NULL, :cid, :typeencoding, :amail_contentpart_id) RETURNING bid';
       $sql[$key] = [':cid'=>$Cid,':typeencoding'=>'BASE64',':amail_contentpart_id'=>''];//TODO CHECK insert save_ModerateEmailsAmailadminlog
       $mcs= \autosystem\DBConn::getTableNameFor('amail_contenttopart_sequence');
       $key ='INSERT INTO `'.$mcs.'`(`amail_contentpart_id`,`amail_conceptmail_id`,`seqranking`,`type`) '.
                            'VALUES( :amail_contentpart_id, :amail_conceptmail_id, :seqranking, \'INBOUNDIMG\')';
       $sql[$key] = [':amail_contentpart_id'=>'',':amail_conceptmail_id'=>$conceptId,':seqranking'=>$this->GET_endpospartsranking($conceptId)];//TODO CHECK insert save_ModerateEmailsMailcontenttopartsequence
       return $this->transaction($sql,true);
    }
    private function GET_endpospartsranking($concept_id){
       $s = \autosystem\DBConn::getTableNameFor('amail_contenttopart_sequence'); 
       $sql='SELECT `seqranking` FROM `'.$s.'` WHERE `amail_conceptmail_id` = '.$concept_id.' ORDER BY seqranking DESC LIMIT 1' ;
       $seqranking = $this->GET_Stmt($sql,[]);
       return $seqranking['seqranking']+1;
    }
    public function save_ModerateEmailsAmailadminlog($id,$accound_id,$info,$imputdate){ 
       $aa= \autosystem\DBConn::getTableNameFor('amail_adminlog');
       $key ='INSERT INTO `'.$aa.'`(`id`,`accound_id`,`info`,`imputdate`) '.
                           'VALUES( :id, :accound_id, :info, :imputdate) RETURNING amail_adminlog_id';
       $sql[$key] = [':id'=>$id,':accound_id'=>$accound_id,':info'=>$info,':imputdate'=>$imputdate];//TODO CHECK insert save_ModerateEmailsAmailadminlog
       return $this->transaction($sql);
    }
    public function update_ModerateEmailsAmailadminlog($id,$accound_id,$info,$imputdate){ 
       $aa= \autosystem\DBConn::getTableNameFor('amail_adminlog');
       $key ='UPDATE `'.$aa.'` SET `accound_id`=:accound_id,`info`=:info,`imputdate`=:imputdate` WHERE id=:id';
       
       $sql[$key]= [':id'=>$id,':accound_id'=>$accound_id,':info'=>$info,':imputdate'=>$imputdate];//TODO CHECK update update_ModerateEmailsAmailadminlog
       return is_array($this->transaction($sql));      
    }
    public function delete_ModerateEmailsAmailadminlog($id){
       $aa= \autosystem\DBConn::getTableNameFor('amail_adminlog');
       $key ='DELETE FROM `'.$aa.'` WHERE `id`=:id';//
       $value = [':id'=>$id];//TODO check if there need more checks before use delete_ModerateEmailsAmailadminlog
       $sql[$key]= $value;
       return $this->transaction($sql);
    }
    public function exist_ModerateEmailsAmailadminlog($id){
       $aa= \autosystem\DBConn::getTableNameFor('amail_adminlog');
       $key ='SELECT * FROM `'.$aa.'` WHERE `id`=:id ;';//
       $value = [':id'=>$id];//TODO check if there are more parameters to use exist_ModerateEmailsAmailadminlog
       return $this->isQuery($key,$value);
    }
    public function GET_amail_adminlog($location,$templatename,$view,$accountid,$id){
       $aa= \autosystem\DBConn::getTableNameFor('amail_adminlog');
       $sql='SELECT `'.$aa.'`.* FROM `'.$aa.'` ';
        $where = '';//
        $link ='WHERE';
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
    public function save_ModerateEmailsAmailemailsYYYY($id,$issend,$SENDIBLE,$STATUS,$statuscomment,$conceptmail_id,$accound_id,$imputDate){ 
       $is_SENDIBLE= ( $isSENDIBLE)?'TRUE':'FALSE';
       $aeY= \autosystem\DBConn::getTableNameFor('amail_emails_YYYY');
       $key ='INSERT INTO `'.$aeY.'`(`id`,`issend`,`SENDIBLE`,`STATUS`,`statuscomment`,`conceptmail_id`,`accound_id`,`imputDate`) '.
                            'VALUES( :id, :issend, :SENDIBLE, :STATUS, :statuscomment, :conceptmail_id, :accound_id, :imputDate) RETURNING amail_emails_YYYY_id';
       $sql[$key] = [':id'=>$id,':issend'=>$isissend,':SENDIBLE'=>$isSENDIBLE,':STATUS'=>$STATUS,':statuscomment'=>$statuscomment,':conceptmail_id'=>$conceptmail_id,':accound_id'=>$accound_id,':imputDate'=>$imputDate];//TODO CHECK insert save_ModerateEmailsAmailemailsYYYY
       return $this->transaction($sql);
    }
    public function update_ModerateEmailsAmailemailsYYYY($id,$issend,$SENDIBLE,$STATUS,$statuscomment,$conceptmail_id,$accound_id,$imputDate){ 
       $is_SENDIBLE= ( $isSENDIBLE)?'TRUE':'FALSE';
       $aeY= \autosystem\DBConn::getTableNameFor('amail_emails_YYYY');
       $key ='UPDATE `'.$aeY.'` SET  `issend`=:issend,`SENDIBLE`=:SENDIBLE,`STATUS`=:STATUS,`statuscomment`=:statuscomment,`conceptmail_id`=:conceptmail_id,`accound_id`=:accound_id,`imputDate`=:imputDate` WHERE id=:id';
       
       $sql[$key]= [':id'=>$id,':issend'=>$isissend,':SENDIBLE'=>$isSENDIBLE,':STATUS'=>$STATUS,':statuscomment'=>$statuscomment,':conceptmail_id'=>$conceptmail_id,':accound_id'=>$accound_id,':imputDate'=>$imputDate];//TODO CHECK update update_ModerateEmailsAmailemailsYYYY
       return is_array($this->transaction($sql));      
    }
    public function delete_ModerateEmailsAmailemailsYYYY($id){
       $aeY= \autosystem\DBConn::getTableNameFor('amail_emails_YYYY');
       $key ='DELETE FROM `'.$aeY.'` WHERE `id`=:id';//
       $value = [':id'=>$id];//TODO check if there need more checks before use delete_ModerateEmailsAmailemailsYYYY
       $sql[$key]= $value;
       return $this->transaction($sql);
    }
    public function exist_ModerateEmailsAmailemailsYYYY($id){
       $aeY= \autosystem\DBConn::getTableNameFor('amail_emails_YYYY');
       $key ='SELECT * FROM `'.$aeY.'` WHERE `id`=:id ;';//
       $value = [':id'=>$id];//TODO check if there are more parameters to use exist_ModerateEmailsAmailemailsYYYY
       return $this->isQuery($key,$value);
    }
    public function GET_amail_emails_YYYY($location,$templatename,$view,$accountid,$id){
       $aeY= \autosystem\DBConn::getTableNameFor('amail_emails_YYYY');
       $sql='SELECT `'.$aeY.'`.* FROM `'.$aeY.'` ';
        $where = '';//
        $link ='WHERE';
    }
    private function GET_amail_emails_YYYYSQL($accountid,$id){
       $aeY= \autosystem\DBConn::getTableNameFor('amail_emails_YYYY');
       $sql='SELECT `'.$aeY.'`.* FROM `'.$aeY.'` ';
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
    public function GET_amail_emails_YYYYsSummary($location,$templatename,$view,$accountid,$id){// TODO: check params
        $sql= $this->GET_amail_emails_YYYYSQL($accound_id,$accountid,$id);
        $len =$this->GET_COUNT($this->get_amail_emails_YYYYCountSQL($accound_id,$accountid,$id));
        $this->paginator->setlistlength($len);
        $this->paginator->setpagenr($pagenr);
        $this->paginator->setpaginatorreload($location,$templatename);
        // TODO: check next line and uncomment
        //parent::GET_Summary($location,$templatename,$view,$sql,null,true,$accound_id);
        $view->getpaginator($this->paginator->getdata());
        print 'NOT IMPLEMENTED';
    }
    private function get_amail_emails_YYYYCountSQL($accound_id,$accountid,$id){
       $aeY= \autosystem\DBConn::getTableNameFor('amail_emails_YYYY');
       $sql='SELECT COUNT(*) AS `count`FROM `'.$aeY.'` ';
       $sql.= $where;
       return $sql;
    }
    public function save_contenttopartsequence($amail_contentpart_id,$amail_content_id,$seqranking,$type){ 
       $mcs= \autosystem\DBConn::getTableNameFor('amail_contenttopart_sequence');
       $key ='INSERT INTO `'.$mcs.'`(`amail_contentpart_id`,`amail_content_id`,`seqranking`,`type`) '.
                            'VALUES( :amail_contentpart_id, :amail_content_id, :seqranking, :type)';
       $sql[$key] = [':amail_contentpart_id'=>$amail_contentpart_id,':amail_content_id'=>$amail_content_id,':seqranking'=>$seqranking,':type'=>$type];//TODO CHECK insert save_ModerateEmailsMailcontenttopartsequence
       return $this->transaction($sql);
    }
    public function update_ModerateEmailsMailcontenttopartsequence($id,$amail_contentpart_id,$amail_content_id,$seqranking,$type){ 
       $mcs= \autosystem\DBConn::getTableNameFor('amail_contenttopart_sequence');
       $key ='UPDATE `'.$mcs.'` SET  `amail_contentpart_id`=:amail_contentpart_id,`amail_content_id`=:amail_content_id,`seqranking`=:seqranking,`type`=:type` WHERE id=:id';
       
       $sql[$key]= [':id'=>$id,':amail_contentpart_id'=>$amail_contentpart_id,':amail_content_id'=>$amail_content_id,':seqranking'=>$seqranking,':type'=>$type];//TODO CHECK update update_ModerateEmailsMailcontenttopartsequence
       return is_array($this->transaction($sql));      
    }
    public function delete_ModerateEmailsMailcontenttopartsequence($id){
       $mcs= \autosystem\DBConn::getTableNameFor('amail_contenttopart_sequence');
       $key ='DELETE FROM `'.$mcs.'` WHERE `id`=:id';//
       $value = [':id'=>$id];//TODO check if there need more checks before use delete_ModerateEmailsMailcontenttopartsequence
       $sql[$key]= $value;
       return $this->transaction($sql);
    }
    public function exist_ModerateEmailsMailcontenttopartsequence($id){
       $mcs= \autosystem\DBConn::getTableNameFor('amail_contenttopart_sequence');
       $key ='SELECT * FROM `'.$mcs.'` WHERE `id`=:id ;';//
       $value = [':id'=>$id];//TODO check if there are more parameters to use exist_ModerateEmailsMailcontenttopartsequence
       return $this->isQuery($key,$value);
    }
    public function GET_amail_contenttopart_sequence($location,$templatename,$view,$accountid,$id){
       $mcs= \autosystem\DBConn::getTableNameFor('amail_contenttopart_sequence');
       $sql='SELECT `'.$mcs.'`.* FROM `'.$mcs.'` ';
        $where = '';//
        $link ='WHERE';
    }
    private function GET_amail_contenttopart_sequenceSQL($accountid,$id){
       $mcs= \autosystem\DBConn::getTableNameFor('amail_contenttopart_sequence');
       $sql='SELECT `'.$mcs.'`.* FROM `'.$mcs.'` ';
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

    private function get_amail_contenttopart_sequenceCountSQL($accound_id,$accountid,$id){
       $mcs= \autosystem\DBConn::getTableNameFor('amail_contenttopart_sequence');
       $sql='SELECT COUNT(*) AS `count`FROM `'.$mcs.'` ';
       $sql.= $where;
       return $sql;
    }
    public function GET_ModerateEmails($location,$templatename,$view,$account_id,$id){
       $ac= \autosystem\DBConn::getTableNameFor('amail_category');
       $ac= \autosystem\DBConn::getTableNameFor('amail_conceptmail');
       $as= \autosystem\DBConn::getTableNameFor('amail_style');
       $ac= \autosystem\DBConn::getTableNameFor('amail_content');
       $ac= \autosystem\DBConn::getTableNameFor('amail_contentpart');
       $ad= \autosystem\DBConn::getTableNameFor('amail_dataset');
       $aa= \autosystem\DBConn::getTableNameFor('amail_adminlog');
       $aeY= \autosystem\DBConn::getTableNameFor('amail_emails_YYYY');
       $mcs= \autosystem\DBConn::getTableNameFor('amail_contenttopart_sequence');
       $sql='SELECT `'.$mcs.'`.* ,`'.$aeY.'`.* ,`'.$aa.'`.* ,`'.$ad.'`.* ,`'.$ac.'`.* ,`'.$ac.'`.* ,`'.$as.'`.* ,`'.$ac.'`.* ,`'.$ac.'`.* FROM `'.$ac.'` LEFT JOIN `'.$ac.'`.ON .`'.$ac.'`.`id=`'.$ac.'`.`amail_conceptmail_id`LEFT JOIN `'.$as.'`.ON .`'.$ac.'`.`id=`'.$as.'`.`amail_style_id`LEFT JOIN `'.$ac.'`.ON .`'.$ac.'`.`id=`'.$ac.'`.`amail_content_id`LEFT JOIN `'.$ac.'`.ON .`'.$ac.'`.`id=`'.$ac.'`.`amail_contentpart_id`LEFT JOIN `'.$ad.'`.ON .`'.$ac.'`.`id=`'.$ad.'`.`amail_dataset_id`LEFT JOIN `'.$aa.'`.ON .`'.$ac.'`.`id=`'.$aa.'`.`amail_adminlog_id`LEFT JOIN `'.$aeY.'`.ON .`'.$ac.'`.`id=`'.$aeY.'`.`amail_emails_YYYY_id`LEFT JOIN `'.$mcs.'`.ON .`'.$ac.'`.`id=`'.$mcs.'`.`amail_contenttopart_sequence_id`';
        $where = '';//
        $link ='WHERE';
    }
    private function GET_SQL($account_id,$id){
       $ac= \autosystem\DBConn::getTableNameFor('amail_category');
       $ac= \autosystem\DBConn::getTableNameFor('amail_conceptmail');
       $as= \autosystem\DBConn::getTableNameFor('amail_style');
       $ac= \autosystem\DBConn::getTableNameFor('amail_content');
       $ac= \autosystem\DBConn::getTableNameFor('amail_contentpart');
       $ad= \autosystem\DBConn::getTableNameFor('amail_dataset');
       $aa= \autosystem\DBConn::getTableNameFor('amail_adminlog');
       $aeY= \autosystem\DBConn::getTableNameFor('amail_emails_YYYY');
       $mcs= \autosystem\DBConn::getTableNameFor('amail_contenttopart_sequence');
       $sql='SELECT `'.$mcs.'`.* ,`'.$aeY.'`.* ,`'.$aa.'`.* ,`'.$ad.'`.* ,`'.$ac.'`.* ,`'.$ac.'`.* ,`'.$as.'`.* ,`'.$ac.'`.* ,`'.$ac.'`.* FROM `'.$ac.'` LEFT JOIN `'.$ac.'`.ON .`'.$ac.'`.`id=`'.$ac.'`.`amail_conceptmail_id`LEFT JOIN `'.$as.'`.ON .`'.$ac.'`.`id=`'.$as.'`.`amail_style_id`LEFT JOIN `'.$ac.'`.ON .`'.$ac.'`.`id=`'.$ac.'`.`amail_content_id`LEFT JOIN `'.$ac.'`.ON .`'.$ac.'`.`id=`'.$ac.'`.`amail_contentpart_id`LEFT JOIN `'.$ad.'`.ON .`'.$ac.'`.`id=`'.$ad.'`.`amail_dataset_id`LEFT JOIN `'.$aa.'`.ON .`'.$ac.'`.`id=`'.$aa.'`.`amail_adminlog_id`LEFT JOIN `'.$aeY.'`.ON .`'.$ac.'`.`id=`'.$aeY.'`.`amail_emails_YYYY_id`LEFT JOIN `'.$mcs.'`.ON .`'.$ac.'`.`id=`'.$mcs.'`.`amail_contenttopart_sequence_id`';
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
        $this->paginator->setSort('');//TODO ORDER BY `???`.`inputDate` DESC
        $this->paginator->setRange(20);
        return $sql;
    }
    public function GET_ModerateEmailssSummary($location,$templatename,$view,$account_id,$id){// TODO: check params
        $sql= $this->GET_SQL($accound_id,$account_id,$id);
        $len =$this->GET_COUNT($this->get_CountSQL($accound_id,$account_id,$id));
        $this->paginator->setlistlength($len);
        $this->paginator->setpagenr($pagenr);
        $this->paginator->setpaginatorreload($location,$templatename);
        // TODO: check next line and uncomment
        //parent::GET_Summary($location,$templatename,$view,$sql,null,true,$accound_id);
        $view->getpaginator($this->paginator->getdata());
    }
    private function get_CountSQL($accound_id,$account_id,$id){
       $ac= \autosystem\DBConn::getTableNameFor('amail_category');
       $ac= \autosystem\DBConn::getTableNameFor('amail_conceptmail');
       $as= \autosystem\DBConn::getTableNameFor('amail_style');
       $ac= \autosystem\DBConn::getTableNameFor('amail_content');
       $ac= \autosystem\DBConn::getTableNameFor('amail_contentpart');
       $ad= \autosystem\DBConn::getTableNameFor('amail_dataset');
       $aa= \autosystem\DBConn::getTableNameFor('amail_adminlog');
       $aeY= \autosystem\DBConn::getTableNameFor('amail_emails_YYYY');
       $mcs= \autosystem\DBConn::getTableNameFor('amail_contenttopart_sequence');
       $sql='SELECT COUNT(*) AS `count`FROM `'.$ac.
       '` LEFT JOIN `'.$ac.'`.ON .`'.$ac.'`.`id=`'.$ac.'`.`amail_conceptmail_id` '.
       'LEFT JOIN `'.$as.'`.ON .`'.$ac.'`.`id=`'.$as.'`.`amail_style_id`LEFT JOIN `'.$ac.'`.ON .`'.$ac.'`.`id=`'.$ac.'`.`amail_content_id`LEFT JOIN `'.$ac.'`.ON .`'.$ac.'`.`id=`'.$ac.'`.`amail_contentpart_id`LEFT JOIN `'.$ad.'`.ON .`'.$ac.'`.`id=`'.$ad.'`.`amail_dataset_id`LEFT JOIN `'.$aa.'`.ON .`'.$ac.'`.`id=`'.$aa.'`.`amail_adminlog_id`LEFT JOIN `'.$aeY.'`.ON .`'.$ac.'`.`id=`'.$aeY.'`.`amail_emails_YYYY_id`LEFT JOIN `'.$mcs.'`.ON .`'.$ac.'`.`id=`'.$mcs.'`.`amail_contenttopart_sequence_id`';
       $sql.= $where;
       return $sql;
    }
  public function getModelInfo(){  return 'amail.moderateEmails.model.info';  }
  public function getVersionNr(){  return (float)5.4;  }
  public function getRuleDefault(){return 'MODERATOR';} 
}
?>