<?PHP
 /**
 * @author Lieven Roegiers
 * @copyright 2007 refactor 2009 refactor 2024
 * @CMS autosite
 */
namespace data;
class DataSplit{
	private	$LineDelimiter=',';//End Of Line
    private $printcontent;
    private $databasedatetype='US';
    private $Darr=[];
    private $DEarr=[];
	public function __construct($LineDelimiter ="\n",$printcontent=false){
	   $this->LineDelimiter = $LineDelimiter;
       $this->printcontent = $printcontent;
    }
    /**
     * DataSplit::spliter()
     * @param mixed $data .be whois.dns.be #from belgium
     * @param mixed $regx "|(\.(?<ext>[A-Z\.a-z]+))( +(?<url>[A-Z\.a-z]+))( #+(?<comment>.*))?\n?|"
     * @param mixed $needle ['ext','url','comment']
     * @return
     */

    public function spliterold(&$data,$regx,$needle){
        $array=[];
        preg_match_all($regx,$data,$result,PREG_PATTERN_ORDER);
        if($this->printcontent){print  '<pre>$regx<br>'.$regx.print_r($result,true).'</pre>';}
        if(isset($result)&& key_exists($needle[0],$result)){
            foreach($result[$needle[0]] as $key => $item){
                $line = [];
                foreach($needle as $item){
                    if (!empty($result[$item][$key]))
                    $line[$item] = $result[$item][$key];
                }
                $array[$key]=$line;
            }
        }else{
            if($this->printcontent){print '<br>'.print_r($result,true);}
        }
        return $array;
    }
    public function spliter(&$data,$regx,$needle){
        $array=[];
        preg_match_all($regx,$data,$result,PREG_PATTERN_ORDER);
        if($this->printcontent){print  '<pre>$regx<br>'.$regx.print_r($result,true).'</pre>';}
        if(is_array($result)){
            
            if(count($needle)>0){
                 foreach($needle as $itemneed){
                    if(key_exists($itemneed,$result)&&is_array($result[$itemneed])){
                        $line[$itemneed] = $result[$itemneed][0];
                        print $itemneed.print_r($result[$itemneed][0]);
                    }                    
                }
            }
        }else{
            if($this->printcontent){print '<br>'.print_r($result,true);}
        }
        return $array;
    }
    public function linespliter(&$data,$regx,$needle){
        $array=[];
        preg_match($regx,$data,$result);
        if(isset($result)&& key_exists($needle[0],$result)){
            foreach($needle as $need){
                if(key_exists($need,$result)){
                    
                    if(in_array($need,$this->Darr)){
                        $array[$need]= $this->checkDate($result[$need],$this->databasedatetype);
                        //print$this->checkDate($result[$need],$this->databasedatetype);
                    }elseif(in_array($need,$this->DEarr)){
                        $array[$need]= $this->checkDECIMAL($result[$need],$this->databasedatetype);
                        //print$this->checkDate($result[$need],$this->databasedatetype);
                    }else{//normal
                        $array[$need] = $result[$need];
                        
                        
                    }
                }
            }
        }
        return $array;
    }
    public function setDateFields($arr){
            $this->Darr = $arr;//.'(?<rest>(.*))?/';
    }
     public function setDECIMALFields($arr){
            $this->DEarr = $arr;//.'(?<rest>(.*))?/';
    }
    
