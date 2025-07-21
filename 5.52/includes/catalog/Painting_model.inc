<?php
/**
* @author Lieven Roegiers
* @copyright 
* @CMS autosite 
*/
namespace catalog;
class Painting_model extends \catalog\Product_model{
    protected $isinstalled = false;
	public function __install(){
	    //$installed = parent::__install();//* isset($this->DB)&&$lnb'*/ 
	    return ; 
	}
    public function isInstalled(){
    	return $this->isinstalled|| $this->isinstalled=true;
    }
    public function GET_GroupInfo($location,$templatename,$view,$accound_id,$id){
        $cpgs= \autosystem\DBConn::getTableNameFor('catalog_product_groups');
    	$sql='SELECT * FROM `'.$cpgs.'` WHERE `'.$cpgs.'`.`id`='.$id  ;
        print $sql;
        print $this->AGET($location,$templatename,$view,$sql,null,true,$accound_id,'paint_');//true,$accound_id
    }
    public function GET_painting($location,$templatename,$view,$id,$accound_id){
        $cp= \autosystem\DBConn::getTableNameFor('catalog_paintingfrompainters');
    	$p= \autosystem\DBConn::getTableNameFor('catalog_product');
    	$cpg= \autosystem\DBConn::getTableNameFor('catalog_product_group');
    	$cpgs= \autosystem\DBConn::getTableNameFor('catalog_product_groups');
    	$cpgpv= \autosystem\DBConn::getTableNameFor('catalog_product_parameter_value');
    	$cpgp= \autosystem\DBConn::getTableNameFor('catalog_product_parameter');
    	$sql='SELECT catalog_paintingfrompainters.*,catalog_product.*,catalog_product_group.group_id ,catalog_product_groups.name AS groupname ,groupinfo '.
		'FROM `'.$cp.'` JOIN `'.$p.'` ON `'.$cp.'`.`product_id` = `'.$p.'`.`id`'.
        'JOIN `'.$cpg.'` ON `'.$cpg.'`.`product_id` = `'.$p.'`.`id`'.
		'JOIN `'.$cpgs.'` ON `'.$cpgs.'`.`id` = `'.$cpg.'`.`group_id` '.
		'WHERE `'.$p.'`.`id` = '.$id
        //.' AND `'.$cp.'`.`VISIBLE`=\'TRUE\''
        ;
        // $templateLocation,$templateName,&$view,$sql,$param=null,$images=false,$imgaccound_id=0,$imgprefix=''
        print $this->AGET($location,$templatename,$view,$sql,null,true,$accound_id,'paint_');//true,$accound_id
    }
    public function GET_paintingE($location,$templatename,$view,$id,$accound_id){
        $cp= \autosystem\DBConn::getTableNameFor('catalog_paintingfrompainters');
    	$p= \autosystem\DBConn::getTableNameFor('catalog_product');
    	$cpg= \autosystem\DBConn::getTableNameFor('catalog_product_group');
    	$cpgs= \autosystem\DBConn::getTableNameFor('catalog_product_groups');
        $cpgpv= \autosystem\DBConn::getTableNameFor('catalog_product_parameter_value');
    	$cpgp= \autosystem\DBConn::getTableNameFor('catalog_product_parameter');
    	$sql='SELECT `'.$cp.'`.*,`'.$p.'`.*'.
		'FROM `'.$cp.'` JOIN `'.$p.'` ON `'.$cp.'`.`product_id` = `'.$p.'`.`id`'.
		'WHERE `'.$p.'`.`id` = '.$id 
        //.' AND `'.$cp.'`.`VISIBLE`=\'TRUE\''
        ;
       $cpgpv= \autosystem\DBConn::getTableNameFor('catalog_product_parameter_value');
    	$cpgp= \autosystem\DBConn::getTableNameFor('catalog_product_parameter');
        $exploreoverwrite['parameters']='SELECT * FROM `'.$cpgpv.'` JOIN `'.$cpgp.'` ON `'.$cpgp.'`.`id` = `'.$cpgpv.'`.`parameter_id` WHERE `product_id`=:product_id ';
        $cpg= \autosystem\DBConn::getTableNameFor('catalog_product_group');
    	$cpgs= \autosystem\DBConn::getTableNameFor('catalog_product_groups');
        
        $exploreoverwrite['groupnames']='SELECT * FROM `'.$cpg.'` LEFT JOIN `'.$cpgs.'` ON `'.$cpg.'`.`group_id`=`'.$cpgs.'`.`id`'. 
        'WHERE `product_id`=:product_id';
        //print $exploreoverwrite['groupnames'];
        //$exploreoverwrite['groupinfo']='SELECT  FROM WHERE `group_id`=:group_id ';
        //print $sql;
        // $templateLocation,$templateName,&$view,$sql,$param=null,$images=false,$imgaccound_id=0,$imgprefix=''
        //print $this->AGET($location,$templatename,$view,$sql,null,true,$accound_id,'paint_');//true,$accound_id
        //$exploreoverwrite['product_id']='SELECT * FROM `'.$cpgpv.'` JOIN `'.$cpgp.'` ON `'.$cpgp.'`.`id` = `'.$cpgpv.'`.`parameter_id` WHERE `product_id`=:product_id ';
        //parent::GET_Explore($location,$templatename,$view,$sql,['product_id'=>'parameters'],$exploreoverwrite,null,true,$accound_id,'paint_');
        parent::GET_ExploreR($location,$templatename,$view,$sql,['groupnames'=>'product_id','parameters'=>'product_id'],$exploreoverwrite,null,true,$accound_id,'paint_');
    }
    private function GET_SQL($id,$group){
    	$cp= \autosystem\DBConn::getTableNameFor('catalog_paintingfrompainters');
    	$p= \autosystem\DBConn::getTableNameFor('catalog_product');
        $cpg= \autosystem\DBConn::getTableNameFor('catalog_product_group');
    	$sql='SELECT `'.$cp.'`.*,`'.$p.'`.*'.
		',IF(`'.$p.'`.`ENABLED`=\'TRUE\',\' checked=checked \',\' \') AS `isenabled` '.
		',IF(`'.$cp.'`.`VISIBLE`=\'TRUE\',\' checked=checked \',\' \') AS `isvisible` '.
		',IF(`'.$p.'`.`PROMOTE`=\'TRUE\',\' checked=checked \',\' \') AS `ispromote` '.
		'FROM `'.$cp.'` JOIN `'.$p.'` ON `'.$cp.'`.`product_id` = `'.$p.'`.`id`'
        ;//' ,
        $where = 'WHERE `'.$cp.'`.`VISIBLE`=\'TRUE\' ';//AND `label` IS NOT NULL
        $link = '';
        if(!empty($group)){// TODO: field not exist
            $where.= 'AND `product_id` IN (SELECT `product_id` FROM `'.$cpg.'` WHERE `group_id`='.$group.')';
        }
        $sql.= $where;
        $this->paginator->setSort(' ');//TODO ORDER BY `???`.`inputDate` DESC
        $this->paginator->setRange(20);
        return $sql;
    }
    public function GET_PaintingsSummary($location,$templatename,$view,$account_id,$id='',$pagenr=0,$group='',$parameters=[]){// TODO: check params
        $sql= $this->GET_SQL($id,$group);
    
        $len =$this->GET_COUNT($this->get_CountSQL($id,$group));
        $this->paginator->setlistlength($len);
        $this->paginator->setpagenr($pagenr);
        $this->paginator->setpaginatorreload($location,$templatename);
        $this->PREpaginator($view,$group,$parameters);
        $cpgpv= \autosystem\DBConn::getTableNameFor('catalog_product_parameter_value');
    	$cpgp= \autosystem\DBConn::getTableNameFor('catalog_product_parameter');
        $exploreoverwrite['parameters']='SELECT * FROM `'.$cpgpv.'` JOIN `'.$cpgp.'` ON `'.$cpgp.'`.`id` = `'.$cpgpv.'`.`parameter_id` WHERE `product_id`=:product_id ';
        
    	$cpg= \autosystem\DBConn::getTableNameFor('catalog_product_group');
    	$cpgs= \autosystem\DBConn::getTableNameFor('catalog_product_groups');
        
        $exploreoverwrite['groupnames']='SELECT * FROM `'.$cpg.'` LEFT JOIN `'.$cpgs.'` ON `'.$cpg.'`.`group_id`=`'.$cpgs.'`.`id`'. 
        'WHERE `product_id`=:product_id';
        //$exploreoverwrite['selectgroups']='SELECT * ,IF(`'.$cpg.'`.`product_id` NOT NULL ,\' checked=checked \',\' \') AS `isselected` FROM `'.$cpgs.'` JOIN `'.$cpg.'` ON `'.$cpg.'`.`group_id` = `'.$cpgs.'`.`id` WHERE `product_id`=:product_id OR `product_id` IS NULL';
        /*		'JOIN  ON `'.$cpgpv.'`.`product_id` = `'.$p.'`.`id` '.*/
        //$this->DEBUG=true;
        //print $sql;
        parent::GET_ExploreR($location,$templatename,$view,$sql,['parameters'=>'product_id','groupnames'=>'product_id'],$exploreoverwrite,null,true,$account_id,'paint_');
        $this->PREpaginator($view,$group,$parameters);
    }
    private function PREpaginator($view,$group,$parameters){
        if($group!=''){
            $view->getpaginator($this->paginator->getdata(),'&filter&group='.$group);
            return;
        }
        if(!empty($parameters)){
            $param ='';
            foreach($parameters as $k =>$v){
                $param .= '&parameter['.$k.']='.$v;
            }
            $view->getpaginator($this->paginator->getdata(),'&filter'.$param);
            return;
        }
        $view->getpaginator($this->paginator->getdata(),'');
    }

