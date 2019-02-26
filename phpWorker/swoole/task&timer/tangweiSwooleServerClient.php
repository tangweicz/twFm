<?php
/**
 * Created by PhpStorm.
 * User: tangwei
 * Date: 16/9/21
 * Time: 11:24
 */
class Client
{
    private $client;

    public function __construct()
    {
        $this->client = new swoole_client(SWOOLE_SOCK_TCP);
    }

    public function connect()
    {
        if(!$this->client->connect('121.41.9.17', 9900, 1)){

            echo "ERROR: ".$fp->errMes;
        }

        fwrite(STDOUT, "请输入消息");

        $msg = trim(fgets(STDOUT));

        $this->client->send($msg);

        sleep(1);

        $message = $this->client->recv();

        echo "Get Message:$message";
    }

}

$client = new Client();

$client->connect();