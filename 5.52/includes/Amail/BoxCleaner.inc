<?php
/**
* @author Lieven Roegiers
* @copyright 
* @CMS autosite 5.50
*/
namespace Amail;
class BoxCleaner extends \autosystem\Controller{
     private $values = [];
     private $categorys;
     private $testmode=false;
      public function init(){
        $this->getModel();
        $this->vars();
        if($this->checkItS()){                 return;}
        if(count($_POST)>0){        	
        	if(key_exists('addfilter',$_POST)&&$this->_validator->isValid('addAmailboxfilter')){
                 $issaved=$this->_model->SAVE_amailbox_filter($this->_validator->getFormData('addAmailboxfilter'));
            }
            if(key_exists('initAmailbox',$_POST)&&$this->_validator->isValid('initmailbox')){ 
            	$data = $this->_validator->getFormData('initmailbox');
                $issaved=$this->_model->SAVE_amailbox_accound(['email'=>$data['mailbox'],'accound_id'=>$this->getAccound_id()]);
              //$param['mailboxsummary']=$this->_values['email'];
              //$this->tosupportreturn($issaved,'initAmailbox',$param);
            }

       /*if(key_exists('saveAmailboxfilter',$_POST)){
          $issaved = $this->_model->update_Amailboxfilter($filtervalue,$REMOVE,$TYPE,$LOCATION,$counter,$cattegory_id,$delaydays);
          $param[]='saveAmailboxfilter';
          $this->tosupportreturn($issaved,'saveAmailboxfilter',$param);
       }*/

      }
      $this->prepare();
   }
  public function prepare(){
    $GETDEFAULT = true;
    print $this->_view->getheader($this->_loc,$this->_action);
    print 'ATTENTION NOT WORKING EDITION WE WORK ON IT YOU SEE WHERE WE WIL GO FOR TO LESS SUPPORT';
    print $this->_view->get_template($this->_loc,$this->_action);
    //$this->status($this->_view,true,true);
    if(isset($_GET['Amailboxfilter'])){
       print $this->_view->getform_head($this->_loc,$this->_action);
       $this->_model->GET_Amailboxfilter($this->_loc,'BoxCleanderGET_Amailboxfilter',$this->_view);
       print $this->_view->getform_end('update_Amailboxfilter','Amailboxfilter');
       $GETDEFAULT =false;
    }
    if(isset($_GET['addAmailboxfilter'])){
       print $this->_view->getform_head($this->_loc,$this->_action);
       print $this->_view->get_template($this->_loc,'BoxCleanerADD_Amailboxfilter',$this->_values);
       print $this->_view->getform_end('addfilter','addfilter');
       $GETDEFAULT =false;
    }
    if(isset($_GET['installsql'])){
       $this->_model->InstallData();
       $import = new \data\SqlImport();
  	   $import->importFile('includes/sqls/filters.sql',false,true);
       $GETDEFAULT =false;
    }
    if(isset($_GET['filters'])){
       if($this->isamailbox($this->_values['mailbox'])){
		   print $this->_view->getform_head($this->_loc,$this->_action);
		   $array['id'] = 'exclusive';
           $array['title'] = $this->_values['filters.info.exclusief'];
           print $this->_view->get_template('global','section_head',$array);
	       		$this->_model->GET_filters(
		   			$this->_loc,'BoxCleanerSUM_Amailboxfilterselected_item',$this->_view,$this->getAccound_id(),$this->_values['mailbox'],$this->_values['page']);
	       print $this->_view->get_template('global','section_foot');
	       print '<hr>';
		   $array['id'] = 'inclusive';
           $array['title'] = $this->_values['filters.info.inclusieve'];
           print $this->_view->get_template('global','section_head',$array);
		   $this->_model->GET_filters($this->_loc,'BoxCleanerSUM_Amailboxfilter_item',$this->_view,$this->getAccound_id(),$this->_values['mailbox'],$this->_values['page'],true);
	       
		   print $this->_view->get_template('global','toolcheck_end',$this->_values);
		   print $this->_view->get_template('global','section_foot');
	       print $this->_view->getform_end('update_filters','update_filters');
	       
		   //print_r($this->_values['mailboxinfo']);
		   $GETDEFAULT = false;
	   }
    }
    if(isset($_GET['Amailboxaccound'])){
       print $this->_view->getform_head($this->_loc,$this->_action);
       $this->_model->GET_Amailboxaccound($this->_loc,'BoxCleanerGET_Amailboxaccound',$this->_view);
       print $this->_view->getform_end('update_Amailboxaccound','Amailboxaccound');
       $GETDEFAULT =false;
    }

    if(isset($_GET['Amailboxbindfiltersummary'])){
       print $this->_view->getform_head($this->_loc,$this->_action);
       $this->_model->GET_AmailboxbindfiltersSummary($this->_loc,'BoxCleanderSUM_Amailboxbindfilter_item',$this->_view,$this->_values['page'],$this->getAccound_id());//TODO check filters looks like "$enabled,$activated "
       print $this->_view->get_template('global','toolcheck_end',$this->_values);
       print $this->_view->getform_end('update_Amailboxbindfilter','Amailboxbindfilter');
       $GETDEFAULT = false;
    }
    if(isset($_GET['Amailboxlog'])){
       print $this->_view->getform_head($this->_loc,$this->_action);
       $this->_model->GET_Amailboxlog($this->_loc,'BoxCleanderGET_Amailboxlog',$this->_view);
       print $this->_view->getform_end('update_Amailboxlog','Amailboxlog');
       $GETDEFAULT =false;
    }
    if(isset($_GET['Amailboxlogsummary'])){
       print $this->_view->getform_head($this->_loc,$this->_action);
       $this->_model->GET_AmailboxlogsSummary($this->_loc,'BoxCleanderSUM_Amailboxlog_item',$this->_view,$this->_values['page'],$this->getAccound_id());//TODO check filters looks like "$enabled,$activated "
       print $this->_view->getform_end('update_Amailboxlog','Amailboxlog');
       $GETDEFAULT = false;
    }
    if(isset($_GET['summary'])){
       $this->_model->GET_Summary($this->_loc,'BoxCleanderSUM_item',$this->_view,$this->_values['page'],$this->getAccound_id());//TODO check filters looks like "$enabled,$activated "
       $GETDEFAULT = false;
    }
    if(isset($_GET['cleaning'])&& $this->_validator->isValid('selectmailbox')){
       $this->cleanMailbox($this->_values['mailbox']);
       $GETDEFAULT =false;
    }
    if(isset($_GET['scan'])&& $this->_validator->isValid('selectmailbox')){
       $this->cleanMailbox($this->_values['mailbox']);
       $GETDEFAULT =false;
    }
    if(isset($_GET['mailboxsummary'])){
       $options['id'] = 'boxes';
       $options['title'] = 'boxes';
       print $this->_view->get_template('global','section_boxhead',$options);
		$path = $this->_user->getPersonalPath();
		$scanner =  new \data\FileScanner();
		$arr['inc']=['.acc.php'] ;
		$scanner->setfilter($arr);
		$scanner->scan2extfiles($path);
		$mailacc = $scanner->getfils();
		foreach($mailacc as $file){
			$acc = $this->ReadProfileFSET($file);
			//$acc['accmailbox']=$file;
			$acc['isautoclean']=($acc['autocleanup']=='')?' checked="checked"':'on';
			print $this->_view->get_Templateclone('users','ProfileEmailacc',$acc);
	   }
	   print $this->_view->get_template($this->_loc,'BoxCleaner_mailbox_menu');
       print $this->_view->get_template('global','toolcheck_end',$this->_values);
       print $this->_view->get_template($this->_loc,'section_boxfoot',$options);
       //print $this->_view->getform_end('1','Updateboxes');
       $GETDEFAULT = false;
    }
    if($GETDEFAULT){
       print $this->_view->getform_head($this->_loc,$this->_action);
       $params['position']=__FILE__.'@line'.__LINE__;
       $params['location']=$this->_loc;
       print $this->_view->get_template('global','TODO',$params);//TODO CHECK 
       print $this->_view->getform_end('1','TODO');
    }
    print $this->_view->getfooter($this->_action);
  }
 /*
 * options 
*/
  private function vars(){
        $this->_values['getAmailboxfilter']=(isset($_POST['getAmailboxfilter']))? $_POST['getAmailboxfilter']:'';
		$this->_values['TYPE']='';
		if(key_exists('mailbox',$_GET)){
			$this->_validator->addForm('selectmailbox',['mailbox','page'],false,['page'],$this->_values);	
			$this->_validator->addAlocat('selectmailbox',['mailbox'=>'isemail']);
			$this->_validator->setDebug(true);
	  		$this->_validator->render('selectmailbox',$_GET,$this->_values);
	  		if($this->_validator->isValid('selectmailbox')){
				$this->_values['mailboxinfo']= $this->getEmailinfo($this->_values['mailbox']);
	  		}
	  	}
		$this->_validator->addForm('addAmailboxfilter',['filtervalue','REMOVE','TYPE','LOCATION','delaydays','cattegory_id'],false,['REMOVE','cattegory_id'],$this->_values);
		if(key_exists('addfilter',$_POST)){
	  		$this->_validator->addAlocat('addAmailboxfilter',['delaydays'=>'isnumeric','cattegory_id'=>'isnumeric','REMOVE'=>'isbool']);
	  		$this->_validator->render('addAmailboxfilter',$_POST,$this->_values);
		}
		if(key_exists('initAmailbox',$_POST)){
			$this->_validator->addForm('initmailbox',['mailbox'],false,[],$this->_values);
	  		//$this->_validator->setDebug(true);
	  		$this->_validator->addAlocat('initmailbox',['mailbox'=>'isemail']);
	  		$this->_validator->render('initmailbox',$_POST,$this->_values);
		}
		$this->_values['types']=$this->_view->getOptions($this->_model->filtertypes,$this->_values['TYPE']);
		$this->_values['locations']=$this->_view->getOptions($this->_model->filterlocations,$this->_values['LOCATION']);
        $this->_values['Amailboxfiltersummary']=(isset($_POST['Amailboxfiltersummary']))?$_POST['Amailboxfiltersummary']:'';
               
        $this->_values['Amailboxbindfiltersummary']=(isset($_POST['Amailboxbindfiltersummary']))?$_POST['Amailboxbindfiltersummary']:'';
        $this->_values['getAmailboxlog']=(isset($_POST['getAmailboxlog']))? $_POST['getAmailboxlog']:'';//TODO create a beter name for get
        $this->_validator->addForm('addlog',['info','amailbox_acound_id','importantlevel','DECODE','ENABLED'],false,[],$this->_values);
	  	if(isset($_POST['addlog'])){
	  		$this->_validator->setDebug(true);
	  		$this->_validator->addAlocat('addlog',['amailfilter_id'=>'isnumeric','importantlevel'=>'isnumeric','ENABLED'=>'isbool','DECODE'=>'isbool']);
	  		$this->_validator->render('addlog',$_POST,$this->_values);
		}
  }
  private function checkItS(){
     if(!isset($_GET['check'])){return ;}
     $return = false;
       if(key_exists('bind',$_POST)){
			$data = $this->_validator->UrlToArray($_SERVER['HTTP_REFERER']);
			//print_r($data);
       		foreach($_POST['bind'] as $id => $bind){
       			//print $data['mailbox'];
       			
       			$mail_id = $this->_model->GETID_amailbox_accound(['email'=>$data['mailbox']]);
       			if(key_exists('id',$mail_id)){
					if($bind=='false'){
	       				$this->_model->UNBIND_amailbox_bind_filter(['amailbox_id'=>$mail_id['id'] ,'amailfilter_id'=>$id]);
					}else{
						$this->_model->BIND_amailbox_bind_filter(['amailbox_id'=>$mail_id['id'] ,'amailfilter_id'=>$id]);
					}
				}else{
					print 'mailbox not found';
				}
			}
			return true; 
       }
	   if(isset($_POST['REMOVE'])){           
                foreach($_POST['REMOVE'] as $id => $REMOVE){
                    if(!empty($id)&& !empty($REMOVE)){
                    	$data['keys']= ['id'];
                    	$data['id']= $id;
                    	$data['REMOVE']= ($REMOVE === 'true')?'TRUE':'FALSE';
						$this->_model->CHANGE_amailbox_filter($data);
                    }
                }
                return true; 
           }
       if(isset($_POST['CLEANON'])){           
                foreach($_POST['CLEANON'] as $id => $CLEANON){
                    if(!empty($id)&& !empty($CLEANON)){
                        //TODO if($this->_model->setCLEANON???($id,$CLEANON)){       $return = true; }
                    }
                }
           }
       if(isset($_POST['AUTOCLEAN'])){           
                foreach($_POST['AUTOCLEAN'] as $id => $AUTOCLEAN){
                    if(!empty($id)&& !empty($AUTOCLEAN)){                //TODO if($this->_model->setAUTOCLEAN???($id,$AUTOCLEAN)){       $return = true; }
                    }
                }
           }
       if(isset($_POST['DECODE'])){           
                foreach($_POST['DECODE'] as $id => $DECODE){
                    if(!empty($id)&& !empty($DECODE)){
                        //TODO if($this->_model->setDECODE???($id,$DECODE)){       $return = true; }
                    }
                }
           }
       if(isset($_POST['ENABLED'])){           
                foreach($_POST['ENABLED'] as $id => $ENABLED){
                    if(!empty($id)&& !empty($ENABLED)){
                        //TODO if($this->_model->setENABLED???($id,$ENABLED)){       $return = true; }
                    }
                }
           }
    }
    private function canDelete($accound_id){
        	return true;
    }
    private function getEmailinfo($email){

			$data = $this->ReadProfileFSET($email.'.acc.php');
			return $data;
    }
    private function getfilterdata($mailboxid){			
	}
    private function cleanMailbox($box,$pre_show=true){

    	if($this->isamailbox($box)){
    		//print_r($this->_values['mailboxinfo']);
    		$filters['ip']= $this->_model->GET_filtersList($this->getAccound_id(),$box,'ip');
    		$filters['domain']=$this->_model->GET_filtersList($this->getAccound_id(),$box,'domain');
    		$filters['regex']=$this->_model->GET_filtersList($this->getAccound_id(),$box,'regex');
    		$filters['email']=$this->_model->GET_filtersList($this->getAccound_id(),$box,'email');
			$arr['mailbox'] = $box;			
    		//$arr['mailbox'] = '<pre>'.print_r($filters,true).'</pre>';
			$host = substr($box,strpos( $box,'@')+1);
            $frm['formname']='clean';
            $frm['id']='clean';
    		print $this->_view->getform_head($this->_loc,'cleaner_head',$frm);//$arr
            //$mailbox = new \Amail\MyMailCleanup($host,$debug= false);
			$mailbox = new \Amail\MyMailCleanup('mail.djdb.be',true);
    		$mailbox->setEdomainsToDEL($filters['domain']);
    		$mailbox->setEMailsAdressesToDEL($filters['email']);
    		$page = 1;
    		$length = 20;
            $islogin = false;
            if(key_exists('test',$_GET)){
				$islogin = $mailbox->loginImap(
                $this->_values['mailboxinfo']['accmailbox'],
                $this->_values['mailboxinfo']['pasword'],
                $this->_values['mailboxinfo']['server_portin'],
                $this->_values['mailboxinfo']['server_in']
                );
				$mailbox->info();
			}

			if(key_exists('eml',$_GET)){
				$path = $this->_user->getPersonalPath().'eml';
				$mailbox->mailsToEml($this->getpro);
			}
    		if($islogin){
                print 'your path'.$this->_user->getPersonalPath(); 
				$mailbox->previewForToDeleteEmails($this->_view,$page,$page+$length);	
			}else{
				print '$islogin not enabled => login falling';
				if(key_exists('checkconnection',$_GET)){
					$mailbox->tryPosibleconfig($this->_values['mailboxinfo']['server_in'],$this->_values['mailboxinfo']['accmailbox'],$this->_values['mailboxinfo']['pasword']);
				}else{
					print $this->_view->get_template($this->_loc,'boxcleaner_loginfaildebugmenu',$this->_values);
       
				}
			}
    		$mailpaginator =  new \autosystem\Paginator();
    		$mailpaginator->setRange(20);
    		$mailpaginator->setlistlength($length);
        	$mailpaginator->setpagenr($page);
        	$mailpaginator->setpaginatorreload($this->_loc,$this->_action);
        	$mailpaginator->setSort('');
            print $this->_view->getpaginator($mailpaginator->getdata());
            print $this->_view->getform_head($this->_loc,'cleaner_foot');
    		//print $this->_view->getform_head($this->_loc,$this->_action);
       		//print $this->_view->get_template($this->_loc,'BoxCleanerINIT_Amailbox',$this->_values);
       		//print $this->_view->getform_end('initAmailbox','initAmailbox');
			return true;
		}
       	return false;
    }
    public function runtest(){
            print '<1>start test';
            $this->loadSettings();// TODO NEED TO SEE
            $host = substr($this->_settings['accmailbox'],strpos($this->_settings['accmailbox'],'@')+1);
        	$mailbox = new \Amail\MyMailCleanup($host,true);
    		$page = 0;
    		$length = 20;
            //$this->_settings['COPYRIGHT'];
            print('<pre>');
 			$mailbox->loginImap($this->_settings['accmailbox'],$this->_settings['TESTPASS']);
            $mailbox->info();
           print('</pre>');
			print 'login falling';
			if(key_exists('checkconnection',$_GET)){
				$mailbox->tryPosibleconfig($host,$this->_settings['accmailbox'],$this->_settings['TESTPASS']);
			}else{
				print $this->_view->get_template($this->_loc,'boxcleaner_loginfaildebugmenu',$this->_values);
			}//imap_ch('mail.djdb.be', 143, 'INBOX', $this->_settings['accmailbox'],$this->_settings['TESTPASS']
            
			print 'end';
    }
    private function readMailbox($box,$pre_show=true){
    	if($this->isamailbox($box)){
    		$mailbox = MyMailCleanup($host,$debug= false);
			return true;
		}	
       	return false;
    }
    private function isamailbox($box){
		if(!$this->_model->exist_mailbox($box)){
	    	print $this->_view->getform_head($this->_loc,$this->_action);
       		print $this->_view->get_template($this->_loc,'BoxCleanerINIT_Amailbox',$this->_values);
       		print $this->_view->getform_end('initAmailbox','initAmailbox');
       		return false;
		}
		return true;
	}
    /*public function canIDo($location=''){
        $this->testmode =($location == 'test');
        return $this->testmode;
    }*/
}
?>