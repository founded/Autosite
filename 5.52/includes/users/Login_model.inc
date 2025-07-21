<?php
    namespace users;
	class Login_model extends \users\Profile_model{
        public function getModelInfo(){
            return 'Login.model.info';
        }
        public function getRuleDefault(){
            return 'PUBLIC';
        }
    }
?>