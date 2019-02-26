<?php

namespace Controllers;

use Models\taskModel;

use Libs\CTemplate;

use Libs\Page;

use Libs\Controller;

use Libs\Upload;

use Monolog\Logger;

use Monolog\Handler\StreamHandler;


/**
 * Created by PhpStorm.
 * User: tangwei
 * Date: 15/7/1
 * Time: 16:20
 */

class demoController extends Controller{
    public function UpdateUserName(){

        $allTask = taskModel::getAllTask();
        $page = new Page(count($allTask),self::$PAGESIZE);
        $data = taskModel::getAllTaskByLimit($page->limit);
        var_dump($data);

        $ctemplate=new CTemplate("task/index.html","dafault",__DIR__."/../Template");//测试模板使用
        $ctemplate->render(array('page'=>$page->showpage()));

        $log = new Logger('name');//开始日志类
        $log->pushHandler(new StreamHandler(__DIR__.'/../Log/your.log', Logger::WARNING));
        $log->addWarning('Foo');// add records to the log
        $log->addError('Bar');

        $upload = new Upload(12);//上传文件开始
        $upload->FileMaxSize = array('image' => 5*1024 * 1024, 'audio' => 2 * 1024 * 1024, 'video' => 20 * 1024 * 1024);
        if($_GET["isVedio"] == 1){
            $upload->FileType = array('audio/mpeg', 'video/x-msvideo','video/mp4'); // 允许上传的文件类型
            $upload->FileSavePath = '../Upload/';
        }else{
            $upload->FileType = array('image/jpeg', 'image/png','image/jpeg'); // 允许上传的文件类型
            $upload->FileSavePath = '../Upload/';
        }
        $file_save_full_name = $upload->UploadFile();
        $str = "";
        if(is_array($file_save_full_name) && !empty($file_save_full_name)){
            foreach($file_save_full_name as $k=>$v){
                $str.=$v.";";
            }
            $str = substr($str,0,-1);
        }else{
            $str = $file_save_full_name;
        }
        echo $str;//得到最终上传文件的文件名

    }


    /**
     * User: tangwei
     * Date: 2019/2/26 10:13
     * Function: 测试框架内事务的使用
     */
    public function testTransaction()
    {
        $pdo = new CDbUtil();
        $pdo->beginTransaction();
        $collect = collectionModel::getOneById(array("id"=>4));
        if(empty($collect)) self::doLog("用户收藏记录不存在！！");
        $collect = $collect[0];
        $r = $collect->delOne();
        $user = usersModel::getOneById(array("id"=>3));
        if(empty($user))self::doLog("用户不存在！！");
        $user = $user[0];
        $m = $user->editOneMobile(array("mobile"=>66666));
        if(!$r && !$m) $pdo->rollBack();
        $pdo->commit();
    }


}