<?php
	namespace images;
    class Gallery extends \autosystem\Controller{
        private $values = ['unconstruct'];
        public function init() {
           $this->vars();
           $this->getModel();
           $this->prepare($this->_loc,$this->_action,$this->_values);
        }
        private function prepare($loc,$action,$values,$find =null){
           print $this->_view->getheader($loc,$action);
           $template ='';
           if(isset($find)){
                print $this->_view->get_template($this->_loc,'findresult',$this->_values);
           }
           $GETEVERYTHINGELSE = true;
           if($GETEVERYTHINGELSE){
           }
           print $this->_view->getfooter($action);
        }
        private function vars(){
            $this->_values = $this->_view->get_translations($this->values);
            

        }
        
    }
?>