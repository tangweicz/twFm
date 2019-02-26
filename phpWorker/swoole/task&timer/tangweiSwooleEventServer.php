<?php
/**
 * Created by PhpStorm.
 * User: tangwei
 * Date: 16/9/22
 * Time: 11:24
 *
 *  *
 * IO多路复用：epool会监听注册在自己名下的所有的socket描述符，当有socket感兴趣的事件发生时，epool才会响应并返回有事件发生的socket集合，交给客户端处理。
 *           epool本质还是阻塞IO，它的优点在于能同时处理大量的socket连接。
 *
 *
 * swoole的eventLoop详解：
 *                      1、eventLoop是一个reactor线程,一致阻塞在epool的监听上。
 *                      2、可以通过接口添加socket描述到epool监听中，并制定事件响应回调函数
 *                      3、eventLoop不可用于FPM环境下
 *
 *                      包括函数：
 *                              swoole_event_add($sock, $read_callback, $write_callback, $event_flags)
 *                              swoole_event_set($fd, $read_callback, $write_callback, $flag)//跟swoole_event_add()一致，如果传入的$fd在eventLoop不存在会报错
 *                              swoole_event_del($sock)
 *                              swoole_event_exit(void)//只能在client中执行
 *                              swoole_event_write()//往一个已经存在的描述符中写内容
 */

class Server
{
    private $ser;

    public function __construct()
    {
        $this->ser = new swoole_server('0.0.0.0', 9900);

        $this->ser->set(

            array(
                'worker_num'=>8,

                'daemonize'=>false,

                "max_request"=>10000,

                "dispatch_mode"=>2,

            )

        );

        $this->ser->on('start',array($this, 'onStart'));

        $this->ser->on('Connect', array($this, 'onConnect'));

        $this->ser->on('receive', array($this, 'onReceive'));

        $this->ser->on('Close', array($this, 'onClose'));

        $this->ser->start();
    }
    public function onConnect($ser, $fd, $from_id)
    {
        echo "Client $fd connect \r\n";
    }

    public function onStart(swoole_server $server)
    {
        echo "manage process is started";
    }

    public function onReceive($ser, $fd, $from_id, $data)
    {
        echo $data;

        foreach ($ser->connections as $client) {

            if( $fd != $client) $ser->send($client, $data);

        }
    }

    public function onClose($ser, $fd)
    {
        echo "Client $fd closed";
    }

}

new Server();