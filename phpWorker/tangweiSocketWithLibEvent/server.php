<?php
/**
 * Created by PhpStorm.
 * User: tangwei
 * Date: 16/11/30
 * Time: 10:01
 */



/*
 *
 * 在php 的libevent扩展具有如下函数：

　　event_base_free() 　　　　释放资源，这不能销毁绑定事件
　　event_base_loop() 　　　　处理事件，根据指定的base来处理事件循环
　　event_base_loopbreak() 　　　　立即取消事件循环，行为和break语句相同
　　event_base_loopexit() 　　　　在指定的时间后退出循环
　　event_base_new() 　　　　创建并且初始事件
　　event_base_priority_init() 　　　　设定事件的优先级
　　event_base_set() 　　　　关联事件到事件base
　　event_buffer_base_set() 　　　　关联缓存的事件到event_base
　　event_buffer_disable() 　　　　禁用一个缓存的事件
　　event_buffer_enable() 　　　　启用一个指定的缓存的事件
　　event_buffer_fd_set() 　　　　改变一个缓存的文件系统描述
　　event_buffer_free() 　　　　释放缓存事件
　　event_buffer_new() 　　　　建立一个新的缓存事件
　　event_buffer_priority_set()　　 　　缓存事件的优先级设定
　　event_buffer_read() 　　　　读取缓存事件中的数据
　　event_buffer_set_callback() 　　　　给缓存的事件设置或重置回调hansh函数
　　event_buffer_timeout_set() 　　　　给一个缓存的事件设定超时的读写时间
　　event_buffer_watermark_set 　　　　设置读写事件的水印标记
　　event_buffer_write() 　　　　向缓存事件中写入数据
　　event_add() 　　　　向指定的设置中添加一个执行事件
　　event_del() 　　　　从设置的事件中移除事件
　　event_free() 　　　　清空事件句柄
　　event_new()　　　　 创建一个新的事件
　　event_set() 　　　　准备想要在event_add中添加事件



    event_set一些参数的解释：

　  (a) EV_TIMEOUT: 超时
    (b) EV_READ: 只要网络缓冲中还有数据，回调函数就会被触发
    (c) EV_WRITE: 只要塞给网络缓冲的数据被写完，回调函数就会被触发
    (d) EV_SIGNAL: POSIX信号量
    (e) EV_PERSIST: 不指定这个属性的话，回调函数被触发后事件会被删除
    (f) EV_ET: Edge-Trigger边缘触发
 *
 *
 *
 */

require_once "./libEvent.php";

class server
{
    public $libevent;

    public $handShake = false;

    public $sockets = array();

    public function __construct()
    {

        $this->libevent = new libEvent();


        //设置定时器，每5秒执行一次，代码开始
        $this->libevent->add(5, libEvent::EV_TIMER_ONCE, array($this, "tangwei"), null);

        $this->libevent->goLoop();
        //设置定时器，每5秒执行一次，代码完毕

        //监听信号执行，代码开始
        pcntl_signal(SIGINT, SIG_IGN, false);

        $this->libevent->add(SIGINT, libEvent::EV_SIGNAL, array($this, "tangwei"), null);

        posix_kill(posix_getpid(), SIGINT);

        $this->libevent->goLoop();
        //监听信号执行，代码完毕

        //监听socket读写代码开始
        $socket = stream_socket_server("tcp://0.0.0.0:8000");

        $this->libevent->add($socket, libEvent::EV_READ, array($this, "tangwei"), null);//监听到有新连接到socket

        $this->libevent->goLoop();
        //监听socket读写代码完毕


    }

    public function tangwei($socket)
    {
        $newSocket = stream_socket_accept($socket);//理论上，因为在event_add的时候，没有指定触发时间，所以会无限循环下去执行，但是走到这儿的时候，如果没有连接进来就会阻塞，所以逻辑可以实现。

        stream_set_blocking($newSocket, 0);//为资源流设置阻塞或者阻塞模式

        $this->libevent->add($newSocket, libEvent::EV_READ, array($this, "test"), null);//将连接进来的socket资源也加入到epoll中去

    }

