<?php
 /**
 * @author Lieven Roegiers
 * @copyright 2010 
 * @CMS autosite
 */
namespace users;
class Profile extends \autosystem\Controller{
    private $Key;
    private $values = array('search','find','profile.welcome','profile.data','user.smaldashboard.data','user.session.data','user.Phone.data','user.history.data','user.adres.data');
    private $use_userpath = true;
    public function init(){
       $this->vars();
       $this->getModel();
       $visitor =  $this->getVisitor();
       if(isset($_GET['check'])){
         if(isset($_POST['adresvisibility'])){
                $cansee = $_POST['adresvisibility'];
                if(is_array($cansee)){
                   foreach($cansee as $key=>$value){
                        if(\autosystem\ValidateAs::isCheckedOrNot($value)){
                            $this->_model->setAdresVisibility($this->getAccound_id(),$key,$value);
                        }else{
                            $result = ['ok'=>false];
                            header('HTTP/1.0 412 NOT VALID ');print json_encode($result);
                            return false;
                        }
                    }
                    $result = ['ok'=>true];
                    header('HTTP/1.0 200 ');print json_encode($result);
                    return;
                }
                header('HTTP/1.0 504 Your To Slow for me');
                return ;
            }
            if(isset($_POST['phonevisibility'])){
                $cansee = $_POST['phonevisibility'];
                if(is_array($cansee)){
                   foreach($cansee as $key=>$value){
                        if(\autosystem\ValidateAs::isCheckedOrNot($value)){
                            $this->_model->setPhoneVisibility($this->getAccound_id(),$key,$value);
                        }else{
                            $result = ['ok'=>true];
                            header('HTTP/1.0 412 NOT VALID ');print json_encode($result);
                            return false;
                        }
                    }
                    $result = ['ok'=>true];
                    header('HTTP/1.0 200 ');print json_encode($result);
                    return;
                }
                header('HTTP/1.0 504 Your To Slow for me');
                return ;
         }
       }
       if(isset($this->_values['newpass'])){
         $this->_user->renewpwx($this->_values['old'],$this->_values['newpass']);
       }
       if(isset($this->_values['newpass'])){
         $this->_user->renewpwx($this->_values['old'],$this->_values['newpass']);
       }
       if(isset($this->_values['saveacc'])){
       	 //print_r($this->_values);
         $created = $this->createMailacc($this->_values['accemail'],$this->_values['accmailuser'],$this->_values['accpwx'],$this->_values['server_in'],$this->_values['server_out']);
         $this->tosupportreturn($created,'');
       }
       if($this->_validator->isValid('del_prosession')){
           if(isset($_GET['delete_hostsessions'])){
                $del = $this->_model->DELETE_accound_prosessions(['accound_id'=>$this->getAccound_id(),'host'=>$_GET['delete_hostsessions']]);
           }
           if(isset($_GET['delete_session'])){
           	 //print_r($this->_values);
                $del = $this->_model->DELETE_accound_prosessions(['accound_id'=>$this->getAccound_id(),'session_id'=>$_GET['delete_session']]);
             
           }
           $this->tosupportreturn($del,'');
       }
       
       $this->prepare($this->_loc,$this->_action,$this->_values);
    }
    private function prepare($loc,$action,$values,$find =null){
      print $this->_view->getheader($loc,$action);
      
      if($this->_user->hasRulle('KING')){
            print $this->_view->get_template($this->_loc,'kingmenu',$this->_values);
      }
      $GETSOMETHING =  true;
      if(isset($_GET['edit'])){
            print $this->_view->getform_head($this->_loc,$this->_action);
            print $this->_view->get_template($this->_loc,'edit',$this->_values);
            print $this->_view->getform_end('','save');
            $GETSOMETHING = false;
      }
      if(isset($_GET['newpassword'])){
            print $this->_view->getform_head($this->_loc,$this->_action);
            print $this->_view->get_template($this->_loc,'UserRenewPwx',$this->_values);
            print $this->_view->getform_end('','newpass');
            $GETSOMETHING = false;
      }
      if(isset($_GET['addmailacc'])){
      	    print '* Developers wil prevent to use function, by criminal use.';
			print '* This accound wil saved on server , do not give your mailaccess, if you not trust owner of you not a user of this website.';
      	    print '* You registrate on this site , You must know if can trust the maker and admininistration of this website.';
            print $this->_view->getform_head($this->_loc,$this->_action);
            print $this->_view->get_template($this->_loc,'UserMailacc',$this->_values);
            print $this->_view->getform_end('','addacc');
            $GETSOMETHING = false;
      }
      if(isset($_GET['data'])){
            print $this->_view->getform_head($this->_loc,$this->_action);
            $this->_model->GET_profile($this->_loc,'data',$this->_view,$this->getAccound_id());
            print $this->_view->getform_end('','update');
            $GETSOMETHING = false;
      }
      if(isset($_GET['history'])){
            if(isset($_GET['clear'])){
                $this->_user->clearHistory();
            }
      }
      $layout['size']= '12';
      print $this->_view->get_template('global','BoxFull_head',$layout);
      $layout['size']= '5';
      print $this->_view->get_template('global','BoxFull_head',$layout);
      $c['id'] = 'history';
      $c['title'] = $this->_values['user.history.data'];
      $c['url']='%path.module->value%?history';
      print $this->_view->get_template('global','sectionb_head',$c);
      if ($history = $this->getVisitor()->getLocationHistory()){
         foreach($history as $url=>$locations){
            if(is_array($locations)){
                print'' ;
                foreach($locations as $param){
                   if(is_array($param)){
                        $link ='';
                        $href = $this->_view->getinstallpath().$url.'?';
                        foreach($param as $key =>$value){
                           $href .= $link.$key.'='.$value;
                           $link = '&';
                        }
                        print '<br>*<a href="'.$href.'" alt="'.$href.'" >'.$url.'</a>';
                   } 
                }
            }else{
                print '<br>*<a href="'.$url.'" >'.$url.'</a>';
            }
             //print_r($value,true)
             ;
         }
      }
      print $this->_view->get_template($this->_loc,'history',$this->_values);
      print $this->_view->get_template('global','section_foot',[]);
      $this->_values['statusinfo']='Your look online,you looks fantastic.';      
      $dashborditem['content']='<div>Go forward and make it ...</div>';
      $this->_values['content']=$this->_view->get_template($this->_loc,'dashboard_item',$dashborditem);

       if(isset($_GET['dashboard'])){
            print $this->_view->get_template($this->_loc,'dashboard',$this->_values);
            if(isset($_GET['options'])){//addoption visable... all install by rolles
                
            }
            if(isset($_GET['layouts'])){//addoption
                
            }
            print $this->_view->getform_head($this->_loc,$this->_action);
            print $this->_view->getform_end('','add');
            $GETSOMETHING = false;
       }else{
            print $this->_view->get_template($this->_loc,'smaldashboard',$this->_values);
            $c['id'] = 'dashboard';
            $c['title'] = $this->_values['user.smaldashboard.data'];
            //$c['content']='Go forward and make it ...';
            
            print $this->_view->get_template('global','sectionb_head',$c);
            print $this->_view->get_template('global','section_foot',[]);
       }
       $c['id'] = 'sessions';
       $c['title'] = $this->_values['user.session.data'];
       //print $this->prosession->getMyProSession_id()."\n";
       //print session_id()."\n";//getLanguage
       print $this->_view->get_template('global','sectionb_head',$c);
       
      	    print '* last pages need.';
       $this->_model->GET_UserSessions($this->_loc,'profile_session_item',$this->_view,$this->getAccound_id());
       $c['id'] = 'forms';
       $c['title'] = $this->_values['user.forms.data'];
       //print $this->prosession->getMyProSession_id()."\n";
       //print session_id()."\n";//getLanguage
       print $this->_view->get_template('global','sectionb_head',$c);
       print '* last forms need.';
       print $this->_view->get_template('global','section_foot',[]);
            
       print $this->_view->get_template('global','BoxFull_foot',[]);
       $layout['size']= '5';
       print $this->_view->get_template('global','BoxFull_head',$layout);
       if($GETSOMETHING){
          if($this->getAccound_id()!==0){
            try{
                $a = $this->_model->GET_ProfileAsArray($this->getAccound_id());
                $c = $a+ $this->_values;
                $c['adresses']='';
                $c['id'] = 'info';
                $c['title'] = $this->_values['profile.welcome'].' '.$c['username'].', '.$this->_values['profile.data'].' .';
                $c['url']='./?data';
                print $this->_view->get_template('global','sectionb_head',$c);
                print $this->_view->get_template($this->_loc,$this->_action,$c);
                print $this->_view->get_template('global','section_foot',[]);
                $c['id'] = 'infotel';
                $c['phones']= '';
                $c['title'] = $this->_values['user.Phone.data'];
                $c['url']='%path.install->value%/users/ContactInfo/?add_Phone';
                print $this->_view->get_template('global','sectionb_head',$c);
                    $this->_model->GET_TelePhones($this->_loc,'telephoneitem',$this->_view,$this->getAccound_id(),true);
                print $this->_view->get_template('global','section_foot',[]);
                print $this->_view->get_template('global','pagebreak',[]);
                $c['id'] = 'adresses';
                $c['title'] = $this->_values['user.adres.data'];
                $c['url']='%path.install->value%/adres/LocationDetail/?add_adres';
                print $this->_view->get_template('global','sectionb_head',$c);
                    $this->_model->GET_Adreses($this->_loc,'AdresItem',$this->_view,$this->getAccound_id(),true);
                print $this->_view->get_template('global','section_foot',[]);
                $c = $this->_values;
                $c['id'] = 'acc';
                $c['title'] = 'mail acc';
                $c['url']='%path.install->value%/users/Profile/?addmailacc';
                print $this->_view->get_template('global','sectionb_head',$c);
                $this->checkMailacc();
                print $this->_view->get_template('global','section_foot',[]);
                
                $c['id'] = 'userdevdetail';
                $c['title'] = 'developer detail';
                $c['url']='./data';
                $e='no messages';
				$this->STATUS($e);
                print $this->_view->get_template('global','sectionb_head',$c);
                print '<section class="OK" >'.(($this->_user->isActive())?'OK':'ERROR').$this->_user->__toString().'</section>';
                print $this->_view->get_template('global','section_foot',[]);                
                
                print $this->_view->get_template('global','toolcheck_end',[]); 
            }catch(Exception $e){
                print 'I DIEEEEE YOU KILLING ME';
                return;
            }
          }
       }
       print $this->_view->get_template('global','BoxFull_foot',[]);
       print $this->_view->get_template('global','BoxFull_foot',[]);
       print $this->_view->getfooter($action);
    }
    private function vars(){  
        $this->_values = $this->_view->get_translations($this->values);
        if(isset($_GET['key'])&&ctype_alnum($_POST['key'])){
            $this->_values['username']=filter_input(INPUT_POST, 'username');
        }else{
            $this->_values['username']='';
        }
        $this->_values['key']=filter_input(INPUT_POST, 'email');
        if(isset($_POST['pwx'])){
            $this->_values['password']=filter_input(INPUT_POST, 'pwx');
        }
        if(isset($_GET['addmailacc'])){
            $this->_values['accmailuser']='';
            $this->_values['accemail']='';
        }
        if(isset($_POST['addacc'])){
        	$this->_values['saveacc']= '';
            $this->_values['accmailuser']=(\autosystem\ValidateAs::isTEXT($_POST['accmailuser']))?$_POST['accmailuser']:'';
            $this->_values['accemail']=(\autosystem\ValidateAs::isAemail($_POST['accemail']))?$_POST['accemail']:'';
			$this->_values['accpwx']=(\autosystem\ValidateAs::isPassword($_POST['accpwx']))?$_POST['accpwx']:'';
            $this->_values['server_in']=(\autosystem\ValidateAs::isTEXT($_POST['server_in']))?$_POST['server_in']:'';
            $this->_values['server_out']=(\autosystem\ValidateAs::isTEXT($_POST['server_out']))?$_POST['server_out']:'';
		}   
		$this->_validator->addForm('del_prosession',['session_id' ,'host'],false,['session_id' ,'host'],$this->_values);
        $this->_validator->addAlocat('del_prosession',['session_id' => 'varchar','host' => 'varchar']);
        if(isset($_GET['delete_hostsessions'])||isset($_GET['delete_session'])){
            $this->_validator->render('del_prosession',$_GET,$this->_values);
        }     
        if(isset($_POST['newpass'])){
            $this->_values['old']=filter_input(INPUT_POST, 'old');
            if(filter_input(INPUT_POST, 'pwx')==filter_input(INPUT_POST, 'pwxe')&&\autosystem\ValidateAs::isPassword($_POST['pwx'])){
                $this->_values['newpass']= filter_input(INPUT_POST, 'pwx');
            }
        }
        $this->_values['phone']='';
    }
    private function createMailacc($email,$user,$paswd,$in,$out,$portin=993,$portout=587){
    	$aso['username'] = $user;
		$aso['accmailbox'] = $email;
		$aso['pasword'] = $paswd;
		$aso['unread'] = 0;
		$aso['lastcleanup'] = 0;
		$aso['autocleanup'] = 'off';
        $aso['lastworking'] = 'create'.date("Ymd");
		$aso['server_in'] = $in;
		$aso['server_out'] = $out;
		$aso['server_portin'] = $portin;
		$aso['server_portout'] = $portout;
		$this->WriteProfileFSET($email.'.acc.php',$aso);
	}
	private function deleteMailacc($mail){
		$path = $this->_user->getPersonalPath();
	}
	private function checkMailacc(){
		$path = $this->_user->getPersonalPath();
		$scanner =  new \data\FileScanner();
		$arr['inc']=['.acc.php'] ;
		$scanner->setfilter($arr);
		$scanner->scan2extfiles($path);
		$mailacc = $scanner->getfils();
		//print_r($mailacc);
		$isfunction = true;
		if (!function_exists('imap_open')) {
			print '[NOK]php.ini imap_open function not active[contact hosting or servic administrator]';
			$isfunction = false;
		}
		foreach($mailacc as $file){
			$acc = $this->ReadProfileFSET($file);
  
            $this->checkemail($acc);
  
			print $this->_view->get_Templateclone($this->_loc,'ProfileEmailacc',$acc);
			$this->WriteProfileFSET($file,$acc);
		}
	}
    private function checkemail(&$acc){
        
    }
    private function checkemailold(&$acc){
        	$acc['connection']='not installed';
			$acc['messages']='unknow';
			$acc['recent']='';
        	$acc['unseen']='';
        	$acc['uidnext'] = '';
        	$acc['uidvalidity'] = '';
			if($isfunction && $acc['server_in']!==''){
				$port = (key_exists('server_port',$acc))?$acc['server_port']:143;
				$acc['connection'] = 'YES' ;
				if($mbox = @imap_open('{'.$acc['server_in'].':'.$port.'}',$acc['username'],$acc['pasword'],OP_HALFOPEN)){
					if($status = @imap_status($mbox,'{'.$acc['server_in'].'}INBOX', SA_ALL)){
						$acc['messages'] = $status->messages ;
		        		$acc['recent'] =  $status->recent ;
		        		$acc['unseen'] =  $status->unseen ;
		        		$acc['uidnext'] =  $status->uidnext ;
		        		$acc['uidvalidity'] =  $status->uidvalidity ;
					}
	        		@imap_close($mbox);
				}else{
					$acc['connection'] = 'NO'.imap_last_error() ;
				}
			}
			$acc['isautoclean']=($acc['autocleanup']=='')?' checked="checked"':'';
    }
}
?>