<?php
namespace plan;
class Week_model extends \plan\Callendar_model{
    public $types = array('NULL'=>'not availeble','ONEDAY'=>'oneday','DAY'=>'day','WEEKDAY'=>'weekday','ALLDAY'=>'allday','WEEKDAY_SELECTION'=>'weekday','ALLDAY_SELECTION'=>'allday','FREEDAY'=>'freeday');
    public function getModelInfo(){        return 'callendar.week.model.info';    }
    public function __install(){
        $installed = parent::__install(); 
        if(!$this->existTableCallendar_detail()){
            $sql = 'CREATE TABLE IF NOT EXISTS `'.\autosystem\DBConn::getTableNameFor('callendar_detail').'`(
                    `callendar_id` INT(12),
                    `start` TIME COLLATE '.\autosystem\DBConn::getCOLLATE().' NOT NULL,
                    `end` TIME COLLATE '.\autosystem\DBConn::getCOLLATE().' NOT NULL,
                    `TYPE` ENUM(\''.implode('\',\'',array_keys($this->modeltypes)).'\') default \'NULL\',
                PRIMARY KEY (`callendar_id`)
            )ENGINE = InnoDB DEFAULT CHARSET='.\autosystem\DBConn::getCHARSET().' COLLATE='.\autosystem\DBConn::getCOLLATE().' ;';
            $result1 = $this->DB->query($sql);
            $return1 = $result1->rowCount()==1;
        }else{
            $return1 = true;
        }
        return $installed && $return1;
    }
    public function isInstalled(){
        return $this->isinstalled|| parent::isInstalled()&& $this->existTableCallendar_detail()&& $this->isinstalled=true;
    }
    public function existTableCallendar_detail(){     return $this->existTableLabel('callendar_detail');   }
    public function getVersionNr(){        return (float)5.4;   }
    public function __upgrade($version){
        return true;
    }
    public function getRuleDefault(){      return 'USER';       }
    public function GET_WItems($year,$weeknr,$accoundid){
        $ids = $this->GET_ids($year,$weeknr,$accoundid);
        if(isset($ids)){
            $val = 'WHERE id=\''.implode('\' OR id=\'',array_keys($ids)).'\'';
            $c = \autosystem\DBConn::getTableNameFor('callendar');
            $c_d = \autosystem\DBConn::getTableNameFor('callendar_detail');
            $sql='SELECT `'.$c.'`.*, `'.$c_d.'`.*, HOUR(`start`) AS start_h, HOUR(`end`) AS end_h, DAY(`from`) AS from_day, DAY(`to`) AS to_day FROM `'.$c.'` '.
                 'LEFT JOIN `'.$c_d.'` ON `'.$c.'`.`id` = `'.$c_d.'`.`callendar_id` '.$val;
            $data = $this->GET_Stmt_key($sql,[],0,'id');
            $return = [];
            $translatekeys = [];
            foreach($ids as $key=>$val){
                $translatekeys[]= $data[$key]['namekey'];
                for ($daynr = $data[$key]['from_day'], $max = $data[$key]['to_day']+1; $daynr < $max; $daynr++){
                    $return[$daynr][]=$data[$key];
                }
            }
            return $return;
        }else{
            return [];
        }
        return [];
    }
    public function GET_SQL($accound_id,$id){
        $c = \autosystem\DBConn::getTableNameFor('callendar');
        $c_d = \autosystem\DBConn::getTableNameFor('callendar_detail');
         $sql='SELECT `'.$c.'`.* ,`'.$c_d.'`.*, HOUR(`start`) AS start_h ,HOUR(`end`) AS end_h ,YEAR(`from`) AS Y,MONTH(`from`) AS m,DAY(`from`) AS d, DAY(`from`) AS from_day ,DAY(`to`) AS to_day FROM `'.$c.'` '.
              'LEFT JOIN `'.$c_d.'` ON `'.$c.'`.`id`=`'.$c_d.'`.`callendar_id` WHERE id=:id';
              //print($sql);
        return $sql;
    }
    public function GET_Id($location,$templatename,$view,$id,$accound_id){
        $ret = $this->GETArray($accound_id,$id);
        return $this->GET_DayItems($location,$templatename,$view,$accound_id,$ret['Y'],$ret['m'],$ret['d'],$id);
    }
    public function GETArray($accound_id,$id){
        $sql = $this->GET_SQL($accound_id,$id);
        return $this->GET_Stmt($sql,[':id'=>$id]);
    }
    public function GET_DayItems($location,$templatename,$view,$accound_id,$year,$month,$day,$id=''){
        $c = \autosystem\DBConn::getTableNameFor('callendar');
        $c_d = \autosystem\DBConn::getTableNameFor('callendar_detail');
        $em = ((strlen($month)==1)?'0':'').$month;
        $m = $view->get_Atranslation('month.'.$this->monthval[$em],$view->getLocale());
        $sql='SELECT `'.$c.'`.* ,`'.$c_d.'`.* ,\''.$year.'\' AS `Y`,\''.$m.'\' AS `m`,\''.$day.'\' AS `d` FROM `'.$c.'` ';
        $sql.='LEFT JOIN `'.$c_d.'` ON `'.$c.'`.`id` = `'.$c_d.'`.`callendar_id` ';
        if($id!=''){
            $sql.='WHERE ((YEAR(`from`)=:year AND MONTH(`from`)=:month AND DAY(`from`)=:day)';
            $sql.='||(YEAR(`to`)=:year AND MONTH(`to`)=:month AND DAY(`to`)=:day))AND `'.$c.'`.`id`=:id ORDER BY `start`';
            $array= [ ':year'=>$year, ':month'=>$month, ':day'=>$day, ':id'=>$id];
        }else{
            $sql.='WHERE (YEAR(`from`)=:year AND MONTH(`from`)=:month AND DAY(`from`)=:day)';
            $sql.='||(YEAR(`to`)=:year AND MONTH(`to`)=:month AND DAY(`to`)=:day) ORDER BY `start`';
            $array= [ ':year'=>$year, ':month'=>$month, ':day'=>$day];
        }
        //print($sql);
        parent::GET_Summary($location,$templatename,$view,$sql,$array,true,$accound_id);
    }
    private function GET_ids($year,$weeknr,$accoundid){
       $time =  strtotime($year.'W'.str_pad($weeknr, 2, 0, STR_PAD_LEFT));
       $c = \autosystem\DBConn::getTableNameFor('callendar');
       $days = [];
       if(date('d',$time)>22){
            $daysA=[];
            $daysB=[];
            $e=1;
            $monthlen = $this->getMonthLen(date('m',$time),$year)+1;
            for ($d = 0; $d < 7; $d++){
                $t = date('d',$time)+$d;
                if($monthlen >$t){
                    $daysA[]= date('d',$time)+$d;
                }else{
                    $daysB[]=$e++;
                }
            }
            $sql='SELECT `id` AS \'key\', DAY(`from`) AS \'value\' FROM `'.$c.'` WHERE';
            $sql.=' ((YEAR(`from`)=\''.$year.'\' AND '.$this->getMysqlDates('from',date('m',$time),$daysA);
            if(count($daysB)>0){
                $sql.=' || YEAR(`from`)=\''.($year).'\' AND '.$this->getMysqlDates('from',date('m',$time)+1,$daysB);
            }
            $p =(date('m',$time)==12)?1:0;
            $sql.=') || (YEAR(`to`)=\''.($year+$p).'\' AND '.$this->getMysqlDates('to',date('m',$time),$daysA);
            if(count($daysB)>0){
                $sql.=' || YEAR(`to`)=\''.($year+$p).'\' AND '.$this->getMysqlDates('to',date('m',$time)+1,$daysB);
            }
            $sql.=')) AND `accound_owner_id`=\''.$accoundid.'\' ';
       }else{
            for ($d = 0; $d < 7; $d++){
               $days[]= date('d',$time)+$d;
            }
            $sql='SELECT `id` AS \'key\', DAY(`from`) AS \'value\' FROM `'.$c.'` '.
                 'WHERE YEAR(`from`)=\''.$year.'\' AND('.$this->getMysqlDates('from',date('m',$time),$days).
                 'OR'.$this->getMysqlDates('to',date('m',$time),$days).') AND `accound_owner_id`=\''.$accoundid.'\' '; 
       }
       //print $sql;
       return parent::GET_Array($sql);
    }
    private function getMysqlDates($K,$M,$DAYS){
       return '(MONTH(`'.$K.'`)=\''.$M.'\' AND(DAY(`'.$K.'`)=\''.implode('\' OR DAY(`'.$K.'`)=\'',$DAYS).'\'))';
    }
    private function getMonthLen($monthnr,$year){
        if($monthnr%2==$monthnr>7){
            if($monthnr!=2){           return 30;
            }else{                     return ($year%4==0)?29:28; 
            }
        }
        return 31;
    }
    public function save_weekitem($namekey,$from,$start,$to,$end,$accound_owner_id,$TYPE,$category_id=0){
        $c = \autosystem\DBConn::getTableNameFor('callendar');
        $c_d = \autosystem\DBConn::getTableNameFor('callendar_detail');
        $ip = parent::getIp();
        $ENABLED = 'TRUE';
        $key ='INSERT INTO `'.$c.'`(`id`,`namekey`,`from`,`to`,`accound_owner_id`,`ip`,`TYPE`,`category_id`) '.
                            'VALUES(NULL,:key,:from,:to,:accound_owner_id,:ip,:TYPE,:catid) RETURNING `callendar_id`';//
        $sql[$key]= [':key'=>$namekey,':from'=>$from,':to'=>$to,':accound_owner_id'=>$accound_owner_id,
        ':ip'=>$ip,':TYPE'=>$TYPE,':catid'=>$category_id];
         $key ='INSERT INTO `'.$c_d.'`(`callendar_id` ,`start`,`end`,`TYPE`) '.
                  'VALUES(:callendar_id,:start,:end,:TYPE) ';//
        $sql[$key]= [':callendar_id'=>'',':start'=>$start,':end'=>$end,':TYPE'=>$TYPE];
        print_r($sql);
        return $this->transaction($sql);
    }
    public function delete($id,$accound_id){
        $ip = parent::getIp();
        $ENABLED = 'TRUE';
        $c = \autosystem\DBConn::getTableNameFor('callendar');
        $key ='DELETE FROM `'.$c.'` WHERE `id`=:id AND `accound_owner_id`=:accound_id';//
        $value = [':id'=>$id,':accound_id'=>$accound_id];
        $sql[$key]= $value;
        return $this->transaction($sql);
    }
    public function Update_weekitem($namedkey,$from,$start,$to,$end,$accound_owner_id,$TYPE,$id,$category_id=0){
        $c = \autosystem\DBConn::getTableNameFor('callendar');
        $c_d = \autosystem\DBConn::getTableNameFor('callendar_detail');
        $key ='UPDATE `'.$c.'` SET `namekey`=:key,`from`=:from, `to`=:to, `accound_owner_id`=:accound_owner_id, `TYPE`=:TYPE, `category_id`=:catid WHERE id=:id';
        $value = [':id'=>$id,':key'=>$namedkey,':from'=>$from,':to'=>$to,
        ':accound_owner_id'=>$accound_owner_id,':TYPE'=>$TYPE,':catid'=>$category_id
        ];
        $sql[$key]= $value;
        $key ='UPDATE `'.$c_d.'` SET `start`=:start, `end`=:end, `TYPE`=:TYPE WHERE `callendar_id`=:callendar_id';
        $value = [':callendar_id'=>$id,':start'=>$start,':end'=>$end,':TYPE'=>$TYPE];
        $sql[$key]= $value;
        return is_array($this->transaction($sql));
    }
}
?>