    private function get_CountSQL($account_id,$group){
       $sql='SELECT COUNT(*) AS `count`';
    	$cp= \autosystem\DBConn::getTableNameFor('catalog_paintingfrompainters');
    	$p= \autosystem\DBConn::getTableNameFor('catalog_product');
    	$cpg= \autosystem\DBConn::getTableNameFor('catalog_product_group');
    	$sql.='FROM `'.$cp.'` JOIN `'.$p.'` ON `'.$cp.'`.`product_id` = `'.$p.'`.`id`'
        ;//' ,
        $where = 'WHERE `'.$cp.'`.`VISIBLE`=\'TRUE\' ';//AND `label` IS NOT NULL
        $link = '';
        if(!empty($group)){// TODO: field not exist
            $where.= 'AND `product_id` IN (SELECT `product_id` FROM `'.$cpg.'` WHERE `group_id`='.$group.')';
        }
        $sql.= $where;
       return $sql;
    }
    public function GET_FilterSQL($group,$incparams,$excparams,$painter_id){
        $p= \autosystem\DBConn::getTableNameFor('catalog_product');
        $ppv= \autosystem\DBConn::getTableNameFor('catalog_product_parameter_value');
        $cp= \autosystem\DBConn::getTableNameFor('catalog_paintingfrompainters');
        //$cpg= \autosystem\DBConn::getTableNameFor('catalog_product_group');
        //	$cpgs= \autosystem\DBConn::getTableNameFor('catalog_product_groups');
        if($group==''){
            $sql = 'SELECT * FROM `'.$ppv.'` LEFT JOIN `'.$p.'` ON `'.$ppv.'`.`product_id` = `'.$p.'`.`id`'; 
            $paramb ='WHERE ';
            foreach($incparams as $param){
                 $sql .= $paramb.'`value`=\''.$param.'\'';
                 $paramb = ' OR';
            }
            if($painter_id>0){// TODO: field not exist
                $sql .= 'AND `id` IN (SELECT `product_id` FROM `'.$cp.'` WHERE `painter_id`='.$painter_id.')';//TODO check it out
            }
            $this->paginator->setRange(20);
            return $sql;
        }else{
            return 'group';
            //select * from catalog_product_group join catalog_product_parameter_value on atalog_product_group.prod
        }
    }
    public function GET_FilterSummary($location,$templatename,$view,$group,$incparams,$excparams,$pagenr,$account_id=0){
        $painter_id =($account_id>0)?  $this->GETID_catalog_painters(['accound_id'=>$account_id]):0;
        $sql = $this->GET_FilterSQL($group,$incparams,$excparams,$painter_id);
        //print $sql;
        $len =$this->GET_COUNT($this->GET_FilterCountSQL($group,$incparams,$excparams,$painter_id));
        
        $this->paginator->setlistlength($len);
        $this->paginator->setpagenr($pagenr);
        $this->paginator->setpaginatorreload($location,$templatename);
        $this->PREpaginator($view,$group,$incparams);
        //parent::GET_Summary($location,$templatename,$view,$sql,null,true,$account_id,'paint_');
        
        $cpgpv= \autosystem\DBConn::getTableNameFor('catalog_product_parameter_value');
    	$cpgp= \autosystem\DBConn::getTableNameFor('catalog_product_parameter');
        $exploreoverwrite['parameters']='SELECT * FROM `'.$cpgpv.'` JOIN `'.$cpgp.'` ON `'.$cpgp.'`.`id` = `'.$cpgpv.'`.`parameter_id` WHERE `product_id`=:product_id ';
        
    	$cpg= \autosystem\DBConn::getTableNameFor('catalog_product_group');
    	$cpgs= \autosystem\DBConn::getTableNameFor('catalog_product_groups');
        
        $exploreoverwrite['groupnames']='SELECT * FROM `'.$cpg.'` LEFT JOIN `'.$cpgs.'` ON `'.$cpg.'`.`group_id`=`'.$cpgs.'`.`id`'. 
        'WHERE `product_id`=:product_id';
        
        parent::GET_ExploreR($location,$templatename,$view,$sql,['parameters'=>'product_id','groupnames'=>'product_id'],$exploreoverwrite,null,true,$account_id,'paint_');
        $this->PREpaginator($view,$group,$incparams);
    }
    public function GET_FilterCountSQL($group,$incparams,$excparams,$painter_id){
        $p= \autosystem\DBConn::getTableNameFor('catalog_product');
        $ppv= \autosystem\DBConn::getTableNameFor('catalog_product_parameter_value');
        //$cpg= \autosystem\DBConn::getTableNameFor('catalog_product_group');
        //	$cpgs= \autosystem\DBConn::getTableNameFor('catalog_product_groups');
        if($group==''){
            $sql= 'SELECT COUNT(*) AS `count` FROM `'.$ppv.'` LEFT JOIN `'.$p.'` ON `'.$ppv.'`.`product_id` = `'.$p.'`.`id`'; 
            $paramb ='WHERE';
            foreach($incparams as $param){
                 $sql .= $paramb.'`value`=\''.$param.'\'';
                 $paramb = ' OR';
            }
            return $sql;
        }else{
            return 'group';
            //select * from catalog_product_group join catalog_product_parameter_value on atalog_product_group.prod
        }
    }
  public function getModelInfo(){  return 'catalog.Painting.model.info';  }
  public function __upgrade($version){ return true;  }
  public function getVersionNr(){  return (float)5.4;  }
  public function getRuleDefault(){return 'PUBLIC';} //'PUBLIC','FINANCIAL','TRANSLATE','MANAGE','ADMINISTRATION','KING','USER','MODERATOR','CHAIRMAN' }
}
?>