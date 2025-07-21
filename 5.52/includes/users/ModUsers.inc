<?php
	namespace users;
    class ModUsers extends \autosystem\Controller{
        private $values = [];
        public function init() {
           $this->vars();
           $this->getModel();
           if($this->Acomplete()){                return;}
           if($this->checkItS()){                 return;}
           if(isset($_POST['save'])){
                $this->prepare($this->_loc,$this->_action,$this->_values);
                return;
           }
           if(isset($_GET['lastactions'])){
                if(isset($this->_values['delete_login'])){       $this->_model->delete_login($this->_values['delete_login']);
                }
                if(isset($this->_values['delete_loginByIp'])){   $this->_model->delete_loginByIp($this->_values['delete_loginByIp']);
                }
				if(isset($this->_values['delete_loginByYear'])){ $this->_model->delete_loginByYear($this->_values['delete_loginByYear']);
                }    
            }
           $this->prepare($this->_loc,$this->_action,$this->_values);
        }
        private function prepare($loc,$action,$values,$find =null){
            print $this->_view->getheader($loc,$action);
            if(isset($this->_values['errors'])){
                print '<section class="ERROR" >'.$this->_values['errors'].'</section>';
            }
            print $this->_view->get_template($this->_loc,$this->_action,$this->_values,array('email'=>'%value%'));
            $GETSOMETHING = true;
            if(isset($_GET['newpassword'])){
                $GETSOMETHING = false;
            }
            if(isset($_GET['reports'])){
                $this->_model->GET_AreportsSummary($this->_loc,'ticket_item',$this->_view,$this->getAccound_id(),true,true);
                $GETSOMETHING = false;
            }
            if(isset($_GET['lastactions'])){//$location,$templatename,$view,$user_id,$accound_id
                if(file_exists('.prosession')){
                    $this->_model->GET_AProsessions($this->_loc,'session_item',$this->_view,$this->_values['page'],$this->getAccound_id());
                }else{
                    print $this->_view->get_AtranslationB('setting.pro.session.notactive');
                }
                print $this->_view->getform_head($this->_loc,$this->_action);
                $this->_model->GET_LoginSummary($this->_loc,'ModUser_detail_loginitem',$this->_view,null,null,$this->_values['page']);
                print $this->_view->getform_end('','save');
                $GETSOMETHING = false;
            }
            if(isset($_GET['editAccound'])){
                print $this->_view->getform_head($this->_loc,$this->_action);
                $this->_model->GET_Accound($this->_loc,'ModEditAccound',$this->_values);
                print $this->_view->getform_end('','save');
                $GETSOMETHING = false;
            }
            if(isset($_GET['infoAboutAccound'])){
                print $_GET['infoAboutAccound'];
                $this->_model->GET_Accound($this->_loc,'ModAccound_detail',$this->_view,$values['infoAboutAccound']);
                $this->_model->GET_LoginSummary($this->_loc,'ModAccound_detail_loginitem',$this->_view,null,$values['infoAboutAccound'],$this->_values['page']);
                print $this->_view->get_template('global','toolcheck_end',$this->_values);
                $GETSOMETHING = false;
            }
            if(isset($_GET['editUser'])){
                print $this->_view->getform_head($this->_loc,$this->_action);
                print $this->_view->get_template($this->_loc,'edit',$this->_values);
                print $this->_view->getform_end('','save');
                $GETSOMETHING = false;
            }
            if(isset($_GET['infoAboutUser']) ){
                $this->_model->getUserDetail($this->_loc,'ModUser_detail',$this->_view,$values['userid'],0);
                $this->_model->GET_LoginSummary($this->_loc,'ModUser_detail_loginitem',$this->_view,$values['userid'],0);
                print $this->_view->get_template('global','toolcheck_end',$this->_values);
                $GETSOMETHING = false;
            }
            if(isset($_GET['searchAccound'])){
                print $this->_view->getform_head($this->_loc,$this->_action);
                print $this->_view->get_template($this->_loc,'Modusers_search',$this->_values);
                print $this->_view->getform_end('','findperson');
                print $this->_view->get_template('global','AutoCompleteList',$this->_values);
                if(isset($this->_values['findmember'])){
                	//Get accound id to view see under wil prepare presentation of the accound
                	//print $this->_values['findmember'];
                    $this->_values['accound']=$this->_model->FIND($this->_values['findmember'],'accound');
                    //print '--'.$this->_values['accound'].'--';
                }               
                $GETSOMETHING = false;
            }
            if(isset($this->_values['user'])||isset($this->_values['accound'])){
                //print_r($this->_values);
                $this->_model->getUserDetail($this->_loc,'Users_detail',$this->_view,$this->_values['user'],$this->_values['accound']);
                print $this->_view->get_template('global','toolcheck_end',$this->_values);
                $GETSOMETHING = false;
            }
            if(isset($_GET['lastlogins'])){

                $this->_model->GET_ASummary($this->_loc,'Users_item',$this->_view,$this->_values['page'],$this->getAccound_id(),'lastlogins', true,true);
                $GETSOMETHING = false;
            }
            if(isset($_GET['sessions'])){

                $this->_model->GET_ASummary($this->_loc,'Users_item',$this->_view,$this->_values['page'],$this->getAccound_id(),'lastlogins', true,true);
                $GETSOMETHING = false;
            }
            
            if(isset($_GET['usersaccess'])){
                print $this->_view->getform_head($this->_loc,$this->_action);
                $enabled = null;        $activated = 'true';
                $this->_model->GET_ASummary($this->_loc,'Users_item',$this->_view,$this->_values['page'],$this->getAccound_id(),'usersaccess',$enabled,$activated);
                print $this->_view->get_template('global','toolcheck_end',$this->_values);
                print $this->_view->getform_end('','edit');
                $GETSOMETHING = false;
            }
            if(isset($_GET['change_acces'])){
                $enabled = null;        $activated = 'true';
                $this->_model->GET_ASummary($this->_loc,'Acces_item',$this->_view,$this->_values['page'],$this->getAccound_id(),'change_acces',$enabled,$activated);
                print $this->_view->get_template('global','toolcheck_end',$this->_values);
                $GETSOMETHING = false;
            }
            if(isset($_GET['notActivated'])){
                $enabled = false;       $activated = 'false';
                $this->_model->GET_NotActiefSummary($this->_loc,'InactiefAccound_item',$this->_view,$this->getAccound_id(),$enabled,true,$this->_values['page']);
                print $this->_view->get_template('global','toolcheck_end',$this->_values);
                $GETSOMETHING = false;
            }
            if($GETSOMETHING){
                $this->_model->GET_AreportsSummary($this->_loc,'ticket_item',$this->_view,$this->getAccound_id(),true,true);
                $enabled = null;
                $this->_model->GET_NotActiefSummary($this->_loc,'InactiefAccound_item',$this->_view,$this->getAccound_id(),$enabled,true,$this->_values['page']);
                print $this->_view->get_template('global','toolcheck_end',$this->_values);
            }
            print $this->_view->getfooter($action);
        }
        private function vars(){
            $this->_values['title']='installation:';
            $this->_values = $this->_view->get_translations($this->values);
            $this->_values['page']=0;
            $this->_values['username']=(isset($_POST['username'])&& ctype_alnum($_POST['username']))?filter_input(INPUT_POST, 'username'):'';
            if(isset($_GET['infoAboutUser'])){    $this->_values['userid']           = $_GET['infoAboutUser'];   }
            if(isset($_GET['infoAboutAccound'])){ $this->_values['infoAboutAccound'] = $_GET['infoAboutAccound'];}
            if(isset($_GET['page'])){             $this->_values['page']             = $_GET['page'];            }
            if(isset($_GET['lastactions'])){
                if(isset($_GET['delete_login'])){     $this->_values['delete_login']     = $_GET['delete_login'];}
                if(isset($_GET['delete_loginByIp'])){ $this->_values['delete_loginByIp'] = $_GET['delete_loginByIp'];}
                if(isset($_GET['delete_loginByYear'])){ $this->_values['delete_loginByYear'] = $_GET['delete_loginByYear'];}
                $this->_values['lastactions']=$_GET['lastactions'];   
            }
            if(isset($_GET['searchAccound'])){
                //$this->_values['findmember']=(isset($_POST['q'])&& ctype_alnum($_POST['q']))?$POST['findmember']:'';//filter_input(INPUT_POST, 'q'):'';;
                if(isset($_POST['findmember'])){
                    $this->_values['findmember']=(\autosystem\ValidateAs::isTEXT($_POST['findmember']))? $_POST['findmember']:'';
                    //$this->_values['type']=in_array();
                }else{
                    $this->_values['findmember']='';
                }
           }
           if(isset($_GET['infoAboutAccound'])){
                $this->_values['accound']= (isset($_GET['infoAboutAccound'])&& ctype_alnum($_GET['infoAboutAccound']))?filter_input(INPUT_GET, 'infoAboutAccound'):'';
           }
           if(isset($_GET['user'])){
                $this->_values['user']= (isset($_GET['user'])&& ctype_alnum($_GET['user']))?filter_input(INPUT_GET, 'user'):'';
           }             
        }
        private function checkItS(){
           if(!isset($_GET['check'])){return ;}
           $return = false;
           if(isset($_POST['accessibility'])){    foreach($_POST['accessibility'] as $userid => $useraccess){
                    if(!empty($userid)&& !empty($useraccess)){
                        if($this->_model->addRules($userid,$useraccess)){                     $return = true;  }
                    }
                }
           }
           if(isset($_POST['enabled'])){          foreach($_POST['enabled'] as $userid => $userenabled){
                    if(!empty($userid)&& !empty($userenabled)){
                        if($this->_model->userEnabled($userid,$userenabled)){                  $return = true; }
                    }
                }
           }
           if(isset($_POST['accound_enabled'])){   foreach($_POST['accound_enabled'] as $accoundid => $accoundenabled){
                    if(!empty($accoundid)&& !empty($accoundenabled)){
                        if($this->_model->accoundEnabledId($accoundid,$accoundenabled)){       $return = true; }
                    }
                }
           }
           if(isset($_POST['mod_accoundvisible'])){ foreach($_POST['mod_accoundvisible'] as $accoundid => $accoundvisible){
                    if(!empty($accoundid)&& !empty($accoundvisible)){
                        if($this->_model->mod_accoundvisible($accoundid,$accoundvisible)){     $return = true; }
                    }
                }
           }
           if(isset($_POST['mod_accoundfollow'])){  foreach($_POST['mod_accoundfollow'] as $accoundid => $accoundfollow){
                    if(!empty($accoundid)&& !empty($accoundfollow)){
                        if($this->_model->mod_accoundfollow($accoundid,$accoundfollow)){       $return = true; }
                    }
                }
           }
           if(isset($_POST['mod_accoundseeonchange'])){           
                foreach($_POST['mod_accoundseeonchange'] as $accoundid => $accoundseeonchange){
                    if(!empty($accoundid)&& !empty($accoundseeonchange)){
                        if($this->_model->mod_accoundseeonchange($accoundid,$accoundseeonchange)){ $return = true; }
                    }
                }
           }
           if(isset($_POST['mod_accoundlog'])){    foreach($_POST['mod_accoundlog'] as $accoundid => $logaccound){
                    if(!empty($accoundid)&& !empty($logaccound)){
                        if($this->_model->mod_accoundlog($accoundid,$logaccound)){      $return = true;
                        }
                    }
                }
           }
           return $return;   
        }
        private function Acomplete(){
           $minsize=10;
           //$this->_values['field']=(isset($_POST['fromid']))?$_POST['fromid']:'null';
           $this->_values['autocomlete']=(isset($_POST['findmember']))?$_POST['findmember']:'null';
           if(key_exists('Acomplete',$_GET)){
               $return = '{"Acompletelist":{}}'; //$startwith,$field=null
               $data = $this->_model->AcompleteSearch($this->_values['autocomlete'],'accound');          
               $return = json_encode($data);
               print $return.'';   
               return true;     
           }
           return false;
        }
    }
?>