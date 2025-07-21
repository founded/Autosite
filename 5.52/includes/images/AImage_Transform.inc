<?php
/**
 * @author Lieven Roegiers
 * @CMS autosite
 * see also
 * https://imagemagick.org/Usage/annotating/
 * https://imagemagick.org/Usage/
 * http://www.magickwand.org/index.html
 * https://www.imagemagick.org/script/command-line-processing.php
 */
namespace images;
class AImage_Transform extends \images\AImage{
    private $copyright = '';
    private $issetlogo = false;
    private $issetfont = false;
    private $logo ;
    private $errors=[];
    private $font;
    private $textcolor;
    private $copyrighttext='copyright';
    public function setCopyright($string){
        $this->copyrighttext=$string;
    }
    public function setLogo($logo){
        $this->setLogoAlfa($logo);
        /*if(!is_file($logo)){   array_push($this->errors,'Logo file not found');    return false;    }
        $this->logo = $this->getAimageas0bject($logo);
        if($this->logo!=false){
            //header('Content-Type: image/jpeg');
            //print imagejpeg($this->logo);
            $this->issetlogo = true;
        }else{            
            array_push($this->errors,'can not create logo image object create implementation');
        }*/
    }
    public function setLogoAlfa($logo,$r=255,$g=255,$b=255){
        if(!is_file($logo)){   array_push($this->errors,'Logo file not found');    return false;    }
        $logosize  = getimagesize($logo);
        $alogo = $this->getImgobj($logo,$logosize['mime']);
        $this->logo = imagecreatetruecolor($logosize[0],$logosize[1]);
        $white = imagecolorallocate($this->logo, $r,$g,$b);
        // Make the background transparent
        imagecolortransparent($this->logo, $white);
        imagealphablending($this->logo, false); // turn off the alpha blending to keep the alpha channel
        imagecopyresampled($this->logo,$alogo, 0, 0, 0, 0,$logosize[0],$logosize[1], imagesx($alogo), imagesy($alogo));
        if($this->logo!=false){
            $this->issetlogo = true;
        }
        $showdebug =  false;
        if($showdebug){
            header('Content-type: image/gif');
            print imagegif($this->logo);
            imagedestroy($tlogo);
			return;
            //imagecolortransparent('', $black);
		}
    }
    public function settxtcolor(){
    }
    public function transformImage($source,$originaldest,$thumb=false,$thumbname='',$showdebug= false){ 
        $filename =strstr($source,'.');
        $ext = $this->getext($source);
        $mime = $this->getmime($ext);
    	$this->actualIMGObj = $this->getImgobj($source,$mime);
        $this->saveasE($originaldest,$ext,$this->actualIMGObj);//save original
        print $source.'**'.$originaldest.'**'.$thumb.'**'.$thumbname.'**<br>';
        try{
        if($thumb && $this->actualIMGObj!=false){
           $img = getimagesize($source);            
            $new = $this->newimagesize($img[1],$img[0]);
             $new['W'] = round($new['W'],0);
             $new['H'] = round($new['H'],0);
            $thumbrs = imagecreatetruecolor( $new['H'], $new['W']);
            //sleep(1);           
            @imagecopyresized($thumbrs,$this->actualIMGObj, 0, 0, 0, 0, $new['H'], $new['W'], $img[0],$img[1]);
            $this->saveasE($thumbname,$ext,$thumbrs);
            imagedestroy($thumbrs);
        } }catch(exception $e){
            print 'i smell an error';
        }
        try{    
        $size = getimagesize($source);
        $x = 25;
    	$y = $size[1]-25;
        $xtend = $size[0]- (30*strlen($this->copyrighttext));
        
        if(isset($txt)){
            $s[0] = 640; $s[1] = 480;
            $rights = imagecreatetruecolor($size[0],$size[1]);//Width Height
            imagecopyresampled($rights,$this->actualIMGObj,0,0,0,0,$size[0],$size[1],$size[0],$size[1]);
            $this->textcolor = imagecolorallocatealpha($this->actualIMGObj,191,0,0,65);
        }
        if($this->issetlogo){
            $markx = round(imagesx($this->actualIMGObj)/4);
            $marky = round((imagesx($this->actualIMGObj)/4)/imagesx($this->logo)*imagesy($this->logo));
            imagecopyresized($this->actualIMGObj, $this->logo, 15, 15, 0, 0,$markx, $marky, imagesx($this->logo), imagesy($this->logo));
        }
        }catch(exception $e){
            print 'i smell an logo error';
        }
        //if(is_array($this->fonts)){
				$b = imagecolorallocatealpha($this->actualIMGObj, 0, 0, 0,99); 
				$colorC = imagecolorallocatealpha($this->actualIMGObj, 255, 255, 255,99);
                $hcenter=$size[0]/2;
				$wcenter=$size[1]/2;
				$fontGrootte = $hcenter/3; 
				$co = imagettfbbox($fontGrootte*4,0, $this->font, ' -©- ');
				/**
				6 upper left corner, X position  		4 upper right corner, X position 
				7 upper left corner, Y position			5 upper right corner, Y position
				0 lower left corner, X position 		2 lower right corner, X position
				1 lower left corner, Y position			3 lower right corner, Y position 
				*/
				$txtWoffset = ($co[2] + $co[0])/2; //position right - left/2
				$txtHoffset = ($co[1] + 600)/2; //position botom - top/2
				imagettftext($this->actualIMGObj, $fontGrootte*4, 0, round($wcenter -$txtWoffset), round($hcenter+$txtHoffset) , $colorC, $this->font, ' -©- ');
				//imagettftext($this->actualIMGObj, $fontGrootte, 0, $wcenter, 20, $b, $this->font, " -©- \n  \n -©- \n  \n -©- \n  \n");
				//imagettftext($this->actualIMGObj, $fontGrootte, 0, $wcenter-2, 20,$colorC , $this->font, " -©- \n -©- \n -©- \n -©- \n -©- \n -©- \n");
                //imagettftext($this->actualIMGObj, $fontGrootte, 0, 0, 20, $colorC, $this->font, " -©- \n -©- \n -©- \n -©- \n -©- \n -©- \n");
				imagettftext($this->actualIMGObj, round($wcenter/20), 0, round($xtend/2), round($wcenter/20), $b, $this->font, $this->copyrighttext);
				imagettftext($this->actualIMGObj, $wcenter/20, 0, $x, $y, $b, $this->font, $this->copyrighttext);
				//imagettftext($target, $fontGrootte, 0, 0, 20, $colloruseblock, $font,print_r($co,true));
    	//}
        //$showdebug=true;
        if($showdebug){//by any not working check alsways if some output is print
			//header('Content-Type: image/jpeg');
            //print imagejpeg($this->actualIMGObj);
            //$this->
            
            
            imagedestroy($this->actualIMGObj);
            imagedestroy($this->logo);
			return;
		}else{
            $this->saveasE($filename,$ext,$this->actualIMGObj);
		}
        imagedestroy($this->actualIMGObj);
        imagedestroy($this->logo);
    }
    public function newimagesize($width, $height){
        $thumpsettings=false;
        if($thumpsettings){
        }else{            $maxh=200;            $maxw=200;
        }
        $i = 1;
        if($width*2>$height){
            $x=$width;
            while($x>$maxw){   $x=$width/($i*2);     $i++;    }
            $return['W']=round($width/($i*2));       $return['H']=round($height/($i*2));
        }else{
            $y=$height;
            while($y>$maxh){   $y=$height/($i*2);     $i++;    }
            $return['W']=round($width/($i*2));       $return['H']=round($height/($i*2));
        }
        return $return;
    }
    public function setFont($fontlocation){
        if(!is_file($fontlocation)){     array_push($this->errors,'Font .ttf file not found'.$fontlocation);    return false;    }
        $this->issetfont = true;
        $this->font=$fontlocation;
    }
    public function createlogo($sourcename,$removecolor){// by error use fotostcape to remove exif
        list($width, $height, $type, $attr) = getimagesize($sourcename);
        $new_img = getimgobj($sourcename,$type);
        $temp = imagecreatetruecolor($w, $h);
        /* making the new image transparent */
        $background = imagecolorallocate($temp, 0, 0, 0);
        ImageColorTransparent($temp, $background); // make the new temp image all transparent
        imagealphablending($temp, false); // turn off the alpha blending to keep the alpha channel
        imagecopyresized($temp, $src, 0, 0, 0, 0, $w, $h, imagesx($src), imagesy($src));
	}
    public function backgroundInvisible($sourcename,$removecolor='red' ,$as = 'png',$destname = ""){
        if(is_file($sourcename)){
            print $sourcename;
            $mime =$this->getFilemime($sourcename);
            $this->actualIMGObj = $this->getImgobj($sourcename,'image/gif');
            $color =$this->AlLocateColor($removecolor);
            //imagealphablending($this->actualIMGObj, true);
            //imagecolortransparent($this->actualIMGObj, $color);
            //imagesavealpha($this->actualIMGObj, true);
            imagealphablending($this->actualIMGObj, false);
            $colorTransparent = imagecolorallocatealpha($this->actualIMGObj, 0, 0, 0, 127);
            //imagetruecolortopalette($this->actualIMGObj, false, 256);
            $destname =($destname=='')?$sourcename:$destname;
            $this->saveas($destname,$as);
            if(is_file($destname)){
                $data =base64_encode(file_get_contents($destname));
                print $this->getMasker($mime,$data); //, $destname
            }else{
               print '<div class="errors" >file not create</div>'; //, $destname
            }
        }else{
            print '<div class="errors" >file not exist</div>'; //, $destname
        }
	}
    public function __destruct() {
        parent::__destruct();
        if($this->issetlogo){
            @imagedestroy($this->logo);
        }
    }
}
?>