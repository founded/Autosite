<?php
namespace autosystem;
class Autocomplete{
    private $cashdir='./oncache/autocompletes/';
    public function init(){
       $this->getModel();
       if(key_exists('search',$_GET)&&$id = parent::getAccound_id()){
            $data = [];
            echo json_encode($data);
       }elseif(key_exists('search',$_GET)){
            $data = [];
            echo json_encode($data);
       }
       return;
    }
}
?>