    public function mysqlfileregex($aso){
        $regex = "|\(NULL,'(?<code>['A-Z\.a-z0-9]+)[',]{3}(?<description>['A-Z\. \-a-z0-9]+)[',]{3}(?<type>['A-Za-z0-9]+)[',]{3}(?<country>['A-Za-z]+)[']{1}.*\),?\n?|";
        $return[] = '|\(';
        $R = "[',]{3}";
        $T = '';
        foreach($aso as $key => $value){
            $return[] = $T.'(?<'.$key.'>[\'A-Z\.a-z0-9]+)';
            $T=$R;
        }
        $return[] = '|\)';
        print_r($return);
    }
    public function arraytopostString($array){
        $encoded = '';
        foreach($array as $name => $value){
            $encoded .= urlencode($name).'='.urlencode($value).'&';
        }
        return $encoded;
    }
    public function ArrayPrintReader($String){
        preg_match_all('/(\[[A-Z,_a-z0-9]+\])?([\)])?(\[[A-Z_a-z0-9]+,Array)?(\[([A-Z_a-z0-9]{1,})\] \Q=>\E (.*'."\n".'))?/U',$String,$matches);
        //return $matches;
        $return = [];
        $levels = [];
        $afind = '/Array/i';
        $adown=')';
        $reset=false;
        foreach($matches[0] as $key => $value){
            $value = rtrim($value);
            if($value==')'){ //preg_match($adown,$value)){
                print 'down'.print_r($levels,true).count($levels);
                array_pop($levels);
            }
            if(strlen($value)< 2){
                continue;
            }
            if(preg_match($afind,$value)){
                $levels[]=$this->arraySplitClean($value);
                print 'up'.print_r($levels,true).count($levels);
                $reset=true;
                continue;
            }
            if(count($levels)>0){
                switch (count($levels)){ 
                	case 1:
                    $return[$levels[0]][]=$value;
                	break;
                
                	case 2:
                          $pos = strpos($value,']');
                          //print $value.$pos;
                          if ($pos >1) {
                               $return[$levels[0]][$levels[1]][$this->arraySplitClean(substr($value,0,$pos))]=$this->arraySplitClean(substr($value,$pos));  
                          }else{
                               $return[$levels[0]][$levels[1]][]=$value;
                          }
                    
                	break;
                
                	case 3:
                        $pos = strpos($value,']');
                        if ($pos >1) {
                            $return[$levels[0]][$levels[1]][$levels[2]][$this->arraySplitClean(substr($value,0,$pos))]=$this->arraySplitClean(substr($value,$pos));  
                        }else{
                            $return[$levels[0]][$levels[1]][$levels[2]][]=$value;
                        }
                	break;
                    case 4:
                        $pos = strpos($value,']');
                        if ($pos >1) {
                            $return[$levels[0]][$levels[1]][$levels[2]][$levels[3]][$this->arraySplitClean(substr($value,0,$pos))]=$this->arraySplitClean(substr($value,$pos));  
                        }else{
                            $return[$levels[0]][$levels[1]][$levels[2]][$levels[3]][]=$value;
                        }
                	break;
                    case 5:
                    $return[$levels[0]][$levels[1]][$levels[2]][$levels[3]][$levels[4]][]='2'.$value;
                	break;
                	default :
                    $return[$levels[0]][]=$value;
                }
                if(is_array($return[$levels[0]][count($return[$levels[0]])-1])&&!$reset){
                  
                }else{
                  
                  $reset = false; 
                }
                /*
                if(!is_array($return[$levels[1]]))$return[$levels[1]]=[];
                $return[$levels[1]][]=$value;
                */
            }else{
                $pos = strpos($value,',');
                //print substr($value,0,$pos).'x'.substr($value,$pos+1);
                if ($pos >1) {
                   $return[$this->arraySplitClean(substr($value,0,$pos))]=$this->arraySplitClean(substr($value,$pos));  
                }/*else{
                    array_push($return,$value);
                }*/
            }
        }
        return $return;
    }
    public function arrayToForm($array ){
        $return = '';
        foreach($array as $key =>$value){
            if(is_array($value)){
                foreach($value as $k =>$v){
                    if(is_array($v)){
                        foreach($v as $ks =>$vs){
                            if(is_array($vs)){
                                foreach($vs as $kse =>$vse){
                                    if(is_array($vse)){
                                         foreach($vse as $kseb =>$vseb){
                                            if(is_array($vseb)){
                                                
                                            }else{
                                                $return.= $this->htmltag($key.'['.$k.']['.$ks.']['.$kse.']['.$kseb.']',$vseb); 
                                            }  
                                        }
                                    }else{
                                        $return.= $this->htmltag($key.'['.$k.']['.$ks.']['.$kse.']',$vse); 
                                    }  
                                }
                            }else{
                                $return.= $this->htmltag($key.'['.$k.']['.$ks.']',$vs); 
                            }  
                        }
                    }else{
                        $return.= $this->htmltag($key.'['.$k.']',$v); 
                    }  
                }
            }else{
              $return.= $this->htmltag($key,$value); 
            }
        }
        return $return;
    }
    private function htmltag($k,$v){
        return '<input type="text" name="'.$k.'" value="'.$v.'" />'."\n";
    }
    private function arraySplitClean($string){
        $order=['[',']','Array','=','>',',',' '];
        $replace='';
        return rtrim(str_replace($order,$replace,$string));
    }
    protected function checkDate($date,$type='US'){
        $a = trim($date);
        $a = str_replace([' ','/'],'-',$a);
        $array = explode('-',$a);
        foreach($array as $key=>$piece){
            if(strlen($piece)==1 ){
                $array[$key]= '0'.$piece;
            }
        }
        if(\autosystem\ValidateAs::isUSAdate($a)){
            if($type =='US'){      return $a;                     }
            if($type =='EU'){      return implode('-',array_reverse($array));     }
        }
        if(\autosystem\ValidateAs::isEUAdate($a)){
            if($type =='EU'){      return $a;                     }
            if($type =='US'){      return implode('-',array_reverse($array));     }
        }
        return false;
    }
    protected function checkDECIMAL($decimal,$type='US'){
        $d = trim($decimal);
        if($type =='US'){      return str_replace([','],'.',$d);                    }
        if($type =='EU'){      return str_replace(['.'],',',$d);      }
        return false;
    }	 	
}
?>
