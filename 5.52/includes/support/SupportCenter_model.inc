<?php
    namespace support;
	class SupportCenter_model extends \support\SupportHome_model{
        public function __install(){
            /*if(!$this->existTablesupport()){
            }else{
                $return2 = true;
            }*/
            return parent::__install();
        }
        public function isInstalled(){      return $this->isinstalled||parent::isInstalled();      }
        public function getModelInfo(){     return 'support.Center.model.info';  }
        public function getVersionNr(){     return (float)5.4;   }
        public function __upgrade($version){return true;        }
        public function save_support($TYPE,$category,$title,$condition,$shortcomment,$detail,$status,$weblocation,$indicationprice,$accound_id,$id,$location_id=0){
            $ip = parent::getIp();
            $ENABLED = 'TRUE';
            $t = \autosystem\DBConn::getTableNameFor('support_tickets');
            $key ='UPDATE `'.$t.'` SET `title`=:title ,`shortcomment`=:shortcomment , `detail`=:detail ,`location_id`=:location_id ,'
                   .' `category_id`=:category_id ,`STATUS`=:STATUS ,`TAG`=:TAG ,`CONDITION`=:CONDITION , '
                   .'`TYPE`=:TYPE ,`weblocation`=:weblocation ,`indicationprice`=:indicationprice WHERE `id`=:id ';
                  
            $value = [':title'=>$title,
            ':shortcomment'=>$shortcomment,':detail'=>$detail,
            ':location_id'=>$location_id,
            ':category_id'=>$category,':STATUS'=>$status,':TAG'=>'',
            ':CONDITION'=>$condition,':TYPE'=>$TYPE,
            ':weblocation'=>$weblocation,
            ':indicationprice'=>$indicationprice,
            ':id'=>$id,
            ];
            $sql[$key]= $value;
            return $this->transaction($sql);
        }
        public function GET_ticket($location,$templatename,$view,$id,$accound_id=null){
            return parent::GET_ticket($location,$templatename,$view,$id,null);
        }
        public function log_accoundHasSeenTicket($ticket_id,$accound_id){  }
        public function GET_TagedOverView($location,$templatename,$view,$filter=null){
            $sql='SELECT * FROM `'.\autosystem\DBConn::getTableNameFor('support_tickets').'` WHERE `TAG`!=" '.((isset($filter)&&$filter!='')? '" AND TAG!='.$filter:'');
            //print $sql;
            parent::GET_Summary($location,$templatename,$view,$sql);
        }
        public function GET_ActiveTicketsSummary($location,$templatename,$view,$filter=null){
            $l= \autosystem\DBConn::getTableNameFor('adres_location');
            $a= \autosystem\DBConn::getTableNameFor('support_tickets');
            $c= \autosystem\DBConn::getTableNameFor('support_category');
            $sql='SELECT `'.$l.'`.* ,`'.$a.'`.* FROM `'.$a.'` LEFT JOIN `'.$l.'` ON `location_id`=`'.$l.'`.`id` '.
            'LEFT JOIN `'.$c.'` ON `'.$a.'`.`category_id`=`'.$c.'`.`id` ORDER BY `inputDate` DESC LIMIT 10';
            print $sql;
            parent::GET_Summary($location,$templatename,$view,$sql);
        }
        public function getRuleDefault(){ return 'ADMINISTRATION';        }
        public function setVisibility($accound_id,$item_id,$bool){
            $ENABLED = ($bool)?'TRUE':'FALSE';
            'UPDATE `'.\autosystem\DBConn::getTableNameFor('support').'` SET `ENABLED`=:ENABLED WHERE `id`=:id';
            $value = [':ENABLED'=>$ENABLED,':id'=>$item_id];      
        }
        public function delete($id){
            $key ='DELETE FROM `'.\autosystem\DBConn::getTableNameFor('support_tickets').'` WHERE `id`=:id';//
            $value = [':id'=>$id];
            $sql[$key]= $value;
            return $this->transaction($sql);
        }
        public function SetItemPriority($ticketid,$priority){
            $st= \autosystem\DBConn::getTableNameFor('support_tickets');
            $key ='UPDATE `'.$st.'` SET `PRIORITY`=:PRIORITY WHERE `'.$st.'`.`id`=:id';
            $sql[$key]= [':PRIORITY'=>$priority,':id'=>$ticketid];
            print \autosystem\DEBUG::printTransaction($sql);
            return is_array($this->transaction($sql));
        }
        public function SetItemStatus($ticketid,$status){
            $st= \autosystem\DBConn::getTableNameFor('support_tickets');
            $key ='UPDATE `'.$st.'` SET `STATUS`=:status WHERE `'.$st.'`.`id`=:id';
            $sql[$key]= [':status'=>$status,':id'=>$ticketid];
            print \autosystem\DEBUG::printTransaction($sql);
            return is_array($this->transaction($sql));
        }
    }
    
?>