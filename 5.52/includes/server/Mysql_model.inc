<?php
/**
* @author Lieven Roegiers
* @copyright 
* @CMS autosite
*/
namespace server;
class Mysql_model extends \autosystem\Model{
    protected $isinstalled;
    public function __install(){         return true;    }
    public function isInstalled(){       return $this->isinstalled||  $this->isinstalled=true;    }
    public function getModelInfo(){      return 'server.mysql.model.info';    }
    public function __upgrade($version){ return true;    }
    public function getVersionNr(){      return (float)5.4;    }
    public function getRuleDefault(){    return 'KING';    }
    public function getwhoisip(){        return $this->isp;    }
    public function GET_ITEM($location,$templatename,$view,$id,$accound_id=''){
        $t = '`mysql`.`'.\autosystem\DBConn::getTableNameFor('help_topic').'`';
        $sql='SELECT `'.$t.'`.* FROM `'.$t.'` WHERE `id`=\''.$id.'\'';
        return parent::GET($location,$templatename,$view,$sql,null,true,$accound_id);
    }
    protected function GET_SQL($category_id,$accound_id,$enabled,$idlist=null){
        $t = '`mysql`.`'.\autosystem\DBConn::getTableNameFor('help_topic').'`';
        $c = '`mysql`.`'.\autosystem\DBConn::getTableNameFor('help_category').'`';
        $r = '`mysql`.`'.\autosystem\DBConn::getTableNameFor('help_relation').'`';
        $sql='SELECT '.$c.'.name AS categoryname , `parent_category`.name AS parentcategoryname ,'.$t.'.*'.
        ' FROM '.$t.' LEFT JOIN '.$c.' ON '.$c.'.`help_category_id`='.$t.'.`help_category_id` '.
        'LEFT JOIN '.$c.' AS `parent_category` ON '.$c.'.`help_category_id`=`parent_category`.`parent_category_id` '.
        $where = '';
        $link =' WHERE';
        if(!empty($category_id)){
            $where.= $link.' ('.$c.'.`help_category_id`=\''.$category_id.'\' OR `'.$c.'`.`category_id`=\''.$category_id.'\' )';
            $link =' && ';    
        }
        if(!empty($idlist)){
            if(is_array($idlist)){
                $where.= $link.'(';
                $link = '';
                foreach($idlist as $id){
                    if(is_numeric($id)){
                        $where.= $link.'`'.$a.'`.`id`=\''.$id.'\'';
                        $link =' OR ';
                    }
                }
                $where.= ')' ;
                $link =' && ';
            }else{
                $where.= $link.' `'.$a.'`.`id`=\''.$idlist.'\'';
                $link =' && '; 
            }
        }
        $sql.= $where;
        $this->paginator->setSort(' GROUP BY help_topic_id ORDER BY parentcategoryname , name');
        $this->paginator->setRange(13);
        print $sql;
        return $sql;
    }
    public function FIND($location,$name,$view,$term,$in,$accound_id){
        $a = \autosystem\DBConn::getTableNameFor('auction');
        if(is_array($term)|| (strlen($term)<$this->minthermlength)|| strpos($term,'\'')){return false;}
        $sql = 'SELECT `id` AS `key`, `STATUS` AS `value` FROM `'.$a.'`'.' WHERE `'.$in.'` LIKE \'%'.$term.'%\'  AND `ENABLED`=\'TRUE\'';
        $array = parent::GET_Array($sql);
        if(count($array)<1){return false;}
        $sql = $this->GET_SQL('','','',array_keys($array));
        parent::GET_Summary($location,$name,$view,$sql,null,true,$accound_id);
        return true;
    }
    private function get_CountSQL($category_id,$accound_id,$enabled){
        $a = '`mysql`.`help_topic`';
        $c = '`mysql`.`help_category`';
        $sql='SELECT count(*) AS `count` FROM `'.$a.'` LEFT JOIN `'.$c.'` ON  `'.$a.'`.`category_id`= `'.$c.'`.`id`';
        $link =' WHERE';
        $where='';
        if(!empty($category_id)){
            $where.= $link.' (`'.$a.'`.`category_id`=\''.$category_id.'\'';
            $where.= ' OR `'.$c.'`.`category_id`=\''.$category_id.'\'  )' ;
            $link =' && ';    
        }
        if(!empty($accound_id)){
            $where.= $link.' `'.$a.'`.`accound_id`=\''.$accound_id.'\'';
            $link =' && ';
        }
        if(!empty($enabled)&&$enabled==true){
            $where.= $link.' `'.$a.'`.`ENABLED`=\'TRUE\'';
            $link =' && '; 
        }
        $sql.= $where;
        return $sql;
    }
    public function GET_AOverView($location,$templatename,$view,$category_id='',$accound_id='',$enabled = null,$ids='',$pagenr=0){ 
        $sql= $this->GET_SQL($category_id,$accound_id,$enabled,$ids);
        $len =$this->GET_COUNT($this->get_CountSQL($category_id,$accound_id,$enabled));
        $this->paginator->setlistlength($len);
        $this->paginator->setpagenr($pagenr);
        $this->paginator->setpaginatorreload($location,$templatename);
        parent::GET_Summary($location,$templatename,$view,$sql,null,true,$accound_id);
        $view->getpaginator($this->paginator->getdata());
    }
    public function GET_queryresult($database,$query){
        $result =  parent::GET_Array_FULL($query,[]);
        return $result;
    }
    public function GET_PrimaireKey($database,$table){
        $t = '`information_schema`.`tabele_constraints`';
        $k = '`information_schema`.`key_colum_usage`'; 
        $sql = 'SELECT '.$k.'.COLUMN_NAME FROM '.$t;
        $sql .= 'LEFT JOIN  '.$k;
        $sql .= 'USING(constraint_name,table_schema,table_name)';
        $sql .= 'WHERE '.$t.'constraint_type=\'PRIMAIRY KEY\'';
        $sql .= 'AND '.$t.'table_schema=DATABASE()';
        $sql .= 'AND '.$t.'table_name=\''.$table.'\'';
    }
    public function resultToTable($res ,&$view){
        $array['id'] = 'result';
        $array['title'] = 'result';
        print $view->get_template('server','result_head',$array);
        $isfirst = true;
        foreach($res as $row){
           if($isfirst){print '<tr>';}
           foreach($row as $k=>$v){
                if($isfirst){ print '<th>'.$k.'</th>';   }
                $line.= '<td> '.$v.' </td>';
            }
            if($isfirst){print '</tr>';}
            print '<tr>'.$line.'</tr>';
            $line='';
            $isfirst= false;
        }
        print $view->get_template('server','result_foot');
    }
    public function GET_HelpCategorySummary($location,$templatename,$view,$vals,$category_id='',$accound_id=''){
        $c = '`mysql`.`help_category`'; 
        $sql='SELECT * FROM '.$c.' '.((isset($category_id)&&$category_id!='')?' WHERE category_id=\''.$category_id.'\'':'');
        parent::GET_Summary($location,$templatename,$view,$sql);
    }
    public function GET_Categorys($category_id){
       $c = '`mysql`.`help_category`'; 
       $sql='SELECT `help_category_id` AS \'key\' , `name` AS \'value\' FROM '.$c.
       ((isset($category_id))?' WHERE `parent_category_id`=\''.$category_id.'\'':'') ;
       return parent::GET_Array($sql);
    }
    public function GET_Databases($location,$templatename,$view){
        $sql='SHOW DATABASES';
        //$sql='SELECT * FROM information_schema.tables where database where table_schema';
		parent::GET_Summary($location,$templatename,$view,$sql);
    }
    public function GET_Database($dbname,$location,$templatename,$view){
        $sql='SHOW TABLE STATUS FROM `'.$dbname.'`';
        //parent::GET_Summary($location,$templatename,$view,$sql);
        $c= \autosystem\DBConn::getTableNameFor('information_schema');
        $exploreoverwrite['collums']='SELECT * FROM `'.$c.'`.`columns` WHERE (`table_schema`=\''.$dbname.'\' AND `TABLE_NAME`=:Name)';       
        $exploreoverwrite['tools']='SELECT * , COUNT(`TABLE_NAME`) FROM `'.$c.'`.`columns` WHERE (`table_schema`=\''.$dbname.'\' AND `TABLE_NAME`=:Name)';
        parent::GET_ExploreR($location,$templatename,$view,$sql,['collums'=>'Name','tools'=>'Name'],$exploreoverwrite,null,true,$account_id,'');    
    }
    public function GET_Collums($location,$templatename,$view,$dbname,$tabel){
        $c= \autosystem\DBConn::getTableNameFor('information_schema');
        //,IF(`'.$p.'`.`PROMOTE`=\'TRUE\',\' checked=checked \',\' \') AS `ispromote` '.
        $sql='SELECT `columns`.*,IF(`IS_NULLABLE`=\'NO\',\' NOT NULL \',\' \') AS `CANNULL` FROM `'.$c.'`.`columns` WHERE `table_schema`=\''.$dbname.'\' AND `TABLE_NAME`=\''.$tabel.'\'';
        //print $sql;
        parent::GET_ExploreR($location,$templatename,$view,$sql,[],$exploreoverwrite,null,true,$account_id,'');    
    }
    public function GET_CollumsArr($dbname,$tabel){
        $c= \autosystem\DBConn::getTableNameFor('information_schema');
        $sql='SELECT `columns`.*,IF(`IS_NULLABLE`=\'NO\',\' NOT NULL \',\' \') AS `CANNULL` FROM `'.$c.'`.`columns` WHERE `table_schema`=\''.$dbname.'\' AND `TABLE_NAME`=\''.$tabel.'\'';
        return parent::GET_Array($sql);
    }
    public function GET_CollumsQueryArr($dbname,$query,$tableAndColl = true){
        //$c= \autosystem\DBConn::getTableNameFor('information_schema');
        $tables = $this->GET_TablesFromQuery($query);
        $cols=[];
        foreach($tables as $table){
            $sql='SHOW columns  FROM `'.$table.'`';
            //$add =
            $cols = array_merge($cols , parent::GET_Array_FULL($sql));
        }
        return $cols;
    }
    public function GET_TablesFromQuery($query){
          $need =['FROM','JOIN'];
          $array =  explode(' ',$query);
          $on  = false;
          $return = [];
          foreach($array as $key=>$value){
               if($on){
                 array_push($return,$value);
                 $on = false;
               }
               if(in_array($value,$need)){
                    $on = true;
               }
               if($value == 'WHERE'){
                   return $return;
               }
          }
          return $return;
    }
    public function GET_Plist($dbname,$location,$templatename,$view){
        //$sql='SHOW full processlist';SHOW   full processlist
        //$sql='SHOW PROCESSLIST -- ' . uniqid('pdo_mysql_close', 1);
        //parent::GET_Summary($location,$templatename,$view,$sql);
        /*$query = 'SHOW PROCESSLIST -- ' . uniqid('pdo_mysql_close', 1);
        $list  = $connection->query($query)->fetchAll(\PDO::FETCH_ASSOC);
        foreach ($list as $thread) {
            if ($thread['Info'] === $query) {
                if($kill == true){
                    return $connection->query('KILL ' . $thread['Id']);
                }else{
                    return false;
                }
            }
        }
        return false;
        }

        $sql = 'SELECT CONNECTION_ID()';*/
    }
    public function GET_TableExtraction($table){
        $t = '`information_schema`.';
        $c = $t.'`COLUMNS`';
        $sql ='SELECT '.$t.'table_name, '.$t.'COLUMN_NAME , '.$t.'COLUMN_TYPE FROM '.$c.' WHERE '.$t.'TABLE_SCHEMA = \''.$table.'\'';
    }
    public function GET_ConnectionId($dbname,$location,$templatename,$view){
        $sql='SELECT CONNECTION_ID()';
        parent::GET($location,$templatename,$view,$sql);
    }
    //Show Warnings
    //SHOW Triggers
    //SHOW  STATUS
    //SHOW   full processlist
    //SHOW VARIABLES
} 
?>