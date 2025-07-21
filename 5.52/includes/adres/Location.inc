<?php
	namespace adres;
    class Location extends \autosystem\Controller{
        private  $values=array('add');
        public function init(){
           $this->getModel();
           if(key_exists('Acomplete',$_GET)){$this->Acomplete();                return;}
           $this->vars();
           $this->prepare($this->_loc,$this->_action,$this->_values,$this->_values['search']);
        }
        private function prepare($loc,$action,$values,$find =null){
           print $this->_view->getheader($loc,$action);
           print $this->_view->get_template($this->_loc,$this->_action,$this->_values);
           $GETSOMETHING = true;
           $template ='';
           if(isset($this->_values['view'])){
                $this->_model->GET_ALocation($this->_loc,'Location_view',$this->_view,$this->_values['view'],$this->getAccound_id());
                $GETSOMETHING = false;
           }
           if(isset($_GET['find'])){
                print $this->_view->getform_head($this->_loc,$this->_action);
                print $this->_view->get_template($this->_loc,'find',$this->_values);
                print $this->_view->getform_end('','find');
                if($this->_values['section']!=''){
                     $this->_model->FIND($this->_loc,'found_item',$this->_view,$this->_values['q'],$this->_model->section[$_POST['section']]);
                }
                print $this->_view->get_template('global','AutoCompleteList');
                print '<h1>test</h1>';
                $GETSOMETHING = false;
           }
           if(isset($_GET['summary'])){
                 print $this->_view->get_template($this->_loc,'item_head');
                 $this->_model->GET_LocationSummary($this->_loc,'locationItem',$this->_view);
                 print $this->_view->get_template($this->_loc,'item_foot');
                $GETSOMETHING = false;
           }
           if($GETSOMETHING){

           }
           print $this->_view->getfooter($action);
        }
        private function vars(){

            $this->_values = $this->_view->get_translations($this->values);
            $this->_values['title']='data:';
            $this->_values['direction']= '';
            $this->_values['search']= '';
            //$categorys = $this->_model->GET_Locations();
            $this->_validator->addForm('find',['search','q','section'],false,[''],$this->_values);
	        if(key_exists('find',$_POST)){
	            $this->_validator->addAlocat('find',['q'=>'istext']);
	            $this->_validator->render('find',$_POST,$this->_values);
	        }
			
	        if(key_exists('view',$_GET)){
	        	$this->_validator->addForm('view',['view','section'],false,['section'],$this->_values);
	            $this->_validator->addAlocat('find',['view'=>'isnumeric']);
	            $this->_validator->render('find',$_GET,$this->_values);
	        }
			/*if(isset($_GET['find'])){
                $this->_values['search']=(isset($_POST['search']))?$_POST['search']:'';
                $this->_values['q']=(isset($_POST['q']))?$_POST['q']:$this->_values['search'];
                $this->_values['section']=(isset($_POST['section'])&&key_exists($_POST['section'],$this->_model->section))?$_POST['section']:'';
              */  $this->_values['sections']=$this->_view->getOptions($this->_model->section,$this->_values['section']);  
            //}

            $this->_values['location_directions']=$this->_view->getOptions($this->_model->directions,$this->_values['direction']);
        }
        private function Acomplete(){
           $minsize=10;
           $this->_values['field']=(isset($_POST['field']))?$_POST['field']:'null';;
           $this->_values['autocomlete']=(isset($_POST['q'])&&strlen($_POST['q'])>0)?$_POST['q']:'null';
           if(key_exists('Acomplete',$_GET)&& $this->_values['autocomlete']!='null' ){
               $return = '{"Acompletelist":{}}'; //$startwith,$field=null
               $data = $this->_model->AcompleteLocation($this->_values['autocomlete'],$value);          
               $return = json_encode($data);
               print $return;   
               return;     
           }
        }
    }
?>