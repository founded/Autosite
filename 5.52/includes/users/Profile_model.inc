<?php
 /**
 * @author Lieven Roegiers
 * @copyright  
 * @CMS autosite
 */
namespace users;
class Profile_model extends \autosystem\Model{
    public $states = array('ACTIVE' => 'active','INACTIVE' => 'inactive','ERROR' => 'on error');
    public $positions = array('INSIDER' => 'insider','OUTSIDER' => 'outsider');
    public $eventtype = array('LOGIN' => 'login','LOGOUT' => 'outsider','HIYJACKIN' => 'hotlink','HOTLINK' => 'hotlink','MYSQLINJECTION' => 'injection','CRIME' => 'crime','VISITOR' => 'visitor');
    private $eventextendor;
    protected $isinstalled = false; 
    public function __install(){
        //IPv6 (39 bytes) :              ABCD:ABCD:ABCD:ABCD:ABCD:ABCD:ABCD:ABCD
        //IPv4-mapped IPv6 (45 bytes) :(chars46)  ABCD:ABCD:ABCD:ABCD:ABCD:ABCD:192.168.158.190
        //by wrong transform from 1byte/1char to 4byte/1char and security varchar 230
        if(!$this->existTableLogins()){
            $sql = 'CREATE TABLE IF NOT EXISTS `'.\autosystem\DBConn::getTableNameFor('user_logins').'`(
                    `id`    INT(10) NOT NULL AUTO_INCREMENT,
                    `accound_id`  INT(20)UNSIGNED ZEROFILL,
                    `ip`  varchar(230) COLLATE '.\autosystem\DBConn::getCOLLATE().' NOT NULL,
                    `lastlocation`  varchar(230) COLLATE '.\autosystem\DBConn::getCOLLATE().' NOT NULL,
                    `browser`  varchar(20) COLLATE '.\autosystem\DBConn::getCOLLATE().' NOT NULL,
                    `time`  timestamp NOT NULL,
                     PRIMARY KEY (`id`)
                )ENGINE = InnoDB DEFAULT CHARSET='.\autosystem\DBConn::getCHARSET().' COLLATE='.\autosystem\DBConn::getCOLLATE().';';
            $result1 = $this->DB->query($sql);
            $return1 = $result1->rowCount()==1;
        }else{
            $return1 = true;
        }
        if(!$this->existTableUser()){
            $sql= 'CREATE TABLE IF NOT EXISTS `'.\autosystem\DBConn::getTableNameFor('user').'`('.
                '`id` INT(10) UNSIGNED ZEROFILL NOT NULL AUTO_INCREMENT,'.
                '`accound_id` INT(20)ZEROFILL,'.
                '`TYPE` ENUM(\''.implode('\',\'',array_keys($this->positions)).'\') NOT NULL,'.
                '`STATUS` ENUM(\''.implode('\',\'',array_keys($this->states)).'\')NOT NULL  default \'ACTIVE\','.
                '`name`  varchar(100) COLLATE '.\autosystem\DBConn::getCOLLATE().' NOT NULL,'.
                '`rule_id`  INT(10) ZEROFILL default 0,'.
                '`contact_id`  INT(20) default 0,'.
                '`location_id`  INT(9) default 0,'.
                '`user_path`  varchar(230) COLLATE '.\autosystem\DBConn::getCOLLATE().' NOT NULL,'.
                '`lastonline` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,'.
                'PRIMARY KEY (`id`)'.
            ')ENGINE = InnoDB DEFAULT CHARSET='.\autosystem\DBConn::getCHARSET().' COLLATE='.\autosystem\DBConn::getCOLLATE().';';
            $result2 = $this->DB->query($sql);
            $return2 = $result2->rowCount()==1;
        }else{
            $return2 = true;
        }
        if(!$this->existTableUser_rull()){
            $sql= 'CREATE TABLE IF NOT EXISTS `'.\autosystem\DBConn::getTableNameFor('user_rull').'`(
                `id` INT(10) UNSIGNED ZEROFILL NOT NULL AUTO_INCREMENT,
                `comment`  varchar(230) COLLATE '.\autosystem\DBConn::getCOLLATE().' NOT NULL,
                 PRIMARY KEY (`id`)
            )ENGINE = InnoDB DEFAULT CHARSET='.\autosystem\DBConn::getCHARSET().' COLLATE='.\autosystem\DBConn::getCOLLATE().';';
            $result3 = $this->DB->query($sql);
            $return3 = $result3->rowCount()==1;
        }else{
            $return3 = true;
        }
        if(!$resultx =$this->DB->query('ALTER TABLE `'.\autosystem\DBConn::getTableNameFor('user_rull').'` COMMENT=\'\'')){
        }
        if(!$this->existTableUserRullset()){
            $sql= 'CREATE TABLE IF NOT EXISTS `'.\autosystem\DBConn::getTableNameFor('user_rullset').'`(
                `rule_id` INT(10)ZEROFILL,
                `TYPE` ENUM(\''.implode('\',\'',array_keys($this->modeltypes)).'\') NOT NULL,
                 PRIMARY KEY (`rule_id`,`TYPE`)
            )ENGINE = InnoDB DEFAULT CHARSET='.\autosystem\DBConn::getCHARSET().' COLLATE='.\autosystem\DBConn::getCOLLATE().';';
            $result4 = $this->DB->query($sql);
            $return4 = $result4->rowCount()==1;
        }else{
            $return4 = true;
        }
        if(!$resultx =$this->DB->query('ALTER TABLE `'.\autosystem\DBConn::getTableNameFor('user_rullset').'` COMMENT=\'\'')){
        }
        return $return2 && $return1 && $return3 && $return4 ;
    }
    public function upgrade01(){
        if(!$this->existTableUserDashboard()){
            $sql= 'CREATE TABLE IF NOT EXISTS `'.\autosystem\DBConn::getTableNameFor('user_dashboard').'`(
                `id` INT(10) UNSIGNED ZEROFILL NOT NULL AUTO_INCREMENT,
                `user_id` INT(10)UNSIGNED ZEROFILL NOT NULL,
                `position` INT(4)default 0,
                `positiontype` INT(4)UNSIGNED ZEROFILL NOT NULL,
                 PRIMARY KEY (`id`,`TYPE`)
            )ENGINE = InnoDB DEFAULT CHARSET='.\autosystem\DBConn::getCHARSET().' COLLATE='.\autosystem\DBConn::getCOLLATE().';';
            $result4 = $this->DB->query($sql);
            $return4 = $result4->rowCount()==1;
        }else{
            $return4 = true;
        }
        if(!$this->existTableUserDashboardPosition()){
            $sql= 'CREATE TABLE IF NOT EXISTS `'.\autosystem\DBConn::getTableNameFor('user_dashboard_position').'`(
                `id` INT(10) UNSIGNED ZEROFILL NOT NULL AUTO_INCREMENT,
                `user_id` INT(10)UNSIGNED ZEROFILL NOT NULL,
                `position` INT(4)UNSIGNED default 0,
                `rastertype_id` INT(4)UNSIGNED ZEROFILL NOT NULL,
                 PRIMARY KEY (`id`,`TYPE`)
            )ENGINE = InnoDB DEFAULT CHARSET='.\autosystem\DBConn::getCHARSET().' COLLATE='.\autosystem\DBConn::getCOLLATE().';';
            $result4 = $this->DB->query($sql);
            $return4 = $result4->rowCount()==1;
        }else{
            $return4 = true;
        }
        if(!$this->existTableUserDashboard_Rastertype()){
            $sql= 'CREATE TABLE IF NOT EXISTS `'.\autosystem\DBConn::getTableNameFor('user_dashboard_rastertype').'`(
                `id` INT(10) UNSIGNED ZEROFILL NOT NULL AUTO_INCREMENT,
                `user_id` INT(10)UNSIGNED ZEROFILL NOT NULL,
                `position` INT(4)UNSIGNED default 0,
                `rastertype` INT(4)UNSIGNED ZEROFILL NOT NULL,
                 PRIMARY KEY (`id`,`TYPE`)
            )ENGINE = InnoDB DEFAULT CHARSET='.\autosystem\DBConn::getCHARSET().' COLLATE='.\autosystem\DBConn::getCOLLATE().';';
            $result4 = $this->DB->query($sql);
            $return4 = $result4->rowCount()==1;
        }else{
            $return4 = true;
        }
    }
    protected function __installUserEvents(){
        if(!$this->existTableUserEvents()){
            $sql= 'CREATE TABLE IF NOT EXISTS `'.\autosystem\DBConn::getTableNameFor('user_events').$this->eventextendor.'`(
                `id` INT(10) UNSIGNED ZEROFILL NOT NULL AUTO_INCREMENT,
                `accound_id`  INT(50),
                `location`  varchar(100) COLLATE '.\autosystem\DBConn::getCOLLATE().' NOT NULL,
                `TYPE` ENUM(\''.implode('\',\'',array_keys($this->eventtype)).'\') NOT NULL,
                `mondth` INT(2) ZEROFILL,
                `time`  timestamp NOT NULL,
                 PRIMARY KEY(id)
            )ENGINE = InnoDB DEFAULT CHARSET='.\autosystem\DBConn::getCHARSET().' COLLATE='.\autosystem\DBConn::getCOLLATE().';';
            $result4 = $this->DB->query($sql);
            return $result4->rowCount()==1;
        }else{
            return true;
        }
    }
    public function isInstalled(){
        return $this->isinstalled|| isset($this->DB)&& $this->existTableLogins()&& $this->existTableUser() && $this->existTableUser_rull() &&$this->existTableUserRullset() && $this->isinstalled=true;
    }
    public function __upgrade($version){      
        
        
        
        
        return true;        }
    public function existTableLogins(){       return $this->existTableLabel('user_logins');      }
    public function existTableUser(){         return $this->existTableLabel('user');        }
    public function existTableUser_rull(){    return $this->existTableLabel('user_rull');        }
    public function existTableUserRullset(){  return $this->existTableLabel('user_rullset');     }
    public function existTableUserEvents(){
        $this->eventextendor = date('Y');
        return $this->existTableLabel('user_events').$this->eventextendor;
    }
    public function getModelInfo(){           return 'Profile.model.info';        }
    public function getVersionNr(){           return (float)5.4;        }
    public function GET_profile($location,$templatename,&$view,$accound_id){
        $a = \autosystem\DBConn::getTableNameFor('accound');
        $u = \autosystem\DBConn::getTableNameFor('user');
        $e = \autosystem\DBConn::getTableNameFor('email');
        $sql='SELECT * FROM `'.$a.'` LEFT JOIN `'.$u.'` ON `'.$u.'`.`accound_id`= `'.$a.'`.`id`'.
        'LEFT JOIN `'.$e.'` ON `'.$a.'`.`email_id`= `'.$e.'`.`id` WHERE `'.$a.'`.`id`=\''.$accound_id.'\'';
        return parent::GET($location,$templatename,$view,$sql);
    }
    public function GET_ProfileAsArray($accound_id){
        $a = \autosystem\DBConn::getTableNameFor('accound');
        $u = \autosystem\DBConn::getTableNameFor('user');
        $e = \autosystem\DBConn::getTableNameFor('email');
        $sql='SELECT * FROM `'.$a.'` LEFT JOIN `'.$u.'` ON `'.$u.'`.`accound_id`= `'.$a.'`.`id`'.
        'LEFT JOIN `'.$e.'` ON `'.$a.'`.`email_id`= `'.$e.'`.`id` WHERE `'.$a.'`.`id`=\''.$accound_id.'\'';
        return parent::GET_Stmt($sql,null);
    }
    public function existUser($username){
        if(!$this->isinstalled){return false;}
        $sql = 'SELECT id FROM '.\autosystem\DBConn::getTableNameFor('accound').' WHERE `username`=:username';
        return $this->isQuery($sql,[':username'=>$username]);
    }
    public function existEmailAdress($email){
        if(!$this->isinstalled){return false;}
        $sql = 'SELECT id FROM '.\autosystem\DBConn::getTableNameFor('email').' WHERE `email`=:email';
        //print $sql;
        return $this->isQuery($sql,[':email'=>$email]);
    }
    public function getRuleDefault(){            return 'USER';        }
    protected function GET_TelePhoneSQL($enabled,$visible){
        $c = \autosystem\DBConn::getTableNameFor('user_contact_telephone');
        $t = \autosystem\DBConn::getTableNameFor('user_telephones');
        $sql='SELECT IF(`VISIBLE`=\'TRUE\',\' checked=checked \',\'\') AS `isVisable`,`user_contact_telephone`.* FROM `'.$c.
        '` LEFT JOIN `'.$t.'` ON `phones_id`=`'.$c.'`.`id` AND `accound_id`=:accound_id ';
        $where = '';
        $link ='WHERE';
        if(!empty($enabled)&&$enabled==true){     $where.= $link.' `ENABLED`=\'TRUE\'';
            $link =' && '; 
        }
        if(!empty($visible)&&$visible==true){     $where.= $link.' `VISIBLE`=\'TRUE\'';
            $link =' && '; 
        }
        $sql.= $where;
        return $sql;
    }
    public function GET_TelePhones_array($accound_id,$enabled = null,$visible= null){
        if(!$this->existTableLabel('user_contact_telephone')||!$this->existTableLabel('user_telephones')){
            print $view->get_template('global','PacketNotWorking');
            return false;
        }
        $sql= $this->GET_TelePhoneSQL($enabled,$visible);
        return parent::GET_Stmt($sql,[':accound_id'=>$accound_id]);
    }
    public function GET_TelePhones($location,$templatename,&$view,$accound_id,$enabled = null,$visible= null){
         if(!$this->existTableLabel('user_contact_telephone')||!$this->existTableLabel('user_telephones')){
            print $view->get_template('global','PacketNotWorking');
            return false;
        }
        $sql= $this->GET_TelePhoneSQL($enabled,$visible);
        return parent::GET_Summary($location,$templatename,$view,$sql,[':accound_id'=>$accound_id]);
    }
    public function GET_Adreses($location,$templatename,&$view,$accound_id,$enabled = null,$visible= null){
        if(!$this->existTableLabel('adres_detail')||!$this->existTableLabel('adres_location')||!$this->existTableLabel('adres_accound')){
            print $view->get_template('global','PacketNotWorking');
            return false;
        }
        return parent::GET_Summary($location,$templatename,$view,$this->GET_AdresesSQL($enabled,$visible),[':accound_id'=>$accound_id])==true;
    }
    public function GET_UserSessions($location,$templatename,&$view,$accound_id){
        if(!$this->existTableLabel('accound_prosessions')){
            print $view->get_template('global','PacketNotWorking');
            return false;
        }
        
        $a = \autosystem\DBConn::getTableNameFor('accound');
        $s = \autosystem\DBConn::getTableNameFor('accound_prosessions');
        $sql = 'SELECT * FROM `'.$a.'` LEFT JOIN `'.$s.'` ON `'.$a.'`.username = `'.$s.'`.`name` WHERE `accound_id` =:accound_id ORDER BY `'.$s.'`.`lastonline` DESC';
        return parent::GET_Summary($location,$templatename,$view,$sql,[':accound_id'=>$accound_id])==true;
    }
    protected function GET_AdresesSQL($enabled,$visible){
        $d = \autosystem\DBConn::getTableNameFor('adres_detail');
        $l = \autosystem\DBConn::getTableNameFor('adres_location');
        $a = \autosystem\DBConn::getTableNameFor('adres_accound');
        $sql='SELECT `'.$l.'`.* ,`'.$a.'`.* , `'.$d.'`.* , IF(`'.$a.'`.`VISIBLE`=\'TRUE\',\' checked=checked \',\' \') AS `isVisable` FROM `'.$a.
        '` LEFT JOIN `'.$d.'` ON `adresdetail_id`=`'.$d.'`.`id` AND `'.$a.'`.`accound_id`=:accound_id'.
        ' LEFT JOIN `'.$l.'` ON `location_id`=`'.$d.'`.`id` ';
        $where = 'WHERE `accound_id`=:accound_id ';
        $link ='AND';
        if(!empty($enabled)&&$enabled==true){     $where.= $link.' `ENABLED`=\'TRUE\'';
            $link =' && '; 
        }
        if(!empty($visible)&&$visible==true){     $where.= $link.' `VISIBLE`=\'TRUE\'';
            $link =' && '; 
        }
        $sql.= $where;
        return $sql;
    }
    public function setAdresVisibility($accound_id,$item_id,$bool){
        $VISIBLE = ($bool=='true')?'TRUE':'FALSE';
        $a = \autosystem\DBConn::getTableNameFor('adres_accound');
        $key ='UPDATE `'.$a.'` SET `VISIBLE`=:VISIBLE WHERE `adresdetail_id`=:adresdetail_id AND `accound_id`=:accound_id';
        $value = [':adresdetail_id'=>$item_id,':accound_id'=>$accound_id,':VISIBLE'=>$VISIBLE];
        $sql[$key]= $value;
        return $this->transaction($sql);
    }
    
    public function setPhoneVisibility($accound_id,$item_id,$bool){
        $VISIBLE = ($bool=='true')?'TRUE':'FALSE';
        $t = \autosystem\DBConn::getTableNameFor('user_telephones');
        $key ='UPDATE `'.$t.'` SET `VISIBLE`=:VISIBLE WHERE `phones_id`=:phones_id AND `accound_id`=:accound_id';
        $value = [':VISIBLE'=>$VISIBLE,':phones_id'=>$item_id,':accound_id'=>$accound_id];
        $sql[$key]= $value;
        return $this->transaction($sql);
    }
}
?>