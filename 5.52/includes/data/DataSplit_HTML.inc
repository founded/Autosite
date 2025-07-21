<?PHP
 /**
 * @author Lieven Roegiers
 * @copyright 2007 refactor 2009
 * @CMS autosite
 */
namespace data;
class DataSplit_HTML extends DataSplit_HTTP{
    public function getkeywords(&$html){
        $pattern = "/name=\"keywords\" content=[\"'](?<keywords>[a-zA-Z0-9, -]*)[\"']*/i";
        preg_match_all($pattern, $html, $keywords);
        print_r($keywords);
        if(key_exists('keywords',$keywords)&& count($keywords)>0){
            if($arr = explode(',',$keywords['keywords'][0])){
               return $arr;
            }else{
               return $keywords['keywords'][0];  
            }
        }
    }
    public function getTitles(&$html){
        $pattern = '/\<h(?<nr>[0-9 ])+>(?<titles>[a-zA-Z0-9, -]*)\<\/h[0-9 ]+>*/i';
        preg_match_all($pattern, $html, $titles);
        if(key_exists('titles',$titles)){
            //print_r($titles['titles'] );
            if($arr = explode(',',$titles['titles'][0])){
               return $arr;
            }else{
               return $titles['titles'][0];
            }
        }
    }
    public function getUrls(&$html){
        $pattern =  '|<a (href="(?<url>[a-zA-Z0-9.: -/?/&/;=]+)")(.*)>('.
                    '(?<content>([a-zA-Z0-9, -]*))?'.
                    '(<img([:.= "0-9A-z\/]*) alt="(?<alt>[a-zA-Z0-9 _]+)"([a-z= 0-9A-Z/]*)*>([<>a-z= 0-9A-Z/-])*)?'.
                    ')</a>|iU';
        //print $pattern;
        preg_match_all($pattern, $html, $urls);
        $return = false;
        if(key_exists('url',$urls)&& is_array($urls['url'])){
            $return=[]; 
            foreach($urls['url'] as $key=>$url){
                $return[$url]='';
                if($urls['content'][$key]!=''){
                    $return[$url]=$urls['content'][$key];
                }
                if($urls['alt'][$key]!=''){
                    $return[$url]=$urls['alt'][$key];
                }
            }
            return $return;  
        }
    }
    /** TODO
	unconstruct
	*/
    public function getInput(&$html,$type,$minlen=3){
        $pattern = '/(?:<input )(?<content>.*)>/isU';
        preg_match_all($pattern, $html, $content);
        if(key_exists('url',$urls)&& is_array($urls['url'])){
            $return=[]; 
            foreach($urls['url'] as $key=>$url){
                $return[$url]='';
                if($urls['content'][$key]!=''){
                    $return[$url]=$urls['content'][$key];
                }
                if($urls['alt'][$key]!=''){
                    $return[$url]=$urls['alt'][$key];
                }
            }
            return $return;  
        }
	}
    public function getTag(&$html,$tag){
        $pattern = '|<'.$tag.'(.*)>(?<content>([a-zA-Z0-9, -]*))</'.$tag.'>|U';
        preg_match_all($pattern, $html, $tags);
        if(key_exists('content',$tags)){
            if($arr = explode(',',$titles['content'][0])){
               return $arr;
            }else{
               return $titles['content'][0];  
            }
        }
    }
    public function getbody(&$html,$tag,$minlen=3){
        $pattern = '/(?:<body[^>]*>)(?<content>.*)<\/body>/isU';
        preg_match_all($pattern, $html, $content);
        if(key_exists('content',$content)){
            $pattern2 = '/(<[^>]*[^\/]>)/i';
            $tags=preg_split($pattern2, $content['content'][0], -1, PREG_SPLIT_NO_EMPTY );
            $return = [];
            array_unique($tags);
            foreach($tags as $tag){
                $trimedTag = trim($tag);
                if(!empty($trimedTag)&& strlen($trimedTag)>$minlen&& 
                preg_match('/^[A-Za-z0-9\. \*\+
    ?-]{2,}$/',$trimedTag) && 
                substr($trimedTag,0,4)!=='var '){
                    array_push($return,$trimedTag);
                }   
            }
            return $return;
        }else{
            print 'no content';
        }
    }
    public function getbody_terms(&$html,$tag,$minlen=3){
        $pattern = '/(?:<body[^>]*>)(?<content>.*)<\/body>/isU';
        $this->removeTag($html,'header');
        $this->removeTag($html,'footer');
        preg_match_all($pattern, $html, $content);
        if(key_exists('content',$content)){
            $pattern2 = '/(<[^>]*[^\/]>)/i';
            $tags=preg_split($pattern2, $content['content'][0], -1, PREG_SPLIT_NO_EMPTY );
            $return = [];
            array_unique($tags);
            foreach($tags as $tag){
                $trimedTag = trim($tag);
                if(!empty($trimedTag)&& strlen($trimedTag)>$minlen&& 
                preg_match('/^[A-Za-z0-9\. \*\+
    ?-]{2,}$/',$trimedTag) && 
                substr($trimedTag,0,4)!=='var '){
                    array_push($return,$trimedTag);
                }   
            }
            return $return;
        }else{
            print 'no content';
        }
    }
    private function removeTag(&$html,$tag){
        $A = substr($html,0,strpos($html, '<'.$tag));
        $B = substr($html,strpos($html, '/'.$tag.'>')+2+strlen($tag));
        $html =  $A.$B;
    }
    public function getAHref(&$html){
        preg_match_all('|<a(.*)>|U',$html,$matches);
        if(is_array($matches[1])){
            $return = []; 
            foreach($matches[1] as $key=>$value){
                $pattern = '/href=((\'(.*)\')|("(.*)"))/';
                preg_match($pattern, $value, $match);
                $a = array_pop($match);
                if(isset($a)){
                    $return[$key]= $this->HTML_ACTIONS->GET_URL_from_A_href($a,$this->host);
                }
            }
            return $return;  
        }
    }
}
?>