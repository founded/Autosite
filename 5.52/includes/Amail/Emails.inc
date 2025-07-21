<?php
/**
* @author Lieven Roegiers
* @copyright 
* @CMS autosite 5.30
*/
namespace Amail;
class Emails extends \autosystem\Controller{
     private $values = [];
     private $categorys;
      public function init(){
        $this->getModel();
        $this->vars();
        if(count($_POST)>0){
       if(key_exists('addAmailconceptmail',$_POST)){
          $data = $this->_validator->getFormData('addAmailconceptmail');
          //$data['accound_id'] = $this->getAccound_id();
          $issaved=$this->_model->SAVE_amail_conceptmail($data);//['email'=>$data['...'],'accound_id'=>$this->getAccound_id()]
          $param[]='addAmailconceptmail';
          $this->tosupportreturn($issaved,'addAmailconceptmail',$param);
       }
       if(key_exists('addAmailstyle',$_POST)){
          $data = $this->_validator->getFormData('addAmailstyle');
          //$data['accound_id'] = $this->getAccound_id();
          $issaved=$this->_model->SAVE_amail_style($data);//['email'=>$data['...'],'accound_id'=>$this->getAccound_id()]
          $param[]='addAmailstyle';
          $this->tosupportreturn($issaved,'addAmailstyle',$param);
       }
       if(key_exists('addAmailcontentpart',$_POST)){
          $data = $this->_validator->getFormData('addAmailcontentpart');
          //$data['accound_id'] = $this->getAccound_id();
          $issaved=$this->_model->SAVE_amail_contentpart($data);//['email'=>$data['...'],'accound_id'=>$this->getAccound_id()]
          $param[]='addAmailcontentpart';
          $this->tosupportreturn($issaved,'addAmailcontentpart',$param);
       }
       if(key_exists('addAmaildataset',$_POST)){
          $data = $this->_validator->getFormData('addAmaildataset');
          //$data['accound_id'] = $this->getAccound_id();
          $issaved=$this->_model->SAVE_amail_dataset($data);//['email'=>$data['...'],'accound_id'=>$this->getAccound_id()]
          $param[]='addAmaildataset';
          $this->tosupportreturn($issaved,'addAmaildataset',$param);
       }
       if(key_exists('addAmailadminlog',$_POST)){
          $data = $this->_validator->getFormData('addAmailadminlog');
          //$data['accound_id'] = $this->getAccound_id();
          $issaved=$this->_model->SAVE_amail_adminlog($data);//['email'=>$data['...'],'accound_id'=>$this->getAccound_id()]
          $param[]='addAmailadminlog';
          $this->tosupportreturn($issaved,'addAmailadminlog',$param);
       }
       if(key_exists('addAmailcontenttopartsequence',$_POST)){
          $data = $this->_validator->getFormData('addAmailcontenttopartsequence');
          //$data['accound_id'] = $this->getAccound_id();
          $issaved=$this->_model->SAVE_amail_contenttopart_sequence($data);//['email'=>$data['...'],'accound_id'=>$this->getAccound_id()]
          $param[]='addAmailcontenttopartsequence';
          $this->tosupportreturn($issaved,'addAmailcontenttopartsequence',$param);
       }
       if(key_exists('addAmailcontentpartdatasetparameter',$_POST)){
          $data = $this->_validator->getFormData('addAmailcontentpartdatasetparameter');
          //$data['accound_id'] = $this->getAccound_id();
          $issaved=$this->_model->SAVE_amail_contentpart_dataset_parameter($data);//['email'=>$data['...'],'accound_id'=>$this->getAccound_id()]
          $param[]='addAmailcontentpartdatasetparameter';
          $this->tosupportreturn($issaved,'addAmailcontentpartdatasetparameter',$param);
       }
        //}<---count
      }
      $this->prepare();
   }
  public function prepare(){
    
    $GETDEFAULT = true;
    print $this->_view->getheader($this->_loc,$this->_action);
    if(isset($_GET['getAmailconceptmail'])){
       print $this->_view->getform_head($this->_loc,$this->_action);
       $this->_model->GET_amail_conceptmail($this->_loc,'EmailsGET_Amailconceptmail',$this->_view);
       print $this->_view->getform_end('update_Amailconceptmail','Amailconceptmail');
       $GETDEFAULT =false;
    }
    if(isset($_GET['addAmailconceptmail'])){
       print $this->_view->getform_head($this->_loc,$this->_action);
       print $this->_view->get_template($this->_loc,'EmailsADD_Amailconceptmail',$this->_values);
       print $this->_view->getform_end('add_Amailconceptmail','add');
       $GETDEFAULT =false;
    }
    if(isset($_GET['Amailconceptmailsummary'])){
       print $this->_view->getform_head($this->_loc,$this->_action);
       $this->_model->GET_AmailconceptmailsSummary($this->_loc,'EmailsSUM_Amailconceptmail_item',$this->_view,$this->_values['page'],$this->getAccound_id());
       print $this->_view->get_template('global','toolcheck_end',$this->_values);
       print $this->_view->getform_end('update_Amailconceptmail','Amailconceptmail');
       $GETDEFAULT = false;
    }
    if(isset($_GET['getAmailstyle'])){
       print $this->_view->getform_head($this->_loc,$this->_action);
       $this->_model->GET_amail_style($this->_loc,'EmailsGET_Amailstyle',$this->_view);
       print $this->_view->getform_end('update_Amailstyle','Amailstyle');
       $GETDEFAULT =false;
    }
    if(isset($_GET['addAmailstyle'])){
       print $this->_view->getform_head($this->_loc,$this->_action);
       print $this->_view->get_template($this->_loc,'EmailsADD_Amailstyle',$this->_values);
       print $this->_view->getform_end('add_Amailstyle','add');
       $GETDEFAULT =false;
    }
    if(isset($_GET['Amailstylesummary'])){
       print $this->_view->getform_head($this->_loc,$this->_action);
       $this->_model->GET_AmailstylesSummary($this->_loc,'EmailsSUM_Amailstyle_item',$this->_view,$this->_values['page'],$this->getAccound_id());
       print $this->_view->getform_end('update_Amailstyle','Amailstyle');
       $GETDEFAULT = false;
    }
    if(isset($_GET['getAmailcontentpart'])){
       print $this->_view->getform_head($this->_loc,$this->_action);
       $this->_model->GET_amail_contentpart($this->_loc,'EmailsGET_Amailcontentpart',$this->_view);
       print $this->_view->getform_end('update_Amailcontentpart','Amailcontentpart');
       $GETDEFAULT =false;
    }
    if(isset($_GET['addAmailcontentpart'])){
       print $this->_view->getform_head($this->_loc,$this->_action);
       print $this->_view->get_template($this->_loc,'EmailsADD_Amailcontentpart',$this->_values);
       print $this->_view->getform_end('add_Amailcontentpart','add');
       $GETDEFAULT =false;
    }
    if(isset($_GET['Amailcontentpartsummary'])){
       print $this->_view->getform_head($this->_loc,$this->_action);
       $this->_model->GET_AmailcontentpartsSummary($this->_loc,'EmailsSUM_Amailcontentpart_item',$this->_view,$this->_values['page'],$this->getAccound_id());
       print $this->_view->getform_end('update_Amailcontentpart','Amailcontentpart');
       $GETDEFAULT = false;
    }
    if(isset($_GET['getAmaildataset'])){
       print $this->_view->getform_head($this->_loc,$this->_action);
       $this->_model->GET_amail_dataset($this->_loc,'EmailsGET_Amaildataset',$this->_view);
       print $this->_view->getform_end('update_Amaildataset','Amaildataset');
       $GETDEFAULT =false;
    }
    if(isset($_GET['addAmaildataset'])){
       print $this->_view->getform_head($this->_loc,$this->_action);
       print $this->_view->get_template($this->_loc,'EmailsADD_Amaildataset',$this->_values);
       print $this->_view->getform_end('add_Amaildataset','add');
       $GETDEFAULT =false;
    }
    if(isset($_GET['Amaildatasetsummary'])){
       print $this->_view->getform_head($this->_loc,$this->_action);
       $this->_model->GET_AmaildatasetsSummary($this->_loc,'EmailsSUM_Amaildataset_item',$this->_view,$this->_values['page'],$this->getAccound_id());
       print $this->_view->getform_end('update_Amaildataset','Amaildataset');
       $GETDEFAULT = false;
    }
    if(isset($_GET['getAmailadminlog'])){
       print $this->_view->getform_head($this->_loc,$this->_action);
       $this->_model->GET_amail_adminlog($this->_loc,'EmailsGET_Amailadminlog',$this->_view);
       print $this->_view->getform_end('update_Amailadminlog','Amailadminlog');
       $GETDEFAULT =false;
    }
    if(isset($_GET['addAmailadminlog'])){
       print $this->_view->getform_head($this->_loc,$this->_action);
       print $this->_view->get_template($this->_loc,'EmailsADD_Amailadminlog',$this->_values);
       print $this->_view->getform_end('add_Amailadminlog','add');
       $GETDEFAULT =false;
    }
    if(isset($_GET['Amailadminlogsummary'])){
       print $this->_view->getform_head($this->_loc,$this->_action);
       $this->_model->GET_AmailadminlogsSummary($this->_loc,'EmailsSUM_Amailadminlog_item',$this->_view,$this->_values['page'],$this->getAccound_id());
       print $this->_view->getform_end('update_Amailadminlog','Amailadminlog');
       $GETDEFAULT = false;
    }
    if(isset($_GET['getAmailcontenttopartsequence'])){
       print $this->_view->getform_head($this->_loc,$this->_action);
       $this->_model->GET_amail_contenttopart_sequence($this->_loc,'EmailsGET_Amailcontenttopartsequence',$this->_view);
       print $this->_view->getform_end('update_Amailcontenttopartsequence','Amailcontenttopartsequence');
       $GETDEFAULT =false;
    }
    if(isset($_GET['addAmailcontenttopartsequence'])){
       print $this->_view->getform_head($this->_loc,$this->_action);
       print $this->_view->get_template($this->_loc,'EmailsADD_Amailcontenttopartsequence',$this->_values);
       print $this->_view->getform_end('add_Amailcontenttopartsequence','add');
       $GETDEFAULT =false;
    }
    if(isset($_GET['Amailcontenttopartsequencesummary'])){
       print $this->_view->getform_head($this->_loc,$this->_action);
       $this->_model->GET_AmailcontenttopartsequencesSummary($this->_loc,'EmailsSUM_Amailcontenttopartsequence_item',$this->_view,$this->_values['page'],$this->getAccound_id());
       print $this->_view->getform_end('update_Amailcontenttopartsequence','Amailcontenttopartsequence');
       $GETDEFAULT = false;
    }
    if(isset($_GET['getAmailcontentpartdatasetparameter'])){
       print $this->_view->getform_head($this->_loc,$this->_action);
       $this->_model->GET_amail_contentpart_dataset_parameter($this->_loc,'EmailsGET_Amailcontentpartdatasetparameter',$this->_view);
       print $this->_view->getform_end('update_Amailcontentpartdatasetparameter','Amailcontentpartdatasetparameter');
       $GETDEFAULT =false;
    }
    if(isset($_GET['addAmailcontentpartdatasetparameter'])){
       print $this->_view->getform_head($this->_loc,$this->_action);
       print $this->_view->get_template($this->_loc,'EmailsADD_Amailcontentpartdatasetparameter',$this->_values);
       print $this->_view->getform_end('add_Amailcontentpartdatasetparameter','add');
       $GETDEFAULT =false;
    }
    if(isset($_GET['Amailcontentpartdatasetparametersummary'])){
       print $this->_view->getform_head($this->_loc,$this->_action);
       $this->_model->GET_AmailcontentpartdatasetparametersSummary($this->_loc,'EmailsSUM_Amailcontentpartdatasetparameter_item',$this->_view,$this->_values['page'],$this->getAccound_id());
       print $this->_view->getform_end('update_Amailcontentpartdatasetparameter','Amailcontentpartdatasetparameter');
       $GETDEFAULT = false;
    }
    if(isset($_GET['summary'])){
       $this->_model->GET_Summary($this->_loc,'EmailsSUM_item',$this->_view,$this->_values['page'],$this->getAccound_id());//TODO check filters looks like "$enabled,$activated "
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
  private function vars(){//
//OPTIONS $tableArrayfalse

        $this->_values['imputdate']=(isset($_POST['imputdate']))?$_POST['imputdate']:'';
//OPTIONS $tableArrayfalse

        $this->_values['getAmailconceptmail']=(isset($_POST['getAmailconceptmail']))? $_POST['getAmailconceptmail']:'';//TODO create a beter name for get
        $this->_validator->addForm('addAmailconceptmail',['id','NAMEDKEY','STATUS','SENDIBLE','amail_category_id','imputdate',],false,[''],$this->_values);
        if(isset($_POST['addAmailconceptmail'])){
            $this->_validator->addAlocat('addAmailconceptmail',['id'=>'isnumeric','NAMEDKEY'=>'istext','SENDIBLE'=>'isbool','amail_category_id'=>'isnumeric',]);
            $this->_validator->render('addAmailconceptmail',$_POST,$this->_values);
           //TODO check generated code
        }else{
           
        }
        $this->_values['Amailconceptmailsummary']=(isset($_POST['Amailconceptmailsummary']))?$_POST['Amailconceptmailsummary']:'';
//OPTIONS $tableArrayfalse

        $this->_values['getAmailstyle']=(isset($_POST['getAmailstyle']))? $_POST['getAmailstyle']:'';//TODO create a beter name for get
        $this->_validator->addForm('addAmailstyle',['amail_conceptmail_id','key','value',],false,[''],$this->_values);
        if(isset($_POST['addAmailstyle'])){
            $this->_validator->addAlocat('addAmailstyle',['amail_conceptmail_id'=>'isnumeric','key'=>'istext','value'=>'istext',]);
            $this->_validator->render('addAmailstyle',$_POST,$this->_values);
           //TODO check generated code
        }else{
           
        }
        $this->_values['Amailstylesummary']=(isset($_POST['Amailstylesummary']))?$_POST['Amailstylesummary']:'';
//OPTIONS $tableArrayfalse

        $this->_values['getAmailcontentpart']=(isset($_POST['getAmailcontentpart']))? $_POST['getAmailcontentpart']:'';//TODO create a beter name for get
        $this->_validator->addForm('addAmailcontentpart',['id','contenttype','TYPE','content',],false,[''],$this->_values);
        if(isset($_POST['addAmailcontentpart'])){
            $this->_validator->addAlocat('addAmailcontentpart',['id'=>'isnumeric','contenttype'=>'istext',]);
            $this->_validator->render('addAmailcontentpart',$_POST,$this->_values);
           //TODO check generated code
        }else{
           
        }
        $this->_values['Amailcontentpartsummary']=(isset($_POST['Amailcontentpartsummary']))?$_POST['Amailcontentpartsummary']:'';
//OPTIONS $tableArrayfalse

        $this->_values['getAmaildataset']=(isset($_POST['getAmaildataset']))? $_POST['getAmaildataset']:'';//TODO create a beter name for get
        $this->_validator->addForm('addAmaildataset',['id','name','selectconverters','query',],false,[''],$this->_values);
        if(isset($_POST['addAmaildataset'])){
            $this->_validator->addAlocat('addAmaildataset',['id'=>'isnumeric','name'=>'istext','selectconverters'=>'istext','query'=>'istext',]);
            $this->_validator->render('addAmaildataset',$_POST,$this->_values);
           //TODO check generated code
        }else{
           
        }
        $this->_values['Amaildatasetsummary']=(isset($_POST['Amaildatasetsummary']))?$_POST['Amaildatasetsummary']:'';
        $this->_values['imputtime']=(isset($_POST['imputtime']))?$_POST['imputtime']:'';
//OPTIONS $tableArrayfalse

        $this->_values['getAmailadminlog']=(isset($_POST['getAmailadminlog']))? $_POST['getAmailadminlog']:'';//TODO create a beter name for get
        $this->_validator->addForm('addAmailadminlog',['id','accound_id','info','imputtime',],false,[''],$this->_values);
        if(isset($_POST['addAmailadminlog'])){
            $this->_validator->addAlocat('addAmailadminlog',['id'=>'isnumeric','accound_id'=>'isnumeric','info'=>'istext',]);
            $this->_validator->render('addAmailadminlog',$_POST,$this->_values);
           //TODO check generated code
        }else{
           
        }
        $this->_values['Amailadminlogsummary']=(isset($_POST['Amailadminlogsummary']))?$_POST['Amailadminlogsummary']:'';
//OPTIONS $tableArrayfalse

        $this->_values['getAmailcontenttopartsequence']=(isset($_POST['getAmailcontenttopartsequence']))? $_POST['getAmailcontenttopartsequence']:'';//TODO create a beter name for get
        $this->_validator->addForm('addAmailcontenttopartsequence',['amail_conceptmail_id','amail_contentpart_id','seqranking','STATUS','type',],false,[''],$this->_values);
        if(isset($_POST['addAmailcontenttopartsequence'])){
            $this->_validator->addAlocat('addAmailcontenttopartsequence',['amail_conceptmail_id'=>'isnumeric','amail_contentpart_id'=>'isnumeric','seqranking'=>'isnumeric',]);
            $this->_validator->render('addAmailcontenttopartsequence',$_POST,$this->_values);
           //TODO check generated code
        }else{
           
        }
        $this->_values['Amailcontenttopartsequencesummary']=(isset($_POST['Amailcontenttopartsequencesummary']))?$_POST['Amailcontenttopartsequencesummary']:'';
        $this->_values['imputtime']=(isset($_POST['imputtime']))?$_POST['imputtime']:'';
//OPTIONS $tableArrayfalse

        $this->_values['getAmailcontentpartdatasetparameter']=(isset($_POST['getAmailcontentpartdatasetparameter']))? $_POST['getAmailcontentpartdatasetparameter']:'';//TODO create a beter name for get
        $this->_validator->addForm('addAmailcontentpartdatasetparameter',['id','amail_dataset_id','amail_parameter_id','amail_contentpart_id','seqranking','imputtime',],false,[''],$this->_values);
        if(isset($_POST['addAmailcontentpartdatasetparameter'])){
            $this->_validator->addAlocat('addAmailcontentpartdatasetparameter',['id'=>'isnumeric','amail_dataset_id'=>'isnumeric','amail_parameter_id'=>'isnumeric','seqranking'=>'isnumeric',]);
            $this->_validator->render('addAmailcontentpartdatasetparameter',$_POST,$this->_values);
           //TODO check generated code
        }else{
           
        }
        $this->_values['Amailcontentpartdatasetparametersummary']=(isset($_POST['Amailcontentpartdatasetparametersummary']))?$_POST['Amailcontentpartdatasetparametersummary']:'';
//OPTIONS $tableArrayfalse

        $this->_values['summary']=(isset($_POST['summary']))?$_POST['summary']:'';
  }
  private function checkItS(){
     if(!isset($_GET['check'])){return ;}
     $return = false;
       if(isset($_POST['SENDIBLE'])){           
                foreach($_POST['SENDIBLE'] as $id => $SENDIBLE){
                    if(!empty($id)&& !empty($SENDIBLE)){
                    	$data['keys']= ['id'];
                    	$data['id']= $id;
                    	$data['SENDIBLE']= ($SENDIBLE === 'true')?'TRUE':'FALSE';
                    	$this->_model->CHANGE_amail_conceptmail($data);
                        //TODO CHECK CODE
                    }
                }
           }
    }
}
?>