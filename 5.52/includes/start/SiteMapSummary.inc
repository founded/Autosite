<?php
 /**
 * @author Lieven Roegiers
 * @copyright 2010 
 * @CMS autosite
 */
namespace start;
class SiteMapSummary extends \autosystem\Controller{
    private $values = array('search','find');
    public function init(){
           $this->getModel();
           $this->vars();
           $this->prepare($this->_loc,$this->_action,$this->values);
    }
    private function vars(){
        $this->addTranslationKeys($this->_model->modeltypes);
        $this->_values = $this->getTranslations();
    }
    private function prepare($loc,$action,$values){
     print $this->_view->getheader($loc,$action);
     //print $this->_view->get_template($loc,$action,$this->_values);
            $GETSOMETHING = true;
            if(isset($_GET['x'])){
                print $this->_view->getform_head($this->_loc,$this->_action);
                print $this->_view->get_template($this->_loc,'add',$this->_values);
                print $this->_view->getform_end('','add');
                $GETSOMETHING = false;
           }
           if($GETSOMETHING){
                if($this->_user->hasRulle('KING')){
                    print $this->_view->get_template($this->_loc,'kingmenu',$this->_values);
                        $array['id'] = 'KING'.'_links';
                        $array['classes'] = 'linksblock';
                        $array['title'] =  $this->_values['KING'];
                        print $this->_view->get_template($this->_loc,'section_link',$array);
                        //$location,$templatename,$view,$type
                        $items = $this->_model->GET_KingSumArray();
                        $ktranslations=$this->_view->get_translations(array_keys($items));
                        //print_r($ktranslations);
                        foreach($items as $key => $item){
                         	$item['key']= $ktranslations[$key];
    						print $this->_view->get_template($this->_loc,'sitemapKINGlink_item',$item);
    					}
                        print $this->_view->get_template($this->_loc,'section_foot',[]);
                        
                        $GETEVERYTHINGELSE = false;
                }else{
                    foreach($this->_user->getAllRules()as $rulle){
                        $array['id'] = $rulle.'_links';
                        $array['classes'] = 'linksblock';
                        $array['title'] =  $this->_values[$rulle];
                        print $this->_view->get_template($this->_loc,'section_link',$array);
                        //$location,$templatename,$view,$type
                        $items = $this->_model->GET_SumArray($rulle);
                        $ktranslations=$this->_view->get_translations(array_keys($items));
                        //print_r($ktranslations);
                        foreach($items as $key => $item){
                         	$item['key']= $ktranslations[$key];
    						print $this->_view->get_template($this->_loc,'sitemaplink_item',$item);
    					}
                        print $this->_view->get_template($this->_loc,'section_foot',[]);
                        
                        $GETEVERYTHINGELSE = false;
                     }
                }
                
                print $this->_view->get_template('global','toolcheck_end',$this->_values);
           }     
        print $this->_view->getfooter($action);
    }
    private function viewItem($name,$path,$location){
    }
    private function canSave(){
        return true;
    }
}
?>