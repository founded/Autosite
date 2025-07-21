<?php
namespace catalog;
class PainterProfile_model extends \catalog\Painting_model{
    protected $isinstalled = false;
  public function __install(){
    //$installed = parent::__install();
    if(!$this->existTablePainters()){
     $sql = 'CREATE TABLE IF NOT EXISTS `'.\autosystem\DBConn::getTableNameFor('catalog_painters').'`('.
       '`id` INT (5)  NOT NULL AUTO_INCREMENT,'.
       '`publicname` VARCHAR(20),'.
       '`accound_id` INT (20),'.
       '`biografic` TEXT NOT NULL,'.
       '`stars` INT (3),'.
      ' PRIMARY KEY ( `id` )'.
     ')ENGINE = InnoDB DEFAULT CHARSET='.\autosystem\DBConn::getCHARSET().' COLLATE='.\autosystem\DBConn::getCOLLATE().' ;';
     $result1 = $this->DB->query($sql);
     $return1 = $result1->rowCount()==1;
    }else{
     $return1 = true;
    }
    if(!$this->existTablePaintingfrompainters()){
     $sql = 'CREATE TABLE IF NOT EXISTS `'.\autosystem\DBConn::getTableNameFor('catalog_paintingfrompainters').'`('.
       '`product_id` INT (10),'.
       '`painter_id` INT (5),'.
       '`VISIBLE` ENUM(\'TRUE\',\'FALSE\')NOT NULL DEFAULT \'FALSE\','.
       '`backgroundwal_id` INT (5),'.
      ' PRIMARY KEY ( `product_id` , `painter_id` )'.
     ')ENGINE = InnoDB DEFAULT CHARSET='.\autosystem\DBConn::getCHARSET().' COLLATE='.\autosystem\DBConn::getCOLLATE().' ;';
     $result2 = $this->DB->query($sql);
     $return2 = $result2->rowCount()==1;
    }else{
     $return2 = true;
    }
    $sql['INSERT INTO `art`.`catalog_product_parameter` (`keyname`, `PUBLIC`) VALUES (\'size\', \'TRUE\');'];
    $return2 =  ($this->transaction($sql)!==false);
//* isset($this->DB)&&$lnb'*/
  	return $return1  && $return2  ; 
  }
    public function isInstalled(){
    return $this->isinstalled|| $this->existTablePainters()&& $this->existTablePaintingfrompainters()&& $this->isinstalled=true;
    }
 /*
 * options 
*/	
    public function GET_MyPainting($location,$templatename,$view,$id,$accound_id){
        $cp= \autosystem\DBConn::getTableNameFor('catalog_paintingfrompainters');
    	$p= \autosystem\DBConn::getTableNameFor('catalog_product');
    	$cpg= \autosystem\DBConn::getTableNameFor('catalog_product_group');
    	$cpgs= \autosystem\DBConn::getTableNameFor('catalog_product_groups');
        $cpgpv= \autosystem\DBConn::getTableNameFor('catalog_product_parameter_value');
    	$cpgp= \autosystem\DBConn::getTableNameFor('catalog_product_parameter');
    	$sql='SELECT `'.$cp.'`.*,`'.$p.'`.*'.
		'FROM `'.$cp.'` JOIN `'.$p.'` ON `'.$cp.'`.`product_id` = `'.$p.'`.`id`'.
		'WHERE `'.$p.'`.`id` = '.$id ;
       $cpgpv= \autosystem\DBConn::getTableNameFor('catalog_product_parameter_value');
    	$cpgp= \autosystem\DBConn::getTableNameFor('catalog_product_parameter');
        $exploreoverwrite['parameters']='SELECT * FROM `'.$cpgpv.'` JOIN `'.$cpgp.'` ON `'.$cpgp.'`.`id` = `'.$cpgpv.'`.`parameter_id` WHERE `product_id`=:product_id ';
        $cpg= \autosystem\DBConn::getTableNameFor('catalog_product_group');
    	$cpgs= \autosystem\DBConn::getTableNameFor('catalog_product_groups');
        $exploreoverwrite['groupnames']='SELECT * FROM `'.$cpg.'` LEFT JOIN `'.$cpgs.'` ON `'.$cpg.'`.`group_id`=`'.$cpgs.'`.`id`'. 
        'WHERE `product_id`=:product_id';
        $exploreoverwrite['selectgroups']='SELECT `group_id` , `name`, IF(`'.$cpg.'`.`group_id` >0 ,\' selected=selected \',\' \') AS `isselected` FROM `'.$cpg.'` LEFT JOIN `'.$cpgs.'` ON `'.$cpg.'`.`group_id` = `'.$cpgs.'`.`id` '. 
        'WHERE `product_id`=:product_id '.
        'UNION DISTINCT SELECT `id`,`name`,NULL '.
        'FROM `'.$cpgs.'` WHERE `id` NOT IN (SELECT `group_id` FROM `'.$cpg.'` WHERE `product_id`=:product_id )';
        parent::GET_ExploreR($location,$templatename,$view,$sql,['selectgroups'=>'product_id','groupnames'=>'product_id','parameters'=>'product_id'],$exploreoverwrite,null,true,$accound_id,'paint_');
    }
    public function SAVEpainting($name,$group_id,$shortcomment,$detail,$accound_id,$promote,$size){
        print $accound_id.$promote.'--'.$size;
		$painter_id = $this->GETID_catalog_painters(['accound_id'=>$accound_id]);
		$parameter_id = $this->GETID_catalog_product_parameter(['keyname'=>'size']);
		$imputdate=$this->nowDateTime();
		$ip = $this->getIp();
		$a = \autosystem\DBConn::getTableNameFor('catalog_product');
		$p = \autosystem\DBConn::getTableNameFor('catalog_paintingfrompainters');
		$g = \autosystem\DBConn::getTableNameFor('catalog_product_group');
		$param = \autosystem\DBConn::getTableNameFor('catalog_product_parameter_value');
		$key ='INSERT INTO `'.$a.'`(`id`,`name`,`shortcomment`, `detail`, `accound_id`, `ENABLED`, `PROMOTE`,`imputDate`,`ip`) '.
	                      'VALUES(NULL,:name,:shortcomment,:detail,:accound_id,\'TRUE\',:PROMOTE,:imputDate,:ip) RETURNING `product_id`';//
	    $sql[$key]= [':name'=>$name,':shortcomment'=>$shortcomment,':detail'=>$detail,':accound_id'=>$accound_id,
		':PROMOTE'=>$promote,':imputDate'=>$imputdate,':ip'=>$ip];
		$key ='INSERT INTO `'.$p.'`(`product_id`,`painter_id`, `VISIBLE`,`backgroundwal_id`) '.
					'VALUES(:product_id,:painter_id,\'TRUE\',:backgroundwal_id)';//
	    $sql[$key]= [':product_id'=>'',':painter_id'=>$painter_id,'backgroundwal_id'=>0];
	    $key ='INSERT INTO `'.$g.'`(`group_id`,`product_id`)'.
					'VALUES(:group_id,:product_id)';//
	    $sql[$key]= [':group_id'=>$group_id,':product_id'=>''];
	    $key ='INSERT INTO `'.$param.'`(`parameter_id`,`product_id`,`value`) '.
					'VALUES(:parameter_id,:product_id,:value)';
	    $sql[$key]= [':parameter_id'=>$parameter_id,':product_id'=>'',':value'=>$size];
		return $this->transaction($sql,true);
	}
    private function GET_PROFILE($painter_id){
       $cp= \autosystem\DBConn::getTableNameFor('catalog_paintingfrompainters');
    	$p= \autosystem\DBConn::getTableNameFor('catalog_product');
    	$cpg= \autosystem\DBConn::getTableNameFor('catalog_product_group');
    	$cpgs= \autosystem\DBConn::getTableNameFor('catalog_product_groups');
    	$sql='SELECT catalog_paintingfrompainters.*,catalog_product.*'.
		',IF(`'.$p.'`.`ENABLED`=\'TRUE\',\' checked=checked \',\' \') AS `isenabled` '.
		',IF(`'.$cp.'`.`VISIBLE`=\'TRUE\',\' checked=checked \',\' \') AS `isvisible` '.
		',IF(`'.$p.'`.`PROMOTE`=\'TRUE\',\' checked=checked \',\' \') AS `ispromote` '.
		'FROM `'.$cp.'` JOIN `'.$p.'` ON `'.$cp.'`.`product_id` = `'.$p.'`.`id`';
        $where = 'WHERE `'.$cp.'`.`painter_id`=\''.$painter_id.'\'';
        $sql.= $where;
        $this->paginator->setSort('  ');
        $this->paginator->setRange(15);
        //print($sql);
        return $sql;
    }
    private function userPromo_SQL($painter_id){
        $cp= \autosystem\DBConn::getTableNameFor('catalog_paintingfrompainters');
    	$p= \autosystem\DBConn::getTableNameFor('catalog_product');
    	$cpg= \autosystem\DBConn::getTableNameFor('catalog_product_group');
    	$sql='SELECT `'.$cp.'`.*,`'.$p.'`.*'.
		',IF(`'.$p.'`.`ENABLED`=\'TRUE\',\' checked=checked \',\' \') AS `isenabled` '.
		',IF(`'.$cp.'`.`VISIBLE`=\'TRUE\',\' checked=checked \',\' \') AS `isvisible` '.
		',IF(`'.$p.'`.`PROMOTE`=\'TRUE\',\' checked=checked \',\' \') AS `ispromote` '.
		'FROM `'.$cp.'` JOIN `'.$p.'` ON `'.$cp.'`.`product_id` = `'.$p.'`.`id`';
        $where = 'WHERE `promote`=\'TRUE\' AND`'.$cp.'`.`painter_id`=\''.$painter_id.'\'';
        return $sql.$where;
    }
    public function GET_userPromoSummary($location,$templatename,$view,$account_id){
        $painter_id = $this->GETID_catalog_painters(['accound_id'=>$account_id]);
        $sql= $this->userPromo_SQL($painter_id);
        $cpgpv= \autosystem\DBConn::getTableNameFor('catalog_product_parameter_value');
    	$cpgp= \autosystem\DBConn::getTableNameFor('catalog_product_parameter');        
        $exploreoverwrite['parameters']='SELECT * FROM `'.$cpgpv.'` JOIN `'.$cpgp.'` ON `'.$cpgp.'`.`id` = `'.$cpgpv.'`.`parameter_id` WHERE `product_id`=:product_id ';
    	$cpg= \autosystem\DBConn::getTableNameFor('catalog_product_group');
    	$cpgs= \autosystem\DBConn::getTableNameFor('catalog_product_groups');
        $exploreoverwrite['selectgroups']='SELECT `group_id` , `name`, IF(`'.$cpg.'`.`group_id` >0 ,\' selected=selected \',\' \') AS `isselected` FROM `'.$cpg.'` LEFT JOIN `'.$cpgs.'` ON `'.$cpg.'`.`group_id` = `'.$cpgs.'`.`id` '. 
        'WHERE `product_id`=:product_id '.
        'UNION DISTINCT SELECT `id`,`name`,NULL '.
        'FROM `'.$cpgs.'` WHERE `id` NOT IN (SELECT `group_id` FROM `'.$cpg.'` WHERE `product_id`=:product_id )';
        $exploreoverwrite['groupnames']='SELECT * FROM `'.$cpg.'` LEFT JOIN `'.$cpgs.'` ON `'.$cpg.'`.`group_id`=`'.$cpgs.'`.`id`'. 
        'WHERE `product_id`=:product_id';
        parent::GET_ExploreR($location,$templatename,$view,$sql,['selectgroups'=>'product_id','parameters'=>'product_id','groupnames'=>'product_id'],$exploreoverwrite,null,true,$account_id,'paint_');
    }
    public function GET_PainterProfilesSummary($location,$templatename,$view,$account_id,$pagenr=0){// TODO: check params
        $painter_id = $this->GETID_catalog_painters(['accound_id'=>$account_id]);
        $sql= $this->GET_PROFILE($painter_id);
        $len =$this->GET_COUNT($this->get_PROFILECountSQL($painter_id));
        $this->paginator->setlistlength($len);
        $this->paginator->setpagenr($pagenr);
        $this->paginator->setpaginatorreload($location,$templatename);
        $cpgpv= \autosystem\DBConn::getTableNameFor('catalog_product_parameter_value');
    	$cpgp= \autosystem\DBConn::getTableNameFor('catalog_product_parameter');
        $exploreoverwrite['parameters']='SELECT * FROM `'.$cpgpv.'` JOIN `'.$cpgp.'` ON `'.$cpgp.'`.`id` = `'.$cpgpv.'`.`parameter_id` WHERE `product_id`=:product_id ';
    	$cpg= \autosystem\DBConn::getTableNameFor('catalog_product_group');
    	$cpgs= \autosystem\DBConn::getTableNameFor('catalog_product_groups');
        $exploreoverwrite['selectgroups']='SELECT `group_id` , `name`, IF(`'.$cpg.'`.`group_id` >0 ,\' selected=selected \',\' \') AS `isselected` FROM `'.$cpg.'` LEFT JOIN `'.$cpgs.'` ON `'.$cpg.'`.`group_id` = `'.$cpgs.'`.`id` '. 
        'WHERE `product_id`=:product_id '.
        'UNION DISTINCT SELECT `id`,`name`,NULL '.
        'FROM `'.$cpgs.'` WHERE `id` NOT IN (SELECT `group_id` FROM `'.$cpg.'` WHERE `product_id`=:product_id )';
        $exploreoverwrite['groupnames']='SELECT * FROM `'.$cpg.'` LEFT JOIN `'.$cpgs.'` ON `'.$cpg.'`.`group_id`=`'.$cpgs.'`.`id`'. 
        'WHERE `product_id`=:product_id';
        parent::GET_ExploreR($location,$templatename,$view,$sql,['selectgroups'=>'product_id','parameters'=>'product_id','groupnames'=>'product_id'],$exploreoverwrite,null,true,$account_id,'paint_');
        //<select size="1" >
        //parent::GET_Summary($location,$templatename,$view,$sql,null,true,$account_id,'paint_');
        $view->getpaginator($this->paginator->getdata());
    }
    private function get_PROFILECountSQL($painter_id){
        $cp= \autosystem\DBConn::getTableNameFor('catalog_paintingfrompainters');
    	$p= \autosystem\DBConn::getTableNameFor('catalog_product');
    	$cpg= \autosystem\DBConn::getTableNameFor('catalog_product_group');
    	$cpgs= \autosystem\DBConn::getTableNameFor('catalog_product_groups');
    	$sql='SELECT COUNT(*) AS `count` FROM `'.$cp.'` JOIN `'.$p.'` ON `'.$cp.'`.`product_id` = `'.$p.'`.`id`'.
		'JOIN `'.$cpg.'` ON `'.$cpg.'`.`product_id` = `'.$p.'`.`id`'.
		'JOIN `'.$cpgs.'` ON `'.$cpgs.'`.`id` = `'.$cpg.'`.`group_id` ';
        $where = 'WHERE `'.$cp.'`.`painter_id`=\''.$painter_id.'\'';
       $sql.= $where;
       return $sql;
    }
    public function GET_NofotoSQL($painter_id){
        $p= \autosystem\DBConn::getTableNameFor('catalog_product');
        $pps= \autosystem\DBConn::getTableNameFor('catalog_paintingfrompainters');
        $ib= \autosystem\DBConn::getTableNameFor('images_bind');
        $ic= \autosystem\DBConn::getTableNameFor('images_category');
        $sql = 'SELECT catalog_paintingfrompainters.*,catalog_product.*'.
		',IF(`'.$p.'`.`ENABLED`=\'TRUE\',\' checked=checked \',\' \') AS `isenabled` '.
		',IF(`'.$pps.'`.`VISIBLE`=\'TRUE\',\' checked=checked \',\' \') AS `isvisible` '.
		',IF(`'.$p.'`.`PROMOTE`=\'TRUE\',\' checked=checked \',\' \') AS `ispromote` '.
        'FROM `'.$p.'` JOIN `'.$pps.'` ON `'.$p.'`.id = `'.$pps.'`.`product_id` '.
'LEFT JOIN `'.$ib.'` ON `'.$ib.'`.`dest_id` = `'.$p.'`.`id` '.
'LEFT JOIN `'.$ic.'` ON `'.$ic.'`.`id`= `'.$ib.'`.`category_id` '.
'WHERE `painter_id` = '.$painter_id.' AND `label` IS NULL';
        $this->paginator->setSort('  ');
        $this->paginator->setRange(20);
        return $sql;
    }
    public function GET_NofotoSummary($location,$templatename,$view,$account_id,$pagenr=0){
        $painter_id = $this->GETID_catalog_painters(['accound_id'=>$account_id]);
        $sql= $this->GET_NofotoSQL($painter_id);
        $len =$this->GET_COUNT($this->GET_NofotoCountSQL($painter_id));
        $this->paginator->setlistlength($len);
        $this->paginator->setpagenr($pagenr);
        $this->paginator->setpaginatorreload($location,$templatename);
        $cpgpv= \autosystem\DBConn::getTableNameFor('catalog_product_parameter_value');
    	$cpgp= \autosystem\DBConn::getTableNameFor('catalog_product_parameter');
        $exploreoverwrite['parameters']='SELECT * FROM `'.$cpgpv.'` JOIN `'.$cpgp.'` ON `'.$cpgp.'`.`id` = `'.$cpgpv.'`.`parameter_id` WHERE `product_id`=:product_id ';
    	$cpg= \autosystem\DBConn::getTableNameFor('catalog_product_group');
    	$cpgs= \autosystem\DBConn::getTableNameFor('catalog_product_groups');
        $exploreoverwrite['selectgroups']='SELECT `group_id` , `name`, IF(`'.$cpg.'`.`group_id` >0 ,\' selected=selected \',\' \') AS `isselected` FROM `'.$cpg.'` LEFT JOIN `'.$cpgs.'` ON `'.$cpg.'`.`group_id` = `'.$cpgs.'`.`id` '. 
        'WHERE `product_id`=:product_id '.
        'UNION DISTINCT SELECT `id`,`name`,NULL '.
        'FROM `'.$cpgs.'` WHERE `id` NOT IN (SELECT `group_id` FROM `'.$cpg.'` WHERE `product_id`=:product_id )';
        $exploreoverwrite['groupnames']='SELECT * FROM `'.$cpg.'` LEFT JOIN `'.$cpgs.'` ON `'.$cpg.'`.`group_id`=`'.$cpgs.'`.`id`'. 
        'WHERE `product_id`=:product_id';
        //$exploreoverwrite['selectgroups']='SELECT * ,IF(`'.$cpg.'`.`product_id` NOT NULL ,\' checked=checked \',\' \') AS `isselected` FROM `'.$cpgs.'` JOIN `'.$cpg.'` ON `'.$cpg.'`.`group_id` = `'.$cpgs.'`.`id` WHERE `product_id`=:product_id OR `product_id` IS NULL';
        /*		'JOIN  ON `'.$cpgpv.'`.`product_id` = `'.$p.'`.`id` '.*/
        //$this->DEBUG=true;
        //print $sql;
        $view->getpaginator($this->paginator->getdata());
        parent::GET_ExploreR($location,$templatename,$view,$sql,['selectgroups'=>'product_id','parameters'=>'product_id','groupnames'=>'product_id'],$exploreoverwrite,null,true,$account_id,'paint_');
        $view->getpaginator($this->paginator->getdata());
    }
    public function getSizes($size){
        //catalog_product_parameter
        return [];
    }
    public function GET_NofotoCountSQL($painter_id){
        $p= \autosystem\DBConn::getTableNameFor('catalog_product');
        $pps= \autosystem\DBConn::getTableNameFor('catalog_paintingfrompainters');
        $ib= \autosystem\DBConn::getTableNameFor('images_bind');
        $ic= \autosystem\DBConn::getTableNameFor('images_category');
        $sql = 'SELECT COUNT(*) AS `count` FROM `'.$p.'` JOIN `'.$pps.'` ON `'.$p.'`.id = `'.$pps.'`.`product_id` 
LEFT JOIN `'.$ib.'` ON `'.$ib.'`.`dest_id` = `'.$p.'`.`id` 
LEFT JOIN `'.$ic.'` ON `'.$ic.'`.`id` = `'.$ib.'`.`category_id` 
WHERE `painter_id` = '.$painter_id.' AND `'.$ic.'`.`label` IS NULL';
        $this->paginator->setSort('  ');
        $this->paginator->setRange(20);
        return $sql;
    }
  public function existTablePainters(){     return $this->existTableLabel('catalog_painters');   }
  public function existTablePaintingfrompainters(){     return $this->existTableLabel('catalog_paintingfrompainters');   }
  public function getModelInfo(){  return 'catalog.PainterProfile.model.info';  }
  public function __upgrade($version){ return true;  }
  public function getVersionNr(){  return (float)5.4;  }
  public function getRuleDefault(){return 'USER';} //'PUBLIC','FINANCIAL','TRANSLATE','MANAGE','ADMINISTRATION','KING','USER','MODERATOR','CHAIRMAN' }
}
?>