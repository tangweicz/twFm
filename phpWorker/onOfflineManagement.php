#!/usr/bin/php

<?php
/**
 * Created by PhpStorm.
 * User: tangwei
 * Date: 16/5/21
 * Time: 11:12
 */

require_once "CDbUtil.php";

class onOfflineManagement
{
    public $num = 1;

    public $pidFile = "/tmp/onOfflineManagement.pid";

    public $logFile = "../Log/onOfflineManagement.log";

    /*
     * @author tangwei
     *
     * @time 2016-05-21 11:17:48
     *
     * @function 使用方法给出
     */
    public function usage()
    {
        echo "example: xxxx.php start/stop\n";
    }

    /*
     * @author tangweire
     *
     * @time 2016-05-21 11:20:20
     *
     * @function 进程控制
     */
    public function runAsDaemon()
    {
        $pid = pcntl_fork();//fork子进程

        if($pid == -1){//fork子进程失败

            die('fork子进程失败');

        }elseif($pid) {//父进程操作区域

            exit;

        }else{//子进程操作区域
            $sid=posix_setsid();//使进程摆脱界面的控制

            if ($sid == -1) {//使进程摆脱界面的控制，操作失败

                die("could not detach from terminal");

            }elseif($sid < 0){//使进程摆脱界面的控制，操作失败

                exit;

            }

            for($i=0;$i<$this->num;$i++){

                $ppid = pcntl_fork();

                if($ppid == -1){//fork子进程失败

                    die('fork子进程失败');

                }elseif($ppid) {//父进程操作区域

                    pcntl_wait($status);

                    $i = $i - 1;

                }else{//子进程操作区域

                    $ssid=posix_setsid();//使进程摆脱界面的控制

                    if ($ssid == -1) {//使进程摆脱界面的控制，操作失败

                        die("could not detach from terminal");

                    }elseif($ssid < 0){//使进程摆脱界面的控制，操作失败

                        exit;

                    }

                    if(isset($this->pidFile)) {

                        $posid = posix_getpid();//获取自己的pid，写入到文件

                        $pid_file_dirname=dirname($this->pidFile);//判断该文件的路径部分是否正在存在，不存在则尝试创建它

                        if(!file_exists($pid_file_dirname)){

                            mkdir($pid_file_dirname,0777,true);

                            if(!file_exists($pid_file_dirname)){

                                posix_kill($posid,  SIGKILL);

                                die('目录不存在，同时尝试创建也失败，daemon不能正常启动！');

                            }
                        }
                        //文件路径部分判断完毕
                        $fp = fopen($this->pidFile, "w");

                        fwrite($fp, $posid);

                        fclose($fp);
                    }
                }
            }


        }

    }

    /*
     * @author tangwei
     *
     * @time 2016-05-21 11:18:09
     *
     * @function 开始启动脚本
     */
    public function startDae()
    {
        if(file_exists($this->pidFile)) die("进程已经启动无法重复启动\r\n");

        $this->runAsDaemon();//将命令变成进程，进入后台作业

        while(true){

            $sql =  "SELECT * FROM `appUser` WHERE `isOnLine` = 1 AND `isLocked` = 0 ORDER BY `id` ASC";

            $onLineUsers = CDbUtil::query($sql);

            if($onLineUsers != false){

                foreach($onLineUsers as $value){

                    $sql = "SELECT * FROM `actionRecord` WHERE `userId` = ".$value["id"]." ORDER BY `actionTime` DESC LIMIT 1";

                    $actionRes = CDbUtil::query($sql);

                    if($actionRes != false){

                        $now = time();

                        $actionRes = $actionRes[0];

                        if($now > ($actionRes["actionTime"] + 30*60)){

                            $sql = "UPDATE `appUser` SET `isOnLine` = 0  WHERE `id`= ".$value["id"];

                            CDbUtil::query($sql);

                        }

                        unset($actionRes);

                        unset($now);

                    }else{

                        $sql = "UPDATE `appUser` SET `isOnLine` = 0  WHERE `id`= ".$value["id"];

                        CDbUtil::query($sql);

                    }

                    unset($actionRes);

                    unset($value);

                }

            }

            unset($onLineUsers);

            sleep(5);

        }
    }

    /*
     * @author tangwei
     *
     * @time 2016-05-21 11:18:32
     *
     * @function 停止脚本
     */
    public function stopDae()
    {
        if(!file_exists($this->pidFile)){

            exit(0);

        }else{

            $pidfile_content=file_get_contents($this->pidFile);

            if($pidfile_content == FALSE || !is_numeric( $pidfile_content)){

                exit(0);

            }else{

                posix_kill($pidfile_content,  SIGKILL);

                unlink($this->pidFile);
            }


        }
    }

}

$onOfflineManagement = new onOfflineManagement();

if (count($argv) < 2) {

    $onOfflineManagement->usage();

    exit(0);

}

switch($argv[1]){

    case "start":

        $onOfflineManagement->startDae();
        break;

    case "stop":

        $onOfflineManagement->stopDae();
        break;

    default:

        $onOfflineManagement->usage();
        break;

}

