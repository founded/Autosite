<?php
/**
* @author Lieven Roegiers
* @copyright 
* @CMS autosite
*/
namespace users;
class Login extends \autosystem\Controller{
    private $Key;
    private $visitor;
    public function init(){
        $this->vars();
        $this->getModel();
        if(isset($this->visitor)&&$this->visitor->islogin()){
            $this->redirect('users/Profile/');
        }
        if(isset($_POST['username'])){
            $this->_values['title']=$this->_view->get_AtranslationB('login.started');
            $user = new User($this->visitor->getloginkey());
            $user->cast($this->visitor);
            //print $this->_values['username'].$this->_values['password'].$this->_values['cripty'];
            if($user->login($this->_values['username'],$this->_values['password'],$this->_values['cripty'])){
               $this->visitor = null;
               $this->setSessionUser($user);
               $this->redirect('users/Profile/');
               print 'Login';
               
            }else{
               $user = null;
               $this->visitor->ItryLogin();
               session_regenerate_id();
               $this->setSessionVisitor($this->visitor);
               //unset($_SESSION['visitor']);
               //$this->status("",true,true);
               //$this->redirect('users/Register/');
               print 'reg';
            }
        }
        $this->prepare($this->_loc,$this->_action,$this->_values);
    }
    private function prepare($loc,$action,$values,$find =null){
        //print_r($_SESSION);
        $this->visitor =  $this->getVisitor();
        //print $this->visitor->getBrowser();
        if(isset($_GET['softconnect'])){
            $result = ['Hello, here autositesoft'=>true];
            $result = ['softconnector'=>true];
            $result = ['loginkey'=>$this->visitor->getloginkey()];
            header('HTTP/1.0 200 ');print json_encode($result);
            return;
        }else{
            print $this->_view->getheader($loc,$action);
            print $this->_view->getform_head($this->_loc,$action,'login');
            if(isset($_GET['lostpassword'])){
                print $this->_view->get_template($loc,'LostPassword',$this->_values);
            }else{
                print $this->_view->get_template($loc,$action,$this->_values);
            }
            print $this->_view->getfooter($action);
        }
    }
    private function vars(){       
        $values['title']='Login';
        $this->_values = $this->_view->get_translations($values);
        if(isset($_POST['username'])&&ctype_alnum($_POST['username'])){
            $this->_values['username']=filter_input(INPUT_POST, 'username');
        }else{
            $this->_values['username']='';
        }
        $this->visitor =  $this->getVisitor();
        $this->_values['loginkey']=$this->visitor->getloginkey();
        $this->_values['cripty']=$this->funrandencript();
        if(isset($_POST['pwx'])){
            $this->_values['password']=filter_input(INPUT_POST, 'pwx');
        }
        //print_r($this->_values);
    }
    private function funrandencript(){
		$i = 0;//TODO rand (0, 1) ;
		switch ($i):
		    case 0:return 'md5';
		        break;
		    case 1:return 'sha1';
		        break;
		    case 2:return 'Base64.encode';
		        break;
		    default:
		        print 'i is niet gelijk aan 0, 1 of 2';
		endswitch;
	}
    private	function ishash($type,$tohach,$code){
    	switch ($type):
		case 'md5':
			return(md5($tohach)=== $code);
        	break;
		case 'sha1':
			return(sha1($tohach) === $code);
    		break;
		case 'Base64.encode':
			return(base64_encode($tohach) === $code);
    		break;
     	default :
	    	return($tohach === $code);
		endswitch;
    }
}
?>