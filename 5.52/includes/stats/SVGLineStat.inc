<?php
/**
 * @author Lieven Roegiers
 * @CMS autosite
 */
namespace stats;
class SVGLineStat{
    private $debug =true;
    private $x;
    private $y;
    private $lines =[];
    private $linestart = 25;
    private $raster = false;
    private $keys =[];
    private $maxsequenceY = 1 ;
    private $rasterlinelen = 5;
    private $scale =0.2;
    private $calc =false;
    private $bottomchart;
    private $Xseperations = 10;
    private $point ;
    public function __construct($x,$y,$calc = false){
        $this->x =$x;
        $this->y =$y;
        $this->calc = $calc;
        $this->bottomchart = ($this->x-10);
        $this->point = ($this->bottomchart-10)/$this->Xseperations;
    }
    public function getStatContent($Xcm,$Ycm,$rastercolor,$seperation = 12,$scale= 0.2 ,$txtrad=0){
        $this->scale = $scale;
        $return  = '<svg width="'.$Ycm.'" height="'.$Xcm.'" xmlns="http://www.w3.org/2000/svg" >';
        foreach($this->lines as $line ){
            $return  .= '<g fill="none" stroke="'.$line['color'].'" stroke-width="1.5" >';
            $y= $this->linestart + $seperation;
            if($this->calc){//SUM line
                $return  .= "\n".'<!--SUMLINE--><polyline points="';
                $x =(isset($line['sum']))?($this->bottomchart -($line['sum']*$this->point*$this->scale)):$this->bottomchart;

                $return  .= ' '.$y-$seperation .','.$x.' '.$y.','.$x;
                $return  .= '" stroke-width="1.5" />';
            }
            $return  .= '<polyline points="';
            foreach($this->keys as $k){
                if(isset($line['data'][$k])){
                    $d =$this->bottomchart-($line['data'][$k]*$this->point*$this->scale);
                    $x = $d;
                }else{
                    $x=$this->bottomchart;
                }
                $return  .= ' '.$y.','.$x;
                $y+=$seperation;
            }
            $return  .= '" />';
            if($this->calc){
                $return  .= "\n".'<!--AVERAGE--><polyline points="';
                $x =(isset($line['average']))?($this->bottomchart -($line['average']*$this->point*$this->scale)):$this->bottomchart;
                $return  .= ' '.$y+$seperation .','.$x.' '.$y.','.$x.'" stroke-width="1.5" />';
                $return  .= '<text x="'.($y+$seperation+$seperation).'" y="'.$x.'" font-size="6" >A '.$line['average'].'</text>';
            }
            $return  .= '</g>'."\n";
        }
        //print_r($this->lines);
        $return  .= $this->getraster($rastercolor,$seperation,$txtrad);
        $return  .= $this->getlegend(array_keys($this->lines),$seperation);
        $return  .= '</svg>';
        return $return;
    }
    public function setData($key,$color,$aso){
        $this->lines[$key]['color']=$color;
        $this->lines[$key]['data']=$aso;
        if($this->calc){
            $this->lines[$key]['sum']=array_sum($aso);
            if(count($aso)==0){
                $this->lines[$key]['average']=0;
            }else{
                $this->lines[$key]['average']= round($this->lines[$key]['sum'] / count($aso),0);
            }           
        }
        $this->keys = $this->keys + array_keys($aso);
        $arr = array_values($aso);
        natsort($arr);
        return array_pop($arr);
    }
    public function getAutoScale($topsize){
        $choises = [50=>0.2,100=>0.1,125=>0.08,200=>0.05 ,250=>0.04,400=>0.025,800=>0.0125,
                            1000=>0.01,1250=>0.008,2000=>0.005, 2500=>0.004,4000=>0.0025,5000=>0.002,8000=>0.00125,
                            10000=>0.001,12500=>0.0008,20000=>0.0005 ,25000=>0.0004,40000=>0.00025,80000=>0.000125,
                            100000=>0.0001,125000=>0.00008,200000=>0.00005 ,250000=>0.00004,400000=>0.000025,800000=>0.0000125];

        foreach($choises as $amout=>$Ascale){
            if($topsize < $amout){
                print 'top'.$topsize.'sc'.$Ascale;
                return $Ascale;
            }
        }
        return 0.01;
     }
     public function resetDimentionY($arr){
        print 'arry len'.count($arr);
        $y = 1000;
        $this->y =$y;
        return 1000;
     }     
  /**
   * SVGLineStat::setNullKeys()
   * wil every key that not exist on other data place to 0
   * or you can use presetrange for sorting 
   * @return void
   */
    public function setNullKeys($presetrange =[]){
		$keys ;
		foreach($this->lines as $linekey => $lineinfo){
			foreach($lineinfo['data']as $datakey => $value){
				$keys[$datakey] = 1 ;
			}
		}
		ksort($keys);
		foreach($this->lines as $linekey => $lineinfo){
			$arr= [];	
			foreach($keys as $datakey => $value){
				$arr[$datakey] = (key_exists($datakey,$lineinfo['data']))? $lineinfo['data'][$datakey]: 0;
			}
			$this->lines[$linekey]['data']= $arr;
		}
		$this->keys = array_keys($keys);
	}
    private function getlegend($keys,$seperation,$vertical=1){
        $return = '';
        $lengendx= (count($this->keys)+3)*($seperation+0.1);
        $lengendy= 10+ $seperation;
        foreach($keys as $key){
            $return  .= "\n".'<text x="'.($lengendx+40) .'" y="'.($lengendy+10).'" fill="'.$this->lines[$key]['color'].'" font-weight="bold" font-size="12px" >'.$key.'  '.(array_key_exists('sum',$this->lines[$key])?$this->lines[$key]['sum']:'').'</text>';
            if($vertical){
                $lengendy =$lengendy+$seperation;
            }
        }
        return $return ;
    }
    private function getraster($rastercolor ,$seperation ,$rotation = 0){
        $linelen=$this->rasterlinelen ;
        $left =  $this->linestart +$seperation ;
        $Lbottom = ($this->x-$seperation);
        $return  = '<g fill="none" stroke="'.$rastercolor.'" stroke-width="1" >';
        $return  .= '<polyline points="0,'.($this->bottomchart).' '.($this->y+$seperation+$this->linestart*2) .",".($this->bottomchart) .', "/>';  
        $y = 0;
        $wy=1;
        foreach($this->keys as $key){//horizontal as
            $y++;
            $wy= $y*$seperation+$this->linestart;
            $return  .= '<text x="'.($wy-6).'" y="'.($this->bottomchart+$linelen+$seperation).'" fill="blue" '.
			(($rotation>0)? 'transform=" rotate('.$rotation.' '.($wy-6).','.($this->bottomchart+$linelen+$seperation).')"':'').
			' font-size="10" >'.$key.'  </text>';//
            $return  .= '<polyline points="'.$wy.','.($this->bottomchart ).' '.$wy.','.($this->bottomchart+$linelen ).', "  />';
        }
        $return  .= '<polyline points="'.$left.',0 '.$left.','.$this->bottomchart  .' "  />';  //vertical line
        $wx=1;
        for($x = 1, $max = $this->Xseperations+1; $x < $max; $x=$x+1){//vertikale as horizontale lijnen lijnen
            $wx= $this->bottomchart-($this->point*$x);
            $return  .= '<polyline points="'.$left.','.$wx .' '.($linelen+$wy).','.$wx.' "  />';
            $return  .= '<text x="0" y="'.($wx+6).'" fill="blue" font-size="12" >'.(($x/$this->scale)).'</text>';
        }
        if($this->debug){
            $wx= $this->bottomchart;// -(10*$y+1)+$linelen;
            $return  .= '<polyline points="0,'.$this->bottomchart.' '.$linelen.','.$this->bottomchart.' " />';
            $return  .= '<text x="'.($this->y).'" y="'.($this->bottomchart +$seperation*2).'" fill="blue" font-size="12" >Y </text>';
        }
        $return  .= '</g>'."\n";
        return $return;
    }
}     
?>