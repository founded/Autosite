<?php
/**
 * @author Lieven Roegiers
 * @copyright 
 * @CMS autosite
*/
namespace autosystem;
class Avalidator{
    private $errors;
    private $forms;
    private $myOwnRulls;
    private $debug ;
    private $clean = false;
    private $dateformattosave = 'US';//'EU'
	public function __construct($locale,$debug=false){
        $this->forms = [];
		$this->locale=$locale;
		$this->myOwnRulls = [];
		$this->debug=$debug;
    }
    public function addForm($formname,$fields,$clean = false,$canEmpty=[],&$REFLECTOR=[]){
		$this->forms[$formname]['fields']=$fields;
		$this->forms[$formname]['fieldsEmptyOrOk']=$canEmpty;
		$this->forms[$formname]['clean']=$clean;
		$this->forms[$formname]['errors']=[];		
		$this->forms[$formname]['formok']=false;
		$this->forms[$formname]['formcontentok']=false;
		$this->forms[$formname]['formdata']=[];
		$this->forms[$formname]['checked']=false;

		foreach($this->forms[$formname]['fields'] as $fieldname){
			if(!key_exists($fieldname,$REFLECTOR)||key_exists($formname,$_GET)){
				$REFLECTOR[$fieldname]= '';
			}
		}
	}
	public function addMenu($name,$fields,&$INPUT,&$REFLECTOR=[]){
		$this->forms[$name]['fields']=$fields;
		$this->forms[$name]['fieldsEmptyOrOk']=$fields;
		$this->forms[$name]['clean']=false;
		$this->forms[$name]['errors']=[];		
		$this->forms[$name]['formok']=false;
        $this->forms[$name]['menu']=true;
		$this->forms[$name]['formcontentok']=false;
		$this->forms[$name]['formdata']=[];
		$this->forms[$name]['checked']=false;
		foreach($this->forms[$name]['fields'] as $fieldname){
			if(!array_key_exists($fieldname,$INPUT)){
		    	
		    }else{
		        $REFLECTOR[$fieldname]= 'selected';
		        $this->forms[$name]['checked']=true;
		    }
		}
	}
    public function getMenu($name,&$view){
        $return ='<span class="p100 lineitem" ><!--dynamic menu['.$name.'] Avalidator-->'.
                '<ul class="Amenu tabs" >';
                foreach($this->forms[$name]['fields'] as $fieldname){
                    //Todo translation
                    $return .=$view->get_Templateclone('global','menu_item',['name'=>$fieldname, 'label' =>'transl'.$fieldname ]) ;
                }
                $return.= '</ul>'.'</span>';
        return $return; 
    }
    public function isChecked($form){
        return $this->forms[$form]['checked'];
    }
	public function addSelect($formname,$field,$collection){ 
	   $this->forms[$formname]['select'][$field]=$collection;
    }
	public function getSelect($formname,$field,&$view,$selectitem=''){
		if(array_key_exists($formname,$this->forms)&& array_key_exists('select',$this->forms[$formname])&& array_key_exists($field,$this->forms[$formname]['select'])){
			$collection = $this->forms[$formname]['select'][$field];
			return $view->getOptions($collection,$selectitem);
		}
		return '';
	}
	public function addAlocat($formname,$asorefrence){		$this->forms[$formname]['alocate']=$asorefrence;	}
	public function getErrors(){		return $this->forms[$formname]['errors'];	}
	public function setDebug($bool){	return $this->debug=$bool;	}
	public function formcreator($formname,$loc,$action){
		$this->existform($formname);
		$form = new \autosystem\AFormCreator();
		//$fielddata,$alocates,$canEmpty=[]
		print_r($this->forms[$formname]['alocate']);
		$alocate = is_array($this->forms[$formname]['alocate'])?$this->forms[$formname]['alocate']:[];
		$return =$form->render($this->forms[$formname]['fields'],	$alocate);
		$return .= '<!-- autosite render by valdator -->';
		return $return;
	}
	public function render($formname,&$INPUT,&$REFLECTOR=[]){
		$this->forms[$formname]['formok'] = true;
		$this->forms[$formname]['formcontentok'] = true;
		if($this->debug){print'start render ...';}
		foreach($this->forms[$formname]['fields'] as $fieldname){
			if($this->debug){print"\n".'('.$fieldname.'';}
		    if(!array_key_exists($fieldname,$INPUT)){
		    	$ok = true;
		    	if(!in_array($fieldname,$this->forms[$formname]['fieldsEmptyOrOk'])){
		    		$ok = false;
					$this->forms[$formname]['formok']= false;
				}
				$this->forms[$formname]['errors'][$fieldname]= (($ok)?'[ok]can be empty,key not exist.':'[nok]key not exist')."\n";
		    }else{
		        if(empty($INPUT[$fieldname])){
		            $this->forms[$formname]['errors'][$fieldname]= 'empty';
		            if($this->debug){print'=>empty';}
		            $this->forms[$formname]['formcontentok'] = ($this->forms[$formname]['formcontentok'] && in_array($fieldname,$this->forms[$formname]['fieldsEmptyOrOk']));
		            $REFLECTOR[$fieldname]= '';
		        }else{
		        	$cleanval = $this->cleanvalue($INPUT[$fieldname]);
		        	$fieldnamed = (key_exists('alocate',$this->forms[$formname])&& key_exists($fieldname, $this->forms[$formname]['alocate']))?$this->forms[$formname]['alocate'][$fieldname]:$fieldname;
		            if($this->debug){print '('.$fieldname.'[validate as]'.$fieldnamed.')';}
					if(!$this->logicValidation($fieldnamed ,$cleanval)){
		              $this->forms[$formname]['errors'][$fieldname]= 'logicinvalid';
		              if($this->debug){print'=>invalid';}
		              $this->forms[$formname]['formcontentok'] = false;
		              $REFLECTOR[$fieldname]= '';
		            }else{
		              if($this->debug){print'=>valid)';}
                      
					  $this->forms[$formname]['formdata'][$fieldname]= $cleanval;
                      //print $fieldname;
                      //print $cleanval;
		              if($cleanval=='on'){
						$REFLECTOR[$fieldname]= 'checked=checked';
					  }else{
						$REFLECTOR[$fieldname]= $cleanval;
					  }
		            }
		        }
		    }
		}
		$this->forms[$formname]['checked']=true;
		return ($this->forms[$formname]['formcontentok']&&$this->forms[$formname]['formok']);		
	}
	private function cleanvalue($value){
		if($this->clean === false){return $value;}
		$value = trim($value);
	    $value = stripslashes($value);
	    $value = htmlspecialchars($value);
	    return $data;
	}
	private function existform($name){
		if(ctype_alnum($name)&&preg_match('/^[A-Za-z]*$/',$name)){
			return key_exists($name,$this->forms);
		}
		return false;
	}
    public function setDataError($formname,$msg){
        $this->forms[$formname]['errors']['dataError'] = $msg;
    }
	public function isValid($formname){
		if(!array_key_exists($formname,$this->forms)){
			return false;
		}
		return ($this->forms[$formname]['formcontentok']&& $this->forms[$formname]['formok']);
	}
	public function UrlToArray($url){
		$trimmed =  explode('&',strstr($url,'?'));
   	    $data= [];
   		foreach($trimmed as $paraminf){
			//print $paraminf;
			$parts = explode('=',$paraminf);
			$data[$parts[0]]= (key_exists(1,$parts))?$parts[1]:'';
		}
		return $data;
	}
	public function getFormData($formname){
		if(!array_key_exists($formname,$this->forms)||!array_key_exists('formdata',$this->forms[$formname])){
			return false;
		}
		return $this->forms[$formname]['formdata'];
	}
	public function HoldForm($user,$keyname){
		
	}
    private function logicValidation($fieldtype ,&$value){
	  switch($fieldtype){
	      case 'name':
		  case 'username':
	         return ctype_alnum($value)&&preg_match('/^[A-Za-z0-9]*$/',$value);break;
		  case 'pwx':
		  case 'pwxe':
	         return ctype_alnum($value)&&preg_match('/^[A-Za-z0-9]*$/',$value);break;		  
		  case 'Email':
	      case 'email':
	      case 'isAemail':
	      	  return (\autosystem\ValidateAs::isAemail($value));  break;
	      case 'isbool':
	      		$ret = ($value = ($value =='on')?'TRUE':'FALSE');
	      	  return $ret; break;
	      case 'PhoneNR':
	      case 'phone':
	          return (\autosystem\ValidateAs::isnumeric($value)); break;
          case 'inselect':
              if(is_array($this->forms[$formname]['select'][$field])){
                print 'isarray';
                 return in_array($value,$this->forms[$formname]['select'][$field]);
              }else{
                 return false; 
              }
	      case 'page':
	      case 'id':
	      case 'isnumeric':
	      case 'category_id':
          case 'int'://database datatype
	          return ( \autosystem\ValidateAs::isnumeric($value)&&(int)$value);break;
	      case 'istext':
	          return ( \autosystem\ValidateAs::isTEXT($value));break;
	      case 'iscode':
	          return ( \autosystem\ValidateAs::isFULLTEXT($value));break;    
	      case 'isurl':
	          return ( \autosystem\ValidateAs::isURL($value));break;
	      case 'isdecimal':
	          return  true;break;
	      case 'isdate':
	          return ( \autosystem\ValidateAs::isAdate($value)&& $value = $this->checkDate($value,$this->dateformattosave));break;
	      case 'istime':
          case 'timestamp'://database datatype
	          $value = $this->prepairtime($value,$isvalid);
	     	  return $isvalid;	
		  case 'LOB':
	          return true ;break;
          case 'isY':
	          return ( \autosystem\ValidateAs::isYear($value));break;      
          case 'isM':
              $value = $this->nullair($value,2);
	          return  \autosystem\ValidateAs::isMonth($value);break;
   	      case 'isD':
              $value = $this->nullair($value,2);
	          return \autosystem\ValidateAs::isDay($value);break;    
	      default:
	        return true;
	      }
	}
    private function nullair($value,$len){
        for($i= strlen($value.'');$i<$len;$i++){
            $value = '0'.$value;
        } 
        return  $value;
    }
    private function superValidation($fieldtype ,&$value,$formname){
	  switch($fieldtype){
          case 'inselect':
              if(is_array($this->forms[$formname]['select'][$field])){
                return in_array($value,$this->forms[$formname]['select'][$field]);
              }else{
                return false; 
              }
          	

	      default:
	        return true;
	      }
	}

