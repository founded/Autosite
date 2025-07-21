<?php
namespace translator;
class TranslateOverview extends \autosystem\Controller{
    private $values = array('from','to');
    public function init() {
       $this->vars();
       if(isset($_POST['save'])){
            $this->getModel();
            $issaved=false;
            if($this->validate($this->_values['localesrc'],$this->_values['key'],$this->_values['from'])){
                $issaved=$this->_model->save($this->_values['localesrc'],$this->_values['key'],$this->_values['from']);
            }
            if($this->validate($this->_values['locale'],$this->_values['key'],$this->_values['to'])){
                $issaved=$this->_model->save($this->_values['locale'],$this->_values['key'],$this->_values['to']);
            }
            if($issaved){
                 $this->_values['title']=$this->_view->get_AtranslationB('saved');
            }
            $this->prepare($this->_loc,$this->_action,$this->_values);
            return;
       }
       $this->getModel();
       $this->prepare($this->_loc,$this->_action,$this->_values);
    }
    private function validate($locale,$key,$val){
        if($this->_model->exist($locale,$key)){
            return false;
        }
        if(trim($val) ==''){
            return false;
        }
        return true;
    }
    private function islocale($input){
        return isset($input);
    }
    private function vars(){
        $this->_values = $this->_view->get_translations($this->values);
        $this->_values['localesrc']=strtolower($this->_defaultLocale);
        if(isset($_POST['localesrc'])&& $this->islocale($_POST['localesrc'])){
            $this->_values['localesrc']=strtolower($_POST['localesrc']);
        }
        if(isset($_GET['localesrc'])&& $this->islocale($_GET['localesrc'])){
            $this->_values['localesrc']=strtolower($_GET['localesrc']);
        }
        $this->_values['localedest']=$this->locale;       
        if(isset($_POST['locale'])){
            $this->_values['locale']=strtolower($_POST['locale']);
        }elseif(isset($_GET['locale'])){
            $this->_values['locale']=strtolower($_GET['locale']);
        }else{
            $this->_values['locale'] = 'en';
        }
        
        if(isset($_GET['page'])){
           $this->_values['page']=(isset($_GET['page'])&&\autosystem\ValidateAs::isnumeric($_GET['page']))?$_GET['page']:0;
        }else{
           $this->_values['page']=0; 
        }
        if(isset($_POST['search'])){
            $this->_values['search'] = $_POST['search'];
        }else{
            $this->_values['key']=filter_input(INPUT_GET, 'key');
            $this->_values['title']=$this->_view->get_AtranslationB('translatekey').$this->_values['key'];
            $this->_values['from']= (isset($_POST['from']))?filter_input(INPUT_POST, 'from'): $this->_view->get_Atranslation($this->_values['key'],$this->_values['localesrc']);
            $this->_values['to']=(isset($_POST['to']))?filter_input(INPUT_POST, 'to'):$this->_view->get_Atranslation($this->_values['key'],$this->_values['locale']);
        }
    }
    private function prepare($loc,$action,$values){
       print $this->_view->getheader($loc,$action);
       print $this->_view->get_template($this->_loc,'TranslateOverview');
        $GETSOMETHING = true;  
        if(isset($_GET['onlyempty'])){
            print $this->_view->get_template($this->_loc,'item_head',$this->_values);
            $this->_model->GET_Summary($this->_loc,'item',$this->_view,'OUT',$this->_values['localesrc'],$this->_values['localedest'],true,$this->_values['page']);
            print $this->_view->get_template($this->_loc,'item_foot');
            $this->_view->getpaginator($this->_model->getPaginatorData(),'&onlyempty');
       }
       if(isset($_GET['summary'])){
            print $this->_view->get_template($this->_loc,'item_head',$this->_values);
            $this->_model->GET_Summary($this->_loc,'item',$this->_view,'OUT',$this->_values['localesrc'],$this->_values['localedest'],false,$this->_values['page']);
            print $this->_view->get_template($this->_loc,'item_foot');
            $this->_view->getpaginator($this->_model->getPaginatorData(),'&summary');
       }
       if(isset($_GET['NeedTranslateASAP'])){
            print $this->_view->get_template($this->_loc,'item_head',$this->_values);
            $locale = null;
            $this->_model->GET_Summary($this->_loc,'',$this->_view,$locale,$this->_values['localesrc'],$this->_values['localedest'],false,$this->_values['page']);
            print $this->_view->get_template($this->_loc,'item_foot');
            $this->_view->getpaginator($this->_model->getPaginatorData(),'&summary');
       }
       if(isset($_GET['notfound'])){
            $locale = $this->locale;
            print $this->_view->get_template($this->_loc,'item_head',$this->_values);
            $this->_model->GET_NotfoundSummary($this->_loc,'moderate_keynotfound',$this->_view,$locale,$this->_values['page']);
            print $this->_view->get_template($this->_loc,'item_foot');
       }
       if(isset($_GET['allnotfound'])){
            $locale = $this->locale;
            print $this->_view->get_template($this->_loc,'item_head',$this->_values);
            $this->_model->GET_ALLNotfoundSummary($this->_loc,'moderate_keynotfound',$this->_view,$locale,$this->_values['page']);
            print $this->_view->get_template($this->_loc,'item_foot');
       }
       if(isset($_GET['cleannotfound'])){
            $locale = $this->locale;
            print $this->_view->get_template($this->_loc,'item_head',$this->_values);
            $this->_model->GET_NotfoundClean();
            print $this->_view->get_template($this->_loc,'item_foot');
       }
       if(isset($this->_values['search'])){
            print $this->_view->get_template($this->_loc,'search_head',$this->_values);
            $this->_model->findkey($this->_loc,'search_item',$this->_view,$this->_values['search']);
            print $this->_view->get_template($this->_loc,'item_foot');
            $GETSOMETHING=false;
       }
       if($GETSOMETHING){
            
       }
       print $this->_view->getfooter($action);
    }
}
?>