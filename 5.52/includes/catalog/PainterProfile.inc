<?php
/**
* @author Lieven Roegiers
* @copyright 
* @CMS autosite 5.3
*/
namespace catalog;
class PainterProfile extends \autosystem\Controller{
     private $values = [];
     private $categorys;
      public function init(){
        $this->getModel();
        if($this->selectItS()){                return;}
        if($this->checkItS()){                 return;}
        $this->vars();
        if(count($_POST)>0){
       if(key_exists('init',$_POST)){
          $data = $this->_validator->getFormData('painterinit');
          $data['accound_id'] = $this->getAccound_id();
          $issaved=$this->_model->SAVE_catalog_painters($data);//['email'=>$data['...'],'accound_id'=>$this->getAccound_id()]
          $param[]='';
          $this->tosupportreturn($issaved,'init',$param);
       }
       if(key_exists('addpainting',$_POST) && $this->_validator->isValid('addpainting')){
          $data = $this->_validator->getFormData('addpainting');
          $issaved=$this->_model->SAVEPainting($data['name'],$data['group_id'],$data['shortcomment'],$data['detail'],$this->getAccound_id(),$data['PROMOTE'],$data['paintingsize']);//['email'=>$data['...'],'accound_id'=>$this->getAccound_id()]
          $param[]='show';
          $param[]='addpainting';
          $this->tosupportreturn($issaved,'show',$param);
       }
	   if(key_exists('addgroup',$_POST) && $this->_validator->isValid('addgroup')){
	          $data = $this->_validator->getFormData('addgroup');
	          $issaved=$this->_model->SAVE_catalog_product_groups($data);
	          $param[]='addgroup';
              $this->tosupportreturn($issaved,'show',$param);
	   }
       if(key_exists('savepainting',$_POST) && $this->_validator->isValid('savepainting')){
	          $data = $this->_validator->getFormData('savepainting');
              $data['keys']= ['id','accound_id'];
              $data['accound_id']= $this->getAccound_id();;
              $data['id']= $this->_values['painting'];
	          $issaved=$this->_model->CHANGE_catalog_product($data);
              $this->_model->DELETE_catalog_product_group(['product_id'=>$this->_values['painting']]);
              foreach($_POST['groupselection'] as $painting_id => $groups){
                foreach($groups as $group_id){
                    if( !empty($group_id)){
                        $return = $this->_model->INSERTNOID_catalog_product_group(['product_id'=>$this->_values['painting'],'group_id'=>$group_id]);
                        $return =true;
                    }
                 }

             }
             $parameters=$_POST['parameter'];
             foreach($parameters as $key => $value){
                $this->_model->CHANGE_catalog_product_parameter_value(['keys'=>['product_id','parameter_id'], 'product_id'=>$this->_values['painting'],'parameter_id'=>$key,'value'=>$value]);
                
             }
	          $param[]='savepainting';              
	   }      
      }
      $this->prepare();
   }
  public function prepare(){
    $GETDEFAULT = true;
    print $this->_view->getheader($this->_loc,$this->_action);
       $x['id'] = 'groups';
       $x['title'] = 'filter';
       print $this->_view->get_template('global','tooltip_start',[]);
       print $this->_view->get_template('global','section_head',$x);
       print $this->_view->get_template($this->_loc,$this->_action.'_menu',$this->_values);
       $this->_model->GET_groups($this->_loc,'Painting_group_item',$this->_view);
              //$this->_model->GET_groupInfo($this->_loc,'Painting_group_item',$this->_view,$this->getAccound_id(cound_id);    
       print $this->_view->get_template('global','section_foot',$x);
    $painter['accound_id']= $this->getAccound_id();
    if(!$this->_model->EXIST_catalog_painters($painter)){
	   print $this->_view->getform_head($this->_loc,$this->_action);
       print $this->_view->get_template($this->_loc,'init',$this->_values);
       print $this->_view->getform_end('init','init');
       $GETDEFAULT =false;
	}
    if(isset($_GET['init'])){
       print $this->_view->getform_head($this->_loc,$this->_action);
       $this->_model->GET_catalog_painters($this->_loc,'PainterProfileGET_painter',$this->_view);
       print $this->_view->getform_end('init','init');
       $GETDEFAULT =false;
    }
    if(isset($_GET['addpainting'])){
       $x['id'] = 'paintsummary';
       $x['title'] = '';
	   print $this->_view->get_template('global','section_head',$x);
       print $this->_view->getform_head($this->_loc,$this->_action);
       print $this->_view->get_template($this->_loc,'addpainting',$this->_values);
	   print $this->_view->getform_end('addpainting','addpainting');
       print $this->_view->get_template('global','section_foot',$x);
       $GETDEFAULT = false;
    }
    if(isset($_GET['painting'])){
       $x['id'] = 'paintsummary';
       $x['title'] = '';
       $paintarray = [];
	   print $this->_view->get_template('global','section_head',$x);//$location,$templatename,$view,$id,$accound_id
       print $this->_view->getform_head($this->_loc,$this->_action);
       if(key_exists('edit',$_GET)){
            $this->_model->GET_MyPainting($this->_loc,'editpainting',$this->_view,$this->_values['painting'],$this->getAccound_id());
       }else{
            print('<center>');
            $this->_model->GET_paintingE($this->_loc,'Painting_info',
            $this->_view,$this->_values['painting'],$this->getAccound_id());
            print('</center>');
       }
	   print $this->_view->getform_end('savepainting','savepainting');
       print $this->_view->get_template('global','section_foot',$x);
       $GETDEFAULT = false;
    }
    if(isset($_GET['addgroup'])){
       $x['id'] = 'paintsummary';
       $x['title'] = '';
	   print $this->_view->get_template('global','section_head',$x);
       print $this->_view->getform_head($this->_loc,$this->_action);
       print $this->_view->get_template($this->_loc,'paintingaddgroup',$this->_values);
	   print $this->_view->getform_end('addgroup','addgroup');
       print $this->_view->get_template('global','section_foot',$x);
       $GETDEFAULT = false;
    }
    if(isset($_GET['groups'])){
       $x['id'] = 'paintsummary';
       $x['title'] = '';
	   print $this->_view->get_template('global','section_head',$x);
       print $this->_view->getform_head($this->_loc,$this->_action);
         $this->_model->GET_Groups($this->_loc,'painterprofilegroup_item',$this->_view,$this->getAccound_id(),$this->_values['page']);
       print $this->_view->getform_end('addgroup','addgroup');
       print $this->_view->get_template('global','section_foot',$x);
       $GETDEFAULT = false;
    }
    if(isset($_GET['nofoto'])){
       $x['id'] = 'paintsummary';
       $x['title'] = '';
	   print $this->_view->get_template('global','section_head',$x);
       print $this->_view->getform_head($this->_loc,$this->_action);
        $this->_model->GET_NofotoSummary($this->_loc,'user_Painting_item',$this->_view,$this->getAccound_id(),$this->_values['page']);
       print $this->_view->get_template('global','toolcheck_end',$this->_values);
       print $this->_view->getform_end('addgroup','addgroup');
       print $this->_view->get_template('global','section_foot',$x);
       $GETDEFAULT = false;
    }
    if(isset($_GET['promoted'])){
       $x['id'] = 'paintsummary';
       $x['title'] = '';
	   print $this->_view->get_template('global','section_head',$x);
       $this->_model->GET_userPromoSummary($this->_loc,'user_Painting_item',$this->_view,$this->getAccound_id());
       print $this->_view->get_template('global','section_foot',$x);
       print $this->_view->get_template('global','toolcheck_end',$this->_values);
       $GETDEFAULT = false;
    }
    if($this->_validator->isValid('isfilter')){
        print 'x';
          if($this->_values['validparameters']){
        	   $x['id'] = 'paintsummary';
               $x['title'] = '';
        	   print $this->_view->get_template('global','section_head',$x);
               print $this->_view->get_template('global','nofilter',$x);
               //$_GET['parameter']
               //$location,$templatename,$view,$account_id,$group,$incparams,$excparams,$account_id=0
                $this->_model->GET_FilterSummary($this->_loc,'Public_Painting_item',$this->_view,$this->_values['group'],$_GET['parameter'],[],$this->_values['page'],$this->getAccound_id());
               //$this->_model->GET_PaintingsSummary($this->_loc,'Public_Painting_item',$this->_view,$this->getAccound_id(),'',$this->_values['page'],$this->_values['group']);
               print $this->_view->get_template('global','section_foot',$x);
           }else{ 
               /*print $this->_view->get_template($this->_loc,'promos_head',[]);
               $this->_model->GET_promoSummary($this->_loc,'Painting_promo_item',$this->_view,$this->getAccound_id());
               print $this->_view->get_template($this->_loc,'promos_foot',[]);
               */
               //print('<hr>');
        	   $x['id'] = 'paintsummary';
               $x['title'] = '';
        	   print $this->_view->get_template('global','section_head',$x);
               $this->_model->GET_PaintingsSummary($this->_loc,'Public_Painting_item',$this->_view,$this->getAccound_id(),'',$this->_values['page'],$this->_values['group']);
               print $this->_view->get_template('global','section_foot',$x);
           }
            $GETDEFAULT = false;
    }
    if($GETDEFAULT){
	   $x['id'] = 'paintsummary';
       $x['title'] = '';
	   print $this->_view->get_template('global','section_head',$x);
       $this->_model->GET_PainterProfilesSummary($this->_loc,'user_Painting_item',$this->_view,$this->getAccound_id(),$this->_values['page']);
       print $this->_view->get_template('global','section_foot',$x);
       print $this->_view->get_template('global','toolselect_end',$this->_values);       
       print $this->_view->get_template('global','toolcheck_end',$this->_values);
       $GETDEFAULT = false;
    }
    print $this->_view->getfooter($this->_action);
  }
  private function vars(){//
        $x= ['key' =>'id','value' => 'name'];
        $groups = $this->_model->ALIST_catalog_product_groups($x);
        $x= ['key' =>'id','value' => 'name'];
        $sizes = $this->_model->getSizes($x);
  		$this->addTranslationValues($groups);
        
        $this->_values = $this->getTranslations();
        $this->_view->setENUM_TRANSLATIONS($this->_values);
        //$this->_values['sizes']=$this->_model->getSizes();
        $this->_validator->addMenu('get',['painting'],$_GET,$this->_values);
        $this->_validator->addAlocat('get',['id'=>'isnumeric']);
        if($this->_validator->isChecked('get')){
            $this->_validator->render('get',$_GET,$this->_values);
        }
        
  		$this->_values['page']=(isset($_GET['page']))?$_GET['page']:'0';  
  		$this->_values['size']='';
  		$this->_values['sizes']=$this->_view->getOptions($sizes,$this->_values['size']);
       $this->_validator->addForm('painterinit',['id','accound_id','publicname','biografic','stars'],false,['accound_id','id','stars'],$this->_values);
        if(isset($_POST['init'])){
            $this->_validator->addAlocat('painterinit',['id'=>'isnumeric','publicname'=>'istext','accound_id'=>'isnumeric','stars'=>'isnumeric',]);
            $this->_validator->render('painterinit',$_POST,$this->_values);
        }
        $this->_values['getCatalogpaintingfrompainters']=(isset($_POST['getCatalogpaintingfrompainters']))? $_POST['getCatalogpaintingfrompainters']:'';//TODO create a beter name for get
        $this->_validator->addForm('addpainting',['name','group_id','paintingsize','shortcomment','detail','backgroundwal_id','PROMOTE'],false,['group_id','shortcomment','detail','paintingsize','backgroundwal_id'],$this->_values);
        if(isset($_POST['addpainting'])){
            $this->_validator->addAlocat('addpainting',['name'=>'istext','shortcomment'=>'istext','detail'=>'istext','group_id'=>'isnumeric','paintingsize'=>'istext','PROMOTE'=>'isbool','backgroundwal_id'=>'isnumeric']);
            $this->_validator->render('addpainting',$_POST,$this->_values);
        }
        $this->_validator->addForm('addgroup',['id','name'],false,['id'],$this->_values);
        $this->_validator->addAlocat('addgroup',['name'=>'istext']);
        if(isset($_POST['addgroup'])){
             $this->_validator->render('addgroup',$_POST,$this->_values);
        } //$this->status('test');
        $this->_values['groups']=$this->_view->getOptions($groups,$this->_values['group_id']);
        
        $this->_validator->addForm('savepainting',['name','shortcomment','detail'],false,['shortcomment','detail'],$this->_values);
        $this->_validator->addAlocat('savepainting',['name'=>'istext','shortcomment'=>'istext','detail'=>'istext']);
        if(isset($_POST['savepainting'])){
            $this->_validator->render('savepainting',$_POST,$this->_values);
        }
        $this->_validator->addForm('isfilter',['group'],false,['group'],$this->_values);
        //$this->_validator->addSelect('isfilter','parameter',$groups);
        $this->_validator->addAlocat('isfilter',['group'=>'isnumeric']);
        if(isset($_GET['filter'])){
             $this->_validator->render('isfilter',$_GET,$this->_values);
        }
        $this->_values['validparameters']=false;
        if(key_exists('parameter',$_GET)&& is_array($_GET['parameter'])){
            $this->_values['validparameters'] = true;
            foreach($_GET['parameter']  as $id => $param){
                if(!is_numeric($id)||!is_string($param)){
                    $this->_values['validparameters'] = true;
                }
            }
        } 
        //$this->status('');
  }
  private function selectItS(){
     if(!isset($_GET['select'])){return ;}
     $return = false;
     if(isset($_POST['groupselection'])&&!isset($_POST['savepainting'])){
            foreach($_POST['groupselection'] as $painting_id => $group_id){
                //print "Message resived : groupselection painting $painting_id merge with $group_id";
                if(!empty($painting_id)&& !empty($group_id)){
                    print "Message resived : groupselection painting $painting_id merge with $group_id";
                     $return = $this->_model->CROIXCHANGE_catalog_product_group(['product_id'=>$painting_id,'group_id'=>$group_id]);
                     $return =true;
                }
            }
      }
      return $return;
  }
  private function checkItS(){
     if(!isset($_GET['check'])){return ;}
     $return = false;
       if(isset($_POST['VISIBLE'])){           
            foreach($_POST['VISIBLE'] as $id => $VISIBLE){
                if(!empty($id)&& !empty($VISIBLE)){
                	$data['keys']= ['product_id','painter_id'];
                    $data['painter_id']= $this->_model->GETID_catalog_painters(['accound_id'=>$this->getAccound_id()]);
                	$data['product_id']= $id;
                	$data['VISIBLE']= ($VISIBLE === 'true')?'TRUE':'FALSE';
                	$this->_model->CHANGE_catalog_paintingfrompainters($data);
                }
            }
        }
       if(isset($_POST['PROMOTE'])){           
            foreach($_POST['PROMOTE'] as $id => $PROMOTE){
                if(!empty($id)&& !empty($PROMOTE)){
                	$data['keys']= ['id'];     	$data['id']= $id;
                	$data['PROMOTE']= ($PROMOTE === 'true')?'TRUE':'FALSE';
                	$this->_model->CHANGE_catalog_product($data);
                }
            }
        }
        if(isset($_POST['ENABLED'])){           
            foreach($_POST['ENABLED'] as $id => $ENABLED){
                if(!empty($id)&& !empty($ENABLED)){
                	$data['keys']= ['id'];        $data['id']= $id;
                	$data['ENABLED']= ($ENABLED === 'true')?'TRUE':'FALSE';
                	$this->_model->CHANGE_catalog_product($data);
                }
            }
        }
    }
}
?>