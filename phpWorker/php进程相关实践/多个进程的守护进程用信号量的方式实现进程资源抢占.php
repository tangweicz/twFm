<?php
/**
 * Created by PhpStorm.
 * User: tangwei
 * Date: 18/4/20
 * Time: 09:19
 */

$num = 4;

$pidFile = "/root/daemonPids.txt";

$ppidFile = "/root/daemonPpids.txt";

function writePidToFile($pid, $isnew = true){

    global $pidFile;

    if(isset($pidFile)){

        $dir = dirname($pidFile);//拿到文件名的路径

        if(!file_exists($dir)){//如果目录不存在，需要创建这个目录

            mkdir($dir, 0777, true);

        }

        if(!file_exists($pidFile)){

            touch($pidFile);

        }

        if($isnew){

            $mode = "w+";

        }else{

            $mode = "a+";

        }

        $fp = fopen($pidFile, $mode);

        if(!$fp)echo "打开文件失败";

        if(flock($fp, LOCK_EX)){

            if($pid == ""){

                fwrite($fp, $pid);

            }else{

                fwrite($fp, $pid.",");

            }

            flock($fp, LOCK_UN);

        }else{

            echo "上锁失败\r\n";

        }

        fclose($fp);


    }else{

        echo "未知文件路径，请设置再运行Daemon";

        exit;

    }

}

function writePidToPpidFile($pid, $isnew = true){

    global $ppidFile;

    if(isset($ppidFile)){

        $dir = dirname($ppidFile);//拿到文件名的路径

        if(!file_exists($dir)){//如果目录不存在，需要创建这个目录

            mkdir($dir, 0777, true);

        }

        if(!file_exists($ppidFile)){

            touch($ppidFile);

        }

        if($isnew){

            $mode = "w+";

        }else{

            $mode = "a+";

        }



        $fp = fopen($ppidFile, $mode);

        if(!$fp)echo "打开文件失败";

        if(flock($fp, LOCK_EX)){

            if($pid == ""){

                fwrite($fp, $pid);

            }else{

                fwrite($fp, $pid.",");

            }



            flock($fp, LOCK_UN);

        }else{

            echo "上锁失败\r\n";

        }

        fclose($fp);


    }else{

        echo "未知文件路径，请设置再运行Daemon";

        exit;

    }

}

function stopDaemon(){//停止运行进程

    global $pidFile;

    echo "停止所有进程运行........\r\n";

    $pids = file_get_contents($pidFile);

    $pidArr = explode(",", $pids);

    if(!empty($pidArr)){

        foreach($pidArr as $key=>$value){

            if($value != ""){

                posix_kill($value, 9);

            }

        }


    }

    exit;

}

$pid = pcntl_fork();//fork子进程

writePidToFile("", true);

writePidToPpidFile("", true);

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

    $key = ftok('/tmp', 'c');

    $sem = sem_get($key);

    //这儿获取到进程的pid
    for($i=0;$i<$num;$i++){

        $ppid = pcntl_fork();

        if($ppid == -1){//fork子进程失败

            die('fork子进程失败');

        }elseif($ppid) {//父进程操作区域

            $continues = false;

            if($i+1 >= $num) $continues = true;

            if($continues){

                $pid = posix_getpid();

                writePidToPpidFile($pid, false);

                pcntl_signal(SIGTERM, "stopDaemon");//安装进程退出信号控制

                pcntl_signal(SIGCHLD, function(){

                    global $i;

                    global $continues;

                    echo "子进程退出，父进程需要重启子进程........\r\n";

                    $i = $i - 1;

                    $continues = false;

                    pcntl_wait($status);//回收子进程的资源

                });

            }

            while($continues){

                echo  "这儿是父进程........\r\n";

                sleep(2);

                pcntl_signal_dispatch();

            }



        }else{//子进程操作区域

            $ssid=posix_setsid();//使进程摆脱界面的控制

            if ($ssid == -1) {//使进程摆脱界面的控制，操作失败

                die("could not detach from terminal");

            }elseif($ssid < 0){//使进程摆脱界面的控制，操作失败

                exit;

            }

            $posid = posix_getpid();//获取自己的pid，写入到文件

            //declare(ticks = 1);

            pcntl_signal(SIGTERM, function(){//安装进程退出信号控制

                echo "子进程被要求退出.....\r\n";

                exit;

            });

            writePidToFile($posid, false);

            sleep(1);

            while(true){

                pcntl_signal_dispatch();

                echo "子进程pid".posix_getpid()."准备处理，需等待.......\r\n";

                sem_acquire($sem);

                echo "子进程pid".posix_getpid()."处理中.......\r\n";

                sleep(10);

                sem_release($sem);

            }
        }
    }


}