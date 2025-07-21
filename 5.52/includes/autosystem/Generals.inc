<?php
    namespace autosystem;
	abstract class Generals{
       function __construct(){
			$path = explode('/',htmlspecialchars($_SERVER['PHP_SELF']));
			array_pop($path);
            $this->installpath=implode('/',$path).'';
            //print $this->installpath;
            $this->imgups = $this->installpath.'/layout/ups/';
	   }
	   public $modeltypes = ['NULL'=>'not availeble','PUBLIC'=>'public','UNDERCONSTRUCTION'=>'unavaible','TESTING'=>'testing','FINANCIAL'=>'financial','TRANSLATE'=>'translate','MANAGE'=>'manage','ADMINISTRATION'=>'admin','DIRECTOR'=>'director','MEDICAL'=>'medical','KING'=>'siteking','USER'=>'user','MODERATOR'=>'moderator','CHAIRMAN'=>'chairman'
       ];//,'SUPPORT'=>'support','PRODUCTION'=>'production','OWNER'=>'owner');
       static protected $charset = 'utf8';
       static protected $collate = 'utf8_unicode_ci' ;
       static protected $locales = ['nl'=>'locale.nl','ua'=>'locale.ua','fr'=>'locale.fr','en'=>'locale.en','es'=>'locale.es','ru'=>'locale.ru','zh'=>'locale.zh'];//TODO int
       public $actions = array('edit','add','delete');
	   protected $imgups ;
	   protected $installpath ;    
	}
?>