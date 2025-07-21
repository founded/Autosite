<?php
/**
* @author Lieven Roegiers
* @copyright 
* @CMS autosite
*/
namespace images;
class IMGByUser  extends \autosystem\Controller{
    private $values = ['unconstruct','images.category.title'];
    private $categorys;
    private $limitrights;
    public function init() {
       $this->getModel();
       $this->loadSettings();// TODO NEED TO SEE
       $this->vars();
       if(isset($_POST['bindto'])){
            foreach($_POST['bindto'] as $key => $value){
                //print $key.$value;
                if($value == 'true'){
                   if(\autosystem\ValidateAs::isTEXT($_GET['bind'])&& \autosystem\ValidateAs::isnumeric($_GET['nr']) && \autosystem\ValidateAs::isnumeric($key)){
                       return $this->_model->BIND_pathimg($_GET['bind'],$_GET['nr'],$this->getAccound_id(),$key);
                   }
                }else{
                   if(\autosystem\ValidateAs::isTEXT($_GET['bind'])&& \autosystem\ValidateAs::isnumeric($_GET['nr']) && \autosystem\ValidateAs::isnumeric($key)){
                       return $this->_model->UNBIND_pathimg($_GET['bind'],$_GET['nr'],$this->getAccound_id(),$key);
                   }
                }
            }
            die('www');
       }
       if(isset($_POST['addImage'])){
            $uploader = new \autosystem\Upload();
            $fields = ['image'];
            $trueExtentions = ['jpg','gif','png'];
            $netH = \autosystem\Key::GET(6);            
            if(is_dir('./layout/ups/'.$this->_model->GET_Title($this->_values['images_category']).'/')){
                $uploader->load($_FILES,$fields,$trueExtentions,'./layout/ups/'.$this->_model->GET_Title($this->_values['images_category']).'/',$this->getAccound_id().'_'.$netH);
                $upslinks = $uploader->getlinks();
                $issaved = false;
                foreach($upslinks as $category => $value){
                    foreach($value as $size => $link){
                        $link = substr($link,1);
                        $ext = '';
                        if($this->_model->save_asPathimage($this->_values['images_name'],$link,$this->_values['images_category'],$this->getAccound_id(),$ext)){
                            $issaved = true;
                        }else{
                           $this->setInConsole(false,'saved','not.saved');
                        }
                    }
                }
                if(is_array($this->_settings)&& $this->_values['marking']=='TRUE'){
                	//Array ( [0] => 387 [1] => 387 [2] => 2 [3] => width="387" height="387" [bits] => 8 [channels] => 3 [mime] => image/jpeg ) 
					$aimage = new \images\AImage_Transform();
                    $aimage->setCopyright($this->_settings['COPYRIGHT']);//.$_SERVER['SERVER_NAME']);
                    $aimage->setLogo($this->_settings['LOGO']);
                    foreach($upslinks as $category => $value){
                    	foreach($value as $size => $uplink){
                            $aimage->settxtcolor();
                            $Darr = explode('/',$uplink);                            
                            //print_r($Darr);
                            $name = array_pop($Darr);
                            $aimage->setFont($this->_settings['FONT']);                                        
                            $aimage->transformImage($uplink,$this->GetProfilePath('images',true).$name,true,implode('/',$Darr).'/thumb_'.$name);
						}
					}					
				}else{ $this->messageto('no settingsfile or marking so no action');	}
				if($this->_values['createcard']=='TRUE'){
                	//Array ( [0] => 387 [1] => 387 [2] => 2 [3] => width="387" height="387" [bits] => 8 [channels] => 3 [mime] => image/jpeg ) 
					$aimage = new \images\AImage_Transform_card();
                    foreach($upslinks as $category => $value){
                    	foreach($value as $size => $uplink){
                            $Darr = explode('/',$uplink);
                            $name = array_pop($Darr);
                            $aimage->setText(['','Ho Ho Ho','SANTERS','HAPPY','NEW YEAR','*** LIEVEN R ***']) ;                                     
                            $aimage->RenderCard($uplink,$this->GetProfilePath('images',true).$name);
						}
					}					
				}else{ $this->messageto('no createcard so no action');	}
                /*if($issaved){
                     $this->_values['title']=$this->_view->get_AtranslationB('addImage');
                     $this->redirect('/'.$this->_loc.'/'.$this->_action.'/?success=true');
                     die();
                }else{
                     $this->redirect('/'.$this->_loc.'/'.$this->_action.'/?failed=true&add');
                }*/
            }
       }

       if(isset($_GET['new'])){
            $this->_values['title']=$this->_view->get_AtranslationB('uploadimage');
            $this->prepare($this->_loc,'gallery.new',$this->_values);
            return;
       }
       if(isset($_GET['delete'])){
            if(\autosystem\ValidateAs::isnumeric($_GET['delete'])){
                $this->_values['title']=$this->_view->get_AtranslationB('uploadimage');
                $isdeleted=$this->_model->delete_Pathimage($_GET['delete'],$this->_values['category'],$this->getAccound_id());
                $param[]='MySummary';
                if($this->_values['category']){$param[]='category='.$this->_values['category'];}
                $this->tosupportreturn($isdeleted,'delete',$param);
            }
       }
       $this->prepare($this->_loc,$this->_action,$this->_values);
    }
    private function copyimage($source,$destiny){
		$aimage = new \autosystem\AImage();
		$aimage->aCOPY($source,$destiny);
	}
    private function prepare($loc,$action,$values,$find =null){
       print $this->_view->getheader($loc,$action);
       print $this->_view->get_template($this->_loc,$this->_action,$this->_values);
       $template ='';
       if(isset($find)){
            print $this->_view->get_template($this->_loc,'findresult',$this->_values);
       }
       $GETEVERYTHINGELSE = true;
       if(isset($_GET['success'])){
            print $this->_view->get_template($this->_loc,'imgbyuser.uploadsuccess',$this->_values);
       }
       if(isset($_GET['failed'])){
            print $this->_view->get_template($this->_loc,'imgbyuser.uploadfailed',$this->_values);
       }
       if(isset($_GET['add'])){
            print $this->_view->getform_head($this->_loc,$action);
            print $this->_view->get_template($this->_loc,'add',$this->_values);
            print $this->_view->getform_end('','addImage');
            $GETEVERYTHINGELSE = false;
       }
      if($this->_user->hasRulle('USER')){
            print $this->_view->get_template($this->_loc,'kingmenu',$this->_values);
            if(key_exists( 'scan',$_GET)){
                $scanner =  new \data\FileScanner();
                $path = './layout/ups/paint_catalog';
        		$arr['inc']=['.jpg','.gif'] ;
        		$scanner->setfilter($arr);
                
        		$scanner->scan2extfiles($path);
        		$files = $scanner->getfils();
                foreach($files as $filename){
                    if(substr($filename,0, 6)=='thumb_' ){
                        print 'XREJECTED->'.$filename.'<br>';
                    }else{
                        print '<-SELECTEDX'.$filename.'<br>';
                    }
                }
                if(is_array($this->_settings)&& key_exists( 'marking',$_GET)){
                    print 'x';
                	//Array ( [0] => 387 [1] => 387 [2] => 2 [3] => width="387" height="387" [bits] => 8 [channels] => 3 [mime] => image/jpeg ) 
					$aimage = new \images\AImage_Transform();
                    $aimage->setCopyright($this->_settings['COPYRIGHT']);//.$_SERVER['SERVER_NAME']);
                    $aimage->setLogo($this->_settings['LOGO']);
                    foreach($files as $filename){
                        $aimage->settxtcolor();
                        $aimage->setFont($this->_settings['FONT']);                                        
                        $aimage->transformImage($path.'/'.$filename,$this->GetProfilePath('images',true).$filename,true,$path.'/thumb_'.$filename);
					}					
				}
                $GETEVERYTHINGELSE = false;
            }
            if(key_exists( 'scannotthump',$_GET)){
                $scanner =  new \data\FileScanner();
                $path = './layout/ups/paint_catalog';
        		$arr['inc']=['.jpg','.gif'] ;
        		$scanner->setfilter($arr);
        		$scanner->scan2extfiles($path);
        		$files = $scanner->getfils();
                foreach($files as $filename){
                    //print $filename.'<br>';
                }
                if(is_array($this->_settings)&& key_exists( 'marking',$_GET)){
                	//Array ( [0] => 387 [1] => 387 [2] => 2 [3] => width="387" height="387" [bits] => 8 [channels] => 3 [mime] => image/jpeg ) 
					/*$aimage = new \images\AImage_Transform();
                    $aimage->setCopyright($this->_settings['COPYRIGHT']);//.$_SERVER['SERVER_NAME']);
                    $aimage->setLogo($this->_settings['LOGO']);
                    */
                    foreach($files as $filename){
                        print $filename;
                        //$aimage->settxtcolor();
                        //$aimage->setFont($this->_settings['FONT']);                                        
                        //$aimage->transformImage($path.'/'.$filename,$this->GetProfilePath('images',true).$filename,true,$path.'/thumb_'.$filename);
					}					
				}
                $GETEVERYTHINGELSE = false;
            }
      }
       if(key_exists('fontimage',$_GET)){        
           if(key_exists('useroriginals',$_GET)){
            
           }               
       }       
       if(isset($_GET['presentation'])){
            $array['id'] = 'detail';
            $array['title'] = $this->_values['unconstruct'];
            print $this->_view->get_template('global','section_head',$array);
               $svgcollection = new SVGcollection(960,1200);
               $path = $this->_view->getinstallpath();
               $images=$this->_model->GET_PathImages();
               print $svgcollection->fetch_FromLeftTop($images,$path,150,150);
            print $this->_view->get_template('global','section_foot');
            $GETEVERYTHINGELSE = false;
       }
       if(isset($_GET['bind'])){//%path.install->value%/images/IMGByUser/?bind=auction&to=%id->value%
            $array['id'] = 'addimage';//
            $array['title'] = $this->_view->get_AtranslationB('add');
            print $this->_view->get_template('global','section_head',$array);
            print $this->_view->getform_head($this->_loc,$action);
            print $this->_view->get_template($this->_loc,'add',$this->_values);
            print $this->_view->getform_end('','addImage');
            print $this->_view->get_template('global','section_foot');
            $array['bind']=(\autosystem\ValidateAs::isTEXT($_GET['bind']))?$_GET['bind']:'';
            $array['nr']=(\autosystem\ValidateAs::isnumeric($_GET['nr']))?$_GET['nr']:'';
            $array['id'] = 'images';//
            $array['title'] = 'BIND';
            print $this->_view->get_template('global','tooltip_start',$array);
            print $this->_view->get_template('global','section_head',$array);
            $this->_model->GET_Images_OverViewToBind($this->_loc,'ImageBind',$this->_view,$array['bind'],$this->getAccound_id(),$array['nr']);
            print $this->_view->get_template('global','section_foot');
            print $this->_view->get_template('images','sender',$array);
            $GETEVERYTHINGELSE = false;
       }
       if($GETEVERYTHINGELSE){
           $array['id'] = 'category';
           $array['title'] = $this->_values['images.category.title'];
           $array['layoutpath']= $this->_view->getimgPath();
           print $this->_view->get_template('global','section_head',$array);
           $array['title'] = '';
           $array['bullet'] = $this->_view->getimgPath().'bubble.svg';
               print $this->_view->get_template($this->_loc,'CategoryItem_head',$array);
                foreach($this->categorys as $key => $value){
                    $array['id']=$key;
                    $array['label']=$this->_values[$value];
                    print $this->_view->get_template($this->_loc,'CategoryItem',$array);
                }
                print $this->_view->get_template($this->_loc,'CategoryItem_foot');
           print $this->_view->get_template('global','section_foot');
           $array['id'] = 'detail';
           $array['title'] = $this->_values['unconstruct'];
           print $this->_view->get_template('global','section_head',$array);
           $this->_model->GET_ImagesPath_OverView($this->_loc,'Item',$this->_view,$this->_values['category'],$this->getAccound_id());  
           print $this->_view->get_template('global','section_foot');
       }
       print $this->_view->getfooter($action);
    }
    private function vars(){
        $this->categorys = $this->_model->GET_Categorys($this->getAccound_id());
        $this->addTranslationValues($this->values);
        $this->addTranslationValues($this->categorys);
        $this->addTranslationKeys($this->_model->acceses);
        $enum_translations=[];
        $enum_translations+=$this->categorys;
        $enum_translations+=$this->_model->acceses;
        $this->_values = $this->getTranslations();
        if(!isset($_GET['bind'])){
            $this->_view->setENUM_TRANSLATIONS($this->_values);
        }
        if(isset($_GET['page'])){             $this->_values['page']             = $_GET['page'];            }
        
        $this->_values['category']='';
        $this->_values['access']='';
        $this->_values['images_name']='';
        $this->_values['collection_id']=(isset($_GET['collection_id']))?$_GET['collection_id']:'';//;
        if(key_exists('category',$_GET)){
            $this->_values['category'] =$_GET['category'];
        }
        $categorys=[];
        if(isset($_POST['find'])){
                $this->_values['search']=$_POST['search'];       
        }else{
            if(isset($_GET['bind'])){
                $key = array_keys($categorys);
                $this->_values['category']= $_GET['bind'];
            }
        }
		$this->_validator->addForm('addImage',['gallery_access','images_category','images_name','marking','createcard'],false,['marking','createcard'],$this->_values);
		foreach($this->categorys as $key => $value){
            $categorys[$key]=$this->_values[$value];
        }
        
        $this->_validator->addSelect('addImage','images_category',$categorys);
        $this->_validator->addSelect('addImage','gallery_access',$this->_model->acceses);
        if(isset($_POST['addImage'])){//addImage
            $this->_validator->addAlocat('addImage',['images_name'=>'istext','marking'=>'isbool','createcard'=>'isbool']);
            $this->_validator->render('addImage',$_POST,$this->_values);
        }
        //$this->status('');
        $this->_values['images_gallerys' ]= $this->_validator->getSelect('addImage','images_category',$this->_view,$this->_values['images_category']);
        $this->_values['images_accesses']= $this->_validator->getSelect('addImage','gallery_access',$this->_view,$this->_values['gallery_access']);
		//$this->status('');
        /*	if(count($_POST)>0){
                $this->_values['title']='data:';
                $this->_values['access']= (key_exists($_POST['gallery_access'],$this->_model->acceses))?$_POST['gallery_access']:'';
                $this->_values['category']=(key_exists($_POST['images_category'],$this->categorys))?$_POST['images_category']:'';
                $this->_values['images_name']=(\autosystem\ValidateAs::isTEXT($_POST['images_name']))?$_POST['images_name']:'';//
            }
        }*/
        //$this->_values['images_gallerys']= $this->_view->getOptions($categorys,$this->_values['category']);
        //$this->_values['images_accesses']= $this->_view->getOptions($this->_model->acceses,$this->_values['access']);
    }
    public function messageto($msg,$print = true){
		if($print){// to fast remove prints to check whats wrong on a hosting
			print $msg;	
		}
		//TODO LOG
	}
}
?>