<?php
/**
 * Created by PhpStorm.
 * User: tangwei 线程的示例
 * Date: 16/9/6
 * Time: 15:21
 *
 * pthreads这个模块 是不能在php7.0、php7.1的版本下编译的，因为不兼容。
 *
 * pthreads需要线程安全环境，在编译php的时候就需要加上特定的参数：--enable-maintainer-zts。因为，线程走的是php-cli模式，因为走web的话，走的是cgi的模式。可能走cgi的模式的时候会报错，是因为pthreads不适合走cgi模式，所以，我们要复制一份php.ini为php-cli.ini，因为走cli的时候呢，会默认找php-cli.ini文件，找不到才走php.ini，所以我们可以在php-cli.ini中引用pthreads模块
 *
 * 下载和安装pthreads:
 *
 * tar zxvf pthreads
 *
 * cd pthreads-0.0.45 // 本文的是第一个stable版本，最好用2.0.0的版本

 * usr/local/php-zts/bin/phpize
 *
 * ./configure --with-php-config=php-config的路径
 *
 * make
 *
 * make install
 *
 *
 *
 * $thread->wait()作用是 thread->start()后线程并不会立即运行，只有收到 $thread->notify(); 发出的信号后才运行
 *
 *
 * 线程不要去主动杀死它，要让子线程自杀（自动退出）。
 */


class crawlerThread extends Thread
{
    private $url;

    public function __construct($url)
    {
        $this->url = $url;
    }

    public function run()
    {
        printf("%s is Thread #%lu\n", __CLASS__, Thread::getCurrentThreadId());

        echo $this->url."\r\n";

        sleep(10);
    }
}

$arr = array(

    "http://www.baidu.com",

    "http://www.sina.com"
);

if(!empty($arr)){

    foreach($arr as $value){

            $crawler = new crawlerThread($value);//实例化线程对象

            $crawler->start();//开始执行一个线程，使用start方法，随即会调用类中的run方法，并且是异步执行，但是不会立即走下一次循环，主线程会卡住，一个线程不能被重复start

            echo 456;

            $crawler->join();//暂停当前主进程的执行，如果是用在网页端可以不用join等待主进程结束即可

            var_dump($crawler->isStarted());
    }

}


