<?php
	namespace users;
    class Logout_model extends \users\Login_model{
        private $lastQ = '';
        public function save($locale,$key ,$value){}
        public function exist($locale,$key){  }
        public function getModelInfo(){       return 'Logout.model.info';    }
        public function getVersionNr(){       return (float)5.4;    }
        public function __upgrade($version){  return true;        }
        public function getRuleDefault(){     return 'USER';     }
    }
?>