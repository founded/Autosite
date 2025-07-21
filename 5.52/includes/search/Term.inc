<?php
	namespace search;
    class Term extends \autosystem\Controller{
        private $values=array( 'not availeble','detail','import','overview','detail','PAGENOTFOUND');
        private $categorys;
        public function init() {
           $this->getModel();
           $this->vars();
           if(key_exists('Acomplete',$_GET)){$this->Acomplete();                return;}           
           if(isset($_POST['attend'])){
                $url = (\autosystem\ValidateAs::isURL($_SERVER['REQUEST_URI']))?$_SERVER['REQUEST_URI']:'invalid input';
                $location = "/$this->_loc/$this->_action/";//setReportAuction
                $issaved=$this->_model->setReportAuction($this->_values['report'],$this->_values['auction_tag'],$this->_values['detail'],$url,$location,$this->getAccound_id());
                unset($this->_values['report']);
           }
           $this->prepare($this->_loc,$this->_action,$this->_values);
        }
        private function prepare($loc,$action,$values){
           $GETEVERYTHINGELSE = true;
           print $this->_view->getheader($loc,$action);
           print $this->_view->get_template($this->_loc,$this->_action,$this->_values);
           $array['id'] = 'category';
           $array['title'] = '';
           $array['layoutpath']= $this->_view->getimgPath();
          if(isset($_POST['find'])){
                $array['id'] = 'info';
                $array['title'] = $this->_values['auction.info.find'];
                print $this->_view->get_template('global','section_head',$array);
                print $this->_view->getform_head($this->_loc,$this->_action);
                print $this->_view->get_template($this->_loc,'find',$this->_values);
                print $this->_view->getform_end('','find');
                if($this->_values['section']!=''){
                     $this->_model->FIND($this->_loc,'found_item',$this->_view,$this->_values['q'],$this->_model->section[$_POST['section']],$this->getAccound_id());
                }
                print $this->_view->get_template('global','section_foot');
                $GETEVERYTHINGELSE = false;
           }
           if(isset($this->_values['result'])){
                if(!$this->_model->GET_ITEM($this->_loc,'detail',$this->_view,$this->_values['view'])){
                    $array['id'] = 'info';
                    $array['title'] = 'ERROR: '.$this->_values['PAGENOTFOUND'];
                    print $this->_view->get_template('global','section_head',$array);
                    print $this->_view->get_template($this->_loc,'HTTP404');
                    print $this->_view->get_template('global','section_foot');
                }
                $GETEVERYTHINGELSE = false;
           }
           if(isset($this->_values['attend'])){
                $array['id'] = 'info';
                $array['title'] = $this->_values['auction.info.ReportPage'];
                print $this->_view->get_template('global','section_head',$array);
                print $this->_view->getform_head($this->_loc,$action);
                print $this->_view->get_template($this->_loc,'report',$this->_values);
                print $this->_view->getform_end('','report');
                print $this->_view->get_template('global','section_foot');
                $GETEVERYTHINGELSE = false;
           }
           if($GETEVERYTHINGELSE){ 
           }
           $array['id'] = 'favorites';
           $array['title'] = '';//$this->_values['auction.favorites.title'];
           print $this->_view->get_template('global','section_head',$array);
           $array = $this->getLocationHistory(true);
           if(is_array($array)&&key_exists('/'.$this->_loc.'/'.$this->_action.'/',$array)){
               foreach($array["/$this->_loc/$this->_action/"] as $key => $value){
                    $this->_model->GET_AOverView($this->_loc,'favs_item',$this->_view,null,null,null,$value);
               }
           }
           print $this->_view->get_template('global','section_foot');
           print $this->_view->getfooter($action);
        }
        private function vars(){
            $this->addTranslationValues($this->values);
            $this->_values = $this->getTranslations();
            $this->_view->setENUM_TRANSLATIONS($this->_values);
            $this->_values['page']=0; 
            if(isset($_POST['find'])){
                $this->_values['search']=(isset($_POST['search']))?$_POST['search']:'';
                $this->_values['q']=(isset($_POST['q']))?$_POST['q']:$this->_values['search'];
                $this->_values['section']=(isset($_POST['section'])&&key_exists($_POST['section'],$this->_model->section))?$_POST['section']:'';
                
                $this->_values['sections']=$this->_view->getOptions($this->_model->section,$this->_values['section']);       
            }else{
                $this->_values['info.title']=$this->_values['detail'];
                if(isset($_GET['view'])){
                    $this->_values['view']=(isset($_GET['view'])&&\autosystem\ValidateAs::isnumeric($_GET['view']))?$_GET['view']:'';
                }
                if(isset($_GET['page'])){
                   $this->_values['page']=(isset($_GET['page'])&&\autosystem\ValidateAs::isnumeric($_GET['page']))?$_GET['page']:0;
                }
                if(isset($_GET['report'])||isset($_POST['report'])){
                    $this->_values['report']=(isset($_GET['report'])&&\autosystem\ValidateAs::isnumeric($_GET['report']))?$_GET['report']:'';
                    $this->_values['auction_tag']=(isset($_POST['auction_tag'])&& key_exists($_POST['auction_tag'],$this->_model->tag))?$_POST['auction_tag']:'';
                    $this->_values['auction_tags']=$this->_view->getOptions($this->_model->tag,$this->_values['auction_tag']);
                    $this->_values['detail']=(isset($_POST['detail'])&&\autosystem\ValidateAs::isTEXT($_POST['detail']))?$_POST['detail']:'';
                }
            }
            $this->_values['search']='';
        }
        private function Acomplete(){
           /*$minsize=10;
           $return = '{"Athermlist":{}}';
           if(isset($_POST['search'])&&\autosystem\ValidateAs::isTerm($_POST['search'])){
               $data = $this->_model->Acomplete($_POST['search']);
               //$return = json_encode($data);
           }
           print $return;   
           return true;*/            
        }

    }
?>