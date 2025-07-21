<?php
namespace support;
class SupportCenter extends \autosystem\Controller{
    private $categorys =[];
    public function init() {
       $this->getModel();
       $this->vars();
       if($this->selectS()){      return ;     }
       //$this->_values['message']['content']='';
       //$this->_values['message']['type']=null;
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
       if(key_exists('delete',$_GET)&&$id = parent::getAccound_id()){
            if($this->_model->delete($_GET['delete'] ,$id)){
                $this->_values['title']=$this->_view->get_AtranslationB('deleted');
            }
       }
       if(key_exists('sendreaction',$_POST)&&$id = parent::getAccound_id()){
            $issaved=$this->_model->save_ticketReaction($this->_values['reaction_detail'],$id,$_GET['view_ticket'] );
            if($issaved){
            	$this->validationmsg('reaction.placed',$issaved);
            }else{
				$this->validationmsg('reaction.notsaved',$issaved); 
			}
       }
       $this->prepare($this->_loc,$this->_action,$this->_values,$this->_values['search']);
    }
    private function prepare($loc,$action,$values,$find =null){
       $GETEVERYTHINGELSE = true;
       print $this->_view->getheader($loc,$action,[]);
       //print_r($this->_values);
       print $this->_view->get_template($this->_loc,$this->_action,$this->_values);
       if(key_exists('message' ,$this->_values)){
            print $this->_view->get_template('global','returnmsg',$this->_values['message']);
            print '<!-- message -->'.$this->_values['message']['type'].$this->_values['message']['value'];
       }
       if(isset($this->_values['filter'])){
           $array['id'] = 'category';
           $array['title'] = $this->_values['ticket.category.title'];
           $array['layoutpath']= $this->_view->getimgPath();
               print $this->_view->get_template('global','section_head',$array);
               $array['title'] = '';//bubble.svg
               $array['bullet'] = $this->_view->getimgPath().'bubble.svg';
                   print $this->_view->get_template($this->_loc,'CategoryItem_head',$array);
                    foreach($this->categorys as $key => $value){
                        $array['id']=$key;
                        $array['label']=$this->_values[$value];
                        print $this->_view->get_template($this->_loc,'CategoryItem',$array);
                    }
                    print $this->_view->get_template($this->_loc,'CategoryItem_foot');
               print $this->_view->get_template('global','section_foot');
       }
       if(isset($this->_values['view_ticket'])){
            if($a = $this->_model->getTicketAsArray($this->_values['view_ticket'],$this->getAccound_id())){
                $c = $a+ $this->_values;
                $categorys = null;
                foreach($this->categorys as $key => $value){
                    $categorys[$key]=$this->_values[$value];
                    if($c['categorylabel'] == $value){
                        $c['categorylabel'] = $key;
                    }
                }
                $c['action_title']= $a['title'];
                $c['chattxt']=($c['accound_id']!=$this->getAccound_id())?'chattxtR':'chattxtL';
                print '<h1>'.$c['accound_id'] .' - '.$this->getAccound_id().'</h1>';
                print $this->_view->get_template('global','chat_head');
                $c['ticket_prioritys']=$this->_view->getOptions($this->_model->priority,$c['PRIORITY']);
                $c['ticket_states']=$this->_view->getOptions($this->_model->states,$c['STATUS']);       
        
                print $this->_view->get_template($this->_loc,'TicketSettings',$c);
                print $this->_view->get_template($this->_loc,'TicketChatItem',$c);
                
                //print '---';
                $this->_model->GET_reactionsSummary($this->_loc,'support_reaction_item',$this->_view,$this->getAccound_id(),$this->_values['view_ticket']);
                //getTicketAsArray(,$this->getAccound_id()));
                // if (open print...)){
                    $c['chattxt']='chattxtL';
                    print $this->_view->getform_head($this->_loc,$this->_action);
                    print $this->_view->get_template($this->_loc,'TicketChatAddReaction',$c);
                    //print_r($_POST);
                    print $this->_view->getform_end('','sendreaction');
                    print $this->_view->get_template('global','chat_foot');
                    
                    print $this->_view->get_template('global','toolselect_end',$c);
                //}
            }else{
                $this->_values['message']['content']='ERROR: '.$this->_values['TicketNOTFOUND'];
                $this->_values['message']['type']= 'ERROR';
                print $this->_view->get_template('global','returnmsg',$this->_values['message']);
            }
            $GETEVERYTHINGELSE = false;
       }
       if($GETEVERYTHINGELSE){
           if(isset($this->_values['category'])&& $this->_values['category']>0){
               if(key_exists($this->_values['category'],$this->categorys)){
                    $array['title'] .= ' '.$this->_values[$this->categorys[$this->_values['category']]];
               }else{
                    $array['title'] .= ' find category';
               }
               print $this->_view->get_template($this->_loc,'item_head');
               $this->_model->GET_ActiveTicketsSummary($this->_loc,'TicketSummaryitem',$this->_view,$this->_values['category'],'',true);
           }else{
               $this->_model->GET_ActiveTicketsSummary($this->_loc,'TicketSummaryitem',$this->_view,$this->_values['category'],'',true);
           }    
           print $this->_view->get_template($this->_loc,'item_foot');
       }
       print $this->_view->get_template('global','tooltip_start');
       print $this->_view->getfooter($action);
    }
    private function vars(){
        $this->_values['category']=(isset($_GET['category'])&& is_numeric($_GET['category']))?$_GET['category']:0;
        $categorys = $this->_model->GET_Categorys($this->_values['category']);
        if(count($categorys)==0){
            $categorys = $this->_model->GET_Categorys(0);
            if(count($categorys)==0){
                $categorys = [ 'eff'=> 'categorys.not.exist'];
            }
        }
        array_push($this->_values,'ticket.category.title');
        array_push($this->_values,'ticket.info.title');
        $this->addTranslationValues($this->_values);
        //print '--';
        $this->addTranslationValues($categorys);
        $this->addTranslationKeys($this->_model->states);
        $this->addTranslationKeys($this->_model->priority);
		$this->_values = $this->getTranslations();
		//print '--';
        $this->_view->setENUM_TRANSLATIONS($this->_values);
        $this->_values['reaction_detail']='';

            $this->_values['info.title']=$this->_values['detail'];
            if(isset($_GET['category'])){
                $this->_values['category']=(isset($_GET['category'])&&\autosystem\ValidateAs::isnumeric($_GET['category']))?$_GET['category']:0;
            }
            if(isset($_GET['view_ticket'])){
                $this->_values['view_ticket']=(isset($_GET['view_ticket'])&&\autosystem\ValidateAs::isnumeric($_GET['view_ticket']))?$_GET['view_ticket']:0;
                //$this->_values['categorys']=$this->_view->getOptions($this->categorys,$this->_values['category']);
            }
            if(isset($_POST['sendreaction'])){
                $this->_values['sendreaction']=(isset($_POST['sendreaction']))?$_POST['sendreaction']:'t';
                $this->_values['reaction_detail']=(isset($_POST['reaction_detail']))?$_POST['reaction_detail']:'';
            }
        
        $this->_values['ticket_priority']=(isset($_POST['ticket_priority'])&& \autosystem\ValidateAs::isTEXT($_POST['ticket_priority']))?$_POST['ticket_priority']:'';
        $this->_values['ticket_prioritys']=$this->_view->getOptions($this->_model->priority,$this->_values['ticket_priority']);
        //var_dump($_POST['ticket_status']);
        $this->_values['ticket_status']=(isset($_POST['ticket_status'])&&\autosystem\ValidateAs::isTEXT($_POST['ticket_status']))?$_POST['ticket_status']:'';
        $this->_values['ticket_states']=$this->_view->getOptions($this->_model->states,$this->_values['ticket_status']);       
        $this->_values['search']='';
    }
    private function selectS(){
       if(!isset($_GET['select'])){return ;}
       $return = false;
       if(isset($_POST['priority'])){
            print '[';
            foreach($_POST['priority'] as $priorityid => $priority){
                if(!empty($priorityid)&& !empty($priority)){
                    if($this->_model->SetItemPriority($priorityid,$priority)){      $return = true; print '["'.$priorityid.'","'.$priority.'"]';}
                }
            }
            print ']';
       }
       if(isset($_POST['status'])){
            print '[';
            foreach($_POST['status'] as $supportid => $status){
                if(!empty($supportid)&& !empty($status)){
                    if($this->_model->SetItemStatus($supportid,$status)){      $return = true; print '["'.$supportid.'","'.$status.'"]';}
                }
            }
            print ']';
       }
       return $return;   
    }
}   
?>