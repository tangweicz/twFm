<?php
/**
 * Created by PhpStorm.
 * User: tangwei
 * Date: 16/10/19
 * Time: 16:52
 */

class server
{

    public function __construct()
    {
        $firstProcess = new swoole_process(array($this, "firstChild"), false);

        $firstProcess->start();//启动子进程，拿到子进程的进程号

        exit;//父进程退出

    }

    public function firstChild($worker)
    {
        while(true){

            $childProcess = new swoole_process(array($this, "secondChild"), false);

            $childProcess->daemon();

            $childProcess->start();//启动子进程

            while($ret =  $childProcess->wait(true)) {

                echo "PID={$ret['pid']}\n";

            }

        }

    }

    public function secondChild($worker)//这儿启动一个子进程去读取有多少个任务。有几个任务就启动几个子进程。同时，子进程要监控有多少个子进程完成了自己的任务，如果全部完成了任务，那么就要重新读取任务数量。
    {

        $getNumProcess = new swoole_process(array($this, "getNum"), false, true);

        $getNumProcess->start();

        echo "第二个子进程启动\r\n";

        swoole_event_add($getNumProcess->pipe, function($pipe)use($getNumProcess){

            $dangqianNum = 0;

            echo "读取获取到的URL的值\r\n";
            $urlString = $getNumProcess->read();

            $urlArray = explode(",", $urlString);

            $totalNum = count($urlArray);

            foreach($urlArray as $value){

                echo "根据URL去启动对应的进程。。。。\r\n";

                $workerTask = new swoole_process(array($this, "workerTask"), false, true);

                $workerTask->start();

                echo "启动".$value."对应的进程\r\n";

                $workerTask->write($value);

                swoole_event_add($workerTask->pipe, function($pipes)use($workerTask, $value, $totalNum, &$dangqianNum, $getNumProcess){

                    $num = $workerTask->read();

                    echo "获取到".$value."进程的回调\r\n";

                    if($num == 1){

                        $dangqianNum = $dangqianNum + 1;

                        $workerTask->wait(true);

                    }

                    echo $totalNum."\r\n";

                    echo $dangqianNum."\r\n";

                    if($totalNum == $dangqianNum){

                        echo "告诉获取URL地址的进程可以再来一遍了！！！\r\n";

                        $getNumProcess->write(1);

                    }

                });

            }

        });

    }

    public function getNum($worker)
    {

        while(true){

            echo "获取URL地址的进程操作中。。。。。\r\n";

            $worker->write("www.baidu.com,www.google.com,www.163.com");

            $canContinue = $worker->read();

            if($canContinue){

                continue;

            }else{

                break;
            }

            sleep(10);

        }

    }

    public function workerTask($worker)
    {

        $url = $worker->read();

        sleep(10);

        echo "对应".$url."的进程运行中。。。。\r\n";

        echo $url."\r\n";

        $worker->write(1);

        exit;
    }
}

new server();