<?php
namespace stats;
class SVGbanana{
    private $debug =false;
    private $x;
    private $y;
    private $lines =[];
    private $linestart = 25;
    private $raster = false;
    private $keys =[];
    private $maxsequenceY = 1 ;
    private $rasterlinelen = 5;
    private $scale =0.5;
    private $calc =true;
    private $bottomchart;
    private $Xseperations = 10;
    private $radius = 75;
    private $point;
    private $ref;
    public function __construct($x,$y,$calc = false){
        $this->x =$x;
        $this->y =$y;
        $this->bottomchart = ($this->x-10);
        $this->point = ($this->bottomchart-10)/$this->Xseperations;
    }
    public function getStatContent($Xcm,$Ycm,$rastercolor,$seperation = 12,$scale= 1){
        $this->scale = $scale;
        $return  = '<svg  width="'.$Ycm*$this->scale.'" height="'.$Xcm*$this->scale.'" xmlns="http://www.w3.org/2000/svg" >';
        $xpos = $Xcm*$this->scale/2;
        $ypos = $Ycm*$this->scale/2;
        $size = 57.29577 *$this->scale;
        $deep = $size;
        //$size = 57.29577 *$this->scale;
        foreach($this->lines as $key => $niveau){
            $counter = 0;
            $keys =array_keys($niveau);
            foreach($keys as $key){
                $counter += $niveau[$key]['sum'];
            }
            $counter = ($counter>0)?round(360 /$counter,4):0;
            $degcounter=0 ;
            $deep-= 6;
            foreach($keys as $key){
                $deg = round($niveau[$key]['sum']*$counter);
                $return .= $this->extlabel($xpos,$ypos,$size+10,$deg ,10 ,$key.' '.$niveau[$key]['sum'],$niveau[$key]['color']);
                $return .= $this->A($xpos,$ypos,$deep,$degcounter,$degcounter+$deg,$niveau[$key]['color']);
                $degcounter += $deg;
                $deep -=1;
            }
        }
        $return  .= $this->getraster($rastercolor ,$xpos,$ypos,$size);
        $return  .= '</svg>';
        return $return;
    }
    public function setData($key,$color,$aso,$linenr = 0){
        $this->lines[$linenr][$key]['color']=$color;
        if($this->calc){
            $this->lines[$linenr][$key]['sum']=array_sum($aso);
            $this->lines[$linenr][$key]['average']= (count($aso)>0)?round($this->lines[$linenr][$key]['sum']/count($aso),0):0;
        }
    }
    private function getlegend($keys,$seperation,$vertical=1){
        $return = '';
        $lengendx=500;
        $lengendy =1;
        foreach($keys as $key){
            $return  .= "\n".'<text x="'.$lengendx .'" y="'.($lengendy).'" fill="'.$this->lines[$key]['color'].'" font-weight="bold" font-size="12px" >'.$key.'  '.(array_key_exists('sum',$this->lines[$key])?$this->lines[$key]['sum']:'').'</text>';
            if($vertical){
                $lengendy =$lengendy+$seperation;
            }
        }
        return $return ;
    }
    private function getraster($rastercolor ,$hpos ,$vpos,$straal){
        $rad = $straal;
        $linelen= 15;
        $return ='';
        $return  .= '<g fill="none" stroke="black" stroke-width="1" >';
        $i = 2;
        for($deg = 0, $max = 360; $deg < $max; $deg=$deg+5){
            $i++;
            if($deg==90||$deg==180 ||$deg==270 ||$deg==0  ){
                $return  .='<polyline points="'.round($hpos-$this->H2($rad,$deg)).','.round($vpos-$this->V2($rad,$deg)).' '.round($hpos-$this->H2($rad+10,$deg)).','.round($vpos-$this->V2($rad+10,$deg)).' "  />';
            }elseif($deg%45 ==0){
                $return  .='<polyline points="'.round($hpos-$this->H2($rad-1,$deg)).','.round($vpos-$this->V2($rad-1,$deg)).' '.round($hpos-$this->H2($rad+6,$deg)).','.round($vpos-$this->V2($rad+6,$deg)).' "  />';
            }elseif($deg%2 ==0){
                $return  .='<polyline points="'.round($hpos-$this->H2($rad,$deg)).','.round($vpos-$this->V2($rad,$deg)).' '.round($hpos-$this->H2($rad+4,$deg)).','.round($vpos-$this->V2($rad+4,$deg)).' "  />';
            }else{
                $return  .='<polyline points="'.round($hpos-$this->H2($rad,$deg)).','.round($vpos-$this->V2($rad,$deg)).' '.round($hpos-$this->H2($rad+2,$deg)).','.round($vpos-$this->V2($rad+2,$deg)).' "  />';
            }
        }
        $return  .= '</g>'."\n";
        return $return;
    }
    private function A($x, $y, $radius, $startAngle, $endAngle,$color = 'blue' ){
        $startx = round($this->polarToCartesianX($x,$radius, $startAngle),2);
        $starty = round($this->polarToCartesianY($y, $radius, $startAngle),2);
        $endx = round($this->polarToCartesianX($x, $radius, $endAngle),2);
        $endy = round($this->polarToCartesianY($y, $radius, $endAngle),2);
        $largeArcFlag = $endAngle - $startAngle <= 180 ? '0' : '1';
        $st = 'M '.$this->startline($startx,$starty,$radius, $startAngle).' A '. $radius.' '.$radius.' 0 '. $largeArcFlag.' 0 '. $endx .' '. $endy ;
        $st.= 'L '.$this->endline($endx,$endy,$radius, $endAngle);
        return '<path fill="none" stroke="'.$color.'" stroke-width="3" d="'.$st.' "  /> ';
    }
    function endline($x,$y,$radius, $deg ,$len=5){
        $endx = $this->polarToCartesianX($x, $len,  $deg);
        $endy = $this->polarToCartesianY($y, $len,  $deg);
        return $x.','. $y.' '. $endx .','. $endy ;
    }
    function startline($x,$y,$radius, $deg ,$len=5){
        $endx = $this->polarToCartesianX($x, $len,  $deg);
        $endy = $this->polarToCartesianY($y, $len,  $deg);
        return  $endx .','. $endy.' '.$x.','. $y ;
    }
    function extlabel($x,$y,$radius, $deg ,$len=5 ,$text,$color){
        $startx = $this->polarToCartesianX($x, $radius,  $deg);
        $starty = $this->polarToCartesianY($y, $radius,  $deg);
        $endx = $this->polarToCartesianX($startx, $len,  $deg);
        $endy = $this->polarToCartesianY($starty, $len,  $deg);
        $txtx = $this->polarToCartesianX($endx, 5,  $deg);
        $txty = $this->polarToCartesianY($endy, 5,  $deg);
        $inv = ($deg>90&&$deg<270)?'end':'start';
        $return = '<path fill="none" stroke="'.$color.'" stroke-width="3" d="M '.$startx.','.$starty.' '. $endx .','. $endy.'" />';
        $return .='<text x="'.$txtx.'" y="'.$txty.'" fill="'.$color.'" style="text-anchor: '.$inv.'" font-weight="bold" font-size="12" >'.$text.'</text>';
        return  $return ;
    }
    function polarToCartesianX($posX, $radius, $angledeg) {
         return $posX + ($radius * cos($this->rad($angledeg)));
    }
    function polarToCartesianY($posY, $radius, $angeldeg) {
         return $posY - ($radius * sin($this->rad($angeldeg)));
    }
    private function rad($deg){
        return  M_PI*$deg/180;//($deg-90) * M_PI / 180.0;
    }
    private function getraster3d($rastercolor ,$seperation){
        return $return;
    }
    private function H2($radius,$angledeg){
        return -$radius*cos($this->rad($angledeg));
    }
    private function V2($radius,$angledeg){
        return -$radius*sin($this->rad($angledeg));
    } 
    private function getAngle($val,$tot){
        return round($val*360/$tot);
    }  
}     
?>