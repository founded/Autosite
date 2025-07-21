<?php
/**
 * @author Lieven Roegiers
 * @copyright 
 * @CMS autosite
 * Qview standard variables reservation
 * $values['path.layout'] this is the place of layout can change to protect something
 * $values['path.module'] this is the place of module 
 * $values['i']//can use of count outputlines or jquery or engine
 * $values['path.install'] this is the place of install can change when upgrade use versionnr
*/
namespace autosystem;
class Qview extends \autosystem\Generals{
	    private $properties;
		private $template;
        private $activetemplatename;
		private $i;
        private $translatedkeys;
        private $cachIT ;
        private $speedup ;
        private $delimiter='%';
        private $cashdir;
        protected $select;
        protected $notselect;
        private $imgpath;
        private $locationpath;
        private $enum_translations;
        private $rulles;
        protected $rull='PUBLIC';
        private $actualrulls;
        private $cancreatetemplate;
        private $sec_checktemplate=false;
        private $mobile=false;
        private $addslaches=false;
        protected $keywords=['autosite'];
        private $pagewindow =4;
        public function settings(){
            $this->config = getAutositePath(['autosystem','QView.ini.php' ]); 
            //$this->config='./includes'.DIRECTORY_SEPARATOR.'autosystem'.DIRECTORY_SEPARATOR.'QView.ini.php';
            if(file_exists($this->config)&& is_readable($this->config)){
                if($config = parse_ini_file($this->config)){
                    $this->speedup=($config['SPEEDUP']=='on');
                    $this->cachIT=($config['CASH']=='on');
                    $this->cancreatetemplate=$config['CREATETEMPLATE'] == 'on';
                    $this->cashdir=$config['CACHDIR'];
                    $this->designpath=$config['DESIGNPATH'];
                }
            }else{
                    $this->speedup=false;
                    $this->cachIT=false;
                    $this->cancreatetemplate=true;
                    $this->cashdir='./oncache/pretemplate/';
                    $this->designpath='./layout/';
            }
        }
 		/**
 		 * Qview::__construct()
 		 * @param string $locale the locale of the user or selection
 		 * @param array $rulles the rules that you have
 		 * @return void
 		 */
 		function __construct($locale,$rulles,$actualrulls=[]){
            self::settings();
            parent::__construct();
            $this->properties = new \autosystem\Properties($locale);
            $result=[];
            foreach($rulles as $key=>$value){//vallidation of rulles
                if(key_exists($value,$this->modeltypes)){ $result[$value]=''; }else{    throw new exception('unknow rulle['.$value.']'); }
                if($value=='TRANSLATE' ){     $this->properties->set_IAMtranslator();   }
                if($value=='KING' ){          $this->cancreatetemplate=true;            }
            }
            if(!empty($actualrulls)){
                foreach($actualrulls as $value){
                    if(!key_exists($value,$this->modeltypes)){                 			throw new exception('unknow rulles');    }
                    $this->actualrulls[$value]='';
                }
            }else{
                $this->actualrulls =$result;
            }
            $this->rulles= $result;
		}
        public function get_template($location,$name,$values=[],$options=''){
            return $this->get_Templateclone($location,$name,$values);
        }
        //<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<TRANSLATIONS>>>>>>>>>>>>>>>>>>>>>>>>
        function get_Atranslation($key,$locale){ return $this->properties->getATranslation($key,$locale); }
        function get_translations($keys){        return $this->properties->getproperties($keys);          }
        function get_AtranslationB($key){        return $this->properties->getPropertie($key);            }
        public function isCachIT(){              return $this->cachIT;                                    }
        public function preSetTemplate($location,$name,$values){
            $this->activetemplatename='';//no cach
			$html = $this->get_Templateclone($location,$name,$values);
            return $html;
        }
        public function reSetTemplate($location,$name,$values,$presetval){
        	//print 'pre'.print_r($presetval).'post'.print_r($values);
        	$html = $this->preSetTemplate($location,$name,$presetval);
        	$this->template = new Qtemplate($html);
        	$this->template->setvalues($values);
        	$html = $this->template->gethtml();
			preg_match_all('|'.$this->delimiter.'(.*)'.$this->delimiter.'|',$html,$keys);
            //print $html .print_r($keys[1]);
			$keys=$this->array_keyfinder($keys[1]);
            $this->template->translate($this->translatekeys($keys));
            return $this->template->gethtml();
        }
        public function pushTemplate($content,$values){
        	$this->template = new Qtemplate($content);
        	$this->template->setvalues($values);
        	$html = $this->template->gethtml();
			preg_match_all('|'.$this->delimiter.'(.*)'.$this->delimiter.'|',$html,$keys);
            //print $html .print_r($keys[1]);
			$keys=$this->array_keyfinder($keys[1]);
            $this->template->translate($this->translatekeys($keys));
            return $this->template->gethtml();
        }
        public function setTemplateSlaches($addslaches){
            $this->addslaches=$addslaches;
        }
        public function get_Templateclone($location,$name,$values=[],$setval = true){
            $templatename = $location.'/'.$name;
            $values['path.install']=$this->installpath;
            $values['path.js']=$this->getScriptPath();
            if($this->activetemplatename!=$templatename){
                $this->activetemplatename=$templatename;
                $this->i=0;
                if($this->cachIT && $this->existcach($location,$name)){
                    $html = $this->getcach($location,$name);
                    $this->template = new Qtemplate($html);
                    $this->template->setAddSlachesOnValues($this->addslaches);
                }else{
                    if(!$view = Properties::get_Qtemplate($location,$name)){
                        if($this->cancreatetemplate){                           
                           $keys = array_keys($values);
                           $evalue = '%'.implode('->value% %',$keys).'->value%';
                           print Properties::getUrlToCreateTemplate($location,$name,$evalue);
                           print $evalue;
                        }
                    }
					preg_match_all('|'.$this->delimiter.'(.*)'.$this->delimiter.'|',$view,$keys);
                    $keys=$this->array_keyfinder($keys[1]);
                    $this->template = new Qtemplate($view);
                    $this->template->translate($this->translatekeys($keys));
                    $this->template->setcompress($this->speedup);
                    $this->template->setAddSlachesOnValues($this->addslaches);
                    if($this->cachIT){
                        $html=$this->template->gethtml();
                        $this->makecach($location,$name,$html);
                    }
                }
                $values['i'] = ++$this->i;
                $path = explode('/',htmlspecialchars($_SERVER['REQUEST_URI']));
                array_pop($path);
                $this->locationpath=implode('/',$path).'/';
                $values['path.layout']=$this->getLayoutPath();
                $values['path.module']=$this->locationpath;
            }else{
                $values['i']= ++$this->i;
                $values['path.layout']=$this->getLayoutPath();
                $values['path.module']=$this->locationpath;
            }

            $temp = clone $this->template;
            if($this->sec_checktemplate!=false){
				$this->securitycheck_template($temp);
			}
            if(is_array($this->enum_translations)){
            	//print_r($values);
                foreach($values as $key => $value){
                	//print $key.'-'. print_r($value,true); 
                    if(key_exists($value,$this->enum_translations)){
                        $values[$key] = $this->enum_translations[$value];
                    }else{
                        if(isset($this->enum_translations[$value])){
                            print $this->enum_translations[$value];
                        }
                    }
                }
                $temp->setvalues($values);//http://autosite.gsucks/v3./template/TemplateSummary/?
                return $temp->gethtml().$this->templateeditor('/template/TemplateSummary/?edit&location='.$location.'&name='.$name);
            }
            if($setval)$temp->setvalues($values);
            return $temp->gethtml();
        }
        private function templateeditor($url){
            $on =false;
            if($on ==false) return '';
            $values['url']=$url;
            $values['path.layout']=$this->getLayoutPath();
            $values['path.install']=$this->installpath;
             $view =  Properties::get_Qtemplate('global','toedit_template',true);
             preg_match_all('|'.$this->delimiter.'(.*)'.$this->delimiter.'|',$view,$keys);
             $keys=$this->array_keyfinder($keys[1]);
             $template= new Qtemplate($view);
             $template->translate($this->translatekeys($keys));
             $template->setvalues($values);
             return $template->gethtml();
        }
        protected function translatekeys($keys){
            if(is_array($keys)){    return $this->properties->getproperties($keys,'');   }
        }
        protected function securitycheck_template(&$template){
        	//you can use this to find bugs or check tag issu
			$data = new DataSplit_HTML($html);
			$data->getInput($html);
		}
        protected function set($k = '',$v = ''){
            if(!empty($k )){
                $k = strtolower($k);
                $this->_weergave[$k] = $v;
            }
        }
        //<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<CACH>>>>>>>>>>>>>>>>>>>>>>>>
        public function existcach($location,$name,$additionaldir=''){
            $lang = $this->properties->getlocale();
            return file_exists($this->cashdir.''.$lang.'/'.$additionaldir.$location.'_'.$name.'_'.$lang.'.cache');
        }
        public function makecach($location,$name,$html,$additionaldir=''){
            $lang = $this->properties->getlocale();
            if($this->makenotexistlangcachdir($lang)){
                return file_put_contents($this->cashdir.''.$lang.'/'.$additionaldir.$location.'_'.$name.'_'.$lang.'.cache',$html);
            }
            return false;
        }
        public function getcach($location,$name,$additionaldir=''){
            $lang = $this->properties->getlocale();
            return file_get_contents($this->cashdir.''.$lang.'/'.$additionaldir.$location.'_'.$name.'_'.$lang.'.cache');
        }
        private function makenotexistlangcachdir($lang){
            if(!is_dir($this->cashdir.''.$lang.'/')){
               //print $this->cashdir.''.$lang.'/';
               return @mkdir($this->cashdir.''.$lang.'/', 0755);
            }else{
               return true;
            }
        }
        private function iscachSelect($location,$name){      return true;   }
        /**
         * Qview::setENUM_TRANSLATIONS()
         * wil if a return value of the database some key and this wil translate like
         * find 'true' can translate to 'yesssss' (no array as value see 4. need changed)
         * @param mixed $asoarray
         * @return void
         */
        public function setENUM_TRANSLATIONS($asoarray){     $this->enum_translations=$asoarray; 
		}
        protected function get($k=''){
            if(!empty($k)){
                $k = strtolower($k);
                if(isset($this->_weergave[$k])){
                    return $this->_weergave[$k];
                }
            }
            return false;
        }
        public function __toString(){       return '=>class:Qview=>[locale=>'.$this->properties->getlocale().']'; }
        public function getform_values($action){ return array();  }
        public function __call($name, $arguments){  /*print '__call:'.$name.print_r($arguments,true).'<br>';*/  }
        public function getfind_values(){
            return ['search'=>((isset($_POST['search']))?((string)filter_input(INPUT_POST, 'search')):'')];
        }
        public function getNAVselector($path,$subpath){
            $transes =  $this->properties->getproperties(array_keys($this->rulles));
            return '<ul id="Mselector" >'.$this->get_liLinks($path.$subpath.'?setMenu=',$transes,'not av','&amp;GOFORIT').'</ul>';
        }
        private function nav($loc,$action){
            $navitems = $this->getnav_array()+$this->getpage_array();          
            $return ='';
            foreach($navitems as $key=>$value){
                $val['nav.title']= $this->properties->getPropertie($key);
                $val['nav'] = $this->installpath.'/'.$value.'';
                $val['class'] = ($loc.'/'.$action.'/'== $value)?'active':'';
                $return .= $this->get_Templateclone('global','nav_item',$val);
            }
            return $return;
        }
        public function getnav_array(){
            $return = [];
            if($this->isinstalled()){
                $i = \autosystem\DBConn::getTableNameFor('installs');
                    $dbSql = 'SELECT `key`,`location` FROM `'.$i.'` WHERE `MENU`="TRUE" AND(';
                    $mer ='';
                    foreach($this->actualrulls as $key=>$value){     $dbSql .= $mer.' `TYPE`="'.$key.'"';       $mer = ' OR ';
                    }
                    $dbSql.=')';
                $data = \autosystem\DBConn::GET();
                $dbStmt = $data->prepare($dbSql);
                $dbStmt->execute();
                while($dbRij = $dbStmt->fetch(\PDO::FETCH_ASSOC)){
                    $return['MenuLabel.'.$dbRij['key']]=$dbRij['location'];
                }
            }
            return $return;
        }
        public function getpage_array(){
            $return = [];
            if($this->isinstalled()){
                $lang=$this->properties->getlocale();
                $p=\autosystem\DBConn::getTableNameFor('page');
                $dbSql='SELECT `'.$p.'`.`id` , `'.$p.'`.`title.key` FROM `'.$p.'` ';
                $dbSql.='WHERE `page_langlocale`=\''.$lang.'\' AND `VISIBLE`= \'TRUE\' AND (';
                $mer ='';
                    foreach($this->actualrulls as $key=>$value){       $dbSql.=$mer.' `access`="'.$key.'"';      $mer=' OR ';
                    }
                    $dbSql.=')';
                $data=\autosystem\DBConn::GET();
                $dbStmt=$data->prepare($dbSql);
                $dbStmt->execute();
                while($dbRij=$dbStmt->fetch(\PDO::FETCH_ASSOC)){
                    $return['MenuLabel.'.$dbRij['title.key']]='page/Watch/?ThePage='.$dbRij['id'];
                }
            }
            return $return;
        }
        private function isinstalled(){
            try{
                $DB=\autosystem\DBConn::GET();
                if(isset($DB)){    $DB->query('SELECT 1 FROM `'.\autosystem\DBConn::getTableNameFor('installs').'` LIMIT 1');
                }else{             return false;
                }
            } catch(Exception $e){   return false;  }
            return true;
        }
        //<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<PREPARTS>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
        public function getdate($string,$date){
            if($date){   echo 'July 1, 2000 is on a ' . date('l', mktime(0, 0, 0,$month,$day,$year, 1, 2000)); 
            }else{       echo date('l jS \of F Y h:i:s A');  }
        }
        public function getheader($loc,$action,$values=[],$menufilter=''){
            
            $arr=$this->getheader_values($loc,$action ,$values);
            //print 'x';
            return $this->get_Templateclone('global','header',$arr);
        }
        /**
         * Qview::getheader_values()
         * 
         * @param mixed $loc is a path and plugin
         * @param mixed $action is a file and action 
         * @param mixed $values
         * @return
         */
        public function getheader_values($loc,$action,$values =[]){
            /** $path & implode to $this->installpath */
			//$path= explode('/',$_SERVER['PHP_SELF']);
            //array_pop($path);
            $return['locale']= $this->properties->getlocale();
            print('<!-- @locale '.$return['locale'].' Qview translator by autosite Qtemplatecache='.($this->cachIT?'TRUE':'FALSE').' speedup='.($this->speedup?'TRUE':'FALSE').' -->');
            $return['path.layout']= $this->getLayoutPath();
            if(file_exists('./layout/views/'.$loc.'/Astyle.css')){
                $return['location.css']= '<link rel="stylesheet" type="text/css" href="'.$return['path.layout'].'views/'.$loc.'/Astyle.css" >';
            }else{
                $return['location.css']= '';
            }
            if(file_exists('./layout/views/'.$loc.'/Astyle'.$action.'.css')){
                $return['location.css'].= '<link rel="stylesheet" type="text/css" href="'.$return['path.layout'].'views/'.$loc.'/Astyle'.$action.'.css" >';
            }
            if(file_exists('./layout/views/'.$loc.'/Ascript.js')){
                $return['location.css'].= '<script type="text/javascript"  href="'.$return['path.layout'].'views/'.$loc.'/Ascript.js" ></script>';
            }
            if(file_exists('./layout/views/'.$loc.'/Ascript'.$action.'.js')){
                $return['location.css'].= '<script type="text/javascript"  href="'.$return['path.layout'].'views/'.$loc.'/Ascript'.$action.'.js" ></script>';
            }
            $return['keywords']=implode(',',$this->keywords);            
            $title = 'title.'.$action;
            $return['title']= $this->properties->getPropertie($title);
            $host = explode('.',$_SERVER['HTTP_HOST']);
            if(isset($host[0])&& strlen($host[0])==2 ||$host[0]=='www'){
                array_shift($host);
            }
            $return['URL_SELF']= implode('.',$host ).''.$this->installpath.((isset($loc)&&isset($action))?'/'.$loc.'/'.$action.'/':'');
            if(is_array($values) && count($values)>0){
                $return = $values+$return;
            }
            //$return['title']='';//.print_r($values,true);
            $return['menutypes']= $this->getNAVselector($this->installpath.'/',$loc.'/'.$action.'/');
            $return['collapse']= ($this->mobile)?'collapse':'collapse in';
            $return['find']= $this->get_Templateclone('global','find',$this->getfind_values());
            $name = 'MENU_'.implode('.',array_keys($this->actualrulls));
            if($this->cachIT && $this->existcach('global',$name)){
                $html = $this->getcach('global',$name);
                $return['nav'] = $this->nav($loc,$action);    
            }else{
                $return['nav'] = $this->nav($loc,$action);//MENU
                $this->makecach('global',$name,$return['nav']);
            }
            $return['basic.css'] = $this->getcss();
            return $return;
        }
        private function getcss(){
            $sunrise = '7:42';
            $evening = '18:35';
            $sunset = '19:45';
            $time = date( 'H:i',time());
            $date1 = \DateTime::createFromFormat('H:i', $sunrise);
            $date2 = \DateTime::createFromFormat('H:i', $time);
            $date3 = \DateTime::createFromFormat('H:i', $sunset);
            $date4 = \DateTime::createFromFormat('H:i', $evening);
            if ($date1 < $date2 && $date2 < $date3){
                if($date2 < $date4){   $element = 'basic.css';
                }else{                 $element = 'basic_evening.css';  }
            }else{                     $element = 'basic_night.css';    }
            $segment = $this->getTimeSegment();
            if($this->existtimesegmentcssfile($segment.'_'.$element)){
				
			}
            return $element;
        }
        private function getTimeSegment(){
        	//noel , easter ...
			return '';
		}
		private function existtimesegmentcssfile($name){
			return false;
		}
        public function getform_head($loc,$action,$id=''){
            $arr = $this->getheader_values($loc,$action);
            $arr['formname']=$action;
            $arr['id']=$id;
            return $this->get_Templateclone('global','form_head',$arr);
        }
        public function getform_end($action,$submitkey=null){
            if(!isset($submitkey)){
                return $this->get_Templateclone('global','form_end',$this->getform_values($action));
            }else{
                $values = $this->getform_values($action);
                $values['submit']= $this->get_AtranslationB($submitkey);
                $values['name']= $submitkey;
                return $this->get_Templateclone('global','form_end_b',$values);
            }
        }
        /**
        * $data['paginpre']=$this->pagenr;
        $data['paginnr']=$this->pagenr;
        $data['paginerp']=($this->totalPages-$this->pagenr>$p)?$p:$this->totalPages-$this->pagenr;
        $data['paginfirst']=0;
        $data['paginprev']=($this->pagenr>0)? $this->pagenr-1:0;
        $data['canIprev']=($this->pagenr == 0 )?'disabled" disabled="disabled':'';
        $data['paginnext']=($this->totalPages > $this->pagenr)? $this->pagenr+1:$this->totalPages;
        $data['canInext']=($this->pagenr==$this->totalPages || $this->pagenr>$this->totalPages)?'disabled" disabled="disabled':'';
        $data['paginlast']=$this->totalPages;
        $data['paginatorreload']=$this->reload;
        $data['path.url']=$this->pathurl;
        */
        public function getpaginator($data,$param='',$numbers=0,$delay=0){
            if($numbers>0||$numbers>0){
                return $this->getpaginator_full($data,$param,$numbers,$delay);
            }
            $data['param']=$param;
            $p = $this->pagewindow-1;
            print $this->get_Templateclone('global','paginator_head',$data);
            $data['class']='selectedpointer';
            print $this->get_Templateclone('global','paginator_item',$data);
            print $this->get_Templateclone('global','paginator_foot',$data);
        }
        private function getpaginator_full($data,$param,$numbers,$delay){
            $data['param']=$param;
            $x = $data;
            $x['class']='notselect';
            print $this->get_Templateclone('global','paginator_head',$data);
            for ($i = 300; $i < 0; $i-100) {
                if($x['paginnr']-$i>0){
                    $x['paginnr'] = $data['paginnr']-($i*100);       print $this->get_Templateclone('global','paginator_item',$x);
                }
            }
            if($x['paginnr']-30>0){
                for ($i = 3; $i > 0; $i--) {
                    $x['paginnr'] = $data['paginnr']-($i*10);       print $this->get_Templateclone('global','paginator_item',$x);
                }
            }
            if($x['paginnr']-3>0){
                for ($i = 3; $i > 0; $i--) {
                    $x['paginnr'] = $data['paginnr']-$i;       print $this->get_Templateclone('global','paginator_item',$x);
                }
            }
            
            $data['class']='selectedpointer';
            print $this->get_Templateclone('global','paginator_item',$data);
            if($data['paginlast']-$x['paginnr']-3>1){
                for ($i = 1; $i <= 3; $i++) {
                    $x['paginnr'] = $data['paginnr']+$i;       print $this->get_Templateclone('global','paginator_item',$x);
                }
            }
            if($data['paginlast']-$x['paginnr']-30>10){
                for ($i = 1; $i <= 3; $i++) {
                    $x['paginnr'] = $data['paginnr']+($i*10);  print $this->get_Templateclone('global','paginator_item',$x);
                }
            }
            if($data['paginlast']-$x['paginnr']-300>100){
                for ($i = 1; $i <= 3; $i++) {
                    $x['paginnr'] = $data['paginnr']+($i*100);  print $this->get_Templateclone('global','paginator_item',$x);
                }
            }
            print $this->get_Templateclone('global','paginator_foot',$data);
        }
        public function getfooter($action){
            return $this->get_Templateclone('global','footer',$this->getfooter_values($action));
        }
        private function getfooter_values($action){
            $return['layoutpath']=$this->installpath.'/layout';
            $return['Y']=date('Y');
            global $_AUTOSITEVERSIONNR ;
            $return['version']=(string)$_AUTOSITEVERSIONNR;
            $return['servicemail']='info@'.$_SERVER['HTTP_HOST'];
            $return['servicenr']='telephone @Qview'.__LINE__;
            $return['error']=' ';
            $return['errorclass']=' hidden';
            $keyboard = new Keyboard();
            $return['keyboard']= $keyboard->render($this);
            return $return;
        }
        private function array_keyfinder($array,$char='->' ){
            $return = [];
            foreach($array as $val){
                if(strpos($val,$this->delimiter)){
                    $subarray =explode($this->delimiter,$val);
                    $i=0;
                     foreach($subarray as $sval){
                        if($i%2==0 && !strpos($sval,$char)){ array_push($return,$sval);  }
                        $i++;
                     }
                }else{
                    if(!strpos($val,$char)){                 array_push($return,$val);     }
                }
            }
            return $return;
        }
        public function getCheckImg($bool=false){
            if(!isset($this->select)||!isset($this->notselect)){
                 try{
                    $this->select =$this->properties->getPropertie('select');
                    $this->notselect =$this->properties->getPropertie('notselect');
                 }catch(exception $e){
                 }
            }
            $pic = ($bool)?'c/check.svg':'c/cross.svg';
            $pic =$this->getimgpath().'icons/'.$pic;
            if($bool){   return '<img src="'.$pic.'" alt="'.$this->select.'" width="30" height="30" />'; }
            return '<img src="'.$pic.'" alt="'.$this->notselect.'" width="15" height="15" />';
        }
        public function newForm($loc,$action,$name,$templatename,&$values,$submit,$options){
            $return = $this->getform_head($loc,$action);
            $return .= $this->get_template($loc,$templatename,$values);
            $return .= $this->getform_end($submit,$submit);
            return $return;
        }
        public function getOptions($items,$keyselected=''){
            $return='';            
            if(isset($items)){
                if(isset($this->enum_translations)){
                    if(is_array($items)){
                       foreach($items as $key=>$value){
                            if( key_exists($key,$this->enum_translations)){
                                $return.='<option value="'.$key.'"'.($key==$keyselected?' selected':'').' > '.$this->enum_translations[$key].' </option>';
                            }else {
                                $return.='<option value="'.$key.'"'.($key==$keyselected?' selected':'').' > '.$value.' </option>';
                            }
                       }
                    }

                }else{
                    foreach($items as $key=>$value){
                        $return.='<option value="'.$key.'"'.($key==$keyselected?' selected':'').' > '.$value.' </option>';
                    }
                }
            }
            return $return;
        }
        private function get_liLinks($baseurl,$items,$keyselected='',$attributes=''){
            $return='';            
            if(isset($this->enum_translations)){
                foreach($items as $key=>$value){
                    $return.=$this->li($baseurl.$key.$attributes,( key_exists($key,$this->enum_translations)?$this->enum_translations[$key]:$value),$key.'.svg', ($key == $keyselected));
                }
            }else{
                foreach($items as $key=>$value){
                    $return.=$this->li($baseurl.$key.$attributes,$value,$key.'.svg',($key==$keyselected));
                }
            }
            return $return;
        }
        private function li($href,$label,$img, $isselect = false){
            $icon = '<img src="'.$this->getimgPath().'icons/'.$img.'" alt="'.$label.'" width="30" height="30" />';
            return'<li><a href="'.$href.'" class="'.($isselect?'selected':'').'" >'.$icon.'<span class="hidden-xs" >'.$label.'</span></a></li>';
        }
        public function getimgPath(){
            if(isset($this->imgpath)){         return $this->imgpath;                       }
            //print $this->installpath.'/layout/img/';
            $this->imgpath = $this->installpath.'/layout/img/';
            return $this->imgpath;
        }
        public function getinstallpath(){       return $this->installpath;                  }
        public function getLayoutPath(){        return $this->installpath.'/layout/';       }
        public function addkeywords($keywords){ array_push($this->keywords,$keywords);      }
        public function getLocale(){            return $this->properties->getlocale();      }
        public function getImageSize(){         return 150;        }
        private function getScriptPath(){       return $this->installpath.'/layout/js/';    }
        private function getistat_values(){     return ['title'=>''];        }
        public function getAllRulles(){
			if(key_exists('KING',$this->actualrulls)){
				return $this->modeltypes;            
			}else{
				return ['PUBLIC' => 'public'];
			}
		}
	}
?>