<?php
 /**
 * @author Lieven Roegiers
 * @CMS autosite
 */
namespace autosystem;
class LogFile{
	private	$Delimiter=',';
	private $filelocation;
	private $file;
    private $logredirect = false;
	/** 
	 * @param mixed $pathname
	 * @param mixed $filelocation
	 * @return
	 */
	public function __construct($pathname,$fname){
	    if($this->logredirect){
	       $pathname = '';  
	    }
		$this->filelocation=$pathname.$fname;
        if(!is_dir($pathname)&& $pathname!=''){mkdir($pathname);}
	}
	private function open($mode){
		return($this->is_readytouse()&& $this->file = fopen($this->filelocation,$mode)); //File!found E53 
	}
	function save_line($arrData,$extraTextDelemiter = '"',$mode='a+'){
		$datastring = @implode($extraTextDelemiter.$this->Delimiter.$extraTextDelemiter,$arrData);//space on the front is for search
		return ($this->open($mode)&& fputs($this->file, $extraTextDelemiter.$datastring.$extraTextDelemiter."\n"));
	}
	public function is_readytouse(){
		return(isset($this->filelocation));
	}
	function __destruct(){
		@fclose($this->file);
	}	 	
}
?>