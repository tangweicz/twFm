<?php
/**
 * Created by PhpStorm.
 * User: tangwei
 * Date: 15/7/1
 * Time: 15:39
 */

ini_set("display_errors", "On");

error_reporting(E_ERROR | E_WARNING | E_PARSE);

ini_set('date.timezone','Asia/Shanghai');

header("Content-type:text/html;charset=utf-8");

require_once "../vendor/autoload.php";

require_once "../Config/Route.php";


//$lo = new Dotenv(__DIR__."/../",".env");//加载全局配置文件
//
//$lo->load();
//
//getenv("S3_BUCKET");
//
///*
// * 这儿可以处理自己的路由，下面的只是范例
// */
//$task = new taskController();
//
//$taskRes = $task->UpdateUserName();

