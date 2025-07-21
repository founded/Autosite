<?php
/**
 * @author Lieven Roegiers
 * @copyright 
 * @CMS autosite
 * Qview standard variables reservation
 * $values['path.layout'] this is the place of layout can change to protect something
 * $values['path.module'] this is the place of module 
 * $values['i']//can use of count outputlines or jquery or engine
 * $values['path.install'] this is the place of install can change when upgrade use versionnr
*/
namespace autosystem;
abstract class Model extends \autosystem\Generals{
	   protected $DB;
       protected $UserRulles;
       protected $DEBUG = false;
       protected $paginator;
       protected $defaultPageRange;
       private $protectables=['accound','email'];
       protected $resizable = false;//images
       protected $regexSpliters = [];//colname => spliter !!!MEMspliter DataSplitAR_interface
       public function __construct($UserRulles=[],$islogin=false,$defaultPageRange=20){
       	 parent::__construct();	
         $this->UserRulles = $UserRulles ;
         if(key_exists('ADMIN',$UserRulles)){
                    $this->DB = \autosystem\DBConn::GET(true,true);
           }else{
              if(key_exists('SUPER',$UserRulles)){
                    $this->DB = \autosystem\DBConn::GET(true,false);
              }else{
                    $this->DB = \autosystem\DBConn::GET();
              }
           }
           $this->paginator = new \autosystem\Paginator();
           $this->defaultPageRange = $defaultPageRange;
           $this->resizable = $this->existTableLabel('images_resize'); //TODO workaround image size
	   }
	   abstract public function __install();
       /**
        * Model::isInstalled()
        * @return void
        */
       abstract public function isInstalled();
       /**
       abstract public function needToUpgrade();
       abstract public function __upgrade(); return bool
       abstract public function downgrade($toVersionNr,$dataoutput); return bool
       abstract public function getUsedPHPlib(); return[]
       */
       /**
        * Model::getModelInfo()
        * use name of model this is a key for translation
        * @return void
        */
       abstract public function getModelInfo();
       abstract public function getVersionNr();
       abstract public function getRuleDefault();
       public function getPaginatorData(){
            return $this->paginator->getdata();
       }
       /** see deprications move to AGET */
       public function GET($templateLocation,$templateName,&$view,$sql,$param=null,$images=false,$imgaccound_id=0,$imgprefix=''){
           print $this->AGET($templateLocation,$templateName,$view,$sql,$param,$images,$imgaccound_id,$imgprefix);
       }
       public function AGET($templateLocation,$templateName,&$view,$sql,$param=null,$images=false,$imgaccound_id=0,$imgprefix=''){
           try{               
               if(\autosystem\ValidateAs::query($sql)&&($dbStmt = $this->DB->prepare($sql))&&$dbStmt->execute($param)){
                    if($row = $dbStmt->fetch(\PDO::FETCH_ASSOC)){
                        if($images==true){
                            $row['images'] = $this->getImages($imgprefix.$templateLocation,$row['id'],$imgaccound_id,$view,'A_ITEM');
                        }
                        $return = $view->get_Templateclone($templateLocation,$templateName,$row);
                        @$dbStmt->closeCursor();
                        return $return;
                    }
                    @$dbStmt->closeCursor();
                }
            }catch(PDOException $e) {
                $this->Modellog($e,'GET'.$templateLocation.'->'.$templateName,__LINE__,$sql .$this->paginator->getsql());
            }catch(Exception $e){
                $this->Modellog($e,'GET'.$templateLocation.'->'.$templateName,__LINE__,$sql .$this->paginator->getsql());
            }
            return false;
       }
       /**
        * Model::GET_OverView()
        * @param mixed $templateLocation 
        * @param mixed $templateName used template/item
        * @param mixed $view object of Qview
        * @param mixed $sql query prepared statement NO SORT, NO LIMITS LIMIT IS USED BY PAGINATOR
        * @param mixed $param sql parameters
        * @param bool $images are images used or not (gallery)
        * @param integer $imgaccound_id id of user 
        * @return
        */
       public function GET_Summary($templateLocation,$templateName,&$view,$sql,$param=null,$images=false,$imgaccound_id=0,$imgprefix=''){
       	 	print $this->GET_QSummary($templateLocation,$templateName,$view,$sql,$param,$images,$imgaccound_id,$imgprefix,false);
	   }
       public function GET_QSummary($templateLocation,$templateName,&$view,$sql,$param=null,$images=false,$imgaccound_id=0,$imgprefix='',$isexploirsql=false){
            try{            
                if(\autosystem\ValidateAs::query($sql)&&($dbStmt = $this->DB->prepare($sql.(($isexploirsql)?'':$this->paginator->getsql())))&&
                $dbStmt->execute($param)){
                    $res = $dbStmt->fetchAll(\PDO::FETCH_ASSOC);
                    $i=0;
                    $return = '';
                    foreach ($res as $row){
                        $row['row']= ($i++%2==0)?'equally':'odd';
                        if($images){
                            $row['images'] = $this->getImages($imgprefix.$templateLocation,$row['id'],$imgaccound_id,$view,'A_LIST');
                            $row['thumb'] = $this->getImages($imgprefix.$templateLocation,$row['id'],$imgaccound_id,$view,'A_THUMP');
                        }
                        $return .= $view->get_Templateclone($templateLocation,$templateName,$row);
                    }
                    @$dbStmt->closeCursor();
                    return $return;     
                }else{
                    return false;
                }
            }catch(\PDOException $e){
                $this->Modellog($e,'GET_Summary@'.$templateLocation.'->'.$templateName,__LINE__,$sql .$this->paginator->getsql());
                return false;
            }catch(Exception $e){
                $this->Modellog($e,'GET_Summary@'.$templateLocation.'->'.$templateName,__LINE__,$sql .$this->paginator->getsql());
                return false;
            }
       }
       public function GET_COUNT($sql){
           try{               
                if(($dbStmt = $this->DB->query($sql))){
                    if($res = $dbStmt->fetch(\PDO::FETCH_ASSOC)){
                       $n=(isset($res['count']))? $res['count']: $dbStmt->rowCount();
                       @$dbStmt->closeCursor();
                       return $n;
                    }
                }
                return false;
            }catch (\PDOException $e){
                return false;
            }
       }
       /**
        * Model::GET_Array()
        * make SELECT yourtabelname as key, yourvaluename as value FROM ...
        * @param mixed $sql
        * @return
        */
       public function GET_Array($sql,$params=[],$debug=false){
            try{
                $return=[];
                if($debug){
				    print \autosystem\DEBUG::printPDO($sql,$params,false,'GET_Array');
                }
                if(\autosystem\ValidateAs::query($sql)&&($dbStmt = $this->DB->prepare($sql))&&$dbStmt->execute($params)){
                    $res = $dbStmt->fetchAll(\PDO::FETCH_ASSOC);
                    $i=0;
                    foreach ($res as $row){
                        $return[$row['key']]= $row['value'];
                    }                    
                    @$dbStmt->closeCursor();
                    return $return;     
                }else{
                    return false;
                }
            }catch (PDOException $e){           $this->Modellog($e,'GET_Array',__LINE__,$sql );}
       }
       public function GET_Array_FULL($sql,$params=[],$debug=false){
            try{
                $return=[];
                if($debug){
				    print \autosystem\DEBUG::printPDO($sql,$params,false,'GET_Array');
                }
                if(\autosystem\ValidateAs::query($sql)&&($dbStmt = $this->DB->prepare($sql))&&$dbStmt->execute($params)){
                    $res = $dbStmt->fetchAll(\PDO::FETCH_ASSOC);
                    $i=0;
                    foreach ($res as $row){
                        $return[]= $row;
                    }                    
                    @$dbStmt->closeCursor();
                    return $return;     
                }else{
                    return false;
                }
            }catch (PDOException $e){           $this->Modellog($e,'GET_Array',__LINE__,$sql );}
       }
       public function GET_Array_count($sql,$params=[]){
            try{
                $return=[];
                if(\autosystem\ValidateAs::query($sql)&&($dbStmt = $this->DB->prepare($sql))&&$dbStmt->execute($params)){
                    $res = $dbStmt->fetchAll(\PDO::FETCH_ASSOC);
                    $i=0;
                    foreach ($res as $row){
                    	if(!key_exists($row['key'],$return)){
							$return[$row['key']]= $row['value'];
						}else{
							$return[$row['key']]+= $row['value'];
						}
                    }                    
                    @$dbStmt->closeCursor();
                    return $return;     
                }else{
                    return false;
                }
            }catch(PDOException $e){           $this->Modellog($e,'GET_Array',__LINE__,$sql );}
       }
       public function GET_Stmt_key($sql,$param,$maxres=0,$skey=null){
            try{
                $return =[];
                if(\autosystem\ValidateAs::query($sql)&&($dbStmt = $this->DB->prepare($sql.$this->paginator->getsql()))&&$dbStmt->execute($param)){
                    $i=0;
                    while($res = $dbStmt->fetchAll(\PDO::FETCH_ASSOC)){
                       foreach($res as $key=>$value){
                            $akey = $value[$skey];
                            if(is_array($value)){
                                foreach ($value as $realkey=>$realvalue){
                                    $return[$akey][$realkey]= $realvalue;
                                }
                            }else{
                                $return[$key]= $value;
                            }
                        }
                        //$i++;
                        //if($i>$maxres) { break;}
                    }
                    @$dbStmt->closeCursor();
                    return $return;     
                }else{
                    return false;
                }
            }catch (PDOException $e) {          $this->Modellog($e,'GET_Stmt',__LINE__,$sql.$this->paginator->getsql() );}
       }
       public function GET_Stmt($sql,$param,$maxres=0){
            try{
                $return =[];
                if(\autosystem\ValidateAs::query($sql)&&($dbStmt = $this->DB->prepare($sql))&&$dbStmt->execute($param)){
                    $i=0;
                    while($res = $dbStmt->fetchAll(\PDO::FETCH_ASSOC)){
                           foreach($res as $key=>$value){
                                if(is_array($value)){
                                    foreach ($value as $realkey=>$realvalue){
                                        $return[$realkey]= $realvalue;
                                    }
                                }else{
                                    $return[$key]= $value;
                                }
                            }
                            $i++;
                            if($i>$maxres) { break;}
                    }
                    @$dbStmt->closeCursor();
                    return $return;     
                }else{
                    return false;
                }
            }catch (PDOException $e) {          $this->Modellog($e,'GET_Stmt',__LINE__,$sql);}
       }
       public function Simple_Query($sql){
            try{
                if(\autosystem\ValidateAs::query($sql)){
                    return $this->DB->query($sql)!=false;     
                }
            }catch (\PDOException $e) {
                $this->Modellog($e,'GET_Stmt',__LINE__,$sql);
            }
            return false;
       }
       protected function isQuery($sql,$param,$default = false){
            try{ 
                if(\autosystem\ValidateAs::query($sql)&&($dbStmt = $this->DB->prepare($sql))&&$dbStmt->execute($param)){
                    return( $dbStmt->rowCount() > 0 );
                }
                @$dbStmt->closeCursor();
                return false;
            }catch (\PDOException $e) { $this->Modellog($e,'ISQ',__LINE__,$sql );        return $default;}
       }
       protected function existTableLabel($label){
	   	    if($this->DEBUG){  if(preg_match('^*[A-Z]{1}*$',$label)){	$e = 'do not use capital char for '.$label;
				    $this->Modellog($e,'existTableLabel',__LINE__,'');}
			   	print $label; 
			}
            return $this->existTable(\autosystem\DBConn::getTableNameFor($label));
       }
       private function existTable($tablename){
            try{                
                $res = $this->DB->query('SHOW TABLES LIKE \''.$tablename.'\'');
                $vals = $res->fetch();
                if($this->DEBUG){ print('['.$vals[0]).'--'.$tablename.']'.(($vals[0]==$tablename)?'TRUE':'FALSE').'SHOW TABLES LIKE \''.$tablename."'\n"; }
                if($vals===false){return false;}
                return($vals[0]==$tablename);
            }catch (PDOException $e) {          $this->Modellog($e,'existTable',__LINE__,'');
            }
       }
       /**
        * Model::transaction()
        * AUTObinding
        * $array["... WHERE ... RETURNING `category_id`"]=[];//returning wil be last insertid
        * $array["... WHERE ... "]=[':category_id'=>'',':accound_id'=>$accound_id];
        * OR use
        * SELECT LAST_INSERT_ID() INTO @mysql_variable_here;
        * or by xtable
        * INSERT INTO table_c (table_a_id,table_b_id) VALUES (SELECT a.id WHERE a.ref="I", SELECT b.id WHERE b.ref="Partner");
        * INSERT INTO table_c (table_a_id,table_b_id) SELECT a.id, b.id FROM a, b WHERE a.Name="I" AND b.Class="Partner";
        * @param mixed $sqlarray
        * @return
        */
       protected function transaction($sqlarray,$debug=false){
            if($debug){
				print '<h1>transactioninfo:</h1><pre>x'. \autosystem\DEBUG::printTransaction($sqlarray,false).'x</pre>';
            }
            try{
               $commit = true;
               $this->DB->beginTransaction();
               $lastIds = [];
               $lastsql ='';
               if(!is_array($sqlarray)){
                    global $_DEBUG;
                    if($_DEBUG){print('Wrong implementation transaction');}
                    throw new \exception('Wrong implementation transaction');
               }
               foreach($sqlarray as $sql=>$param){
                    try {
                        $lastsql =$sql;
                        $dbStmt;
                        if(is_array($param)){
                            if($debug){print 'par <br>'.print_r($lastIds,true);}
                            foreach($param as $key=>$value){
                                
                                if(array_key_exists($key,$lastIds)){
                                    $param[$key]= $lastIds[$key];
                                    if($debug){print 'FILL "'.$key.'" as "'.$lastIds[$key].'"<br>';}
                                }else{
                                    //if($debug){print 'NOFILL "'.$key.'" as "'.$lastIds[$key].'"<br>';}
                                }   
                            }
                        }else{
                            if($debug){  print 'param can not be string';
            			    }
                        }
                        $returnlabel = $this->getReturnlabel($sql);
                        if($debug){ 
                            print ''.$sql.__LINE__.'******param:<--'.print_r($param,true).'-->'.count($param).'<br>';
            				print \autosystem\DEBUG::printPDO($sql,$param,''.__LINE__.'param:'.print_r($param,true).'<br>');
            			}
                        if(!\autosystem\ValidateAs::query($sql)||
                        !($dbStmt = $this->DB->prepare($sql))||
                        !$dbStmt->execute($param)){
                            $commit = false;//$resulteffect = 
                            if($debug){print 'NO comit'.__LINE__.'******param:'.print_r($param,true).'<br>';}
                        }else{
                            if($debug){print 'comit'.__LINE__.'id'.$this->DB->lastInsertId().'<br>';}

                            if(!empty($returnlabel)){
                                $lastIds[':'.$returnlabel]=$this->DB->lastInsertId();
                                if($debug){print 'see transaction debug msg set "'.$returnlabel.'" as "'.$lastIds[':'.$returnlabel].'"<br>'.print_r($lastIds,true);}
                                if($lastIds[':'.$returnlabel]==0){
                                    $vals = $dbStmt->fetch(\PDO::FETCH_ASSOC);
                                    $lastIds[':'.$returnlabel]=$vals[$returnlabel];
                                }
                            }else{
                                if($debug){print '**----------NO RET<br>'.print_r($lastIds,true);}
                                
                            }
                        }
                        //@$dbStmt->closeCursor();
                    }catch(\PDOException $ex){
                        global $_DEBUG;
                        if($_DEBUG){\autosystem\DEBUG::printERR($ex);}
                        $this->Modellog($ex,true,'in-transaction',__LINE__,$sql );           $commit = false;
                    }
               }
               if($commit && $this->DB->commit()){
                    return $lastIds;
               }else{
                    $this->DB->rollback();
                    return false;
               }
            }catch (\PDOException $e) {
               $this->Modellog($e,'transaction',__LINE__,$lastsql );
               @$this->DB->rollback();
               return false;
            }
       }
       

