<?php
	namespace adres;
    class ModLocation extends \autosystem\Controller{
        private  $values=array('add','adres.lookup.sum.title');
        private  $selects = ['postcode','town','city','state','province','region','locale','country'];
        public function init() {
           $this->getModel();
           if(isset($_GET['check'])){
                if(key_exists('setLocationChecked',$_POST)&& is_array($_POST['setLocationChecked'])){
                    $this->saveChecked($_POST['setLocationChecked']);
                    $result = ['ok'=>true];
                    header('HTTP/1.0 200 ');print json_encode($result);
                    return;
                }
                header('HTTP/1.0 504 Your To Slow for me');
                return ;
           }
           if(isset($_GET['complete'])){
                if(key_exists('country',$_POST)){
                    //$this->GET_COMPLETE($_POST['country']);
                    $result = $this->_model->GET_COMPLETE($_POST['country']);
                    header('HTTP/1.0 200 ');print json_encode($result);
                    return;
                }
                header('HTTP/1.0 504 Your To Slow for me');
                return ;
           }
           $this->vars();
           if(isset($_POST['add'])){
                if($this->canSave()){
                        $issaved=$this->_model->save_Location($this->_values['type'],
                        $this->_values['category'],
                        $this->_values['action_title'],
                        $this->_values['condition'],
                        $this->_values['comment'],
                        $this->_values['detail'],
                        $this->_values['status'],
                        $this->_values['weblocation']);  
                     if($issaved){
                        $this->_values['title']=$this->_view->get_AtranslationB("saved");
                     }                        
                     $this->redirect("./$this->_loc/$this->_action");
                }
           }
           $this->prepare($this->_loc,$this->_action,$this->_values);
        }
        private function prepare($loc,$action,$values,$find =null){
           print $this->_view->getheader($loc,$action);
           print $this->_view->get_template($this->_loc,$this->_action,$this->_values);

           $GETSOMETHING = true;
           $template ="";
           if(isset($_GET['addLocation'])){
                print $this->_view->getform_head($this->_loc,$this->_action);
                print $this->_view->get_template($this->_loc,'location_add',$this->_values);
                print $this->_view->getform_end("","add");
                $GETSOMETHING = false;
           }
           if(isset($this->_values['view'])){
                $this->_model->GET_ALocation($this->_loc,'Location_view',$this->_view,$this->_values['view'],$this->getAccound_id());
                $GETSOMETHING = false;
           }
           if(isset($_GET['equals'])){
                $array['id'] = 'info';
                $array['title'] = $this->_values['adres.lookup.sum.title'];
                print $this->_view->get_template('global','section_head',$array);
                $filter =[];
               $this->_model->GET_EqualsSummary($this->_loc,'equals_items',$this->_view,$this->getAccound_id(),$filter);
                print $this->_view->get_template('global','section_foot');
                $GETSOMETHING = false;
           }
           if(isset($_GET['merge'])&&isset($_GET['postcode'])){
                print $this->_view->getform_head($this->_loc,$this->_action);
                print $this->_view->get_template($this->_loc,'location_merge',$this->_values);
                print $this->_view->getform_end('','add');
                $this->_model->GET_LocationSummary($this->_loc,'found_item',$this->_view,'',$_GET['postcode']);
                $GETSOMETHING = false;
           }
           if(isset($_GET['Summary'])){
                print $this->_view->getform_head($this->_loc,$this->_action);
                print $this->_view->get_template($this->_loc,'add',$this->_values);
                print $this->_view->getform_end('','add');
                $this->_model->GET_LocationSummary($this->_loc,$this->_action,$this->_view,'');
                $GETSOMETHING = false;
           }
           if(isset($_GET['notchecked'])){
                $filter =[];
                $this->_model->GET_NOTchecked($this->_loc,'notchecked_item',$this->_view,$this->getAccound_id(),$this->_values['page'],$filter);
                print $this->_view->get_template('global','toolcheck_end',$this->_values);
                $GETSOMETHING = false;
           }
           if(isset($_GET['addLocale'])){
                print $this->_view->getform_head($this->_loc,$this->_action);
                print $this->_view->get_template($this->_loc,'addLocale_add',$this->_values);
                print $this->_view->getform_end('','add');
                $GETSOMETHING = false;
           }
           if(isset($_GET['LocalesSummary'])){
                print $this->_view->getform_head($this->_loc,$this->_action);
                print $this->_view->get_template($this->_loc,'location_add',$this->_values);
                print $this->_view->getform_end('','add');
                $GETSOMETHING = false;
           }
           if(isset($this->_values['Checklocales'])){
                $array['id'] = 'info';
                $array['title'] = $this->_values['locales.sum.title'];
                print $this->_view->getform_head($this->_loc,$this->_action);
                print $this->_view->get_template($this->_loc,'localesCheck',$this->_values);
                print $this->_view->getform_end('','Check');
                $GETSOMETHING = false;
                if(isset($_POST['Checklocales'])){
                     $datasplitter = new \data\DataSplit();
                     $regex = "|(\.(?<ext>[A-Z\-\.a-z]+))( +(?<url>[A-Z0-9\-\.a-z]+))( #+(?<comment>.*))?\n?|";
                     $needle = ['ext','url','comment'];
                     $array = $datasplitter->spliter($this->_values['tocheck'],$regex ,$needle);
                     foreach($array as $key=>$value){
                           $vals = $value;
                           $vals['is']=$this->_view->getCheckImg($this->_model->existwhoisserver($value['ext'],$value['url'])); 
                           print $this->_view->get_template($this->_loc,'localesCheck_item',$vals);
                     }
                     //print "<pre>".print_r($array,true)."</pre>";
                }
           }
           if(isset($_GET['testautocomplete'])){
                $filter =[];
                print $this->_view->get_template($this->_loc,'testAutocomplete',$this->_values);
                print $this->_view->get_template('global','toolcheck_end',$this->_values);
                $GETSOMETHING = false;
           }
           if(isset($_POST['find'])){
                print $this->_view->getform_head($this->_loc,$this->_action);
                print $this->_view->get_template($this->_loc,'find',$this->_values);
                print $this->_view->getform_end('',"find");
                if($this->_values['section']!=''){
                     $this->_model->FIND($this->_loc,'found_item',$this->_view,$this->_values['q'],$this->_model->section[$_POST['section']]);
                }
                $GETSOMETHING = false;
           }
           if(isset($_GET['search'])){
                print $this->_view->getform_head($this->_loc,$this->_action);
                print $this->_view->get_template($this->_loc,'add',$this->_values);
                print $this->_view->getform_end('','add');
                $this->_model->GET_LocationSummary($this->_loc,$this->_action,$this->_view,'');
                $GETSOMETHING = false;
           }
           if($GETSOMETHING){
                 //print $this->_view->get_template($this->_loc,'item_head');
                 $this->_model->GET_LocationSummary($this->_loc,'found_item',$this->_view,'');
                 //print $this->_view->get_template($this->_loc,'item_foot');
           }
           print $this->_view->getfooter($action);
        }
        private function vars(){
            $this->_values = $this->_view->get_translations($this->values);
            $this->_values['title']='data:';
            //print_r($_POST);
            $this->_values['direction']= '';
            $this->_values['search']= '';
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
            $categorys = $this->_model->GET_Locations();
            if(isset($_POST['find'])){
                    $this->_values['search']=(isset($_POST['search']))?$_POST['search']:'';
                    $this->_values['q']=(isset($_POST['q']))?$_POST['q']:$this->_values['search'];
                    $this->_values['section']=(isset($_POST['section'])&&key_exists($_POST['section'],$this->_model->section))?$_POST['section']:"";
                    $this->_values['sections']=$this->_view->getOptions($this->_model->section,$this->_values['section']);       
            }else{
                if(count($_POST)>0){ 
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
                if(isset($_GET['view'])){
                    $this->_values['view']=(isset($_GET['view'])&&\autosystem\ValidateAs::isnumeric($_GET['view']))?$_GET['view']:'';
                }
                if(isset($_GET['addLocales'])){
                    $this->_values['addLocale']=(isset($_GET['addLocale']))?true:null;
                }
                if(isset($_GET['merges'])){
                    $this->_values['merges']=(isset($_GET['merges'])&&\autosystem\ValidateAs::isnumeric($_GET['merges']))?$_GET['merges']:'';
                }
                if(isset($_GET['equals'])){
                    $this->_values['equals']='';
                    $this->_values['of']='';
                }
                if(isset($_GET['page'])){
                   $this->_values['page']=(isset($_GET['page'])&&\autosystem\ValidateAs::isnumeric($_GET['page']))?$_GET['page']:"";
                }else{
                   $this->_values['page']=0; 
                }
            }
            $this->_values['location_directions']=$this->_view->getOptions($this->_model->directions,$this->_values['direction']);
        }
        private function saveChecked($asoarr){
           foreach($asoarr as $key=>$value){
                if(\autosystem\ValidateAs::isCheckedOrNot($value)){
                    $this->_model->setChecked($key,$value);
                }else{
                    return false;
                }
            }
            return true;       
        }
        private function canSave(){
            $valid = true;    

            return $valid;
        }
    }
?>