<?php 
namespace autosystem;
abstract class Access {
    protected $_rulles;
    protected $_user;
    protected $isuser=false;
    protected $islog=false;
    protected $acceptcookies;
    protected $userspath ='members';
    protected $prosession ;
    protected $htref ='https';
    protected $protocol = '';
    public function __construct($menufilter,$acceptcookies = true) {
        $this->acceptcookies;
        if(file_exists('.http')){
            $this->htref = 'http';
            $this->protocol=$this->htref;
        }else{
            $this->protocol='https';
            //TODO CHECK LINE https
            //$this->protocol=current(explode('/',$_SERVER['SERVER_PROTOCOL']));
        }
        
        if($acceptcookies){
            if(file_exists('.prosession')){
                $this->prosession = new \autosystem\ProSession();
                $this->prosession->__INSTALL();
                session_set_save_handler($this->prosession);
                @session_start();
            }else{
                @session_start();
            }
            if(!isset($_SESSION['user'])){
    	        if(isset($_SESSION['visitor'])){  $this->_user = unserialize($_SESSION['visitor']);
                    if(file_exists('.prosession')){
                        $sesid= session_id();//getLanguage
                        //$sessionId,$acc_id,$browser,$ip ,$name ,$haslogin, $language
                        $this->prosession->pagenote($sesid,$this->_user->getAccound_id(),$this->_user->getBrowser(),$this->_user->getIp() ,'visiter unknow',false,'-');
                    }
                    return;
                }
    	    }else{
    	       if($this->_user = unserialize($_SESSION['user'])){
                  $this->isuser=true; 
                  if(isset($menufilter)){          $this->setActualUserRulles($menufilter);        }
                  if($this->_user->islogin()){     $userName = $this->_user->getUserName();
					   $this->_user->setPersonalPath($this->getAccoundDir($userName));
				  }
                  if(file_exists('.prosession')){
                        $sesid= session_id();//getLanguage
                        $this->prosession->pagenote($sesid,$this->_user->getAccound_id(),$this->_user->getBrowser(),$this->_user->getIp() ,$userName ,true,'-');
                  }
                  return;
    	       }
    	    }
        }
        $this->_user = new \users\Visitor();
        $_SESSION['visitor'] = serialize($this->_user);
    }
    public function addProsessionInfo($location,$action){
         if(file_exists('.prosession')){
                $sesid= session_id();//getLanguage
                $this->prosession->addInfo($sesid,'/'.$location.'/'.$action.'/',time(),'true',$this->_user->getAccound_id());
         }
    }
    public function setNOLegalSession(){//need pro id on this class
        if(file_exists('.prosession')){
            //getLanguage
            //$sessionId,$acc_id,$browser,$ip ,$name ,$haslogin, $language
            //$this->prosession->setcrime();
        }
        return;
    }
    private function hasAccess($loc,$modelname){   return $this->_user->canIDo($loc,$modelname);    }
    private function getUser(){	    return $this->_user; }
    protected function getVisitor(){return $this->_user; }
    public function getAccound_id(){               return ($this->isuser)? $this->_user->getAccound_id(): false;    }
    public function haveIAccess($rulle){           return $this->_user->hasRulle($rulle);    }
    protected function SetActualUserRull($rull){   return ($this->isuser)? $this->_user->setActualrulles([$rull]):0;    }
    protected function setActualUserRulles($arr){  return($this->isuser)? $this->_user->setActualrulles($arr): 0;    }
    protected function setSessionUser(&$accound){
        try{
            if(isset($accound)&&$accound->islogin()){     $_SESSION['user']=serialize($accound);      return true; }
        }catch(exception $e){  }
        return false;
    }
    protected function getSession_id(){
        if(file_exists('.prosession')){
            return $this->prosession->getMyProSession_id();
        }else{
            return session_id();
        }
               
    }
    protected function setLoginWrongTry(){
        if(file_exists('.prosession')){
            $this->prosession->setStatus('LOGOUT');
        }
    }
    protected function setSessionVisitor(&$visitor){
        try{
            if(isset($visitor)){	$_SESSION['visitor']=serialize($visitor); unset($_SESSION['user']);  return true;
            }
            return false;
        }catch(exception $e){
            return false;
        }
    }
    protected function LogoutUser(){
        if(file_exists('.prosession')){
            $this->prosession->setStatus('LOGOUT');
        }
        try{
            if(isset($this->_user)&&$this->_user->islogin()){
                $vis = new \users\Visitor();
                $vis->cast($this->_user);
                $this->_user->logout();
                $_SESSION['REuser']=$_SESSION['user'];
                $_SESSION['visitor']=serialize($vis);
                unset($_SESSION['user']);
                $this->_user = $vis;
               return true;
            }else{
                if(isset($_SESSION['user'])){
                    $_SESSION['REuser'] = $_SESSION['user'];
                    $_SESSION['user'] = null;
                    unset($_SESSION['user']);
                }
                unset($_SESSION);//TODO if your want to recover user comment line
            }
            return false;
        }catch(exception $e){
            $this->accesslog('exception logoutuser'.$this->__toString());
            return false;
        }
    }
    public function GOESTO($loc,$modelname,$param = []){
        $parameters = (count($param)>0)? '?'.implode('&', $param):'';
        $this->redirect('./'.$loc.'/'.$modelname.'/'.$parameters);
    }
    public function redirect($url){
        $b = $this->baseurl($url);
        if (!headers_sent()){//new
			header('Location: '.$b);
            die('GOTO '.$b);
		}else {
			die('--'.$b.'--<a href="'.$b.'" > here the link</a>');
		}
        /*old
        $s=htmlspecialchars($_SERVER['PHP_SELF']);
		$len= strripos($s,'/');
		$l = htmlspecialchars($_SERVER['SERVER_NAME']).substr($s,0,$len);
		if (!headers_sent()){
			header('Location: '.$this->protocol.'://'.$l.'/'.$url);
            die('GOTO '.$this->protocol.'://'.$l.'/'.$url);
		}else {
			die('<a href="'.$this->protocol.'://'.$l.'" >here the link</a>');
		}*/
	}
    public function baseurl($urlextendion){
        $s=htmlspecialchars($_SERVER['PHP_SELF']);
		$len= strripos($s,'/');
		$l = htmlspecialchars($_SERVER['SERVER_NAME']).substr($s,0,$len);
        return $this->protocol.'://'.$l.'/'.$urlextendion;
    }
    public function goToProfile($force=false){
        if($force || !$this->_model->isInstalled()){
            $this->redirect('/users/profile/');
            die('please use header or :');
        }
    }
    public function canIDEBUG(){        return $this->_user->hasRulle('KING');    }
    protected function existAccoundDir($accoundName){     return is_dir('./'.$this->userspath.'/'.$accoundName);    }
    protected function canICreateAccounddir($accoundName){  return true;    }
    private function CreateAccounddir($accoundName){
		if(!@mkdir('./'.$this->userspath.'/'.$accoundName,0744,true)){
			$this->accesslog('fail to create personal userpath '.$accoundName.' dir','access.inc'.__LINE__);
			return false;
		}
		return true;
	}
    protected function getAccoundDir($accoundName){
        if($this->existAccoundDir($accoundName)){
            return './'.$this->userspath.'/'.$accoundName.'/';
        }else {
            if($this->canICreateAccounddir($accoundName)&& $this->CreateAccounddir($accoundName)){
                return './'.$this->userspath.'/'.$accoundName.'/';
            }else{
               $this->accesslog('exception path not exist can not create'.$this->userspath.'/'.$accoundName.'/'. __FILE__.__LINE__);
               return false;  
            }
        }
    }
    public function canIDo($location=''){
        $a = \autosystem\DBConn::getTableNameFor('accound');
        try{
            if($accesslevel =$this->getAccessFrom($location)){
                if(key_exists('TYPE',$accesslevel)){
                    if(isset($this->_user)&&$this->_user->islogin()){                      
                        //$this->_user->log($location.$this->_user->__toString() );
                        return $this->_user->hasRulle($accesslevel['TYPE']);
                    }else{  
                        return $accesslevel['TYPE']=='PUBLIC';       
                    }
                }
            }
        }catch(Exception $e){
            $this->LogoutUser();
        }
        return false;
    }
    private function getAccessFrom($location){
         try{
             if($database = \autosystem\DBConn::GET()){
                 $dbStmt = $database->prepare('SELECT `TYPE` FROM `installs` WHERE location =:loc');
                 $dbStmt->execute([':loc'=>$location]);
                 /* TODO
				 idee om meer specifieke access
				 $this->_user->islogin() 
				 parameter $accound_id 
				 $accound_id == $this->_user->getAccound_id()
				 SELECT `TYPE` FROM `installs_access` WHERE location =:loc
				 */
                 return $dbStmt->fetch(\PDO::FETCH_ASSOC);
             }else{
                  $this->accesslog('no database');
                  return false;
             }
         }catch(exception $e){
            $this->accesslog('pdoexcept:'.$e->getMessage());
            return false;
         }
    }
    private function statlog(){
        if(!$this->islog){return ;}
        try{
            if($database = \autosystem\DBConn::GET()){
                $dbStmt = $database->prepare('');
                $dbStmt->execute([]);
                return  $dbStmt->fetch(\PDO::FETCH_ASSOC);
            }else{
                $this->accesslog('no database');
                return false;
            }
        }catch(exception $e){
            $this->accesslog('exception'.$this->__toString());
            return false;
         }
    }
    public function __destruct(){
        if(is_a($this->_user,'\users\Visitor')){
            $_SESSION['visitor']= serialize($this->_user);
        }
        if(is_a($this->_user,'\users\User')&& $this->_user->islogin()){
            $_SESSION['user']=null;
            $_SESSION['user']= serialize($this->_user);
        }
        if(file_exists('.prosession')){
            $this->prosession->destroy(session_id());
        }
        //sessieWrite(,$_SESSION);
        $this->_user=null;
        unset($this->_rulles);       
    }
    public function __toString(){   return '=>class:'.get_called_class().'user=>'.$this->_user.'\' rollel=>'.$this->rulles.'\' ';  }
    protected function accesslog($msg,$from='unknow'){
        $msg = 'on: '.date('M j Y - G:i:s').',model '.__file__.' :'.$from.' MSG '.$msg.PHP_EOL;
        file_put_contents('./log/acceses.log', $msg, FILE_APPEND);
    }
}
?>