       protected function renderdata($basesql,$param, $transactionsqlarray,$debug=false,$splitteron=false,$info=true ){
            try{
                if($info) print 'RENDER START ';
                $returnlabel = $this->getReturnlabel($basesql);                
                if(\autosystem\ValidateAs::query($basesql)&&($dbStmt = $this->DB->prepare($basesql))&& $dbStmt->execute($param)){
                    $baseres = $dbStmt->fetchAll(\PDO::FETCH_ASSOC);
                   
                    $i=0;
                    $return = '';
                    foreach ($baseres as $irow => $row){
                        if (is_array($row)){
                            foreach ($row as $k => $value){
                                if(!empty($returnlabel)&& $k ==$returnlabel){
                                    $row[':'.$returnlabel]=$value;
                                }
                                if($splitteron){
                                    /*if($this->isdatasplitter(array_keys($baseres))){
                                        print '<h1>key exist'.$k.'</h1>';
                                    }*/
                                    if($splitter = $this->getSpliter($k)){
                                        if($splitter !== false){
                                            //$a = $splitter->renderSplit($v);
                                            if($info) print '<h1>key spliter'.$k.'->'.$value.'</h1>'.print_r($splitter->renderTest($value),true);
                                            $row = array_merge($splitter->renderSplit($value),$row) ;
                                        }
                                    }
                                }
                                
                            }
                            //print $irow.'<pre>*******'.print_r($row,true).'</pre>';
                            $executesqlarray=[];
                            foreach($transactionsqlarray as $query =>$params){
                                foreach($params as $key=>$value){
                                    if(key_exists($key,$row)){
                                        $executesqlarray[$query][$key]=$row[$key];
                                    }else{
                                        $kk = substr($key,1);
                                        if(key_exists($kk,$row)){
                                            $executesqlarray[$query][$key]=trim($row[$kk]);
                                        }else{
                                            $executesqlarray[$query][$key]=$value;
                                        }
                                    }
                                }
                            }
                            print '<span class="'.($this->transaction($executesqlarray,$debug)?'ok':'fail').'">test</span><br>';
                        }                                               
                    }
                     @$dbStmt->closeCursor();
                    if($info) print 'RENDER STOP ';
                }
            }catch (\PDOException $e) {
               $this->Modellog($e,'renderdata',__LINE__,$lastsql );
               @$this->DB->rollback();
               return false;
            }
       }
/**
 * GET_Explore()
 * @param mixed $templateLocation
 * @param mixed $templateName
 * @param mixed $view
 * @param mixed $sql
 * @param mixed $explore_ids  {[key=>val]} $accound_id=>accounddata   -> %accounddata->value%
 * @param mixed $expore_overwrite
 * @param mixed $param {null}
 * @param bool $images {}
 * @param integer $imgaccound_id {0 = public} access to view image
 * @param string $imgprefix {''} no startprefix see imagecollection name ...  prefix_location
 * @return
 */
       protected function GET_Explore($templateLocation,$templateName,&$view,$sql,$explore_ids,$explore_overwrite,$param=null,$images=false,$imgaccound_id=0,$imgprefix=''){
			try{            
                if(\autosystem\ValidateAs::query($sql)&&($dbStmt = $this->DB->prepare($sql))AND
                $dbStmt->execute($param)){//print $this->paginator->getsql();
                    $res = $dbStmt->fetchAll(\PDO::FETCH_ASSOC);
                    $i=0;
                    foreach ($res as $row){
                        foreach($row as $k =>$v){
                        	if($this->DEBUG){
								 print('['.$k.'--'.$v.']'.((key_exists($k,$explore_ids))?'TRUE':'FALSE')."'\n"); 
						    }
							if(key_exists($k,$explore_ids)&& $v>0){
								if($explore_ids[$k]=='modeltypes'){
									if($this->DEBUG){
								 		print('modeltypes'); 
						    		}
									$row[$explore_ids[$k]] = $this->explorer_modeltypes($explore_ids[$k] , $k ,$v,$view,$explore_overwrite);
								}else{
									$row[$explore_ids[$k]] = $this->explorer_id($templateLocation,$templateName.'_'.$explore_ids[$k],$k , $k ,$v,$view,$explore_overwrite,$imgaccound_id,$imgprefix);
								}
                        	}else{
                        	   if(key_exists($k,$explore_ids)){
                        	       $row[$explore_ids[$k]] = '';
                        	   }
							}
						}
						$row['row']= ($i++%2==0)?'equally':'odd';
						$hsize=100;
						$wsize=100;
                        if($images){
                            $row['images'] = $this->getImages($imgprefix.$templateLocation,$row['id'],$imgaccound_id,$view);
                            $row['thumb'] = $this->getImages($imgprefix.$templateLocation,$row['id'],$imgaccound_id,$view,'A_THUMP');
                        }
                        print $view->get_Templateclone($templateLocation,$templateName,$row);
                    }
                    @$dbStmt->closeCursor();
                    return true;     
                }else{
                    return false;
                }
            }catch(\PDOException $e){
                $this->Modellog($e,'GET_Explore@'.$templateLocation.'->'.$templateName,__LINE__,$sql .$this->paginator->getsql());
                return false;
            }catch(Exception $e){
                $this->Modellog($e,'GET_Explore@'.$templateLocation.'->'.$templateName,__LINE__,$sql .$this->paginator->getsql());
                return false;
            }
       }
       /**
 * GET_ExploreR() reverse
 * @param mixed $templateLocation
 * @param mixed $templateName
 * @param mixed $view
 * @param mixed $sql
 * @param mixed $explore_ids  {[key=>val(returnids))]} $accound_id=>accounddata   -> %accounddata->value%
 * @param mixed $expore_overwrite
 * @param mixed $param {null}
 * @param bool $images {}
 * @param integer $imgaccound_id {0 = public} access to view image
 * @param string $imgprefix {''} no startprefix see imagecollection name ...  prefix_location
 * @return
 */
       protected function GET_ExploreR($templateLocation,$templateName,&$view,$sql,$explore_vals,$explore_overwrite,$param=null,$images=false,$imgaccound_id=0,$imgprefix=''){
			try{            
                if(\autosystem\ValidateAs::query($sql)&&($dbStmt = $this->DB->prepare($sql.$this->paginator->getsql()))AND
                $dbStmt->execute($param)){//print $this->paginator->getsql();
                    if(is_array($param)){
                        $returnIds=$param;
                    }else{
                        $returnIds=[];
                    }
                    $returnlabel = $this->getReturnlabel($sql);
                    $res = $dbStmt->fetchAll(\PDO::FETCH_ASSOC);
                    $i=0;
                    foreach ($res as $row){
                        if(!empty($returnlabel)){
                            $returnIds[':'.$returnlabel]=$row[$returnlabel];
                        }
                        if($this->isdatasplitter(array_keys($row))){
                            $hassplitter = true;
                        }else{
                            $hassplitter = false;
                        }                        
                        foreach($row as $k =>$v){
                            if($hassplitter){
                                if($splitter =  $this->getSpliter($k)){
                                    if($splitter !== false){
                                        $row = array_merge($splitter->renderSplit($v),$row) ;
                                    }
                                }
                            }
                        	if($this->DEBUG){
								 print('['.$k.'--'.$v.']'.((array_search($k,$explore_vals))?'TRUE':'FALSE')."'\n"); 
						    }
                            
                            //$tpl = array_search($k,$explore_vals);//return key of array
                            foreach($explore_vals as $tpl => $key){
                                if($k == $key && $v>0){
                                    if($this->DEBUG){
								        print('[template:'.$templateName.'_'.$tpl.'-->'.$v.']'.((array_search($k,$explore_vals))?'TRUE':'FALSE')."'\n"); 
						            }
                                    //print $tpl.' => '.$key;
    								if($tpl=='modeltypes'){ 
    									if($this->DEBUG){
    								 		print('modeltypes'); 
    						    		}
    									$row[$tpl] = $this->explorer_modeltypes($tpl , $k ,$v,$view,$explore_overwrite);
                                    }else{
    									$row[$tpl] = $this->explorer_id($templateLocation,$templateName.'_'.$tpl,$tpl, $k ,$v,$view,$explore_overwrite,$imgaccound_id,$imgprefix);
    								}
                            	}else{
                            	   if($k == $key){
                            	       $row[$tpl] = '';
                            	   }
    							}
                            }
							
						}
						$row['row']= ($i++%2==0)?'equally':'odd';
						$hsize=100;
						$wsize=100;
                        if($images){
                            $row['images'] = $this->getImages($imgprefix.$templateLocation,$row['id'],$imgaccound_id,$view);
                            $row['thumb'] = $this->getImages($imgprefix.$templateLocation,$row['id'],$imgaccound_id,$view,'A_THUMP');
                        }
                        print $view->get_Templateclone($templateLocation,$templateName,$row);
                    }
                    @$dbStmt->closeCursor();
                    return true;     
                }else{
                    return false;
                }
            }catch(\PDOException $e){
                $this->Modellog($e,'GET_Explore@'.$templateLocation.'->'.$templateName,__LINE__,$sql .$this->paginator->getsql());
                return false;
            }catch(Exception $e){
                $this->Modellog($e,'GET_Explore@'.$templateLocation.'->'.$templateName,__LINE__,$sql .$this->paginator->getsql());
                return false;
            }
       }
       private function explorer_modeltypes($key ,$id ,$idvalue,&$view,&$overwrite){
			 try{
			 	$u = \autosystem\DBConn::getTableNameFor('user');
            	$s = \autosystem\DBConn::getTableNameFor('user_rullset'); 
				$sql='SELECT `'. $s.'`.*,`'.$u.'`.`id` AS `user_id` FROM `'. $u.'`'; 
				$sql.=' LEFT JOIN `'.$s.'` ON `'.$u.'`.`rule_id`=`'. $s.'`.`rule_id` WHERE `'.$u.'`.`accound_id`=:accound_id' ;          
                if(($dbStmt = $this->DB->prepare($sql))&& $dbStmt->execute([':'.$id=>$idvalue])){
                    $res = $dbStmt->fetchAll(\PDO::FETCH_ASSOC);
                    $i=0;
                    $return = '';
                    $x=[];
                    $empty['TYPE']='';
					$empty['is']='';
					$empty['user_id']=0;
                    foreach($res as $row){
                    	$empty['user_id']= $row['user_id'];
                    	$x[$row['TYPE']]['TYPE']=$row['TYPE'];
						$x[$row['TYPE']]['is']=' checked=checked ';
						$x[$row['TYPE']]['user_id']=$row['user_id'];
					}
					foreach(array_keys($this->modeltypes)as $type){
						if(key_exists($type,$x)){
							$return .= $view->get_Templateclone('global','modeltype_item',$x[$type]);
						}else{
							$empty['TYPE']=$type;
							$return .= $view->get_Templateclone('global','modeltype_item',$empty);
						}
					}
                    @$dbStmt->closeCursor();
                    return $return;     
                }else{
                    return false;
                }
            }catch(\PDOException $e){
                $this->Modellog($e,'GET_Summary@'.$templateLocation.'->'.$templateName,__LINE__,$sql .$this->paginator->getsql());
                return false;
            }catch(Exception $e){
                $this->Modellog($e,'GET_Summary@'.$templateLocation.'->'.$templateName,__LINE__,$sql .$this->paginator->getsql());
                return false;
            }
	   }
       private function explorer_id($location ,$template , $key ,$id ,$idvalue,&$view,&$overwrite,$imgaccound_id=0,$imgprefix=''){
			//print_r($overwrite);
            if(key_exists($key,$overwrite)){
				$sql= $overwrite[$key];
			}else{
				print 'not overwrite';
				/*$tablename = \autosystem\DBConn::getTableNameFor('install_explorer_table');
				$sql = 'SELECT `query` FROM `'.$tablename.'` WHERE `id`=:id';
       			$param = ['id'=>$id];
       			$Esql = $this->GET_Stmt($sql,$param);
       			if(key_exists('query',$Esql)){
					$sql = $Esql['query'];
					$location  = $Esql['location'];
				    $template = $Esql['template'];
				}*/
                
                if($sql==''){
                    if($this->DEBUG){
    					return '[overwrite error '.__FILE__.__LINE__.']';
    				}
                    return '[overwrite error]';
                }
			}
			if($sql==''){				return '';			}
			$param =[':'.$id=>$idvalue];
            //print '**'.$sql;
			return $this->GET_Qsummary($location,$template,$view,$sql,$param,true,$imgaccound_id,$imgprefix,true);
	   }
       public function addSplitter(string $row, \data\DataSplitAR_interface $object){
            if(!is_object($object)|| is_subclass_of($object,'DataSplit_preg')){return[];}
            $this->regexSpliters[$row]=$object;
       }
       public function isdatasplitter($rowkeys){
            //is rowkey used as return throw error
            return count(array_intersect(array_keys($this->regexSpliters),$rowkeys));
       }
       public function getSpliter($rowname){
            if(key_exists($rowname,$this->regexSpliters)){
                 return $this->regexSpliters[$rowname];
            }else{
                 return false;
            }
       }
       public function pregRowdataSplit($rowname,$rowdata){
            return $this->regexSpliters[$row]->renderSplit($rowdata);
       }
       private function getReturnlabel(&$sql){
             if(preg_match('/RETURNING `(.*)`/i',$sql,$idlabel)||preg_match('/RETURNING (.*)/i',$sql,$idlabel)){//([0-9A-Za-z_])
                $start = strpos($sql,'RETURNING');
                 if($start !=0 ){
                    $sql = substr($sql,0,$start);  
                 }
                 return $idlabel[1];
             }else{
                return false;
             }
       }
       public function killcon(){
             @$this->DB = null;
       }
       public function __call($methodename,$parameters){//*** ALL NOT EXIST METHODES WIL USE AUTOQUERYSYSTEM
           $methode = explode('_',$methodename);
		   //print_r($methode);
           if(!is_array($methode)){
                $e='not error(methode not exist)§'.$methodename;
                $this->Modellog($e,'model:__call@','LINE'.__LINE__,'no sql'); 
               return false;
           }
           $action = array_shift($methode);
		   $tablename = \autosystem\DBConn::getTableNameFor(implode('_',$methode)) ;
           if(empty($tablename) || !$this->existTableLabel($tablename)|| key_exists($tablename,$this->protectables)){
			   $e='not error(methode not exist)§'.$methodename;

               $this->Modellog($e,'model:__call->'.print_r($methode).'table'.$tablename,'LINE'.__LINE__,'no sql');
               return false;
		   }
           
		   $fields = (is_array($parameters))?$parameters[0]:false;
           $debug = false;
		   switch($action){
                case 'GETID':
                	$sql = 'SELECT `id` FROM `'.$tablename.'` WHERE ';
                	//print $key;
					$param =[];
					foreach($fields as $k=>$value){
						$sql .= '`'.$k . '`= :' .$k.' ';
						$param[':'.$k]=''.$value;
					}
					
					$return = $this->GET_Stmt($sql,$param);
					return $return['id'];
					break;
				case 'GET':
			        $sql = 'SELECT * FROM `'.$tablename.'` WHERE ';
                    $param =[];
                    $where = '';
                    $m='';
					foreach($fields as $k=>$value){
						$where .= $m.'`'.$k . '`= :' .$k.' ';
						$m = 'AND ';
						$param[':'.$k]=''.$value;
					}
					return $this->GET_Stmt($sql.$where,$param);
					break;
				case 'CHANGE':
				     if(key_exists('keys', $fields)){
						$key= 'UPDATE `'.$tablename.'`';
						$set = ' SET';
						$where = ' WHERE';
                        $bind= '';
                        $bbind= '';
						$values=[];
						foreach($fields as $k=>$v){
							if($k !== 'keys'){
								if(in_array($k,$fields['keys'])){
									$where .= $bind.' `'.$k.'`=:'.$k;    $bind= ' AND';
								}else{
									$set .= $bbind.' `'.$k.'`=:'.$k ;    $bbind= ',';
								}
								$values[':'.$k]=''.$v;
							}
						}
						$sql[$key.$set.$where]= $values;
						//print_r($sql);
                        return $this->transaction($sql,$debug);					
					 }
					return false;	
					break;
				/**
				SAVE_table_name($where['key'=>value])  !id implement
				*/
				case 'INSERT':
                case 'SAVE':
                    $key = 'INSERT INTO `'.$tablename.'`(`id`,`'.implode('`,`',array_keys($fields)).'`) VALUES(NULL,:'.implode(',:',array_keys($fields)).')';
					$values =[];
					foreach($fields as $k=>$v){
						$values[':'.$k]=''.$v;
					}
					$sql[$key]= $values;
					//print($key)
					return $this->transaction($sql,$debug);
                	break;
                case 'EXPLOIRE'://not working
                    /*$face = $this->GetRegistratedFace($tablename);
					$key = 'SELECT * FROM `'.$tablename.'` WHERE ';
					$param =[];
	                $where = '';
	                $m='';
					foreach($fields as $k=>$value){
						$where .= $m.'`'.$k . '`= :' .$k.' ';
						$m = 'AND ';
						$param[':'.$k]=''.$value;
					}
					$result = $this->GET_Stmt($sql.$where,$param);
					$values =[];
					foreach($result as $k=>$v){
						$sub = substr($k, -3, 3);
						if($sub=='_id' && $face = $this->GetIdRegistration($k)){
							$face['tablename'];
							$face['template'];
							$param =[];
							$key = $face['query'];
						}
					}
					$sql[$key]= $values;
					
					$this->GET_Explore($templateLocation,$templateName,&$view,$sql,$explore_ids,$explore_overwrite,$param=null,$images=false,$imgaccound_id=0,$imgprefix='');
                	*/
					break;
              case 'CROIXCHANGE':
                     //***EXIST***
                    $sql = 'SELECT * FROM `'.$tablename.'` WHERE ';
                    $param =[];
                    $bind = '';
					foreach($fields as $k=>$value){
						$sql .= $bind.'`'.$k . '`= :' .$k.' ';
						$param[':'.$k]=''.$value;
                        $bind = 'AND';
					}
                    //print $sql;
                    //print \autosystem\DEBUG::printPDO($sql,$param);
					//print $this->isQuery($sql,$param)?"\n".'exist TRUE':"\n".'exist FALSE';
              case 'INSERTNOID':      
                    $exist = !empty($sql)&&$this->isQuery($sql,$param);
                    //*** NO -> INSERT
                    if(!$exist){
                        $key = 'INSERT INTO `'.$tablename.'`(`'.implode('`,`',array_keys($fields)).'`) VALUES(:'.implode(',:',array_keys($fields)).')';
    					$values =[];
    					foreach($fields as $k=>$v){
    						$values[':'.$k]=''.$v;
    					}
                        //print$key;
    					$esql[$key]= $values;
    					
    					return $this->transaction($esql,$debug);
                        break;  
                    }
                    //*** SEE NO BREAK SO I FALLING TO DELETE
				case 'UNBIND':
				case 'DELETE':
					$Dkey = 'DELETE FROM `'.$tablename .'` WHERE ';
                    $values =[];
                    $m='';
					foreach($fields as $k=>$value){
						$Dkey .= $m.'`'.$k . '`= :' .$k.' ';
						$m = 'AND ';
						$values[':'.$k]=''.$value;
					}
					$Dsql[$Dkey]= $values;
					return $this->transaction($Dsql,$debug)!==false;
                    break; 
				case 'SUMSQL':
					    $sql = 'SELECT * FROM `'.$tablename.'` ';
						$where = 'WHERE ';
						return $sql;
                    break;
                /**
				ASUMY_table_name($where['key'=>value])
                       $data= ['page'=> $this->_values['page'],'location' => $this->_loc,
                        'templatename' => $this->_action,
                        'view' => $this->_view, 
                        'accound_id'=>  $this->getAccound_id(),
                        'params'=>  ['disksummary']];
				*/
                case 'ASUMY':
                        if(key_exists('page', $fields)
                        && key_exists('location', $fields) 
                        && key_exists('templatename', $fields)
                        && key_exists('view', $fields)
                        && key_exists('accound_id', $fields)
                        && key_exists('params', $fields)
                        ){  
                            $this->paginator->setRange($this->defaultPageRange);
                            $this->paginator->setpagenr($fields['page']);
                            $this->paginator->setpaginatorreload($fields['location'],$fields['templatename']);
                            $sql= 'SELECT * FROM `'.$tablename.'` ';
                            $len =$this->GET_COUNT($sql);
                            
                            $this->paginator->setlistlength((int)$len);
                            $this->GET_Summary($fields['location'],$fields['templatename'],$fields['view'],$sql,null,true,$fields['accound_id']);
                            $fields['view']->getpaginator($this->paginator->getdata(),'&'.implode('&',$fields['params']),7);
                        }
                        
                    break;
                /**BIND_table_name($where['key'=>value])*/
                case 'BIND':
                    $key = 'INSERT INTO `'.$tablename.'`(`'.implode('`,`',array_keys($fields)).'`) VALUES(:'.implode(',:',array_keys($fields)).')';
                    $values =[];
					foreach($fields as $k=>$value){
						$values[':'.$k]=''.$value;
					}
					$sql[$key]= $values;
					return $this->transaction($sql,$debug);
                    break;
                case 'UPDATE':
				    //where issue to check how todo param where?
					/*$key = 'UPDATE `'.$tablename.'`(`id`,`'.implode('`,`',array_keys($fields)).'`) VALUES(NULL,:'.implode(',:',array_keys($fields)).')';
					$values =[];
					foreach($fields as $k=>$v){
						$values[':'.$k]=''.$v;
					}
					$sql[$key]= $values;
					return $this->transaction($sql);*/
					break;	
                /**
                $groups = $this->_model->ALIST_catalog_product_groups($x);
                $x= ['key' =>'id','value' => 'name'];
                */    
                case 'ALIST':
	                if(key_exists('key', $fields) && key_exists('value', $fields)){
							$key='SELECT `'.$fields['key'].'` AS \'key\' ,`'.$fields['value'].'` AS \'value\' FROM `'.$tablename.'`' ;
							$where = '';
							$c = ' WHERE ';
							$values=[];
							foreach($fields as $k=>$v){
								if($k !== 'key' && $k !== 'value'){
									$where .= $c.'`'.$k.'`=:'.$k;
									$values[':'.$k]=''.$v;
									$c=' AND ';
								}
							}
                            
							$sql =$key.$where ;
							return $this->GET_Array($sql,$values);					
						 }
						 return false;
                break;
				case 'FIND':                
                    return print 'SELECT * FROM '.$tablename.'WHERE `'.implode('`AND`',array_keys($fields)).'`';break;
                case 'COUNT':
                    $sql = 'SELECT COUNT(*) AS `count` FROM '.$tablename;
                    $param =[];
                    $link = ' WHERE ';$where='';
					foreach($fields as $k=>$value){
						$where .= $link.' `'.$k . '`= ' .$value;
						$link=' AND';
					}
					return $this->GET_COUNT($sql.$where);
					break;
				case 'EXISTTABLE':
                    return $this->existTableLabel($tablename); 
	   				break;
                
                case 'IS':
				/**
				EXIST_table_name($where['key'=>value])
				*/	
                case 'EXIST':
                	$sql = 'SELECT * FROM '.$tablename.' WHERE ';
                    $param =[];
					foreach($fields as $k=>$value){
						$sql .= '`'.$k . '`= :' .$k.' ';
						$param[':'.$k]=''.$value;
					}
				   //print($sql);
					//print_r($param);
					return $this->isQuery($sql,$param);
	   				break;
				case 'REPORTSUPPORT':
			        $ticket = new \support\Ticket();
			        $location_id=0;
			        $accound_id=0;
			        $url=(key_exists('url', $fields)?$fields['url']:'');
			        $msg=(key_exists('msg', $fields)?$fields['msg']:'');
			        $id=(key_exists('id', $fields)?$fields['id']:'');
			        $nr = $ticket->create($id,$id.'__CALL-REPORT:'.$action,'',$msg,$url,$location,$accound_id,$location_id);
                case 'SHOW':
                    print '<div class="ERROR" >tablename'.$tablename.'['.(($this->existTableLabel($tablename))?'':'not').'exist ]';
                    foreach($fields as $k=>$v){
						print":$k=$v <br>";
					}
                    print '</div>';
                default:
                    $e = '---"'.$methode.'""'.$methodename.'"--- methode not exist';
                     global $_DEBUG;
            			if($_DEBUG){
            			  print'<H1>'.$tablename.'</H1>';
            				print $action;
            				print_r($fields);
							print  '<div class="ERROR" >choice: REPORTSUPPORT,(EXIST,IS),COUNT,FIND,UPDATE,BIND(UNBIND,DELETE),SAVE,CHANGE,GET,GETID</div>';
						}
                    $this->Modellog($e,'model:__call@'.$methodename,__LINE__,$sql);
           }            
       }
       public function __destruct(){
            @$this->DB= null;
            @$this->UserRulles= null;
       }       
       protected function getIp() {
            $ip = htmlspecialchars($_SERVER['REMOTE_ADDR']);
            if (!empty($_SERVER['HTTP_CLIENT_IP'])){
                $ip = $_SERVER['HTTP_CLIENT_IP'];
            } elseif (!empty($_SERVER['HTTP_X_FORWARDED_FOR'])) {
                $ip = $_SERVER['HTTP_X_FORWARDED_FOR'];
            }
            return $ip;
        }
       /**
        * Model::nowDateTime()
        * @return
        */
       public function nowDateTime(){
            return date( 'Y-m-d H:i:s', time());
       }
       protected function GetIdRegistration($idkeyname){
            return false;
       }
       protected function findDublicates($templateLocation,$templateName,&$view ,$tlbl,$colum,$filters=[]){
            $sql =  'SELECT a.*,count(a.`id`) as `e`, count(b.`'.$colum.'`) as `count` FROM `'.$tlbl.'` as a '.
                    ' LEFT JOIN `'.$tlbl.'` as b on a.`'.$colum.'`= b.`'.$colum.'` ';
            if(is_array($filters)&& count($filters)>0){
                $sql .=  'WHERE ';
                $ref = '';
                foreach($filters as $filter){
                    $sql .= $ref .' a.`'.$filter.'` = b.`'.$filter.'`';
                    $ref = ' AND ';
                }
            }         
            $sql .=  ' GROUP BY b.`'.$colum.'`,a.`id` HAVING `count` > 1;';
            try{               
                if(\autosystem\ValidateAs::query($sql)&&($dbStmt = $this->DB->prepare($sql))&&$dbStmt->execute()){
                    $res = $dbStmt->fetchAll(\PDO::FETCH_ASSOC);
                    $i=0;
                    foreach ($res as $row){
                        $row['row']= ($i++%2==0)?'equally':'odd';
                        print $view->get_Templateclone($templateLocation,$templateName,$row);
                    }
                    @$dbStmt->closeCursor();
                    return true;     
                }else{
                    return false;
                }
            }catch(\PDOException $e) {          $this->Modellog($e,'findDublicates',__LINE__,$sql );
                return false;
            }
       }
       private function tablelenght(){
           /*$dbSql='SELECT 1 FROM persoon';//simples voor controle lengte
           $dbStmt = \Db::GET()->prepare($dbSql);$dbStmt->execute();
           $aantal = $dbStmt->rowCount();*/
           return current($this->DB->query('SELECT COUNT(*) FROM table')->fetch());
       }
       protected function getRullesSQL($table, $rulles){
            $link = ' OR ';
            $return ='(';
            if(is_array($rulles)){
                foreach($rulles  as $rull){       $table.'=\''.$rull .'\'';        }    
            }
            $return.=' )';
            return $return;
       }
       /**
        * Model::getImages()
        * abouse if no image is found "images_category.`TYPE`='PUBLIC'"
        * @param mixed $categoryname
        * @param mixed $dest_id
        * @param mixed $accound_id NO ID 
        * @return
        */
       private function getImages($categoryname,&$dest_id,&$accound_id,$view,$sizelabel=''){
            if(empty($dest_id)){  return;  }
            if(!is_numeric($dest_id)||$dest_id==0){
                throw new Exception('MODEL Images ?dest_id: NOT EXIST '.__file__. __line__);
            }
            $return='';
            $h;$w;$alt;
            $images = $this->getImageArray($categoryname,$dest_id,$accound_id);
            foreach ($images as $id=>$value){
                $local = $this->installpath.'';//$this->imgups.'';
                $this->loadimageinfo($h,$w,$alt,$value,$sizelabel);
                //print '------'.$this->imgups.'-------';
                if($sizelabel=='A_THUMP'){
                    $p = explode('/',$value);
        		    $filename = array_pop($p);
                    if($sizelabel=='A_THUMP' && is_file('./'.implode('/',$p).'/thumb_'.$filename)){
                        $return.= '<img src="'.$local.implode('/',$p).'/thumb_'.$filename.'" alt="'.$alt.'"  >';
                    }else{
                        $return.= '<img src="'.$local.$value.'" alt="'.$alt.'" width="'.$w.'"  >';
                    }
                }else{
                    $return.= '<img src="'.$local.$value.'" alt="'.$alt.'" width="'.$w.'"  >';
                }
            }
            return $return;
       }
       private function loadimageinfo(&$h,&$w,&$alt,$name,$sizelabel){
            if($sizelabel==''){
                $h=500;
		        $w=500;
                $alt='';
            }
            $loadsize = $this->resizable;
            if($this->existTableLabel('images_size')){//size to view
                $this->GET_Stmt('SELECT * FROM '.\autosystem\DBConn::getTableNameFor('images_size'),$param,$maxres=0);
                $h=150;
		        $w=150;
                $alt='';
            }
            if($sizelabel=='A_LIST'){
                $h=300;
		        $w=300;
                $alt='';
            }
            if($sizelabel=='A_THUMP'){
                $h=200;
		        $w=200;
                $alt='';
            }       
       }
	   private function loadimagedata(&$data,$fromresource='disk',$encript=false){
		
	   }
       private function isAccound_id($id){
            return empty($accound_id)||!is_numeric($accound_id)||$accound_id==0;
       }
       protected function hasSevireAccess($accound_id,$location,$action,$SevireOption){
            //TODO: need done by upgrade 
            return true;
       }
       public function numRows($result){
        /*$dbStmt = $this->DB->prepare('');
        pdo
		return $dbStmt->W
        mysqli_num_rows($result);*/
	   }
       private function getAccessSql($accound_id){
            $hasNoUserid = $this->isAccound_id($accound_id);
            $p = \autosystem\DBConn::getTableNameFor('images_path');
            $sql='';
            if($hasNoUserid){
                if(isset($this->UserRulles)){
                    $sql .='AND (';
                    $ref = '';
                    foreach($this->UserRulles as $key => $rull){
                        $sql .= $ref.'`images_category`.`TYPE`=\''.$rull.'\'';           $ref = ' OR ';
                    }
                    $sql .=')';
                }else{
                    $sql .=' AND `images_category`.`TYPE`=\'PUBLIC\'';
                }
            }else{
                if(isset($this->UserRulles)){
                    $sql .='AND ('.$p.'.`accound_id`=\''.$accound_id.'\'';               $ref = ' OR';
                    foreach($this->UserRulles as $key => $rull){
                        $sql .= $ref.' `images_category`.`TYPE`=\''.$rull.'\'';
                    }
                    $sql .=')';
                }else{
                    $sql .=' AND `'.$p.'`.`accound_id`=\''.$accound_id.'\'';
                }   
            }
            return ($sql == 'AND ()')?'':$sql;
       }
       public function getImage(&$categoryname,&$dest_id,&$accound_id,$namedkey){
            return $this->getImageArray($categoryname,$dest_id,$accound_id,$namedkey);
       }
       public function getSystemIMG(&$dest_id,$categoryname='Amail'){//Mail
            $p = \autosystem\DBConn::getTableNameFor('images_path');
            $c = \autosystem\DBConn::getTableNameFor('images_category');
            $b = \autosystem\DBConn::getTableNameFor('images_bind');
            $sql = 'SELECT namekey,images_id,urlto,PATHIMG FROM '.$p. 
                   ' LEFT JOIN `'.$b.'` ON `'.$b.'`.`images_id` = '.$p.'.`id` AND `dest_id`='.$dest_id.' '.
                   ' LEFT JOIN `'.$c.'` ON `'.$c.'`.`id` = '.$p.'.`category_id`'.
            ' WHERE `dest_id` IS NOT NULL AND `'.$c.'`.`label`=\''.$categoryname.'\' ';
            $return =[];
            if(\autosystem\ValidateAs::query($sql)&&($dbStmt = $this->DB->prepare($sql))&&$dbStmt->execute()){
                $res = $dbStmt->fetchAll(\PDO::FETCH_ASSOC);
                foreach ($res as $row){
                    $path= $row['PATHIMG']=='TRUE';
                    $key=($namedkey)?$row['namekey']:$row['images_id'];
                    $return[$key] = ($path)? $row['urlto']:'data:'.'[error]';
                } 
            }
            return $return;
       }
       private function getImageArray($categoryname,$dest_id,$accound_id,$namedkey=false){
            $p = \autosystem\DBConn::getTableNameFor('images_path');
            $c = \autosystem\DBConn::getTableNameFor('images_category');
            $b = \autosystem\DBConn::getTableNameFor('images_bind');
            $sql = 'SELECT namekey,images_id,urlto,PATHIMG FROM `'.$p. 
                   '` LEFT JOIN `'.$b.'` ON `'.$b.'`.`images_id` = `'.$p.'`.`id` AND `dest_id`='.$dest_id.
                   ' LEFT JOIN `'.$c.'` ON `'.$c.'`.`id` = `'.$p.'`.`category_id`'.
                   ' WHERE `dest_id` IS NOT NULL AND `'.$c.'`.`label`=\''.$categoryname.'\' ';                
            $sql.=$this->getAccessSql($accound_id); 
            //print $sql;
            $return =[];
            if(\autosystem\ValidateAs::query($sql)&&($dbStmt = $this->DB->prepare($sql))&&$dbStmt->execute()){
                $res = $dbStmt->fetchAll(\PDO::FETCH_ASSOC);
                foreach ($res as $row){
                    $path= $row['PATHIMG']=='TRUE';
                    $key=($namedkey)?$row['namekey']:$row['images_id'];
                    $return[$key] = ($path)? $row['urlto']:'data:'.'[error]';//$row['urlto'];
                }     
            }
            return $return;
       }
       private function Modellog(&$e,$from,$line,$sql){
            global $_DEBUG;
            if($_DEBUG){'<div class="xdebug-error" >'. (($e!==false)?print_r($e). \autosystem\DEBUG::printERR_B($e):'').'</div>';}
            //print '<div class="xdebug-error" >'.$from.' '.$sql.'  LineNr'.$line.' FROM model : '.__LINE__;
            //print '</div>';
            $msg = 'on: '.date('M j Y - G:i:s').','.get_called_class().' model '.__file__.' :'.$from.' pdoexcept:'.(is_object($e)&&method_exists($e,'getMessage')?$e->getMessage():$e).PHP_EOL;
            $msg .= $sql;
			file_put_contents('./log/dberror.log', $msg, FILE_APPEND);
            return 'ok';      
       }
       private function PDOExceptionCRL(&$e,$forward=false){
            $this->Modellog($e,'get_array',__line__);
            $this->setInstallSTATUS('ERROR');
            if(!$forward){die('i eat error');}
       }
       protected function getENUM($key,$check){
            return ' `'.$key.'`='.((isset($check)&& $check!='true')?' \'FALSE\' ':' \'TRUE\' ');
       }
       protected function getmodeltypes(){//TODO Replace $this->modeltypes to $this->getmodeltypes (upgrade database)
          if(is_array($this->modeltypes)){
            return $this->modeltypes;
          }
          return [];
       }
       public function setInERROR($hint,$e,$modelinfo,$linenr = 0){
            global $_DEBUG;
            if($_DEBUG){print '<h1>E</h1>ERROR writeBy Model '.__LINE__.' '.$modelinfo.' '.$e;}
            $this->setInstallSTATUS('ERROR',$modelinfo,$e);
            $this->Modellog($e,$hint.$modelinfo,$linenr,'NO SQL setIn ERROR remote');
       }
       protected function DBtableadd(){
            $sql = 'ALTER TABLE `catalog_product_groups` ADD COLUMN `groupinfo` TEXT NULL COLLATE \''.\autosystem\DBConn::getCOLLATE().'\' AFTER `name`;';
       }
       private function setInstallSTATUS($STATUS,$modelinfo,$e){ 
            try{
               $i = \autosystem\DBConn::getTableNameFor('installs');
               $this->DB->beginTransaction();
               $sql = 'UPDATE `'.$i.'` SET `STATUS`=\''.$STATUS.'\' WHERE `key`= \''.$this->getModelInfo().'\'';
               if(($dbStmt = $this->DB->prepare($sql)) //
                   &&$dbStmt->execute($param)){
                    @$dbStmt->closeCursor();
                    return $this->DB->commit();
               }else{
                    $this->DB->rollback();
                    return false;
               }
            }catch (\PDOException $ee){           @$this->DB->rollback();
                $this->Modellog($ee,'setInstallSTATUS',__LINE__,'---');
               return false;
            }
       }
}
?>