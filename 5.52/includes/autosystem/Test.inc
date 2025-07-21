<?php
/**
 * @author Lieven Roegiers
 * @copyright 2016 
 * @CMS autosite
*/
namespace autosystem;
class Test{
	private $location;
	private $filename;
	private $isdebug;
	public function __construct($location,$filename){
		$this->location = '.'.DIRECTORY_SEPARATOR.'tests'.DIRECTORY_SEPARATOR.$location.DIRECTORY_SEPARATOR;
		$this->filename = $this->location.$filename.'.test';
		 global $_DEBUG;
		$this->isdebug = $_DEBUG; 
	}
	public function loadTest(&$ReflectArray){
        global $_DEBUG;
        if(!$_DEBUG){return;}
        print $ln.'<br>[ok]filelocation :'.__FILE__;
        print $ln.'<br>'.(is_dir($this->location)?'[ok]':'[nok]').'dir :.'.$this->location;
        print $ln.'<br>'.(is_file($this->filename)?'[ok]]':'[nok]').'file:'.$this->filename;
        if(is_file($this->filename)&&($values = parse_ini_file($this->filename))){
        	print $ln.'<br>'.(is_array($values)?'[ok]':'[nok]').'array:'.print_r($values);
            foreach($values as $key =>$val ){
            	print $ln.'<br>'.$key.'->'.$val;
				$ReflectArray[$key] = $this->regeneratArray($val);				
            }
            return true;
        }
        return false;
    }
	private function regeneratArray($string){
      if(is_array($string)){return $string;}
      if(preg_match('/array/i',$val)){
		$pre=explode('Array',$string);
      	$n = 0;
      	$i=0;
      	$arr=[];
	    foreach($pre as $piece){
	          if($arr = $this->regeneratArray($string)){
	             
	          }            
	          if(substr($trimedpiece, 0, 1)=='[') {
	            $arr[$i] = $trimedpiece ;
	            $i++;
	          }
	      }
      	 return $arr;
	  }
	  if(preg_match('/\[/i',$string)){
	  	$pre=explode('[',$string);
		print('<pre>');
		$n=0;
		$arr=[];
		foreach($pre as $val){
			$n++;
			if(preg_match('/=>/',$string)){
				print '<br>'.$val;
			}
			print '<br>'.$val;
		}
		print('</pre>');
	  }
	  return $string;

    }
    private function prepare($prearray){
		foreach($prearray as $val){
			if(preg_match('/\]/i',$val)){
					
			}
		}
	}
	private function field(){
		
		
	}
    private function regeneratArrayRecursiveIter($string){
       $first =  substr(trim($string), 0, 1);
       if($trimedpiece=='('){       $n++; }
       if($trimedpiece==')'){       $n--; }
       $array = [];
       return $array;
    }
}