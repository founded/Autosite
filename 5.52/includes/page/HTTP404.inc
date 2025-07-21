<?php
namespace page;
class HTTP404 extends \autosystem\Controller{
    public function init() {
       print $this->_view->getheader('page','HTTP404');
       print $this->_view->get_template('page','HTTP404');
       print $this->_view->getfooter('HTTP404');
    }
    public function canIDo($location=''){
         return true;
    }
}
?>