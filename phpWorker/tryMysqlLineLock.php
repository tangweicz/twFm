<?php

/*
 * Worker 是一个具有持久化上下文的线程对象，通常用来重复使用线程
 *
 * 当一个 Worker 对象开始之后，会执行它的 run 方法，但是即使 run 方法执行完毕，线程本身也不会消亡，除非遇到以下情况
 *
 * 1、Worker 对象超出作用范围（没有指向它的引用了）
 *
 * 2、代码调用了 Worker 对象的 shutdown 方法
 *
 * 3、整个脚本终止了
 *
 * 这意味着程序员可以在程序执行过程中重用这个线程上下文：在 Worker 对象的栈中添加对象会激活 Worker 对象执行被加入对象的 run 方法。
 *
 * Pool 对象是多个 Worker 对象的容器，同时也是它们的控制器
 *
 * mutex和cond都已经被最新版的php7的pthreadsV3版上移除了 所以如果用的是最新版的话并不要继续学习下去。在php5用的pthreadsV2版上mutext和cond还是存在的！！！
 */




class TestWork1 extends Worker//所有要干的事情都在这儿进行，比如爬虫的操作
{
    static $dbh;


    public function run()//真正干活的地方在这儿
    {
        self::$dbh = new PDO('mysql:host=localhost;dbname=live','root','');

        self::$dbh->beginTransaction();

        $sql = "select * from live where id =1 for update";

        printf("%s is Thread #%lu start\n", __CLASS__, Thread::getCurrentThreadId());

        self::$dbh->query($sql);

        sleep(10);

        printf("%s is Thread #%lu stop\n", __CLASS__, Thread::getCurrentThreadId());

        self::$dbh->commit();
    }
}

class TestWork2 extends Worker//所有要干的事情都在这儿进行，比如爬虫的操作
{
    static $dbh;

    public function run()//真正干活的地方在这儿
    {
        self::$dbh = new PDO('mysql:host=localhost;dbname=live','root','');

        self::$dbh->beginTransaction();

        $sql = "select * from live where id =1 for update";

        printf("%s is Thread #%lu start\n", __CLASS__, Thread::getCurrentThreadId());

        self::$dbh->query($sql);

        printf("%s is Thread #%lu stop\n", __CLASS__, Thread::getCurrentThreadId());

        self::$dbh->commit();
    }
}

class tangweiPool extends Pool
{
    public $data = array();

    public function process()
    {
        $this->shutdown();//线程池退出

        return $this->data;
    }
}

class runtangwei
{
    public function __construct()
    {
        $pool = new tangweiPool(2);//设置线程池中最多能同时运行的线程有多少个，如果达到限制个数，就会阻塞继续往线程池中放线程，等待正在运行的线程被回收，再往线程池中添加

        $pool->submit(new TestWork1());//将worker（干活的代码）扔进线程池，并且执行者部分的代码，只是执行run中的代码，这儿是异步操作

        $pool->submit(new TestWork2());//将worker（干活的代码）扔进线程池，并且执行者部分的代码，只是执行run中的代码，这儿是异步操作

        $pool->process();//待所有线程执行完毕，获取到执行完毕的结果
    }

}

new runtangwei();






