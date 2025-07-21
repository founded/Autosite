<?php
/* model*/
/**
* @author Lieven Roegiers
* @copyright 
* @CMS autosite 
*/
namespace catalog;
class Product_model extends \autosystem\Model{
  protected $states = ['READY'=>'progress','SELLOUT' => 'sellout','WAIT' => 'wait','CANCELED' => 'canceled'];
  public $type = ['NEW'=>'new','HOTSELL'=>'hotsell','UNIQUE'=>'unique','DEADSTOCK'=>'deadstock','OTHER'=>'other'];
  public $section = [''=>'not selected','TITLE'=>'title','DETAIL'=>'detail'];
  protected $parametertype = ['PRODUCTSIZE'=>'size','BOXSIZE'=>'size','VOLUME'=>'volume','WEIGHT'=>'weight','AMOUNT'=>'amount','OTHER'=>'other'];
  protected $isinstalled = false;
  public function __install(){
    //$installed = parent::__install();
    if(!$this->existTableProductWorkspaceTask()){
     $sql = 'CREATE TABLE IF NOT EXISTS `'.\autosystem\DBConn::getTableNameFor('catalog_product_workspace_task').'`('.
       '`product_id` INT (10),'.
       '`atask_id` INT (7),'.
       '`EXUCUTED` ENUM(\'TRUE\',\'FALSE\')NOT NULL DEFAULT \'FALSE\','.
      ' PRIMARY KEY ( `product_id`,`atask_id` )'.
     ')ENGINE = InnoDB DEFAULT CHARSET='.\autosystem\DBConn::getCHARSET().' COLLATE='.\autosystem\DBConn::getCOLLATE().' ;';
     $result1 = $this->DB->query($sql);
     $return1 = $result1->rowCount()==1;
    }else{
     $return1 = true;
    }

    if(!$this->existTableProductCategory()){
     $sql = 'CREATE TABLE IF NOT EXISTS `'.\autosystem\DBConn::getTableNameFor('catalog_product_category').'`('.
       '`id` INT (4) NOT NULL AUTO_INCREMENT,'.
       '`label` VARCHAR (230) COLLATE '.\autosystem\DBConn::getCOLLATE().' NOT NULL,'.
       '`ENABLED` ENUM(\'TRUE\',\'FALSE\')NOT NULL DEFAULT \'FALSE\','.
       '`CANBACKORDER` ENUM(\'TRUE\',\'FALSE\')NOT NULL DEFAULT \'FALSE\','.
       '`imput_user` INT (10),'.
       '`departement_id` INT (10),'.
      ' PRIMARY KEY ( `id` )'.
     ')ENGINE = InnoDB DEFAULT CHARSET='.\autosystem\DBConn::getCHARSET().' COLLATE='.\autosystem\DBConn::getCOLLATE().' ;';
     $result2 = $this->DB->query($sql);
     $return2 = $result2->rowCount()==1;
    }else{
     $return2 = true;
    }

    if(!$this->existTableProductVat()){
     $sql = 'CREATE TABLE IF NOT EXISTS `'.\autosystem\DBConn::getTableNameFor('catalog_product_vat').'`('.
       '`id` INT (4) NOT NULL AUTO_INCREMENT,'.
       '`procent` INT (3),'.
       '`info` VARCHAR (5000) COLLATE '.\autosystem\DBConn::getCOLLATE().' NOT NULL,'.
       '`imputdate` date NOT NULL,'.
      ' PRIMARY KEY ( `id` )'.
     ')ENGINE = InnoDB DEFAULT CHARSET='.\autosystem\DBConn::getCHARSET().' COLLATE='.\autosystem\DBConn::getCOLLATE().' ;';
     $result3 = $this->DB->query($sql);
     $return3 = $result3->rowCount()==1;
    }else{
     $return3 = true;
    }

    if(!$this->existTableProduct()){
     $sql = 'CREATE TABLE IF NOT EXISTS `'.\autosystem\DBConn::getTableNameFor('catalog_product').'`('.
       '`id` INT (10) NOT NULL AUTO_INCREMENT,'.
       '`name` VARCHAR (230) COLLATE '.\autosystem\DBConn::getCOLLATE().' NOT NULL,'.
       '`shortcomment` VARCHAR (1000) COLLATE '.\autosystem\DBConn::getCOLLATE().' NOT NULL,'.
       '`detail` VARCHAR (3000) COLLATE '.\autosystem\DBConn::getCOLLATE().' NOT NULL,'.
       '`accound_id` INT (20),'.
       '`category_id` INT (4),'.
       '`ENABLED` ENUM(\'TRUE\',\'FALSE\')NOT NULL DEFAULT \'FALSE\','.
       '`PROMOTE` ENUM(\'TRUE\',\'FALSE\')NOT NULL DEFAULT \'FALSE\','.
       '`imputdate` timestamp NOT NULL,'.
       '`ip` VARCHAR (50) COLLATE '.\autosystem\DBConn::getCOLLATE().' NOT NULL,'.
       '`stars` INT (4),'.
      ' PRIMARY KEY ( `id` )'.
     ')ENGINE = InnoDB DEFAULT CHARSET='.\autosystem\DBConn::getCHARSET().' COLLATE='.\autosystem\DBConn::getCOLLATE().' ;';
     $result4 = $this->DB->query($sql);
     $return4 = $result4->rowCount()==1;
    }else{
     $return4 = true;
    }

    if(!$this->existTableProductPrice()){
     $sql = 'CREATE TABLE IF NOT EXISTS `'.\autosystem\DBConn::getTableNameFor('catalog_product_price').'`('.
       '`id` INT (15) NOT NULL AUTO_INCREMENT,'.
       '`product_id` INT (10),'.
       '`price` ENUM(\'TRUE\',\'FALSE\')NOT NULL DEFAULT \'FALSE\','.
       '`ENABLED` ENUM(\'TRUE\',\'FALSE\')NOT NULL DEFAULT \'FALSE\','.
       '`imputdate` timestamp NOT NULL,'.
       '`ENABLEDBYTENDER` ENUM(\'TRUE\',\'FALSE\')NOT NULL DEFAULT \'FALSE\','.
      ' PRIMARY KEY ( `id` )'.
     ')ENGINE = InnoDB DEFAULT CHARSET='.\autosystem\DBConn::getCHARSET().' COLLATE='.\autosystem\DBConn::getCOLLATE().' ;';
     $result5 = $this->DB->query($sql);
     $return5 = $result5->rowCount()==1;
    }else{
     $return5 = true;
    }

    if(!$this->existTableProductFavorites()){
     $sql = 'CREATE TABLE IF NOT EXISTS `'.\autosystem\DBConn::getTableNameFor('catalog_product_favorites').'`('.
       '`product_id` INT (10),'.
       '`accound_id` INT (20),'.
      ' PRIMARY KEY ( `product_id` , `accound_id` )'.
     ')ENGINE = InnoDB DEFAULT CHARSET='.\autosystem\DBConn::getCHARSET().' COLLATE='.\autosystem\DBConn::getCOLLATE().' ;';
     $result6 = $this->DB->query($sql);
     $return6 = $result6->rowCount()==1;
    }else{
     $return6 = true;
    }

    if(!$this->existTableProductSupplier()){
     $sql = 'CREATE TABLE IF NOT EXISTS `'.\autosystem\DBConn::getTableNameFor('catalog_product_supplier').'`('.
       '`product_id` INT (10),'.
      ' PRIMARY KEY ( `product_id` )'.
     ')ENGINE = InnoDB DEFAULT CHARSET='.\autosystem\DBConn::getCHARSET().' COLLATE='.\autosystem\DBConn::getCOLLATE().' ;';
     $result7 = $this->DB->query($sql);
     $return7 = $result7->rowCount()==1;
    }else{
     $return7 = true;
    }

    if(!$this->existTableProductSuppliers()){
     $sql = 'CREATE TABLE IF NOT EXISTS `'.\autosystem\DBConn::getTableNameFor('catalog_product_suppliers').'`('.
       '`id` INT (6) NOT NULL AUTO_INCREMENT,'.
       '`name` VARCHAR (100) COLLATE '.\autosystem\DBConn::getCOLLATE().' NOT NULL,'.
       '`accound_id` INT (20),'.
       '`suppliertype` INT (4),'.
      ' PRIMARY KEY ( `id` )'.
     ')ENGINE = InnoDB DEFAULT CHARSET='.\autosystem\DBConn::getCHARSET().' COLLATE='.\autosystem\DBConn::getCOLLATE().' ;';
     $result8 = $this->DB->query($sql);
     $return8 = $result8->rowCount()==1;
    }else{
     $return8 = true;
    }

    if(!$this->existTableProductSuppliersType()){
     $sql = 'CREATE TABLE IF NOT EXISTS `'.\autosystem\DBConn::getTableNameFor('catalog_product_suppliers_type').'`('.
       '`id` INT (4) NOT NULL AUTO_INCREMENT,'.
       '`label` VARCHAR (100) COLLATE '.\autosystem\DBConn::getCOLLATE().' NOT NULL,'.
       '`priority` INT (3),'.
       '`QUALITYRANK` INT (3),'.
       '`TYPE` INT (10),'.
      ' PRIMARY KEY ( `id` )'.
     ')ENGINE = InnoDB DEFAULT CHARSET='.\autosystem\DBConn::getCHARSET().' COLLATE='.\autosystem\DBConn::getCOLLATE().' ;';
     $result9 = $this->DB->query($sql);
     $return9 = $result9->rowCount()==1;
    }else{
     $return9 = true;
    }

    if(!$this->existTableProductParameter()){
     $sql = 'CREATE TABLE IF NOT EXISTS `'.\autosystem\DBConn::getTableNameFor('catalog_product_parameter').'`('.
       '`id` INT (4),'.
       '`keyname` VARCHAR (230) COLLATE '.\autosystem\DBConn::getCOLLATE().' NOT NULL,'.
       '`detail` VARCHAR (230) COLLATE '.\autosystem\DBConn::getCOLLATE().' NOT NULL,'.
       '`PUBLIC` ENUM(\'TRUE\',\'FALSE\')NOT NULL DEFAULT \'FALSE\','.
      ' PRIMARY KEY ( `id` )'.
     ')ENGINE = InnoDB DEFAULT CHARSET='.\autosystem\DBConn::getCHARSET().' COLLATE='.\autosystem\DBConn::getCOLLATE().' ;';
     $result10 = $this->DB->query($sql);
     $return10 = $result10->rowCount()==1;
    }else{
     $return10 = true;
    }

    if(!$this->existTableProductParameterValue()){
     $sql = 'CREATE TABLE IF NOT EXISTS `'.\autosystem\DBConn::getTableNameFor('catalog_product_parameter_value').'`('.
       '`product_id` INT (10),'.
       '`parameter_id` INT (4),'.
       '`value` VARCHAR (100) COLLATE '.\autosystem\DBConn::getCOLLATE().' NOT NULL,'.
       '`ENABLED` ENUM(\'TRUE\',\'FALSE\')NOT NULL DEFAULT \'FALSE\','.
       '`VISIBLE` ENUM(\'TRUE\',\'FALSE\')NOT NULL DEFAULT \'FALSE\','.
      ' PRIMARY KEY ( `product_id` , `parameter_id` )'.
     ')ENGINE = InnoDB DEFAULT CHARSET='.\autosystem\DBConn::getCHARSET().' COLLATE='.\autosystem\DBConn::getCOLLATE().' ;';
     $result11 = $this->DB->query($sql);
     $return11 = $result11->rowCount()==1;
    }else{
     $return11 = true;
    }

    if(!$this->existTableProductGroup()){
     $sql = 'CREATE TABLE IF NOT EXISTS `'.\autosystem\DBConn::getTableNameFor('catalog_product_group').'`('.
       '`group_id` INT (15),'.
       '`product_id` INT (10),'.
      ' PRIMARY KEY ( `product_id`,`group_id` )'.
     ')ENGINE = InnoDB DEFAULT CHARSET='.\autosystem\DBConn::getCHARSET().' COLLATE='.\autosystem\DBConn::getCOLLATE().' ;';
     $result12 = $this->DB->query($sql);
     $return12 = $result12->rowCount()==1;
    }else{
     $return12 = true;
    }

    if(!$this->existTableProductGroups()){
     $sql = 'CREATE TABLE IF NOT EXISTS `'.\autosystem\DBConn::getTableNameFor('catalog_product_parameters').'`('.
       '`id` INT (15) NOT NULL AUTO_INCREMENT,'.
       '`parameter_id` INT (4),'.
       '`group_id` INT (15),'.
       '`info` VARCHAR (500) COLLATE '.\autosystem\DBConn::getCOLLATE().' NOT NULL,'.
      ' PRIMARY KEY ( `id` )'.
     ')ENGINE = InnoDB DEFAULT CHARSET='.\autosystem\DBConn::getCHARSET().' COLLATE='.\autosystem\DBConn::getCOLLATE().' ;';
     $result13 = $this->DB->query($sql);
     $return13 = $result13->rowCount()==1;
    }else{
     $return13 = true;
    }

    if(!$this->existTableProductStorage()){
     $sql = 'CREATE TABLE IF NOT EXISTS `'.\autosystem\DBConn::getTableNameFor('catalog_product_storage').'`('.
       '`id` INT (15) NOT NULL AUTO_INCREMENT,'.
       '`location_id` INT (4),'.
       '`product_id` INT (10),'.
       '`warehouse_id` INT (10),'.
       '`x` INT (15),'.
       '`y` INT (15),'.
       '`z` INT (15),'.
       '`amout` INT (15),'.
       '`storage_identifier` VARCHAR (500) COLLATE '.\autosystem\DBConn::getCOLLATE().' NOT NULL,'.
      ' PRIMARY KEY ( `id` )'.
     ')ENGINE = InnoDB DEFAULT CHARSET='.\autosystem\DBConn::getCHARSET().' COLLATE='.\autosystem\DBConn::getCOLLATE().' ;';
     $result14 = $this->DB->query($sql);
     $return14 = $result14->rowCount()==1;
    }else{
     $return14 = true;
    }

    if(!$this->existTableProductRisk()){
     $sql = 'CREATE TABLE IF NOT EXISTS `'.\autosystem\DBConn::getTableNameFor('catalog_product_risk').'`('.
       '`id` INT (10) NOT NULL AUTO_INCREMENT,'.
       '`product_id` INT (10),'.
       '`risklevel` INT (4),'.
       '`risk_id` INT (10),'.
       '`safty_accound_id` INT (20),'.
       '`TYPE` VARCHAR (50) COLLATE '.\autosystem\DBConn::getCOLLATE().' NOT NULL,'.
       '`peridiameter` INT (10),'.
       '`risk_info` TEXT NOT NULL,'.
      ' PRIMARY KEY ( `id` )'.
     ')ENGINE = InnoDB DEFAULT CHARSET='.\autosystem\DBConn::getCHARSET().' COLLATE='.\autosystem\DBConn::getCOLLATE().' ;';
     $result15 = $this->DB->query($sql);
     $return15 = $result15->rowCount()==1;
    }else{
     $return15 = true;
    }

    if(!$this->existTableProductUseterm()){
     $sql = 'CREATE TABLE IF NOT EXISTS `'.\autosystem\DBConn::getTableNameFor('catalog_product_useterm').'`('.
       '`id` INT (15) NOT NULL AUTO_INCREMENT,'.
       '`product_id` INT (10),'.
       '`bestbefore` date NOT NULL,'.
       '`sellbefore` date NOT NULL,'.
       '`LOTNR` INT (9),'.
       '`REMOVE` ENUM(\'TRUE\',\'FALSE\')NOT NULL DEFAULT \'FALSE\','.
       '`comment` VARCHAR (5000) COLLATE '.\autosystem\DBConn::getCOLLATE().' NOT NULL,'.
      ' PRIMARY KEY ( `id` )'.
     ')ENGINE = InnoDB DEFAULT CHARSET='.\autosystem\DBConn::getCHARSET().' COLLATE='.\autosystem\DBConn::getCOLLATE().' ;';
     $result16 = $this->DB->query($sql);
     $return16 = $result16->rowCount()==1;
    }else{
     $return16 = true;
    }

    if(!$this->existTableProductBackorder()){
     $sql = 'CREATE TABLE IF NOT EXISTS `'.\autosystem\DBConn::getTableNameFor('catalog_product_backorder').'`('.
       '`id` INT (15) NOT NULL AUTO_INCREMENT,'.
       '`product_id` INT (10),'.
       '`amount` INT (10),'.
       '`MAXBACK` INT (10),'.
       '`comment` VARCHAR (5000) COLLATE '.\autosystem\DBConn::getCOLLATE().' NOT NULL,'.
      ' PRIMARY KEY ( `id` )'.
     ')ENGINE = InnoDB DEFAULT CHARSET='.\autosystem\DBConn::getCHARSET().' COLLATE='.\autosystem\DBConn::getCOLLATE().' ;';
     $result17 = $this->DB->query($sql);
     $return17 = $result17->rowCount()==1;
    }else{
     $return17 = true;
    }

    if(!$this->existTableProductDocuments()){
     $sql = 'CREATE TABLE IF NOT EXISTS `'.\autosystem\DBConn::getTableNameFor('catalog_product_documents').'`('.
       '`id` INT (15)NOT NULL AUTO_INCREMENT,'.
       '`product_id` INT (10),'.
       '`upload_id` INT (10),'.
       '`image_id` INT (9),'.
       '`PUBLIC` ENUM(\'TRUE\',\'FALSE\')NOT NULL DEFAULT \'FALSE\','.
       '`TYPE` VARCHAR (50) COLLATE '.\autosystem\DBConn::getCOLLATE().' NOT NULL,'.
       '`comment` VARCHAR (5000) COLLATE '.\autosystem\DBConn::getCOLLATE().' NOT NULL,'.
      ' PRIMARY KEY ( `id` )'.
     ')ENGINE = InnoDB DEFAULT CHARSET='.\autosystem\DBConn::getCHARSET().' COLLATE='.\autosystem\DBConn::getCOLLATE().' ;';
     $result18 = $this->DB->query($sql);
     $return18 = $result18->rowCount()==1;
    }else{
     $return18 = true;
    }
    if(!$this->existTableProductGroups()){
     $sql = 'CREATE TABLE IF NOT EXISTS `'.\autosystem\DBConn::getTableNameFor('catalog_product_groups').'`('.
       '`id` INT (15)NOT NULL AUTO_INCREMENT,'.
       '`name` VARCHAR (10),'.
       '`groupinfo` TEXT,'.
      ' PRIMARY KEY (`id` )'.
     ')ENGINE = InnoDB DEFAULT CHARSET='.\autosystem\DBConn::getCHARSET().' COLLATE='.\autosystem\DBConn::getCOLLATE().' ;';
     $result19 = $this->DB->query($sql);
     $return19 = $result19->rowCount()==1;
    }else{
     $return19 = true;
    }
//* isset($this->DB)&&$lnb'*/ 
  return $return1  && $return2  && $return3  && $return4  && $return5  && $return6  && $return7  && $return8  && $return9  && $return10  && $return11  && $return12  && $return13  && $return14  && $return15  && $return16  && $return17  && $return18 && $return19 ; 
  }
    public function isInstalled(){
    return $this->isinstalled|| $this->existTableProductWorkspaceTask()
	&& $this->existTableProductCategory()
	&& $this->existTableProductVat()
	&& $this->existTableProduct()
	&& $this->existTableProductPrice()
	&& $this->existTableProductFavorites()
	&& $this->existTableProductSupplier()
	&& $this->existTableProductSuppliers()
	&& $this->existTableProductSuppliersType()
	&& $this->existTableProductParameter()
	&& $this->existTableProductParameterValue()
	&& $this->existTableProductGroup()
	&& $this->existTableProductGroups()
	&& $this->existTableProductParameters()
	&& $this->existTableProductStorage()
	&& $this->existTableProductRisk()
	&& $this->existTableProductUseterm()
	&& $this->existTableProductBackorder()
	&& $this->existTableProductDocuments()
	&& $this->isinstalled=true;
    }
 /*
 * options 
*/
    private function GET_catalog_product_categorySQL($accountid,$id){
       $cpc= \autosystem\DBConn::getTableNameFor('catalog_product_category');
       $sql='SELECT `'.$cpc.'`.* FROM `'.$cpc.'` ';
//' IF(`'.$a.'`.`ENABLED`='TRUE',' checked=checked ',' ') AS `isVisable`,
        $where = '';
        $link ='WHERE';
        if(!empty($accountid)){// TODO: field not exist
            $where.= $link.' `???`.`accound_id`=\''.$accountid.'\';';//TODO check it out
        }
        if(!empty($id)){// TODO: field not exist
            $where.= $link.' `???`.`accound_id`=\''.$id.'\';';//TODO check it out
        }
        $sql.= $where;
        $this->paginator->setSort('  ');//TODO ORDER BY `???`.`inputDate` DESC
        $this->paginator->setRange(20);
        return $sql;
    }
    public function GET_catalog_product_categorysSummary($location,$templatename,$view,$accountid,$id){// TODO: check params
        $sql= $this->GET_catalog_product_categorySQL($accound_id,$accountid,$id);
        $len =$this->GET_COUNT($this->get_catalog_product_categoryCountSQL($accound_id,$accountid,$id));
        $this->paginator->setlistlength($len);
        $this->paginator->setpagenr($pagenr);
        $this->paginator->setpaginatorreload($location,$templatename);
        // TODO: check next line and uncomment
        //parent::GET_Summary($location,$templatename,$view,$sql,null,true,$accound_id);
        $view->getpaginator($this->paginator->getdata());
        print 'NOT IMPLEMENTED';
    }
    private function get_catalog_product_categoryCountSQL($accound_id,$accountid,$id){
       $cpc= \autosystem\DBConn::getTableNameFor('catalog_product_category');
       $sql='SELECT COUNT(*) AS `count`FROM `'.$cpc.'` ';
       $sql.= $where;
       return $sql;
    }
    private function GET_catalog_product_vatSQL($accountid,$id){
       $cpv= \autosystem\DBConn::getTableNameFor('catalog_product_vat');
       $sql='SELECT `'.$cpv.'`.* FROM `'.$cpv.'` ';
//' IF(`'.$a.'`.`ENABLED`='TRUE',' checked=checked ',' ') AS `isVisable`,
        $where = '';
        $link ='WHERE';
        if(!empty($accountid)){// TODO: field not exist
            $where.= $link.' `???`.`accound_id`=\''.$accountid.'\';';//TODO check it out
        }
        if(!empty($id)){// TODO: field not exist
            $where.= $link.' `???`.`accound_id`=\''.$id.'\';';//TODO check it out
        }
        $sql.= $where;
        return $sql;
    }
    public function GET_catalog_product_vatsSummary($location,$templatename,$view,$accountid,$id){// TODO: check params
        $sql= $this->GET_catalog_product_vatSQL($accound_id,$accountid,$id);
    }

//no options

//no options

//no options
    private function GET_SQL($account_id,$id){
       $cpwt= \autosystem\DBConn::getTableNameFor('catalog_product_workspace_task');
       $cpc= \autosystem\DBConn::getTableNameFor('catalog_product_category');
       $cpv= \autosystem\DBConn::getTableNameFor('catalog_product_vat');
       $cp= \autosystem\DBConn::getTableNameFor('catalog_product');
       $cpp= \autosystem\DBConn::getTableNameFor('catalog_product_price');
       $cpf= \autosystem\DBConn::getTableNameFor('catalog_product_favorites');
       $cps= \autosystem\DBConn::getTableNameFor('catalog_product_supplier');
       $cps= \autosystem\DBConn::getTableNameFor('catalog_product_suppliers');
       $cpst= \autosystem\DBConn::getTableNameFor('catalog_product_suppliers_type');
       $cpp= \autosystem\DBConn::getTableNameFor('catalog_product_parameter');
       $cppv= \autosystem\DBConn::getTableNameFor('catalog_product_parameter_value');
       $cpg= \autosystem\DBConn::getTableNameFor('catalog_product_group');
       $cpg= \autosystem\DBConn::getTableNameFor('catalog_product_parameters');
       $cps= \autosystem\DBConn::getTableNameFor('catalog_product_storage');
       $cpr= \autosystem\DBConn::getTableNameFor('catalog_product_risk');
       $cpu= \autosystem\DBConn::getTableNameFor('catalog_product_useterm');
       $cpb= \autosystem\DBConn::getTableNameFor('catalog_product_backorder');
       $cpd= \autosystem\DBConn::getTableNameFor('catalog_product_documents');
       $sql='SELECT `'.$cpd.'`.* ,`'.$cpb.'`.* ,`'.$cpu.'`.* ,`'.$cpr.'`.* ,`'.$cps.'`.* ,`'.$cpg.'`.* ,`'.$cpg.'`.* ,`'.$cppv.'`.* ,`'.$cpp.'`.* ,`'.$cpst.'`.* ,`'.$cps.'`.* ,`'.$cps.'`.* ,`'.$cpf.'`.* ,`'.$cpp.'`.* ,`'.$cp.'`.* ,`'.$cpv.'`.* ,`'.$cpc.'`.* ,`'.$cpwt.'`.* FROM `'.$cpwt.'` LEFT JOIN `'.$cpc.'`.ON .`'.$cpwt.'`.`id=`'.$cpc.'`.`catalog_product_category_id`LEFT JOIN `'.$cpv.'`.ON .`'.$cpwt.'`.`id=`'.$cpv.'`.`catalog_product_vat_id`LEFT JOIN `'.$cp.'`.ON .`'.$cpwt.'`.`id=`'.$cp.'`.`catalog_product_id`LEFT JOIN `'.$cpp.'`.ON .`'.$cpwt.'`.`id=`'.$cpp.'`.`catalog_product_price_id`LEFT JOIN `'.$cpf.'`.ON .`'.$cpwt.'`.`id=`'.$cpf.'`.`catalog_product_favorites_id`LEFT JOIN `'.$cps.'`.ON .`'.$cpwt.'`.`id=`'.$cps.'`.`catalog_product_supplier_id`LEFT JOIN `'.$cps.'`.ON .`'.$cpwt.'`.`id=`'.$cps.'`.`catalog_product_suppliers_id`LEFT JOIN `'.$cpst.'`.ON .`'.$cpwt.'`.`id=`'.$cpst.'`.`catalog_product_suppliers_type_id`LEFT JOIN `'.$cpp.'`.ON .`'.$cpwt.'`.`id=`'.$cpp.'`.`catalog_product_parameter_id`LEFT JOIN `'.$cppv.'`.ON .`'.$cpwt.'`.`id=`'.$cppv.'`.`catalog_product_parameter_value_id`LEFT JOIN `'.$cpg.'`.ON .`'.$cpwt.'`.`id=`'.$cpg.'`.`catalog_product_group_id`LEFT JOIN `'.$cpg.'`.ON .`'.$cpwt.'`.`id=`'.$cpg.'`.`catalog_product_parameters_id`LEFT JOIN `'.$cps.'`.ON .`'.$cpwt.'`.`id=`'.$cps.'`.`catalog_product_storage_id`LEFT JOIN `'.$cpr.'`.ON .`'.$cpwt.'`.`id=`'.$cpr.'`.`catalog_product_risk_id`LEFT JOIN `'.$cpu.'`.ON .`'.$cpwt.'`.`id=`'.$cpu.'`.`catalog_product_useterm_id`LEFT JOIN `'.$cpb.'`.ON .`'.$cpwt.'`.`id=`'.$cpb.'`.`catalog_product_backorder_id`LEFT JOIN `'.$cpd.'`.ON .`'.$cpwt.'`.`id=`'.$cpd.'`.`catalog_product_documents_id`';
//' IF(`'.$a.'`.`ENABLED`='TRUE',' checked=checked ',' ') AS `isVisable`,
        $where = '';
        $link ='WHERE';
        if(!empty($account_id)){// TODO: field not exist
            $where.= $link.' `???`.`accound_id`=\''.$account_id.'\';';//TODO check it out
        }
        if(!empty($id)){// TODO: field not exist
            $where.= $link.' `???`.`accound_id`=\''.$id.'\';';//TODO check it out
        }
        $sql.= $where;
        $this->paginator->setSort('  ');//TODO ORDER BY `???`.`inputDate` DESC
        $this->paginator->setRange(20);
        return $sql;
    }
    private function Promo_SQL(){
    	$p= \autosystem\DBConn::getTableNameFor('catalog_product');
    	$sql='SELECT *'."FROM `$p` WHERE `promote`='TRUE' ";
        //print $sql;
        return $sql;
    }
    public function GET_PromoSummary($location,$templatename,$view,$account_id,$imgprefix = 'product_'){// TODO: check params
        $sql= $this->Promo_SQL();
        
        parent::GET_Summary($location,$templatename,$view,$sql,null,true,$account_id,$imgprefix);
    }
    public function GET_ProductsSummary($location,$templatename,$view,$account_id,$id){// TODO: check params
        $sql= $this->GET_SQL($accound_id,$account_id,$id);
        $len =$this->GET_COUNT($this->get_CountSQL($accound_id,$account_id,$id));
        $this->paginator->setlistlength($len);
        $this->paginator->setpagenr($pagenr);
        $this->paginator->setpaginatorreload($location,$templatename);
        // TODO: check next line and uncomment
        //parent::GET_Summary($location,$templatename,$view,$sql,null,true,$accound_id);
        $view->getpaginator($this->paginator->getdata());
        print 'NOT IMPLEMENTED';
    }
    private function GET_groupSQL(){
       $g= \autosystem\DBConn::getTableNameFor('catalog_product_groups');
       $sql='SELECT `'.$g.'`.* FROM `'.$g.'` ';
       return $sql;
    }
    public function GET_groups($location,$templatename,$view){
        $sql= $this->GET_groupSQL();
        $this->GET_Summary($location,$templatename,$view,$sql);
    }
    private function get_CountSQL($accound_id,$account_id,$id){
       $cpwt= \autosystem\DBConn::getTableNameFor('catalog_product_workspace_task');
       $cpc= \autosystem\DBConn::getTableNameFor('catalog_product_category');
       $cpv= \autosystem\DBConn::getTableNameFor('catalog_product_vat');
       $cp= \autosystem\DBConn::getTableNameFor('catalog_product');
       $cpp= \autosystem\DBConn::getTableNameFor('catalog_product_price');
       $cpf= \autosystem\DBConn::getTableNameFor('catalog_product_favorites');
       $cps= \autosystem\DBConn::getTableNameFor('catalog_product_supplier');
       $cps= \autosystem\DBConn::getTableNameFor('catalog_product_suppliers');
       $cpst= \autosystem\DBConn::getTableNameFor('catalog_product_suppliers_type');
       $cpp= \autosystem\DBConn::getTableNameFor('catalog_product_parameter');
       $cppv= \autosystem\DBConn::getTableNameFor('catalog_product_parameter_value');
       $cpg= \autosystem\DBConn::getTableNameFor('catalog_product_group');
       $cpg= \autosystem\DBConn::getTableNameFor('catalog_product_parameters');
       $cps= \autosystem\DBConn::getTableNameFor('catalog_product_storage');
       $cpr= \autosystem\DBConn::getTableNameFor('catalog_product_risk');
       $cpu= \autosystem\DBConn::getTableNameFor('catalog_product_useterm');
       $cpb= \autosystem\DBConn::getTableNameFor('catalog_product_backorder');
       $cpd= \autosystem\DBConn::getTableNameFor('catalog_product_documents');
       $sql='SELECT COUNT(*) AS `count`FROM `'.$cpwt.'` LEFT JOIN `'.$cpc.'`.ON .`'.$cpwt.'`.`id=`'.$cpc.'`.`catalog_product_category_id`LEFT JOIN `'.$cpv.'`.ON .`'.$cpwt.'`.`id=`'.$cpv.'`.`catalog_product_vat_id`LEFT JOIN `'.$cp.'`.ON .`'.$cpwt.'`.`id=`'.$cp.'`.`catalog_product_id`LEFT JOIN `'.$cpp.'`.ON .`'.$cpwt.'`.`id=`'.$cpp.'`.`catalog_product_price_id`LEFT JOIN `'.$cpf.'`.ON .`'.$cpwt.'`.`id=`'.$cpf.'`.`catalog_product_favorites_id`LEFT JOIN `'.$cps.'`.ON .`'.$cpwt.'`.`id=`'.$cps.'`.`catalog_product_supplier_id`LEFT JOIN `'.$cps.'`.ON .`'.$cpwt.'`.`id=`'.$cps.'`.`catalog_product_suppliers_id`LEFT JOIN `'.$cpst.'`.ON .`'.$cpwt.'`.`id=`'.$cpst.'`.`catalog_product_suppliers_type_id`LEFT JOIN `'.$cpp.'`.ON .`'.$cpwt.'`.`id=`'.$cpp.'`.`catalog_product_parameter_id`LEFT JOIN `'.$cppv.'`.ON .`'.$cpwt.'`.`id=`'.$cppv.'`.`catalog_product_parameter_value_id`LEFT JOIN `'.$cpg.'`.ON .`'.$cpwt.'`.`id=`'.$cpg.'`.`catalog_product_group_id`LEFT JOIN `'.$cpg.'`.ON .`'.$cpwt.'`.`id=`'.$cpg.'`.`catalog_product_parameters_id`LEFT JOIN `'.$cps.'`.ON .`'.$cpwt.'`.`id=`'.$cps.'`.`catalog_product_storage_id`LEFT JOIN `'.$cpr.'`.ON .`'.$cpwt.'`.`id=`'.$cpr.'`.`catalog_product_risk_id`LEFT JOIN `'.$cpu.'`.ON .`'.$cpwt.'`.`id=`'.$cpu.'`.`catalog_product_useterm_id`LEFT JOIN `'.$cpb.'`.ON .`'.$cpwt.'`.`id=`'.$cpb.'`.`catalog_product_backorder_id`LEFT JOIN `'.$cpd.'`.ON .`'.$cpwt.'`.`id=`'.$cpd.'`.`catalog_product_documents_id`';
       $sql.= $where;
       return $sql;
    }
  public function existTableProductWorkspaceTask(){     return $this->existTableLabel('catalog_product_workspace_task');   }
  public function existTableProductCategory(){     return $this->existTableLabel('catalog_product_category');   }
  public function existTableProductVat(){     return $this->existTableLabel('catalog_product_vat');   }
  public function existTableProduct(){     return $this->existTableLabel('catalog_product');   }
  public function existTableProductPrice(){     return $this->existTableLabel('catalog_product_price');   }
  public function existTableProductFavorites(){     return $this->existTableLabel('catalog_product_favorites');   }
  public function existTableProductSupplier(){     return $this->existTableLabel('catalog_product_supplier');   }
  public function existTableProductSuppliers(){     return $this->existTableLabel('catalog_product_suppliers');   }
  public function existTableProductSuppliersType(){     return $this->existTableLabel('catalog_product_suppliers_type');   }
  public function existTableProductParameter(){     return $this->existTableLabel('catalog_product_parameter');   }
  public function existTableProductParameterValue(){     return $this->existTableLabel('catalog_product_parameter_value');   }
  public function existTableProductGroup(){     return $this->existTableLabel('catalog_product_group');   }
  public function existTableProductGroups(){     return $this->existTableLabel('catalog_product_groups');   }
  public function existTableProductParameters(){     return $this->existTableLabel('catalog_product_parameters');   }
  public function existTableProductStorage(){     return $this->existTableLabel('catalog_product_storage');   }
  public function existTableProductRisk(){     return $this->existTableLabel('catalog_product_risk');   }
  public function existTableProductUseterm(){     return $this->existTableLabel('catalog_product_useterm');   }
  public function existTableProductBackorder(){     return $this->existTableLabel('catalog_product_backorder');   }
  public function existTableProductDocuments(){     return $this->existTableLabel('catalog_product_documents');   }
  
  public function getModelInfo(){  return 'catalog.Product.model.info';  }
  public function __upgrade($version){ return true;  }
  public function getVersionNr(){  return (float)5.4;  }
  public function getRuleDefault(){return 'PUBLIC';} //'PUBLIC','FINANCIAL','TRANSLATE','MANAGE','ADMINISTRATION','KING','USER','MODERATOR','CHAIRMAN' }
}
?>