<?php
	namespace plan;
    /**
    * @author Lieven Roegiers
    * @copyright 
    * @CMS autosite *
    */
    class Week extends \autosystem\Controller{
        private  $values=array('prev','search','find','Callendar.edit','Callendar.add','Callendar.year','week.weeknr', 'item.detail');
        private  $days= array('monday','tuesday','wednesday','thursday','friday','saturday','sunday');
        private  $items=[];
        public function init() {
           $this->getModel(); 
           $this->varsFIVE();
           if(count($_POST)>0){
                if(isset($this->_values['additem'])&& $id = parent::getAccound_id()){
                     $issaved=$this->_model->save_weekitem($this->_values['namekey'],$this->_values['from'],$this->_values['start'],$this->_values['to'],$this->_values['end'],$id,$this->_values['TYPE']
                     );
                     if($issaved){
                        $this->_values['title']=$this->_view->get_AtranslationB('saved');
                        $this->GOESTO($this->_loc,$this->_action,['year='.$this->_values['Y'],'weeknr='.($this->_values['weeknr'])]);
                     }
                }
                if(isset($this->_values['save'])&&isset($this->_values['edit'])&& $id = parent::getAccound_id()){
                    
					 $issaved=$this->_model->update_weekitem(
                        $this->_values['namekey'],
                        $this->_values['from'],$this->_values['start'],
                        $this->_values['to'],$this->_values['end'],
                        $id,$this->_values['TYPE'],$this->_values['id']
                     );   
                     if($issaved){
                        $this->_values['title']=$this->_view->get_AtranslationB('saved');
                        $this->GOESTO($this->_loc,$this->_action,['year='.$this->_values['Y'],'weeknr='.($this->_values['weeknr']+1)]);
                     }else{
                        print'not saved';
                     }
                }
                 
           }
           if(isset($this->_values['delete'])&&isset($this->_values['id'])&& $id = parent::getAccound_id()){
			  print 'test';
	          $isdeleted = $this->_model->delete($this->_values['id'],$id);  
	          $param[]='year='.$this->_values['Y'];
	          $param[]='weeknr='.($this->_values['weeknr']+1);
	          $this->tosupportreturn($isdeleted,'delete',$param);
		   } 
           $this->prepare($this->_loc,$this->_action,$this->_values);
        }
        private function prepare($loc,$action,$values,$find =null){
           print $this->_view->getheader($this->_loc,$action);
           print $this->_view->get_template($this->_loc,$this->_action,$this->_values);
           $GETEVERYTHINGELSE = true;
           if(isset($this->_values['add'])){
                print $this->_view->getform_head($this->_loc,$this->_action);
                print $this->_view->get_template($this->_loc,'week_item_add',$this->_values);
                print $this->_view->getform_end('','add');
                $GETEVERYTHINGELSE = false;
           }
           if(isset($_GET['detail'])){
                $array['id'] = 'glob_detail';
                $array['title'] =  $this->_values['item.detail'];
                print $this->_view->get_template('global','section_head',$array);
                if(!isset($this->_values['id'])|| !$this->_model->GET_Id($this->_loc,'day_item_detail',$this->_view,$this->_values['id'],$this->getAccound_id())){
                    print $this->_view->get_template($this->_loc,'error_notfound',$array);
                }
                print $this->_view->get_template('global','section_foot');
                print $this->_view->getform_head($this->_loc,$this->_action);
                $items = $this->_model->GET_DayItems($this->_loc,'day_item',$this->_view,$this->getAccound_id(),$this->_values['Y'],$this->_values['m'],$this->_values['d']);
                print $this->_view->getform_end('','save');
                $GETEVERYTHINGELSE = false;
           }
                      
           if(isset($_GET['edit'])){
                print $this->_view->getform_head($this->_loc,$this->_action);
                $a = $this->_model->GETArray($this->getAccound_id(),$this->_values['id']);
                print $a['to'];
                //print_r($a);
                $c = $a + $this->_values;
                foreach($this->categorys as $key => $value){
                    $categorys[$key]=$this->_values[$value];
                    if($c['categorylabel'] == $value){
                        $c['categorylabel'] = $key;
                    }
                }
                $c['types']= $this->_view->getOptions($this->_model->modeltypes,$c['TYPE']);
                print $this->_view->get_template($this->_loc,'week_item_add',$c);
                print $this->_view->getform_end('','save');
                $GETEVERYTHINGELSE = false;
           }
           if(isset($_GET['day'])&&!empty($this->_values['d'])){
                print $this->_view->getform_head($this->_loc,$this->_action);
                $array['id'] = 'day_summary';
                $array['title'] =  $this->_values['d'].'-'.$this->_values['m'].'-'.$this->_values['Y'] ;
                $items = $this->_model->GET_DayItems($this->_loc,'day_item',$this->_view,$this->getAccound_id(),$this->_values['Y'],$this->_values['m'],$this->_values['d']);
                $GETEVERYTHINGELSE = false;
           }
           if($GETEVERYTHINGELSE){
               $i = $this->_values['d'];
               print $this->_view->get_template('global','tooltip_start');
               $from = 9;
               $to = 20;
               $items = $this->_model->GET_WItems($this->_values['Y'],$this->_values['weeknr'],$this->getAccound_id());
               $a['id'] = 'week_monthlist';
               $a['title'] =  '' ;
               print $this->_view->get_template($this->_loc,'week_monthlist_head',$a);
               if($this->_values['m']>1){                    
                    $a['id'] = 'content_prev';
                    $a['title'] =  $this->_values['m']-1 .'-'.$this->_values['Y'] ;
                    print $this->_view->get_template($this->_loc,'week_monthitem_head',$a);
                    $cal = new \plan\Month($this->_values['m']-1,$this->_values['Y']);
                    $cal->render_smal($this->_view,$this->_values['m']-1);
                    print $this->_view->get_template('global','section_foot');
                    print '<br>';
               }
               $array['id'] = 'content_cat';
               $array['title'] =  $this->_values['m'].'-'.$this->_values['Y'] ;
               $cal = new \plan\Month($this->_values['m'],$this->_values['Y']);
               print $this->_view->get_template('global','section_head',$array);
               $cal->render_smal($this->_view,$this->_values['m']);
               print $this->_view->get_template('global','section_foot');
               if($this->_values['m']<12){
                    print '<br>';
                    $a['id'] = 'content_next';
                    $a['title'] =  $this->_values['m']+1 .'-'.$this->_values['Y'] ;
                    print $this->_view->get_template('global','section_head',$a);
                    $cal = new \plan\Month($this->_values['m']+1,$this->_values['Y']);
                    $cal->render_smal($this->_view,$this->_values['m']+1);
                    print $this->_view->get_template('global','section_foot');
               } 
               print $this->_view->get_template('global','section_foot');
               $array['id'] = 'content';
               $array['prevweeknr']= ($this->_values['weeknr']-1<=0)? '52' : $this->_values['weeknr']-1;
               $array['nextweeknr']= ($this->_values['weeknr']+1<53)? $this->_values['weeknr']+1: '01';
               $array['Y']=$this->_values['Y'];
               $array['title'] = $this->_values['week.weeknr'] . $this->_values['weeknr'];
               print $this->_view->get_template($this->_loc,'week_section_head',$array);
               $monthlen = $this->getMonthLen($this->_values['m'],$this->_values['Y']);
               foreach($this->days  as $key=>$val){
                    if($monthlen<$i){
                        if(++$values['m']==13){
                           $values['m']='01';
                           $values['Y']=$array['Y']+1;
                        }
                        $i =  1;
                        $values['m']=(strlen($values['m'])==1)?'0'.$values['m']:$values['m'];//='07';
                    }
                    $values['Iweek'] =(strlen($i)==1)?'0'.$i:$i;
                    print '<span class="week_day" >';
                    $values['Callendar_title'] = $val;
                    print $this->_view->get_Templateclone($this->_loc,'week_item_head',$values);
                        if(key_exists($i,$items)){
                            $day_data['status']='active';
                            $day_data['amount']= count($items[$i]);
                        }else{
                            $day_data['status']='empty';
                            $day_data['amount']='0';
                        }
                    print $this->_view->get_Templateclone($this->_loc,'week_item_info',$day_data);
                    for ($y = $from; $y <= $to; $y++){//hours
                        $values['time_item_hour'] =$y;
                        $arr = (key_exists($i,$items))?$items[$i]:[];
                        $has = false;
                        $values['items']=$this->items_render($y,$arr,$key,$has,$values);
                        $values['status']=($has)? 'active' :'empty';
                        print $this->_view->get_Templateclone($this->_loc,'time_item',$values);
                    }
                    $i++;
                    print '</span>';
               }
               print $this->_view->get_template($this->_loc,'week_section_foot');   
           }
           print $this->_view->getfooter($action);
        }
        private function vars(){
            $this->_values = $this->_view->get_translations($this->values);
            $this->days = $this->_view->get_translations($this->days);
            $this->_values['title']='callendar:';
            $this->_values['Y'] = (isset($_GET['year'])&&\autosystem\ValidateAs::isYear($_GET['year']))? $_GET['year']:date('Y');
            $this->_values['m'] =date('m');
            if(isset($_GET['day'])){
                $this->_values['d'] = \autosystem\ValidateAs::isnumeric(($_GET['day']))? $_GET['day']:date('d');
                $this->_values['m'] = (isset($_GET['month'])&&\autosystem\ValidateAs::isMonth($_GET['month']))? $_GET['month']:$this->_values['m'];
                $this->_values['weeknr']=$this->getWeekNR($this->_values['m'],$this->_values['Y'],$this->_values['d']);
            }else{
                if(isset($_GET['weeknr'])){
                    $this->_values['weeknr']=(\autosystem\ValidateAs::isnumeric($_GET['weeknr']))?$_GET['weeknr']:'';
                    $this->_values['m'] = $this->getMonth($this->_values['weeknr'],$this->_values['Y']);
                    $this->_values['d'] = $this->getStartof($this->_values['weeknr'],$this->_values['Y']);
                    if($this->_values['d']<1){
                        $this->_values['d'] = 31 + $this->_values['d'];
                    }
                }else{
                    $this->_values['weeknr']=$this->getWeekNR($this->_values['m'],$this->_values['Y']);
                }
            }
            if(!key_exists('d',$this->_values)){
                $this->_values['d']=1;
            }
            $timestamp=  mktime(0, 0, 0,$this->_values['m'],1,$this->_values['Y']);
            $this->_values['month.'.date('M',$timestamp)]=$this->_view->get_AtranslationB('month.'.date('M',$timestamp));
            $this->_values['M']= $this->_values['month.'.date('M',$timestamp)];
            $this->_values['TYPE']='';
            $this->_values['namekey']='';
            $this->_values['from']='';
            $this->_values['to']='';
            if(isset($_POST['find'])){
                    $this->_values['search']=$_POST['search'];       
            }else{
                				
				if(count($_POST)>0){
                      $args = array(
                          'TYPE'    => array('filter'    => FILTER_REQUIRE_ARRAY, 
                                             'options'   => $this->_model->modeltypes),
                          'namekey'     => FILTER_SANITIZE_STRING,
                          'category_id' => FILTER_VALIDATE_INT
                      );
                    $a = filter_input_array(INPUT_POST, $args);
                    $this->_values = $a+$this->_values;
                    //$this->_values['category']=(key_exists($_POST['auction_category'],$this->categorys))?$_POST['auction_category']:"";
                    $adate = $this->checkDate(filter_input(INPUT_POST, 'from'));
                    $this->_values['from']=($adate!=false)?$adate:'';
                    $adate = filter_input(INPUT_POST, 'start');
                    $this->_values['start']=($adate!=false)?$adate:'';
                    $adate = $this->checkDate(filter_input(INPUT_POST, 'to'));
                    $this->_values['to']=($adate!=false)?$adate:'';
                    $adate = filter_input(INPUT_POST, 'end');
                    $this->_values['end']=($adate!=false)?$adate:'';
                }else{
                    $date = $this->_values['Y'].'-'.$this->_values['m'].'-'.$this->_values['d'];
                    $this->_values['from']=$date;
                    $this->_values['to']=$date;
                    if(isset($_GET['add'])){
                       $this->_values['hour']=(isset($_GET['hour'])&&\autosystem\ValidateAs::isnumeric($_GET['hour']))?int($_GET['hour']):0;
                       $h = ($this->_values['hour']<10)? '0'.$this->_values['hour'] .':00':$this->_values['hour'] .':00';
                       $this->_values['start']=(isset($_GET['start'])&&\autosystem\ValidateAs::isTime($_GET['start']))?$_GET['start']:$h;
                       //TODO CHECK UP TO 8
                       $dhour = $this->_values['hour']+1;//int($this->_values['hour'])+1 ;
			           $h = ($dhour<10)? '0'.$dhour.':00':$dhour .':00';
                       $this->_values['end']=(isset($_GET['end'])&&\autosystem\ValidateAs::isTime($_GET['end']))?$_GET['end']:$h;
                       $this->_values['types']= $this->_view->getOptions($this->_model->modeltypes,$this->_values['TYPE']);
                       $this->_values['add']=(isset($_GET['add'])&&\autosystem\ValidateAs::isnumeric($_GET['add']))?$_GET['page']:'';
                    }else{
                       $this->_values['page']=0; 
                    }
                }
                if(isset($_GET['view'])){
                    $this->_values['view']=(isset($_GET['view'])&&\autosystem\ValidateAs::isnumeric($_GET['view']))?$_GET['view']:'';
                }
                if(isset($_GET['edit'])){
                    $this->_values['edit']= '';
                    $this->_values['id']=(\autosystem\ValidateAs::isnumeric($_GET['id']))?$_GET['id']:null;
                }
                if(isset($_GET['copy'])){
                    $this->_values['copy']=(\autosystem\ValidateAs::isnumeric($_GET['copy']))?$_GET['copy']:null;
                }
                if(isset($_GET['detail'])){
                    $this->_values['id']=(\autosystem\ValidateAs::isnumeric($_GET['id']))?$_GET['id']:null;
                }
                if(isset($_POST['add'])){
                    $this->_values['additem']='';
                }
                if(isset($_POST['save'])){
                    $this->_values['save']='';
                }
            }
        }
        private function varsFIVE(){
            $this->_values = $this->_view->get_translations($this->values);
            $this->days = $this->_view->get_translations($this->days);
            $this->_values['title']='callendar:';
            $this->_values['Y'] = (isset($_GET['year'])&&\autosystem\ValidateAs::isYear($_GET['year']))? $_GET['year']:date('Y');
            $this->_values['m'] =date('m');
            if(isset($_GET['day'])){
                $this->_values['d'] = \autosystem\ValidateAs::isnumeric(($_GET['day']))? $_GET['day']:date('d');
                $this->_values['m'] = (isset($_GET['month'])&&\autosystem\ValidateAs::isMonth($_GET['month']))? $_GET['month']:$this->_values['m'];
                $this->_values['weeknr']=$this->getWeekNR($this->_values['m'],$this->_values['Y'],$this->_values['d']);
            }else{
                if(isset($_GET['weeknr'])){
                    $this->_values['weeknr']=(\autosystem\ValidateAs::isnumeric($_GET['weeknr']))?$_GET['weeknr']:'';
                    $this->_values['m'] = $this->getMonth($this->_values['weeknr'],$this->_values['Y']);
                    $this->_values['d'] = $this->getStartof($this->_values['weeknr'],$this->_values['Y']);
                    if($this->_values['d']<1){
                        $this->_values['d'] = 31 + $this->_values['d'];
                    }
                }else{
                    $this->_values['weeknr']=$this->getWeekNR($this->_values['m'],$this->_values['Y']);
                }
            }
            if(!key_exists('d',$this->_values)){
                $this->_values['d']=1;
            }
            $timestamp=  mktime(0, 0, 0,$this->_values['m'],1,$this->_values['Y']);
            $this->_values['month.'.date('M',$timestamp)]=$this->_view->get_AtranslationB('month.'.date('M',$timestamp));
            $this->_values['M']= $this->_values['month.'.date('M',$timestamp)];
            $this->categorys=[];
            if(isset($_POST['find'])){
                    $this->_values['search']=$_POST['search'];       
            }else{
                $this->_validator->addForm('add_item',['id','TYPE','namekey','category_id','from','to','start','end'],false,['id'],$this->_values);
				$this->_validator->addSelect('add_item','category_id',$this->categorys);
				$this->_validator->addSelect('add_item','TYPE',$this->_model->modeltypes);
				$default=false;
				if(isset($_POST['add'])){
					$render=true;
                    $this->_values['additem']='';				    
				}elseif(isset($_POST['save'])){
                     $this->_values['save']='';
					 $render=true;	
				}elseif(isset($_GET['push'])){
                     $this->_values['add']='';
					 $render=true;	
				}else{
					$default=true;
                    $render=false;
				}
				if($render){
					$this->_validator->addAlocat('add_item',['id'=>'isnumeric','category_id'=>'isnumeric','namekey'=>'istext',
					'TYPE'=>'isselect','from'=>'isdate','to'=>'isdate','start'=>'istime','end'=>'istime']);
				    $this->_validator->render('add_item',$_POST,$this->_values);
				}
				//$this->status('',true,true,true,true);
				if($default!==false){
					if(isset($_GET['add'])){
                        $this->_validator->addForm('addWItem',['year','month','day','add','namekey'],false,['add','namekey'],$this->_values);
                        $this->_validator->addAlocat('addWItem',['year'=>'isY','day'=>'isD','month'=>'isM','namekey'=>'istext']);
                        $this->_validator->render('addWItem',$_GET,$this->_values);
                        if($this->_validator->isValid('addWItem')){
                            $date = $this->_values['year'].'-'.$this->_values['month'].'-'.$this->_values['day'];
                        }else{
                            $date = $this->_values['Y'].'-'.$this->_values['m'].'-'.$this->_values['d'];
                        }
                        $this->_values['from']=$date;
	                    $this->_values['to']=$date;
                    
                    if(isset($_GET['add'])){
                       $this->_validator->addForm('addWHItem',['hour','start','end','add','types'],false,['add','namekey'],$this->_values);
                       $this->_validator->addAlocat('addWHItem',[]);
                       $this->_validator->addSelect('addWHItem','TYPE',$this->_model->modeltypes);
                       $this->_validator->render('addWHItem',$_GET,$this->_values);
                       $this->_values['types'] = $this->_validator->getSelect('addWHItem','TYPE',$this->_view,$_GET['TYPE']);
                       $this->_values['hour']=(isset($_GET['hour'])&&\autosystem\ValidateAs::isnumeric($_GET['hour']))?$_GET['hour']:0;
                       $h = ($this->_values['hour']<10)? '0'.$this->_values['hour'] .':00':$this->_values['hour'] .':00';
                       $this->_values['start']=(isset($_GET['start'])&&\autosystem\ValidateAs::isTime($_GET['start']))?$_GET['start']:$h;
                       $h = ($this->_values['hour']+1<10)? '0'.$this->_values['hour']+1 .':00':$this->_values['hour']+1 .':00';
                       $this->_values['end']=(isset($_GET['end'])&&\autosystem\ValidateAs::isTime($_GET['end']))?$_GET['end']:$h;
                       $this->_values['types']= $this->_view->getOptions($this->_model->modeltypes,$this->_values['TYPE']);
                       $this->_values['add']=(isset($_GET['add'])&&\autosystem\ValidateAs::isnumeric($_GET['add']))?$_GET['page']:'';
                    }   
	                $this->_values['page']=0;
				}
                if(isset($_GET['view'])){
                    $this->_values['view']=(isset($_GET['view'])&&\autosystem\ValidateAs::isnumeric($_GET['view']))?$_GET['view']:'';
                }
                if(isset($_GET['edit'])){
                    $this->_values['edit']= '';
                    $this->_values['id']=(\autosystem\ValidateAs::isnumeric($_GET['id']))?$_GET['id']:null;
                }
                if(isset($_GET['copy'])){
                    $this->_values['copy']=(\autosystem\ValidateAs::isnumeric($_GET['copy']))?$_GET['copy']:null;
                }
                if(isset($_GET['detail'])){
                    $this->_values['id']=(\autosystem\ValidateAs::isnumeric($_GET['id']))?$_GET['id']:null;
                }
                if(isset($_GET['delete'])){
                	$this->_values['delete']='';
                	print '-----';
                    $this->_values['id']=(\autosystem\ValidateAs::isnumeric($_GET['id']))?$_GET['id']:null;
                }
            }
        }
        }
        private function checkDate($date,$type='US'){
                $a = trim($date);
                $a = str_replace([' ','/'],'-',$a);
                if(\autosystem\ValidateAs::isUSAdate($a)){
                    if($type =='US'){ return $a;    }
                    if($type =='EU'){ return implode('-',array_reverse(explode('-',$a)));   }
                }
                if(\autosystem\ValidateAs::isEUAdate($a)){
                    if($type =='EU'){ return $a;    }
                    if($type =='US'){ return implode('-',array_reverse(explode('-',$a)));   }
                }
                return false;
        }
        private function getMonthLen($monthnr,$year){
            if($monthnr%2==$monthnr>7){
                if($monthnr!=2){    return 30;
                }else{              return ($year%4==0)?29:28; 
                }
            }
            return 31;
        }
        private function items_render($hour,$asoarray,$day,&$has,&$extras){
            $return ='';//' <li>'.print_r($asoarray,true).$values['start_h'].'<'.$hour.'+10&& ('. $values['end_h'].'>'.$hour.'||('.$arr[1].'>0 &&'.$values['end_h'].'+1>'.$hour.')||'.$values['to_day'].'>'.$day.'</li>';//
            $inf['namekey']='';
            $inf['from']='';
            $inf['id']='';
            $inf['time_item_hour']='';
            $i = 0;
            foreach ($asoarray as $key=>$values){
                $values['time_item_hour']=$hour ;
                if(key_exists('start_h',$values) && key_exists('end_h',$values)&& !empty($values['start_h'])){
                    $arr =  explode(':',$values['end']);
                    if($values['start_h']<$hour+1 && ( $values['end_h']>$hour ||($arr[1]>0 &&  $values['end_h']+1>$hour)||$values['to_day']>$day)  ){
                        $values['namekey']=$values['namekey'].'-'.$values['end'];
                        $has = true;
                        $render = $values+$extras;
                        $return .= $this->_view->get_Templateclone($this->_loc,'week_time_key',$render);
                        $i++;
                    }
                }else{
                    $has = true;
                    $render = $values+$extras;
                    $return .= $this->_view->get_Templateclone($this->_loc,'week_time_key',$render);
                    $i++;
                }
                
            }
            $to = 4 - $i;//count($asoarray);
            for ($y = 0; $y <= $to; $y++){
                $return .= $this->_view->get_Templateclone($this->_loc,'week_time_key',$inf); 
            }
            return $return;
        }
        private function item_render(){

        }
        function getStartof($weeknr, $year) {
          $keys = array_keys($this->days);
          $day = strtolower(date('l', strtotime($year.'W'.str_pad($weeknr, 2, 0, STR_PAD_LEFT))));
          $key=0;
          foreach($keys as $k=>$value){
             if($day==$value){
                break;
             }
             $key++;
          }
          return date('d', strtotime($year.'W'.str_pad($weeknr, 2, 0, STR_PAD_LEFT)))-$key;
        }
        function getMonth($weeknr, $year){
          return date('m', strtotime($year.'W'.str_pad($weeknr, 2, 0, STR_PAD_LEFT)));
        }
        private function getstart($monthnr,$year){
            $first_day = mktime(0, 0, 0, $monthnr, 1, $year);
            return date('w', $first_day);
        }
        private function getWeekNR($monthnr,$year,$day=1){
            $first_day = mktime(0, 0, 0, $monthnr, $day, $year);
            return date('W', $first_day);
        }
    }
?>