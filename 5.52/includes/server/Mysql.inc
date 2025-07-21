<?php
/**
* @author Lieven Roegiers
* @copyright 
* @CMS autosite
*/
	namespace server;
    //READSQL (backup) and ingore if exist check if equals
    //make sql backup and split by table maximal lines , filesize ...
    //create binding by autobinding
    //readtable and go to modulecreator ...
    class Mysql extends \autosystem\Controller{
        private $values=['titlemysql','databases', 'not availeble','detail','import','overview','detail','PAGENOTFOUND'];
        private $categorys;
        public function init() {
           if(isset($_POST['follow'])){
                $follow = $_POST['follow'];
                if(\autosystem\ValidateAs::isCheckedOrNot($follow)){
                    $this->addTofav(['view'=>$_GET['follow']]);
                    $result = ['ok'=>true];
                    header('HTTP/1.0 200 ');print json_encode($result);
                    return;
                }
                header('HTTP/1.0 504 Your To Slow for me');
                return ;
           }
           $this->getModel();
           $this->vars();
           $this->prepare($this->_loc,$this->_action,$this->_values);
        }
        private function prepare($loc,$action,$values){
           $GETEVERYTHINGELSE = true;
           print $this->_view->getheader($loc,$action);
           print $this->_view->get_template($this->_loc,$this->_action,$this->_values);
           print $this->_view->get_template($this->_loc,'Mysql_menu_options',$this->_values);
           if(isset($this->_values['view'])){
                if(!$this->_model->GET_ITEM($this->_loc,'MysqlDetail',$this->_view,$this->_values['view'])){
                    $array['id'] = 'info';
                    $array['title'] = 'ERROR: '.$this->_values['PAGENOTFOUND'];
                    print $this->_view->get_template('global','section_head',$array);
                    print $this->_view->get_template($this->_loc,'HTTP404');
                    print $this->_view->get_template('global','section_foot');
                }
                $GETEVERYTHINGELSE = false;
           }
           if(isset($this->_values['prepareforplugin'])){
                    $array['id'] = 'info';
                    $array['title'] = 'ERROR: ';
                    print $this->_view->get_template('global','section_head',$array);
                    print $this->_view->getform_head('cms','plugin');
       				print $this->_view->get_template('cms','plugin_createitem_basic_one',$this->_values);
       				print $this->_view->get_template($this->_loc,'plugin_createitem_basic_one',$this->_values);
                    print $this->_view->get_template('global','section_foot');
                $GETEVERYTHINGELSE = false;
           }
           if(isset($this->_values['summary'])){
                $array['id'] = 'info';
                $array['title'] = '';
                if(isset($this->_values['category'])&& $this->_values['category']>0){
                   $array['id'] = 'info';
                   if(key_exists($this->_values['category'],$this->categorys)){
                        $array['title'] .= ' '.$this->_values[$this->categorys[$this->_values['category']]];
                   }else{
                        $array['title'] .= ' find category';
                   }
                   print $this->_view->get_template('global','sectionv_head',$array);
                   print $this->_view->get_template($this->_loc,'item_head');
                   $this->_model->GET_AOverView($this->_loc,'MysqlItem',$this->_view,$this->_values['category'],'',true,'',$this->_values['page']);
                }else{
                   print $this->_view->get_template('global','sectionv_head',$array);
                   $this->_model->GET_AOverView($this->_loc,'MysqlItem',$this->_view,$this->_values['category'],'',true,'',$this->_values['page']);
                }    
                print $this->_view->get_template($this->_loc,'item_foot');
                print $this->_view->get_template('global','section_foot');
                $GETEVERYTHINGELSE = false;
           }
           if(isset($this->_values['sqlcategory'])){
                $array['id'] = 'categorys';
                $array['title'] = '';
                print $this->_view->get_template($this->_loc,'item_head',$array);
                $this->_model->GET_HelpCategorySummary($this->_loc,'MysqlHelpCategoryItem',$this->_view,$this->_values['category'],'',true,'',$this->_values['page']);
                print $this->_view->get_template($this->_loc,'item_foot',$array);
		   }
           if(isset($this->_values['e_query'])){
                    $this->status('test');  
           			if(isset($this->_values['database'])&&$this->_values['database']!=''){
           			    $array['id'] = 'query';
	                    $array['title'] = 'query field';
                            $this->_validator->addForm('query',['select','from','table' ],true,['']);
                            $this->_validator->addAlocat('query',['from'=>'istext']);
                            $aget = [];
                            $this->_validator->render('query',$_GET,$aget);
    		                $this->_validator->render('makequery',$_GET,$aget);
                            
                            $this->status('');
                        if($this->_validator->isValid('query')){

                            print_r($aget);
                            $this->_values['query']=  'SELECT '.$aget['select'].' FROM '.$aget['table'];
                        }else{
                            $this->_values['query']=  'SELECT '.$aget['select'].' FROM '.$aget['table'];
                            
                        }
	                    print $this->_view->getform_head($this->_loc,$this->_action);
	                    print $this->_view->get_template('global','section_head',$array);
	       				print $this->_view->get_template($this->_loc,'query',$this->_values);
	                    print $this->_view->get_template('global','section_foot');
	                    print $this->_view->getform_end('execute','execute');
	                    $GETEVERYTHINGELSE = false;
                    }else{
						$this->_values['databases']=true;
					}
           }
           if(isset($_POST['execute'])){                    
	       			print $this->_view->get_template($this->_loc,'mysql_excecutinfo',$this->_values);
                    print '<h2>'.$this->_values['query'].'</h2>';
                    print '<pre>';
           			if(isset($this->_values['database'])){
                      print $this->_values['query'];
                      $res = $this->_model->GET_queryresult($this->_values['database'],$this->_values['query']);
                      if(isset($_POST['resultto'])){
                            switch ($_POST['resultto']){ 
                            	case 'validator' :
                                    $dbinfo =  $this->getDbinfo($table,$res);
                                    $this->resultTo($dbinfo,'mysql_validator_src');
                            	break;
                                        
                            	case 'dbinstall':
                            	break;
                            	case 'functionpreset' :
                                    print 'print $this->getSectionform($this->_loc,$this->_action,\'adddrive\',$this->_values,\'adddrive\',\'\',\'\',\'adddrive\');';
                            	break;
                                case 'tableinfo' ://get all tables and collumninfo
            
                                   //print_r($this->_model->GET_TablesFromQuery($this->_values['query']));                	break;
                                case 'explore' ://get all tables and collumninfo$explorereturn = '       $sql= \''.$this->_values['query'].'  ;//SELECT * FROM '.$d_original.' WHERE ';  
                                   $explorereturn = '       $sql= \''.
                                    print $this->_values['query'].'\'';
                                   $cols = $this->_model->GET_CollumsQueryArr($this->_values['database'],$this->_values['query']);
                                   //$this->_model->GET_CollumsArr($this->_values['database'],$d_original);
                                   $subtemplates = '';
                                   foreach($cols as $key=> $col){
                                        //print_r($col);
                                        if( key_exists('Field',$col) && substr($col['Field'],-3)=='_id'){
                                            $table = substr($col['Field'],0,-3);
                                            $subtemplatename = 'subtpl_'.$table;
                                            /*if($this->_model->existtable($table)||($table = $this->_model_findtable($table)) ){
                                               break;
                                            }*/
                                            $subtemplates .= '\''.$subtemplatename.'s\' => \''.$col.'\', ';
                                            $explorereturn.= '  	$d'.$table.' = \autosystem\DBConn::getTableNameFor('.$table.');<br>';
                                            $explorereturn.= '$exploreoverwrite[\''.$subtemplatename.'\']=\'SELECT * FROM `\'.$d'.$table.
                                            '.\'` WHERE `id`=:'.$col.';//\'FROM `??` WHERE `id` NOT IN (SELECT `group_id` FROM `???` WHERE `???`=:??? );<br>'; 
                                        }
                                   }
                                   $explorereturn.= 'parent::GET_ExploreR($location,$templatename,$view,$sql,'.
                                   '['.$subtemplates.'],$exploreoverwrite,null,true,$account_id,\'\');';
                                   print $explorereturn;
                            	break;
                                case 'sqlfailuretracker' ://get all tables and collumninfo
                            	break;
                            	default :
                                    $this->_model->resultToTable($res ,$this->_view);
                                
                            }
                            print '</pre>';
                      }else{
                        $this->_model->resultToTable($res,$this->_view);
                      }       
                      $line='';
	                  $GETEVERYTHINGELSE = false;
                    }else{
						$this->_values['databases']=true;
					}
           }//   thetruemanshow.com
           if(isset($this->_values['code'])&&$this->_values['code']==''&&$this->_values['database']!=''){
              	$array['id'] = 'info';
	            $array['title'] = '[CODE]Qgenerate';
	            print $this->_view->get_template($this->_loc,'MysqlCODE_head',$array);
                print $this->_view->get_template($this->_loc,'Mysql_Code_start',['name'=>$n,]);
                //$this->_view->reSetTemplate($this->_loc,'MysqlTable_item',[],$this->_values);
	            $this->_model->GET_Collums($this->_loc,'MysqlCollum_item',$this->_view,$this->_values['database'],$this->_values['table']);
	            print $this->_view->get_template($this->_loc,'Mysql_Code_end',['name'=>$n,]);
                $n = 'add'.$this->_values['table'];
                print $this->_view->get_template($this->_loc,'Mysql_Code_start',['name'=>$n,]);
                print $this->_view->get_template($this->_loc,'Mysql_Code_form',['name'=>$n,'table'=>$this->_values['table'],]); 
                print $this->_view->get_template($this->_loc,'Mysql_Code_end',['name'=>$n,]);
                print $this->_view->get_template($this->_loc,'Mysql_Code_start',['name'=>$n,]);
                print $this->_view->get_template($this->_loc,'Mysql_Code_validator_Addform_start',['name'=>$n,]);
	            $this->_model->GET_Collums($this->_loc,'Mysql_Code_validator_Addform_element',$this->_view,$this->_values['database'],$this->_values['table']);
	            print $this->_view->get_template($this->_loc,'Mysql_Code_validator_Addform_end',['name'=>$n,]);
                print $this->_view->get_template($this->_loc,'Mysql_Code_validator_allocat_start',['name'=>$n,]);
	            $this->_model->GET_Collums($this->_loc,'Mysql_Code_validator_allocate_element',$this->_view,$this->_values['database'],$this->_values['table']);
	            print $this->_view->get_template($this->_loc,'Mysql_Code_validator_allocat_end',['name'=>$n,]);
	            print $this->_view->get_template($this->_loc,'Mysql_Code_validator_render',['name'=>$n,]);
                print $this->_view->get_template($this->_loc,'Mysql_Code_end',['name'=>$n,]);
                
                print $this->_view->get_template($this->_loc,'Mysql_Code_start',['name'=>$n,]);
                print $this->_view->get_template($this->_loc,'Mysql_Code_handel',['name'=>$n,'table'=>$this->_values['table'],]); 
                print $this->_view->get_template($this->_loc,'Mysql_Code_end',['name'=>$n,]);
                print $this->_view->get_template($this->_loc,'Mysql_Code_start',['name'=>$n,]);
                print $this->_view->get_template($this->_loc,'Mysql_Code_explore',['name'=>$n,'table'=>$this->_values['table'],]); 
                print $this->_view->get_template($this->_loc,'Mysql_Code_end',['name'=>$n,]);
                print $this->_view->get_template($this->_loc,'MysqlCODE_foot',$array);	            
	            $GETEVERYTHINGELSE = false;
           }
           if(isset($this->_values['code'])&&$this->_values['code']=='laravale'&&$this->_values['database']!=''){
              	$array['id'] = 'info';
	            $array['title'] = '[CODE]Qgenerate';
	            print $this->_view->get_template($this->_loc,'MysqlCODE_head',$array);
                print $this->_view->get_template($this->_loc,'Mysql_Code_start',['name'=>$n,]);
                //$this->_view->reSetTemplate($this->_loc,'MysqlTable_item',[],$this->_values);
	            $this->_model->GET_Collums($this->_loc,'MysqlCollum_item',$this->_view,$this->_values['database'],$this->_values['table']);
	            print $this->_view->get_template($this->_loc,'Mysql_Code_end',['name'=>$n,]);
                $n = 'add'.$this->_values['table'];
                print $this->_view->get_template($this->_loc,'Mysql_Code_start',['name'=>$n,]);
                print $this->_view->get_template($this->_loc,'Mysql_LCode_form',['name'=>$n,'table'=>$this->_values['table'],]); 
                print $this->_view->get_template($this->_loc,'Mysql_Code_end',['name'=>$n,]);
                print $this->_view->get_template($this->_loc,'Mysql_Code_start',['name'=>$n,]);
                print $this->_view->get_template($this->_loc,'Mysql_LCode_validator_Addform_start',['name'=>$n,]);
	            $this->_model->GET_Collums($this->_loc,'Mysql_Code_validator_Addform_element',$this->_view,$this->_values['database'],$this->_values['table']);
	            print $this->_view->get_template($this->_loc,'Mysql_LCode_validator_Addform_end',['name'=>$n,]);
                print $this->_view->get_template($this->_loc,'Mysql_LCode_validator_allocat_start',['name'=>$n,]);
	            $this->_model->GET_Collums($this->_loc,'Mysql_Code_validator_allocate_element',$this->_view,$this->_values['database'],$this->_values['table']);
	            print $this->_view->get_template($this->_loc,'Mysql_Code_validator_allocat_end',['name'=>$n,]);
	            print $this->_view->get_template($this->_loc,'Mysql_Code_validator_render',['name'=>$n,]);
                print $this->_view->get_template($this->_loc,'Mysql_Code_end',['name'=>$n,]);
                
                print $this->_view->get_template($this->_loc,'Mysql_Code_start',['name'=>$n,]);
                print $this->_view->get_template($this->_loc,'Mysql_LCode_handel',['name'=>$n,'table'=>$this->_values['table'],]); 
                print $this->_view->get_template($this->_loc,'Mysql_Code_end',['name'=>$n,]);
                print $this->_view->get_template($this->_loc,'Mysql_Code_start',['name'=>$n,]);
                print $this->_view->get_template($this->_loc,'Mysql_LCode_explore',['name'=>$n,'table'=>$this->_values['table'],]); 
                print $this->_view->get_template($this->_loc,'Mysql_Code_end',['name'=>$n,]);
                print $this->_view->get_template($this->_loc,'MysqlCODE_foot',$array);	            
	            $GETEVERYTHINGELSE = false;
           }
           if($GETEVERYTHINGELSE&&isset($this->_values['database'])&&$this->_values['database']!=''){
              	$array['id'] = 'info';
	            $array['title'] = $this->_values['database'];
                print $this->_view->get_template('global','tooltip_start');
	            print $this->_view->get_template($this->_loc,'MysqlTable_head',$array);
                //$this->_view->reSetTemplate($this->_loc,'MysqlTable_item',[],$this->_values);
	            $this->_model->GET_Database($this->_values['database'],$this->_loc,'MysqlTable_item',$this->_view);
                
	            print $this->_view->get_template($this->_loc,'MysqlTable_foot',$array);
	            $GETEVERYTHINGELSE = false;
           }

           if($GETEVERYTHINGELSE&&isset($this->_values['databases'])){
             $array['id'] = 'info';
             $array['title'] = $this->_values['databases'];
             print $this->_view->get_template($this->_loc,'MysqlTable_head',$array);
             $this->_model->GET_Databases($this->_loc,'MysqlDatabase_item',$this->_view);
             print $this->_view->get_template($this->_loc,'MysqlTable_foot',$array);
             $this->_model->GET_Plist($dbname,$location,$templatename,$view);
             $GETEVERYTHINGELSE = false;
           }
           if($this->_values['backup']){
				$this->_model->GET_Tables($this->_loc,'Mysqltable_item',$this->_view);
		   }
		   if($this->_values['sqlhelp']){
			   $array['id'] = 'category';
	           $array['title'] = '';          
	           $array['layoutpath']= $this->_view->getimgPath();
               print $this->_view->get_template('global','sectionv_head',$array);
               $array['title'] = '';
               $array['bullet'] = $this->_view->getimgPath().'bubble.svg';
                   print $this->_view->get_template($this->_loc,'CategoryItem_head',$array);
                   print_r($this->categorys);
                   foreach($this->categorys as $key => $value){
                        $array['id']=$key;
                        $array['label']=$value;
                        print $this->_view->get_template($this->_loc,'MysqlCategoryItem',$array);
                   }
                   print $this->_view->get_template($this->_loc,'CategoryItem_foot');
               print $this->_view->get_template('global','section_foot');
		   }
		   if($this->_values['help_topic']){
			    $array['id'] = 'info';
                $array['title'] = '';
                print $this->_view->get_template($this->_loc,'item_head',$array);
                $this->_model->GET_ITEM($this->_loc,'MysqlHelpTopicItem',$this->_view,$this->_values['help_topic'],'',true,'',$this->_values['page']);
                print $this->_view->get_template($this->_loc,'item_foot',$array);
		   }
           if($GETEVERYTHINGELSE){
                $array['id'] = 'info';
                $array['title'] = '';
                print $this->_view->get_template($this->_loc,'item_head',$array);
                print 'ATTENTION THIS CAN NOT WORK BY SHARED HOSTING ';
                print $this->_view->get_template($this->_loc,'item_foot',$array);
           }
           $array['id'] = 'favorites';
           $array['title'] = '';//$this->_values['auction.favorites.title'];

           print $this->_view->getfooter($action);
        }
        private function getDbinfo($table,$result){
             $dbinfo=[];
             foreach($res as $row){
                 foreach($row as $k=>$v){
                    $dbinfo['keys'][]=$k;
                    $dbinfo[$k]['null']=false;
                    if($dbinfo[$k]['null']){
                        $dbinfo['keys_null'][]=$k;
                    }
                    $dbinfo[$k]['primary']=false;
                    $dbinfo[$k]['len']=0;
                    $dbinfo[$k]['type']=0;
                }
             }
        }
        /**
        
        $this->_validator->addForm('addpart', ['id','contenttype','TYPE','content','amail_conceptmail_id'],
        false,['id','amail_conceptmail_id'], $this->_values);

		if(isset($_POST['addcontentpartto'])){
		    $this->_validator->addAlocat('addpart',['id'=>'isnumeric','contenttype'=>'istext','TYPE'=>'istext','amail_conceptmail_id'=>'isnumeric']);
		    $this->_validator->render('addpart',$_POST,$this->_values);     
		}
		$this->status('');

        */
        private function resultTo($dbinfo,$pretemp){
            $render['keys']= implode($dbinfo['keys']);  
            $render['keys_null']= implode($dbinfo['keys_null']);
            print  $this->_view->get_template('server',$pretemp,$render);
        }
        private function vars(){
            $category=(isset($_GET['category'])&& is_numeric($_GET['category']))?$_GET['category']:0;
            /*$this->categorys = $this->_model->GET_Categorys($category);
            if(count($this->categorys)==0){
                $this->categorys = $this->_model->GET_Categorys(0);
            }*///
            $this->addTranslationValues($this->values);
            //$this->addTranslationValues($this->categorys);
            $this->_values = $this->getTranslations();
            //$this->_view->setENUM_TRANSLATIONS($this->_values);
            $this->_values['category']=$category;
            $this->_values['page']=0;
            $this->_validator->addMenu('menuselector',['e_query','prepareforplugin','code','table'],$_GET,$this->_values);
            
            $this->_validator->addForm('makequery',['query','database'],false,[''],$this->_values);
    		if(isset($_POST['query'])){
    		    $this->_validator->addAlocat('makequery',['database'=>'istext']);
    		    $this->_validator->render('makequery',$_POST,$this->_values);
    		}
    		$this->_validator->addForm('executequery',['query','resultto'],false,['resultto'],$this->_values);

    		if(isset($_POST['execute'])){
    		    $this->_validator->addAlocat('executequery',['database'=>'istext','resultto'=>'istext']);
    		    $this->_validator->render('executequery',$_POST,$this->_values);  
    		}
            //$this->status('');
            $this->_validator->addForm('plugin',['query'],false,[''],$this->_values);
    		if(isset($_POST['plugin'])){
    		    $this->_validator->addAlocat('plugin',['database'=>'istext']);
    		    $this->_validator->render('plugin',$_POST,$this->_values);  
    		}
            if(isset($_POST['find'])){
                $this->_values['search']=(isset($_POST['search']))?$_POST['search']:'';
                $this->_values['q']=(isset($_POST['q']))?$_POST['q']:$this->_values['search'];
                $this->_values['section']=(isset($_POST['section'])&&key_exists($_POST['section'],$this->_model->section))?$_POST['section']:'';
                
                $this->_values['sections']=$this->_view->getOptions($this->_model->section,$this->_values['section']);       
            }else{
                if(isset($_GET['database'])){
                    $this->_values['database']=(\autosystem\ValidateAs::isTEXT($_GET['database']))?$_GET['database']:'';
                }
                if(isset($_GET['table'])){
                    $this->_values['table']=(\autosystem\ValidateAs::isTEXT($_GET['table']))?$_GET['table']:'';
                }
                if(isset($_GET['databases'])){
                    $this->_values['databases']=(\autosystem\ValidateAs::isTEXT($_GET['databases']))?$_GET['databases']:'';
                }
                $this->_values['info.title']=$this->_values['detail'];
                if(isset($_GET['category'])){
                    $this->_values['category']=(isset($_GET['category'])&&\autosystem\ValidateAs::isnumeric($_GET['category']))?$_GET['category']:0;
                }
                if(isset($_GET['view'])){
                    $this->_values['view']=(isset($_GET['view'])&&\autosystem\ValidateAs::isnumeric($_GET['view']))?$_GET['view']:'';
                }
                if(isset($_GET['page'])){
                   $this->_values['page']=(isset($_GET['page'])&&\autosystem\ValidateAs::isnumeric($_GET['page']))?$_GET['page']:0;
                }
                if(isset($_GET['sqlhelp'])){
                   $this->_values['sqlhelp']=(isset($_GET['sqlhelp'])&&\autosystem\ValidateAs::isnumeric($_GET['sqlhelp']))?$_GET['sqlhelp']:0;
                }
                if(isset($_GET['report'])||isset($_POST['report'])){
                    $this->_values['report']=(isset($_GET['report'])&&\autosystem\ValidateAs::isnumeric($_GET['report']))?$_GET['report']:'';
                    $this->_values['detail']=(isset($_POST['detail'])&&\autosystem\ValidateAs::isTEXT($_POST['detail']))?$_POST['detail']:'';
                }
            }
            $this->_values['search']='';
        }
    }
?>