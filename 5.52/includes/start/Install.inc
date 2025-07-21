<?php
/**
 * @author Lieven Roegiers
 * @copyright $Y$
 * @CMS autosite
*/
namespace start;
class Install extends \autosystem\Controller{
    private $values = ['search','find','notinstalled','install','installed','uninstall','console','unconstruct','title.InstallsAccess','title.InstallsAccess','return.label.compactible','return.label.incompactible'];
    private $installoptions = ['0'=>'notinstalled','install'=>'install'];
    private $uninstalloptions = ['1'=>'installed','uninstall'=>'uninstall'];
    private $onlyinstalled = false;
    private $onlynotinstalled = false;
    //$UserRulles = [],$islogin=false,$defaultPageRange=20
    private $packets=[] ;
    public function __construct($loc, $modelname,$locale,$menufilter=[]){
    	//__construct($loc='',$modelname='',$locale,$menufilter,$debug=false)
        parent::__construct($loc, $modelname,$locale,$menufilter, true);
    }
    public function init($incpath='./includes/'){
       $this->getModel();
       $this->incpath = getAutositePath(['']);
       //$this->incpath = './'.implode('/',getRealinstall()).'/';
       
       $this->_model->setincludeslocation($this->incpath);
       $this->vars();
       if(isset($_POST['search'])&&$this->_values['search']!=''){
            $this->prepare($this->_loc,$this->_action,$this->_values,$this->_values['search']);
            return;
       }
       if(key_exists('reset',$_GET)){
            $this->_model->setSTATUS('OK',$_GET['reset']);
       }
       if(key_exists('cancel',$_GET)){
            $this->_model->setSTATUS('CANCELED',$_GET['cancel']);
       }
       if(key_exists('upgrade',$_GET)){
       		if(\autosystem\ValidateAs::isFULLTEXT($_GET['upgrade'])){
              	//print 'upgrade'.$_GET['upgrade'];
                
                $arr = $this->_model->GET_InstallAsArray($_GET['upgrade']);
                $path = explode('/',$arr['location']);
                $empty = [''];
                $res = array_diff($path, $empty);//remove empty values
                $modelname = '\\'.implode('\\',$res).'_model';
                if($this->_model->getVersionNr() > $arr['version']){
                    $this->_model->upgradeModule($modelname);
                }else{
                    $this->errors[]='hola la is actual version';
                }
            }else{
              $this->errors[]='hola la a  upgrade key must be valid see getModelInfo() used for translation to';
            }
       }
       if(key_exists('prepare',$_GET)){
		    if(isset($_GET['translationbackup'])){
		       
    		}
	   }
       if(key_exists('upgradeall',$_GET)){ 
			if ($d = Dir ($this->incpath)){
            	while(false !== ($entry = $d->read())){
              	$pos = strpos ($entry, '.');
                if($pos === false){
                    if ($ds = Dir($this->incpath.'/'.$entry)){
			            while(false !== ($item = $ds->read())){
			                
			                if( substr($item, -9, -4) == 'model' ){
			                    //print($item)."\n";  
			                    $modelname = '\\'.$entry.'\\'.substr($item, 0, -4);
                                $this->_model->upgradeModule($modelname);			                        
			                }
			        }}
                    
                }
            }
			}
		}
       if($this->checkItS()){  return;}
       if(key_exists('type',$_POST)){
            $cansee = $_POST['ispublic'];
            if(is_array($cansee)){
                $this->saveVisibility($cansee,$this->getAccound_id());
            }
       }
       if(isset($_POST['install'])){
            $this->setInConsole(true,'start','no.installs.todo');
            if($this->setInConsole(isset($_POST['installs']),'installs','no.installs.todo')){
                $this->render($_POST['installs']);
            }
            if($this->setInConsole(isset($_POST['inmenus']),'menus','no.menus.todo')){
                $this->menu_install($_POST); 
            }
            if($this->setInConsole(isset($_POST['type']),'mnu_types','no.mnu_types.todo')){
                $this->type_install($_POST);
            }
            $this->prepare();
            return;
       }
       $this->prepare();
    }
    private function vars(){
        $this->addTranslationValues($this->values);
        $this->addTranslationKeys($this->_model->modeltypes);
        $this->_values = $this->getTranslations();
        $this->_values['title']='installation:';
        $this->_values['search'] = (key_exists('search',$_POST))?$_POST['search']:'';
        if(isset($_POST['find'])){
                $this->_values['search']=(isset($_POST['search']))?$_POST['search']:'';
                $this->_values['q']=(isset($_POST['q']))?$_POST['q']:$this->_values['search'];
        }else{
                if(isset($_GET['edit'])){
                    $this->_values['edit']=(isset($_GET['edit'])&&\autosystem\ValidateAs::isnumeric($_GET['edit']))?$_GET['edit']:0;
                }
        }
        //@print_r($_POST['plugin']);
    }
    private function render($packets){
        foreach($packets as $location=>$model){
           foreach($model as $key=>$installtype){
               $modelloc =  $location.'\\'.$key.'_model';
               if($installtype=='install'){     $this->installModel($modelloc,$location.'/'.$key.'/',false,false);}
               if($installtype=='uninstall'){   array_push($this->errors,$this->_view->get_AtranslationB('start_uninstall').': '.$modelloc);}
               if($installtype=='upgrade'){     array_push($this->errors,$this->_view->get_AtranslationB('start_upgrade').': '.$modelloc);}
           }
        }
    }
    private function installModel($modelname,$surfpath){
        if($model = new $modelname()){
            if(!$model->isInstalled()||!$this->_model->existInstalledItem($model->getModelInfo())){
               $isinstalled =  $model->__install();
               $key = $model->getModelInfo();
               $refmenu = 'null';
               $type = $model->getRuleDefault();
               $version = (double)$model->getVersionNr();
               $this->_model->addInstalledItem($key,$surfpath,'OK',$refmenu,$type,$version);
               array_push($this->errors,'[ok]version :'.$model->getVersionNr().' and module '.$this->_view->get_AtranslationB('isinstalled').': '.$modelname.' ');
            }else{
               $isinstalled = true;
               array_push($this->errors,'[Nok]'.$this->_view->get_AtranslationB('wasinstalled').': '.$modelname.' ');
            }
        }else{
            array_push($this->errors,$this->_view->get_AtranslationB('cannotintall').': '.$modelname.' ');
        }
    }
    private function menu_install(&$post){
        foreach($post['inmenus'] as $location=>$model){
           foreach($model as $key=>$installtype){
               $modelname =  $location.'\\'.$key.'_model';
               $ismenu = $installtype=='on';
               $ispublic = isset($post['ispublic']) && isset($post['ispublic'][$location]) && @$post['ispublic'][$location][$key]=='on';
               $this->installMenu($modelname,$location.'/'.$key.'/',$ismenu,$ispublic);
               if(isset($post['type']) && isset($post['type'][$location])){
                    @array_push($this->errors,$post['ispublic'][$location][$key].': '.$modelname.' ');
               }
           }
        }
    }
    private function type_install(&$post){
        foreach($post['type'] as $location=>$model){
           foreach($model as $key=>$installtype){
               $loc =  $location.'/'.$key.'/';
               if(isset($post['type']) && isset($post['type'][$location])){
                    $this->_model->setTYPE($loc,$post['type'][$location][$key]);
               }
           }
        }
    }
    private function installMenu($modelname,$urlloc,$inmenu=false,$ispublic=false){
        try{
		if($model = new $modelname()){
            if($model->isInstalled() ){
                    $key= $model->getModelInfo();
                    if($this->_model->existInstalledItem($key)){
                        if(!$this->_model->existInstalledItem($key)){
                            $this->_model->addInstalledItem($key,$urlloc,'OK');
                        }
                        if($inmenu||$this->_model->isInMenu($key,$urlloc)){
                            $comment = ($this->_model->setToMenu($urlloc,$inmenu)&&$inmenu)?'addedtomenu':'removedtomenu';
                            array_push($this->errors,$this->_view->get_AtranslationB($comment).': '.$modelname.' ');
                        }
                        if($ispublic||$this->_model->isMenuPublic($key,$modelname)){
                            $comment = ($this->_model->SetMenuPublic($urlloc,$ispublic)&&$ispublic)?'addedtopublic':'removedtopublic';
                            array_push($this->errors,$this->_view->get_AtranslationB($comment).': '.$modelname.' ');
                        }
                    }
            }
        }else{
            array_push($this->errors,$this->_view->get_AtranslationB('canaddtomenu').': '.$modelname.' ');
        }
        }catch(Exception $e){
                        $e['error']= '<br> </li> </ul> </li> </ul>...<div class="errors" > '.$this->_view->get_AtranslationB('error.classnotfound').$modelname.' </div>';
                        print $this->_view->get_template($this->_loc,'pluginitem_error',$e);
                        print '<h1> check </h1>';
        }
    }
    //<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<VIEW SOMETHING>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
    private function prepare(){
       print $this->_view->getheader($this->_loc,$this->_action);
       $array['id'] = 'Sub_mnu';
       $array['title'] = '';
       print $this->_view->get_template('global','section_head',$array);
       print $this->_view->get_template($this->_loc,$this->_action,$this->_values);
       print $this->_view->get_template('global','section_foot',[]);
       print $this->_view->getform_head($this->_loc,$this->_action);
       $GETEVERYTHINGELSE = true;
       if(isset($_GET['access'])){
           $array['id'] = 'info';
           $array['title'] = $this->_values['title.InstallsAccess'];
           print $this->_view->get_template('global','section_head',$array);
                 $this->_model->GET_AOverView($this->_loc,'accessPlugin',$this->_view);
           print $this->_view->get_template('global','section_foot',[]);
           $GETEVERYTHINGELSE = false;
       }
       if(isset($_GET['edit'])){
           $array['id'] = 'edit';
           $array['title'] = $this->_values['title.install.edit'];
           print $this->_view->get_template('global','section_head',$array);
                 $this->_model->GET_AInstall($this->_loc,'edititem',$this->_view,$this->_values['edit']);
           print $this->_view->get_template('global','section_foot',[]);
           $GETEVERYTHINGELSE = false;
       }
       if(isset($_GET['addnewinstall'])){
            $template ='';
           if(isset($find)){      print $this->_view->get_template($this->_loc,'findresult',$this->_values);      }
            $find=false;
            $console = '';
            print $this->checkinstallation($this->incpath,$template,$console,$find);
            //------- replace next edition by $this->status($console);
            $array['id'] = 'consol_output';
            $array['title'] = $this->_values['console'];
            print $this->_view->get_template('global','section_head',$array);
            print '<div class="errors" ><pre>';
            $ln = "\n";
            print $ln.'[ok]start';
            print $ln.'[ok]cmsversion :'.$this->_model->getVersionNr();
            print $ln.'[ok]phpversion :'.phpversion();
            print $ln.'[ok]filelocation :'.__FILE__;
            print $ln.'[ok]inclocation  :'.$this->incpath;
            print $ln.'[ok]excuses from developers from bug at packing version 5.30 :';
            print $ln.'[ok]please report bugs or exploits to i@one2try.be :';

            
            print $ln.'[NEED CHECK]./?hostinfo : checks in the futures';
            print $ln.'[NEED CHECK]different between hosting and own server and unlock more functions';
            print $ln.'[NOK] NOT INIT THE owner functions [unconstruct] look forward';
            print $ln.'[!0!]'.(file_exists('.prosession'))?'prosession is choice':'no prosession see access construct';
            print $ln. $console;
            print $ln.'[ok]templatecache:'.($this->_view->isCachIT()?'TRUE':'FALSE');
            print  implode($ln,$this->packets);
            print  implode($ln,$this->errors);
            print '</pre></div>';
            print $this->_view->get_template('global','section_foot',[]);
            //-------
           $GETEVERYTHINGELSE = false;
       }
       if(isset($_GET['installsdir'])){
            print $this->checkinstalldirs($this->incpath,'dirview');
       }
       if(isset($_GET['hostinfo'])){
       	    $console = '';
            //------- replace next edition by $this->status($console);
            $array['id'] = 'consol_output';
            $array['title'] = $this->_values['console'];
            print $this->_view->get_template('global','section_head',$array);
            print '<div class="errors" ><pre>';
            $ln = "\n";
            print $ln.'[ok]start';
            print $ln.'[ok]cmsversion :'.$this->_model->getVersionNr();
            print $ln.'[ok]phpversion :'.phpversion();
            print $ln.'[ok]filelocation :'.__FILE__;
            print $ln.'[ok]inclocation  :'.$incpath;
             print $ln.'[ok]'.ini_get('post_max_size');
            print $ln.'[NEED CHECK]max exe '.ini_get('max_execution_time');
            print $ln.'INFO :'.$ln;
			print_r(phpinfo());
			print $ln.'GDINFO :'.$ln;
			print_r(gd_info());
            print $ln.'[ok]templatecache:'.($this->_view->isCachIT()?'TRUE':'FALSE');
            print '</pre></div>';
            print $this->_view->get_template('global','section_foot',[]);
	   }
       if(isset($_GET['sitemap'])){
           foreach($this->_user->getAllRules()as $rulle){
                $array['id'] = $rulle.'_links';
                $array['classes'] = 'linksblock';
                $array['title'] =  $this->_values[$rulle];
                print $this->_view->get_template($this->_loc,'section_link',$array);
                $this->_model->GET_ASiteMapOverView($this->_loc,'sitemapmod_item',$this->_view,$rulle);
                print $this->_view->get_template($this->_loc,'section_foot',[]);
           }
           $GETEVERYTHINGELSE = false;
       }
       if($GETEVERYTHINGELSE){
            //tegels
            $array['id'] = 'packages';
            $array['title'] = $this->_values['title.InstallsAccess'];
            print $this->_view->get_template('global','tooltip_start',[]);
            print $this->_view->get_template('global','section_head',$array);
            
                  $this->_model->GET_AOverView($this->_loc,'plugintegel',$this->_view);
            print $this->_view->get_template('global','section_foot',[]);
       }
       print '<br>';
       print $this->_view->getform_end($this->_action,'install',[]);
       print $this->_view->get_template($this->_loc,'foot',$this->_values);
       print $this->_view->getfooter($this->_action);
    }
    private function checkinstallation($path,&$template,&$console,$find){
            $d = scandir($path);
            print '<section id="install_category" > <ul id="install" >';
            foreach($d as $dir){
              	$pos = strpos ($dir, '.');
                if($pos === false && is_dir($path.$dir)){
                    //$console .= "\n".'[ok] scan @'.$path.$dir;
                    if($this->hasModel($path.$dir)){
                        $this->viewItems($dir,$path,$dir,$console);
                    }else{
                        $console .= "\n".'->'.$path.$dir.' HAS NO model';
                    }
                }
            }
            print ' <br></ul> </section>';
    }
    private function checkinstalldirs($path,$template='pluginitem',$installed=true){
            $d = scandir($path);
            $installed;
            foreach($d as $dir){
              	$pos = strpos ($dir, '.');
                if($pos === false && is_dir($path.$dir)){
                    $this->viewdirItems($dir,$path,$dir,$template,$installed);
                }
            }
    }
    private function viewdirItems($name,$path,$location,$template,$installed){ 
        if ($d = Dir ($path.'/'.$location)){
            print '<li><img src="'.$this->_view->getimgPath().'/icons/d/directory.svg'.'" height="25" ><span> '.$location.' </span>+<ul>';
            while(false !== ($entry = $d->read())){           
                if( substr($entry, -9, -4) == 'model' ){
                    $modelname = $location.'\\'.substr($entry, 0, -4);
                    try{
                            if($model = new $modelname()){
                            if($installed && !$model->isInstalled()){//installed
                                continue;
                            }
                            if($this->onlynotinstalled && $model->isInstalled()){//installed
                                continue;
                            }
                            $isinstall = $model->isInstalled()&& $this->_model->existInstalledItem($model->getModelInfo());
                            $values['name']=substr($entry, 0, -10);
                            $values['location']=$location;//includes\start\Install_model.inc
                            $values['msg']=$this->_view->get_AtranslationB($model->getModelInfo());
                            $values['installoptions'] = $this->_view->getOptions(($isinstall?$this->uninstalloptions:$this->installoptions));
                            if($model->isInstalled()&&isset($_POST['installs'][$location])&&$_POST['installs'][$location]=='installed'){
                                print 'not installed';
                            }
                            $values['inmenu']=($this->_model->isInMenu($model->getModelInfo(),$location.'/'.$values['name'].'/'))?' checked':'';
                            $values['ispublic']=($this->_model->isMenuPublic($model->getModelInfo(),$location.'/'.$values['name'].'/'))?' checked':'';
                            $values['type'] = '';                            
                            $values['types']=$this->_view->getOptions($this->_model->modeltypes,$values['type']);                            
                            $values['isinstalled']=$this->_view->getCheckImg($isinstall);
                            $values['moduleversion']= $model->getVersionNr();
                            $iscompactible = $this->checkCompactible($model);
                            if(!$iscompactible){
                                $this->errors[] = '<font style="color:'.(($isinstall)?'red':'orange').'" >[NOK]'.$modelname.' '.$this->_values['return.label.incompactible'].' NEED UPGRADE (CAN WORK NEED CHECKUP)) '.(($isinstall)?'INSTALLED':'').' from'.$model->getVersionNr().' to '.$this->_model->getVersionNr().'<font>';
                                //$console .= "\n".'->[NOK]'.$modelname.' NEED UPGRADE from'.$model->getVersionNr().' to '.$this->_model->getVersionNr();
                            }else{
								$this->errors[] = '<font style="color:'.(($isinstall)?'green':'blue').'" >[OK]'.$modelname.' '.$this->_values['return.label.compactible'].' '.(($isinstall)?'INSTALLED':'').' from'.$model->getVersionNr().' to '.$this->_model->getVersionNr().' to './*$this->_model->get().*/'<font>';
							}
                            $values['color']=($iscompactible)?'OK':'FAIL';
                            $values['iscompactible']=$this->_view->getCheckImg($iscompactible);
                            print $this->_view->get_template($this->_loc,$template,$values);
                        }else{
                            print '<h1> '.$location.'\\'.substr($entry, 0, -4).' </h1>';
                        }
                    }catch(AException $e){
                        $e['error']= '<br> </li> </ul> </li> </ul>...<div class="errors" > '.$this->_view->get_AtranslationB('error.classnotfound').$modelname.' </div>';
                        print $this->_view->get_template($this->_loc,'pluginitem_error',$e);
                        print '<h1> check </h1>';
                        continue;
                    }catch(Exception $e){
                        $e['error']= '<h1>ERROR</h1><br> </li> </ul> </li> </ul>...<div class="errors" > '.$this->_view->get_AtranslationB('error.classnotfound').$modelname.' </div>';
                        print $this->_view->get_template($this->_loc,'pluginitem_error',$e);
                        print '<h1> check </h1>';
                        continue;
                    }
                }
        }print '</ul></li>';  }
    }
    public function hasModel($path){
        if ($d = Dir ($path)){   while(false !== ($entry = $d->read())){
                if( substr($entry, -9, -4) == 'model' ){
                    return true;
                }
        }  }
        return false; 
    }
    private function viewItems($name,$path,$location,&$console){ 
        if ($d = Dir ($path.'/'.$location)){
            print '<li> <a href="#" > <span> '.$location.' </span> </a> <ul>';
            while(false !== ($entry = $d->read())){           
                if( substr($entry, -9, -4) == 'model' ){
                    $modelname = $location.'\\'.substr($entry, 0, -4);
                    try{
                        //IF MODEL IS NOT FOUND WATCH TO CLASSNAME AND OR NAMESPACE (OR IF YOU IMPLEMENT ALL METHODES)
                        if($model = new $modelname()){
                            if($this->onlyinstalled && !$model->isInstalled()){//installed
                                continue;
                            }
                            if($this->onlynotinstalled && $model->isInstalled()){//installed
                                continue;
                            }
                            $isinstall = $model->isInstalled()&& $this->_model->existInstalledItem($model->getModelInfo());
                            $values['name']=substr($entry, 0, -10);
                            $values['location']=$location;//includes\start\Install_model.inc
                            $values['msg']=$this->_view->get_AtranslationB($model->getModelInfo());
                            $values['installoptions'] = $this->_view->getOptions(($isinstall?$this->uninstalloptions:$this->installoptions));
                            if($model->isInstalled()&&isset($_POST['installs'][$location])&&$_POST['installs'][$location]=='installed'){
                                print 'not installed';
                            }
                            $values['inmenu']=($this->_model->isInMenu($model->getModelInfo(),$location.'/'.$values['name'].'/'))?' checked':'';
                            $values['ispublic']=($this->_model->isMenuPublic($model->getModelInfo(),$location.'/'.$values['name'].'/'))?' checked':'';
                            $values['type'] = '';                            
                            $values['types']=$this->_view->getOptions($this->_model->modeltypes,$values['type']);                            
                            $values['isinstalled']=$this->_view->getCheckImg($isinstall);
                            $values['lblcompactible']=($iscompactible)?$this->_values['return.label.compactible']:$this->_values['return.label.incompactible'];
                            
                            $values['moduleversion']= $model->getVersionNr();
                            $iscompactible = $this->checkCompactible($model);
                            if(!$iscompactible){
                                $this->errors[] = '<font style="color:'.(($isinstall)?'red':'orange').'" >[NOK]'.$modelname.' '.$this->_values['return.label.incompactible'].' NEED UPGRADE (CAN WORK NEED CHECKUP)) '.(($isinstall)?'INSTALLED':'').' from'.$model->getVersionNr().' to '.$this->_model->getVersionNr().'<font>';
                                $console .= "\n".'->[NOK]'.$modelname.' NEED UPGRADE from'.$model->getVersionNr().' to '.$this->_model->getVersionNr();
                            }else{
								$this->errors[] = '<font style="color:'.(($isinstall)?'green':'blue').'" >[OK]'.$modelname.' '.$this->_values['return.label.compactible'].' '.(($isinstall)?'INSTALLED':'').' from'.$model->getVersionNr().' to '.$this->_model->getVersionNr().' to './*$this->_model->get().*/'<font>';
							}
                            $values['color']=($iscompactible)?'OK':'FAIL';
                            $values['iscompactible']=$this->_view->getCheckImg($iscompactible);
                            print $this->_view->get_template($this->_loc,'pluginitem',$values);
                        }else{
                            print '<h1> '.$location.'\\'.substr($entry, 0, -4).' </h1>';
                        }
                    }catch(AException $e){
                        $e['error']= '<br> </li> </ul> </li> </ul>...<div class="errors" > '.$this->_view->get_AtranslationB('error.classnotfound').$modelname.' </div>';
                        print $this->_view->get_template($this->_loc,'pluginitem_error',$e);
                        print '<h1> check </h1>';
                        continue;
                    }catch(Exception $e){
                        $e['error']= '<h1>ERROR</h1><br> </li> </ul> </li> </ul>...<div class="errors" > '.$this->_view->get_AtranslationB('error.classnotfound').$modelname.' </div>';
                        print $this->_view->get_template($this->_loc,'pluginitem_error',$e);
                        print '<h1> check </h1>';
                        continue;
                    }
                }
        }print '</ul></li>';  }
    }
    /*public function makegui($isinstalled){
        if(isinstalled($entry,'01')){
            print '<td> INSTALLED </td><td>'.$name.'</td>';
        }else{
            if(isset($_POST['instals'][$entry])&&$_POST['instals'][$entry]['install']=='on'){
                print 'Start install'.$entry.'<br>';
                if(is_file($path.$entry.'/install.php')){
                    include($path.$entry.'/install.php');
                }
            }
        }
    }*/
    private function checkItS(){
        if(!key_exists('check',$_GET)){return ;}
        $return = false;
        if(key_exists('ispublic',$_POST)){ 
            $ispublic = $_POST['ispublic'];
            if(is_array($ispublic)){
                foreach($ispublic as $key =>$value){
                    foreach($value as $k =>$v){
                        if($this->_model->SetMenuPublic($key.'/'.$k.'/',$v)){       $return = true; }
                    }
                }
            }
        }
        if(key_exists('inmenus',$_POST)){
            $inmenus = $_POST['inmenus'];
            print_r($inmenus);
            if(is_array($inmenus)){
                foreach($inmenus as $key =>$value){
                    foreach($value as $k =>$v){
                        if($this->_model->setToMenu($key.'/'.$k.'/',$v)){       $return = true; }
                        print 'inmenu?';
                    }
                }
                
            } 
        }
        if($return === true){
            $result = ['ok'=>true ];
            header('HTTP/1.0 200 ');print json_encode($result);
        }else{
            $result = ['nok'=>false ];
            header('HTTP/1.0 200 ');print json_encode($result);
        }
        return $return;
    }
    private function checkCompactible(&$model){
        //more check wil be follow
        /**
         * switch()...
        */        
        return  (float)$this->_model->getVersionNr()==(float)$model->getVersionNr();
    }
}
?>