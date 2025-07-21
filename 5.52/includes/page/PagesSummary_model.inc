<?php
/* model*/
namespace page;
class PagesSummary_model extends \page\Watch_model{
    public function getModelInfo(){    return 'page.summary.model.info';    }
    public function getVersionNr(){    return (float)5.31;   }
    public function __upgrade($version){
        return true;
    }
    public function getRuleDefault(){  return 'ADMINISTRATION';    }
    public function GET_APageSummary($location,$templatename,$view,$accoundid,$enabled){
        $p = \autosystem\DBConn::getTableNameFor('page');
        $c = \autosystem\DBConn::getTableNameFor('page_category');
        $sql='SELECT `'.$p.'`.*, IF(`VISIBLE`=\'TRUE\',\' checked=checked \',\' \') AS `isvisable`, `'.$p.'`.`id` FROM `'.$p.'` '.
            'LEFT JOIN `'.$c.'` ON `'.$p.'`.`category_id`=`'.$c.'`.`id` ORDER BY `imputDate` ';
        parent::GET_Summary($location,$templatename,$view,$sql);
    }
    public function save($title,$shortcomment,$content,$accound_id,$category,$status,$access ,$locale,$titlekey=null,$VISIBLE=false){
            $ip = parent::getIp();
            $VISIBLE = 'FALSE';
            if(isset($titlekey)){
                $transkey =$titlekey;
            }else{
                $transkey = 'page.title.'.$title;
                $transvalue = $title;
            }
            $p = \autosystem\DBConn::getTableNameFor('page');
            $key ='INSERT INTO `'.$p.'`(`id`,`title.key`,`title`,`shortcomment`,`content`,`accound_id`,`category_id`,`STATUS`,`access`,`VISIBLE`,`page_langlocale`,`imputDate`,`ip`) '.
                      'VALUES(NULL,:titlekey, :title ,:shortcomment ,:content ,:accound_id ,:category_id ,:STATUS ,:access ,:VISIBLE ,:locale,:imputDate ,:ip ) RETURNING `id`';
            $value = [':title'=>$title,
            ':titlekey'=>$transkey,
            ':shortcomment'=>$shortcomment,':content'=>$content,
            ':accound_id'=>$accound_id,':category_id'=>$category,
            ':STATUS'=>$status,':access'=>$access,':VISIBLE'=>$VISIBLE,
            ':imputDate'=>$this->nowDateTime(),':locale'=>$locale,':ip'=>$ip];
            $sql[$key]= $value;
            return $this->transaction($sql);
    }
    public function update( $id,$page_status,$title,$page_access,$shortcomment,$content,$page_category=0){
        $key ='UPDATE `'.\autosystem\DBConn::getTableNameFor('page').'` '
               .'SET `STATUS`=:STATUS, `access`=:access, `title`=:title, `shortcomment`=:shortcomment, `content`=:content, `category_id`=:category_id WHERE `id`=:id ';
        $value = [  ':id'=>$id,':title'=>$title,':STATUS'=>$page_status,
                    ':access'=>$page_access,':shortcomment'=>$shortcomment,
                    ':content'=>$content,':category_id'=>$page_category,
            ];
        $sql[$key]= $value;
        return $this->transaction($sql);
    }
    public function setVisibility($accound_id,$item_id,$bool){
        $VISIBLE = ($bool=='true')?'TRUE':'FALSE';
        $key= 'UPDATE `'.\autosystem\DBConn::getTableNameFor('page').'` SET `VISIBLE`=:VISIBLE WHERE `id`=:id ';
        $value = [':VISIBLE'=>$VISIBLE,':id'=>$item_id];
        $sql[$key]= $value;
        return $this->transaction($sql);   
    }
    public function GET_Categorys(){    return ['NULL'=>'not.availeble'];   }
    public function GETPageArray($id){
        $p = \autosystem\DBConn::getTableNameFor('page');
        $sql='SELECT * FROM `'.$p.'` WHERE `'.$p.'`.`id`=\''.$id.'\'';
        return $this->GET_Stmt($sql,[]);
    }
    public function GETTranslateArray($id,$accound_id,$locale){
        $p = \autosystem\DBConn::getTableNameFor('page');
        $sql='SELECT a.* FROM `'.$p.'` a LEFT JOIN `'.$p.'` b ON a.`title.key`= b.`title.key`'.
            ' WHERE b.`id`=\''.$id.'\' AND a.`title.key`= b.`title.key` AND a.`page_langlocale`=\''.$locale.'\'';
        return $this->GET_Stmt($sql,[]);
    }
    public function existPage($key,$locale){
        if(!$this->isinstalled){return false;}
        $p = \autosystem\DBConn::getTableNameFor('page');
        $sql = 'SELECT id FROM '.$p.' WHERE `title.key`=:key AND `page_langlocale`=:locale ' ;
        $value = [':key'=>$key,':locale'=>$locale];
        return $this->isQuery($sql,$value);
    }
    public function getTitlekey($id){
        if(!$this->isinstalled){return false;}
        $p = \autosystem\DBConn::getTableNameFor('page');
        $sql = 'SELECT `title.key` FROM '.$p.' WHERE `id`=:id' ;
        $value = [':id'=>$id];
        return ($return = $this->GET_Stmt($sql,$value))?$return['title.key']:null;
    }
    public function delete($id){
        $key ='DELETE FROM `'.\autosystem\DBConn::getTableNameFor('page').'` WHERE `id`=:id ';
        $value = [':id'=>$id];
        $sql[$key]= $value;
        return $this->transaction($sql);
    }
}
?>
