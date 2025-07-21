<?php
/**
 * @author Lieven Roegiers
 * @CMS autosite
 */
namespace images;
class AImage {
	private $TRUEext = ['.gif','.jpg','.png','.jpeg'];
    private $mime_ext = ['image/jpeg' => 'jpg',
                         'image/pjpeg' => 'jpg',
                         'image/x-png' => 'png',
                         'image/png' => 'png',
                         'image/gif' => 'gif',
                         'image/bmp' => 'bmp',//dont use on web return 'bmp';
                         'image/psd' => 'psd',
                         'image/tiff' => 'tiff',
                         'image/pjpeg' => 'jpg',
                         'application/x-shockwave-flash' => 'swf',
                         'image/iff' => 'aiff',
                         'image/xbm' => 'xbm',
                         'image/jp2' => 'jp2',
                         'application/octet-stream' => null ];
	private $actualimgsize;
    public $actualIMGObj;
    protected $images = [];//['key'=>'image']
	function __construct() {
		/*if (!function_exists('imagecreatetruecolor')){
            die('Error: Please make sure you have GD library ver 2+(contact your hosting or watch to php.ini)');
        }*/
	}
    private function getsize($fname){
        //list($this->actualimgsize['y'], $this->actualimgsize['x']) = getimagesize($fname); //width , height
        $array = getimagesize($fname); //width , height
        $this->actualimgsize['y']= imagesY($this->actualIMGObj);
        $this->actualimgsize['x']= imagesX($this->actualIMGObj);
    }
	public function is_trueextentie($ext){
       return(!in_array($ext,$this->TRUEext));
  	}
    protected function getext($Fname){
        $e=strrchr($Fname,'.');
        if(strlen($e) < 2){         return false;       }
        return substr($e,1);//remove . before extention
	}
    protected function newImgObject($x,$y){
        $this->actualIMGObj = imagecreatetruecolor($this->actualimgsize['y'],$this->actualimgsize['x']);
    }
    public function set0bjectfrom($sourcefilename,$htmlpreview=false){
    	$mime = $this->getFilemime($sourcefilename);
    	$this->actualIMGObj = $this->getImgobj($sourcefilename,$mime);
    	if($htmlpreview){
			print $this->getMasker($mime,$this->actualIMGObj);
		}
	}
    public function getAimageas0bject($filename){
    	$mime = $this->getFilemime($filename);
        //print $mime.$filename;
    	return $this->getImgobj($filename,$mime);
	}
	public function aCOPY($sourcefilename,$destfilename,$editmimeto = ''){
    	$mime = $this->getFilemime($sourcefilename);
    	$actualIMGObj = $this->getImgobj($sourcefilename,$mime);
    	file_put_contents($uplink,imagejpeg($target));
	}
	protected function getImgobj($Fname,$mime){
	   //print $Fname.$mime;
		switch($mime){ 
			case 'image/pjpeg':
			case 'image/jpeg':
				return imagecreatefromjpeg($Fname);
				break;
			case 'image/x-png':
			case 'image/png':
				return imagecreatefrompng($Fname);
				break;
			case 'image/bmp':
				return imagecreatefromwbmp($Fname);
				break;
            case 'image/gif':
				return imagecreatefromgif($Fname);
				break;
            /*case "image/gif":
				return imagecreatefromgif($Fname);
				break;*/
			default :
				return false;
				break;
		}
	}
    protected function getextend($mime){
        if(key_exists($mime,$this->mime_ext)){
            return $this->mime_ext;
        }
    }
    protected function getmime($extend){
        if($key = array_search($extend,$this->mime_ext)){
            return $key;
        }
    }
    public function getFilemime($filename){
        $ext =$this->getext($filename);
        return $this->getmime($ext);
    }
    public function getimg(&$imgname,&$height,&$width){
       //header("Content-type: " . image_type_to_mime_type(IMAGETYPE_JPEG));
       //header('Content-type: ' .image_type_to_mime_type($type));
       //print $imgname;
       //return exif_thumbnail(&$imgname,&$height,&$width,IMAGETYPE_JPEG);
       // echo 'No thumbnail available';         
    }
    public function __destruct() {
        //@imagedestroy($this->actualIMGObj);
    }
    protected function AlLocateColor($s){
        switch($s){ 
			case 'red':
			case 'RED':
                return imagecolorallocate($this->actualIMGObj, 255, 0, 0);
				//return imagecolorallocate($this->actualIMGObj, 255, 43, 51);
				break;
            case 'black':
			case 'BLACK':
				return imagecolorallocate($this->actualIMGObj, 0, 0, 0);
            case 'white':
			case 'WHITE':
				return imagecolorallocate($this->actualIMGObj, 255, 255, 255);         
			default :
				return null;
				break;
		}
    }
    public function getMasker($contentype,&$data){
        return '<!-- img src="data:'.$contentype.';base64,'.$data.'" alt="embedded folder icon" -->';
    }
    public function saveas($Fname,$ext,$attendext=false,$encoded64=false){
        
        $this->saveasE($Fname.'.'.$ext,$ext,$this->actualIMGObj);
        /*if($attendext){
            $Fname.='.'.$ext;
        }
 		switch($ext){
		    case 'jpg':
			case 'jpeg':
				 imagejpeg($this->actualIMGObj,$Fname);
				break;
			case 'png':
				 imagepng($this->actualIMGObj,$Fname);
				break;
            case 'gif':
				 imagegif($this->actualIMGObj,$Fname);
				break;
            /*case "image/gif":
				 imagecreatefromgif($Fname);
				break;*
			default :
				 false;
				break;
	   }*/
    }
    public function saveasE($name,$ext,&$source){
		switch($ext){
		    case 'JPG':
		    case 'jpg':
			case 'jpeg':
				@imagejpeg($source,$name);
				break;
			case 'png':
				 imagepng($source,$name);
				break;
            case 'gif':
				 imagegif($source,$name);
				break;
            /*case "image/gif":
				 imagecreatefromgif($Fname);
				break;*/
			default :
				 false;
				break;
	   }
       @chmod($source, 0755);
    }
}
?>