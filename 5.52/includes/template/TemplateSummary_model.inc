<?php
/* model*/
namespace template;
class TemplateSummary_model extends \autosystem\Model{
    protected $isinstalled = false;
    public function getModelInfo(){     return 'templatesummary.model.info';   }
    public function __install(){        return true;    }
    public function __upgrade($version){return true;    }
    public function isInstalled(){
        return $this->isinstalled|| isset($this->DB)&& $this->isinstalled=true;
    }
    public function getVersionNr(){        return (float)5.4;  }
    public function getRuleDefault(){      return 'KING';    }
    public function GET_Template($location,$templatename){
        $file =  \autosystem\Properties::get_templatelocUrl($location,$templatename);
        try{
            if(!is_file($file)){return ''; }
            return file_get_contents($file);
        }catch(\exception $e){
            return '';
        }
    }
    public function save_Template($title,$shortcomment,$content,$accound_id,$category,$status,$access ,$locale,$titlekey=null,$VISIBLE=false){
    }
    public function update_Template( $id,$page_status,$title,$page_access,$shortcomment,$content,$page_category=0){
    }
    public function create($name,$location,$content){
        $file =  \autosystem\Properties::get_templatelocUrl($location,$name);
        if(!\autosystem\Properties::exist_location($location)){
            print 'save location do not exist';
        }
        file_put_contents($file,$content);
    }
    public function GET_Categorys(){
        $FileScanner = new \data\FileScanner();
        $FileScanner->scandir('./layout/views');
        return $FileScanner->getdirs();
    }
    public function GETTranslateArray($id,$accound_id,$locale){

    }
    public function exist_Template($module,$templatename){

    }
    public function delete_Template($module,$templatename){

    }
}
?>
