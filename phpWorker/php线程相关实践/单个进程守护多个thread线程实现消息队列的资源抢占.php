<?php
/**
 * Created by PhpStorm.
 * User: tangwei
 * Date: 18/4/26
 * Time: 16:16
 */

class myThread extends Thread{

    public $sem;

    public $message_queue;

    public $wantStop = false;

    public function __construct($message_queue, $sem){

        $this->message_queue = $message_queue;

        $this->sem = $sem;

    }


    public function run(){

        sem_acquire($this->sem);

        for($i =0; $i <= 5; $i++){//这儿相当于子线程在工作的时间

            if($this->wantStop){

                sem_release($this->sem);

                echo "子线程".$this->getCurrentThreadId()."被要求退出........\r\n";

                exit;

            }

            if($i > 3){

                try{

                    $file = "adsasdsfsfdsd.php";

                    if(!file_exists($file)) throw new Exception("文件不存在.....\r\n");

                    require_once $file;

                }catch (Exception $e){

                    echo "子线程".$this->getCurrentThreadId()."遇到错误....\r\n";

                    var_dump($e->getMessage());

                    sem_release($this->sem);

                    exit;

                }

            }

            if($i == 0){

                $res = msg_receive($this->message_queue, 0, $message_type, 1024 , $message , true,  MSG_IPC_NOWAIT);

                if($res){

                    echo "子线程".$this->getCurrentThreadId()."成功获取到数据：$message........\r\n";

                }else{

                    echo "子线程".$this->getCurrentThreadId()."未获取到数据........\r\n";

                }

            }



            echo "子线程".$this->getCurrentThreadId()."在运行中........\r\n";

            sleep(3);

        }

        sem_release($this->sem);

    }

}


$key = ftok('/tmp', 'd');

$message_queue = msg_get_queue($key, 4096);

$key1 = ftok(__FILE__, "c");

$sem = sem_get($key1);

$threadArray = array();

for($i=0;$i<3;$i++){

    $thread = new myThread($message_queue, $sem);

    $thread->start();

    echo "new出一个新的子线程，线程ID：".$thread->getThreadId().".......\r\n";

    $threadArray[$thread->getThreadId()] = $thread;

}

pcntl_signal(SIGINT, function(){

    global $message_queue;

    global $threadArray;

    msg_remove_queue($message_queue);

    echo "主进程准备退出，等待子进程退出.......\r\n";

    if(!empty($threadArray)){

        foreach($threadArray as $k=>$v){

            $v->wantStop = true;

        }


    }

    sleep(3);

    echo "主进程退出完毕.........\r\n";

    exit;

});

pcntl_signal(SIGTERM, function(){

    global $message_queue;

    global $threadArray;

    msg_remove_queue($message_queue);

    echo "主进程准备退出，等待子进程退出.......\r\n";

    if(!empty($threadArray)){

        foreach($threadArray as $k=>$v){

            $v->wantStop = true;

        }


    }

    sleep(3);

    echo "主进程退出完毕.........\r\n";

    exit;

});

while(true){

    pcntl_signal_dispatch();

    sleep(8);

    for($x = 0; $x < 5; $x++){

        msg_send($message_queue, 1, "主进程放进来的数据".time());

    }

    echo "主进程运行中，正在守护所有线程.......\r\n";

    echo "处理前的数组.................\r\n";

    var_dump($threadArray);

    echo "处理前的数组.................\r\n";

    if(!empty($threadArray)){

        echo "开始处理线程数组.........\r\n";

        foreach($threadArray as $key=>$value){

            if(!$value->isRunning()){

                echo "线程ID：".$value->getThreadId()."不在运行中，需要被join.........\r\n";

                $value->join();

                unset($threadArray[$value->getThreadId()]);

                echo "处理后的数组.................\r\n";

                var_dump($threadArray);

                echo "处理后的数组.................\r\n";

                if($value->isTerminated()){

                    echo "线程：".$value->getThreadId()."遇到异常已经退出......\r\n";

                }

                $thread = new myThread($message_queue, $sem);

                $thread->start();

                echo "创建出一个新的子线程，线程ID：".$thread->getThreadId()."........\r\n";

                $threadArray[$thread->getThreadId()] = $thread;

                echo "创建后的数组.................\r\n";

                var_dump($threadArray);

                echo "创建后的数组.................\r\n";
            }

        }

    }

}



