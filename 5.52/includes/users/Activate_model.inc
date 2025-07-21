<?php
/**
* @author Lieven Roegiers
* @copyright 
* @CMS autosite
*/
    namespace users;
	class Activate_model extends \users\Register_model{
        public function getModelInfo(){     return 'Activate.model.info';        }
        public function getVersionNr(){     return (float)4.7;        }
        public function __upgrade($version){  return true;        }
        public function getRuleDefault(){   return 'PUBLIC';      }
        public function isemailchecked($activationkey){
            $e = \autosystem\DBConn::getTableNameFor('email');
            $a = \autosystem\DBConn::getTableNameFor('accound');
            $sql= 'SELECT `emailchecked` FROM '.$e.' LEFT JOIN '.$a.' ON '.$a.'.email_id = '.$e.'.id WHERE `emailchecked`=\'TRUE\' AND `key`=:key';
            $param= [':key'=>$activationkey];
            return $this->isQuery($sql,$param);
        }
        public function activateThis($activatekey,$activate=false){
            $ACTIVATED = ($activate)?'TRUE':'FALSE';
            $accoundid = $this->getAccoundid($activatekey);
            $e = \autosystem\DBConn::getTableNameFor('email');
            $a = \autosystem\DBConn::getTableNameFor('accound');
            $key =' UPDATE '.$a.' SET `ACTIVATED`=\''.$ACTIVATED.'\' WHERE `key`=:key ';//
            $sql[$key]= [':key'=>$activatekey];
            if(!$this->isemailchecked($activatekey)){
                $key ='UPDATE '.$e.' LEFT JOIN '.$a.' ON '.$a.'.email_id = '.$e.'.id SET `emailchecked`=\'TRUE\' WHERE `key`=:key ';
                $sql[$key]= [':key'=>$activatekey];  
            }
            if($accoundid!=false && !$this->isUser($accoundid)){
                $u = \autosystem\DBConn::getTableNameFor('user'); 
                $key ='INSERT INTO '.$u.'(`id`,`accound_id`,`TYPE`,`name`,`lastonline`,`user_path`) '.
                      'VALUES(NULL,:accoundid,:TYPE,:name,:lastonline,:path) RETURNING accound_id';
                $value = [':accoundid'=>$accoundid,':TYPE'=>'OUTSIDER',':name'=>' ',':lastonline'=>$this->nowDateTime(),':path'=>''];
                $sql[$key]= $value;  
            }
            return is_array($this->transaction($sql));
        }
        public function isUser($accoundid){
            $sql ='SELECT id FROM '.\autosystem\DBConn::getTableNameFor('user').' WHERE `accound_id`=:accound_id ';//
            $param = [':accound_id'=>$accoundid];
            return $this->isQuery($sql,$param);
        }
        private function getAccoundid($activatekey){
            $sql =' SELECT `id` FROM '.\autosystem\DBConn::getTableNameFor('accound').' WHERE `key`=:key ';//
            $param= [':key'=>$activatekey];
            $array = $this->GET_Stmt($sql,$param);
            return (is_array($array)&& array_key_exists('id',$array))?$array['id']:false;
        }
        public function getUserid($activatekey){
            $u = \autosystem\DBConn::getTableNameFor('user');
            $a = \autosystem\DBConn::getTableNameFor('accound');
            $sql ='SELECT `'.$u.'`.`id` FROM `'.$u.'` LEFT JOIN `'.$a.'` ON `'.$u.'`.`accound_id`=`'.$a.'`.`id` WHERE `key`=:key';//
            $param = [':key'=>$activatekey];
            $array = $this->GET_Stmt($sql,$param);
            return (is_array($array)&& array_key_exists('id',$array))?$array['id']:false;
        }   
    }
?>