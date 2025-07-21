<?php
namespace users;
class Logout extends \autosystem\Controller{
    private $Key;
    private $values ;
    public function init(){      
       $this->LogoutUser();
       $this->redirect('/users/Login');
    }
}
?>