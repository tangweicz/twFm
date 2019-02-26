<?php
/**
 * Created by PhpStorm.
 * User: tangwei
 * Date: 18/4/25
 * Time: 14:11
 */

class myThread extends Thread{

    public function run(){

        for($i =0; $i <= 5; $i++){//这儿相当于子线程在工作的时间

            if($i > 3){

                try{

                    $file = "adsasdsfsfdsd.php";

                    if(!file_exists($file)) throw new Exception("文件不存在");

                    require_once $file;

                }catch (Exception $e){

                    echo "子线程遇到错误....\r\n";

                    var_dump($e->getMessage());

                    exit;

                }

            }

            echo "主线程".$this->getCreatorId().".......\r\n";

            echo "子线程".$this->getCurrentThreadId()."在运行中........\r\n";

            sleep(3);

        }

    }

}

$myThread = new myThread();

if(!$myThread->isStarted()){

    $myThread->start();

}



while(true){//这儿是相当于父进程在操作

    echo "主进程在运行中".posix_getpid()."........\r\n";

    sleep(5);//这儿的sleep就是相当于主进程在工作

    if(!$myThread->isRunning()){//判断子线程是否还在运行中，如果不在运行，那么重新启动子线程。

        if(!$myThread->isJoined()){

            $myThread->join();//主进程在这儿等待，子线程的退出（包括，正常退出、执行完毕、遇到错误主动退出）。主进程里没有后续操作不join也是可以的。不join的话，主进程直接结束，由于线程安全，主进程会等待子线程运行完毕之后再退出。

            echo "子线程join到主进程.......\r\n";

        }

        if($myThread->isTerminated()){//只有先join到主进程，然后才能判断是否为异常终止

            echo "子线程遇到异常被异常终止.....\r\n";

        }

        echo "重新启动子线程......\r\n";

        $myThread = new myThread();//注意，这儿重新启动的子线程，线程号跟之前的线程号是一致的

        if(!$myThread->isStarted()){

            $myThread->start();

        }

    }

}

echo "主进程执行完毕......\r\n";

//得到当前线程句柄: GetCurrentThread(void)

//得到当前线程ID: GetCurrentThreadId(void)

//根据句柄得到线程ID: GetThreadId

