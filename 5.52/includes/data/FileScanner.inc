<?php
namespace data;
/**
 * @author Lieven Roegiers
 * @CMS autosite
 */
class FileScanner{
	private $dirs =[];
	private $files =[];
	private $inc_ext =[];
	private $exc_ext =[];
	private $isnotinc_ext = false;//!!!not!!!
	private $scaneddir;
    private $infoindirs=[];
    private $expandedshow=false;
    private $templates = ['file'=>'Files_file_Item','dir'=>'Files_dir_Item'];
    private $icons = [];
    private $templatelocation ='';
	public function scandir($dir){
		$this->dirs =[];
		$this->files =[];
        //print $dir;
		if ($d = Dir($dir)){
			while (false !== ($entry = $d->read())) {
            	$pos = strripos($entry, '.');
            	//print $entry.$pos;
            	if ($pos === false){
        	       array_push($this->dirs,$entry);	
	            }elseif(substr($entry, 0, 1) != '.'){
	               
        		   $name = substr($entry, 0, $pos);
        		   $ext = substr($entry, $pos-strlen($entry));
	               if ($this->isInSelection($ext)){
	                   //print $entry.$pos;
					   array_push($this->files,$entry);
	               }
       	        }
            }
		}
	}
	public function scan2extfiles($dir){
		$this->dirs =[];
		$this->files =[];
		if ($d = Dir($dir)){
			while (false !== ($entry = $d->read())) {
            	$pos = strripos($entry, '.');
            	//print $entry.$pos;
            	if ($pos === false){
        	       array_push($this->dirs,$entry);	
	            }elseif(substr($entry, 0, 1) != '.'){
        		   $name = substr($entry, 0, $pos);
        		   $ext = substr($entry, $pos-strlen($entry));
        		   $pos = strripos($name, '.');
        		   if($pos !== false){
        		   	    $ext = substr($name, $pos-strlen( $name)).$ext;
						$name = substr( $name, 0, $pos);
				   }
                   //print $entry;
	               if ($this->isInSelection($ext)){
					   array_push($this->files,$entry);
	               }
       	        }
            }
		}
		//$this->test();
	}
	public function set_inc_extfilter($arrlist){
		$this->isnotinc_ext = false;//!!!not!!!
		$this->inc_ext=$arrlist;
	}
    public function show(&$view,$dir,$location){
        $this->templatelocation =$location;
        $this->scanin($view,$dir);
    }
    private function scanin(&$view,$dir,$level =1,$files=true){
        $fscan = new \data\FileScanner(); 
        $fscan->scandir($dir);
        $empty=true;
        if($files){
             foreach($fscan->getfils() as $key => $f){
                 $a= $this->fileInfo($f);
                 $a['file']=$f;
                 $a['level']=$level;
                 $a['IMGEXT']='';
                 print $view->get_template($this->templatelocation,$this->templates['file'],$a);
                 $empty=false;
             }
        }
        foreach($fscan->getdirs() as $key => $d){
             $img = '-open'; 
             $dd = ['dir'=>$d,'level'=>$level,'IMGEXT'=>$img ];
             print $view->get_template($this->templatelocation,$this->templates['dir'],$dd);
             $this->scanin($view,$dir.DIRECTORY_SEPARATOR.$d,$level+1,$files);
        }
    }
    public function fileInfo($file,$ineedyou = false){
        $values = [];
        if(!$ineedyou)return $values;
        $values['file']=$file;
		$values['filetime']=date('D. F jS, Y - h:ma', filemtime($file));
        $values['filesize']=filesize(rtrim($path, '/') . '/' . $t);
        return $values;
    }
	/**
	* @param $array[inc]=array('.inc','.htm','.old'); 
	* @param $array[exc]=array(".htaccess");
 	*/
 	public function setfilter($arr){
 		if(isset($arr['inc'])){
 		     $this->inc_ext=$arr['inc'];
             $this->isnotinc_ext = true;
	    }
 		if(isset($arr['exc'])){
 		     $this->inc_ext=$arr['exc'];
        }
	}
	public function renderDirectories(&$view , $location, $template){
		$return = '';
		foreach($this->dirs as $dir){
			$values['dir']=$dir;
			$return .= $view->get_Templateclone($location, $template,$values);
		}
        return $return;
	}
        
	public function renderFiles(&$view , $location, $template, $detail=false){
		$return = '';    
        $total_size = 0;        
		$this->dirs =[];
		foreach($this->files as $file){
			$values['file']=$file;
			$values['filetime']=date('D. F jS, Y - h:ma', filemtime($file));
            $values['filesize']=filesize(rtrim($path, '/') . '/' . $t);
            $total_size+=$values['filesize'];            
			print $view->get_Templateclone($location, $template,$values);
            
		}
        return $total_size;
	}
    private function dirTXTamount($size){
        $mod = 1024;
        $section = ['B','KB','MB','GB','TB','PB'];
        for($i = 0; $size > $mod; $i++) {
            $size /= $mod;
        }
        return round($size, 2) . ' ' . $section[$i];
    }
    private function getExtentionS($fname){
        return substr(strrchr($fname, '.'), 1);
    }    
    public function isInSelection($ext){
        return $this->isinclude($ext)&& !$this->isexclude($ext);
    }
    private function isinclude($ext){
        return !$this->isnotinc_ext || in_array($ext,$this->inc_ext);
    }
    private function isexclude($ext){
       return in_array($ext,$this->exc_ext);
    }
	public function getdirs(){			return $this->dirs;
	}
	public function getfils(){			return $this->files;
	}
	private function test(){
		print '<br><br>print dirs<br>';
		print_r($this->dirs);
		print '<br><br>print files<br>';
		print_r($this->files);
	}
}
?>