<?php

namespace Libs;

use Symfony\Component\Templating\PhpEngine;

use Symfony\Component\Templating\TemplateNameParser;

use Symfony\Component\Templating\Loader\FilesystemLoader;

/**
 * Created by PhpStorm.
 * User: tangwei
 * Date: 15/7/2
 * Time: 10:17
 */

class CTemplate {
    public $template_file;
    public $theme;
    private $_template_dir;

    function __construct($f="", $theme="default", $template_dir) {
        if (isset($template_dir) ||
            !empty($template_dir)) {
            $this->_template_dir = $template_dir;
        }
        else {
            $this->_template_dir = $GLOBALS['template_dir'];
        }

        if (empty($f)) {
            print __CLASS__."::".__METHOD__. ": line: ".__LINE__.": empty template file specified. ";
            exit(-1);
        }
        else {
            $this->template_file = $f;
        }
    }

    public function render(array $vars) {
        $loader = new FilesystemLoader($this->_template_dir."/".$this->theme."/%name%");
        $view = new PhpEngine(new TemplateNameParser(), $loader);

        echo $view->render($this->template_file, $vars);
    }
}