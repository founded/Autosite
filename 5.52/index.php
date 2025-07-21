<?php
include('./general.php');
$locale = getlang($_SERVER['HTTP_HOST']);
//error_reporting(E_ALL ^ E_DEPRECATED ^ E_NOTICE ^ E_WARNING);
function getlang($HTTP_HOST){
    if(preg_match('^([A-Za-z]+){2,3}\.(([A-Za-z0-9]+)\.)+([A-Za-z]+){2,10}$^',$HTTP_HOST)){
        return substr($HTTP_HOST,0,strpos($HTTP_HOST,'.'));
    }else{
        return 'en';
    }
}
new Init($locale);
?>
