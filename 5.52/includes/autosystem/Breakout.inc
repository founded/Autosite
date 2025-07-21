<?php
namespace autosystem;
class Breakout{
    protected $browser;
    private $log = false;
    private $agent	= 'Mozilla/5.0 (compatible; MSIE 9.0; Windows NT 6.1; WOW64; Trident/5.0; .NET CLR 3.5.30729; .NET CLR 3.0.30729; .NET CLR 2.0.50727; Media Center PC 6.0';//)
    private $host;
    public function __construct($host) {
        $this->host = $host;
    }
    public function send($location,$action,$data){
        $ch = curl_init($this->host.DIRECTORY_SEPARATOR.$location.DIRECTORY_SEPARATOR.$action);
        curl_setopt($ch, CURLOPT_FAILONERROR,    false);
        curl_setopt($ch, CURLOPT_POST,           1); 
        curl_setopt($ch, CURLOPT_HEADER,         true);
        curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);
        curl_setopt($ch, CURLOPT_CONNECTTIMEOUT, 120);
        curl_setopt($ch, CURLOPT_USERAGENT,      $this->agent);
        curl_setopt($ch, CURLOPT_POSTFIELDS,     $this->arraytopostString($data));
        return curl_exec($ch);
    }

}
?>

