<?php
//use PDO;
namespace autosystem;
class DBConn{
    static private $_obj = null;
    private $_dbConn = null;
    static protected $_DBprefix = '';
    static protected $_CharSet;
    static public $_Collate;
    static public $connected;
    private $config ;
    static private $hasinitialconnection;
    public function __construct($issuper = false,$isadmin = false) {
        $url='/failed';
        $this->config = getAutositePath(['autosystem','Db.ini.php']);
        //$this->config = './includes'.DIRECTORY_SEPARATOR.'autosystem'.DIRECTORY_SEPARATOR.'Db.ini.php';
        if(file_exists($this->config)&& is_readable($this->config)){
            try{
                if($config = parse_ini_file($this->config)){//@SEE if charset not exist WHITE SCREEN OF DEAD
                    self::$_CharSet = $config['CHARSET'];
                    self::$_Collate = $config['COLLATE'];
                    if($isadmin){
                        self::$connected =$this->connect($config['HOST'],$config['DATABASE'],$config['ADMIN'],$config['PASSWORD']);
                        self::$hasinitialconnection = true; 
                        return;
                    }
                    if($issuper){
                        self::$connected =$this->connect($config['HOST'],$config['DATABASE'],$config['SUPERUSER'],$config['PASSWORD']);
                        self::$hasinitialconnection = true; 
                        return;
                    }
                    self::$connected =$this->connect($config['HOST'],$config['DATABASE'],$config['USER'],$config['PASSWORD']);
                    self::$hasinitialconnection = true;     
                }
                
                if(!self::$connected){self::$hasinitialconnection = true; die('no connection remove configfile');}
            }catch(Exception $ex){
                die('YOU KILLING ME ? CONNECTION FAILED!o i am sorry i dont work today maybe later, if admin has seen this fault.');
            }
        }else{
            $this->install();
            die(' <!--FATALE FOUT... '.' DATABASE CONFIG-->');
        }
    }
    /**
     * DBConn::GET()
     * @return connection
     */
    static public function GET($issuper = false,$isadmin = false){
        if(!(self::$_obj instanceof self)){
            self::$_obj=new self($issuper,$isadmin);
        }
        return self::$_obj->_dbConn;
    }
    private function connect($host,$dbname,$usr,$pwd){
       $conn = 'mysql:host='.$host.';dbname='.$dbname;
        //odbc_connect($conn,$usr,$pwd) or die(odbc_errormsg() );
        try{
            $this->_dbConn = new \PDO($conn, $usr, $pwd,self::getoptions());
            //$this->_dbConn->exec('SET GLOBAL general_log_file = "/log/mysql.log";');
            //$this->_dbConn->exec("SET GLOBAL general_log = 'ON';");
            return  $this->_dbConn;
        }catch(Exception $ex){
            return false;
        }
    }
    /**
     * DBConn::getoptions()
     * @return array()
     */
    private function getoptions(){
        return array(\PDO::MYSQL_ATTR_INIT_COMMAND => 'SET NAMES \''.self::$_CharSet.'\'',\PDO::ATTR_ERRMODE => \PDO::ERRMODE_EXCEPTION);
    }
    private function getOpenPort(){
        $ports =[ ];
        foreach($ports as $portnr){
            if($this->isPortOpen($portnr)){
                return $portnr; 
            }
        }
        return false;
    }
    private function isPortOpen($nr){
        return false;
    }
    static public function PREFIX(){                return self::$_DBprefix; }
    static public function getTableNameFor($key){   
        return strtolower(self::PREFIX().$key);
    }
    static public function getCHARSET(){            return self::$_CharSet;  }
    static public function getCOLLATE(){            return self::$_Collate;  }
    public function install(){        \start\Install_model::dbinstall($this->config);}
    /**
     * DBConn::__destruct()
     * @return void
     */
    public function __destruct(){
        if(isset(self::$_obj) &&self::$_obj!=null){
            if(isset(self::$_obj->_dbConn)&& self::$_obj->_dbConn!=null){
                unset(self::$_obj->_dbConn);
                self::$_obj->__destruct();
            }
            unset($this->_dbConn);
        }
        $this->_dbConn = null;
        if($this->_dbConn!=null){
            $this->$_obj->__destruct();
            unset($this->$_obj->$this->_dbConn);
            unset($this->$_obj);
            $this->_dbConn = null;
        }
    }
    /**
     * DBConn::__processlist()
     * @param bool $kill
     * @return
     */
    public function __processlist($kill = false){
        $query = 'SHOW PROCESSLIST -- ' . uniqid('pdo_mysql_close', 1);
        $list  = $connection->query($query)->fetchAll(\PDO::FETCH_ASSOC);
        foreach ($list as $thread) {
            if ($thread['Info'] === $query) {
                if($kill == true){
                    return $connection->query('KILL ' . $thread['Id']);
                }else{
                    return false;
                }
            }
        }
        return false;
    }
    public function GET_connection_id(){
        $sql = 'SELECT CONNECTION_ID()';
    }
} 
?>