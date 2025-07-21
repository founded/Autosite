<?php
/**
 * @author Lieven Roegiers
 * @copyright 
 * @CMS autosite
*/
namespace autosystem;
class ValidateAs{
    static public $checkPerformace = false;
    static public $checksqlbug = true;
    /**
     * ValidateAs::isLocale()
     * @param mixed $String be->
     */
    static public function isLocale($String) {
        return preg_match('^[A-Za-z]{3}$',$String);// && $String!='www';
    }
	/**
	 * ValidateAs::isMonth()
	 * @param mixed $String 1->12
	 */
	static public function isMonth($String){
	   return preg_match('/^(0?[1-9]{1}|1[012]{1})$/',$String);
    }
    /**
	 * ValidateAs::isYear($String) mixed  1900->2999
	 */
	static public function isYear($String){
        return preg_match('^([19]|[20])[0-9]\d\d$^',$String);
    }
	static public function isDay($String){
        return preg_match('^(0[1-9]|[12][0-9]|3[01])$^',$String);
    }
    /**
     * ValidateAs::isAShortDate($String)dd-mm-yy
     */
    static private function isAShortDate($String){                 
        return preg_match('^(0[1-9]|[12][0-9]|3[01])(0[1-9]|1[012])[0-9]\d\d$^',$String);
    }
    static public function getdatetype($date){
        if($this->isEUAdate($date)){
            return 'EU';
        }
        if($this->isUSAdate($date)){
            return 'US';
        }
        if($this->isEUAdate($date)){
            return 'EU';
        }
    }
    static public function getDate_Arr($dateString,$type,$delemiter){
         if($type == ''){
            $type = self::getdatetype($dateString);
         }
         if($delimiter == ''){
            $delimiter = self::getdatedelemiter($dateString);
         }
         $first = strpos($dateString, $delimiter);
         $second = strpos($dateString,$delimiter,$first);
         if($type='EU'){//validator
                $day=substr($dateString,0, $first);
                $month = substr($dateString,$first,$second);
                $year = substr($dateString,$second);
         }
         return ['Y'=>$year,'m'=>$month, 'd'=> $day];
    }
    static public function getdatedelimiter($date){
        if($this->isEUAdate($date)){
            return 'EU';
        }
        if($this->isUSAdate($date)){
            return 'US';
        }
        if($this->isEUAdate($date)){
            return 'EU';
        }
    }
    /**
    * ValidateAs::isEUAdate()
    * dd   (0[1-9]|[12][0-9]|3[01])   01->09  10->29    30->31
    * mm   (0[1-9]|1[012])            01->09  10->12
    * yyyy (19|20)\d\d)               1900 -> 2099
    * @param mixed $String dd-mm-yyyy
    * @return
    */
    static public function isEUAdate($String){ 
        return preg_match('/^([012]?[0-9]{1}|[3][01])\-(0?[1-9]{1}|1[012]{1})\-(19|20){1}[0-9]{2}$/',$String);
        //return preg_match("^(0[1-9]|[12][0-9]|3[01])-(0[1-9]|1[012])-([19]|[20])[0-9]\d\d$^",$String);
    }
    static public function isAdate($String){                 
        return self::isEUAdate($String)||self::isUSAdate($String);
    }
   /**
    * ValidateAs::isUSAdate()
    * dd   (0[1-9]|[12][0-9]|3[01])   01->09  10->29    30->31
    * mm   (0[1-9]|1[012])            01->09  10->12
    * yyyy (19|20)\d\d)               1900 -> 2099
    * @param mixed $String yyyy-mm-dd 
    * @return
    */
    static public function isUSAdate($String){                 
        return preg_match('^([19]|[20])[0-9]\d\d-(0[1-9]|1[012])-(0[1-9]|[12][0-9]|3[01])$^',$String);
    }
    static public function isAemail($String){
    	if(substr_count($String, '.')>4){return false; }
        return preg_match('/^[A-Za-z0-9._%-]+@[A-Za-z0-9.-]+\.[A-Za-z]{2,4}$/',$String);
            
    }
    static public function isTime($String){
        return preg_match('/^(0[1-9]|1[0-9]|2[01234]):([0-5][0-9]|6[0])$/',$String);
    }
    static public function isTimeHM($String){
    	return preg_match('/^(0[1-9]|1[0-9]|2[01234]):([0-5][0-9]|6[0])$/',$String);	
        //return preg_match('/^(([0]?0[1-9]|1[0-9]|2[01234]):([0-5][0-9]))$/',$String);
    }
    static public function isBadbrowserTime($String){
    	
        return preg_match('/^*(1[0-2]|[1-9]):[0-5][0-9] *(a|p|A|P)(m|M) *$/',$String);
        //return preg_match('/^(0[1-9]|1[0-9]|2[01234]){2}:([0-5][0-9]|6[0]){2}:(am|pm)$/',$String);
    }
    static public function isTimeHMS($String){
    	//return preg_match('/(?[:](/d{2})){3}$/',$String);
		return preg_match('/^(([0]?0[1-9]|1[0-9]|2[01234]):([0-5][0-9]):([0-5][0-9]))$/',$String);
        //return preg_match('/^(0[1-9]|1[0-9]|2[01234]){2}\:([0-5][0-9]|6[0]){2}\:([0-5][0-9]|6[0]){2}$/',$String);
    }
    static public function isTEXT(&$text){    
        return preg_match('/\w/',$text);             
    }
    static public function isFULLTEXT(&$text){
        return preg_match('/[A-Za-z0-9.-]/',$text);
    }
    static public function isPassword(&$text){
        return preg_match('/[A-Za-z0-9.-]/',$text);
    }
    static public function query(&$string){
        if(self::$checkPerformace && file_exists('.offline')){
            $checks = ['/ on /','/ where /','/ as /','/  /','/ and /','/ left join /','/ group by /','/ or /','/ not null /','/ is null /','/ is NULL /','/ order by /','/count\(\*\)/'];//(to uppercase)
            foreach($checks as $check){
                if(preg_match($check,$string)){print 'Performance exception by query "'.$check.'"'.$string ;  throw PerformaceException('');                 }
            }
        }
        if(self::$checksqlbug){
            $checks = ['/DEFAULT \'PROGRESS\'/','/ where /','/ && /'];
            foreach($checks as $check){
                if(preg_match($check,$string)){print 'BUG by query "'.$check.'"'.$string ;  throw InvalidSQLException('');                 }
            }
        }
        return $string!='';//("'/^(SELECT|UPDATE|INSERT) ([a-z])(FROM)(WHERE) )$/",$date);     
    }
    static public function isTerm($term){
        return preg_match('/^[A-Za-z0-9\. \+]{2,}$/',$term);
    }
    static public function isCheckedOrNot($check){
        return $check == true || $check== false;
            
    }
    static public function isnumeric($nr){
        return is_numeric($nr);
    }
    /**
     * ValidateAs::iscolor()
     * @param mixed $collor FFFFFF OR FFF
     * @return
     */
    static public function iscolor($collor){
        return preg_match('^#([A-Fa-f0-9]{6}|[A-Fa-f0-9]{3})$',$collor);
    }
    static public function isMailheaderinject($fromstr){
        //%0D%0A HTML-equivalent =>    \r\n
        return pos($var, '%0D%0A') || pos($var, '%0D%0Abcc:');
    }
    static public function isURL($url){//need first see cron url
        return preg_match('/^([-a-z0-9]{2,100})\.([a-z\.]{2,10})(\/[a-zA-Z\.\/]{2,})?/i', $url);
        //return preg_match('/^([-a-z0-9]{2,100})\.([a-z\.]{2,10})\/([a-zA-Z0-9\.]{2,10}) $/i', $url);
    }
    static public function isDomain($domain) {
    	return preg_match('/^([-a-z0-9]{2,100})\.([a-z\.]{2,10})$/i', $domain);
    }
    static public function isPostalcode($local,$codeString){
        $reg ='';
        $four = ['BE','AT','BG','CH','CY','DK','HU','LU','NO','SI'];
        $five = ['DE','EE','ES','FI','FR','GF','GP','IT','LT','MQ','SK','WF'];
        if(in_array($local,$four)){
            $reg = '(^[0-9]{4}$)';
        }elseif(in_array($local,$five)){
            $reg = '(^[0-9]{5}$)'; 
        }else{
            switch ($local){ 
            case 'RO'://Martinique
                $reg = '(^[0-9]{6}$)';
                break;
        	case 'CA' ://Bulgaria
                $reg ='^([A-Z]\d[A-Z]\s\d[A-Z]\d)$^';
        	    break;
        	case 'CZ' ://CZECH REPUBLIC
                $reg ='(^[0-9]{3}[[:space:]]{0,}[0-9]{2}$)';
                break;
            case 'GB' :
                $alpha1 = '[A-PR-UWYZ]'; // Character 1
                $alpha2 = '[A-HK-Y]'; // Character 2
                $alpha3 = '[A-HJKSTUW]'; // Character 3
                $alpha4 = '[ABEHMNPRV-Y]'; // Character 4
                $alpha5 = '[ABD-HJLNP-UW-Z]';
                $reg =  '^('.$alpha1.'{1}'.$alpha2.'{0,1}[0-9]{1,2})([[:space:]]{0,})([0-9]{1}'.$alpha5.'{2})';                                  // Expression for postcodes: AN NAA, ANN NAA, AAN NAA, and AANN NAA with a space
                $reg .= '|('.$alpha1.'{1}[0-9]{1}'.$alpha3.'{1})([[:space:]]{0,})([0-9]{1}'.$alpha5.'{2})';                                     // Expression for postcodes: ANA NAA
                $reg .= '|('.$alpha1.'{1}'.$alpha2.'[0-9]{1}'.$alpha4.')([[:space:]]{0,})([0-9]{1}'.$alpha5.'{2})';  // Expression for postcodes: AANA NAA
                $reg .= '|(gir)(0aa)';
                $reg .= '|(bfpo)([0-9]{1,4})';
                $reg .= '|(bfpo)(c\/o[0-9]{1,3})';
                $reg .= '|([a-z]{4})(1zz)$/i^'; 
           case 'GR'://GREECE
           case 'SE'://SWEDEN
                $reg = '(^[0-9]{3}[[:space:]]{0,}[0-9]{2}$)';
                break;    
           case 'LV'://Ladvia
                $reg = '(^[A-Z]{2,3}-{0,}[0-9]{4}$)';
                break; 
           case 'MT'://MALTA
                $reg = '/[a-z]{3}\s[0-9]{4}/i';
                break;
           case 'NL'://Netherlands
                $reg = '(^[0-9]{4}[a-zA-Z]{2}$)';
                break;
           case 'PL'://POLAND
                $reg = '(^[0-9]{2}-[0-9]{3}$)';
                break;      
        	default :
                throw exception('ValidateAs POSTAL NOT EXIST');
            }
        }
        return preg_match($reg,$codeString);
    }
    static public function isIP4($ip){
        $ipnums = explode('.', $ip);
        if(count($ipnums) != 4) {        		            return false;}
        foreach($ipnums as $ipnum) {
        	if(!is_numeric($ipnum) || ($ipnum > 255)) {     return false;}
        }
        if(filter_var($ip, FILTER_VALIDATE_IP, FILTER_FLAG_IPV4)===true){
            return false;
        }
        return true;
    }
    static public function isIP6($ip){
        if(filter_var($ip, FILTER_VALIDATE_IP, FILTER_FLAG_IPV6) === false) {
            return false;
        }
        return true;
    }
    static public function isIP($ip){
        return self::isIP4($ip)||self::isIP6($ip);
    }
}
?>