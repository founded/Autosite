<?PHP
 /**
 * @author Lieven Roegiers
 * @copyright 2007 refactor 2009
 * @CMS autosite
 */
namespace data;
class DataSplit_HTTP extends DataSplit{
    
    public function getASPsessionId($header){
        
    }
    public function getPHPsessionId($header){
        /* @param mixed $data .be whois.dns.be #from belgium
        * @param mixed $regx "|(\.(?<ext>[A-Z\.a-z]+))( +(?<url>[A-Z\.a-z]+))( #+(?<comment>.*))?\n?|"
        * @param mixed $needle ['ext','url','comment']*/
        $array =  $this->spliter($header,
        '|(Set-Cookie: (?<key>[A-Z\.a-z_]+))(=+(?<value>[A-Z\.a-z0-9]+)); |i'
        ,['key','value']);
        if(isset($array[0])){
            return $array[0];
        }
        //preg_match('/ ASP.NET_SessionId=(.*);/i', $data0, $__ASPNET);
    } 
}
?>