<?php
/**
 * @author Lieven Roegiers
 * @copyright $Y$
 * @CMS autosite
*/
/*
watch to simple stored procedure in MySQL database:
DELIMITER $$
CREATE DEFINER=`vidhu`@`%` PROCEDURE `test`(var_datain TEXT)
BEGIN
    SELECT var_datain;
END
*/
namespace start;
class Install_model extends \autosystem\Model{
    public $menu = ['FALSE'=>'0','TRUE' => '1'];
    public $public = ['FALSE'=>'0','TRUE' => '1'];
    public $states = array('PROGRESS' => 'progress','ERROR' => 'error','WAIT' => 'wait','CANCELED' => 'canceled','OK' => 'ok');   
    public $versionactual ;
    private $ingoredlist=array();
    private $incpath = '';
    static private $isinstalled = false;
    public function __construct(){
        global $_AUTOSITEVERSIONNR ;
        $this->versionactual = $_AUTOSITEVERSIONNR;
        parent::__construct();
        $this->isinstalled();
    }
    public function save($locale,$key ,$value){
    }
    public function exist($locale,$key){
    }
    public function setincludeslocation($location){
        $this->incpath = $location;
    }
    public function getModelInfo(){   return 'Install.model.info'; }
    public function isInstalled(){
        return self::$isinstalled|| isset($this->DB)&&  $this->existTableLabel('installs')&& self::$isinstalled=true;
    }
    public function isInMenu($key,$urlloc){
        if(!self::$isinstalled){return false;}
        $database = \autosystem\DBConn::GET();
        $dbStmt = $database->prepare('SELECT * FROM '.\autosystem\DBConn::getTableNameFor('installs').' WHERE `location`=:location AND `key`=:key ');
        $dbStmt->bindParam(':location',$urlloc);
        $dbStmt->bindParam(':key',$key);
        $dbStmt->execute();
        //print 'SELECT * FROM '.\autosystem\DBConn::getTableNameFor('installs').' WHERE `location`=\''.$urlloc.'\' AND `key`=\''.$key.'\' ';
        return (bool)$dbStmt->fetch(\PDO::FETCH_ASSOC);
    }
    public function getVersionNr(){
        return (float)$this->versionactual;
    }
    public function __install(){
        $database = \autosystem\DBConn::GET();
        $dbSql = 'CREATE TABLE IF NOT EXISTS `'.\autosystem\DBConn::getTableNameFor('installs').'` ('.
                      '`id` INT(50) NOT NULL AUTO_INCREMENT,'.
                      '`key` varchar(50) COLLATE '.\autosystem\DBConn::getCOLLATE().' NOT NULL UNIQUE,'.
                      '`location` varchar(250) COLLATE '.\autosystem\DBConn::getCOLLATE().' NOT NULL,'.
                      '`STATUS` ENUM(\'PROGRESS\',\'ERROR\',\'WAIT\',\'CANCELED\',\'OK\')NOT NULL ,'.
                      '`MENU` ENUM(\'FALSE\',\'TRUE\')NOT NULL ,'.
                      '`PUBLIC` ENUM(\'FALSE\',\'TRUE\')NOT NULL ,'.
                      '`TYPE` ENUM(\''.implode('\',\'',array_keys($this->modeltypes)).'\') default \'NULL\' ,'.
                      '`imputDate` datetime NOT NULL,'.
                      '`version` decimal(3,2) COLLATE '.\autosystem\DBConn::getCOLLATE().' NOT NULL DEFAULT \'0.00\','.
                      '`refMenuKey` varchar(50) COLLATE '.\autosystem\DBConn::getCOLLATE().','.
                      'PRIMARY KEY (`id`)'.
                    ') ENGINE=InnoDB DEFAULT CHARSET='.\autosystem\DBConn::getCHARSET().' COLLATE='.\autosystem\DBConn::getCOLLATE().';';
        $dbResultaat = $database->query($dbSql);
    }
    public function upgradeModule($modelname){
        try{
            if($model = new $modelname()){
                $key = $model->getModelInfo();
                if($this->existInstalledItem($key)){
                    //print '<span class="ok" >exist'.$key.'</span><br>';
                    $model->__upgrade($this->versionactual);
			        $this->UpdateVersionNr($model->getVersionNr(),$model->getModelInfo());
                }else {
                    // print '<span class="fail" >notexist'.$key.'</span><br>';
                }
            }
        }catch(Exception $e){
            //print_r($e);
            return ;
        }   
        
    }
    private function startUpgrade($location,$installversion){
        $locArr = explode('/',$location);
        $modelname = '\\'.$locArr[0].'\\'.$locArr[1].'_model';
        print  implode( '\\',getRealinstall()).$modelname;
        if(!$this->isModelFileExist($locArr[0],$locArr[1])){           //print 'model not exist';
            return false;
        }
        $cmsversion = $this->getVersionNr();
		if($model = new $modelname()){
		    if($cmsversion > $installversion){
				$this->UpdateVersionNr($model->getVersionNr(),$model->getModelInfo());
            }
            return true;
        }else{
            print 'startUpgrade instance error';
        }
    }
    private function isModelFileExist($location,$action){
        $file = '.\\'.$location.'\\'.$action.'_model';
        return is_ClassExist($file);
    }
    public function UpdateVersionNr($version,$modelinfo){ 
        $i = \autosystem\DBConn::getTableNameFor('installs');
        $key = 'UPDATE `'.$i.'` SET `version`=:version WHERE `key`=:key';
        $value = [':version'=>$version,':key'=>$modelinfo];
        $sql[$key]= $value;
        return $this->transaction($sql); 
    }
    public function GET_InstallAsArray($key){
       
       $i = \autosystem\DBConn::getTableNameFor('installs'); 
       $sql='SELECT * FROM '.$i.' WHERE `key`=:key ';
       return parent::GET_Stmt($sql,[':key'=>$key]);
    }
    public function GET_InstalledAsArray(){
    	$i = \autosystem\DBConn::getTableNameFor('installs'); 
        $sql='SELECT * FROM '.$i.'';
        print($sql);
       return parent::GET_Stmt($sql,[]);
    }
    public function existInstalledItem($key){
        try{
           $sql ='SELECT `id` FROM '.\autosystem\DBConn::getTableNameFor('installs').' WHERE `key`=:key ';
           $value = [':key'=>$key];
            return $this->isQuery($sql,$value);
        }catch(Exception $e){
            return false;
        }
    }
    public function isMenuPublic($key,$urlloc){
        if(!self::$isinstalled){return false;}
        $sql ='SELECT `id` FROM '.\autosystem\DBConn::getTableNameFor('installs').' WHERE `MENU`=\'TRUE\' AND `location`=:location AND `key`=:key ';
        $value = [':location'=>$urlloc,':key'=>$key];
        return $this->isQuery($sql,$value);
    }
    public function addInstalledItem($key,$location ,$status,$refMenuKey='NULL',$type='NULL',$version ='0.00'){
        if($this->existInstalledItem($key)){        return false;}
        $i =\autosystem\DBConn::getTableNameFor('installs');
        $sql = 'INSERT INTO '.$i.'(`id`,`key`,`location`,`STATUS`,`MENU`,`PUBLIC`,`refMenuKey`,`TYPE`,`imputDate`,`version`)
                     VALUES(NULL,:key,:location,:status,:menu,:public,:refmenu,:type,:imputDate,:version)';
        $inmenu = 'FALSE';
        $ispublic ='FALSE';
        $database = \autosystem\DBConn::GET();
        $dbStmt = $database->prepare($sql);
        $value = [':key'=>$key,':location'=>$location,':status'=>$status,':menu'=>$inmenu,':public'=>$ispublic,
                  ':refmenu'=>$refMenuKey,':type'=>$type,':imputDate'=>$this->nowDateTime(),':version'=>number_format($version, 2, '.', '')
        ];
        try{
            return $dbStmt->execute($value);
        }catch(Exception $e){
            return false;
        }
    }
    public function existTYPE($TYPE){
        return isset($TYPE)&& array_key_exists($TYPE,$this->modeltypes);
    }
    public function setTYPE($location,$TYPE){
        if(!$this->existTYPE($TYPE)){            return false;        }
        $i =\autosystem\DBConn::getTableNameFor('installs');
        $key = ' UPDATE '.$i.' SET `TYPE`=:type WHERE `location`=:location ';
        $sql[$key] = [':location'=>$location,':type'=>$TYPE];
        return parent::transaction($sql);
    }
    public function GET_ASiteMapOverView($location,$templatename,&$view,$type){
        $i = \autosystem\DBConn::getTableNameFor('installs');
        $sql='SELECT `'.$i."`.* ,SUBSTRING_INDEX(`location`, '/', 1) AS `loc`,SUBSTRING_INDEX(SUBSTRING_INDEX(`location`, '/', -2), '/', 1) AS `module`, IF(`MENU`='TRUE',' checked=checked ',' ') AS `isinmenu`".
        'FROM `'.$i.'`WHERE `TYPE`=\''.$type.'\' ORDER BY `imputDate`';
        parent::GET_Summary($location,$templatename,$view,$sql);
    }
    static public function dbinstall($fname){
        $config['PROTECTION'] = '<?php /*<!--';
        $config['HOST']=(isset( $_POST['HOST'] )) ? addslashes($_POST['HOST'] ) : '' ;
        $config['DATABASE']=(isset($_POST['DATABASE'] )) ? addslashes($_POST['DATABASE'] ) : '' ;
        $config['USER']=(isset($_POST['USER'] )) ? addslashes($_POST['USER'] ) : '' ;
        $config['PASSWORD']=(isset($_POST['PASSWORD'] )) ? addslashes($_POST['PASSWORD'] ) : '' ;
        $config['SUPERUSER']=(isset($_POST['USER'] )) ? addslashes($_POST['USER'] ) : '' ;
        $config['SUPERPASSWORD']=(isset($_POST['PASSWORD'] )) ? addslashes($_POST['PASSWORD'] ) : '' ;
        $config['ADMINUSER']=(isset($_POST['USER'] )) ? addslashes($_POST['USER'] ) : '' ;
        $config['ADMINPASSWORD']=(isset($_POST['PASSWORD'] )) ? addslashes($_POST['PASSWORD'] ) : '' ;
        $config['CHARSET']='utf8';
        $config['COLLATE']='utf8_unicode_ci' ;
        $config['PROTECTIONI'] = '-->*/';
        if(!self::$isinstalled && isset($_POST['save'])){
           $conn = 'mysql:host='.$config['HOST'].';dbname='.$config['DATABASE'];
           try{
                //print_r($_POST);
                /**
				CREATE USER /''.$config['NEWUSER'].'/'@'%' IDENTIFIED VIA mysql_native_password USING /''.$config['NEWPAS'].'/';
				GRANT ALL PRIVILEGES ON *.* TO /''.$config['NEWUSER'].'/'@'%' REQUIRE NONE WITH GRANT OPTION 
				MAX_QUERIES_PER_HOUR 0 
				MAX_CONNECTIONS_PER_HOUR 0 
				MAX_UPDATES_PER_HOUR 0 
				MAX_USER_CONNECTIONS 0;
				GRANT ALL PRIVILEGES ON `'.$config['dbname'].'`.* TO /''.$config['NEWUSER'].'/'@'%'; 
				*/
				/**
				REVOKE ALL PRIVILEGES ON *.* FROM /''.$config['NEWUSER'].'/'@'%';
				GRANT SELECT, INSERT, UPDATE, DELETE, CREATE, DROP, RELOAD, SHUTDOWN, PROCESS, FILE, REFERENCES, INDEX, ALTER, SHOW DATABASES, SUPER, CREATE TEMPORARY TABLES, LOCK TABLES, REPLICATION SLAVE, REPLICATION CLIENT, CREATE VIEW, EVENT, TRIGGER, SHOW VIEW, CREATE ROUTINE, ALTER ROUTINE, 
				EXECUTE ON *.* TO /''.$config['NEWUSER'].'/'@'%' REQUIRE NONE WITH GRANT OPTION 
				MAX_QUERIES_PER_HOUR 0 
				MAX_CONNECTIONS_PER_HOUR 0 
				MAX_UPDATES_PER_HOUR 0 
				MAX_USER_CONNECTIONS 0; 
				*/
                $dbConn = new \PDO($conn, $config['USER'],$config['PASSWORD']);
                if($dbConn){
                    self::write_dbfile($fname, $config);
                    $Ginstall = new \start\Install_model();
                    $Ginstall->__install();
                    $Ginstall->addInstalledItem($Ginstall->getModelInfo(),'start/Install/','OK','NULL',$Ginstall->getRuleDefault(),$Ginstall->getVersionNr());
                    $Ginstall->SetMenuPublic('start/Install/',true);
                    $install = new \translator\Translate_model();
                    $install->__install();
                    $Ginstall->addInstalledItem($install->getModelInfo(),'translator/Translate/','OK','NULL',$install->getRuleDefault(),$install->getVersionNr());
                    $install = new \images\Gallery_model();
                    $install->__install();
                    $path = explode('/',htmlspecialchars($_SERVER['PHP_SELF']));
					array_pop($path);            
            		$imgups = implode('/',$path).'';
                    mkdir($imgups.'/gallery',644);
                    mkdir($imgups.'/page',644);
                    mkdir($imgups.'/globalSite',644);
                    mkdir($imgups.'/Amail',644);
                    $Ginstall->addInstalledItem($install->getModelInfo(),'images/Gallery/','OK','NULL',$install->getRuleDefault(),$install->getVersionNr());
                    $install = new \images\GalleryManage_model();
                    $install->__install();
                    $install->save('gallery','gallery','PUBLIC','0','1');
					$install->save('page','page','PUBLIC','0','1');
					$install->save('Amail','Amail','USER','0','1');
					$install->save('globalSite','globalSite','MODERATOR','0','1');                   
                    $Ginstall->addInstalledItem($install->getModelInfo(),'images/GalleryManage/','OK','NULL',$install->getRuleDefault(),$install->getVersionNr());
                    //save($categoryname,$categorylabel,$access,$user_id,$sub_Category='NULL')
					$install = new \page\Watch_model();
                    $install->__install();
                    $Ginstall->addInstalledItem($install->getModelInfo(),'page/Watch/','OK','NULL',$install->getRuleDefault(),$install->getVersionNr());
                    $install = new \users\Profile_model();
                    $install->__install();
                    $Ginstall->addInstalledItem($install->getModelInfo(),'users/Profile/','OK','NULL',$install->getRuleDefault(),$install->getVersionNr());
                    $install = new \users\Login_model();
                    $install->__install();
                    $Ginstall->addInstalledItem($install->getModelInfo(),'users/Login/','OK','NULL',$install->getRuleDefault(),$install->getVersionNr());
                    $install = new \users\Logout_model();
                    $install->__install();
                    $Ginstall->addInstalledItem($install->getModelInfo(),'users/Logout/','OK','NULL',$install->getRuleDefault(),$install->getVersionNr());
                    $install = new \users\Register_model();
                    $install->__install();
                    $install->save($config['USER'],$config['PASSWORD'],'info@',$config['USER'].'KING');
                    $Ginstall->addInstalledItem($install->getModelInfo(),'users/Register/','OK','NULL',$install->getRuleDefault(),$install->getVersionNr());
                    $install = new \users\Activate_model();
                    $install->__install();
                    $install->activateThis($config['USER'].'KING',true);
                    $user_id =  $install->getUserid($config['USER'].'KING');
                    $Ginstall->addInstalledItem($install->getModelInfo(),'users/Activate/','OK','NULL',$install->getRuleDefault(),$install->getVersionNr());
                    $install = new \users\ModUsers_model();
                    $install->__install();
                    $install->accoundEnabled($config['USER'].'KING',true);
                    $amodels=[];
					foreach($install->modeltypes as $amodel=>$ainfo){
						$amodels[$amodel] = true;
					}
                    $install->addRules($user_id,$amodels);
                    $Ginstall->addInstalledItem($install->getModelInfo(),'users/ModUsers/','OK','NULL',$install->getRuleDefault(),$install->getVersionNr());
                    $install = new \users\UserStats_model();
                    $install->__install();
                    $Ginstall->addInstalledItem($install->getModelInfo(),'users/UserStats/','OK','NULL',$install->getRuleDefault(),$install->getVersionNr());
                    $install = new \users\ContactInfo_model();
                    $install->__install();
                    $Ginstall->addInstalledItem($install->getModelInfo(),'users/ContactInfo/','OK','NULL',$install->getRuleDefault(),$install->getVersionNr());
                    $install = new \adres\Location_model();
                    $install->__install();
                    $Ginstall->addInstalledItem($install->getModelInfo(),'users/Location_model/','OK','NULL',$install->getRuleDefault(),$install->getVersionNr());
                    $Ginstall->setToMenu('users/Login/','true');
                    $Ginstall->setToMenu('users/Logout/','true');
                    $Ginstall->setToMenu('start/Install/','true');
                    //$Ginstall->readsql('includes/sqls/database.sql');
                    //mkdir($this->installpath.$this->userspath.'/');
                }
            }catch(Exception $ex){
                return false;
            }
        }
        $view = implode(DIRECTORY_SEPARATOR, array('.','layout','views','global','connect'.'.Qtemplate'));
        $html =  \autosystem\Properties::get_Qtemplate('global','connect');
        $temp= new \autosystem\Qtemplate($html);
        $temp->setvalues($config);
        print $temp->gethtml();
    }
    public function readsql($file){
       $import = new \data\SqlImport();
  	   $import->importFile($file,false,true);
    }
    static private function write_dbfile($config,$assoc_arr){
        $content = ''; 
        foreach ($assoc_arr as $key=>$elem){
            $content .= ''.$key.' = "'.$elem.'"'." \n";
        }
        //print $content;
        if (!$handle = fopen($config, 'w')) { 
            return false;
        }
        $success = fwrite($handle, $content);
        fclose($handle);
        return $success; 
    }
    public function setToMenu($urlloc,$inmenu='true'){
        $inmenu = ($inmenu=='true')?'TRUE':'FALSE';
        $sql = ' UPDATE '.\autosystem\DBConn::getTableNameFor('installs').' SET `MENU`=\''.$inmenu.'\' WHERE `location`=:location ';
        $database = \autosystem\DBConn::GET();
        $dbStmt = $database->prepare($sql);
        $dbStmt->bindParam(':location',$urlloc);
        return  $dbStmt->execute();
    }
    
