<?php
namespace page;
class HTTP403 extends \autosystem\Controller{
    public function init() {
       print $this->_view->getheader('page','HTTP403');
       print $this->_view->get_template('page','HTTP403');
       print $this->_view->getfooter('HTTP403');
    }
    public function canIDo($location=''){
         return true;
    }
}
?>