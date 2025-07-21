<?php
/* model*/
namespace Amail;
class CreateAEmail extends \Amail\Emails_model{
    public function activationmail($accound_id = 0,$subject='activate accound'){
        $table = $this->getEmailtbl();
        $concept_id = $this->getConceptId('activation');
        $key ='INSERT INTO `'.$table.'`( `id`,`STATUS`, `conceptmail_id` , `accound_id`,`imputDate`,`subject`)'.
              ' VALUES(NULL, \'SENDIBLE\',:conceptmail_id,:accound_id,:imputDate ,:subject )' ;
        $value = [':conceptmail_id'=>$concept_id,':accound_id' => $accound_id,
                  ':imputDate' => $this->nowDateTime(),':subject'=>$subject];
        $sql[$key]= $value;
        
        //transaction
    }
    private function getConceptId($name){
        return 1; 
    }
    public function reactivationmail($accound_id = 0,$subject='reactivate accound'){
        $table = $this->getEmailtbl();
        $concept_id = $this->getConceptId('reactivation');
        $key ='INSERT INTO `'.$table.'`( `id`,`STATUS`, `conceptmail_id` , `accound_id`,`imputDate`,`subject`)'.
              ' VALUES(NULL, \'SENDIBLE\',:conceptmail_id,:accound_id,:imputDate ,:subject )' ;
        $value = [':conceptmail_id'=>$concept_id,':accound_id' => $accound_id,
                  ':imputDate' => $this->nowDateTime(),':subject'=>$subject];
        $sql[$key]= $value;
        /**$key ='INSERT INTO `'.$e.'`(`id`,`email`,`emailchecked`,`emailsubscribed`)'.
                  ' VALUES(NULL,:email,:emailchecked,:emailsubscribed) RETURNING email_id';//
                $value = [':email'=>$email,':emailchecked'=>'FALSE',':emailsubscribed'=>'FALSE'];
                $sql[$key]= $value;*/
        //transaction
    }
    public function newmail(){
        /**$key ='INSERT INTO `'.$e.'`(`id`,`email`,`emailchecked`,`emailsubscribed`)'.
                  ' VALUES(NULL,:email,:emailchecked,:emailsubscribed) RETURNING email_id';//
                $value = [':email'=>$email,':emailchecked'=>'FALSE',':emailsubscribed'=>'FALSE'];
                $sql[$key]= $value;*/
    }
    public function setTestmail($accound_id,$concept_id,$subject){
        $table = $this->getEmailtbl();
        $key ='INSERT INTO `'.$table.'`( `id`,`STATUS`, `conceptmail_id` , `accound_id`,`imputDate`,`subject`)'.
              ' VALUES(NULL, \'SENDIBLE\',:conceptmail_id,:accound_id,:imputDate ,:subject )' ;
        $value = [':conceptmail_id'=>$concept_id,':accound_id' => $accound_id,
                  ':imputDate' => $this->nowDateTime,':subject'=>$subject];
        $sql[$key]= $value;
        return $this->transaction($sql);
    }
}
?>