	private function prepairtime($time,&$isvalid){
		if(\autosystem\ValidateAs::isTimeHMS($time)){
			$isvalid=true;
			return $time;
		}
		print '2'.$time;
		if(\autosystem\ValidateAs::isTimeHM($time)){
			$isvalid=true;
			return $time.':00';
		}
		print '3'.$time;
		if(\autosystem\ValidateAs::isBadbrowserTime($time)){
			$t = explode(':',$time);
			$x = strtolower(array_pop($t));
			if($x =='am'){
				array_push($t ,'00');
			}
			if($x =='am'){
				$t[0]=intval($t[0]) +12;
				array_push($t ,'00');
			}
			$isvalid=true;
			return $time.':00';
		}
		print '4'.$time;
		return '00:00:00';
	}
	private function checkDate($date,$type){
        $a = trim($date);
        $a = str_replace([' ','/'],'-',$a);
        if(\autosystem\ValidateAs::isUSAdate($a)){
            if($type =='US'){ return $a;    }
            if($type =='EU'){ return implode('-',array_reverse(explode('-',$a)));   }
        }
        if(\autosystem\ValidateAs::isEUAdate($a)){
            if($type =='EU'){ return $a;    }
            if($type =='US'){ return implode('-',array_reverse(explode('-',$a)));   }
        }
        return false;
    }
	public function errorinfo(&$view,$formname){
		$error ='';
		if(!array_key_exists($formname,$this->forms)){
			return false;
		}
		foreach($this->forms[$formname]['errors'] as $fieldname => $msg){
			$error .= '<br>'.$formname.'.'.$fieldname.'.'.$msg;
		}
		return $error;
	}
    public function getActiveForms(){
        $forms =[];
        foreach($this->forms as $formname => $fieldname){
            if($this->forms[$formname]['checked']){
                $forms[]= $formname;
            }
        }
        return $forms;
    }
	public function status(&$view ,$check=false){
		foreach($this->forms  as $formname=>$data){
			$array['id'] = 'consol_output';
        	$array['title'] = 'A-VALIDATOR:-['.$formname.']-';
        	print '<div class="validator" >';
			$returnstring  = '';
			$info2='<div class="validatormoreinfo" ><pre><table><tr>';
			if(!$check ||$this->forms[$formname]['checked']){
				foreach($data  as $categoryname => $cdata){
					if(is_array($cdata)){
						$info = ' '.'<pre>'.$categoryname;
						if(count($cdata)>0){
							$info .= '<table><tr><td> '.implode(' </td><td> ',array_keys($cdata)).'</td></tr><tr><td> ';
							if(is_array($cdata)){	$info .=implode(' </td><td> ',array_values($cdata));	}
							$info .='</td></tr></table>';
						}
						$info .=  '</pre>';
						$returnstring  .= $view->get_template('global','validatorinfo',['validinfo'=>$info]);
					}else{
						$info2 .=  '<td>["'.$categoryname.':'.$cdata.'"';
						if($cdata ==1 ){	$info2 .='(bool)TRUE';	}
						if($cdata ==0){		$info2 .='(bool)FALSE';	}
						$info2 .=  ']</td>';
					}
				}
			}else{
				print 'NOT active form';
			}
			$info2 .=  '</tr></table></pre>';
			print $view->get_template('global','section_head',$array).$info2.$returnstring;
        	print $view->get_template('global','section_foot',[]);
        	print '</div>';
		}
	}
}
?>
