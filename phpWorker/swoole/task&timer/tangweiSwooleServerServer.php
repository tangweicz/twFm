<?php
/**
 * Created by PhpStorm.
 * User: tangwei
 * Date: 16/9/20
 * Time: 17:21
 *
 * 编译安装swoole的php扩展
 *
 * swoole的构成，有master进程，在master中有main reactor这个主线程，主线程下又有很多个reactor线程，在诸多reactor线程中注册了很多个epoll去分别处理不同的worker或者task。
 *              有manager进程，主要用于管理worker和task进程的
 *
 * Master进程内的回调函数 1、onStart 2、onShutdown 3、onMasterConnect 4、onMasterClose
 *
 * Worker进程内的回调函数 1、onWorkerStart 2、onWorkerStop 3、onConnect 4、onClose 5、onReceive 6、onTimer 7、onFinish
 *
 * Task进程内的回调函数 1、onTask
 *
 * Manager进程内的回调函数 1、onManagerStart 2、onManagerStop
 *
 *
 * 用户的请求，发送到服务器端，被main reactor进程获取，并注册进一个reactor线程中，然后通知对应的worker进程去处理，worder进程完毕，返回给对应的reactor线程，线程再返回给客户端
 *
 *
 * worker进程                                   task进程
 *
 * task()--------------------------------------->onTask()//可以直接传递一个对象，在task中对这个对象的操作，不会反映到worker进程中。数据库连接、网络连接对象不能传递。
 *                                                  |
 *                                                  |
 *                                                  \/
 * onFinish()<---------------------------------------finish()
 *
 * 每个worker都有一个worker_id
 *
 * swoole_set_process_name('名称');
 *
 * task worker进程中，要传递的数据必须是字符串，那么数组和对象就要序列化下，数据库连接和网络连接不能当成数据传递。
 *
 * timer定时器的使用
 *
 * 每个timer都有一个timer_id
 *
 * swoole_timer_tick(时间， 回调函数);//创建一个永久的定时器，一直在swoole中运行，以指定的间隔时间去运行回调函数
 *
 * swoole_time_after(时间， 回调函数);//只运行一次性的定时器
 *
 * timer中的对象传递
 * 在timer的回调函数中不能被传递一个对象进来，那怎么办？需要单独定义一个onTick方法（),所有的调用都在onTick（）中进行，然后修改为swoole_timer_tick(时间， array($this, 'onTick'), 参数)
 *
 *
 */

class Test
{
    public $index;
}

class server
{
    public function __construct()
    {
        $this->ser = new swoole_server("0.0.0.0", 9900);

        $this->ser->set(array(

           'worker_num'=>8,

           "daemonize"=>false,

           "max_request"=>10000,

           "dispatch_mode"=>2,

            "task_worker_num"=>8

        ));

        $this->ser->on('Start', array($this, 'onStart'));

        $this->ser->on('Connect', array($this, 'onConnect'));

        $this->ser->on('Receive', array($this, 'onReceive'));

        $this->ser->on('Close', array($this, 'onClose'));

        $this->ser->on('Task', array($this, "onTask"));

        $this->ser->on('finish', array($this, "onFinish"));

        $this->ser->on("workerStart", array($this, "onWorkerStart"));

        $this->ser->start();
    }

    public function onStart($ser)
    {
        echo "Start\r\n";
    }

    public function onConnect($ser, $fd, $from_id)
    {
        echo "Client $fd connect \r\n";
    }
    public function onWorkerStart($ser, $worker_id)
    {
        if($worker_id == 0){

            swoole_timer_tick(1000, array($this, 'onTick'), "Hello");
        }
    }

    public function onTick($timer_id,  $params = null)
    {
        echo "1\r\n";
    }
    public function onReceive($ser, $fd, $from_id, $data)
    {
        echo "receive data:$data\r\n";

        $test = new test();

        $test->index = 2;

        $this->ser->task($test);

        $this->ser->send($fd, "hahah");
    }

    public function onClose($ser, $fd)
    {
        echo "Client $fd closed";
    }

    public function onTask($ser,$task_id,$from_id, $data)
    {
        echo "This Task {$task_id} from Worker {$from_id}\n";

        var_dump($data->index);

        return "66666";
    }

    public function onFinish($ser, $task_id, $data)
    {
        echo $data;
    }


}

new server();