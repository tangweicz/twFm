<?php
/**
 * Created by PhpStorm.
 * User: tangwei
 * Date: 16/9/28
 * Time: 09:28
 */

class srever
{
    private $ser;

    private $pdo;

    public function __construct()
    {
        $this->ser = new swoole_websocket_server('0.0.0.0', 9900);

        $this->ser->set(array(

            'worker_num'=>2

        ));

        $this->ser->on('workerStart', array($this, 'onWorkerStart'));

        $this->ser->on('open', array($this, 'onOpen'));

        $this->ser->on('message', array($this, 'onMessage'));

        $this->ser->start();

    }

    public function onWorkerStart($serv, $worker_id)
    {
        if($worker_id == 0){//如果是第一个worker进程，那么写个循环在这儿一直读取数据，当然在没有连上数据库的时候 需要连上数据库

            swoole_timer_tick(1000, function($time_id){



            });

        }

        $this->pdo = new PDO("mysql:dbname=testdb;host=127.0.0.1", "root", "");
    }
}
