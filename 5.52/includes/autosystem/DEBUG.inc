<?php
namespace autosystem;
class DEBUG{
   static public function printPDO($sql,$param,$pre = true,$msg=''){
        $return = $sql;
        if(is_array($param)){
            foreach ($param as $order => $replace){
            	$return = str_replace($order,"'".$replace."'", $return);
            }
        }
        return ($pre)? '<pre class="error PDO-error" >'.$return.'[info::'.$msg.'] </pre>':$return;
   }
   static public function printVALUES($arr,$status='ERROR'){
        $return = $sql;
        if(is_array($param)){
            foreach ($param as $order => $replace){
            	$return = str_replace($order,'\''.$replace.'\'', $return);
            }
        }
        print '<pre class="ERROR" >'.$return.'</pre>';
   }
   static public function printTransaction($sql,$pre = true){
        $return = '';
        foreach ($sql as $sqlitem => $param){
            print $sqlitem .'*<br>'."\n";
            $item = $sqlitem;
            if(is_array($param)){
        	   foreach ($param as $order => $replace){
            	   $item = str_replace($order,'\''.$replace.'\'', $item);
               }
            }
            $return .= '['.$item."],\n";
        }
        return ($pre)? '<pre class="error PDO-error" >ARRAY['.$return.']</pre>':$return;
   }
   static public function transformToSelect($sql,$param){
        preg_match_all('#UPDATE(.*)SET#U',$sql,$matches);
        //print_r($matches);
        self::LTRIM('WHERE',$sql);
        $return = $sql;
        if(is_array($param)){
            foreach ($param as $order => $replace){
            	$return = str_replace($order,"'".$replace."'", $return);
            }
        }
        return 'SELECT * FROM '.$matches[1][0]. ' WHERE '.$return ;
   }
   public function CUT($start_String,$end_String,$content){
        $start = strpos($content,$start_String);
        $len = strlen($end_String);
        $end = strpos($content,$end_String,$start) + $len;//+length of key
        if($start !=0 ){          $content = substr($content,$start,$end-$start);}
        return $content;
   }
   private function LTRIM($start_String,&$content){
        $start = strpos($content,$start_String)+ strlen($start_String);
        if($start !=0 ){          $content = substr($content,$start);        }
   }
   private function RTRIM($stop_String,&$content){
        $start = strpos($content,$stop_String);
        if($start !=0 ){          $content = substr($content,0,$start);      }
    }
    static public function printERR($e,$comment=''){
        $s = print_r($e,true);
        $b = preg_replace(array('-\<-'),'&gd;',$s);
        $b = '<div class="PDO-error" ><table class="errorline" ><tr>'.preg_replace(array('-\[file\]-'),' </tr><tr class="errorline" >[file] ',$s).'</pre></tr></table></div>';
        $b = preg_replace(array('-\[file\]-'),'</td><td class="erroritem errorfile" > FILE: ',$b);
        $b = preg_replace(array("-\n-"),'',$b);
        $b = preg_replace(array('-  -'),'',$b);
        $b = preg_replace(array('-\[0\]-'),'',$b);
        $b = preg_replace(array('-\[line\]-'),'</td><td class="erroritem errorlinenr" > LINE: ',$b);
        $b = preg_replace(array('-\[function\]-'),'</td><td class="erroritem errorfunction" > ',$b);
        $b = preg_replace(array('-\[class\]-'),'<br> ',$b);//
        $b = preg_replace(array('-Qtemplate NOT FOUND-'),'<h2 class="errorline" >Qtemplate NOT FOUND</h2> ',$b);
        $b = preg_replace(array('-bgcolor="#FFCC00"-'),'bgcolor="#eeeeec"',$b);
        $b = preg_replace(array('-Dir not exist-'),'<h2 class="errorline" >Dir not exist</h2> ',$b);
        $b = preg_replace(array('-\[type\]-'),' ',$b);
        $b = preg_replace(array('-\[args\]-'),'</td><td class="erroritem errorargs" ><pre>  ',$b);
        $b = preg_replace(array('-=>-'),'',$b);
        $b = preg_replace(array('-#eeeeec-'),'',$b);
        $b = preg_replace(array('-Array\(-'),'',$b);   
        print $b;
        print('ER IS EEN FOUT<br><br><br><br><br><br>');
        print $comment.'<link rel="stylesheet" href="/v3.0/layout/Xdebug.css" >';
        die();
    }
    static public function printERR_B($e){
        print('x');
        $s =  print_r($e,true);
        $b = "\n".'<div class="PDO-error" > <ul id="siteerror" ><li><a href="#" ><span>start</span></a><ul><pre>'.preg_replace(array('-\[file\]-'),' </pre>[file]',$s).'</li></pre></ul></div>';
        $b = preg_replace(array('-\[file\]-'),"\n".'</li></ul></li><li class="erroritem_li" ><a href="#" ><span><h1 class="ERROR" >FILE: ',$b);
        $b = preg_replace(array('-\[line\]-'),' LINE: ',$b);
        $b = preg_replace(array('-\[function\]-'),"</h1></span></a>\n<ul>".'<li class="erroritem_li" > <div id="tooltip-" class="mytooltip " > ',$b);
        $b = preg_replace(array('-\[class\]-'),'<br> ',$b);//
        $b = preg_replace(array('-Qtemplate NOT FOUND-'),'<h2 class="errorline" >Qtemplate NOT FOUND</h2><a >/template/TemplateSummary/?create'.'<a>',$b);
        $b = preg_replace(array('-bgcolor="#FFCC00"-'),' bgcolor="#eeeeec"',$b);
        $b = preg_replace(array('-Dir not exist-'),"<h2 class='errorline' >Dir not exist</h2> ",$b);
        $b = preg_replace(array('-\[type\]-'),' ',$b);
        //$b = preg_replace(array('-\[args\]-'),"<li class='erroritem errorargs' >\n ".' <pre> ',$b);
        $b = preg_replace(array('-\[args\]-'),"<li class='erroritem errorargs' >\n ".' <pre> ',$b);
        
		$b = preg_replace(array('-=>-'),'',$b);
        //$b = preg_replace(array("-\n            [-"),"",$b);
        $b = preg_replace(array('-#eeeeec-'),'',$b);   
        print $b;
        print('ER IS EEN FOUT<br><br><br><br><br><br>');
        print '<link rel="stylesheet" href="/v3.0/layout/Xdebug.css" >';
    }
}
?>