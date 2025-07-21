<?php
/**
 * @author Lieven Roegiers
 * @CMS autosite 
 */
 namespace autosystem;
class Key{
     /**
	 * @param int $len
	 * @return
	 */
    private $string;
    static public function GET($len){
        $tekens = ['a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z',
         'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','W','X','Y','Z',
         '0','1','2','3','4','5','6','7','8','9','0','1','2','3','4','5','6','7','8','9'];
		  srand(((double) microtime() ) * 1000000 );
          $string = '';
		  for( $i = 0; $i < $len; $i++ ){
		  	//48-57  65,90 97-122
		  	$r = rand(48,122);
		  	if($r<57||$r>96){
				$string .= chr($r);
			}else{
				if($r<91&&$r>64){
					$string .= chr($r);
				}else{
					$i--;
				}
			}
		  	//$string .= chr(rand(65,90));
              /*$tekens{ 
		  	   rand( 0, (
               strlen( $tekens ) - 1 ) ) };*/
		  }
		  return $string;
    }
}

?>