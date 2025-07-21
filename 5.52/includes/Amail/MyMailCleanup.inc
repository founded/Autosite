<?php
    //Catchable fatal error: Object of class stdClass could not be converted to string in E:\_A_servers\xampp\htdocs\myhome\autositesvn\autosite --username DJDataB@gmail.com\ autositeV2.8\function\Email\Imap.inc on line 32 Call Stack: 0.0019 344264 1. {main}() E:\_A_servers\xampp\htdocs\myhome\autositesvn\autosite --username DJDataB@gmail.com\ autositeV2.8\function\Email\Imap.inc:0 1.8671 345296 2. MyImap->readheader() E:\_A_servers\xampp\htdocs\myhome\autositesvn\autosite --username DJDataB@gmail.com\ autositeV2.8\function\Email\Imap.inc:4 
    namespace Amail;
	class MyMailCleanup{
       private $host;
       private $port =110;
       private $MyOpenMailBox;
       private $debug;
       private $adresses;
       private $isDomainsToDel;
       private $DomainsToDel;
       private $isEmailsToDel;
       private $EmailsToDel;
       private $Headertags;
       private $msgNrToDelete =array();
       private $url;
       public function __construct($host,$debug= false) {
           $this->host = $host;
           $this->debug = $debug;
           if($debug){
		   		if(!function_exists('imap_open')){
			   		print 'function imap not enabled';
	              //die('PHP.INI =>uncoment ;extension=php_imap.dll');
	              //imap_clearflag_full
                }
          }
       }
       public function loginpop3($user,$pwx){///novalidate-cert
            $this->port=110;    $this->setUrl('/pop3/novalidate-cert');
            return $this->connect($user,$pwx,'');
       }

       public function loginImap($user,$pwx,$port,$host){///novalidate-cert
            $this->host=$host;
            $this->port=$port;     
            $this->setUrl('/imap/ssl/novalidate-cert');
            return $this->connect($user,$pwx);
         }
       public function loginImapInfo($user,$pwx,$port,$host){///novalidate-cert
            $this->host=$host;
            $this->port=$port;     
            $this->setUrl('/imap/ssl/novalidate-cert');
            return $this->connect($user,$pwx);
       }
       public function loginImapDEBUG($user,$pwx){///novalidate-cert
            $this->port=993;
            if($this->debug){print'{'.$this->host.':'.$this->port.'/imap/ssl/debug}INBOX'.$user.'*******';}//.$pwx};
            $isopen = ($this->MyOpenMailBox = imap_open('{'.$this->host.':'.$this->port.'/imap/ssl/debug}INBOX',$user,$pwx));
			print ($isopen)?'open':'notopen';
			$imap_obj = imap_check($this->MyOpenMailBox);
			var_dump($imap_obj);
            return $isopen;//or die(new Exception('connection failed'));
       }
       private function setUrl($back){
            $this->url = $this->host.':'.$this->port.$back;
       }
       private function connect($user,$pwx,$map = 'INBOX'){
            if($this->debug){print'{'.$this->url.'}'.$map.' '.$user.'*******';}
            return ($this->MyOpenMailBox = imap_open('{'.$this->url.'}'.$map,$user,$pwx));
       }
       public function setAdresses($array){
            $this->adresses = $array;
       }
       public function setHeadertags($array){
            $this->Headertags = $array;
       }
       public function checkalloptions($url){
            $url = $url.'/imap';
			$st = new \autosystem\Stragegy();
			
	    	//$st->add(['','/imap', '/imap2',  '/imap4',  '/pop3']);//'/imap2bis','/imap4rev1',
	    	//$st->add(['', '/norsh']);
			$st->add(['', '/ssl']);
            $st->add(['', '/tls', '/notls']);
			$st->add(['', '/validate-cert', '/novalidate-cert']);
            $st->add([ '/debug']);
	    	return $st->allcombinationsby($url, 1);
	   }
   public function tryPosibleconfig($host,$username, $passw){
		$return = [];
		$theports= ['993'];//,'143','2096'];
		$locations=[];
			foreach($theports as $port){
				$locations = $this->checkalloptions($host.':'.$port);
				foreach($locations as $location){
                    $this->url=$location;
					$this->imap_test($location, $username, $passw);
				}
			}
			/*foreach($theports as $port){
				$locations = $this->checkalloptions('127.0.0.1:'.$port);
				foreach($locations as $location){
					$this->imap_test($location, $username, $passw);
				}
			}*/

	}
	function imap_test($connectionstring, $username, $passw) {
		$basedir=['INBOX'];//,''];
		$mbox=false;
        foreach($basedir as $dir) {
            $this->MyOpenMailBox = @imap_open("{$connectionstring}".$dir, $username, $passw);
            echo '<br>{'.$connectionstring.'}'.$dir.'</b> ';
            if (false !== $this->MyOpenMailBox ) {
                echo '<span style="color: green"> success</span>';
                $this->info();
            }else{
                echo '<span style="color: red"> failed</span>last error: ' . imap_last_error() ;
            }
        }
    }

// imap_open("{w2010ExchangeServer:993/imap/ssl}", $user, $password, NULL, 1, array('DISABLE_AUTHENTICATOR' => 'GSSAPI'));

       public function checkForToDeleteEmails($start=1,$length){
            $this->validateInputs();
            if(isset($this->MyOpenMailBox)&&$headers = imap_check($this->MyOpenMailBox)){
                $endnr =$headers->Nmsgs;
                if($mails =imap_fetch_overview($this->MyOpenMailBox,$start.':'.$length,0)){
                    foreach($mails as $mailnr => $mailcontent){
                        if(++$mailnr>=0 && $Actiefmailheader = imap_header($this->MyOpenMailBox,$mailnr)){
                            if($this->MustEmailDeleteFromHead($Actiefmailheader)){
                                //imap_delete($this->MyOpenMailBox,$mailnr);
                                $Actiefmailheader = imap_header($this->MyOpenMailBox,$mailnr);
                                //print($mailnr);
                                $this->debugprint($Actiefmailheader);
                                print '<h1>DELETE</h1>';
                                
                            }else{
                               // print_r($Actiefmailheader->sender);
                                //$this->debugprint($Actiefmailheader);
                            }
                            if(isset($header->sender[0])){
                                $this->printHeader($Actiefmailheader);
                            }else{
                                print '<pre>'.print_r($Actiefmailheader,true).print_r($mailcontent,true).'</pre>';
                            }
                        }
                    }
                }
            }
       }
       public function previewForToDeleteEmails(&$view,$start=1,$length,$template = 'MyMailCleanup_item',$loc = 'Amail'){
            $this->validateInputs();
            if(isset($this->MyOpenMailBox)&&$headers = imap_check($this->MyOpenMailBox)){
                $endnr =$headers->Nmsgs;
                //print '<br><br>previewForToDeleteEmails'.$endnr;
                if($mails =imap_fetch_overview($this->MyOpenMailBox,"$start:$endnr",0)){
                    foreach($mails as $mailnr => $overview){
                        //print '<br>select'.$mailnr.'<br>';
                        if(++$mailnr>=0 && $Actiefmailheader = imap_header($this->MyOpenMailBox,$overview->msgno)){
                        	//$Actiefmailheader = imap_header($this->MyOpenMailBox,$mailnr);
							$c=[];
                        	$c['class']= $this->MustEmailDeleteFromHead($Actiefmailheader)?'DELETE ERROR':'OK NOT';
                            $c['mailnr']= $overview->msgno .' /./ teller'.$mailnr;
                            $c['from']=$overview->from;
                            $c['date']=$overview->date;
                            $c['subject']=$overview->subject;
                            $c['header']='<pre>'.print_r($Actiefmailheader,true).'</pre>';
                            $c['sender']='<pre>'.print_r($Actiefmailheader->sender,true).'</pre>';
                            if(is_array($Actiefmailheader->sender) && $Actiefmailheader->sender instanceof stdClass){
                                $c['sender']=trim($Actiefmailheader->sender->mailbox).trim($Actiefmailheader->sender->host);
                                $c['nicname']=$Actiefmailheader->sender->personal;
                                if (trim($c['nicname'])==''){
                                    $c['nicname']=$c['sender'];
                                }
                            }
                            $c['STATUS']='ok';
                            
                            /*if(isset($header->sender[0])){
								$this->printHeader($Actiefmailheader);
                            }else{
                                print '<pre>'.print_r($Actiefmailheader,true).print_r($mailcontent,true).'</pre>';
                            }*/
                            
							print $view->get_Templateclone($loc,$template,$c);
                        }
                    }
                }
            }
       }
       public function Simulator(&$view,$start=1,$length,$template = 'MyMailCleanup_item',$loc = 'Amail'){
            $c=[];
        	$c['class']= $this->MustEmailDeleteFromHead($Actiefmailheader)?'DELETE ERROR':'OK NOT';
            $c['mailnr']= $overview->msgno .' /./ teller'.$mailnr;
            $c['from']=$overview->from;
            $c['date']=$overview->date;
            $c['subject']=$overview->subject;
            $c['header']='<pre>'.print_r($Actiefmailheader,true).'</pre>';
            $c['sender']='<pre>'.print_r($Actiefmailheader->sender,true).'</pre>';
            $c['STATUS']='ok';
            print $view->get_Templateclone($loc,$template,$c);
       }
       /**
	   INBOX
	   */
       public function mailsToEml($path,$id=0,$delete=false){
			//$this->MyOpenMailBox = imap_open("{mail.messagingengine.com:143}INBOX", "**", "**");
			if(isset($this->MyOpenMailBox)&& $message_count = imap_num_msg($this->MyOpenMailBox)){
				echo '<h3>'.$message_count.' Emails Returned</h3>';
				if($id !==0){
					$this->mailIdToEml($path,$id);
				}else{
					for ($i = 1; $i <= $message_count; ++$i){
						$this->mailIdToEml($path,$i);
					}
				}
			}	
	   }
	   private function mailIdToEml($path,$id){
			$header = imap_header($this->MyOpenMailBox, $id);
			$personal = (isset($header->from[0]->personal))? $header->from[0]->personal:$header->from[0]->mailbox;
			$eml = "date : $header->Date \n";
			$eml .= "from : $personal
			<{$header->from[0]->mailbox}@{$header->from[0]->host}> \n";
			$eml .= "subject : $header->Subject \n";
			$eml .= 'to : '.$personal.'
			<{'.$header->to[0]->mailbox.'}@{'.$header->from[0]->host.'}> '."\n";
			$body =imap_body($this->MyOpenMailBox, $id, 'FT_PEEK');
			$eml .= $body ."\n";
			$messageID = imap_bodystruct($this->MyOpenMailBox, $id, 'message_id');			
			if($this->debug){
				echo '<h3>header</h3><pre style="color:red;" >'.print_r($header,true).'</pre>';
				echo '<h3>Body</h3><pre style="color:red;" >'.print_r(imap_body($this->MyOpenMailBox, $id, 'FT_PEEK'),true).'</pre>';
			}
			$file = $path.$header->from[0]->host.'_'.$messageID.'.eml';
			if (!$file_handle = fopen($file,'a')) { echo 'Cannot open file'; }
			if (!fwrite($file_handle, $eml)) { echo 'Cannot write to file'; }
			echo '<h4>You have successfully written data to $file</h4>';
			fclose($file_handle);
	   }
	   public function mailsToEmlB(){
	   		if(isset($this->MyOpenMailBox)){
				$res = imap_savebody($imap, '../eml/test.eml', 2);
			}
	   }
       private function validateInputs(){
            $this->isDomainsToDel = is_array($this->DomainsToDel);
            $this->isEmailsToDel = is_array($this->EmailsToDel);
       }
       public function deleteSelection(){
            imap_expunge($this->MyOpenMailBox);
       }
       public function MustEmailDeleteFromHead($header){           
            if(isset($header->sender[0])){
                return $this->isDomainInCleaningList($header->sender[0]->host)||
                $this->isEmailInCleaningList($header->sender[0]->mailbox.'@'.$header->sender[0]->host);
            }else{
                var_dump($header); 
                return false;
            }
       }
       private function isDomainInCleaningList($domain){    return $this->isDomainsToDel && in_array($domain,$this->DomainsToDel);       }
       private function isEmailInCleaningList($domain){     return $this->isEmailsToDel && in_array($domain,$this->EmailsToDel);       }
       public function setEdomainsToDEL($domainsarray){
            $this->DomainsToDel = $domainsarray;
       }
       public function setEMailsAdressesToDEL($emailarray){
            $this->EmailsToDel = $emailarray;
       }
       public function deleteEmailFromAdresses(){
            
       }
       public function deleteEmailFromHeadertags(){
        
       }
       public function get(){
            if(isset($this->MyOpenMailBox)){
                $fstruct = imap_fetchstructure();
            }
       }
       private function isSelectForDel($header){
            return $header->Deleted == 'D';
       }
       private function debugprint($array){
            print '<pre>'.print_r($array,true).'</pre>'; 
       }
       /**
        * MyImap::__destruct()
        * @return void
        */
       public function __destruct() {
          if(isset($this->MyOpenMailBox)&&$this->MyOpenMailBox!=false){
                imap_close($this->MyOpenMailBox);
          } 
       }
       public function printHeader($header){
            print('<span> NR:'.$header->Msgno.'</span><span> '.(($this->isSelectForDel($header))?'DELETE':'').'</span><span> Mail:'.$header->sender[0]->mailbox.'@'.$header->sender[0]->host.'</span><br><span> Subject:'.@$header->Subject.'</span><br>');
       }
       public function info(){
       		echo ' last error: ' . imap_last_error() . "<br />\n";
			if(isset($this->MyOpenMailBox)){
                $status = @imap_status($this->MyOpenMailBox, '{'.$this->url.'}INBOX', SA_ALL);
			        echo 'Messages:   ' . $status->messages    . "-<br />\n";
			        echo 'Recent:     ' . $status->recent      . "-<br />\n";
			        echo 'Unseen:     ' . $status->unseen      . "-<br />\n";
			        echo 'UIDnext:    ' . $status->uidnext     . "-<br />\n";
			        echo 'UIDvalidity:' . $status->uidvalidity . "-<br />\n";
            }
            var_dump($this->MyOpenMailBox);
			echo ' last error: ' . imap_last_error() . "<br />\n";		
	   }
	   public function debug(){
   			$configs = parse_ini_file('config.ini', true);
			print_r($configs['email'] );
			print_r($configs['imap'] );
			print_r($configs['pop3'] );
			print_r($configs['smtp'] );
	   }  
    }
?>