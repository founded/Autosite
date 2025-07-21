<?php
namespace template;
class TemplateSummary extends \autosystem\Controller{
    private $values = [];
    public function init(){
       $this->getModel();
       $this->vars();
       if(isset($_POST['edit'])&&$this->_validator->isValid('edittemplate')){
            $this->_model->create($this->_values['name'],$this->_values['location'],$this->_values['content']);
       }
       if(isset($_GET['preparepackout'])){           
            $FileScanner = new \data\FileScanner();
            $exts['inc']= ['.Qtemplate'];
        	$FileScanner->setfilter($exts);
            $path = 'layout/views/'.$this->_values['template_path'];
            $FileScanner->scandir('./layout/views/'.$this->_values['template_path']);
            $files = $FileScanner->getfils();
            
        	foreach($files as $file){
        	    if(substr($file, 0, 1) != '.'){
                    print '[\''.implode('\',\'',explode('/',$path)).'\',\''.$file.'\'],';
        	    }
            }
            return;
       }
       $this->prepare($this->_loc,$this->_action,$this->_values,$this->_values['search']);
    }
    private function vars(){
        $categorys = $this->_model->GET_Categorys();
        if(count($categorys)==0){
            $this->categorys = $this->_model->GET_Categorys(0);
        }
        $this->addTranslationValues($this->values);
        $this->addTranslationValues($categorys);
        $this->addTranslationKeys($this->_model->modeltypes);
        $this->_values = $this->getTranslations();
        $this->_values['search']='';
        $this->_values['Templates']='';
        $this->_values['name']='';
        if(isset($_POST['find'])){
        }else{
            if(isset($_GET['template_path'])){
                $this->_values['template_path']=(isset($_GET['template_path']))?$_GET['template_path']:'';
            }
            $this->_validator->addForm('createtemplate',['name','location','content'],false,['name','location','content'],$this->_values);
            $this->_validator->addAlocat('createtemplate',['name'=>'istext','location'=>'istext','content'=>'istext']);
            if(isset($_GET['create'])){
                 $this->_validator->render('createtemplate',$_GET,$this->_values);
            }
            $this->_validator->addForm('edittemplate',['name','location','content'],false,['name','location','content'],$this->_values);
            $this->_validator->addAlocat('edittemplate',['name'=>'istext','location'=>'istext']);
            if(isset($_POST['edit'])){
                 $this->_validator->render('edittemplate',$_POST,$this->_values);
            }
            if(isset($_GET['edit'])){
                 $this->_validator->render('edittemplate',$_GET,$this->_values);
            }
            if(isset($_GET['previewTemplatecache'])){
                $this->_values['previewTemplatecache']='';
            }
            if(isset($_POST['RemoveOldcache'])){
                $this->_values['name']=(\autosystem\ValidateAs::isTEXT($_POST['name']))?$_POST['name']:null;
                $this->_values['location']=(\autosystem\ValidateAs::isTEXT($_POST['location']))?$_POST['location']:null;
                $this->_values['RemoveOldcache']='';
            }
        }
        //$this->status('',false,false,true);
    }
    private function prepare($loc,$action,$values,$find =null){
      print $this->_view->getheader($loc,$action);
      print $this->_view->get_template($loc,$action,$this->_values);
      print 'BUG FIXED sorry from developers';
            $GETSOMETHING = true;  
           if(isset($_GET['templates'])){
            	$FileScanner = new \data\FileScanner();
                $dirs = $this->_model->GET_Categorys();
                foreach($dirs as $key){
                	$array['id'] = 'info';
                    $array['key'] = $key;
                    print $this->_view->get_template($this->_loc,'Path_item',$array);
                }
                if($this->_values['Templates']=='all'){
                    foreach($dirs as $dir){
                    	$FileScanner->scandir('./layout/img/icons/'.$dir);
                        $files = $FileScanner->getfils();
                    	foreach($files as $file){
                            $array['urlto'] = './layout/img/icons/'.$dir.$file;
                            $array['key'] = $key;
                            print $this->_view->get_template($this->_loc,'Path_iconItem',$array);
                        }
                    }
                }
                $GETSOMETHING = false;
           }
           if(isset($this->_values['template_path']) && is_dir('./layout/views/'.$this->_values['template_path'])){
                $FileScanner = new \data\FileScanner();
                $exts['inc']= ['.Qtemplate'];
            	$FileScanner->setfilter($exts);
                $FileScanner->scandir('./layout/views/'.$this->_values['template_path']);
                $files = $FileScanner->getfils();
                $list['id'] = 'category';
                $list['title'] = './layout/views/'.$this->_values['template_path'];
                print $this->_view->get_template('global','section_head',$list);
            	foreach($files as $file){
            	    if(substr($file, 0, 1) != '.'){
            	        $pos = stripos($file, '.');
            	        $name = substr($file, 0, $pos);
                        $array['urlto'] = $name;
                        $array['namekey'] = $name;
                        $array['template_path'] = $this->_values['template_path'];
            	    }
                    print $this->_view->get_template($this->_loc,'Templates_item',$array);
                }
                print $this->_view->get_template('global','section_foot');
                $GETSOMETHING = false;
           }
           if(isset($_GET['template'])){
                $array['urlto'] = $name;
                $array['namekey'] = $name;
                $GETSOMETHING = false; 
           }
           if(isset($_GET['create'])){
                print $_GET['param'];
                $this->_values['content']= $_GET['param'];
                $this->_values['content'].= $this->_view->get_Templateclone($this->_values['location'],$this->_values['name'],[],false);
                $this->_values['formname']='editor';
                print $this->_view->get_template($this->_loc,'edit',$this->_values);
                $GETSOMETHING = false;
           }
           if(isset($_GET['preparepack'])){           
                $FileScanner = new \data\FileScanner();
                $exts['inc']= ['.Qtemplate'];
            	$FileScanner->setfilter($exts);
                $path = '/layout/views/'.$this->_values['template_path'];
                $FileScanner->scandir('./layout/views/'.$this->_values['template_path']);
                $files = $FileScanner->getfils();
                $list['id'] = 'category';
                $list['title'] = '';
                print $this->_view->get_template('global','section_head',$list);
            	foreach($files as $file){
            	    if(substr($file, 0, 1) != '.'){
                        print '[\''.implode('\',\'',explode('/',$path)).'\',\''.$file.'\'],';
            	    }
                }
                print $this->_view->get_template('global','section_foot');
                $GETSOMETHING = false;
           }
           if(isset($_GET['Qcreate'])){
                $this->_values['content']= $_GET['param'];
                $pretemps['tegel']= 'Apre_tegel';
                foreach($pretemps as $name => $template){
                        $this->_values['content']= $_GET['param'];
                        $this->_values['content'].= $this->_view->get_Templateclone($this->_loc,$template,[],false);
                        $this->_values['formname']='editor';
                        print $this->_view->get_template($this->_loc,'qedit',$this->_values);
                }
                $GETSOMETHING = false;
           }

          if(isset($_GET['edit'])){
                $list['id'] = 'info';
                $list['title'] = '[-- -- --]';
                print $this->_view->get_template('global','section_head',$list);
                $this->_values['content']=  \autosystem\Properties::get_Qtemplate($this->_values['template_path'],$this->_values['name'],true);
                print $this->_view->get_template($this->_loc,'edit',$this->_values);
                print $this->_view->get_template('global','section_foot');
          }
          if(isset($this->_values['previewTemplatecache'])){//&DeleteCacheFrom
                $FileScanner = new \data\FileScanner();//data\FileScanner
                print './layout/views/';
                $FileScanner->scandir('./layout/views/');
                $dirs = $FileScanner->getdirs();
                	foreach($dirs as $dir){
                        $array['namekey'] = $dir;
                        $array['path.module.template']=$dir;
                        $FileScanner->scandir('./layout/views/'.$dir);
                        $Files = $FileScanner->getdirs();
                        foreach($Files as $file){
                            $name = substr($file, 0, stripos($file, '.'));
                            if($this->_view->existcach($dir,$name)){
                                $array['module.template.name']=$name;
                                print $this->_view->get_template($this->_loc,'templatelocation_item',$array);
                            }
                         }
                        print $this->_view->get_template($this->_loc,'templatelocation_item',$array);
                    }
                $GETSOMETHING = false;
           }
           if($GETSOMETHING){
                $accoundid = null;
                $enabled = null;
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