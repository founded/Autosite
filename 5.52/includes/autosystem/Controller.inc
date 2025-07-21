<?php
namespace autosystem;
abstract class Controller extends \autosystem\Access{
    protected $DEBUG;
    protected $_loc;//location
    protected $_action ='Start' ;
    protected $_model = NULL;//object
    protected $_view = NULL;//object
    //protected $_pad = '';
    protected $_defaultLocale ='en';
    protected $errors;
    protected $locale;
    protected $_translationKeys=[];
    protected $incpath = './includes/';
    protected $_version ;
    protected $_values;
    protected $_validator;//object
    protected $_trigeraddslash=false;
    public function __construct($loc,$modelname,$locale,$menufilter,$debug=false){
        $this->incpath =  getAutositePath(['']); 
        $this->locale=$locale;
        $this->errors = [];
        $this->_values = [];
        parent::__construct($menufilter);
        $this->_loc = $loc;
        $this->_action = $modelname;
        $this->_validator = new \autosystem\Avalidator($locale,$debug);
        $rulles = @$this->_user->getRules();
        if(!empty($rulles)){  $this->_view = new \autosystem\Qview($locale,$this->_user->getAllRules(),$rulles);
        }else{                $this->_view = new \autosystem\Qview($locale,$this->_user->getAllRules());
        }
        if(is_file($this->incpath. $this->_loc .'/this.tags')){
            $this->_view->addkeywords(file_get_contents(getAutositePath([$this->_loc,'this.tags'])));
        }
        if(is_file($this->incpath.'this.tags')){
            $this->_view->addkeywords(file_get_contents(getAutositePath(['this.tags'])));
        }
        $this->DEBUG = $debug;
        global $_AUTOSITEVERSIONNR;
        $this->version = $_AUTOSITEVERSIONNR;
    }
    public function setIncPath($path){        $this->incpath = $path;    }
    protected function addTranslationKeys($keys){
        foreach($keys as $key => $value){
            array_push($this->_translationKeys,$key);
        }
    }
    protected function getstartlocation(){     return '/'.$this->_loc.'/'.$this->_action.'/';    }
    protected function addTofav($get){ $this->_user->setLocationToHistory($this->getstartlocation(),$get,true); }
    public function setLocation($get){ $this->_user->setLocationToHistory($this->getstartlocation(),$get); }
    protected function getLocationHistory($favs){       return $this->_user->getLocationHistory($favs);  }
    protected function getLocationHistoryKeys($favs){   return $this->_user->getLocationHistory($favs);  }
    protected function addTranslationValues($values){
        if(isset($values)&& is_array($values)){
           foreach($values as $key => $value){
                array_push($this->_translationKeys,$value);
           }
           return;
        }
        throw new \Exception('addTranslationValues input not array');
    }
    protected function getTranslations(){     return $this->_view->get_translations($this->_translationKeys);   }
    protected function Qform($formname,$submit=''){
        $submit = ($submit!='')?$submit:$formname;
        print$this->getSectionform($this->_loc,$this->_action,$formname,$this->_values,$submit,'','',$formname);
    }
    protected function getSectionform($loc,$action,$template,$values,$submitbtn,$id,$title,$validatorform){
        $values['Qformname']=$validatorform;
        if($this->_user->hasRulle('KING')){
                print "KING OPTION<!-- $loc,$action,$template,$values,$submitbtn,$id,$title,$validatorform -->";
                print (key_exists($submitbtn,$_POST))?'SUBMITED FORM':'';
            	print $this->_view->get_template('global','formtemplatetools',$values);            	
		}
        if(key_exists($validatorform,$_GET)){
            if(key_exists($submitbtn,$_POST)){       	          return;}
            if(key_exists('runcreator',$_GET)){
        		$params = '?create&location='.$loc.'&name='.$validatorform;
			    print '<form name="editor" class="form-horizontal" action="'.$this->_view->getinstallpath().'/template/TemplateSummary/'.$params.'" method="post" target="_blank" name="editor" enctype="multipart/form-data" >';
			    print '<input type="hidden" name="edit" value="create" >';
				print '<input type="text" id="location" name="location" value="'.$loc.'" required />';
    			print '<input type="text" id="name" name="name" value="'.$template.'" required />';
				print '<textarea name="content" >';
				print $this->_validator->formcreator($validatorform,$loc,$action);
				print '</textarea>';
				print '<input type="submit" name="toeditor" >';
				print '</form>';
			}else{
			    $cashname = 'Sectionform_'.$template;
                if($this->_view->isCachIT()&&$this->_view->existcach($loc,$cashname)){
                    return $this->_view->get_template($loc,$cashname,$values);
                }else{
                    $html = '*';
                    $html .= $this->_view->get_template('global','section_head',[$id,$title]);
                    $html .= $this->_view->getform_head($loc,$this->_action);
                    if($this->_view->isCachIT()){
                        $html .= $this->_view->get_template($loc,$template);
                    }else{
                        $html .= $this->_view->get_template($loc,$template,$values);
                    }
                    $html .= $this->_view->getform_end('',$submitbtn);
                    $html .= $this->_view->get_template('global','section_foot');
                    if($this->_view->isCachIT()){
                        $this->_view->makecach($loc,$cashname,$html);
                        return $this->_view->get_template($loc,$cashname,$values);
                    }else{
                        return $html ;
                    }
                    print $html;
                }
			}
		}else{
		      return false;
		}
        
		
        
        
        /**/

    }
    protected function tosupportreturn($isdone,$whatidid,$param=[]){
         if($isdone){  $param[]='success='.$whatidid;
         }else{        $param[]='failed='.$whatidid;
         }
         $this->GOESTO($this->_loc,$this->_action,$param);
    }
    protected function getModel(){
        $model = get_called_class().'_model';
        if(is_ClassExist($model)){
                $rulles = @$this->_user->getAllRules();
                $this->_model =(!empty($rulles))? new $model($rulles):new $model();
                if($this->_model->getModelInfo() == 'Install.model.info'){
                    return;
                }
                if(!$this->_model->isInstalled()){
                    $this->_model->setInERROR('Controller','Model Not installed',$this->_model->getModelInfo(),__LINE__);
                    print '---';
                    //$this->GOESTO('start','Install',['addnewinstall'=>'','model'=>$this->_model->getModelInfo()]);
                    sleep(1);
                    die($this->_view->get_AtranslationB('Package.not.installed'));
                }else{
                    $us=\autosystem\DBConn::getTableNameFor('user_stats');
                    $k = ' UPDATE `'.$us.'` SET `pages` = `pages` + 1 WHERE YEAR(`date`)=\''.date('Y').'\' AND MONTH(`date`)=\''.date('m').'\' AND DAY(`date`) =\''.date('d').'\'';
                    $this->_model->Simple_Query($k);                 
                }
        }else{  die('Geen model '.$model);   }
    }
    public function __destruct(){
        if($this->isuser && key_exists('status',$_GET) ){
            $this->status('des tru ct statusreport',false,true);
        }
        unset($this->DEBUG);
        unset($this->_loc);
        unset($this->_action);
        unset($this->_pad);
        unset($this->_defaultLocale);
        unset($this->errors);
        unset($this->locale);
        unset($this->_translationKeys);
        parent::__destruct();
        if(isset($this->_model)){ $this->_model->killcon();  $this->_model->__destruct(); unset($this->_model);  }
        if(isset($this->_view)){  $this->_view->__destruct(); unset($this->_view); }        
    }
    abstract public function init();
    public function __toString() {
        return '=>class:'.get_called_class().'action=>[\''.$this->_action.'\'],model=>[\''.$this->_model->getModelInfo().'\'],view=>\''.$this->_view->__toString().'\'';
    }
    public function setInConsole($res,$keytrue,$keyfalse){
        $lnb="\n";
        $msg = ($res)?$lnb.'[ok] '.$this->_view->get_AtranslationB($keytrue):$lnb.'[nok]'.$this->_view->get_AtranslationB($keyfalse);
        array_push($this->errors,$msg);
        return $res;
    }
    public function validationmsg($msgkey,$isok = false){
		$lnb="\n";
        $msg=[];
		$msg['status'] = ($isok)?'VALIDBOX':'INVALIDBOX';
		$msg['message']= $this->_view->get_AtranslationB($msgkey);
        $this->_values['errors'].= $this->_view->get_template('global','MessageInfo',$msg);    
	}
    public function loadSettings(){
        $sl = $this->getSettingslocation();
        if(file_exists($sl)&&($this->_settings = parse_ini_file($sl))){
        	//print 'load settings';
            return true;
        }
        //print 'not load settings'.$sl;
        return false;
    }
    public function hasSettings($pad,$actie){
        $sl = getAutositePath([$pad,($actie.'.ini.php') ]); 
        //$sl = './includes'.DIRECTORY_SEPARATOR.$pad.DIRECTORY_SEPARATOR.$actie.'.ini.php';
        return file_exists($sl)&& is_readable($sl);
    }
    public function getSettingslocation(){
        return getAutositePath([$this->_loc,($this->_action.'.ini.php') ]);
        //return './includes'.DIRECTORY_SEPARATOR.$this->_loc.DIRECTORY_SEPARATOR.$this->_action.'.ini.php';
    }
    public function saveSettingslocation(){
        //return './includes'.DIRECTORY_SEPARATOR.$this->_pad.DIRECTORY_SEPARATOR.$this->_actie.'.ini.php';
    }
    public function loadTest($fname){
    	$test = new \autosystem\Test($this->_loc,$fname);
    	$istest = $test->loadTest($this->_values);
    	return $istest;
    }
    public function WriteProfileFSET($Fname,$asoarr,$subpath='',$eyeprotection = true){
        $filename = $this->_user->getPersonalPath().$subpath.$Fname;
        //print $filename;
		if (!$handle = fopen($filename, 'w')) {             return false;        }
		
        $content = ($eyeprotection)?'IPROTECTION = "<?php /*<!--" '." \n":'';
        foreach ($asoarr as $key=>$elem){
            $content .= ''.$key.' = "'.$elem.'"'." \n";
        }
        $content .= ($eyeprotection)?'ICLOSEPROTECTION = "-->*/" '." \n":'';
        //print $content;
        $success = fwrite($handle, $content);
        fclose($handle);
        return $success; 
	}
	public function ReadProfileFSET($Fname,$subpath=''){
		$filename = $this->_user->getPersonalPath().$subpath.$Fname;
		//print $filename;
        if(file_exists($filename)&& is_readable($filename)){
            if($data = parse_ini_file($filename)){//@SEE if charset not exist WHITE SCREEN OF DEAD
                return $data;
            }
        }
		return false;
	}
    public function GetProfilePath($subpath='',$create=false){
        if(!is_dir($this->_user->getPersonalPath().$subpath)){
            if(!$create){
                return false;
            }
            mkdir($this->_user->getPersonalPath().$subpath,0644);
        }
        return $this->_user->getPersonalPath().$subpath.'/';
	}
    protected function InputMessages($print=true){
	    $array['id'] = 'inputMessages';
        $array['title'] = $this->_view->get_AtranslationB('validator.input.title');
        $hasmessages = false;
        $Aforms = $this->_validator->getActiveForms();
        $info = '';
        foreach($Aforms as $activeformname){
            $info .= '<div clase="validatorforminfo '.(($this->_validator->isValid($activeformname))?'VALIDBOX':'INVALIDBOX').'" ><h7> '.$activeformname.' </h7> ';
            if(!$this->_validator->isValid($activeformname)){
                $info .= $this->_view->get_AtranslationB('validator.input.invalid');
                $info .= $this->_validator->errorinfo($this->_view,$formname);
                $hasmessages = true;
            }else{
                $info .= $this->_view->get_AtranslationB('validator.input.valid');
            }
            $info .= '</div>';
        }
        if(!$hasmessages){
            //return;
        }
        $returnstring  = '<div class="errors" >';
        //$returnstring .= $this->_view->get_template('global','section_head',$array);
        $returnstring .= $info;
        $grandaccess = false;
        if($grandaccess){
            $returnstring  .= 'grandaccess more options not implement';
            $this->_validator->status($this->_view,true);
        }
        $returnstring .= '</div>';
        if($print){
            print $returnstring;
        }else{
            return $returnstring;
        }

	}
    protected function ShowInfo(){
        $array['id'] = 'Info';
        $array['title'] = $this->_view->get_AtranslationB('info');
        print $this->_view->get_template('global','section_head',$array);
        foreach($this->errors as $error){
            print $error;
        }
        $forms = $this->_validator->getActiveForms();
        foreach($forms as $form){
            print $form . (($this->_validator->isValid($form))?'ok':'nok').'<br>';
        }
        print $this->_view->get_template('global','section_foot',[]);
    }
	protected function status($console,$showvalues=false,$showinput=false,$showforms=true,$print=true){
	    $array['id'] = 'consol_output';
        $array['title'] = $this->_view->get_AtranslationB('statusconsole');
        $returnstring  = $this->_view->get_template('global','section_head',$array);
        $returnstring  .= '<div class="errors" >status($console,$showvalues,$showinput,$showforms,$print)';
        $returnstring  .= 'status('.((empty($console))?'EMPTY':'STRING').','.(($showvalues)?'TRUE':'FALSE').','.(($showinput)?'TRUE':'FALSE').','.(($showforms)?'TRUE':'FALSE').','.(($print)?'TRUE':'FALSE').')<pre>';
        print $returnstring;
		$ln = "\n";
        print $ln.'[ok]start';
        print $ln.'[ok]cmsmodelversion :'.$this->_model->getVersionNr();
        print $ln.'[ok]phpversion :'.phpversion();
        print $ln.'[ok]filelocation :';//.__FILE__
        print $ln.'[ok]inclocation  :'.$this->incpath;
        global $_DEBUG;
        print $ln.'[ok]debug :'.(($_DEBUG)?'TRUE':'FALSE');
        print $ln. $console;
        print $ln.'[ok]templatecache:'.($this->_view->isCachIT()?'TRUE':'FALSE');
        print  implode($ln,$this->errors);
        print '</pre></div>';
        print $this->_view->get_template('global','section_foot',[]);
        if($showvalues){
			print '<div class="" ><pre>';
        	//print_r($this->_values);
        	print '</pre></div>';
        }
        if($showforms){
			print '';
			$this->_validator->status($this->_view,true);
        	print '';
        }
        if($showinput){
			print '<div class="" ><h3>GET</h3><pre>';
        	print_r($_GET);
        	print '</pre></div>';
        }
        if($showinput){
			print '<div class="" ><h3>POST</h3><pre>';
        	print_r($_POST);
        	print '</pre></div>';
        }
        if($showinput){
			print '<div class="" ><h3>SESSION</h3><pre>';
        	print_r($_SESSION);
            print unserialize($_SESSION['visitor'])->__toString();
            print unserialize($_SESSION['user'])->__toString();
        	print '</pre></div>';
        }
	}


}
?>