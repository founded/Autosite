<?php
	namespace adres;
    class AutoCompleteLocation extends \autosystem\Controller{
        private  $values=['add'];
        public function init(){
           $this->getModel();
           if(key_exists('Acomplete',$_GET)&&$id = parent::getAccound_id()){
                //$data = ['list' => [ 0=>[ 0, "alpaca"],1=> [1=>"buffalo"],2=> [2=>"cat"],3=> [3=>"tiger"]]];
                $fields=['country','postcode','street','town','state','city'];
                $return = '{"Acompletelist":{}}';
                foreach($_POST as $key=>$value){
                    if(in_array($key,$fields)&& \autosystem\ValidateAs::isTherm($value)){
                        if($this->_view->isCachIT()&&$this->_view->existcach($this->_loc,'Acomplete_'.$value,'Autocomplete/'.$this->_loc.'/')){
                           $return = $this->_view->getcach($this->_loc,'Acomplete_'.$value,'Autocomplete/'.$this->_loc.'/');
                        }else{
                           $data = $this->_model->AcompleteLocation($key,$value);
                           $return = json_encode($data);
                           //print $this->_view->isCachIT();
                            if($this->_view->isCachIT()){   
                              if(count($data['Acompletelist'])>1){
                                    //print 'Autocomplete/'.$this->_loc.'/'; 
                                    $this->_view->makecach($this->_loc,'Acomplete_'.$value,'Autocomplete/'.$this->_loc.'/');
                              }
                            }
                        }  
                    }
                }
                print $return;   
                    
           }
           $this->vars();
           $this->prepare();
        }
        private function prepare(){
           if(isset($_GET['test'])){
                print $this->_view->getheader($this->_loc,$this->_action);
                print $this->_view->getform_head($this->_loc,$this->_action);
                print $this->_view->get_template($this->_loc,'testAutocomplete',$this->_values);
                //print $this->_view->get_template($this->_loc,'Autocomplete',$this->_values);
                print $this->_view->getform_end('','test');
                print $this->_view->getfooter($this->_action);
           }
        }
        private function vars(){
            $enum_translations=[];
            $this->_values = $this->getTranslations();
            $this->_view->setENUM_TRANSLATIONS($this->_values);
            
        }
    }
?>