    public function SetMenuPublic($urlloc,$public='true'){
        $public = ($public=='true')?'TRUE':'FALSE';
        $sql = ' UPDATE '.\autosystem\DBConn::getTableNameFor('installs').' SET `PUBLIC`=\''.$public.'\' WHERE `location`=:location ';
        $database = \autosystem\DBConn::GET();
        $dbStmt = $database->prepare($sql);
        $dbStmt->bindParam(':location',$urlloc);
        return  $dbStmt->execute();
    }
    public function GET_AOverView($location,$templatename,&$view){//wrench.svg
        $i = \autosystem\DBConn::getTableNameFor('installs');
        $sql='SELECT IF(`'.$i.'`.`version`<\''.$this->versionactual.'\',\' NEEDUPGRADE\',\' COMPACTIBLE\') AS `upgradelayout`, IF(`'.$i.'`.`version`<\''.$this->versionactual.'\',\' checked=checked \',\' \') AS `mustupgrade`,`'.$i.'`.* FROM `'.$i.'`';
        if(!empty($category_id)){
            $sql.= ' WHERE `category_id`=\''.$category_id.'\''.((!empty($accound_id))?' && `accound_id`=\''.$accound_id.'\'':'');
        }
        $sql.=' ORDER BY `type`';
        //print $sql;
        $this->GET_OverViewPage($location,$templatename,$view,$sql);
    }
    public function GET_AInstall($location,$templatename,&$view,$id){//wrench.svg
        $i = \autosystem\DBConn::getTableNameFor('installs');
        $sql='SELECT IF(`'.$i.'`.`version`<\''.$this->versionactual.'\',\' checked=checked \',\' \') AS `mustupgrade`,`'.$i.'`.* FROM `'.$i.'`';
        $sql.= ' WHERE `id`=\''.$id.'\'';
        //print $sql;
        $this->GET_OverViewPage($location,$templatename,$view,$sql);
    }
    public function GET_OverViewPage($templateLocation,$templateName,&$view,$sql,$param=null){
            //$view->setENUM_TRANSLATIONS(['TRUE'=>' checked','FALSE'=>' ']);//TODO WATCH overrull text
            try{
                if(\autosystem\ValidateAs::query($sql)&&($dbStmt = $this->DB->prepare($sql))&&$dbStmt->execute($param)){
                    $res = $dbStmt->fetchAll(\PDO::FETCH_ASSOC);
                    $i=0;
                    foreach ($res as $row){
                        $row['row']= ($i++%2==0)?'equally':'odd';
                        $row['layoutpath']= $view->getimgPath();
                        $row['types'] = $view->getOptions($this->modeltypes,$row['TYPE']);
                        $row['key_translated']=$view->get_AtranslationB($row['key']);
                        $e = explode('/',$row['location']);
                        $row['location']=$e[0];
                        $row['name']=$e[1];
                        //[%location->value%][%name->value%]
                        print $view->get_Templateclone($templateLocation,$templateName,$row);
                    }
                    if(isset($param['paginator.page'])&&isset($param['paginator.limit'])){
                        $this->GET_Paginator();
                    }
                    return true;     
                }else{
                    return false;
                }
            }catch (PDOException $e) {
               die($e);
            }
    }
    public function setSTATUS($STATUS,$modelinfo){ 
        $i = \autosystem\DBConn::getTableNameFor('installs');
        $key = 'UPDATE `'.$i.'` SET `STATUS`=:status WHERE `key`=:key';
        $value = [':status'=>$STATUS,':key'=>$modelinfo];
        $sql[$key]= $value;
        //print \autosystem\DEBUG::printTransaction($sql);
        return $this->transaction($sql); 
    }
    public function getRuleDefault(){        return 'ADMINISTRATION';    }  
}
?>