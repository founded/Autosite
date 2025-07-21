<?php
namespace plan;
class Callendar extends \autosystem\Controller{
    private  $values=array('search','find','Callendar.edit','Callendar.add','Callendar.next','Callendar.early', 'Callendar.late','Callendar.prev','Callendar.month','Callendar.year','Callendar.inf','Callendar.titel','Callendar.weeknr');
    private  $days= array('monday','tuesday','wednesday','thursday','friday','saturday','sunday');
    private  $items=[];
    public function init() {
       $this->getModel(); 
       $this->vars();
       if(isset($this->_values['delete'])){
            $this->_model->delete($this->_values['delete'],$this->getAccound_id());
       }
       if(count($_POST)>0){
            if(isset($this->_values['add'])&& $id = parent::getAccound_id()){
                 $issaved=$this->_model->save(
                    $this->_values['namekey'],$this->_values['from'],$this->_values['to'],
                    $id,$this->_values['TYPE']
                 );
                 if($issaved){
                    $this->_values['title']=$this->_view->get_AtranslationB('saved');
                 }
            }
            if(isset($this->_values['save'])&&isset($this->_values['edit'])&& $id = parent::getAccound_id()){
                 $issaved=$this->_model->update(
                    $this->_values['id'],$this->_values['namekey'],$this->_values['from'],
                    $this->_values['to'],$id,$this->_values['TYPE']
                 );   
                 if($issaved){
                    $this->_values['title']=$this->_view->get_AtranslationB('saved');
                 }
            }               
       }
       $this->prepare($this->_loc,$this->_action,$this->_values);
    }
    private function prepare($loc,$action,$values,$find =null){
       print $this->_view->getheader($this->_loc,$action);
       print $this->_view->get_template($this->_loc,$this->_action,$this->_values);
       $GETEVERYTHINGELSE = true;
       if(isset($this->_values['view'])){
            print $this->_view->getform_head($this->_loc,$this->_action);
            print $this->_view->get_template($this->_loc,'CallendarItem_add',$this->_values);
            print $this->_view->getform_end('','add');
            $GETEVERYTHINGELSE = false;
       }
       if(isset($this->_values['ics'])){
            print $this->_view->get_template($this->_loc,'Callendar_ics_home',$this->_values);
			$path = $this->_user->getPersonalPath().'ics';
			print $path;
			if(!is_dir($path)){
				if(!is_dir($this->_user->getPersonalPath())){
					print 'error';
				}else{
					mkdir($path, 0644);
				}
			}else{
				$icsscan = new \plan\ICSScanner();
				$invitations = $icsscan->getIcsFiles($path.'/');
				foreach($invitations as $file){
				   $x=$this->_values ;
				   $x['filename']= $file;
				   $x['data']= print_r($icsscan->readics($path.'/'.$x['filename']),true);
		           print $this->_view->get_template($this->_loc,'Callendar_ics_item',$x);
		        }
			}
			$GETEVERYTHINGELSE = false;
	   }
       if(isset($this->_values['importics'])){
            print $this->_view->getform_head($this->_loc,$this->_action);
            print $this->_view->get_template($this->_loc,'CallendarItem_add',$this->_values);
            print $this->_view->getform_end('','add');
            $GETEVERYTHINGELSE = false;
       }
       if(isset($this->_values['summary'])){
             $this->_model->GET_Summary($this->_loc,'CallendarItem_add',$this->_values);
            $GETEVERYTHINGELSE = false;
       }
       if($GETEVERYTHINGELSE){
           $i = 0;
           print $this->_view->get_template('global','tooltip_start');
           $items = $this->_model->GET_Items($this->_values['Y'],$this->_values['m'],parent::getAccound_id());
           //print "<pre>".print_r($items,true)."</pre>";
           $values['Callendar_title'] = $values['Callendar.weeknr'];
           

           print $this->_view->get_Templateclone($this->_loc,'month_item_head',$values);
           foreach($this->days  as $key=>$val){
                $i++;
                $values['Callendar_title'] = $val;
                print $this->_view->get_Templateclone($this->_loc,'month_item_head',$values);
                if($i%7==0){print '<br>';}
           }
           $values['weeknr']=$this->getWeekNR($this->_values['m'],$this->_values['Y']);
           print $this->_view->get_Templateclone($this->_loc,'month_week_foot',$values);
           $i = 0;
           print '<div class="Callendar_month_body" >';
           $values['team']=$this->getPloeg($this->getAccound_id());
           $values['worktime']=$this->getWorkTime($values['team'],$values['weeknr']);
           print $this->_view->get_Templateclone($this->_loc,'month_week_head',$values);
           for ($i = 0, $max = $this->getstart($this->_values['m'],$this->_values['Y']); $i < $max; $i++){
                print $this->_view->get_Templateclone($this->_loc,'month_empty_item',$values);
           }
           $monthlen =$this->getMonthLen($this->_values['m'],$this->_values['Y']);
           if($this->_values['m']==1){
                $values['weeknr']=0;
           }
           //print $monthlen ."--".$this->_values['d']."--".$this->_values['m'];
           $day_data['m']=$this->_values['m'];
           $day_data['Y']=$this->_values['Y'];
           if(date('m')==$this->_values['m']&&date('Y')==$this->_values['Y']){
             $isActualMonth = true;
           }
           for($day = 1; $day <= $monthlen; $day++){
                $i++;
                $day_data['Imonth']=$day;
                if(key_exists($day,$items)){
                    $day_data['status']='active';
                    $day_data['items_small']=$this->items_small(count($items[$day]));
                    $day_data['items_detail']=$this->items_detail($items[$day]);
                    $day_data['tooltip']='visible';
                }else{
                    $day_data['status']='empty';
                    $day_data['tooltip']='hidden';
                    $day_data['items_small']='';
                    $day_data['items_detail']='';
                }
                if($isActualMonth && date('d')==$day){
                    $day_data['status']=($day_data['status']!='active')?'today':'active today';
                }
                print $this->_view->get_Templateclone($this->_loc,'month_item',$day_data);
                if($i%7==0){
                    print $this->_view->get_Templateclone($this->_loc,'month_week_foot',$values);                        
                    //print '<br>';
                    $values['weeknr']++;

                    $values['team']=$this->getPloeg($this->getAccound_id());
                    $values['worktime']=$this->getWorkTime($values['team'],$values['weeknr']);
                    $values['worktimeinfo']=$values['Callendar.'.$values['worktime']];
                    if($ploeg=='B')return(($weeknr%2)==1)?:$values['Callendar.late'];
                    print $this->_view->get_Templateclone($this->_loc,'month_week_head',$values);
                }
           }
           $this->_view->get_Templateclone($this->_loc,'month_week_foot',$values);
           print '</div>';
       }
       print $this->_view->getfooter($action);
    }
    private function getPloeg($acound_id){
        return 'A';
    }
    private function getWorkTime($ploeg,$weeknr){//'Callendar.next',
        if($ploeg=='A')return (($weeknr%2)==1)?'L':'V';
        if($ploeg=='B')return(($weeknr%2)==1)?'V':'L';
    }
    private function items_small($amount){
        $return = ' ';
        for ($day = 1; $day <= $amount; $day++){
            $return .='<span class="" >X</span>';
        }
        return $return;
    }
    private function items_detail($asoarray){
        $return = ' ';
        foreach ($asoarray as $key=>$values){
            $return .= $this->_view->get_Templateclone($this->_loc,'month_item_tooltipitem',$values);
        }
        return $return;
    }
    private function vars(){
        $this->_values = $this->_view->get_translations($this->values);
        $this->days = $this->_view->get_translations($this->days);
        $this->_values['title']='callendar:';
        $this->_values['Y'] = (isset($_GET['year'])&&\autosystem\ValidateAs::isYear($_GET['year']))? $_GET['year']:date('Y');
        $this->_values['m'] = (isset($_GET['month'])&&\autosystem\ValidateAs::isMonth($_GET['month']))? $_GET['month']:date('m');
        $this->_values['d'] = (isset($_GET['day'])&&\autosystem\ValidateAs::isDay($_GET['day']))? $_GET['day']:date('m');;
        $timestamp=  mktime(0, 0, 0,$this->_values['m'],1,$this->_values['Y']);
        $this->_values['month.'.date('M',$timestamp)]=$this->_view->get_AtranslationB('month.'.date('M',$timestamp));
        $this->_values['M']= $this->_values["month.".date("M",$timestamp)];
        $this->_values['nextm']= '<a class="btn btn-info" href="./?year='. (($this->_values['m']+1>12)?($this->_values['Y']+1).'&month=01':$this->_values['Y'].'&month='.($this->_values['m']+1)).'" >'.$this->_values['Callendar.next'].' '.$this->_values['Callendar.month'].'</a>';
        $this->_values['prevm']= '<a class="btn btn-info" href="./?year='. (($this->_values['m']-1>0)?$this->_values['Y'].'&month='.($this->_values['m']-1):($this->_values['Y']-1).'&month=12').'" >'.$this->_values['Callendar.prev'].' '.$this->_values['Callendar.month'].'</a>';
        $this->_values['nextY']= '<a class="btn btn-info" href="./?year='. ($this->_values['Y']+1).'&month='.$this->_values['m'].'" >'.$this->_values['Callendar.next'].' '.$this->_values['Callendar.year'].'</a>';
        $this->_values['prevY']= '<a class="btn btn-info" href="./?year='. ($this->_values['Y']-1).'&month='.$this->_values['m'].'" >'.$this->_values['Callendar.prev'].' '.$this->_values['Callendar.year'].'</a>';
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
                $this->_values['from']=($adate!=false)?$adate:''   ;
                $adate = $this->checkDate(filter_input(INPUT_POST, 'to'));
                $this->_values['to']=($adate!=false)?$adate:'';
                
            }else{
                $date = $this->_values['d'].'-'.$this->_values['m'].'-'.$this->_values['Y'];
                $this->_values['from']=$date;
                $this->_values['to']=$date;
            }
            if(isset($_GET['view'])){
                $this->_values['view']=(isset($_GET['view'])&&\autosystem\ValidateAs::isnumeric($_GET['view']))?$_GET['view']:'';
            }
            $this->icsvars();
            if(isset($_GET['delete'])){
                $this->_values['delete']=(isset($_GET['id'])&&\autosystem\ValidateAs::isnumeric($_GET['id']))?$_GET['id']:'';
            }
            if(isset($_GET['edit'])){
                $this->_values['edit']=(\autosystem\ValidateAs::isnumeric($_GET['edit']))?$_GET['edit']:null;
            }
            if(isset($_GET['copy'])){
                $this->_values['copy']=(\autosystem\ValidateAs::isnumeric($_GET['copy']))?$_GET['copy']:null;
            }
            if(isset($_GET['add'])){
               if(false){
               }
               $this->_values['types']= $this->_view->getOptions($this->_model->modeltypes,$this->_values['TYPE']);
               $this->_values['add']=(isset($_GET['add'])&&\autosystem\ValidateAs::isnumeric($_GET['add']))?$_GET['page']:'';
            }else{
               $this->_values['page']=0; 
            }
            if(isset($_POST['add'])){    $this->_values['additem']='';         }
            if(isset($_POST['save'])){   $this->_values['save']='';            }
        }
    }
    private function icsvars(){
         if(isset($_GET['ics'])){
            $this->_values['ics']=(isset($_GET['ics'])&&\autosystem\ValidateAs::isnumeric($_GET['ics']))?$_GET['ics']:'';
         }
         if(isset($_GET['MyicsFiles'])){       $this->_values['MyicsFiles']='';             }
         if(isset($_GET['MyicsFiles'])){       $this->_values['MyicsFiles']='';             }
         if(isset($_GET['MyicsFiles'])){       $this->_values['MyicsFiles']='';             }
    }
    private function checkDate($date,$type='US'){
        $a = trim($date);
        $a = str_replace([' ','/'],'-',$a);
        $array = explode('-',$a);
        foreach($array as $key=>$piece){
            if(strlen($piece)==1 ){
                $array[$key]= '0'.$piece;
            }
        }
        if(\autosystem\ValidateAs::isUSAdate($a)){
            if($type =='US'){      return $a;                     }
            if($type =='EU'){      return implode('-',array_reverse($array));     }
        }
        if(\autosystem\ValidateAs::isEUAdate($a)){
            if($type =='EU'){      return $a;                     }
            if($type =='US'){      return implode('-',array_reverse($array));     }
        }
        return false;
    }
    private function getMonthLen($monthnr,$year){
        if($monthnr%2==$monthnr>7){
            if($monthnr!=2){           return 30;
            }else{                     return ($year%4==0)?29:28; 
            }
        }
        return 31;
    }
    private function getstart($monthnr,$year){
        $first_day = mktime(0, 0, 0, $monthnr, 0, $year);
        return date('w', $first_day);
    }
    private function getWeekNR($monthnr,$year){
        $first_day = mktime(0, 0, 0, $monthnr, 1, $year);
        return date('W', $first_day);
    }
    private function isSpecialday($monthnr,$year){
        /*
        $Pasen ;
        a = DateSerial($year, 4, 1) / 7
        if($year Mod 19 = 0){
            $b = 19;
        }
        c = ($year Mod 19 + $b) * 19 - 7
        d = (c Mod 30) * 14 / 100
        Pasen = FormatNumber(a + d, 0) * 7 - 6
        Goede_Vrijdag = Pasen - 2
        EerstePaasdag = Pasen
        Tweedepaasdag = Pasen + 1
        Hemelvaart = Pasen + 39
        EerstePinksterdag = Pasen + 49
        Tweedepinksterdag = Pasen + 50
        EersteKerstdag = DateSerial($year, 12, 25)
        Tweedekerstdag = DateSerial($year, 12, 26)
        */
    }
}
?>