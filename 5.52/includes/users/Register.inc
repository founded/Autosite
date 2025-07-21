<?php
	namespace users;
    class Register extends \autosystem\Controller{
        private $values = array('search','find');
        public function init() {
           $this->vars();
           $this->getModel();
           if(isset($_POST['registrate'])&&$this->_validator->isValid('registrate')){
           	 if($this->_values['pwx']==$this->_values['pwxe']){
                if(isset($_POST['GDPR'])){
                    $key = \autosystem\Key::GET(30);
                    if(!$this->_model->existEmailAdress($this->_values['email']) && !$this->_model->existUser($this->_values['username'])){
                         //print $this->_values['username'].'-'.$this->_values['password'].'-'.$this->_values['email'].'-'.$key;
						 $issaved=$this->_model->save($this->_values['username'],$this->_values['password'],$this->_values['email'],$key);
                         if($issaved){
                            $this->validationmsg('register.saved',$issaved);
                         }else{
							$this->validationmsg('register.notsaved',$issaved);
						 }
                         $this->goToProfile();  
                    }else{	$this->validationmsg('register.emailoruserexist');           }
                }else{     	$this->validationmsg('register.gdprnotchecked');             }
             }else{
				$this->validationmsg('register.paswordnotequals'); 
			 }   
           }else{
				$this->_values['errors']=$this->_validator->errorinfo($this->_view,'registrate');
		   }
           $this->prepare($this->_loc,$this->_action,$this->_values);
        }
        private function prepare($loc,$action,$values,$find =null){
           print $this->_view->getheader($loc,$action);
           if(isset($this->_values['errors'])){
                print '<section >'.$this->_values['errors'].'</section>';
           }
           print $this->_view->getform_head($this->_loc,$action);
           print $this->_view->get_template($this->_loc,$this->_action,$this->_values,array('email'=>'%value%'));
           print $this->_view->getform_end($action,'registrate');
           print '<br><br>';
           print $this->_view->getfooter($action);
        }
        private function vars(){
            $this->_values['title']='';
            $this->_values = $this->_view->get_translations($this->values);
            //$this->_validator->setDebug(true);
            $this->_validator->addForm('registrate',['username','email','pwx','pwxe'],false,[],$this->_values);
	  		if(isset($_POST['registrate'])){
	  				$this->_validator->addAlocat('addconceptmail',['username'=>'istext','pwx'=>'istext','pwxe'=>'istext','email'=>'isAemail']);
			    	
					$this->_validator->render('registrate',$_POST,$this->_values);
					$this->_values['password']=($this->_validator->isValid('registrate')&&$this->_values['pwx']==$this->_values['pwxe'])?$this->_values['pwx']:'';
			}
        }
        private function isusername($username){
        	$badusernames = ['autosystem'];
        	return !(in_array($username,$badusernames));
		}
    }
?>