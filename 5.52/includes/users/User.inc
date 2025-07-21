<?php
/**
 * @author Lieven Roegiers
 * @CMS autosite
 */
namespace users;
class User extends Visitor{
    private $id;
    private $accound_id; 
    private $TYPE; // ENUM('INSIDER','OUTSIDER') NOT NULL,
    private $STATUS;//ENUM('ACTIVE','ERROR','INACTIVE')NOT NULL  default 'ACTIVE' ,
    private $name;
    private $rule_id;
    private $adress_id;
    private $phone_id;
    private $user_path;
    private $islogin;
    private $userrules;
    private $actualrulls;
    private $personalpath;
    private $attacked = false;
    public function __construct($key =''){
        if($key !=''){            $this->loginkey = $key; }
        parent::__construct();
        $this->actualrulls = $this->userrules;
    }
    public function login($username,$password,$cripty){
        $a = \autosystem\DBConn::getTableNameFor('accound');
        $u = \autosystem\DBConn::getTableNameFor('user');
        if(isset($this->counter)){   $this->counter=$this->counter+1;
        }else{            			 $this->counter = 0;
        }
        $this->poging++;
        $database = \autosystem\DBConn::GET();
        $dbStmt = $database->prepare('SELECT '.$a.'.* ,'.$u.'.`STATUS` FROM '.$a.' LEFT JOIN '.$u.' ON '.$a.'.`id` = '.$u.'.`accound_id`  WHERE `ACTIVATED`=\'TRUE\' and `ENABLED`=\'TRUE\' and `username`=:username ');
        $dbStmt->execute([':username'=>$username]);
        $row = $dbStmt->fetch(\PDO::FETCH_ASSOC);
        $cripty = (is_array($row)&&key_exists('pwdx',$row))?$this->cripty($row['pwdx'],$cripty):'';
        
        $this->islogin = $cripty === $password && $row['ACTIVATED'] === 'TRUE';
        $this->STATUS=$row['STATUS'];
        $datearr= [':year'=>date('Y'),':month'=>date('m'),':day'=>date('d')];
        $this->name = $username;
		if($this->islogin){          
            $this->setRemoteip();
            $this->accound_id = $row['id'].'';
            $this->userrules = $this->getAcesses($database,$this->accound_id);
            $this->actualrulls = $this->userrules;
            $this->addstat('users',$datearr);
            $this->addstat('validlogins',$datearr);
            $this->log('VALID user #');
            return $this->islogin();
        }else{
            $this->addstat('invalidlogins',$datearr);
            $this->ItryLogin();
            $this->log('TRY login @'.$username);
            return false;
        }
    }
    private function cripty($pass,$cript){        return md5($this->loginkey.$pass);   }
    private function getAcesses($database,$accoundId){
        $return =['USER','PUBLIC'];
        $u = \autosystem\DBConn::getTableNameFor('user');
        $r = \autosystem\DBConn::getTableNameFor('user_rull');
        $set = \autosystem\DBConn::getTableNameFor('user_rullset');
        $dbSql = 'SELECT `'.$set.'`.`TYPE` FROM `'.$u.'` LEFT JOIN `'.$r.'` ON `'.$u.'`.`rule_id`=`'.$r.'`.`id` LEFT JOIN `'.$set.'` ON `'.$u.'`.`rule_id`=`'.$set.'`.`rule_id`' ;
        $dbSql .= ' WHERE `accound_id`=:accoundid ;';
        $database = \autosystem\DBConn::GET();
        $arr = [':accoundid'=>$accoundId];
        if($dbStmt = $database->prepare($dbSql)){
            $dbStmt->execute($arr);
            foreach ($dbStmt->fetchAll(\PDO::FETCH_COLUMN) as $row){
                if(!empty($row)&& !in_array($row,$return)){            array_push($return,$row);      } 
            } 
        }
        return $return;
    }
    public function islogin(){       return $this->islogin && $this->logintriggers();//&& $this->isRemoteip();//;    
    }
    public function logout(){        $this->islogin = false;    }
    public function checkup(){       /*print 'checkup user';*/   }
    public function isActive(){ 	 return $this->STATUS=='ACTIVE'; }
    public function getAllRules(){   return ($this->islogin)?$this->userrules:parent::getAllRules(); }
    public function getRules(){      return ($this->islogin)?$this->actualrulls:parent::getRules();  }
    public function getUserName(){	 return $this->name;}
    public function setPersonalPath($path){	    $this->personalpath = $path; }
	public function getPersonalPath(){		    return $this->personalpath;	}
    public function isMykey($key){
        $a = \autosystem\DBConn::getTableNameFor('accound');
        $database = \autosystem\DBConn::GET();
        $dbStmt = $database->prepare('SELECT `key` FROM '.$a.' WHERE `ENABLED`=\'TRUE\' and `key`=:key and `id`=:id');
        $dbStmt->bindParam(':key',$key);
        $dbStmt->bindParam(':id',$this->accound_id);
        $dbStmt->execute();
        $row = $dbStmt->fetch(\PDO::FETCH_ASSOC);
        return $row['key'] === $key;
    }
    //TODO
    private function logintriggers(){        return true;    }
    //TODO
    private function isenabled(){            return true;    }
    //TODO
    private function isopensession(){        return true;    }
    public function __call($name, $arguments) {
        if($this->attacked){
            $this->islogin=false;
            @trigger_error('Session Hijacking detected!', E_USER_WARNING);
        }
    }
    private function isRemoteip(){
        print "$this->remoteip !== $this->getIp(";
        if($this->remoteip !== $this->getIp()){
            $this->attacked = true;
            $this->islogin = false;
        }
		return $this->remoteip == $this->getIp();
	}
    private function getAdressId(){       $this->adress_id;    }
    private function getphoneId(){        $this->phone_id;     }
    public function setActualrulles($arr){
        if(is_array($arr)){
            $isok= true;
            foreach($arr as $rull){//vallidation of rulles
                if(!in_array($rull,$this->userrules)){
                    $isok=false;
                }
            }
            if($isok==true){      $this->actualrulls = $arr;   }
        }
    }
    public function __toString(){
        return '@access:'.$this->counter.'<br> I am a user :'.$this->name.'<br> IP:'.$this->remoteip.'<br>  Browser:'.$this->browser.'<br>recent['.']<br>actual rulles['.print_r($this->actualrulls,true).']islogin '.(($this->islogin==true)?'true':'false').' LoginPoging:'.$this->poging .' loginkey:'.$this->loginkey;
    }
    public function getAccound_id(){     return $this->accound_id; }
    public function hasRulle($rulle){    return in_array($rulle,$this->userrules); }
    public function renewpwx($old,$new){
        $a = \autosystem\DBConn::getTableNameFor('accound');
        if($this->accound_id ===0){            throw new Exception('unable to renew password by zero accound');  }
        $database = \autosystem\DBConn::GET();
        $dbStmt = $database->prepare('SELECT `pwdx` FROM '.$a.' WHERE `ACTIVATED`=\'TRUE\' and `ENABLED`=\'TRUE\' and `id`=:id');
        $dbStmt->bindParam(':id',$this->accound_id);
        $dbStmt->execute();
        $row = $dbStmt->fetch(\PDO::FETCH_ASSOC);
        if($row['pwdx'] === $old){
            $dbStmt = $database->prepare('UPDATE '.$a.' SET `pwdx`=:pwdx ,`updateDate`=:updateDate WHERE `id`=:id');//`key`=:key 
            $dbStmt->bindParam(':id',$this->accound_id);
            $dbStmt->bindParam(':pwdx',$new);
            $dbStmt->bindParam(':updateDate',date( 'Y-m-d H:i:s', time()));
            return $dbStmt->execute();
        }
    }
    public function lostpwx($key,$new){
        $a = \autosystem\DBConn::getTableNameFor('accound');
        $database = \autosystem\DBConn::GET();
        $dbStmt = $database->prepare('SELECT * FROM '.$a.' WHERE `ACTIVATED`=\'TRUE\' and `ENABLED`=\'TRUE\' and `key`=:key');
        $dbStmt->bindParam(':key',$key);
        $dbStmt->execute();
        $row = $dbStmt->fetch(\PDO::FETCH_ASSOC);
        if($row['pwdx'] === $old){
            $dbStmt = $database->prepare('UPDATE '.$a.' SET `pwdx`=:pwdx ,`updateDate`=:updateDate WHERE `key`=:key');// 
            $dbStmt->bindParam(':key',$key);
            $dbStmt->bindParam(':pwdx',$new);
            $dbStmt->bindParam(':updateDate',date( 'Y-m-d H:i:s', time()));
            $dbStmt->execute();
            return $dbStmt->fetch(\PDO::FETCH_ASSOC);
        }
    }
    public function log($lastlocation = ''){
        $database = \autosystem\DBConn::GET();
        $i = \autosystem\DBConn::getTableNameFor('user_logins');
        $dbStmt = $database->prepare('INSERT INTO '.$i.'(`id`,`accound_id`,`ip`,`lastlocation`,`browser`,`time`)'.
                       'VALUES(NULL,:accoundid,:ip,:lastlocation,:browser,:time) ');
        $param=[    ':accoundid'=>$this->accound_id,
                    ':ip'=>$this->getIp(),
                    ':lastlocation'=>$lastlocation,
                    ':browser'=>$this->getBrowser(),
                    ':time'=>date( 'Y-m-d H:i:s', time())];
        $dbStmt->execute($param); 
    }
    public function cast($object){
        if (is_object($object)&&is_subclass_of($object,'Visitor')) {
            foreach($object as $key => $value){            $this->$key = $value;          }
        }
    }
}     
?>