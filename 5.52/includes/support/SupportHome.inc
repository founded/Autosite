<?php
	namespace support;
    class SupportHome extends \autosystem\Controller{
        private $values=array( 'not availeble','detail','import','overview_ticket','detail','TicketNOTFOUND','TicketNOTYOURS','ticket.favorites.title');
        private $categorys;
        public function init() {
           $this->_values['message']['content']='';
           $this->_values['message']['type']=null;
           $this->getModel();
           $this->vars();
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
           print $this->_view->getheader($loc,$action);
           print $this->_view->get_template($this->_loc,$this->_action,$this->_values);
           if(isset($find)&& trim($find)!=''){
               print $this->_view->get_template($this->_loc,'findresult',$this->_values);
           }
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
           if(isset($this->_values['MyTickets'])){
               print $this->_view->get_template($this->_loc,'item_head');
               $this->_model->GET_MyTicketSummary($this->_loc,'TicketSummary_user_item',$this->_view,$this->getAccound_id());
               print $this->_view->get_template($this->_loc,'item_foot');
               $GETEVERYTHINGELSE = false;
           }
           if(isset($this->_values['addTicket'])){
               print $this->_view->getform_head($this->_loc,$this->_action);
               $this->_model->GET_MyTicketSummary($this->_loc,'SupportHome_addTicket',$this->_view,$this->getAccound_id());
                print $this->_view->getform_end('','send.request');
               $GETEVERYTHINGELSE = false;
           }
           if(isset($this->_values['view_ticket']) ){
                if($this->_model->isOwnerOfticket($this->getAccound_id(),$this->_values['view_ticket'])){
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
                        print $this->_view->get_template('global','chat_head');
                        print $this->_view->get_template($this->_loc,'TicketChatItem',$c);
                        print '---';
                        $this->_model->GET_reactionsSummary($this->_loc,'support_reaction_item',$this->_view,$this->getAccound_id(),$this->_values['view_ticket']);
                        $c['chattxt']='chattxtL';
                        print $this->_view->getform_head($this->_loc,$this->_action);
                        print $this->_view->get_template($this->_loc,'TicketChatAddReaction',$c);
                        print $this->_view->getform_end('','sendreaction');
                        print $this->_view->get_template('global','chat_foot'); 
                    }else{
                        $array['id'] = 'info';
                        $array['title'] = 'ERROR: '.$this->_values['TicketNOTFOUND'];
                        print $this->_view->get_template('global','section_head',$array);
                        print $this->_view->get_template($this->_loc,'TicketNotFound');
                        print $this->_view->get_template('global','section_foot');
                    }
                    print $this->_view->get_template('global','chat_foot');
                }else{
                    $this->_values['message']['content']=$this->_view->get_AtranslationB('TicketNOTYOURS');
                    $this->_values['message']['type']= 'ERROR';
                    print $this->_view->get_template('global','returnmsg',$this->_values['message']);
                }
                $GETEVERYTHINGELSE = false;
           }
           if($GETEVERYTHINGELSE){
                   $array['id'] = 'info';
                   $array['title'] = $this->_values['ticket.info.title'].$this->_values['overview_ticket'];
                   if(isset($this->_values['category'])&& $this->_values['category']>0){
                       $array['id'] = 'info';
                       if(key_exists($this->_values['category'],$this->categorys)){
                            $array['title'] .= ' '.$this->_values[$this->categorys[$this->_values['category']]];
                       }else{
                            $array['title'] .= ' find category';
                       }
                       print $this->_view->get_template('global','section_head',$array);
                       print $this->_view->get_template($this->_loc,'item_head');
                                                //$location,$templatename,$view,$filter='',$accound_id=''
                       $this->_model->GET_AOverView($this->_loc,'item',$this->_view,$this->_values['category'],'',true);
                   }else{
                       print $this->_view->get_template('global','section_head',$array); 
                       $this->_model->GET_support_ticketssSummary($this->_loc,'item',$this->_view,$this->getAccound_id(),0);//,$this->_values['category']
                   }
                   print $this->_view->get_template($this->_loc,'item_foot');
                   print $this->_view->get_template('global','section_foot');
           }
           $array['id'] = 'favorites';
           $array['title'] = '';
           print $this->_view->get_template('global','section_head',$array);
           $array = $this->getLocationHistory(true);
           if(is_array($array)&&key_exists('/'.$this->_loc.'/'.$this->_action.'/',$array)){
               foreach($array['/'.$this->_loc.'/'.$this->_action.'/'] as $key => $value){
                    $this->_model->GET_AOverView($this->_loc,'favs_item',$this->_view,null,null,null,$value);
               }
           }
           print $this->_view->get_template('global','section_foot');
           print $this->_view->getfooter($action);
        }
        private function vars(){
            $category=(isset($_GET['category'])&& is_numeric($_GET['category']))?$_GET['category']:0;
            $this->categorys = $this->_model->GET_Categorys($category);
            //print_r($this->categorys);
            if(count($this->categorys)==0){
                $this->categorys = $this->_model->GET_Categorys(0);
                if(!is_array($this->categorys)){
                    $this->categorys = ['0'=> 'categorys.not.exist'];
                }
            }
            array_push($this->values,'ticket.category.title');
            array_push($this->values,'ticket.info.title');
            $this->addTranslationValues($this->values);
            $this->addTranslationValues($this->categorys);
            $this->_values = $this->getTranslations();
            //print_r($this->_values);
            $this->_view->setENUM_TRANSLATIONS($this->_values);
            $this->_values['category']=$category;
            if(isset($_POST['find'])){
        
            }else{
                $this->_values['info.title']=$this->_values['detail'];
                if(isset($_GET['category'])){
                    $this->_values['category']=(isset($_GET['category'])&&\autosystem\ValidateAs::isnumeric($_GET['category']))?$_GET['category']:'';
                }
                if(isset($_GET['view_ticket'])){
                    $this->_values['view_ticket']=(isset($_GET['view_ticket'])&&\autosystem\ValidateAs::isnumeric($_GET['view_ticket']))?$_GET['view_ticket']:'';
                    $this->_values['reaction_detail']=(isset($_POST['reaction_detail']))?$_POST['reaction_detail'].'':'';
                }
                if(isset($_GET['addTicket'])){
                    $this->_values['addTicket']='';
                }
                if(isset($_GET['MyTickets'])){
                    $this->_values['MyTickets']='';
                    $this->_values['reaction_detail']=(isset($_POST['reaction_detail']))?$_POST['reaction_detail'].'':'';
                }
                if(isset($_GET['report'])||isset($_POST['report'])){
                    $this->_values['report']=(isset($_GET['report'])&&\autosystem\ValidateAs::isnumeric($_GET['report']))?$_GET['report']:'';
                    $this->_values['tickettag']=(isset($_POST['ticket_tag'])&& key_exists($_POST['ticket_tag'],$this->_model->tag))?$_POST['ticket_tag']:'';
                    $this->_values['ticket_tags']=$this->_view->getOptions($this->_model->tag,$this->_values['ticket_tag']);
                }
                if(isset($_POST['sendreaction'])){
                    $this->_values['sendreaction']=(isset($_POST['sendreaction']))?$_POST['sendreaction']:'t';
                    $this->_values['reaction_detail']=(isset($_POST['reaction_detail']))?$_POST['reaction_detail'].'':'';
                }
            }
            $this->_values['search']='';
        }
    }
?>