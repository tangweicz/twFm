<?php
/**
 * Created by PhpStorm.
 * User: tangwei
 * Date: 18/5/2
 * Time: 09:46
 *
 *  一个Worker线程，通过stack()方法把其他的线程加入到自己的线程栈中，即并入到同一个Worker线程（编程一个线程）去执行，加入到worker中的线程都可以使用$this->worker去访问worker中的数据。这个特点被用在线程使用mysql连接数据库上。
 *
 *  worker 与 pdo 同时使用是，需要注意一点，需要静态声明public static $dbh;并且通过单例模式访问数据库连接。参考：http://netkiller.github.io/journal/thread.php.html#idp56896064
 *
 *  Worker线程的__construct()发放中如果要传对象，也需要是Thread线程类。
 *
 *
 */

class MyWorker extends Worker {

    public function run() {

        echo "worker thread is running \r\n";

    }

    public function te(){

        echo "worker thread is running te \r\n";

    }


}


class thread1 extends Thread {

    public function __construct()
    {
        echo "Work1被new到了.......\r\n";

    }

    public function run() {

        $this->worker->te();

        printf("%s is Thread #%lu\r\n", __CLASS__, $this->getThreadId());

        sleep(2);

    }

    public function tes(){

        echo "this is tes function .......\r\n";

    }

}

class thread2 extends Thread {

    public function __construct()
    {
        echo "Work2被new到了.......\r\n";

    }

    public function run() {

        $this->worker->te();

        printf("%s is Thread #%lu\r\n", __CLASS__, $this->getThreadId());

        sleep(2);

    }

    public function tes(){

        echo "this is tes function .......\r\n";

    }

}

$worker = new MyWorker();

$worker->start();//开启这个MyWorker线程，等待任务被加入

$work1 = new thread1();

$work2 = new thread2();

$num = $worker->stack($work1);//往MyWorker线程的栈中添加对象，会直接执行这个被加入对象的run方法。

echo "现在栈中有".$num."个数据........\r\n";

$num = $worker->stack($work2);//往MyWorker线程的栈中添加对象，会直接执行这个被加入对象的run方法。

echo "现在栈中有".$num."个数据........\r\n";

$num = $worker->stack($work1);//往MyWorker线程的栈中添加对象，会直接执行这个被加入对象的run方法。

echo "现在栈中有".$num."个数据........\r\n";

$worker->shutdown();//等待加入到MyWorker的栈中的线程执行完毕，关闭MyWorker，类似Thread的join()。

echo "worker关闭完成";