<?php
namespace Amail;
class Mailpool extends \autosystem\Controller{
    private $poolsize = 5;
    public function init(){
           $this->loadSettings();// TODO NEED TO SEE
           $this->getModel();
           $this->vars();
           if(count($_POST)>0){
                if(key_exists('addtest',$_POST)&&$this->canSave()&& $id = parent::getAccound_id()){
                     $newmailer = new \Amail\CreateAEmail();
                     $newmailer->$setTestmail($id,$concept_id,$subject);
                     if($issaved){
                        $this->_values['title']=$this->_view->get_AtranslationB('saved');
                     }
                     if($this->_values['sendnow']){
                        $data =  $this->_model->getConceptById($concept_id,$forAccound_id);
                        $return['from']='noreplay'.$this->antispamdetection;
                        $return['plain']=$this->getParts($concept_id,'PLAIN');
                        $return['html']=$this->getParts($concept_id,'HTML');
                        $return['values']=$this->getEValues($concept_id,$forAccound_id);
                        $return['name']=$return['values']['username'];
                        $return['to']='';
                        $return['cashname']=date('Y').'-'.date('m').'-'.$return['to'].'emailcach';
                        $return['activebackup']=false;
                        $return['subject']='';
                        $sender->SendMail(
                            $data['locale'],'mailpooltest@djdb.be',$data['fromme'],
                            $data['content'],$data['html'],$vals,
                            $data['cash'],$data['backup'],
                            $data['to'],$data['subject'],false);
                        }
                     $this->GOESTO($this->_loc,$this->_action);
                } 
                if(key_exists('testconcept',$_POST)&&$this->_validator->isValid('addtest')){
                    $data =  $this->_validator->getFormData('addtest');
                    $concept = $this->_model->getConceptById($data['concept'],$this->getAccound_id(),$this->_view,$data['content']);
                    $sender = new \Amail\AmailSender();
                    $Accound_id = $this->getAccound_id();
                    $this->_model->addTESTmail($data ,$Accound_id);
                    //$sender->setsubject('subject');
                    $vals = [];
                    //($locale,$from,$name,$text,$html,$values,$cashname,$activebackup,$to,$subject,$sendit = false)
                    $from = 'noreplay.mailpool@'.$_SERVER['host'].'';
                    $cach = '';
                    print $sender->SendMail(
                    $this->_values['locale'],$data['toemail'],$data['name'],
                    $concept['plain'],$concept['html'],$vals,
                    $cach ,false,
                    $data['toemail'],$data['subject'],false);
                }       
           }

           if(isset($_GET['testcron'])){
                 $e =$this->_model->GET_amailsToSend($_GET['concepttest'],parent::getAccound_id());
                 print_r($e);
           }
           if(isset($_GET['cron'])){
                 //$e =$this->_model->GET_amailsToSend($_GET['concepttest'],parent::getAccound_id());
                 //print_r($e);
           }
           $this->prepare($this->_action,$this->_values);
    }
    private function prepare($action,$values){ 
           if(isset($this->_values['runcron'])){
                print 'unconstruct';
                return;
           }
           if(isset($this->_values['getemailsource'])){
                print '<html><head></head>';
                print '<body></body></html>';
                return;
           }
           print $this->_view->getheader($this->_loc,$this->_action);
           $this->_values['mailpoolinfo']='[ok] info offline ';
           $this->_values['mailpoolinfostatus']='OK';
           $GETSOMETHING = true;  
           if(isset($_GET['addtest'])){
                $this->__Addtest();
                $GETSOMETHING = false;
           }
           if(isset($_GET['testmailform'])){
                $this->testMail();
                $GETSOMETHING = false;
           }           
           if(isset($_GET['test'])){
                $this->_values['id']=(isset($_GET['id'])&&\autosystem\ValidateAs::isnumeric($_GET['id']))?$_GET['id']:0;
                if($this->_values['id']>0){
                   $sender = new \Amail\AmailTestSender();
                   $GETSOMETHING = false;
                }
           }
           print $this->_view->get_template($this->_loc,'MailPool_Menu',$this->_values);
            if(isset($this->_values['getemail'])){
                print '<iframe src="'.$this->_view->getinstallpath().'/'.$this->_loc.'/'.$this->_action.'/?getemailsource=$this->_values[\'getemail\']" >';
                print '</iframe>';
                $GETSOMETHING = false;
           }
           if(key_exists('sendtest',$_GET)&&$this->_validator->isValid('sendtest')){
                    $mailid =  $this->_validator->getFormData('sendtest');
                    $data =$this->_model->GET_testToSendByID($mailid['sendtest']);
                    $array['id'] = 'sendtest';
                    $array['title'] = $this->_view->get_AtranslationB('title.sendtest').' ID:'.$mailid['sendtest'];
                    print $this->_view->get_template('global','section_head',$array);
                    print $this->_view->get_template($this->_loc,'Mailpool_mailinfo',$data[$mailid['sendtest']]);
                    print $this->_view->get_template('global','section_foot',[]);
                    $em = $this->_model->GET_EmailAdress($data[$mailid['sendtest']]['accound_id']);
                    $array['id'] = 'desttestinf';
                    $array['title'] = $this->_view->get_AtranslationB('title.desttestinf').' ACCOUND:'.$data[$mailid['sendtest']]['accound_id'];
                    print $this->_view->get_template('global','section_head',$array);
                    print $this->_view->get_template($this->_loc,'Mailpool_destinyinfo',$em);
                    print $this->_view->get_template('global','section_foot',[]);
                    $acc_id =  $data['accound_id'];
                    $concept = $this->_model->getConceptById($data[$mailid['sendtest']]['conceptmail_id'],$this->getAccound_id(),$this->_view,$data['content']);
                    $sender = new \Amail\AmailSender();
                    //print_r($concept);
                    $values = [];
                    $cachname='';
                    $array['id'] = 'sender';
                    $array['title'] = $this->_view->get_AtranslationB('title.sender').' ACCOUND:'.$data[$mailid['sendtest']]['accound_id'];
                    print $this->_view->get_template('global','section_head',$array);
                    $sender->setsubject($data[$mailid['sendtest']]['subject']);
                    $sender->setSource($this->_values['locale'],$from,$concept['name'],$concept['plain'],$concept['html'],$values,$cachname);
                    foreach($concept['parts']as $part){
                        print $this->_view->get_template($this->_loc,'Mailpool_otherpartinfo',$part);
                        $name = 'cept'. $part['amail_conceptmail_id'].'_'.$part['amail_contentpart_id'];
                        $sender->addVpart($name,$part['content'],$part['contenttype'],$part['cid']);
                    } 
                    print $sender->SendParts($concept['to'],$data[$mailid['sendtest']]['subject'],$concept['name'],false);
                    print $this->_view->get_template('global','section_foot',[]);
                    //($locale,$from,$name,$text,$html,$values,$cashname,$activebackup,$to,$subject,$sendit = false)
                    //$from = 'noreplay.mailpool@'.$_SERVER['host'].'';
                    
                    /*print $sender->SendMail($this->_values['locale'],$from,$concept['name'],
                    $concept['plain'],$concept['html'],$vals,$cach ,false,$concept['to'],,false);
                   */ 
                }       
           $this->status('');
           if(isset($this->_values['send'])){
                $sender = new \Amail\AmailTestSender();
                $a = $this->_model->GETAMailToSend($this->_values['send']);
                $a['link.activate']='http://autosite.dev/v4.0/users/Activate/';
                $a['link.subscribe']='http://autosite.dev/v4.0/Amail/Mailpool';
                $a['name']='"Your name"';
                $html = $this->_view->get_template($this->_loc,'Ahead');
                $html .= $this->_view->get_template($this->_loc,'Amail_head');
                $html .= $this->_view->get_template($this->_loc,'Amail_foot');
                $html .= $this->_view->get_template($this->_loc,'Afoot');
               $array = $this->_model->getSystemIMG($dest_id);
               foreach($array as $key =>$values){
                    print $values;
               }
               $cdi = $sender->addimg('./layout/img/icons/KING.jpg');
               $a['img.header'] = $cdi;
               $namekey = ''.'_mailto'.$this->_values['id'].'';
               if($sender->SendMail('nl','no-replay@'.$_SERVER['HTTP_HOST'],'me',$a,true,$a['email'],'test subject',false)){
                    print('mogelijks verzonden');
               }else{
                    print('mogelijks niet verzonden');
               }
               $GETSOMETHING = false;
           }
           if(isset($this->_values['concepttest'])){
            
           }
           if(isset($this->_values['source'])){
                print 'maildata:<pre>';
                $sender = new \Amail\AmailTestSender();
                $a = $this->_model->GETAMailToSend($this->_values['source']);
                $a['link.activate']='http://autosite.dev/v4.0/users/Activate/';
                $a['link.subscribe']='http://autosite.dev/v4.0/Amail/Mailpool';
                $a['name']='"Your name"';
                print_r($a);
                print '</pre>';
                print 'mailsource:<pre>';
                $html = $this->_view->get_template($this->_loc,'Ahead');
                $html .= $this->_view->get_template($this->_loc,'Amail_head');
                //$html .= $a['contenthtml'];
                $html .= $this->_view->get_template($this->_loc,'Amail_foot');
                $html .= $this->_view->get_template($this->_loc,'Afoot');
                $sender->SendTest('nl','no-replay@'.$_SERVER['HTTP_HOST'],'me',$a['content'],$html,$a,true,$a['email'],'test subject',false);
                //$sender->MailSource();
                print '</pre>';
                $GETSOMETHING = false;
           } 
           if($GETSOMETHING){
                $this->Summary();
           }     
        print $this->_view->getfooter($this->_action);
    }
    private function canSave(){
        return true;
    }
    private function vars(){        
        $this->_values['issend']='';
        $this->_values['namekey']='';
        $this->_values['template']='';
        $this->_values['content']='';
        $this->_values['status']=''; 
        $this->_values['statuscomment']='';
        //$this->_values['templates']=$this->_view->getOptions($this->templates,$this->_values['template']);
        //$this->_values['contents']= $this->_view->getOptions($this->contents,$this->_values['content']);
        if(isset($_POST['find'])){
                $this->_values['search']=$_POST['search'];       
        }else{
            if(isset($_GET['view'])){
                $this->_values['view']=(isset($_GET['view'])&&\autosystem\ValidateAs::isnumeric($_GET['view']))?$_GET['view']:'';
            }
            if(isset($_GET['source'])){
                $this->_values['source']=(isset($_GET['source'])&&\autosystem\ValidateAs::isnumeric($_GET['source']))?$_GET['source']:'';
            }
            if(isset($_POST['addtest'])){
                //$this->_values['content']=(\autosystem\ValidateAs::isnumeric($_POST['content'])&& key_exists($_POST['content'],$this->contents))?$_POST['content']:'';
                //$this->_values['template']=(\autosystem\ValidateAs::isnumeric($_POST['template'])&& key_exists($_POST['template'],$this->templates))?$_POST['template']:'';
            }
            $this->_values['status']=(isset($_POST['status'])&& key_exists($_POST['status'],$this->_model->sendstates))?$_POST['status']:'';
            if(isset($_POST['add'])){
                $args = array(
                          'content' => array('filter'    => FILTER_VALIDATE_INT ,
                                             'flags'     => FILTER_REQUIRE_ARRAY,   
                                             'options'   => $this->contents),
                          'template' => array('filter'    => FILTER_VALIDATE_INT ,
                                             'flags'     => FILTER_REQUIRE_ARRAY,   
                                             'options'   => $this->templates),
                          'TYPE'    => array('filter'    => FILTER_REQUIRE_ARRAY, 
                                             'options'   => $this->_model->modeltypes),
                          'name'     => FILTER_SANITIZE_STRING,
                          'title'     => FILTER_SANITIZE_STRING,
                          'site'     => FILTER_SANITIZE_URL,
                          'email'     => FILTER_SANITIZE_EMAIL,
                          'category_id' => FILTER_VALIDATE_INT
                      );
                $a = filter_input_array(INPUT_POST, $args);
                $this->_values = $a+$this->_values;
                $this->_values['txtcolor']=(isset($_POST['txtcolor'])&&\autosystem\ValidateAs::iscolor($_POST['txtcolor']))?$_POST['txtcolor']:'';
                $this->_values['backcolor']=(isset($_POST['backcolor'])&&\autosystem\ValidateAs::iscolor($_POST['backcolor']))?$_POST['backcolor']:'';
                $this->_values['phone']=(isset($_POST['phone'])&&\autosystem\ValidateAs::isphone($_POST['phone']))?$_POST['phone']:'';
            }
            if(isset($_GET['testmailform'])){
                $this->_values['to']=(isset($_POST['to']))?$_POST['to']:'';
                $this->_values['cc']=(isset($_POST['cc']))?$_POST['cc']:'';;
                $this->_values['bcc']=(isset($_POST['bcc']))?$_POST['bcc']:'';;
                $this->_values['replyto']=(isset($_POST['replyto']))?$_POST['replyto']:'';;
                $this->_values['followupto']=(isset($_POST['followupto']))?$_POST['followupto']:''; 
                $this->_values['subject']=(isset($_POST['subject']))?$_POST['subject']:'';
                $this->_values['content']=(isset($_POST['content']))?$_POST['content']:'';
                $this->loadTest('pooltestmail');
                //print_r($this->_values);
            }
        $this->_validator->addForm('addtest',['concept','subject','content','name','toemail'],false,[],$this->_values);
		if(isset($_POST['testconcept'])){
		    $this->_validator->addAlocat('addtest',['concept'=>'isnumeric','subject'=>'istext','content'=>'istext','name'=>'istext','toemail' =>'isemail']);
		    $this->_validator->render('addtest',$_POST,$this->_values);   
		}
        $this->_validator->addForm('sendtest',['sendtest'],false,[],$this->_values);
		if(isset($_GET['sendtest'])){
		    $this->_validator->addAlocat('sendtest',['sendtest'=>'isnumeric']);
		    $this->_validator->render('sendtest',$_GET,$this->_values);   
		}

        }
        $this->_values['states']=$this->_view->getOptions($this->_model->sendstates,$this->_values['status']);
    }
    private function __Addtest(){
        $this->_values['concepts'] = $this->_view->getOptions($this->_model->GET_Concepts_LIST(),$this->_values['concept_id']);
        //print_r($this->_values['concepts']);
        $this->vars();
        print $this->_view->getform_head($this->_loc,$this->_action);
        print $this->_view->get_template($this->_loc,'MailPool_Add',$this->_values);
        print $this->_view->getform_end('','testconcept');
    }
    private function testMail(){
        $this->vars();
        print $this->_view->getform_head($this->_loc,$action);
        print $this->_view->get_template($this->_loc,'Mail_menu',$this->_values);
        print $this->_view->get_template($this->_loc,'MailPool_testmailform',$this->_values);
        print $this->_view->get_template($this->_loc,'Mail_status',$this->_values);
        print $this->_view->getform_end('','test');
    }
    private function Summary(){
        $accoundid = 'null';
        $enabled = 'null';
        $this->addTranslationKeys($this->_model->sendstates);
        $this->_values = $this->getTranslations();
        $this->_view->setENUM_TRANSLATIONS($this->_values);
        $values['states']=$this->_view->getOptions($this->_model->sendstates,'');
        $this->_view->preSetTemplate($this->_loc,'MailPool_item',$values);
        //$location,$templatename,&$view,$accound_id,$enabled,$filter
        $filters = [];
        $this->_model->GET_MailPoolSummary($this->_loc,'MailPool_item',$this->_view,$accoundid,$enabled,$filters);
        print $this->_view->get_template('global','toolcheck_end',$this->_values);
    }
}
?>