<?php
    namespace page;
class Locales_model extends \autosystem\Model{
    public function GetLocales(){
        return ['locale.nl'=>'','locale.fr'=>'','locale.en'=>'','locale.es'=>'','locale.ru'=>'','locale.zh'=>''];
    }
    protected $isinstalled;
    public function getModelInfo(){
        return 'locales.model.info';
    }
    public function getVersionNr(){
        return (float)4.6;
    }
    public function isInstalled(){
        return true;
    }
    public function __install(){
        return true;
    }
    public function __upgrade($version){
            return true;
    }
    public function getRuleDefault(){
            return 'ADMINISTRATION';//"USER";//"PUBLIC";//
    }
}
?>