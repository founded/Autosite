<?php
 /**
 * @author Lieven Roegiers
 * @copyright 2010 
 * @CMS autosite
 */
namespace users;
class Visitor{
    protected $browser;
	protected $remoteip;
	private $enabled;//disable by criminal use
	private $isheajack;
    private $userrules;
    private $actualrulls;
    protected $loginkey;
    protected $poging;
    protected $recent;
    //protected $counter;
    public function __construct() {
        //$this->counter= 0;
        $this->userrules = ['PUBLIC'];
        $this->actualrulls = ['PUBLIC'];
        if(empty($this->remoteip)){
            self::setRemoteip();
            self::updateStad();
        }        
        if(empty($this->browser)){      self::resetBrowser();}
        if(empty($this->poging)){       $this->poging =0;    }
        if(empty($this->loginkey)){     $this->loginkey = \autosystem\Key::GET(20);}
        $this->recent = [];
    }
	//<<<<<<<<<<<<<<<<<<<<<<<FUNCTIONS>>>>>>>>>>>>>>>>>>>>>>>>>
	private function isRemoteip(){
		if($this->remoteip === $_SERVER['REMOTE_ADDR']){	return true;
		}else{			$this->isheajack = true;			return false;
		}
	}
	//<<<<<<<<<<<<<<<<<<<<<<<Setters>>>>>>>>>>>>>>>>>>>>>>>>>
    public function ItryLogin(){        $this->poging++;    }
    public function setLocationToHistory($location,$GET=[],$fav = false){
       $this->recent = [];       
       /*if($fav){
            $key ='favorites';
            foreach($GET as $k=>$value){
                if(!key_exists($key,$this->recent)||!key_exists($location,$this->recent[$key])||!key_exists($k,$this->recent[$key][$location])){
                    $this->recent[$key][$location][$k]=[];
                }
                if(!key_exists($value,$this->recent[$key][$location][$k])){
                    array_push($this->recent[$key][$location][$k],$value);
                }
            }
       }else{
            $key ='history';
            $dif = ['loc'=>'','action'=>''];
            if(count(array_diff_key($GET, $dif))!=0){
                $this->recent[$key][$location][]= array_diff_key($GET, $dif);
            }else{
                $this->recent[$key][$location]=[];
            }
       }*/           
    }
    public function clearHistory($location=null){
        $key ='history';
         if(key_exists($key,$this->recent)){
            $this->recent['history']=null;
            unset($this->recent[$key]);
         }
    }
    public function getLocationHistory($fav = false){
       if($fav){    return key_exists('favorites',$this->recent)?$this->recent['favorites']:false;
       }else{       return key_exists('history',$this->recent)?$this->recent['history']:false;
       }
    }
    protected function resetBrowser(){
		$object = new \autosystem\Browser();
        $this->browser = $object->getType();
	}
    protected function forceRemip($ip){      $this->remoteip = $ip;	    }
    protected function setRemoteip(){        $this->remoteip = $this->getIp();	    }
    public function setActualrulles($null){ $this->actualrulls = $this->getRules();}
    public function setBrowser($browername){$this->brower= $browername;   	}
    public function islogin(){              return false;                   }
    public function isMykey($key){          return false;                   }
    public function hasRulle($rulle){       return false;                   }
    public function setLanguage($langname){ $this->lang= $langname;         }
    public function getLanguage(){         	return $this->lang;          	}
	//<<<<<<<<<<<<<<<<<<<<<<<Getters>>>>>>>>>>>>>>>>>>>>>>>>>
    public function getIp(){             return htmlspecialchars($_SERVER['REMOTE_ADDR']); }
    public function getRules(){             return ['PUBLIC'];              }
    public function getAllRules(){          return $this->getRules();       }
    public function getBrowser(){       	return $this->browser;        	}
    public function getAccound_id(){        return 0;                       }
    public function getloginkey(){          return $this->loginkey;         }
    
    public function __toString(){
        return '@access:I am a Visitor IP:'.$this->remoteip.'<br> Browser:'.@$this->browser.'LoginPoging:'.$this->poging ."\n loginkey:".$this->loginkey;//" <pre>".print_r($_SERVER,true)."</pre>";
    }
    private function updateStad(){
       $us=\autosystem\DBConn::getTableNameFor('user_stats');
       $sql = ' SELECT id FROM '.$us.' WHERE YEAR(`date`)=:year AND MONTH(`date`)=:month AND DAY(`date`) =:day';
       $database = \autosystem\DBConn::GET();
       $dbStmt = $database->prepare($sql);
       $datearr = [':year'=>date('Y'),':month'=>date('m'),':day'=>date('d')];
       $dbStmt->execute($datearr);
       $row = $dbStmt->fetch(\PDO::FETCH_ASSOC);
       if(isset($row) && $row!=false && @key_exists('id',$row)){
           $this->addstat('visitors',$datearr);
           $dbStmt->execute();
       }else{
           $sql = 'INSERT INTO '.$us.' (`id`,`date`,`visitors`)VALUES(NULL,\''.date('Y-m-d').'\',1)';
           $dbStmt = $database->prepare($sql);
           $dbStmt->execute();
       }
    }
    protected function addstat($colum,$datearr){
        $database = \autosystem\DBConn::GET();
        $sql = 'UPDATE '.\autosystem\DBConn::getTableNameFor('user_stats').'
         SET `'.$colum.'` = `'.$colum.'` + 1 WHERE YEAR(`date`)=:year AND MONTH(`date`)=:month AND DAY(`date`) =:day';
        $dbStmt = $database->prepare($sql);
        $dbStmt->execute($datearr);
    }
    public function kill(){        @session_unset();    }
    public function cast($object){
        if(is_object($object)&& is_subclass_of($object,'Visitor')){
            foreach ($object as $key => $value) {
                $this->$key = $value;
            }
        }
    }
}
?>