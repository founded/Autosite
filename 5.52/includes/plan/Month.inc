<?php
	namespace plan;
    class Month {
        private $days =['monday','tuesday','wednesday','thursday','friday','saturday','sunday'];
        private $month;
        private $year;
        private $weekNr;
        private $loc ='plan';
        public function __construct($month,$year) {
            $this->month = $month;
            $this->year = $year;
            $this->weekNr = $this->getWeekNR();
        }
        public function render(&$view ,$title = 'title',$items =[]){
           $i = 0;
           print $view->get_template('global','tooltip_start');
           $values['Callendar_title'] = $title;
           print $view->get_Templateclone($this->loc,'month_item_head',$values);
           foreach($this->days  as $key=>$val){
                $i++;
                $values['Callendar_title'] = $val;
                print $view->get_Templateclone($this->loc,'month_item_head',$values);
                if($i%7==0){print '<br>';}
           }
           $values['weeknr']=$this->getWeekNR($this->month,$this->year);
           print $view->get_Templateclone($this->loc,'month_week_foot',$values);
           $i = 0;
           print '<div class="Callendar_month_body" >';
           print $view->get_Templateclone($this->loc,'month_week_head',$values);
           $this->emtyplaces($view,$this->getStartInWeek()); 
           $monthlen =$this->getMonthLen($this->month,$this->year);
           for ($y = 1; $y <= $monthlen; $y++){
              $i++;
              print $view->get_Templateclone($this->loc,'month_item',['Y'=>$this->year,'m'=>$this->month,'Imonth'=>$y]);
              if($i%7==0){
                 $this->AnewWeek($view,++$values['weeknr']);
              }
           }
           $view->get_Templateclone($this->loc,'month_week_foot',$values);
        }
        public function render_smal(&$view ,$items =[]){
           $i = $this->getStartInWeek();
           $this->monthheader($view);
           $this->emtyplaces($view,$i,'month_small_item',['Imonth'=>'']); 
           $monthlen =$this->getMonthLen($this->month,$this->year);
           for ($y = 1; $y <= $monthlen; $y++){
              $i++;
              print $view->get_Templateclone($this->loc,'month_small_item',['Imonth'=>$y]);
              if($i%7==0){
                 print '<br>';
              }
           }
        }
        private function AnewWeek(&$view,$weeknr){
            print $view->get_Templateclone($this->loc,'month_week_foot',[]);                        
            print '<br>';
            print $view->get_Templateclone($this->loc,'month_week_head',['weeknr'=>$weeknr]);
        }
        private function emtyplaces(&$view,$amount,$template ='month_empty_item',$values=[]){
            for ($i = 0, $max = $amount; $i < $max; $i++) {
                print $view->get_Templateclone($this->loc,$template,$values);
            }
        }
        private function monthheader(&$view,$template ='month_small_item'){
            $a =['M','T','W','T','F','S','S'];
            foreach($a as $v){
                $values = ['Imonth'=>$v];
                print $view->get_Templateclone($this->loc,$template,$values);
            }
             print '<br>';
        }
        public function getMonthLen(){
            if($this->month%2==$this->month>7){
                if($this->month!=2){    return 30;
                }else{              return ($this->year%4==0)?29:28; 
                }
            }
            return 31;
        }
        private function getStartInWeek(){
            $first_day = mktime(0, 0, 0, $this->month, 0, $this->year);
            return date('w', $first_day);
        }
        public function getWeekNR(){
            $first_day = mktime(0, 0, 0, $this->month, 1, $this->year);
            return date('W', $first_day);
        }
    }
?>