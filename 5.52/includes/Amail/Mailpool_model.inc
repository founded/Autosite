<?php
namespace Amail;
class MailPool_model extends \Amail\Emails_model{
    public $chartstati = array( 'SENDED'=>'sended','READED' => 'readed','PREPARED' => 'prepared');
    private $config ;
    private $poolsize ;
    protected $mailhost;
    protected $mailport;
    /*private function __getsettings(){
        $this->config = ROOT.'includes'.DIRECTORY_SEPARATOR.'system'.DIRECTORY_SEPARATOR.'MpoolConfig.ini.php';
        if(file_exists($this->config)&& is_readable($this->config)&& $config = parse_ini_file($this->config)){
               $this->poolsize = $config['POOLSIZE'];
               $this->mailhost = $config['MAILHOST'];
               $this->mailport = $config['MAILPORT'];
        }
    }  */      
    public function __upgrade($version){return true;    }
    public function getModelInfo(){     return 'pool.Amail.model.info';  }
    public function getVersionNr(){     return (float)5.4;  }
    public function GET_MailPoolSummary($location,$templatename,&$view,$accound_id,$enabled,$filters=[]){
        if($this->existTableAmail_emails()){
            $a = \autosystem\DBConn::getTableNameFor('accound');
            $e = \autosystem\DBConn::getTableNameFor('amail_emails').$this->mailextendor.'';
            
            $sql='SELECT `'.$e.'`.* , IF(`SENDIBLE`=\'TRUE\',\' checked=checked \',\' \') AS `issendible` '.
            ',`accound`.username,`accound`.email_id '.
            'FROM `'.$e.'` '.
            'LEFT JOIN `'.$a.'` ON `'.$a.'`.`id` = `'.$e.'`.accound_id '.
            //'LEFT JOIN `'.$t.'` ON `template_id`=`'.$t.'`.`id` LEFT JOIN `'.$c.'` ON `content_id`=`'.$c.'`.`id`'.
            '';
            //print $sql;
            parent::GET_Summary($location,$templatename,$view,$sql);
        }        
    }
    public function save($content_id,$accound_id,$statuscomment =''){
            $p = \autosystem\DBConn::getTableNameFor('amail_emails').$this->mailextendor;
            $sql[$key]= $value;
            return $this->transaction($sql);
    }
    public function getRuleDefault(){     return 'KING';    }
    public function isInstalled(){        return parent::isInstalled()&& ($this->existTableAmail_emails()|| $this->__installEmails());
    }
    public function GETPoolItems($location,$templatename,$view,$accound_id,$enabled){
        $sql=$this->poolsql();
        parent::GET_Summary($location,$templatename,$view,$sql);
    }
    public function GET_Contents($category= 0){
        return [];
    }
    public function GET_Templates($category= 0){
        return [];
    }
    public function addTESTmail($data,$accound_id){
        $table = $this->getEmailtbl();
        $concept_id = $this->getConceptId('activation');
        $key ='INSERT INTO `'.$table.'`( `id`,`STATUS`, `conceptmail_id` , `accound_id`,`imputDate`,`subject`)'.
              ' VALUES(NULL, \'SENDIBLE\',:conceptmail_id,:accound_id,:imputDate ,:subject )' ;

        $value = [':conceptmail_id'=>$data['concept'],':accound_id' => $accound_id,
                  ':imputDate' => $this->nowDateTime(),':subject'=>$data['subject']];
        $sql[$key]= $value;
        return $this->transaction($sql);
    }
    private function poolsql($accound_id=null,$id=null){
        /*$e = \autosystem\DBConn::getTableNameFor('amail_emails').$this->mailextendor;
        $t = \autosystem\DBConn::getTableNameFor('amail_templates');
        $c = \autosystem\DBConn::getTableNameFor('amail_content');
        $a = \autosystem\DBConn::getTableNameFor('accound');
        $em = \autosystem\DBConn::getTableNameFor('email');
        $sql='SELECT `'.$t.'`.* ,`'.$c.'`.* , `content_key` ,`'.$e.'`.* ,`'.$a.'`.* , `'.$em.'`.* '.
        ' FROM `'.$e.'` '.
        ' LEFT JOIN `'.$t.'` ON `template_id`=`'.$t.'`.`id`'.
        ' LEFT JOIN `'.$c.'` ON `content_id`=`'.$c.'`.`id`'.
        ' LEFT JOIN `'.$a.'` ON `'.$e.'`.`accound_id`=`'.$a.'`.`id`'.
        ' LEFT JOIN `'.$em.'` ON `email_id`=`'.$em.'`.`id`';
        $where = 'WHERE `SENDIBLE`=\'TRUE\' and (`'.$e.'`.`STATUS`=\'SENDIBLE\' )';
        if(!empty($accound_id)){
            $where.= $link.' `accound_id`=\''.$accound_id.'\'';
            $link =' && ';
        }
        if(!empty($id)){
            $where.= $link.' `id`=\''.$id.'\'';
            $link =' && ';
        }
        $sql.= $where;
        print $sql ;*/
        return $sql;
    }
    public function amountUnsubscribe($DateFrom,$Dateto){     return 50; }
    public function amountReaded($DateFrom,$Dateto){          return 50; }
    public function amountSended($DateFrom,$Dateto){        return 60;   }        
}
?>