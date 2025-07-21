<?php
/**
 * @author Lieven Roegiers
 * @copyright 2016 
 * @CMS autosite
*/
namespace autosystem;
class Paginator{
    private $pagenr =0;//actual page
    private $totalPages;
    private $range ;//size of view
    private $sort = '';
    private $reload ='';
    private $pathurl='';
    private $pagewindow=5;
    public function setpagenr($pagenr){
        //if($pagenr>$this->range)return;
        if($pagenr>0){
            $this->pagenr = $pagenr;
        }
    }
    public function getsql(){
       if(isset($this->pagenr)&& isset($this->range)&& $this->pagenr!=0){
            return ' '.$this->sort.' LIMIT '.($this->pagenr*$this->range).','.$this->range.'';
       }else{
            return ' '.$this->sort.((isset($this->range)&& $this->range>0)?' LIMIT '.$this->range.'':'');
       }
    }
    public function setlistlength($max){
        if(empty($this->range)){print '<h1>NO RANGE SET BY PAGINATOR</h1>'; return false;}
        $pagesequents = $max-($max%$this->range);
        if($pagesequents>=$this->range){
            $this->totalPages = $pagesequents/$this->range;
            if($max%$this->range==0){
                $this->totalPages--;
            }
        }else{
            $this->totalPages = 0;
        }
        //print '<br>'.$this->totalPages."=(($max-(".($max%$this->range)."))/$this->range)";//-1
    }
    public function setRange($range){
        $this->range = $range;
    }
    public function setSort($sort){
        $this->sort = $sort;
    }
    public function setpathurl($pathurl){
        $this->pathurl = $pathurl;
    }
    public function setpaginatorreload($location,$templatename){
        $this->reload = '/'.$location.'/'.$templatename.'/';
    }
    public function getdata(){
        $p = $this->pagewindow-1;
        $data['paginpre']=$this->pagenr;
        $data['paginnr']=$this->pagenr;
        $data['paginerp']=($this->totalPages-$this->pagenr>$p)?$p:$this->totalPages-$this->pagenr;
        $data['paginfirst']=0;
        $data['paginprev']=($this->pagenr>0)? $this->pagenr-1:0;
        $data['canIprev']=($this->pagenr == 0 )?'disabled" disabled="disabled':'';
        $data['paginnext']=($this->totalPages > $this->pagenr)? $this->pagenr+1:$this->totalPages;
        $data['canInext']=($this->pagenr==$this->totalPages || $this->pagenr>$this->totalPages)?'disabled" disabled="disabled':'';
        $data['paginlast']=$this->totalPages;
        $data['paginatorreload']=$this->reload;
        $data['path.url']=$this->pathurl;
        return $data;
    }
}
?>