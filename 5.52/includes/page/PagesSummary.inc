<?php
namespace page;
class PagesSummary extends \autosystem\Controller{
    private $values = array('search','find');
    public function init(){
           $this->getModel();
           //print_r($this->_values);
           if(isset($_GET['check'])){
                $cansee = $_POST['visibility'];
                if(is_array($cansee)){//$accound_id,$item_id,$bool
                    //print_r($cansee);
                    foreach($cansee as $key=>$value){
                        $this->_model->setVisibility($this->getAccound_id(),$key,$value);
                    }
                    $result = ['ok'=>true];
                    header('HTTP/1.0 200 ');print json_encode($result);
                    return;
                }
                header('HTTP/1.0 504 Your To Slow for me');
                return ;
           }
           $this->vars();
           if(isset($this->_values['delete'])){
                $this->_model->delete($this->_values['delete']);
           }
           if(count($_POST)>0){
            $locale = str_replace('locale.','',$this->_values['page_langlocale']);//
                if(key_exists('add',$_POST)&&$this->canSave()&& $id = parent::getAccound_id()){
                        $issaved=$this->_model->save($this->_values['title'],
                        $this->_values['shortcomment'],$this->_values['content'],$id,
                        $this->_values['page_category'],$this->_values['page_status'],$this->_values['page_access'],
                        $locale
                        );
                     $param[]='add';
                     if($issaved){
                        $this->tosupportreturn($issaved,'summary',$param);
                     }
					                        
                     
                }
                //print_r($_POST);
                if(key_exists('edit',$_POST)&&$id = parent::getAccound_id()){
                    $key = $this->_model->getTitlekey((isset($this->_values['translate']))?$this->_values['translate'] :$this->_values['id'] );
                    //print $key;
                    if($this->_model->existPage($key,$this->_values['page_langlocale'])){
                        $this->_model->update($this->_values['id'],$this->_values['page_status'],$this->_values['title'],
                        $this->_values['page_access'],$this->_values['shortcomment'],$this->_values['content']);
                    }else{
                        $issaved=$this->_model->save($this->_values['title'],
                        $this->_values['shortcomment'],
                        $this->_values['content'],
                        $id,
                        $this->_values['page_category'],
                        $this->_values['page_status'],
                        $this->_values['page_access'],
                        $this->_values['translatetolang'],
                        $key);
                    }
                   $this->GOESTO($this->_loc,$this->_action);
                }              
           }
           $this->prepare($this->_loc,$this->_action,$this->_values,$this->_values['search']);
    }
    private function vars(){
        $locales = $this->_model->GetLocales();
        $categorys = $this->_model->GET_Categorys();
        if(count($categorys)==0){
            $this->categorys = $this->_model->GET_Categorys(0);
        }
        array_push($this->values,'page.category.title');
        array_push($this->values,'page.info.title');
        $this->addTranslationValues($this->values);
        $this->addTranslationValues($categorys);
        //$this->addTranslationValues($locales);
        $this->addTranslationKeys($locales);
        //$this->addTranslationKeys($this->_model->states);
        $this->addTranslationKeys($this->_model->modeltypes);
        $this->_values = $this->getTranslations();
            //print_r($this->_values);
        $this->_view->setENUM_TRANSLATIONS($this->_values);
        $this->_values['search']='';
        
        
		$this->_validator->addForm('addPage',['title','shortcomment','page_category','content','page_status','page_access'],false,[],$this->_values);
	  	if(isset($_POST['add'])){
	  		//$this->_validator->setDebug(true);
	  		$this->_validator->addAlocat('addPage',['title'=>'istext','shortcomment'=>'istext']);
	  		$this->_validator->render('addPage',$_POST,$this->_values);
		}
		
		$this->_validator->addForm('addTranslation',['title','shortcomment','page_category','content','page_status','page_access','translatetolang'],false,[],$this->_values);
	  	if(isset($_POST['addTranslation'])){
	  		//$this->_validator->setDebug(true);
	  		$this->_validator->addAlocat('addTranslation',['title'=>'istext','shortcomment'=>'istext']);
	  		$this->_validator->render('addTranslation',$_POST,$this->_values);
		}
		$this->_validator->addForm('editPage',['id', 'title','shortcomment','content','page_status','page_access'],false,[],$this->_values);
	  	if(isset($_POST['edit'])){
	  		//$this->_validator->setDebug(true);
	  		$this->_validator->addAlocat('editPage',['title'=>'istext','shortcomment'=>'istext']);
	  		$arr['id'] = $_GET['edit'] ;
	  		$this->_validator->render('editPage',array_merge_recursive($arr,$_POST),$this->_values);
		}
        //$this->status($this->_view);

        
        $this->_values['delete']=(isset($_GET['delete'])&& is_numeric($_GET['delete']))?$_GET['delete']:null;
        $this->_values['view']=(isset($_GET['view'])&& is_numeric($_GET['view']))?$_GET['view']:null;
        $this->_values['edit']=(isset($_GET['edit'])&& is_numeric($_GET['edit']))?$_GET['edit']:null;
        $this->_values['translate']=(isset($_GET['translate'])&& is_numeric($_GET['translate']))?$_GET['translate']:null;
        
        $this->_values['id']=(key_exists('edit',$_POST)&& is_numeric($_POST['edit']))?$_POST['edit']:'';
        $this->_values['translatetolang']=(isset($_POST['translatetolang'])&&$_POST['translatetolang']!='%locale->value%')? str_replace('locale.','',$_POST['translatetolang']):null;
        $this->_values['page_status']=(isset($_POST['page_status'])&&\autosystem\ValidateAs::isTEXT($_POST['page_status']))?$_POST['page_status']:'';
        $this->_values['page_states']=$this->_view->getOptions($this->_model->states,$this->_values['page_status']);
        $this->_values['title']=(isset($_POST['title'])&&\autosystem\ValidateAs::isTEXT($_POST['title']))?$_POST['title']:'';
        $this->_values['content']=(isset($_POST['content'])&&\autosystem\ValidateAs::isFULLTEXT($_POST['content']))?$_POST['content']:'';
        $this->_values['page_category']=(isset($_POST['page_category'])&&\autosystem\ValidateAs::isTEXT($_POST['page_category']))?$_POST['page_category']:'';
        $this->_values['page_categorys']=$this->_view->getOptions($categorys,$this->_values['page_category']);
        $this->_values['page_langlocale']=(isset($_POST['page_langlocale'])&&\autosystem\ValidateAs::isTEXT($_POST['page_langlocale']))?$_POST['page_langlocale']:'';
        $this->_values['page_langlocales']=$this->_view->getOptions($locales,$this->_values['page_langlocale']);
        $this->_values['page_access']=(isset($_POST['page_access'])&&\autosystem\ValidateAs::isFULLTEXT($_POST['page_access']))?$_POST['page_access']:'';       
        $this->_values['page_accesses']=$this->_view->getOptions($this->_model->modeltypes,$this->_values['page_access']);
        $this->_values['shortcomment']=(isset($_POST['shortcomment'])&&\autosystem\ValidateAs::isFULLTEXT($_POST['shortcomment']))?$_POST['shortcomment']:'';
    }
    private function prepare($loc,$action,$values,$find =null){
      $key = $this->_model->getTitlekey($this->_values['translate']);
      if($this->_model->existPage($key,$this->_values['translatetolang'])){
           //so view page
           //$this->redirect("./$this->_loc/$this->_action/?edit=".$this->_values['translate']);
      }  
      print $this->_view->getheader($loc,$action);
      print $this->_view->get_template($loc,$action,$this->_values);
            $GETSOMETHING = true;  
            if(isset($this->_values['view'])){
                $this->_model->GETPage($this->_loc,'page',$this->_view,$this->_values['view'],$this->getAccound_id());
                $GETSOMETHING = false;
            }
            if(isset($this->_values['translate'])){
                print '<div class="col-lg-6 col-md-8 col-sm-8" >';
                $this->_model->GETPage($this->_loc,'page',$this->_view,$this->_values['translate'],$this->getAccound_id());
                print '</div>';
                print '<div class="col-lg-6 col-md-8 col-sm-8" >';
                    if(isset($this->_values['translate'])){
                        if(isset($this->_values['translatetolang'])){//translatetolang
                            $a = $this->_model->GETTranslateArray($this->_values['translate'],$this->getAccound_id(),$this->_values['translatetolang']);
                            $c['page_langlocale']=$this->_values['translatetolang'];
                            $c = $a+$this->_values;
                            $c['page_accesses']=$this->_view->getOptions($this->_model->modeltypes,$a['access']);
                            print $this->_view->get_template($this->_loc,'edit',$c);
                        }else{
                            print $this->_view->getform_head($this->_loc,$this->_action);
                            print $this->_view->get_template($this->_loc,'pre_translate',$this->_values);
                            print $this->_view->getform_end('go','go');
                        }
                    }
                print '</div>';
                $GETSOMETHING = false;
            }
            if(isset($_GET['add'])){
                print $this->_view->getform_head($this->_loc,$this->_action);
                print $this->_view->get_template($this->_loc,'add',$this->_values);
                print $this->_view->getform_end('','add');
                $GETSOMETHING = false;
           }
           if(isset($this->_values['edit'])){
                $a = $this->_model->GETPageArray($this->_values['edit']);
                $c = $a+$this->_values;
                $c['page_accesses']=$this->_view->getOptions($this->_model->modeltypes,$a['access']);
                $images = ['1'=>'%path.layout->value%img/icons/c/code.svg','2'=>'%path.layout->value%img/icons/c/code.svg'];
                $images = $this->_model->GET_PageImages();
                $c['editorimages']='';
                foreach($images as $k =>$v){
                    $e['id'] = $k;
                    $e['fname']= $v;
                    $e['imagealt']= ''; 
                    $c['editorimages'].=$this->_view->get_template($this->_loc,'editorimage',$e);
                }
                print $this->_view->get_template($this->_loc,'edit',$c);
                $GETSOMETHING = false;
           }
           if($GETSOMETHING){
                $accoundid = null;
                $enabled = null;
                $this->_model->GET_APageSummary($this->_loc,'pages_item',$this->_view,$accoundid,$enabled);
                print $this->_view->get_template('global','toolcheck_end',$this->_values);
           }     
        print $this->_view->getfooter($action);
    }
    private function viewItem($name,$path,$location){
    }
    private function canSave(){
        return true;
    }
}
?>