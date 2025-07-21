<?php
namespace autosystem;
class Browser {
    protected $browser;
    protected $typesA=[  'AIR'=>'AIR','AOL'=>'AOL',
                         'Konqueror'=>'Konqueror','Dalvik'=>'Dalvik',
                         'Netscape'=>'Netscape','Safari'=>'Apple Safari',
                         'Opera'=>'Opera','PaleMoon'=>'PaleMoon',
                        ];
    protected $typesB=[  'Avant'=>'Avant','AIR'=>'AIR','AOL'=>'AOL','Amaya'=>'Amaya',
                         'Camino'=>'Camino','Edge'=>'Edge',  'Gecko'=>'Gecko',
                         'Flock'=>'Flock','Fluid'=>'Fluid',
                         'Lynx'=>'Lynx','Mosaic'=>'Mosaic',
                         'MSIE'=>'Internet Explorer',
                         'Navigator'=>'Navigator','Netscape'=>'Netscape',
                         'Safari'=>'Apple Safari','Seamonkey'=>'Seamonkey',
                         'Omniweb'=>'Omniweb','Opera'=>'Opera',
                        ];
//bad browsers
    protected $typesC=[   'Firefox'=>'Mozilla Firefox','mozilla'=>'mozilla'];
    protected $typesD=[   'Chrome'=>'Google Chrome'             
	    ];
    private $spiders=[ 'mail.ru'=>'BOT ru mail','Googlebot'=>'BOT google','SemrushBot'=>'BOT Semrush',
                       'Baiduspider'=>'BOT Baidu', 'bingbot'=>'BOT Bing','KomodiaBot '=>'BOT Komodia',  
                       'Vagabondo'=>'BOT Vagabondo','AhrefsBot'=>'BOT Hrefs', 
                    ];
    private $blocked=[ 'Website-audit'=>'bullshit','facebook'=>'spy','python-requests'=>'hack',
                       'ia_archiver'=>'hack','netsystemsresearch'=>'bullshit','expanseinc'=>'bullshit',
                       'netEstate'=>'spy',
                       
                    ];
    private $agents =[   'win'=>'Windows',
                         'mac'=>'MacIntosh',
                         'linux'=>'Linux',
                         'OS/2'=>'OS/2',
                         'BeOS'=>'BeOS'
                        ];
    private $isBOT = false;
    private $log = false;
    private $hidden = false;
    private $ip;
    public function __construct() {
        //print $_SERVER["HTTP_USER_AGENT"];
        if(isset($_SERVER)&& array_key_exists('HTTP_USER_AGENT',$_SERVER)&&($_SERVER['HTTP_USER_AGENT'] != '')) {
            $this->browser = $this->checkbrowser($_SERVER['HTTP_USER_AGENT']);
        }else {
            $this->browser = '!notexist!';
        }
        $this->ip = $this->getIp();
    }
    private function checkbrowser($user_agent){
        if($this->isBlocked($user_agent)) return '';
        foreach($this->typesA as $type=>$name){
            if(preg_match('/'.$type.'/i', $user_agent)){
                return $name;
            }
        }
        foreach($this->typesB as $type=>$name){
            if(preg_match('/'.$type.'/i', $user_agent)){
                return $name;
            }
        }
        foreach($this->typesC as $type=>$name){
            if(preg_match('/'.$type.'/i', $user_agent)){
                return $name;
            }
        }
        foreach($this->spiders as $spider=>$name){
            if(preg_match('/'.$spider.'/i', $user_agent)){
                $this->isBOT = true;
                return $name;
            }
        }
        foreach($this->typesd as $type=>$name){
            if(preg_match('/'.$type.'/i', $user_agent)){
                return $name;
            }
        }
        $this->logbrowser();
        return '!notfound!';
    }
    private function isInject(){
        /*if('/DriverMysql/i'){
            @trigger_error(__FILE__.':'.__LINE__.'Browser Injection !', E_USER_WARNING);
        }*/
    }
    private function isBlocked($user_agent){
        foreach($this->blocked as $block=>$name){
            if(preg_match('/'.$block.'/i', $user_agent)){
                $this->hidden = true;
                return true;
            }
        }
    }
    public function isSignBlocked(){
        return $this->hidden;
    }
    public function __toString() {
        return 'browser='.$this->browser;
    }
    public function logbrowser(){
        $data['browser']=$_SERVER['HTTP_USER_AGENT'];
        $logfile = new \autosystem\LogFile('./log/','browsers.log');
        $logfile->save_line($data);
    }
    public function getType(){
        return $this->browser;
    }
    public function getIp() {
        if(isset($this->ip))return $this->ip;
        $ip = false;        
        if (!empty($_SERVER['HTTP_CLIENT_IP'])) {
            $ip = $_SERVER['HTTP_CLIENT_IP'];
        }elseif (!empty($_SERVER['HTTP_X_FORWARDED_FOR'])) {
            $ip = $_SERVER['HTTP_X_FORWARDED_FOR'];
        }elseif(!empty($_SERVER['REMOTE_ADDR'])){
            $ip = $_SERVER['REMOTE_ADDR'];
        }
        if(($seperator = strpos($ip, ',')) > 0){
           return substr($ip, 0, ($seperator - 1));
        }else{
           return $ip ;
        }
    }
    private function isvalidIp($ip){
        
    }
    public function gethostbyip(){
        return gethostbyaddr($this->ip);
    }
    public function isCrime(){
      $crimes = ['BlackWidow','Bot','ChinaClaw','Custo','DISCo','Download Demon','eCatch','EirGrabber',
      'EmailSiphon','EmailWolf','Express WebPictures','ExtractorPro','EyeNetIE','FlashGet','GetRight',
      'GetWeb!','Go!Zilla','Go-Ahead-Got-It','GrabNet','Grafula','HMView','HTTrack','Image Stripper',
      'Image Sucker','Indy Library','InterGET','Internet Ninja','JetCar','JOC Web Spider','larbin', 
      'LeechFTP','Mass Downloader','MIDown tool','Mister PiX','Navroad','NearSite','NetAnts','NetSpider',
      'Net Vampire','NetZIP','Octopus','Offline Explorer','Offline Navigator','PageGrabber','Papa Foto',
      'pavuk','pcBrowser','RealDownload','ReGet','SiteSnagger','SmartDownload','SuperBot','SuperHTTP',
      'Surfbot','tAkeOut','Teleport Pro','VoidEYE','Web Image Collector','Web Sucker','WebAuto','WebCopier',
      'WebFetch','WebGo IS','WebLeacher','WebReaper','WebSauger','Website eXtractor','Website Quester',
      'WebStripper','WebWhacker','WebZIP','Wget','Widow','WWWOFFLE','Xaldon WebSpider','Zeus'];
    }
}
?>