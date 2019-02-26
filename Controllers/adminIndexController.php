<?php

namespace Controllers;

use Libs\Controller;

use Libs\CTemplate;

/**
 * Created by PhpStorm.
 * User: tangwei
 * Date: 16/3/2
 * Time: 14:06
 */
class adminIndexController extends Controller
{
    public function index()
    {
        $ctemplate=new CTemplate("admin/index.php","dafault",__DIR__."/../Template");

        $ctemplate->render(array('page'=>""));
    }

    public function top()
    {
        $ctemplate=new CTemplate("admin/top.php","dafault",__DIR__."/../Template");

        $ctemplate->render(array('page'=>""));
    }

    public function menu()
    {
        $ctemplate=new CTemplate("admin/menu.php","dafault",__DIR__."/../Template");

        $ctemplate->render(array('page'=>""));
    }

    public function listall()
    {
        $ctemplate=new CTemplate("admin/listall.php","dafault",__DIR__."/../Template");

        $ctemplate->render(array('page'=>""));
    }
}