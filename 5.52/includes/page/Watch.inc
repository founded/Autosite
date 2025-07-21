<?php
namespace page;
class Watch extends \autosystem\Controller{
    private $values = array('search','find');
    private $fullcash = false;
    public function init(){        
        if($this->fullcash ){
            if($this->existCash()){
                print $this->takeCash();
            }
        }
        $this->vars();
        $this->getModel();
        if(isset($_POST['report'])){
            $issaved=$this->_model->setReportPage($this->_values['report'],$this->_values['auction_tag'],'');
            unset($this->_values['report']);
        }
        $this->prepare($this->_loc,$this->_action);
    }
    private function prepare($loc,$action){
        $this->_values['title']='d';
        if(empty($action)){            $action = 'startuppage';        }
        $GETSOMETHING = true;
        if(!isset($this->_values['ThePage'])){
            $this->_values['servicenr']= $this->_view->get_AtranslationB('service.notavaileble');
            $this->_values['title']= '';
            print $this->_view->getheader($this->_loc,$action,$this->_values);
            print '<!-- page/watch -->';
            if(!$this->_model->GETPageByTitle($this->_loc,'ThePage',$this->_view,'Home')){//create a page Home on the database
                
                print $this->_view->get_template($this->_loc,'home');
            }
            if(file_exists('.promo')){
                   print '<!-- page/watch/ @promo -->';
                   
                   print $this->_view->get_template('catalog','promos',[]);
                   print $this->_view->get_template('catalog','promos_head',[]);
                   //print $this->_view->get_template('catalog','testpromobanner',[]);
                   $products = new \catalog\Product_model();
                   $this->_view->setTemplateSlaches(true);
                   $products->GET_PromoSummary('catalog','promo_item',$this->_view,$this->getAccound_id(),'paint_');
                   $this->_view->setTemplateSlaches(false);
                   $this->_model->GET_promoSummary($this->getAccound_id());
                   print $this->_view->get_template('catalog','promos_foot',[]);
            }
            if(file_exists('.chat')){
                   print '<!-- page/watch/ @chat -->';
                   print $this->_view->get_template('support','chat_head',[]);
                   $chat =  new \support\Chat_model();
                   $chat->openChat('support','chat_item',$this->_view,$this->getAccound_id(),$this->getSession_id());
                   //$this->_model->GET_promoSummary('support','Chat',$this->_view,$this->getAccound_id());
                   print $this->_view->get_template('support','chat_foot',[]);
            }
            if(file_exists('.callendar')){
                   print '<!-- page/watch/ @calendar-->';
                   print $this->_view->get_template('plan','callendar_head',[]);
                   //$this->_model->GET_promoSummary('catalog','Painting_promo_item',$this->_view,$this->getAccound_id());
                   print $this->_view->get_template('plan','callendar_foot',[]);
            }
            print $this->_view->getfooter($action);
            return;
        }   
        $exist = $this->_model->existId($this->_values['ThePage']);
        if($exist){
            $a = $this->_model->GETArray($this->_values['ThePage']);
            $this->_values = $this->_values +$a;
            $this->_values['title']= $this->_view->get_AtranslationB($a['title.key']);
        }else{
            $this->_values['title']= '';
        }
        print $this->_view->getheader($loc,$action,$this->_values);
        if($exist){
            $visitor =  parent::getVisitor();
            $hasaccess = $visitor->hasRulle('MODERATOR');
            if($hasaccess){
                print $this->_view->get_template($this->_loc,'controls',$this->_values);
            }
            if($this->_model->isPageVisibled($this->_values['ThePage'])||$hasaccess){
                print $this->_view->get_template($this->_loc,'ThePage',$this->_values);
            }else{ print $this->_view->get_template($this->_loc,'HTTP403',$this->_values);  }
        }else{     print $this->_view->get_template($this->_loc,'HTTP404',$this->_values);  }
        print $this->_view->getfooter($action);
    }
    private function vars(){
        $this->_values = $this->getTranslations();     
        if(isset($_POST['find'])){
        }else{
            if(isset($_GET['ThePage'])){
                $this->_values['ThePage']=(isset($_GET['ThePage'])&&\autosystem\ValidateAs::isnumeric($_GET['ThePage']))?$_GET['ThePage']:0;
            }
            if(key_exists('ThePage',$_GET)&&$_GET['ThePage']<0){
                //$this->redirect('page/HTTP404/');
            }
            if(isset($_GET['report'])||isset($_POST['report'])){
                $this->_values['report']=(isset($_GET['report'])&&is_numeric($_GET['report']))?$_GET['report']:'';
            }
        }
   }
   private function existCash(){        
   }
   private function takeCash(){        
   }
}
?>