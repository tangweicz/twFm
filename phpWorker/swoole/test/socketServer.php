<?php
/**
 * Created by PhpStorm.
 * User: tangwei
 * Date: 16/10/21
 * Time: 09:13
 */

class server
{
    public $socketServer;

    public function __construct()
    {
        $this->socketServer = new swoole_server("127.0.0.1", "9900");

        $this->socketServer->set(array(

            "max_conn" => 200,

            "daemonize" => 1 ,

            "reactor_num" => 2,

            "worker_num" => 4,

            "max_request" => 2000,

            "dispatch_mode" => 1

        ));

        $this->socketServer->on("Start", array($this, "onStart"));

        $this->socketServer->on("Connect", array($this, "onConnect"));

        $this->socketServer->on("Receive", array($this, "onReceive"));

        $this->socketServer->on("WorkerStart", array($this, "onWorkerStart"));

        $this->socketServer->start();
    }

    public function onStart(swoole_server $server)
    {
        echo "socketServer is started......\r\n";
    }

    public function onConnect(swoole_server $server, $fd, $from_id)
    {
        echo "$fd is connecting.....\r\n";
    }

    public function onWorkerStart(swoole_server $server, $worker_id)
    {
        echo "worker process id is $worker_id \r\n";
    }

    public function onReceive(swoole_server $server, $fd, $from_id, $data)
    {
        echo "receive data:";

        var_dump($data);

        echo "\r\n";

        $this->socketServer->send($fd, "hhahahahah");
    }

}

new server();