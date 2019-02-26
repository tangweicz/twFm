<?php
/**
 * Created by PhpStorm.
 * User: tangwei
 * Date: 16/9/26
 * Time: 16:10
 *
 * 继承自httpServer，在此基础上新增了websocket的协议解析
 *
 * swoole_websocket_server 继承自 swoole_http_server
 *
 * 设置了onRequest回调，websocket服务器也可以同时作为http服务器
 *
 * 未设置onRequest回调，websocket服务器收到http请求后会返回http 400错误页面
 *
 * 完整的websocket协议请求会被解析并封装在frame对象内
 *
 * 新增push方法用于发送websocket数据
 *
 * 新增回调函数：1、onHandShake WebSocket建立连接后进行握手。WebSocket服务器已经内置了handshake，如果用户希望自己进行握手处理，可以设置onHandShake事件回调函数。
 *             2、onOpen 当WebSocket客户端与服务器建立连接并完成握手后会回调此函数。
 *             3、onMessage 当服务器收到来自客户端的数据帧时会回调此函数
 *
 *
 *
 * 判断一个连接是不是一个websocket的连接，$ser->connection_info获取连接信息，如果有一个叫websocket_status，那这就是个websocket连接
 */


class shellSocketServer
{
    private $sock;

    private $process;

    private $poll = array();

    public function __construct()
    {
        $this->sock = new swoole_websocket_server("0.0.0.0", 9900);

        $this->sock->set(array(

            "worker_num"=>2,

        ));

        $this->sock->on('open', array($this, 'onOpen'));//注册三次握手完成的事件

        $this->sock->on('message', array($this, 'onMessage'));//注册socket接收到消息的事情

        $this->sock->on('workerStart', array($this, 'onWorkerStart'));

        $this->process = new swoole_process(array($this, 'onProcess'), true);//注意到第二个参数的true的使用

        $this->sock->addProcess($this->process);//这个进程经过步之后，就能够与task/worker进行通讯

        $this->sock->start();
    }

    public function onOpen()
    {
        echo "socket is established \r\n";
    }

    public function onWorkerStart(swoole_websocket_server $server, $worker_id)
    {
        swoole_process::signal(SIGCHLD, function(){

            while($res = swoole_process::wait(false)){

                echo "PID为".$res["pid"]."的已经退出";
            }

        });
    }

    public function onMessage(swoole_server $server, swoole_websocket_frame $frame)
    {

        $data = $frame->data;

        $data = json_decode($data);

        $cmd = $data->cmd;

        $isblock = $data->is_block;

        $isblock = $isblock == 1 ? $isblock : 0;

        if($isblock){

            if(isset($this->poll[$frame->fd])){

                $process = $this->poll[$frame->fd];

            }else{

                $process = new swoole_process(array($this, "onTmpProcess"), true);

                $this->poll[$frame->fd] = $process;

                swoole_event_add($process->pipe, function($pipe) use($process, $frame){

                    $data = $process->read();

                    if($data == "exit"){

                        $process->exit();

                    }

                    $this->sock->push($frame->fd, $data);

                });

                $process->start();

            }

            $process->write($cmd);


        }else{
            $this->process->write($cmd);

            $res = $this->process->read();

            $this->sock->push($frame->fd, $res);

        }
    }

    public function onProcess($worker)
    {

        while(true){
            $data = $worker->read();

            if($data == "exit"){

                $worker->exit();

                break;

            }
            echo system($data);

        }

    }

    public function onTmpProcess($worker)
    {
        $cmd = $worker->read();

        $handle = popen($cmd, 'r');

        swoole_event_add($worker->pipe, function($pipe) use($worker, $handle){

            $data = $worker->read();

            if($data == 'exit')
            {
                $worker->exit();
            }

            fwrite($handle,$data);

        });

        while(!feof($handle)) {
            $buffer = fread($handle, 18192);
            echo $buffer;
        }
    }
}

new shellSocketServer();





