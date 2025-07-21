<?php
	namespace template;
    /**
     * @author Lieven Roegiers
     * @copyright 2009 
     * @CMS autosite
     */ 
 
class TemplateScanner extends \data\FileScanner{
    public function getTemplates($path){
        $exts['inc']= ['.Qtemplate'];
    	$FileScanner->setfilter($exts);
        $FileScanner->scandir('./layout/views/'.$path);
        return $FileScanner->getfils();
    } 
}
?>