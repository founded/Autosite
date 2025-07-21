<?php
	namespace users;
	class ContactInfo extends \autosystem\Controller{
        private $values;
        public function init() {
           $this->getModel(); 
           if(isset($_GET['check'])){
                $cansee = $_POST['visibility'];
                if(is_array($cansee)){
                    $this->saveVisibility($cansee,$this->getAccound_id());
                    $result = ['ok'=>true];
                    header('HTTP/1.0 200 ');print json_encode($result);
                    return;
                }
                header('HTTP/1.0 504 Your To Slow for me');
                return ;
           }
           $this->vars();
           if(count($_POST)>0){
                $issaved = false;
                if(isset($this->_values['add_Phone'])&&$this->canSave()&& $id = parent::getAccound_id()){
                      $issaved=$this->_model->save_telPhone($this->_values['PhoneNR'],$this->_values['phone_type'],$id);    
                }
                if(isset($this->_values['save_Phone'])&&$this->canSave()&& $id = parent::getAccound_id()){
                     $issaved=$this->_model->update($this->_values['PhoneNR'],$this->_values['phone_type'],$id);
                }
               if($issaved){
                        $this->_values['title']=$this->_view->get_AtranslationB('saved');
               }                      
               $this->goToProfile(true);
           }
           if(key_exists('delete_phone',$this->_values)&&$this->canDelete()&&$id = parent::getAccound_id()){
                    if($this->_model->delete_telPhone($this->_values['delete_phone'] ,$id)){
                        $this->_values['title']=$this->_view->get_AtranslationB('deleted');
                    }
               $this->goToProfile(true);
           }
           $this->prepare($this->_loc,$this->_action,$this->_values);
        }
        private function prepare($loc,$action,$values,$find =null){
           print $this->_view->getheader($loc,$action);
           $accound_id = parent::getAccound_id();
           if(isset($find)&& trim($find)!=''){
                print $this->_view->get_template($this->_loc,'findresult',$this->_values);
           }
           $GETSOMETHING = true;
           if(isset($_GET['add_Phone'])){
                print $this->_view->getform_head($this->_loc,$this->_action);
                print $this->_view->get_template($this->_loc,'Phone_add',$this->_values);
                print $this->_view->getform_end('','add_Phone');
                $GETSOMETHING = false;
           }
           if(isset($this->_values['tel_edit'])){
                print $this->_view->getform_head($this->_loc,$this->_action);
                $a = $this->_model->GETArray('','','',$this->_values['edit']);
                $c = $a+ $this->_values;
                $categorys = null;
                foreach($this->categorys as $key => $value){
                    $categorys[$key]=$this->_values[$value];
                }
                $c['action_title']= $a['title'];
                $c['auction_category']=$this->_view->getOptions($categorys,$c['category_title']);
                $c['auction_condition']= $this->_view->getOptions($this->_model->condition,$c['CONDITION']);
                $c['auction_type']= $this->_view->getOptions($this->_model->type,$c['TYPE']);
                print $this->_view->get_template($this->_loc,'add',$c);
                print $this->_view->getform_end('','save');
                $GETSOMETHING = false;
           }
           if(isset($this->_values['view'])){
                if(!$this->_model->GET($this->_loc,'Mydetail',$this->_view,$this->_values['view'])){
                    $array['id'] = 'info';
                    $array['title'] = 'ERROR: '.$this->_values['PAGENOTFOUND'];;
                    print $this->_view->get_template('global','section_head',$array);
                    print $this->_view->get_template($this->_loc,'HTTP404');
                    print $this->_view->get_template('global','section_foot');
                }
                $GETEVERYTHINGELSE = false;
           }
           if($GETSOMETHING){
                 //$location,$name,$view,$filter,$category_id="",$accound_id=""
                 //$this->_model->GET_AOverView($this->_loc,'userItem',$this->_view,"","",$accound_id)
           }
           print $this->_view->getfooter($action);
        }
        private function vars(){
            $this->_values['search']='';
            $this->_values['phonetype']='';
            $this->_values['indicationprice']='NOTK';
            if(isset($_POST['find'])){
                    $this->_values['search']=$_POST['search'];       
            }else{
                if(count($_POST)>0){
                    $this->_values['title']='data:';
                    //print_r($_POST);
                    $this->_values['PhoneNR']= (isset($_POST['PhoneNR'])&&\autosystem\ValidateAs::isnumeric($_POST['PhoneNR']))?$_POST['PhoneNR']:"";;
                    $this->_values['phone_type']=(key_exists($_POST['phone_type'],$this->_model->phonetypes))?$_POST['phone_type']:"";
                }
                if(isset($_GET['view'])){
                    $this->_values['view']=(isset($_GET['view'])&&\autosystem\ValidateAs::isnumeric($_GET['view']))?$_GET['view']:"";
                }
                if(isset($_GET['edit'])){
                    $this->_values['edit']=(\autosystem\ValidateAs::isnumeric($_GET['edit']))?$_GET['edit']:null;
                }
                if(key_exists('delete_phone',$_GET)){
                    $this->_values['delete_phone']=(\autosystem\ValidateAs::isnumeric($_GET['delete_phone']))?$_GET['delete_phone']:null;
                }
                if(isset($_POST['add_Phone'])){     $this->_values['add_Phone']='';        }
                if(isset($_POST['save_Phone'])){    $this->_values['save_Phone']='';       }
            }
            $this->_values['phone_types']=$this->_view->getOptions($this->_model->phonetypes,$this->_values['phonetype']);
        }
        private function save_tel(){
            if(count($_POST)>0){
                if(isset($this->_values['additem'])&&$this->canSave()&& $id = parent::getAccound_id()){
                     $issaved=$this->_model->save(                    $id);
                     if($issaved){
                        $this->_values['title']=$this->_view->get_AtranslationB('saved');
                     }                      
                     $this->redirect("./$this->_loc/$this->_action/");
                }
                if(isset($this->_values['save'])&&isset($this->_values['edit'])&&$this->canSave()&& $id = parent::getAccound_id()){
                     $issaved=$this->_model->update($id);
                     if($issaved){
                        $this->_values['title']=$this->_view->get_AtranslationB('saved');
                     }
                }               
           }
        }
        private function saveVisibility($asoarr,$userid){
           foreach($asoarr as $key=>$value){
                if(\autosystem\ValidateAs::isCheckedOrNot($value)){
                    $this->_model->setVisibility($userid,$key,$value);
                }else{
                    return false;
                }
            }
            return true;       
        }
        private function canSave(){
            return true;
        }
        private function canDelete(){
            return true;
        }
    }
?>