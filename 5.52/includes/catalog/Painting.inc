<?php
/**
* @author Lieven Roegiers
* @copyright 
* @CMS autosite 5.3
*/
namespace catalog;
class Painting extends \autosystem\Controller{
   private $values = [];
   private $categorys;
   public function init(){
      $this->getModel();
      $this->vars();
      $this->prepare();
   }
   public function prepare(){
       print $this->_view->getheader($this->_loc,$this->_action);
       if(key_exists('painting',$_GET)){       
            print('<center>');
            $this->_model->GET_paintingE(
            $this->_loc,'Painting_info',
            $this->_view,$this->_values['painting'],$this->getAccound_id());
            print('</center>');
       }else{
           $x['id'] = 'groups';
           $x['title'] = '';
           print $this->_view->get_template('global','tooltip_start',[]);
           print $this->_view->get_template('global','section_head',$x);
           print '<ul>';
           $this->_model->GET_groups($this->_loc,'Painting_group_item',$this->_view);
           print '</ul>';
    	   print $this->_view->get_template('global','section_foot',$x);  
           if($this->_values['group']>0){
                //$this->_model->GET_GroupInfo($this->_loc,'Painting_group_info',$this->_view,$this->getAccound_id(),$this->_values['group']);
           }
           if(key_exists('parameters',$this->_values)){
              //print print_r($this->_values['parameters'],true);
        	   $x['id'] = 'paintsummary';
               $x['title'] = '';
        	   print $this->_view->get_template('global','section_head',$x);
               print $this->_view->get_template('global','nofilter',$x);
               //$location,$templatename,$view,$account_id,$group,$incparams,$excparams,$account_id=0
                $this->_model->GET_FilterSummary($this->_loc,'Public_Painting_item',$this->_view,$this->_values['group'],$this->_values['parameters'],[],$this->_values['page']);
               //$this->_model->GET_PaintingsSummary($this->_loc,'Public_Painting_item',$this->_view,$this->getAccound_id(),'',$this->_values['page'],$this->_values['group']);
               print $this->_view->get_template('global','section_foot',$x);
           }else{ 
               /*print $this->_view->get_template($this->_loc,'promos_head',[]);
               $this->_model->GET_promoSummary($this->_loc,'Painting_promo_item',$this->_view,$this->getAccound_id());
               print $this->_view->get_template($this->_loc,'promos_foot',[]);
               */
               //print('<hr>');
        	   $x['id'] = 'paintsummary';
               $x['title'] = '';
        	   print $this->_view->get_template('global','section_head',$x);
               $this->_model->GET_PaintingsSummary($this->_loc,'Public_Painting_item',$this->_view,$this->getAccound_id(),'',$this->_values['page'],$this->_values['group']);
               print $this->_view->get_template('global','section_foot',$x);
           }
       }
	   print $this->_view->getfooter($this->_action);
   }
   private function vars(){//
        $this->_values['page']=(isset($_POST['page']))?$_POST['page']:0;
        $this->_values['group']=(isset($_GET['group'])&&is_numeric($_GET['group']))?$_GET['group']:'';
        $this->_values['painting']=(isset($_GET['painting'])&&is_numeric($_GET['painting']))?$_GET['painting']:'';
        if(key_exists('parameter',$_GET)&& is_array($_GET['parameter'])){
            $this->_values['parameters']  =$_GET['parameter']   ;  
        }     
        if(isset($_GET['page'])){             $this->_values['page']             = $_GET['page'];            }
        $this->_validator->addForm('addfilter',['group'],false,['group'],$this->_values);
        $this->_validator->addAlocat('addfilter',['group'=>'isnumeric']);
        if(isset($_GET['filter'])){
             $this->_validator->render('addfilter',$_GET,$this->_values);
        }
   }
}
?>