    public function test($socket)
    {
        echo "-------------\r\n";

        if(!isset($this->sockets[(int)$socket]["handShake"]) || $this->sockets[(int)$socket]["handShake"] == false){//判断该链接是否已经完成握手，如果已经握手了，那么就不需要再次握手

            $res = fread($socket,65535);//理论上，因为在event_add的时候，没有指定触发时间，所以会无限循环下去执行，但是走到这儿的时候，如果没有读取到内容就会阻塞在这儿，所以逻辑可以实现

            if($res == "") return;

            var_dump($socket);

            echo ("\nRequesting handshake...\r\n");

            list($resource, $host, $origin, $key) = $this->getHeaders($res);//解析websocket发送的header信息，获取到sec-websocket-key值

            echo "key:".$key."\r\n";

            $acceptKey = base64_encode(sha1($key.'258EAFA5-E914-47DA-95CA-C5AB0DC85B11', true));//计算sec-websocket-accept值

            echo ("Handshaking...\r\n");

            echo $upgrade  = "HTTP/1.1 101 Switching Protocol\r\n" .//组织返回给客户端的头信息
                "Upgrade: websocket\r\n" .
                "Connection: Upgrade\r\n" .
                "Sec-WebSocket-Accept: " . $acceptKey . "\r\n\r\n";  //必须以两个回车结尾

            fwrite($socket, $upgrade, strlen($upgrade));//将sec-websocket-accept回传给客户端，以示握手成功，从而web端就会触发onOpen事件

            $this->handShake = true;
            echo ("Done handshaking...\r\n");

            $this->sockets[(int)$socket]["handShake"] = true;

            return true;

        }else{//已经握手了，那么直接收发消息

            echo "77777777777777777\r\n";

            $res = fread($socket,65535);//这儿的数据都是数据帧，打印出来也是乱七八糟的东西 那咱们解决

            var_dump($this->decode($res, $socket));//这儿接收到数据。。。。

            $msg = $this->frame("tangwei");//对需要发送的数据进行编码处理。。。。，这儿也要考虑下，如何把该链接从epoll中去掉

            @fwrite($socket, $msg, strlen($msg));

        }

    }

    /*
     * @author tangwei
     *
     * @time 2016-12-05 19:49:34
     *
     * @function 当websocket握手时，第一次需要获取到当前的头信息和sec-websocket-key值
     */
    public function getHeaders($req)
    {
        echo "get header \r\n";
        $r = $h = $o = null;
        if(preg_match("/GET(.*) HTTP/"   , $req, $match))
            $r = $match[1];
        if(preg_match("/Host:(.*)\r\n/"  , $req, $match))
            $h = $match[1];
        if(preg_match("/Origin:(.*)\r\n/", $req, $match))
            $o = $match[1];
        if(preg_match("/Sec-WebSocket-Key:(.*)\r\n/", $req, $match))
            $key = $match[1];

        $arr = array(trim($r), trim($h), trim($o), trim($key));

        echo "all headers\r\n";

        var_dump($arr);

        return $arr;
    }

    /*
     * @author tangwei
     *
     * @time 2016-12-08 16:41:13
     *
     * @function 对接收到的数据将二进制转化成utf-8
     */
    public function decode($content, $socket)  {

        $opcode = ord(substr($content, 0, 1)) & 0x0F;//获取到数据帧类型

        $payloadlen = ord(substr($content, 1, 1)) & 0x7F;//获取到数据长度

        $ismask = (ord(substr($content, 1, 1)) & 0x80) >> 7;//获取到是否使用了掩码

        $maskkey = null;//存放掩码

        $oridata = null;//存放获取到的值

        $decodedata = null;//获取要返回的值

         //关闭连接
         if ($ismask != 1 || $opcode == 0x8)
         {
             $this->disconnect($socket);
             return null;
         }

         //获取掩码密钥和原始数据
         if ($payloadlen <= 125 && $payloadlen >= 0)
         {
             $maskkey = substr($content, 2, 4);
             $oridata = substr($content, 6);
         }
         else if ($payloadlen == 126)
         {
             $maskkey = substr($content, 4, 4);
             $oridata = substr($content, 8);
         }
         else if ($payloadlen == 127)
         {
             $maskkey = substr($content, 10, 4);
             $oridata = substr($content, 14);
         }
         $len = strlen($oridata);
         for($i = 0; $i < $len; $i++)
         {
             $decodedata .= $oridata[$i] ^ $maskkey[$i % 4];
         }
         return $decodedata;
    }

    /*
     * @author tangwei
     *
     * @time 2016-12-14 17:05:11
     *
     * @function 关闭websocket的连接
     */
    public function disconnect($socket)
    {

        echo "\r\nclose\r\n";
        socket_close($socket);//这儿关闭socket的连接
    }


    /*
     * @author tangwei
     *
     * @time 2016-12-08 16:40:44
     *
     * @function 对要发送的数据，进行二进制化处理
     */

    function frame($s) {
        $a = str_split($s, 125);
        if (count($a) == 1) {
            return "\x81" . chr(strlen($a[0])) . $a[0];
        }
        $ns = "";
        foreach ($a as $o) {
            $ns .= "\x81" . chr(strlen($o)) . $o;
        }
        return $ns;
    }
}

new server();