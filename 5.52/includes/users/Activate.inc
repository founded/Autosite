<?php
/**
* @author Lieven Roegiers
* @copyright 
* @CMS autosite
*/
namespace users;
class Activate extends \autosystem\Controller{
    private $Key;
    private $values = array('unconstruct','users.activate.ask') ;
    public function init(){
       $this->vars();
       $this->getModel();
       if(isset($this->_values['key'])){
            if(isset($_GET['activate'])){
                if($this->_model->activateThis($this->_values['key'],true)){
                    $this->_values['message']=$this->_view->get_AtranslationB('accound.activated');
                }else {
                    $this->_values['message']=$this->_view->get_AtranslationB('accound.activateError');
                }
            }
            if(isset($_GET['deactivate'])){
                if($this->_model->activateThis($this->_values['key'],false)){
                    $this->_values['message']=$this->_view->get_AtranslationB('accound.deactivated');
                }else {
                    $this->_values['message']=$this->_view->get_AtranslationB('accound.deactivateError');
                }
            }
            if(isset($_GET['ask'])){
                $ticket = new \support\Ticket();
                $location = '/'.$this->_loc.'/'.$this->_action.'/';
                $accound_id = $this->getAccound_id();
                $msg = '';
                $url = '';
                $msg .=($this->_user->isMykey($this->_values['key']))?' has A Login': 'has A no login';
                $nr = $ticket->create($accound_id,$this->_values['title'],'users activate:ask',$this->_values['detail'],$url,$location,$accound_id);
            }
       }
       $this->prepare($this->_loc,$this->_action,$this->_values);
    }
    private function prepare($loc,$action,$values,$find =null){
        print $this->_view->getheader($loc,$action);
        print $this->_view->getform_head($this->_loc,$action);
        print $this->_view->get_template($this->_loc,$this->_action,$this->_values);
        if(isset($_GET['ask'])){
            $array['id'] = 'info';
            $array['title'] = $this->_values['users.activate.ask'];
            print $this->_view->get_template('global','section_head',$array);
            print $this->_view->getform_head($this->_loc,$this->_action);
            print $this->_view->get_template($this->_loc,'SupportTicket',$this->_values);
            print $this->_view->getform_end('','ask');
            print $this->_view->get_template('global','section_foot');
            $GETEVERYTHINGELSE = false;
        }
        print $this->_view->getfooter($action);
    }
    private function vars(){       
        $this->_values['title']='Login';
        $this->_values = $this->_view->get_translations($this->values);
        $this->_values['message']= 'activate NULL'.$this->_values['unconstruct'];
        $this->_values['key']='';
        if(isset($_GET['key'])){
            $this->_values['key']= filter_input(INPUT_GET, 'key');
            if(isset($_POST['pwx'])){
                $this->_values['password']=filter_input(INPUT_POST, 'pwx');
            }
        }
        $this->_validator->addForm('paswordrenew',['pwx','pwxe'],false,[],$this->_values);
	  	if(isset($_POST['paswordrenew'])){
			$this->_validator->addAlocat('paswordrenew',['pwx'=>'istext','pwxe'=>'istext']);
			$this->_validator->render('paswordrenew',$_POST,$this->_values);
			$this->_values['password']=($this->_validator->isValid('registrate')&&$this->_values['pwx']==$this->_values['pwxe'])?$this->_values['pwx']:'';
		}
        /*$this->_validator->addForm('frmactivation',['key'],false,[],$this->_values);
        if(isset($_GET['key'])){
            $this->_validator->addAlocat('frmactivation',['key'=>'istext']);
            $this->_validator->render('frmactivation',$_GET,$this->_values);
        }
        $this->_validator->addForm('frmsupport',['detail','title','category_id'],false,[],$this->_values);
        if(isset($_GET['ask'])){
            $this->_validator->addAlocat('frmsupport',['detail'=>'istext','title'=>'istext']);
            $this->_validator->render('frmsupport',$_GET,$this->_values);
        }*/
        
        
        
                
        if(isset($_GET['ask'])){
            $this->_values['detail']='';
            $this->_values['title']='';
            if(count($_POST)>0){
                $args = array(
                      'title'     => FILTER_SANITIZE_STRING,
                      'detail'     => FILTER_SANITIZE_STRING,
                      'category_id' => FILTER_VALIDATE_INT
                );
                $a = filter_input_array(INPUT_POST, $args);
                $this->_values = $a+$this->_values;
            }
            $this->_values['key']= filter_input(INPUT_GET, 'key');
        }
    }
}
?>