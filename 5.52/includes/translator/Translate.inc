<?php
namespace translator;
class Translate extends \autosystem\Controller{
    private $values = array('from','to');
    public function init() {
        $this->vars();
       if(isset($_POST['save'])){
            $this->getModel();
            $issaved=false;
            if($this->validate($this->_values['localesrc'],$this->_values['key'],$this->_values['from'])){
                $issaved=$this->_model->save($this->_values['localesrc'],$this->_values['key'],$this->_values['from']);
            }
            if($this->validate($this->_values['locale'],$this->_values['key'],$this->_values['to'],true)){
                $issaved=$this->_model->save($this->_values['locale'],$this->_values['key'],$this->_values['to']);
            }
            if($issaved){
                 $this->_values['title']=$this->_view->get_AtranslationB("saved");
            }
            $this->prepare($this->_loc,$this->_action,$this->_values);
            return;
       }
       $this->getModel();
       $this->prepare($this->_loc,$this->_action,$this->_values);
    }
    private function validate($locale,$key,$val,$dest = false){
        if($this->_model->exist($locale,$key)&&$dest==false){
            return false;
        }
        if(trim($val) ==''){            return false;        }
        if(trim($locale) ==''){         return false;        }
        return true;
    }
    public function __call($name, $arguments) {
        print '<h1>'.$name.$arguments.'</h1>';
    }
    private function islocale($input){
        return isset($input);
    }
    private function vars(){
        $this->_values = $this->_view->get_translations($this->values);
        $this->_values['key']=filter_input(INPUT_GET, 'key');
        $this->_values['title']=$this->_view->get_AtranslationB('translatekey').$this->_values['key'];
        $this->_values['localesrc']=strtolower($this->_defaultLocale);
        if(isset($_POST['localesrc'])&& $this->islocale($_POST['localesrc'])){
            $this->_values['localesrc']=strtolower($_POST['localesrc']);
        }
        if(isset($_GET['localesrc'])&& $this->islocale($_GET['localesrc'])){
            $this->_values['localesrc']=strtolower($_GET['localesrc']);
        }
        if(isset($_POST['locale'])){
            $this->_values['locale']=strtolower($_POST['locale']);
        }elseif(isset($_GET['locale'])){
            $this->_values['locale']=strtolower($_GET['locale']);
        }else{
            $this->_values['locale'] = 'en';
        }
        if(isset($_POST['search'])){
            $this->_values['search'] = $_POST['search'];
        }else{
            $this->_values['from']= (isset($_POST['from']))?filter_input(INPUT_POST, 'from'): $this->_view->get_Atranslation($this->_values['key'],$this->_values['localesrc']);
            $this->_values['to']=(isset($_POST['to']))?filter_input(INPUT_POST, 'to'):$this->_view->get_Atranslation($this->_values['key'],$this->_values['locale']);
        }
        /*$this->_validator->addForm('translate',['from','to'],false,[''],$this->_values);
		if(isset($_POST['-'])){
            //$this->_validator->addAlocat('translate',[]);
			$this->_validator->render('translate',$_POST,$this->_values);
           //TODO check generated code
        }*/
    }
    private function prepare($loc,$action,$values){
       print $this->_view->getheader($loc,$action);
       print $this->_view->get_template($this->_loc,$this->_action,$this->_values);
       print '<table>';
       $this->_model->GET_RelatedView($this->_loc,'item',$this->_view,$this->_values['localesrc'],$this->_values['locale'],$this->_values['from']);
       print '</table>';
       print $this->_view->getfooter($action);
    }
}
?>