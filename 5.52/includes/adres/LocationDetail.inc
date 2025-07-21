<?php
	namespace adres;
    class LocationDetail extends \autosystem\Controller{
        private  $values=array('add');
        public function init() {
           $this->getModel();
           if(key_exists('delete_adres',$_GET)&&$id = parent::getAccound_id()){
                if($this->canDelete($id)&& $this->_model->delete($_GET['delete_adres'] ,$id)){
                    $this->goToProfile(true);
                }
           }
           $this->vars();
           if(isset($_POST['add_adres'])){
                if($this->canSave()){
                    $issaved=$this->_model->save_adres(
                        $this->getAccound_id(),$this->locale,            
                        $this->_values['street'],$this->_values['houseNumber'],$this->_values['bus'],
                        $this->_values['postcode'],$this->_values['town'],$this->_values['adres'],
                        $this->_values['city'],$this->_values['state'],$this->_values['country'],
                        $this->_values['gpslat'],$this->_values['gpslng']
                    );  
                     if($issaved){
                        $this->_values['title']=$this->_view->get_AtranslationB('saved');
                     }
                     $this->GOESTO('users','Profile');
                }
           }
           $this->prepare($this->_loc,$this->_action,$this->_values);
        }
        private function prepare($loc,$action,$values){
           print $this->_view->getheader($loc,$action);
           print $this->_view->get_template($this->_loc,$this->_action,$this->_values);
           if(isset($find)&& trim($find)!=''){
                print $this->_view->get_template($this->_loc,'findresult',$this->_values);
           }
           $GETSOMETHING = true;
           $template ='';
           if(isset($_GET['add_adres'])){
                print $this->_view->getform_head($this->_loc,$this->_action);
                print $this->_view->get_template($this->_loc,'Add_adres',$this->_values);
                print $this->_view->getform_end('','add_adres');
                $GETSOMETHING = false;
           }
           if(isset($this->_values['edit_adres'])){
                print $this->_view->getform_head($this->_loc,$this->_action);
                $this->_model->GET_Adres($this->_loc,'Edit_adres',$this->_view,$this->getAccound_id(),$this->_values['edit_adres']);
                print $this->_view->getform_end('','save_adres');
                $GETSOMETHING = false;
           }
           if($GETSOMETHING){
                 //print $this->_view->get_template($this->_loc,'item_head');
                 $this->_model->GET_LocationSummary($this->_loc,$this->_action,$this->_view,'');
                 //print $this->_view->get_template($this->_loc,'item_foot');
           }
           print $this->_view->getfooter($action);
        }
        private function vars(){
            $enum_translations=[];
            $this->_values = $this->getTranslations();
            $this->_view->setENUM_TRANSLATIONS($this->_values);
            $this->_values['title']='data:';
            //print_r($_POST);
            $this->_values['adres']= '';
            $this->_values['adres_E']='';
            $this->_values['postcode']='';
            $this->_values['town']='';
            $this->_values['street']='';
            $this->_values['houseNumber']='';
            $this->_values['bus']='';
            $this->_values['state']='';
            $this->_values['country']='';
            $this->_values['city']='';
            $this->_values['gpslat']='';
            $this->_values['gpslng']='';
            $this->_values['adres_type']='';
            if(count($_POST)>0){
                if(isset($_POST['find'])){
                    $this->_values['search']=$_POST['search'];       
                }else{
                    $this->_values['title']='data:';
                    //print_r($_POST);
                    $this->_values['adres']=(\autosystem\ValidateAs::isTEXT($_POST['adres']))?$_POST['adres']:'';
                    $this->_values['adres_E']= (\autosystem\ValidateAs::isTEXT($_POST['adres_E']))?$_POST['adres_E']:'';
                    $this->_values['postcode']=(\autosystem\ValidateAs::isTEXT($_POST['postcode']))?$_POST['postcode']:'';
                    $this->_values['town']=(\autosystem\ValidateAs::isTEXT($_POST['town']))?$_POST['town']:'';
                    $this->_values['street']=(\autosystem\ValidateAs::isTEXT($_POST['street']))?$_POST['street']:'';
                    $this->_values['houseNumber']=(\autosystem\ValidateAs::isTEXT($_POST['houseNumber']))?$_POST['houseNumber']:'';
                    $this->_values['bus']=(\autosystem\ValidateAs::isTEXT($_POST['bus']))?$_POST['bus']:'';
                    $this->_values['state']=(\autosystem\ValidateAs::isTEXT($_POST['state']))?$_POST['state']:'';
                    $this->_values['country']=(\autosystem\ValidateAs::isTEXT($_POST['country']))?$_POST['country']:'';
                    $this->_values['city']=(\autosystem\ValidateAs::isTEXT($_POST['city']))?$_POST['city']:'';
                    $this->_values['gpslat']=(\autosystem\ValidateAs::isTEXT($_POST['gpslat']))?$_POST['gpslat']:'';
                    $this->_values['gpslng']=(\autosystem\ValidateAs::isTEXT($_POST['gpslng']))?$_POST['gpslng']:'';
                }
            }
                if(isset($_GET['view_adres'])){
                    $this->_values['view_adres']=(isset($_GET['view'])&&\autosystem\ValidateAs::isnumeric($_GET['view']))?$_GET['view']:'';
                }
                if(isset($_GET['edit_adres'])){
                    $this->_values['edit_adres']=(\autosystem\ValidateAs::isnumeric($_GET['edit_adres']))?$_GET['edit_adres']:null;
                }
                if(isset($_POST['add_adres'])){
                    $this->_values['add_adres']='';
                }
                if(isset($_POST['save_adres'])){
                    $this->_values['save_adres']='';
                }
            $this->_values['adres_type']= $this->_view->getOptions($this->_model->types,$this->_values['adres_type']);
        }
        private function canSave(){
            $valid = true;
            return $valid;
        }
        private function canDelete($id){
            return true;
        }
    }
?>