<?php
namespace autosystem;
class SessionAuthException extends \RuntimeException{
    public function __construct($message = 'Unknown reason', $code = 0, Exception $previous = null){
        parent::__construct('Session could not be authenticated: ' . $message, $code, $previous);
    }
}
class Prosession extends \autosystem\Model implements \SessionHandlerInterface{   
    private $link;
    private $cOptions = [ 'regen_interval' => 600, 'idle_timeout' => 3600];
    private $t_currentrequest;
    private $numWrites;
    private $isNew = false;
    private $doExpire = false;
    private $PROS_ID = '';//prosession id
    private $PROS_LEN;//prosession id length
    protected $DB;
    public function __construct(bool $autoInit = true){
        $this->DB = \autosystem\DBConn::GET();
        $this->t_currentrequest = $this->nowDateTime();//$_SERVER['REQUEST_TIME'];
        //$this->nowDateTime();
        if( $autoInit ){
            //session_set_save_handler($this);
        }
        return;
    }
    public function start(array $options = []){
            @session_start($options);
    }
    public function open($savePath, $sessionName) : bool{
        /* mysqli->ping() returns null if connection has been closed */
        //return @$this->link->ping() ?: false;
        return true ;//!($this->DB);
    }
    public function create_sid(){
        $checkCollision = session_status() == PHP_SESSION_ACTIVE;
        $sid_len = ini_get('session.sid_length');
        $key = '';
        $attempts = 0;
        do{
            if( $attempts >= 5){
                throw new Exception('generate sessionkey fail please start cleaning see gc');
            }
            $key = \autosystem\Key::GET($sid_len);
            $attempts++;
        }while( $checkCollision && $this->existSession($key));
        $this->PROS_ID = $key;
        $this->issessionnew = true;
        return $key;
    }
    public function validateId(string $sid) : bool{
        return $this->isNew ?: $this->existSession($sid);//todo check if need change to ||
    }
    public function read($sid) : string{
        try{
            if( $this->isNew ){
                $this->numWrites = 0;
                $out = '';
            }elseif( ($result = $this->querySession($sid)) ){
                $isexpired = false;
                if(!isset($result) ||$isexpired){
                    $this->isNew = true;
                    $this->numWrites = 0;
                    $out = '';
                    //throw new SessionAuthException('Session is expired');
                }else{
                    $this->isNew = false;
                    $this->numWrites = $result['write_try'];
                    $this->PROS_ID = $result['session_id'];
                    //print $this->PROS_ID.'<br>';
                    $out = \stripslashes($result['session_data']);
                }
            }else{
                $this->isNew = true;
                $this->numWrites = 0;
                $out = '';
            }
            return $out;
            }catch(PDOException $e){           $this->Modellog($e,'GET_Array',__LINE__,$sql );}
    }
    private function querySession(string $sid) : ?array{
       $ap= \autosystem\DBConn::getTableNameFor('accound_prosessions');
       $sql=[];
       $key ='SELECT * FROM accound_prosessions WHERE `session_id` = :session_id ';
       $res = $this->GET_Stmt($key,[':session_id' => $sid]);
       return ($res !== false)?$res : null;
        
    }
    public function write($sessionId, $data) : bool{
        $host = $_SERVER['HTTP_HOST'];
        //$d = (array) unserialize($data);
        //print '<pre>'.$data;
     //var_dump($data);
        //print '</pre>';
        /*if(key_exists('user',$d)){
            print 'forms';
        }*/
        $row =[':session_id'=>$sessionId,':host'=>$host, ':session_data'=>\addslashes($data) ];

        if(!$this->isNew ){     
            return $this->overwriteSession($sessionId, $row);
        } 
        return $this->newSession($sessionId, $row);
    }
    private function newSession($sessieId, $row){
       $ap= \autosystem\DBConn::getTableNameFor('accound_prosessions');
       $sql=[];
       $key ='INSERT INTO `'.$ap.'`(`session_id`,`write_try`,`host`,`session_data`)'.
           'VALUES(:session_id , 1,:host,:session_data )';
       $sql[$key] = $row;
       return $this->transaction($sql)!==false;
    }
    private function overwriteSession($sessieId, $row){
       $ap= \autosystem\DBConn::getTableNameFor('accound_prosessions');
       $sql=[];
       $key ='UPDATE `'.$ap.'` SET '.
           '`write_try` = `write_try` + 1,`host`=:host,`session_data`=:session_data WHERE `session_id`=:session_id ';      
       $sql[$key] = $row;
       return $this->transaction($sql)!==false;
    }
    private function existSession($id){
       $ap= \autosystem\DBConn::getTableNameFor('accound_prosessions');
       $sql ='SELECT * FROM `'.$ap.'` WHERE `session_id`=:id ';//
       $param = [':id'=>$id];
       return $this->isQuery($sql,$param) ;
    }
    public function pagenote($sessionId,$acc_id,$browser,$ip ,$name ,$haslogin, $language ){
        $loggedIn= ($haslogin)?'TRUE':'FALSE';
        $row =[':session_id'=>$sessionId,
       ':accound_id'=>$acc_id ,':browser'=>$browser, ':ip'=>$ip ,':name'=>$name ,':HASLOGIN'=>$loggedIn ,':lang'=>$language         
       ];
      if ($this->existSession($sessionId)) {
            $this->overwriteSessionnote($sessionId, $row);               
        } else {
            $this->anewSessionnote($sessionId,$row);
        }
    }
    private function anewSessionnote($sessieId, $row){
       $ap= \autosystem\DBConn::getTableNameFor('accound_prosessions');
       $sql=[];
       $key ='INSERT INTO `'.$ap.'`(`session_id`,`accound_id`,`browser`,`ip`,`HASLOGIN`,`name`,`language`)'.
           'VALUES(:session_id ,:accound_id,:browser ,:ip,:HASLOGIN,:name,:lang )';
       $sql[$key] = $row;
       return $this->transaction($sql);
    }
    private function overwriteSessionnote($sessieId, $row){
       $ap= \autosystem\DBConn::getTableNameFor('accound_prosessions');
       $sql=[];
       $key ='UPDATE `'.$ap.'` SET '.
           '`accound_id`=:accound_id ,`browser`=:browser ,`ip`=:ip ,`HASLOGIN`=:HASLOGIN ,`name`=:name ,`language`=:lang'.
           ' WHERE `session_id`=:session_id ';
       $sql[$key] = $row;
       return $this->transaction($sql);
    }
    public function addInfo($sesid,$url,$time,$ack,$accound_id){      
       $ap= \autosystem\DBConn::getTableNameFor('accound_prosessions_info');
       $sql=[];
       $key ='INSERT INTO `'.$ap.'`(`id`,`session_id`,`url`,`time`,`ACK`,`accound_id`)'.
           'VALUES(NULL,:session_id ,:url,:time,:ACK,:accound_id)';
       $sql[$key] = [':session_id'=> session_id(),':url'=>$url,':time'=>$this->nowDateTime(),':ACK'=>$ack,':accound_id'=>$accound_id];
      // print $sesid .'<br>'.session_id();
       return $this->transaction($sql);
    }
    public function __INSTALL(){        
        if(!$this->existTableLabel('accound_prosessions')){
           $sql = 'CREATE TABLE IF NOT EXISTS `'.\autosystem\DBConn::getTableNameFor('accound_prosessions').'`('.
           '`session_id` varchar(48) NOT NULL,'.
           '`host` VARCHAR (100) COLLATE '.\autosystem\DBConn::getCOLLATE().' DEFAULT NULL,'.
           '`created` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,'.
           '`STATUS` ENUM(\'KILL\',\'LOGOUT\',\'OPEN\',\'END\',\'START\',\'ENABLED\',\'DESTROY\')NOT NULL DEFAULT \'START\','.//TODO check DEFAULT parameter
           '`lastonline` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,'.
           '`session_data` text ,'.
           '`accound_id` INT (20) DEFAULT 0,'.
           '`browser` VARCHAR (1000) COLLATE '.\autosystem\DBConn::getCOLLATE().' DEFAULT NULL,'.
           '`write_try` int(6) DEFAULT NULL,'.
           '`ip` VARCHAR (230) COLLATE '.\autosystem\DBConn::getCOLLATE().' DEFAULT NULL,'.
           '`name` VARCHAR (100) COLLATE '.\autosystem\DBConn::getCOLLATE().' DEFAULT NULL,'.
           '`HASLOGIN` ENUM(\'TRUE\',\'FALSE\')NOT NULL DEFAULT \'FALSE\','.
           '`SUPERVISION` ENUM(\'TRUE\',\'FALSE\')NOT NULL DEFAULT \'FALSE\','.
           '`language` VARCHAR (100) COLLATE '.\autosystem\DBConn::getCOLLATE().' DEFAULT NULL,'.
           '`location_id` INT (9) DEFAULT NULL,'.
          ' PRIMARY KEY (`session_id`)'.
         ')ENGINE = InnoDB DEFAULT CHARSET='.\autosystem\DBConn::getCHARSET().' COLLATE='.\autosystem\DBConn::getCOLLATE().' ;';
         //print($sql);
         $result1 = $this->DB->query($sql);
         $return1 = $result1->rowCount()==1;
        }else{
         $return1 = true;
        }
        if(!$this->existTableLabel('accound_prosessions_info')){
         $sql = 'CREATE TABLE IF NOT EXISTS `'.\autosystem\DBConn::getTableNameFor('accound_prosessions_info').'`('.
           '`id` int(10) NOT NULL AUTO_INCREMENT,'.
           '`session_id` varchar(48) NOT NULL'.
           '`url` VARCHAR (1000) COLLATE '.\autosystem\DBConn::getCOLLATE().' NOT NULL,'.
           '`time` timestamp NOT NULL,'.
           '`TRIGER` ENUM(\'TRUE\',\'FALSE\')NOT NULL DEFAULT \'FALSE\','.
           '`ACK` ENUM(\'TRUE\',\'FALSE\')NOT NULL DEFAULT \'FALSE\','.
           '`accound_id` INT (20) DEFAULT 0,'.
          ' PRIMARY KEY (`id`)'.
         ')ENGINE = InnoDB DEFAULT CHARSET='.\autosystem\DBConn::getCHARSET().' COLLATE='.\autosystem\DBConn::getCOLLATE().' ;';
         //print($sql);
         $result2 = $this->DB->query($sql);
         $return2 = $result2->rowCount()==1;
        }else{
         $return2 = true;
        }
        if(!$this->existTableLabel('accound_prosessions_forms')){
         $sql = 'CREATE TABLE IF NOT EXISTS `'.\autosystem\DBConn::getTableNameFor('accound_prosessions_forms').'`('.
           '`session_id` varchar(48) NOT NULL,'.
           '`formname` VARCHAR (1000) COLLATE '.\autosystem\DBConn::getCOLLATE().' NOT NULL,'.
           '`formdata` longtext NOT NULL,'.
           '`url` VARCHAR (1000) COLLATE '.\autosystem\DBConn::getCOLLATE().' NOT NULL,'.
           ' PRIMARY KEY (`session_id`)'.
         ')ENGINE = InnoDB DEFAULT CHARSET='.\autosystem\DBConn::getCHARSET().' COLLATE='.\autosystem\DBConn::getCOLLATE().' ;';
         //print($sql);
         $result3 = $this->DB->query($sql);
         $return3 = $result3->rowCount()==1;
        }else{
         $return3 = true;
        }
        if(!$this->existTableLabel('accound_prosessions_terminator')){
         $sql = 'CREATE TABLE IF NOT EXISTS `'.\autosystem\DBConn::getTableNameFor('accound_prosessions_terminator').'`('.
           '`session_id`  varbinary(192) NOT NULL,'.
           '`ip` VARCHAR (192) COLLATE '.\autosystem\DBConn::getCOLLATE().' NOT NULL,'.
           '`browser` longtext NOT NULL,'.
           '`BLOCKIP` ENUM(\'TRUE\',\'FALSE\')NOT NULL DEFAULT \'FALSE\','.
           '`BLOCKBROWSER` ENUM(\'TRUE\',\'FALSE\')NOT NULL DEFAULT \'FALSE\','.
           ' PRIMARY KEY (`session_id`)'.
         ')ENGINE = InnoDB DEFAULT CHARSET='.\autosystem\DBConn::getCHARSET().' COLLATE='.\autosystem\DBConn::getCOLLATE().' ;';
         print($sql);
         $result4 = $this->DB->query($sql);
         $return4 = $result4->rowCount()==1;
        }else{
         $return4 = true;
        }
        if(!$this->existTableLabel('accound_prosessions_unaspectlog')){
         $sql = 'CREATE TABLE IF NOT EXISTS `'.\autosystem\DBConn::getTableNameFor('accound_prosessions_unaspectlog').'`('.
           '`id` int(6) NOT NULL AUTO_INCREMENT,'.
           '`msg` longtext NOT NULL,'.
           ' PRIMARY KEY (`id`)'.
         ')ENGINE = InnoDB DEFAULT CHARSET='.\autosystem\DBConn::getCHARSET().' COLLATE='.\autosystem\DBConn::getCOLLATE().' ;';
         print($sql);
         $result5 = $this->DB->query($sql);
         $return5 = $result5->rowCount()==1;
        }else{
         $return5 = true;
        }        
        if(!$this->existTableLabel('accound_prosessions_stats')){
         $sql = 'CREATE TABLE IF NOT EXISTS `'.\autosystem\DBConn::getTableNameFor('accound_prosessions_stats').'`('.
           '`id` int(6) NOT NULL AUTO_INCREMENT,'.
           '`sessiontimesize` int(6),'.
           '`accound_id` INT (20),'.
           '`amout` INT (20),'.
           ' PRIMARY KEY (`id`)'.
         ')ENGINE = InnoDB DEFAULT CHARSET='.\autosystem\DBConn::getCHARSET().' COLLATE='.\autosystem\DBConn::getCOLLATE().' ;';
         $result6 = $this->DB->query($sql);
         $return6 = $result6->rowCount()==1;
        }else{
         $return6 = true;
        }
        return $return1 && $return2 && $return3&& $return4&& $return5&& $return6;
    }
    public function gc($maxLifetime) : bool{
        $past = time() - $maxlifetime;
        $ap= \autosystem\DBConn::getTableNameFor('accound_prosessions');
        $sql=[];
        $key ='DELETE FROM `'.$ap.'` WHERE `created` < :t ';      
        $sql[$key] = [':t' => $past];
        return $this->transaction($sql)!==false;
    }
    public function close() : bool{
        $numWrites = null;
        $this->isNew = false;
        return true;
    }
    public function destroy($sid) : bool{
       /*$ap= \autosystem\DBConn::getTableNameFor('accound_prosessions');
       $sql=[];
       $key ='UPDATE `'.$ap.'` SET `STATUS` = \'DESTROY\' WHERE `session_id`=:session_id ';      
       $sql[$key] = [':session_id' => $sid];*/
       return true;//$this->transaction($sql)!==false;
    }
    public function setStatus($status){
       $ap= \autosystem\DBConn::getTableNameFor('accound_prosessions');
       $sql=[];
       $key ='UPDATE `'.$ap.'` SET `STATUS` = \''.$status.'\' WHERE `session_id`=:session_id ';      
       $sql[$key] = [':session_id' => $this->getMyProSession_id()];
       return $this->transaction($sql)!==false; 
    }
    public function getMyProSessions(){
        return [];
    }
    public function getMyProSession_id(){
        return $this->PROS_ID;
    }
    public function __destruct(){
        parent::__destruct();/* This will not be called in the case of Exception - resource handle will persist until PHP GC happens */
        //@$this->link->close();
    }
    protected function __status(){
        return $this->getModelInfo();
    }
    public function __toString(){
        return 'PROSESSION handeler set by cms autosite'.$this->PROS_ID;
    }
    private function Sessionlog(&$e,$from,$line,$sql){
            global $_DEBUG;
            if($_DEBUG){'<div class="xdebug-error" >'. (($e!==false)?print_r($e). \autosystem\DEBUG::printERR_B($e):'').'</div>';}
            //print '<div class="xdebug-error" >'.$from.' '.$sql.'  LineNr'.$line.' FROM model : '.__LINE__;
            //print '</div>';
            $msg = 'on: '.date('M j Y - G:i:s').','.get_called_class().' model '.__file__.' :'.$from.' except:'.(is_object($e)&&method_exists($e,'getMessage')?$e->getMessage():$e).PHP_EOL;
            $msg .= $sql;
			file_put_contents('./log/ProSessionException.log', $msg, FILE_APPEND);
            return 'ok';      
    }
    public function isInstalled(){      return $this->__INSTALL();    }
    public function existDB(){          return $this->DB = \autosystem\DBConn::GET();    }
    public function getModelInfo(){     return 'prossion.by.lieven.roegiers';    }
    public function getVersionNr(){     return (float)5.4;  }
    public function getRuleDefault(){   return 'USER';    }
}