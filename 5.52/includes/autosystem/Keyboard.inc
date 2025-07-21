<?php
/**
* @author Lieven Roegiers
* @copyright 
* @CMS autosite
*/
namespace autosystem;
class Keyboard {
	private $keyboard = [];
    public function __toString(){            return '=>class:Keyboard=>[data=>'.print_r($keyboard,true).']';
    }
    public function render(&$view ,$type = 'querty',$debug = false){
    	$this->SetType($type); 
	    $i = 0;
	    $return = '';
	    if($debug){
			print '<input type="text" name="test" id="test" value="test" onclick="settextselectedfield(\'test\')" >';
			print '<input type="text" name="tost" id="tost" value="tost" onclick="settextselectedfield(\'tost\')" >';
			print '<h1>'.$type.'</h1>';
		}else{
			$values['keyboardtype'] = $type;
	        $return .= $view->get_Templateclone('global','keyboardtop',$values);
		}
		foreach($this->keyboard as $rownr => $keys){
			if($debug){
				foreach($keys as $keynr => $key){		
					print '<input type="button" value="'.$key.'" class="keybtn btngrey keylarge" name="'.$key.'" id="'.$key.'" onclick="keyboardpush(\''.$key.'\')" accesskey="'.$key.'" title="key '.$key.'" alt="'.$key.'" >';
				}
				if($rownr == 0){	print '<input type="button" class="keybtn btngrey keylarge" value="%backspace%" name="backspace" id="backspace" onclick="keyboardbackspace()" >';			}
				if($rownr == 1){	print '<input type="submit" class="keybtn btngrey " value="%enter%" name="enter" id="enter" >';			}
			}else{
				foreach($keys as $keynr => $key){
					$values['keyboardkey'] = $key;
					$return .= $view->get_Templateclone('global','keyboard_key',$values);
			    }
			    if($rownr == 0){	$return .= $view->get_Templateclone('global','keyboard_key_backspace',$values);		}
				if($rownr == 1){	$return .= $view->get_Templateclone('global','keyboard_key_enter',$values);	}
			}
			$return .= '<br>';
		}
		return $return;
	}
	public function SetType($type,$data=[],$install = false){
		if($install == true ){
			$keyboards[$type]=$data;
		}
		$keyboards['azerty'][0] = ['1','2','3','4','5','6','7','8','9','0'];
		$keyboards['azerty'][1] = ['a','z','e','r','t','y','u','i','o','p'];
		$keyboards['azerty'][2] = ['q','s','d','f','g','h','j','k','l','m'];
		$keyboards['azerty'][3] = ['w','x','c','v','b','n'];
		$keyboards['querty'][0] = ['1','2','3','4','5','6','7','8','9','0'];
		$keyboards['querty'][1] = ['q','w','e','r','t','y','u','i','o','p'];
		$keyboards['querty'][2] = ['a','s','d','f','g','h','j','k','l'];
		$keyboards['querty'][3] = ['z','x','c','v','b','n','m'];
        $keyboards['querty'][3] = ['@','&euro;','.','_','-'];
		$keyboards['numpath'][3] = ['0','1','2','3','4','5','6','7','8','9'];
		$this->keyboard = $keyboards[$type];
	}
}
?>

