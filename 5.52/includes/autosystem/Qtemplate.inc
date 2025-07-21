<?php
/**
* @author Lieven Roegiers
* @copyright 2009
* @CMS autosite
*/
namespace autosystem;
class Qtemplate{
	private $html;
	private $toolsbar='';
    private $compress = false;
    private $addslach = false;
	/** @param var $html */
	function __construct(&$html){  $this->html=$html;  }
    function settoolbar($toolbar,$link){		$this->toolsbar=$toolbar;              		}
	public function reset($html){  $this->html=$html;  }
    /** @return translated data */
	public function gethtml(){		return $this->html;	}
	/**
     * @onerror if an error array to string convertion you cant give any array to the $value
	 * @param mixed $key
	 * @param mixed $value
	 */
	function toreplace($key,$value){  
	   if($this->addslach){
	       $this->html=str_replace('%'.$key.'%',addslashes($value),$this->html);
	   }else{
	       $this->html=str_replace('%'.$key.'%',$value,$this->html); 
	   }	
    }
    public function translate($asoarr){
	    if(!is_array($asoarr)){return; }
        foreach($asoarr as $key => $value){
            if($key!='toolbar'){
               	if ($value !=' '){           	$this->toreplace($key,$value);             	}
            }else{                          	$this->toreplace($key,$this->toolsbar);     } 	
        }
	}
	public function setvalues($asoarr){
	  if(is_array($asoarr)){ foreach($asoarr as $key=>$value){   $this->toreplace($key.'->value',(string)$value);	}
	  }
	}
    public function setcompress($bool){
        if($bool ){// do not use comment as //
            $arr=["\r\n", "\n", "\r" , "\t" , '  '];//
            $this->html = str_replace($arr,'',$this->html);
        }
    }
    /**
    *some scripts need slaches
    */
    public function setAddSlachesOnValues($addslach){
        $this->addslach = $addslach;
    }
    public function __clone(){ 
	}
}
?>