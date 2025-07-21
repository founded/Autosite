<?php
/**
* @author Lieven Roegiers
* @copyright 
* @CMS autosite 
*/
namespace Amail;
class ModerateEmails extends \autosystem\Controller{
     private $values = [];
     private $categorys= [];
      public function init(){
        $this->getModel();
        if($this->checkItS()){return;}//upgrade speed
		$this->vars(); 
        if(count($_POST)>0){
       if(key_exists('addAmailcategory',$_POST)){
          if(!$this->_model->exist_ModerateEmailsAmailcategory($this->_values['name'])){
            $issaved = $this->_model->save_category($this->_values['name'],$this->_values['type_id'],$this->_values['mailadminlog_id'],$this->_values['menuindex']);
          }else{
            $issaved = false;
          } 
          $param[]='categorys';
          $this->returnsupport($issaved,'addAmailcategory',$param);
       }
       if(key_exists('saveAmailcategory',$_POST)){
          $issaved = $this->_model->update_Amailcategory($name,$type_id,$mailadminlog_id,$menuindex);
          $param[]='saveAmailcategory';
          $this->returnsupport($issaved,'saveAmailcategory',$param);
       }
       if(key_exists('addconceptmail',$_POST)){
       	  //print $this->_model->exist_conceptmail($this->_values['NAMEDKEY'])?'true':'false';
	   	  $accound_id = $this->getAccound_id();
		  if($this->_model->exist_conceptmail($this->_values['NAMEDKEY'])){
			$param[]='addconceptmail';
			$issaved = false;
			$this->_model->Log_Admin($accound_id,'try to add concept named '.$this->_values['NAMEDKEY'].'');
		  }else{
			$param[]='conceptmails';	//print_r($this->_values);				//($NAMEDKEY,$STATUS,$SENDIBLE,$amail_category_id,$content,$contenttype='text/plain')
			$issaved = $this->_model->save_conceptmail($this->_values['NAMEDKEY'],$this->_values['STATUS'],$this->_values['issendible'],$this->_values['amail_category_id'],$this->_values['content'],$accound_id,$this->locale);
          	$this->_model->Log_Admin($accound_id,'add concept named '.$this->_values['NAMEDKEY'].''.$this->_values['info']);
		  }  	       
          
          $this->returnsupport($issaved,'addAmailconceptmail',$param);
       }
       if(key_exists('saveAmailconceptmail',$_POST)){
          $issaved = $this->_model->update_Amailconceptmail($name,$type_id,$mailadminlog_id,$menuindex,$NAMEDKEY,$STATUS,$SENDIBLE,$amail_category_id,$mail_content_id,$imputdate);
          $param[]='saveAmailconceptmail';
          $this->returnsupport($issaved,'saveAmailconceptmail',$param);
       }
       if(key_exists('addAmailstyle',$_POST)){
          $this->_model->add_Amailstyle($name,$type_id,$mailadminlog_id,$menuindex,$NAMEDKEY,$STATUS,$SENDIBLE,$amail_category_id,$mail_content_id,$imputdate,$amail_conceptmail_id,$key,$value);
          $param[]='addAmailstyle';
          $this->returnsupport($issaved,'addAmailstyle',$param);
       }
       if(key_exists('saveAmailstyle',$_POST)){
          $issaved = $this->_model->update_Amailstyle($name,$type_id,$mailadminlog_id,$menuindex,$NAMEDKEY,$STATUS,$SENDIBLE,$amail_category_id,$mail_content_id,$imputdate,$imputdate,$amail_conceptmail_id,$key,$value);
          $param[]='saveAmailstyle';
          $this->returnsupport($issaved,'saveAmailstyle',$param);
       }
       if(key_exists('addAmailcontent',$_POST)){
          $this->_model->add_Amailcontent($name,$type_id,$mailadminlog_id,$menuindex,$NAMEDKEY,$STATUS,$SENDIBLE,$amail_category_id,$mail_content_id,$imputdate,$imputdate,$amail_conceptmail_id,$key,$value,$s,$content_namedkey);
          $param[]='addAmailcontent';
          $this->returnsupport($issaved,'addAmailcontent',$param);
       }
       if(key_exists('saveAmailcontent',$_POST)){
          $issaved = $this->_model->update_Amailcontent($name,$type_id,$mailadminlog_id,$menuindex,$NAMEDKEY,$STATUS,$SENDIBLE,$amail_category_id,$mail_content_id,$imputdate,$imputdate,$amail_conceptmail_id,$key,$value,$s,$content_namedkey);
          $param[]='saveAmailcontent';
          $this->returnsupport($issaved,'saveAmailcontent',$param);
       }
       if(key_exists('addcontentpart',$_POST)){
          $this->_model->save_ModerateEmailsAmailcontentpart($this->_values['id'], $this->_values['contenttype'],$this->_values['content']);
          $param[]='addcontentpart';
          $this->returnsupport($issaved,'addcontentpart',$param);
       }
       if(key_exists('addcontentpartto',$_POST)&&$this->_validator->isValid('addpart')){
          //print'test'.$this->_values['contenttype'].$this->_values['TYPE'].$this->_values['content'].$this->_values['amail_conceptmail_id'];
		  
          $issaved = $this->_model->save_contentpartto(
          
          $this->_values['contenttype'], $this->_values['TYPE'],$this->_values['content'],
		  
		  $this->_values['amail_conceptmail_id']);
          $param[]='addpartto';
          $param[]='concept='.$this->_values['amail_conceptmail_id'];
          
          //$this->tosupportreturn($issaved,'addcontentpartto',$param);
          //$this->returnsupport($issaved,'addcontentpart',$param);
       }
       if(key_exists('addimgcid',$_POST)){
            if($this->_validator->isValid('addimgcid')){
                $uploader = new \autosystem\Upload();
                $fields = ['image'];
                $trueExtentions = ['jpg','gif','png'];
                
                $Dcid = \autosystem\Key::GET(20);            
                //if(is_dir('./layout/ups/'.$this->_model->GET_Title($this->_values['images_category']).'/')){
                $path = $this->GetProfilePath('images',true) ;   
                $uploader->load($_FILES,$fields,$trueExtentions,$path,$this->getAccound_id().'_'.$Dcid);
                $upslinks = $uploader->getlinks();
                $aimage =  new \images\AImage();
                $issaved = false;
                foreach($upslinks as $category => $value){
                    foreach($value as $size => $link){
                        $link = substr($link,1);
                        $mime = $aimage->getFilemime( $link);
                        $filecontent =  base64_encode(file_get_contents('.'.$link));
                        if(!empty($filecontent)&& $this->_model->save_cidimg($this->_values['amail_contentpart_id'],$mime,$Dcid,$filecontent,$this->_values['amail_conceptmail_id'])){
                            $issaved = true;
                        }else{
                           print $link; 
                           $this->setInConsole(false,'saved','not.saved');
                        }
                    }
                }
                //print_r($upslinks);
            }
           /**
            *                               '`id` INT (4) NOT NULL AUTO_INCREMENT,'.
               '`cid` VARCHAR (230) COLLATE '.\autosystem\DBConn::getCOLLATE().' NOT NULL,'.
               '`typeencoding` VARCHAR (230) COLLATE '.\autosystem\DBConn::getCOLLATE().' NOT NULL,'.
               '`amail_contentpart_id` INT(7) NOT NULL,'.
              ' PRIMARY KEY ( `id` )'.
            
            SAVE_amail_contentpart_cid( )
            
*/
       }
       $this->Qform('addamail_dataset_param');
       if(key_exists('saveAmailcontentpart',$_POST)){
          $issaved = $this->_model->update_Amailcontentpart($name,$type_id,$mailadminlog_id,$menuindex,$NAMEDKEY,$STATUS,$SENDIBLE,$amail_category_id,$mail_content_id,$imputdate,$imputdate,$amail_conceptmail_id,$key,$value,$s,$content_namedkey,$contenttype,$amail_dataset_id);
          $param[]='saveAmailcontentpart';
          $this->returnsupport($issaved,'saveAmailcontentpart',$param);
       }
       if(key_exists('addAmaildataset',$_POST)&&$this->_validator->isValid('adddataset')){//'adddataset'
          $this->_model->save_dataset($this->_values['selectconverters'],$this->_values['query']);
          $param[]='addAmaildataset';
          $this->returnsupport($issaved,'addAmaildataset',$param);
       }
       if(key_exists('saveAmaildataset',$_POST)){
          $issaved = $this->_model->update_Amaildataset($name,$type_id,$mailadminlog_id,$menuindex,$NAMEDKEY,$STATUS,$SENDIBLE,$amail_category_id,$mail_content_id,$imputdate,$imputdate,$amail_conceptmail_id,$key,$value,$s,$content_namedkey,$contenttype,$amail_dataset_id,$selectconverters,$query);
          $param[]='saveAmaildataset';
          $this->returnsupport($issaved,'saveAmaildataset',$param);
       }
       if(key_exists('addAmailadminlog',$_POST)){
          $this->_model->add_Amailadminlog($name,$type_id,$mailadminlog_id,$menuindex,$NAMEDKEY,$STATUS,$SENDIBLE,$amail_category_id,$mail_content_id,$imputdate,$imputdate,$amail_conceptmail_id,$key,$value,$s,$content_namedkey,$contenttype,$amail_dataset_id,$selectconverters,$query,$accound_id,$info,$imputdate,$imputdate);
          $param[]='addAmailadminlog';
          $this->returnsupport($issaved,'addAmailadminlog',$param);
       }
       if(key_exists('saveAmailadminlog',$_POST)){
          $issaved = $this->_model->update_Amailadminlog($name,$type_id,$mailadminlog_id,$menuindex,$NAMEDKEY,$STATUS,$SENDIBLE,$amail_category_id,$mail_content_id,$imputdate,$imputdate,$amail_conceptmail_id,$key,$value,$s,$content_namedkey,$contenttype,$amail_dataset_id,$selectconverters,$query,$accound_id,$info,$imputdate,$imputdate);
          $param[]='saveAmailadminlog';
          $this->returnsupport($issaved,'saveAmailadminlog',$param);
       }
       if(key_exists('addAmailemails',$_POST)){
          $this->_model->add_Amailemails($name,$type_id,$mailadminlog_id,$menuindex,$NAMEDKEY,$STATUS,$SENDIBLE,$amail_category_id,$mail_content_id,$imputdate,$imputdate,$amail_conceptmail_id,$key,$value,$s,$content_namedkey,$contenttype,$amail_dataset_id,$selectconverters,$query,$accound_id,$info,$imputdate,$imputdate,$issend,$SENDIBLE,$statuscomment,$conceptmail_id,$accound_id,$imputDate,$imputDate);
          $param[]='addAmailemails';
          $this->returnsupport($issaved,'addAmailemails',$param);
       }
       if(key_exists('saveAmailemails',$_POST)){
          $issaved = $this->_model->update_Amailemails($name,$type_id,$mailadminlog_id,$menuindex,$NAMEDKEY,$STATUS,$SENDIBLE,$amail_category_id,$mail_content_id,$imputdate,$imputdate,$amail_conceptmail_id,$key,$value,$s,$content_namedkey,$contenttype,$amail_dataset_id,$selectconverters,$query,$accound_id,$info,$imputdate,$imputdate,$issend,$SENDIBLE,$statuscomment,$conceptmail_id,$accound_id,$imputDate,$imputDate);
          $param[]='saveAmailemails';
          $this->returnsupport($issaved,'saveAmailemails',$param);
       }
       if(key_exists('addMailcontenttopartsequence',$_POST)){
          $this->_model->add_Mailcontenttopartsequence($name,$type_id,$mailadminlog_id,$menuindex,$NAMEDKEY,$STATUS,$SENDIBLE,$amail_category_id,$mail_content_id,$imputdate,$imputdate,$amail_conceptmail_id,$key,$value,$s,$content_namedkey,$contenttype,$amail_dataset_id,$selectconverters,$query,$accound_id,$info,$imputdate,$imputdate,$issend,$SENDIBLE,$statuscomment,$conceptmail_id,$accound_id,$imputDate,$imputDate,$amail_contentpart_id,$amail_content_id,$seqranking);
          $param[]='addMailcontenttopartsequence';
          $this->returnsupport($issaved,'addMailcontenttopartsequence',$param);
       }
       if(key_exists('saveMailcontenttopartsequence',$_POST)){
          $issaved = $this->_model->update_Mailcontenttopartsequence($name,$type_id,$mailadminlog_id,$menuindex,$NAMEDKEY,$STATUS,$SENDIBLE,$amail_category_id,$mail_content_id,$imputdate,$imputdate,$amail_conceptmail_id,$key,$value,$s,$content_namedkey,$contenttype,$amail_dataset_id,$selectconverters,$query,$accound_id,$info,$imputdate,$imputdate,$issend,$SENDIBLE,$statuscomment,$conceptmail_id,$accound_id,$imputDate,$imputDate,$amail_contentpart_id,$amail_content_id,$seqranking);
          $param[]='saveMailcontenttopartsequence';
          $this->returnsupport($issaved,'saveMailcontenttopartsequence',$param);
       }
      }
      if(isset($this->_values['deleteCategory'])){
          $isdeleted = $this->_model->delete_category($this->_values['deleteCategory']);  
          $param[]='categorys';
          $this->returnsupport($isdeleted,'deleteCategory',$param);
      }
      if($this->_values['deleteConceptmail']){
          $isdeleted = $this->_model->delete_category($this->_values['deleteConceptmail']);  
          $param[]='conceptmails';
          $this->returnsupport($isdeleted,'deleteConceptmail',$param);
      }
      
      if(isset($_GET['conceptmail'])){
        $this->prepareconcept();
        return;
      }
      $this->prepare();
   }
  public function prepareconcept(){
      $conceptid = $_GET['conceptmail'];
      $arr = [];
      $arr['charset']='';
      $arr['backcolor']='#FFCC00';
      $arr['title']=''.$conceptid.'test';
      print $this->_view->get_template($this->_loc,'Ahead',$arr);
      $this->_model->GET_conceptmail($this->_loc,'ModerateEmailsGET_Amailinfo_tools',$this->_view,$conceptid);
      $arr = $this->_model->GET_EmailConceptArrayList($conceptid);
      
      /*
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
      */
      //print_r($arr);
      $sender = new AmailSender();
      print '<pre>';
      $e['boundary']='-boundery-';
      $e['charset']='charset';
      $e['encoding']='encoding';
      //$this->_view->preSetTemplate($this->_loc,'ModerateEmailsGET_Amailconceptpart',$e);
      foreach($arr as $key => $vals){
        //$html =$this->_view->preSetTemplate($this->_loc,'ModerateEmailsGET_Amailconceptpart',$e);
		if(key_exists('amail_dataset_id',$vals)&& $vals['amail_dataset_id'] != 0){
             $dataset = $this->_model->getAmail_dataset($vals['amail_dataset_id'],$this->getAccound_id());
        }else{
             $dataset['link.activate']='http://autosite.dev/v4.0/users/Activate/';
             $dataset['key']='-key-';
             $dataset['link.subscribe']='http://autosite.dev/v4.0/Amail/Mailpool';
        }
        print $this->_view->reSetTemplate($this->_loc,'ModerateEmailsGET_Amailconceptpart',$vals,$e);
      }
      print '</pre>';
      print '<h1>end</h1>';
      print $this->_view->get_template($this->_loc,'Afoot',$this->_values);
  }
  public function prepare(){
    $GETDEFAULT = true;
    print $this->_view->getheader($this->_loc,$this->_action);
    print 'Use qualified personal to use this page or you need check contenttype diehard and add lib';
    print $this->_view->get_template($this->_loc,'ModerateEmailsMenu',$this->_values);
    
    $this->InputMessages();//print_r($this->_values);
    if(isset($_GET['Amailcategory'])){
        //ModerateEmails_model.inc on line 128
       print $this->_view->getform_head($this->_loc,$this->_action);
       $this->_model->GET_Amailcategory($this->_loc,'ModerateEmailsGET_Amailcategory',$this->_view);
       print $this->_view->getform_end('update_Amailcategory','Amailcategory');
       $GETDEFAULT =false;
    }
    print $this->getSectionform($this->_loc,$this->_action,'addimgcid',$this->_values,'addimgcid','','','addimgcid');
    
    if(isset($_GET['addAmailcategory'])){
       print $this->_view->getform_head($this->_loc,$this->_action);
       print $this->_view->get_template($this->_loc,'ModerateEmailsADD_Amailcategory',$this->_values);
       print $this->_view->getform_end('add_Amailcategory','addAmailcategory');
       $GETDEFAULT =false;
    }
    if(isset($_GET['categorys'])){
       print $this->_view->get_template($this->_loc,'ModerateEmailsCategorysMenu',$this->_values);
       $this->_model->GETcategorysSummary($this->_loc,'ModerateEmailsSUM_Amailcategory_item',$this->_view,$this->_values['page'],$this->getAccound_id());//TODO check filters looks like "$enabled,$activated "
       $GETDEFAULT = false;
    }
    if(isset($_GET['addconceptmail'])){
       print $this->_view->getform_head($this->_loc,$this->_action);
       print $this->_view->get_template($this->_loc,'ModerateEmailsADD_Amailconceptmail',$this->_values);
       print $this->_view->getform_end('addconceptmail','addconceptmail');
       $GETDEFAULT =false;//            
    }
    if(isset($_GET['conceptmails'])){
        $array['id'] = 'sectionemails';
        $array['title'] = '';
        print $this->_view->get_template('global','section_head',$array);
        print $this->_view->getform_head($this->_loc,$this->_action);
        $this->_model->GET_conceptmailsSummary($this->_loc,'ModerateEmailsSUM_Amailconceptmail_item',$this->_view,$this->_values['page'],$this->getAccound_id());//TODO check filters looks like "$enabled,$activated "
        print $this->_view->getform_end('update_Amailconceptmail','saveconceptmails');
        print $this->_view->get_template('global','section_foot',[]);
        $array['id'] = 'sectionpground';
        $array['title'] = '';
        print $this->_view->get_template('global','section_head',$array);
        print $this->_view->get_template($this->_loc,'ModerateEmailsConceptMenu',$this->_values); 
        print $this->_view->get_template($this->_loc,'ModerateEmailsConceptIframe',$this->_values);
        print $this->_view->get_template('global','toolcheck_end',$this->_values);
        print $this->_view->get_template('global','section_foot',[]);
        $GETDEFAULT = false;
    }
    /*if(isset($_GET['Amailstyle'])){
       print $this->_view->getform_head($this->_loc,$this->_action);
       $this->_model->GET_Amailstyle($this->_loc,'ModerateEmailsGET_Amailstyle',$this->_view);
       print $this->_view->getform_end('update_Amailstyle','Amailstyle');
       $GETDEFAULT =false;
    }*/
    //$loc,$action,$template,$values,$submitbtn,$id,$title='',$validatorform
	//print $this->getSectionform($this->_loc,$this->_action,'ModerateEmailsADD_Amailstyle',$this->_values,'add_Amailstyle','','','amailstyle' );

    if(isset($_GET['addAmailstyle'])){
       print $this->_view->getform_head($this->_loc,$this->_action);
       print $this->_view->get_template($this->_loc,'ModerateEmailsADD_Amailstyle',$this->_values);
       print $this->_view->getform_end('add_Amailstyle','add');
       $GETDEFAULT =false;
    }
    if(isset($_GET['partsfrom'])){
       print $this->_view->get_template($this->_loc,'ModerateEmailsSUM_partsfrom_head',$this->_values); 
       print $this->_model->GET_amail_contentpartsSummary($this->_loc,'ModerateEmailsSUM_partsfrom_item',$this->_view,$this->accountid,$this->_values['amail_conceptmail_id']);
       print $this->_view->get_template($this->_loc,'ModerateEmailsSUM_partsfrom_foot',$this->_values); 
       $GETDEFAULT =false;
    }
    /*if(isset($_GET['Amailstylesummary'])){
       print $this->_view->getform_head($this->_loc,$this->_action);
       $this->_model->GET_AmailstylesSummary($this->_loc,'ModerateEmailsSUM_Amailstyle_item',$this->_view,$this->_values['page'],$this->getAccound_id());//TODO check filters looks like "$enabled,$activated "
       print $this->_view->getform_end('update_Amailstyle','Amailstyle');
       $GETDEFAULT = false;
    }*/

    if(isset($_GET['datasets'])){
       print $this->_view->get_template($this->_loc,'ModerateEmailsDatasetsMenu',$this->_values);
       print $this->_view->getform_head($this->_loc,$this->_action);
       $this->_model->GET_datasetsSummary($this->_loc,'ModerateEmailsSUM_Amaildataset_item',$this->_view,$this->_values['page'],$this->getAccound_id());//TODO check filters looks like "$enabled,$activated "
       print $this->_view->get_template('global','toolcheck_end',$this->_values);
       print $this->_view->getform_end('update_Amailcontent','updatecontents');
       $GETDEFAULT = false;
    }
    if(isset($_GET['Amailcontent'])){
       print $this->_view->getform_head($this->_loc,$this->_action);
       $this->_model->GET_Amailcontent($this->_loc,'ModerateEmailsGET_Amailcontent',$this->_view);
       print $this->_view->getform_end('update_Amailcontent','Amailcontent');
       $GETDEFAULT =false;
    }
    if(isset($_GET['addcontent'])){
       print $this->_view->getform_head($this->_loc,$this->_action);
       print $this->_view->get_template($this->_loc,'ModerateEmailsADD_Amailcontent',$this->_values);
       print $this->_view->getform_end('add_Amailcontent','add');
       $GETDEFAULT =false;
    }
    if(isset($_GET['Amailcontentpart'])){
       print $this->_view->getform_head($this->_loc,$this->_action);
       if(!isset($_GET['for'])||!is_numeric($_GET['for'])||$_GET['for']<1 ){
            $this->_model->GET_Amailcontentpart($this->_loc,'ModerateEmailsGET_Amailcontentpart_id',$this->_view);
       }
       $this->_model->GET_Amailcontentpart($this->_loc,'ModerateEmailsGET_Amailcontentpart',$this->_view);
       print $this->_view->getform_end('update_Amailcontentpart','Amailcontentpart');
       $GETDEFAULT =false;
    }
    if(isset($_GET['addpartto'])){
       print $this->_view->getform_head($this->_loc,$this->_action);
       print $this->_view->get_template($this->_loc,'ModerateEmailsADD_addpartto',$this->_values);
       print $this->_view->getform_end('add','addcontentpartto');
       $GETDEFAULT =false;
    }
    if(isset($_GET['Amailcontentpartsummary'])){
       print $this->_view->getform_head($this->_loc,$this->_action);
       $this->_model->GET_AmailcontentpartsSummary($this->_loc,'ModerateEmailsSUM_Amailcontentpart_item',$this->_view,$this->_values['page'],$this->getAccound_id());//TODO check filters looks like "$enabled,$activated "
       print $this->_view->getform_end('update_Amailcontentpart','savecontentpart');
       $GETDEFAULT = false;
    }
    if(isset($_GET['Amaildataset'])){
       print $this->_view->getform_head($this->_loc,$this->_action);
       $this->_model->GET_Amaildataset($this->_loc,'ModerateEmailsGET_Amaildataset',$this->_view);
       print $this->_view->getform_end('update_Amaildataset','Amaildataset');
       $GETDEFAULT =false;
    }
    if(isset($_GET['newdataset'])){
       print $this->_view->getform_head($this->_loc,$this->_action);
       print $this->_view->get_template($this->_loc,'ModerateEmailsADD_Contentpartdataset',$this->_values);
       print $this->_view->getform_end('addDataset','addAmaildataset');
       $GETDEFAULT =false;
    }
    if(isset($_GET['Amailadminlog'])){
       print $this->_view->getform_head($this->_loc,$this->_action);
       $this->_model->GET_Amailadminlog($this->_loc,'ModerateEmailsGET_Amailadminlog',$this->_view);
       print $this->_view->getform_end('update_Amailadminlog','Amailadminlog');
       $GETDEFAULT =false;
    }
    if(isset($_GET['addAmailadminlog'])){
       print $this->_view->getform_head($this->_loc,$this->_action);
       print $this->_view->get_template($this->_loc,'ModerateEmailsADD_Amailadminlog',$this->_values);
       print $this->_view->getform_end('add_Amailadminlog','add');
       $GETDEFAULT =false;
    }
    if(isset($_GET['Amailadminlogsummary'])){
       print $this->_view->getform_head($this->_loc,$this->_action);
       $this->_model->GET_AmailadminlogsSummary($this->_loc,'ModerateEmailsSUM_Amailadminlog_item',$this->_view,$this->_values['page'],$this->getAccound_id());//TODO check filters looks like "$enabled,$activated "
       print $this->_view->getform_end('update_Amailadminlog','Amailadminlog');
       $GETDEFAULT = false;
    }
    if(isset($_GET['Amailemails'])){
       print $this->_view->getform_head($this->_loc,$this->_action);
       $this->_model->GET_Amailemails($this->_loc,'ModerateEmailsGET_Amailemails',$this->_view);
       print $this->_view->getform_end('update_Amailemails','Amailemails');
       $GETDEFAULT =false;
    }
    if(isset($_GET['addAmailemails'])){
       print $this->_view->getform_head($this->_loc,$this->_action);
       print $this->_view->get_template($this->_loc,'ModerateEmailsADD_Amailemails',$this->_values);
       print $this->_view->getform_end('add_Amailemails','add');
       $GETDEFAULT =false;
    }
    if(isset($_GET['Amailemailssummary'])){
       print $this->_view->getform_head($this->_loc,$this->_action);
       $this->_model->GET_AmailemailssSummary($this->_loc,'ModerateEmailsSUM_Amailemails_item',$this->_view,$this->_values['page'],$this->getAccound_id());//TODO check filters looks like "$enabled,$activated "
       print $this->_view->get_template('global','toolcheck_end',$this->_values);
       print $this->_view->getform_end('update_Amailemails','Amailemails');
       $GETDEFAULT = false;
    }
    if(isset($_GET['Mailcontenttopartsequence'])){
       print $this->_view->getform_head($this->_loc,$this->_action);
       $this->_model->GET_Mailcontenttopartsequence($this->_loc,'ModerateEmailsGET_Mailcontenttopartsequence',$this->_view);
       print $this->_view->getform_end('update_Mailcontenttopartsequence','Mailcontenttopartsequence');
       $GETDEFAULT =false;
    }
    if(isset($_GET['addMailcontenttopartsequence'])){
       print $this->_view->getform_head($this->_loc,$this->_action);
       print $this->_view->get_template($this->_loc,'ModerateEmailsADD_Mailcontenttopartsequence',$this->_values);
       print $this->_view->getform_end('add_Mailcontenttopartsequence','add');
       $GETDEFAULT =false;
    }
    if(isset($_GET['Mailcontenttopartsequencesummary'])){
       print $this->_view->getform_head($this->_loc,$this->_action);
       $this->_model->GET_MailcontenttopartsequencesSummary($this->_loc,'ModerateEmailsSUM_Mailcontenttopartsequence_item',$this->_view,$this->_values['page'],$this->getAccound_id());//TODO check filters looks like "$enabled,$activated "
       print $this->_view->getform_end('update_Mailcontenttopartsequence','Mailcontenttopartsequence');
       $GETDEFAULT = false;
    }
    //todo werk af
    $this->_validator->addMenu('menuselector',['summary','Mailcontenttopartsequencesummary','addMailcontenttopartsequence'],$_GET,$this->_values);
    if(isset($_GET['summary'])){
       $this->_model->GET_Summary($this->_loc,'ModerateEmailsSUM_item',$this->_view,$this->_values['page'],$this->getAccound_id());//TODO check filters looks like "$enabled,$activated "
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
    	$this->_values = $this->_view->get_translations($this->values);
		$this->_validator->addForm('addpart', ['id','contenttype','TYPE','content','amail_conceptmail_id'],
        false,['id','amail_conceptmail_id'], $this->_values);

		if(isset($_POST['addcontentpartto'])){
		    $this->_validator->addAlocat('addpart',['id'=>'isnumeric','contenttype'=>'istext','TYPE'=>'istext','amail_conceptmail_id'=>'isnumeric']);
		    $this->_validator->render('addpart',$_POST,$this->_values);     
		}
        $this->_validator->addForm('addimgcid', ['image','amail_contentpart_id'],false,['image'], $this->_values);
        $this->_validator->addAlocat('addimgcid',['image'=>'istext','amail_contentpart_id'=>'isnumeric']);
		if(isset($_POST['addimgcid'])){
		    $this->_validator->render('addimgcid',$_POST,$this->_values);     
		}
        $this->_validator->addForm('addamail_dataset_param',['id' ,'paramname' ,'TYPE' ,'value' ,],false,[],$this->_values);
        $this->_validator->addAlocat('addamail_dataset_param',['id' => 'int','paramname' => 'varchar','TYPE' => 'enum','value' => 'varchar',]);
        if(isset($_POST['addamail_dataset_param'])){
            $this->_validator->render('addamail_dataset_param',$_POST,$this->_values);
        }
//		$this->status('');conceptpart
		$this->_values['amail_conceptmail_id']=(isset($_GET['concept'])&&\autosystem\ValidateAs::isnumeric($_GET['concept']))?$_GET['concept']:0;
        $this->_values['amail_contentpart_id']=(isset($_GET['conceptpart'])&&\autosystem\ValidateAs::isnumeric($_GET['conceptpart']))?$_GET['conceptpart']:0;
        $this->_values['id']=(isset($_POST['id']))?$_POST['id']:'';
        $this->_values['name']=(isset($_POST['name']))?$_POST['name']:'';
        $this->_values['type_id']=(isset($_POST['type_id']))?$_POST['type_id']:'';
        $this->_values['mailadminlog_id']=(isset($_POST['mailadminlog_id']))?$_POST['mailadminlog_id']:'';
        $this->_values['menuindex']=(isset($_POST['menuindex']))?$_POST['menuindex']:'';
        $this->_values['getAmailcategory']=(isset($_POST['getAmailcategory']))? $_POST['getAmailcategory']:'';//TODO create a beter name for get
        if(isset($_POST['addAmailcategory'])){
           $this->_values['addAmailcategory']=$_POST['add_Amailcategory'];
           $this->_values['name']=(isset($_POST['name'])&&\autosystem\ValidateAs::isTEXT($_POST['name']))?$_POST['name']:'';
           $this->_values['type_id']=(isset($_POST['type_id'])&&\autosystem\ValidateAs::isnumeric($_POST['type_id']))?$_POST['type_id']:'';
           $this->_values['mailadminlog_id']=(isset($_POST['mailadminlog_id'])&&\autosystem\ValidateAs::isnumeric($_POST['mailadminlog_id']))?$_POST['mailadminlog_id']:'';
           $this->_values['menuindex']=(isset($_POST['menuindex'])&&\autosystem\ValidateAs::isnumeric($_POST['menuindex']))?$_POST['menuindex']:'';//TODO check
        }
        
        if(key_exists('deleteCategory',$_POST)){
           $this->_values['deleteCategory']=(isset($_POST['deleteCategory'])&&\autosystem\ValidateAs::isnumeric($_POST['deleteCategory']))?$_POST['deleteCategory']:'';//TODO check
        }
        if(key_exists('deleteCategory',$_GET)){
           $this->_values['deleteCategory']=(isset($_GET['deleteCategory'])&&\autosystem\ValidateAs::isnumeric($_GET['deleteCategory']))?$_GET['deleteCategory']:'';//TODO check
        }
        $this->_values['Amailcategorysummary']=(isset($_POST['Amailcategorysummary']))?$_POST['Amailcategorysummary']:'';
        /*$this->_values['id']=(isset($_POST['id']))?$_POST['id']:'';
        $this->_values['NAMEDKEY']=(isset($_POST['NAMEDKEY']))?$_POST['NAMEDKEY']:'';
        $this->_values['STATUS']=(isset($_POST['STATUS']))?$_POST['STATUS']:'';
        $this->_values['SENDIBLE']=(isset($_POST['SENDIBLE']))?$_POST['SENDIBLE']:'';
        $this->_values['amail_category_id']=(isset($_POST['amail_category_id']))?$_POST['amail_category_id']:'';
        $this->_values['mail_content_id']=(isset($_POST['mail_content_id']))?$_POST['mail_content_id']:'';
        $this->_values['imputdate']=(isset($_POST['imputdate']))?$_POST['imputdate']:'';*/
        $this->_values['getAmailconceptmail']=(isset($_POST['getAmailconceptmail']))? $_POST['getAmailconceptmail']:'';//TODO create a beter name for get
        
        if(isset($_GET['conceptmails'])||isset($_GET['addconceptmail'])||isset($_POST['addconceptmail'])){
            $this->conceptmailvars();
        }
        if(isset($_GET['contentpart'])|| isset($_GET['addcontentpart'])|| isset($_GET['addpartto'])|| isset($_GET['editconcept'])){
            $this->contentmailvars();
        }
        if(isset($_GET['partsfrom'])){
            $this->partsfromvars();
            return;
        }

        $this->_values['Amailconceptmailsummary']=(isset($_POST['Amailconceptmailsummary']))?$_POST['Amailconceptmailsummary']:'';
        $this->_values['key']=(isset($_POST['key']))?$_POST['key']:'';
        $this->_values['value']=(isset($_POST['value']))?$_POST['value']:'';
        $this->_values['getAmailstyle']=(isset($_POST['getAmailstyle']))? $_POST['getAmailstyle']:'';//TODO create a beter name for get
        if(isset($_POST['addAmailstyle'])){
           $this->_values['addAmailstyle']=$_POST['add_Amailstyle'];
           //$this->_values['amail_conceptmail_id']=(isset($_POST['amail_conceptmail_id'])&&\autosystem\ValidateAs::isnumeric($_POST['amail_conceptmail_id']))?$_POST['amail_conceptmail_id']:'';
           $this->_values['key']=(isset($_POST['key'])&&\autosystem\ValidateAs::isTEXT($_POST['key']))?$_POST['key']:'';
           $this->_values['value']=(isset($_POST['value'])&&\autosystem\ValidateAs::isTEXT($_POST['value']))?$_POST['value']:'';//TODO check
           //TODO input make other variables
        }else{
           
        }
        $this->_values['Amailstylesummary']=(isset($_POST['Amailstylesummary']))?$_POST['Amailstylesummary']:'';

        $this->_values['Amailcontentsummary']=(isset($_POST['Amailcontentsummary']))?$_POST['Amailcontentsummary']:'';
        $this->_values['id']=(isset($_POST['id']))?$_POST['id']:'';
        $this->_values['contenttype']=(isset($_POST['contenttype']))?$_POST['contenttype']:'';
        $this->_values['amail_dataset_id']=(isset($_POST['amail_dataset_id']))?$_POST['amail_dataset_id']:'';
        $this->_values['getAmailcontentpart']=(isset($_POST['getAmailcontentpart']))? $_POST['getAmailcontentpart']:'';//TODO create a beter name for get
        
        $this->_values['Amailcontentpartsummary']=(isset($_POST['Amailcontentpartsummary']))?$_POST['Amailcontentpartsummary']:'';
        $this->_values['id']=(isset($_POST['id']))?$_POST['id']:'';
         
        $this->_values['getAmaildataset']=(isset($_POST['getAmaildataset']))? $_POST['getAmaildataset']:'';//TODO create a beter name for get

        $this->_validator->addForm('adddataset',['selectconverters','query'],false,[],$this->_values);
		if(isset($_GET['newdataset'])){
		    $this->_validator->addAlocat('adddataset',[]);
		    $this->_validator->render('adddataset',$_POST,$this->_values);     
		}
        $this->_validator->addForm('adddataset',['selectconverters','query','id'],false,['id'],$this->_values);
		if(isset($_POST['addAmaildataset'])){
		    $this->_validator->addAlocat('adddataset',['id'=>'isnumeric']);
		    $this->_validator->render('adddataset',$_POST,$this->_values);     
		}
        $this->_values['Amaildatasetsummary']=(isset($_POST['Amaildatasetsummary']))?$_POST['Amaildatasetsummary']:'';
        $this->_values['id']=(isset($_POST['id']))?$_POST['id']:'';
        $this->_values['accound_id']=(isset($_POST['accound_id']))?$_POST['accound_id']:'';
        $this->_values['info']=(isset($_POST['info']))?$_POST['info']:'';
        $this->_values['imputdate']=(isset($_POST['imputdate']))?$_POST['imputdate']:'';
        $this->_values['getAmailadminlog']=(isset($_POST['getAmailadminlog']))? $_POST['getAmailadminlog']:'';//TODO create a beter name for get
        if(isset($_POST['addAmailadminlog'])){
           $this->_values['addAmailadminlog']=$_POST['add_Amailadminlog'];
           $this->_values['id']=(isset($_POST['id'])&&\autosystem\ValidateAs::isnumeric($_POST['id']))?$_POST['id']:'';
           $this->_values['accound_id']=(isset($_POST['accound_id'])&&\autosystem\ValidateAs::isnumeric($_POST['accound_id']))?$_POST['accound_id']:'';
           $this->_values['info']=(isset($_POST['info'])&&\autosystem\ValidateAs::isTEXT($_POST['info']))?$_POST['info']:'';//TODO check
           //TODO input make other variables
        }else{           
        }
        $this->_values['Amailadminlogsummary']=(isset($_POST['Amailadminlogsummary']))?$_POST['Amailadminlogsummary']:'';
        $this->_values['id']=(isset($_POST['id']))?$_POST['id']:'';
        $this->_values['issend']=(isset($_POST['issend']))?$_POST['issend']:'';
        $this->_values['SENDIBLE']=(isset($_POST['SENDIBLE']))?$_POST['SENDIBLE']:'';
        $this->_values['statuscomment']=(isset($_POST['statuscomment']))?$_POST['statuscomment']:'';
        $this->_values['conceptmail_id']=(isset($_POST['conceptmail_id']))?$_POST['conceptmail_id']:'';
        $this->_values['accound_id']=(isset($_POST['accound_id']))?$_POST['accound_id']:'';
        $this->_values['imputDate']=(isset($_POST['imputDate']))?$_POST['imputDate']:'';
        $this->_values['getAmailemails']=(isset($_POST['getAmailemails']))? $_POST['getAmailemails']:'';//TODO create a beter name for get
        if(isset($_POST['addAmailemails'])){
          $this->_values['addAmailemails']=$_POST['add_Amailemails'];
          $this->_values['id']=(isset($_POST['id'])&&\autosystem\ValidateAs::isnumeric($_POST['id']))?$_POST['id']:'';
          $this->_values['issend']=(isset($_POST['issend']))?$_POST['issend']:'';
          $this->_values['SENDIBLE']=(isset($_POST['SENDIBLE']))?$_POST['SENDIBLE']:'';
          $this->_values['statuscomment']=(isset($_POST['statuscomment'])&&\autosystem\ValidateAs::isTEXT($_POST['statuscomment']))?$_POST['statuscomment']:'';
          $this->_values['conceptmail_id']=(isset($_POST['conceptmail_id'])&&\autosystem\ValidateAs::isnumeric($_POST['conceptmail_id']))?$_POST['conceptmail_id']:'';
          $this->_values['accound_id']=(isset($_POST['accound_id'])&&\autosystem\ValidateAs::isnumeric($_POST['accound_id']))?$_POST['accound_id']:'';
          $this->_values['imputDate']=(isset($_POST['imputDate']))?$_POST['imputDate']:'';//TODO check
           //TODO input make other variables
        }else{           
        }
        $this->_values['Amailemailssummary']=(isset($_POST['Amailemailssummary']))?$_POST['Amailemailssummary']:'';
        $this->_values['amail_content_id']=(isset($_POST['amail_content_id']))?$_POST['amail_content_id']:'';
        $this->_values['seqranking']=(isset($_POST['seqranking']))?$_POST['seqranking']:'';
        $this->_values['getMailcontenttopartsequence']=(isset($_POST['getMailcontenttopartsequence']))? $_POST['getMailcontenttopartsequence']:'';//TODO create a beter name for get
        if(isset($_POST['addMailcontenttopartsequence'])){
           $this->_values['addMailcontenttopartsequence']=$_POST['add_Mailcontenttopartsequence'];
           $this->_values['amail_contentpart_id']=(isset($_POST['amail_contentpart_id'])&&\autosystem\ValidateAs::isnumeric($_POST['amail_contentpart_id']))?$_POST['amail_contentpart_id']:'';
           $this->_values['amail_content_id']=(isset($_POST['amail_content_id'])&&\autosystem\ValidateAs::isnumeric($_POST['amail_content_id']))?$_POST['amail_content_id']:'';
           $this->_values['seqranking']=(isset($_POST['seqranking'])&&\autosystem\ValidateAs::isnumeric($_POST['seqranking']))?$_POST['seqranking']:'';//TODO check
           //TODO input make other variables
        }else{
        }
        $this->_values['Mailcontenttopartsequencesummary']=(isset($_POST['Mailcontenttopartsequencesummary']))?$_POST['Mailcontenttopartsequencesummary']:'';
        $this->_values['summary']=(isset($_POST['summary']))?$_POST['summary']:'';
        
        $this->_values['contenttypes']=$this->_view->getOptions($this->_model->contenttypes,$this->_values['contenttype']);
  }
  private function conceptmailvars(){
    	$this->_validator->addForm('conceptmail',['id','NAMEDKEY','STATUS','SENDIBLE','amail_category_id','imputdate'],false,['id'],$this->_values);
    	$this->_validator->addForm('addcontent',['id','contenttype','TYPE','content'],false,['id'],$this->_values);
		if(isset($_POST['addconceptmail'])){
		    $this->_validator->addAlocat('conceptmail',['id'=>'isnumeric','NAMEDKEY'=>'istext','SENDIBLE'=>'isbool','amail_category_id'=>'isnumeric']);
		    $this->_validator->render('conceptmail',$_POST,$this->_values);
		    $this->_validator->addAlocat('addcontent',['id'=>'isnumeric','contenttype'=>'istext']);
		    $this->_validator->render('addcontent',$_POST,$this->_values);     
		}
	    $this->_values['issendible']='';
	    //$this->_values['status']='';
	    $this->_values['conceptid']='';
	    $this->categorys =$this->_model->GET_Categorys();

        if(isset($_GET['concept'])){
	        $this->_values['concept_id']=(isset($_GET['concept'])&&\autosystem\ValidateAs::isnumeric($_GET['concept']))?$_GET['concept']:'';
	    }
	    $this->_values['states']=$this->_view->getOptions($this->_model->states,$this->_values['status']);
	    $this->_values['categorys']=$this->_view->getOptions($this->categorys,$this->_values['amail_category_id']);
	    if(key_exists('deleteconceptmail',$_POST)){
	       $this->_values['deleteconceptmail']=(isset($_POST['deleteconceptmail'])&&\autosystem\ValidateAs::isnumeric($_POST['deleteconceptmail']))?$_POST['deleteconceptmail']:'';//TODO check
	    }
	    if(key_exists('deleteconceptmail',$_GET)){
	       $this->_values['deleteconceptmail']=(isset($_GET['deleteconceptmail'])&&\autosystem\ValidateAs::isnumeric($_GET['deleteconceptmail']))?$_GET['deleteconceptmail']:'';//TODO check
	    }
  }
  private function contentmailvars(){
    $this->_values['content']='';
    $this->_values['contenttypes']=$this->_view->getOptions($this->_model->contenttypes,$this->_values['contenttype']);
    $this->_values['enumtypes']=$this->_view->getOptions($this->_model->parttypes,$this->_values['TYPE']);
    $this->_values['datasets']=$this->_view->getOptions($this->_model->GETdatasets_arr(),$this->_values['dataset']);        
    //print_r($this->_values);
	/*if(isset($_POST['addcontentpart'])){
       $this->_values['addcontentpart']=$_POST['addcontentpart'];
       $this->_values['id']=(isset($_POST['id'])&&\autosystem\ValidateAs::isnumeric($_POST['id']))?$_POST['id']:'';
       $this->_values['content']=(isset($_POST['content'])&&\autosystem\ValidateAs::isTEXT($_POST['content']))?$_POST['content']:'';
	   $this->_values['contenttype']=(isset($_POST['contenttype'])&&\autosystem\ValidateAs::isTEXT($_POST['contenttype']))?$_POST['contenttype']:'';
	   $this->_values['datasetid']=(isset($_POST['datasetid'])&&\autosystem\ValidateAs::isnumeric($_POST['datasetid']))?$_POST['datasetid']:'';//TODO check
    }else{
    }*/
  }
  private function partsfromvars(){
        if(isset($_GET['concept'])){
	        $this->_values['concept_id']=(isset($_GET['concept'])&&\autosystem\ValidateAs::isnumeric($_GET['concept']))?$_GET['concept']:'';
	    }
  }
  private function checkItS(){
     if(!isset($_GET['check'])){return ;}
     $return = false;

     if(isset($_POST['sendible'])){           
        foreach($_POST['sendible'] as $id => $SENDIBLE){
            if(!empty($id)&& !empty($SENDIBLE)){
                if($this->_model->mod_ConceptMailSendible($id,$SENDIBLE)){       $return = true; }
            }
        }
      }
      return $return;
    }
    private function returnsupport($issaved,$action,$param=[]){
         if($issaved){  $param[]='success='.$action;
         }else{         $param[]='failed='.$action;
         }
         $this->GOESTO($this->_loc,$this->_action,$param);
    }
}
?>