<?php
	namespace users;
    class UserStats extends \autosystem\Controller{
        private $values = array('search','find');
        private $choises = [50=>0.2,100=>0.1,125=>0.08,200=>0.05 ,250=>0.04,400=>0.025,800=>0.0125,
                            1000=>0.01,1250=>0.008,2000=>0.005, 2500=>0.004,4000=>0.0025,5000=>0.002,8000=>0.00125,
                            10000=>0.001,12500=>0.0008,20000=>0.0005 ,25000=>0.0004,40000=>0.00025,80000=>0.000125,
                            100000=>0.0001,125000=>0.00008,200000=>0.00005 ,250000=>0.00004,400000=>0.000025,800000=>0.0000125];
        public function init() {
           $this->vars();
           $this->getModel();
           $this->prepare($this->_loc,$this->_action,$this->_values);
        }
        private function prepare($loc,$action,$values,$find =null){
            $GETSOMETHING = true;
            //print 'started';
            //print_r($this->_values);
            if(isset($this->_values['createMonthStat'])){
                $month = date('m');
                $year = date('Y');
                $topsize =1;
                $stat = new \stats\SVGLineStat(180,450,true);
                $level=$stat->setData('users','green',$this->_model->GET_logins($year,$month));             $topsize=($level>$topsize)?$level:$topsize;
                $level=$stat->setData('crimes','red',$this->_model->GET_crimes($year,$month));              $topsize=($level>$topsize)?$level:$topsize;
                $level=$stat->setData('visitors','blue',$this->_model->GET_visitors($year,$month));         $topsize=($level>$topsize)?$level:$topsize;
                $level=$stat->setData('pages','black',$this->_model->GET_pages($year,$month));              $topsize=($level>$topsize)?$level:$topsize;
                $level=$stat->setData('invalidlogin','black',$this->_model->GET_invalidlogins($year,$month));        $topsize=($level>$topsize)?$level:$topsize;
                if($month-1 ==0){//first month prev year        $topsize=($level>$topsize)?$level:$topsize;
                    $level=$stat->setData('pre users','#4E6A3C',$this->_model->GET_logins($year-1,12));     $topsize=($level>$topsize)?$level:$topsize;
                    $level=$stat->setData('pre crimes','orange',$this->_model->GET_crimes($year-1,12));     $topsize=($level>$topsize)?$level:$topsize;
                    $level=$stat->setData('pre visitors','#207AA0',$this->_model->GET_visitors($year-1,12));$topsize=($level>$topsize)?$level:$topsize;
                    $level=$stat->setData('pre pages','gray',$this->_model->GET_pages($year-1,12));         $topsize=($level>$topsize)?$level:$topsize;
                }else{
                    $level=$stat->setData('pre users','#4E6A3C',$this->_model->GET_logins($year,$month-1)); $topsize=($level>$topsize)?$level:$topsize;
                    $level=$stat->setData('pre crimes','orange',$this->_model->GET_crimes($year,$month-1)); $topsize=($level>$topsize)?$level:$topsize;
                    $level=$stat->setData('pre visitors','#207AA0',$this->_model->GET_visitors($year,$month-1));        $topsize=($level>$topsize)?$level:$topsize;
                    $level=$stat->setData('pre pages','gray',$this->_model->GET_pages($year,$month-1));     $topsize=($level>$topsize)?$level:$topsize;
                }
                $scale=1;
                foreach($this->choises as $amout=>$Ascale){
                    if($topsize<$amout){
                        $scale = $Ascale;
                        break;
                    }
                }
                if(isset($_GET['debug'])||isset($_POST['debug'])){
                   print $stat->getStatContent(300,500,'black',10,$scale);
                   print 'level:'.$level;
                   print 'write to:'.($put)?'OK':'NOK';
                }
                $put = file_put_contents('./layout/'.'stats/users.svg',$stat->getStatContent(300,500,'black',10,$scale));
                $GETSOMETHING=false;
            }
            if(isset($this->_values['createMonthStatb'])){
                $month = date('m');
                $year = date('Y');
                $topsize =1;
                $stat = new \stats\SVGBars(180,450,true);
                $level=$stat->setData('users','green',$this->_model->GET_logins($year,$month));             $topsize=($level>$topsize)?$level:$topsize;
                $level=$stat->setData('crimes','red',$this->_model->GET_crimes($year,$month));              $topsize=($level>$topsize)?$level:$topsize;
                $level=$stat->setData('visitors','blue',$this->_model->GET_visitors($year,$month));         $topsize=($level>$topsize)?$level:$topsize;
                $level=$stat->setData('pages','black',$this->_model->GET_pages($year,$month));              $topsize=($level>$topsize)?$level:$topsize;
                $level=$stat->setData('invalidlogin','black',$this->_model->GET_invalidlogins($year,$month));        $topsize=($level>$topsize)?$level:$topsize;
                if($month-1 ==0){//first month prev year        $topsize=($level>$topsize)?$level:$topsize;
                    $level=$stat->setData('pre users','#4E6A3C',$this->_model->GET_logins($year-1,12));     $topsize=($level>$topsize)?$level:$topsize;
                    $level=$stat->setData('pre crimes','orange',$this->_model->GET_crimes($year-1,12));     $topsize=($level>$topsize)?$level:$topsize;
                    $level=$stat->setData('pre visitors','#207AA0',$this->_model->GET_visitors($year-1,12));$topsize=($level>$topsize)?$level:$topsize;
                    $level=$stat->setData('pre pages','gray',$this->_model->GET_pages($year-1,12));         $topsize=($level>$topsize)?$level:$topsize;
                }else{
                    $level=$stat->setData('pre users','#4E6A3C',$this->_model->GET_logins($year,$month-1)); $topsize=($level>$topsize)?$level:$topsize;
                    $level=$stat->setData('pre crimes','orange',$this->_model->GET_crimes($year,$month-1)); $topsize=($level>$topsize)?$level:$topsize;
                    $level=$stat->setData('pre visitors','#207AA0',$this->_model->GET_visitors($year,$month-1));        $topsize=($level>$topsize)?$level:$topsize;
                    $level=$stat->setData('pre pages','gray',$this->_model->GET_pages($year,$month-1));     $topsize=($level>$topsize)?$level:$topsize;
                }
                $scale=1;
                foreach($this->choises as $amout=>$Ascale){
                    if($topsize<$amout){
                        $scale = $Ascale;
                        break;
                    }
                }
                if(isset($_GET['debug'])||isset($_POST['debug'])){
                   print $stat->getStatContent(300,500,'black',10,$scale);
                   print 'level:'.$level;
                   print 'write to:'.($put)?'OK':'NOK';
                }
                $put = file_put_contents('./layout/'.'stats/users.svg',$stat->getStatContent(300,500,'black',10,$scale));
                $GETSOMETHING=false;
            }
            if(isset($this->_values['createYearStat'])){
                $month = date('m');
                $year = date('Y');
                $statb = new \stats\SVGbanana(180,450,true);
                $statb->setData($year.'users','green',$this->_model->GET_logins($year),0);
                $statb->setData($year.'crimes','red',$this->_model->GET_crimes($year),0);
                $statb->setData($year.'visitors','blue',$this->_model->GET_visitors($year),0);
                $statb->setData($year.'invalidlogin','black',$this->_model->GET_invalidlogins($year),0);
                if(isset($_GET['debug'])){
                   print $statb->getStatContent(300,450,'black',10,1); 
                }
                file_put_contents('./layout/'.'stats/'.$year.'usersyear.svg',$statb->getStatContent(300,450,'black',10,1));
                $GETSOMETHING=false;
            }
            if(isset($this->_values['createYearStat'])){
                $month = date('m');
                $year = date('Y');
                $statb = new \stats\SVGbanana(180,450,true);
                $statb->setData($year.'users','green',$this->_model->GET_logins($year),0);
                $statb->setData($year.'crimes','red',$this->_model->GET_crimes($year),0);
                $statb->setData($year.'visitors','blue',$this->_model->GET_visitors($year),0);
                $statb->setData($year.'invalidlogin','black',$this->_model->GET_invalidlogins($year),0);
                if(isset($_GET['debug'])){
                   print $statb->getStatContent(300,450,'black',10,1); 
                }
                file_put_contents('./layout/'.'stats/'.$year.'usersyear.svg',$statb->getStatContent(300,450,'black',10,1));
                $GETSOMETHING=false;
            }
            if(isset($this->_values['createYearsecuritystat'])){
                $month = date('m');
                $year = $this->_values['year'];
                $topsize =1;
                $stat = new \stats\SVGLineStat(180,450,true);
                    //$level=$stat->setData('Y '.$year.' users','#4E6A3C',$this->_model->GET_logins($year)); $topsize=($level>$topsize)?$level:$topsize;
                    $level=$stat->setData('Y '.$year.' crimes','#FF0000',$this->_model->GET_crimes($year)); $topsize=($level>$topsize)?$level:$topsize;
                    $level=$stat->setData('Y '.$year-1 .' crimes','#FFFF00',$this->_model->GET_crimes($year-1)); $topsize=($level>$topsize)?$level:$topsize;
                    $level=$stat->setData('Y '.$year.' invalid login','#3366FF',$this->_model->GET_invalidlogins($year));     $topsize=($level>$topsize)?$level:$topsize;
                    $level=$stat->setData('Y '.$year-1 .' invalid login','#99CCFF',$this->_model->GET_invalidlogins($year-1));     $topsize=($level>$topsize)?$level:$topsize;
                $scale=1;
                foreach($this->choises as $amout=>$Ascale){
                    if($topsize<$amout){
                        $scale = $Ascale;
                        break;
                    }
                }
                
                if(isset($_GET['debug'])){
                    print $topsize;
                   print $stat->getStatContent(300,500,'black',10,$scale); 
                }
                file_put_contents('./layout/'.'stats/'.$year.'securitystat.svg',$stat->getStatContent(300,500,'black',10,$scale));
                $GETSOMETHING=false;
            }
            if(isset($this->_values['createYearvisitorsstat'])){
                $month = date('m');
                $year = $this->_values['year'];
                $topsize =1;
                $stat = new \stats\SVGLineStat(180,450,true);
                    //$level=$stat->setData('Y '.$year.' users','#4E6A3C',$this->_model->GET_logins($year)); $topsize=($level>$topsize)?$level:$topsize;
                $level=$stat->setData('Y '.$year.' visitors','#3366FF',$this->_model->GET_visitors($year));
                $level=$stat->setData('Y '.$year-1 .' visitors','#3366FF',$this->_model->GET_visitors($year-1));
                $level=$stat->setData('Y '.$year-2 .' visitors','#3366FF',$this->_model->GET_visitors($year-2));         $topsize=($level>$topsize)?$level:$topsize;
                $scale=1;
                foreach($this->choises as $amout=>$Ascale){
                    if($topsize<$amout){
                        $scale = $Ascale;
                        break;
                    }
                }
                
                if(isset($_GET['debug'])){
                    print $topsize;
                   print $stat->getStatContent(300,500,'black',10,$scale); 
                }
                file_put_contents('./layout/'.'stats/'.$year.'securitystat.svg',$stat->getStatContent(300,500,'black',10,$scale));
                $GETSOMETHING=false;
            }          
            if(isset($this->_values['errors'])){
                print '<section class="ERROR" >'.$this->_values['errors'].'</section>';
            }
            if($GETSOMETHING){
                print $this->_view->getheader($loc,$action);
                print $this->_view->get_template($this->_loc,$this->_action,$this->_values);
                $month = date('m');
                $year = $this->_values['year'];
                $topsize =1;
                $stat = new \stats\SVGLineStat(180,450,true);
                $level = $stat->setData('users','green',$this->_model->GET_logins($year,$month));                $topsize=($level>$topsize)?$level:$topsize;
                $level = $stat->setData('crimes','red',$this->_model->GET_crimes($year,$month));                 $topsize=($level>$topsize)?$level:$topsize;
                $level = $stat->setData('visitors','blue',$this->_model->GET_visitors($year,$month));            $topsize=($level>$topsize)?$level:$topsize;
                $level = $stat->setData('pages','black',$this->_model->GET_pages($year,$month));                 $topsize=($level>$topsize)?$level:$topsize;
                $level = $stat->setData('invalidlogin','black',$this->_model->GET_invalidlogins($year,$month));  $topsize=($level>$topsize)?$level:$topsize;
                if($month-1 ==0){//first month prev year
                    $level = $stat->setData('pre users','#4E6A3C',$this->_model->GET_logins($year-1,12));        $topsize=($level>$topsize)?$level:$topsize;
                    $level = $stat->setData('pre crimes','orange',$this->_model->GET_crimes($year-1,12));        $topsize=($level>$topsize)?$level:$topsize;
                    $level = $stat->setData('pre visitors','#207AA0',$this->_model->GET_visitors($year-1,12));   $topsize=($level>$topsize)?$level:$topsize;
                    $level = $stat->setData('pre pages','gray',$this->_model->GET_pages($year-1,12));            $topsize=($level>$topsize)?$level:$topsize;
                }else{
                    $level = $stat->setData('pre users','#4E6A3C',$this->_model->GET_logins($year,$month-1));    $topsize=($level>$topsize)?$level:$topsize;
                    $level = $stat->setData('pre crimes','orange',$this->_model->GET_crimes($year,$month-1));    $topsize=($level>$topsize)?$level:$topsize;
                    $level = $stat->setData('pre visitors','#207AA0',$this->_model->GET_visitors($year,$month-1));$topsize=($level>$topsize)?$level:$topsize;
                    $level = $stat->setData('pre pages','gray',$this->_model->GET_pages($year,$month-1));        $topsize=($level>$topsize)?$level:$topsize;
                }
                //print '<h1>'.$topsize. '</h1>';
                $scale=1;
                foreach($this->choises as $amout=>$Ascale){
                    if($topsize>$amout){
                        $scale = $Ascale;
                        break;
                    }
                }
                print $stat->getStatContent(300,450,'green',10,$scale);
                $statb = new \stats\SVGbanana(180,450,true);
                $statb->setData('users','green',$this->_model->GET_logins($year,$month));
                $statb->setData('crimes','red',$this->_model->GET_crimes($year,$month));
                $statb->setData('visitors','blue',$this->_model->GET_visitors($year,$month));
                $statb->setData('pages','black',$this->_model->GET_pages($year,$month));
                $statb->setData('invalidlogin','black',$this->_model->GET_invalidlogins($year,$month));
                
                file_put_contents('./layout/'.'stats/'.$year.'usersyear.svg',$statb->getStatContent(300,450,'black',10,0.004));
                print $statb->getStatContent(300,450,'black',10,0.004);
                print $this->_view->getfooter($action);
            }
        }
        private function vars(){
            $this->_values['year']=date('Y');
            if(isset($_GET['createMonthStat'])){      $this->_values['createMonthStat']=' ';       return;  }
            if(isset($_POST['createMonthStat'])){     $this->_values['createMonthStat']=' ';       return;  }
            if(isset($_GET['createYearsecuritystat'])){  $this->_values['createYearsecuritystat']=' ';
               if(isset($_GET['year'])&&\autosystem\ValidateAs::isYear($_GET['year'])){  $this->_values['year']=$_GET['year']; } 
               return;  }
            if(isset($_POST['createYearsecuritystat'])){ $this->_values['createYearsecuritystat']=' ';
               if(isset($_POST['year'])&&\autosystem\ValidateAs::isYear($_POST['year'])){  $this->_values['year']=$_POST['year']; 
			   }
               return;  }
            if(isset($_GET['createYearStat'])){               
               if(isset($_GET['setyear'])&&\autosystem\ValidateAs::isYear($_GET['setyear'])){
                    $this->_values['year']=$_GET['setyear']; 
               }
               $this->_values['createYearStat']=' ';   return;  
            }
            if(isset($_POST['createYearStat'])){ $this->_values['createYearStat']=' ';   return;  }
            if(isset($_POST['setyear'])){ $this->_values['createYearStat']=' ';   return;  }
            $this->_values = $this->_view->get_translations($this->values);
        }
    }
?>