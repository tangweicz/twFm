<?php
/**
 * Created by PhpStorm.
 * User: tangwei
 * Date: 18/5/3
 * Time: 09:29
 *
 * 被submit到pool中的thread都是并行执行。
 */


class WebWorker extends Worker {

    public $loger;

    public function __construct(SafeLog $logger) {

        $this->logger = $logger;

    }

    public function run(){

        echo "webWorker is running.......\r\n";

    }

}

class WebWork extends Thread {

    public $complete;

    public function isComplete() {

        return $this->complete;

    }

    public function run() {

        $this->worker->logger->log(__CLASS__." executing in Thread ".$this->worker->getThreadId().".......\r\n");

        sleep(2);

        $this->complete = true;

        echo $this->worker->getThreadId()."执行完毕........\r\n";

    }

}

class SafeLog extends Thread{

    protected function log($message) {

        echo $message;

    }

}

$pool = new Pool(8, "WebWorker", [new SafeLog()]);//创建一个有8个WebWorker线程的线程池，其中new SafeLog()是每个WebWorker()初始化的参数。

for($i=0;$i<8;$i++){

    $pool->submit(new WebWork());//运行一下submit()会运行worker的run方法和thread的run方法。

    sleep(1);

}

sleep(30);

$pool->collect(function($work){//自己循环回收

    echo "回收.....\r\n";

    var_dump($work->complete);

    return $work->complete;

});

$pool->shutdown();