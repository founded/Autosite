<?php
	namespace images;
    class GalleryManage extends \autosystem\Controller{
        private $values = ['unconstruct'];
        public function init() {
           $this->getModel();
           $this->vars();
           if(isset($_POST['add'])){
                if($this->_model->save($this->_values['gallery_categoryname'],$this->_values['gallery_categorylabel'],$this->_values['gallery_type'],$this->getAccound_id())){
                    $this->redirect('/'.$this->_loc.'/'.$this->_action.'/');
                }else{
                    
                }
                $this->prepare($this->_loc,$this->_action,$this->_values);
                return;
           }
           if(isset($_GET['new'])){
                $this->_values['title']=$this->_view->get_AtranslationB('uploadimage');
                $this->prepare($this->_loc,'gallery.new',$this->_values);
                return;
           }//
           /*if(isset($_GET['ManageCategorys'])){
                $this->_values['title']=$this->_view->get_AtranslationB("uploadimage");
                $this->prepare($this->_loc,"ManageCategorys",$this->_values);
                return;
           }¨*/
           $this->prepare($this->_loc,$this->_action,$this->_values);
        }
        private function prepare($loc,$action,$values,$find =null){
           print $this->_view->getheader($loc,$action);
           print $this->_view->get_template($this->_loc,$this->_action,$this->_values);
          
           $template ='';
           if(isset($find)){
                print $this->_view->get_template($this->_loc,'findresult',$this->_values);
           }else{
            
           }
           $GETEVERYTHINGELSE = true;
           if(isset($_GET['setCollectionsSizes'])){
                print $this->_view->getform_head($this->_loc,$action);
                print $this->_view->get_template('global','unconstruct',$this->_values);
                print $this->_view->getform_end('','save');
                $GETEVERYTHINGELSE = false;
           }
           if(isset($_GET['addCategory'])){
                print $this->_view->getform_head($this->_loc,$action);
                print $this->_view->get_template($this->_loc,'addCategory',$this->_values);
                print $this->_view->getform_end('','add');
                $GETEVERYTHINGELSE = false;
           }
           if(isset($_GET['ManageCategorys'])){
                print $this->_view->getform_head($this->_loc,$action);
                //print $this->_view->get_template($this->_loc,'add',$this->_values);
                print $this->_view->getform_end('');
                $GETEVERYTHINGELSE = false;
           }
           if(isset($_GET['Icons'])){
            	$FileScanner = new \data\FileScanner();//data\FileScanner
                $exts['inc']= ['.svg'];
            	$FileScanner->setfilter($exts);
            	$FileScanner->scandir('./layout/img/icons');
                $dirs = $FileScanner->getdirs();
                foreach($dirs as $key){
                	$array['id'] = 'info';
                    $array['key'] = $key;
                    print $this->_view->get_template($this->_loc,'Path_item',$array);
                }
                if($this->_values['Icons']=='all'){
                    foreach($dirs as $dir){
                    	$FileScanner->scandir('./layout/img/icons/'.$dir);
                        $files = $FileScanner->getfils();
                    	foreach($files as $file){
                            $array['urlto'] = './layout/img/icons/'.$dir.$file;
                            $array['key'] = $key;
                            print $this->_view->get_template($this->_loc,'Path_iconItem',$array);
                        }
                    }
                }else{   
                    if(isset($this->_values['Icons'])&&is_dir('./layout/img/icons/'.$this->_values['Icons'])){
                        $FileScanner->scandir('./layout/img/icons/'.$this->_values['Icons']);
                        $files = $FileScanner->getfils();
                    	foreach($files as $file){
                            $array['urlto'] = 'img/icons/'.$this->_values['Icons'].'/'.$file;
                            $array['namekey'] = $file;
                            print $this->_view->get_template($this->_loc,'Path_iconItem',$array);
                        }
                    }
                }
           }
           if($GETEVERYTHINGELSE){
                   $array['id'] = 'info';
                   $array['title'] = $this->_values['unconstruct'];
                   print $this->_view->get_template('global','section_head',$array);
                   print $this->_view->get_template('global','section_foot');
               
           }
           print $this->_view->getfooter($action);
        }
        private function vars(){
            $this->_values = $this->_view->get_translations($this->values);
            $this->_values['gallery_categoryname']='';
            $this->_values['gallery_categorylabel']='';
            $this->_values['gallery_type']='';
            if(isset($_POST['find'])){
        
            }else{
                if(count($_POST)>0){
                    //print_r($_POST);
                    $this->_values['gallery_categoryname']= $_POST['gallery_categoryname'];
                    $this->_values['gallery_categorylabel']= $_POST['gallery_categorylabel'];
                    $this->_values['gallery_type']= $_POST['gallery_type'];
                }
            }
            $types = $this->_model->modeltypes;
            $this->_values['gallery_types']= $this->_view->getOptions($types,$this->_values['gallery_type']);
            if(isset($_GET['Icons'])){
                $this->_values['Icons'] = (\autosystem\ValidateAs::isTEXT($_GET['Icons']))? $_GET['Icons']:'';
            }
            
        }
        
    }
?>