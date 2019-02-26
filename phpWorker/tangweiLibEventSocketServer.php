<?php
/**
 * Created by PhpStorm.
 * User: tangwei
 * Date: 16/9/19
 * Time: 16:46
 *
 * 一般情况下，在php的socket编程时才会使用
 *
 *
 * libevent是一个基于事件驱动的高性能网络库。支持多种 I/O 多路复用技术， epoll、 poll、 dev/poll、 select 和 kqueue 等；支持 I/O，定时器和信号等事件；注册事件优先级。
 * PHP libevent扩展安装，php的libevent扩展依赖于原始的libevent库，必须先把libevent库安装。
 *
 * (1)安装libevent库
 * wget http://cloud.github.com/downloads/libevent/libevent/libevent-2.0.20-stable.tar.gz（从libevent.org官网下载）
 * tar zxvf libevent-2.0.20-stable.tar.gz
 * cd libevent-2.0.20-stable/
 * ./configure --prefix=/usr/local/libevent-2.0.20/
 * make
 * make install
 *
 * (2)安装php的libevent扩展(http://pecl.php.net/package/libevent)
 * wget http://pecl.php.net/get/libevent-0.1.0.tgz
 * tar -zxvf libevent-0.1.0.tgz
 * cd libevent-0.1.0
 * ./configure --with-php-config=/目录/php-config --with-libevent=/usr/local/libevent-2.0.20/
 * make && make install
 * #php.ini添加extension=libevent.so
 *
 *
 *
 * EV_TIMEOUT 超过时间后事件成为激活状态
 *
 * EV_READ FD就绪，可以读取的时候 ，事件成为激活状态
 *
 * EV_WRITE FD就绪，可以写入的时候 ，事件成为激活状态
 *
 * EV_SIGNAL 用于实现信号检测
 *
 * EV_PERSIST 表示事件是持久的
 *
 * EV_ET 表示底层是否支持边沿触发事件
 *
 * EVLOOP_ONCE 如果设置了EVLOOP_ONCE，循环将等待某些事件成为激活的，执行激活的事件直到没有更多的事件可以执行，然会返回。
 *
 * EVLOOP_NONBLOCK 如果设置了EVLOOP_NONBLOCK，循环不会等待事件被触发：循环将仅仅检测是否有事件已经就绪，可以立即触发，如果有，则执行事件的回调。
 *
 *
 *  event_base_free() 释放资源，这不能销毁绑定事件
 *  event_base_loop() 处理事件，根据指定的base来处理事件循环
    event_base_loopbreak() 立即取消事件循环，行为各break语句相同
    event_base_loopexit() 在指定的时间后退出循环
    event_base_new() 创建并且初始事件
    event_base_priority_init() 设定事件的优先级
    event_base_set() 关联事件到事件base
    event_buffer_base_set() 关联缓存的事件到event_base
    event_buffer_disable() 禁用一个缓存的事件
    event_buffer_enable() 启用一个指定的缓存的事件
    event_buffer_fd_set() 改变一个缓存的文件系统描述
    event_buffer_free() 释放缓存事件
    event_buffer_new() 建立一个新的缓存事件
    event_buffer_priority_set() 缓存事件的优先级设定
    event_buffer_read() 读取缓存事件中的数据
    event_buffer_set_callback() 给缓存的事件设置或重置回调hansh函数
    event_buffer_timeout_set() 给一个缓存的事件设定超时的读写时间
    event_buffer_watermark_set 设置读写事件的水印标记
    event_buffer_write() 向缓存事件中写入数据
    event_add() 向指定的设置中添加一个执行事件
    event_del() 从设置的事件中移除事件
    event_free() 清空事件句柄
    event_new() 创建一个新的事件
    event_set() 准备想要在event_add中添加事件
 */

//--这儿的顺序不能动
$socket = stream_socket_server ('tcp://0.0.0.0:8000', $errno, $errstr);//创建一个网络或Unix域套接字服务器

stream_set_blocking($socket, 0);//为资源流设置阻塞或者阻塞模式

$base = event_base_new();//创建和初始化新的事件库，每一个事件都和单个evnet_base相关

$event = event_new();//在事件库中创建一个新事件

event_set($event, $socket, EV_READ | EV_PERSIST, 'ev_accept', $base);//准备一个事件，‘ev_accept’是回调函数

event_base_set($event, $base);//将事件与事件库关联起来

event_add($event);//1、事件注册到IO事件模型epool，并注册到ev_base的eventqueue 2、如果该事件已经在活动事件队列或者超时队列中，先从中删除 3、计算超时时标，并将ev加入超时队列

event_base_loop($base);//等待事件被触发，然后调用它们的回调函数

//--这儿的顺序不能动

function ev_accept($socket, $flag, $base) {

    $connection = stream_socket_accept($socket);//接受由 stream_socket_server() 创建的套接字连接，得到接受套接之后的资源流

    stream_set_blocking($connection, 0);//为资源流设置阻塞或者阻塞模式

    $buffer = event_buffer_new($connection, 'ev_read', NULL, 'ev_error',  $connection);//创建新的缓冲事件

    event_buffer_base_set($buffer, $base);//将缓冲事件跟事件库关联起来

    event_buffer_timeout_set($buffer, 30, 30);//设置事件的超时时间

    event_buffer_watermark_set($buffer, EV_READ, 0, 0xffffff);//设置读写事件的水印

    event_buffer_priority_set($buffer, 10);//分配一个缓冲事件的优先级

    event_buffer_enable($buffer, EV_READ | EV_PERSIST);//启用缓冲事件

    $GLOBALS['_'] = $buffer;  //这个buffer一定要赋给个全局的变量 貌似是传值过程中的bug 或者5.3.8的闭包还是有问题？
}


function ev_error($buffer, $error, $connection) {

    event_buffer_disable($buffer, EV_READ | EV_WRITE);//关闭一个缓冲事件

    event_buffer_free($buffer);//销毁一个缓冲事件

    fclose($connection);//关闭一个socket连接资源
}


function ev_read($buffer, $connection) {
//      var_dump($connection);
    while ($read = event_buffer_read($buffer, 256)) {//从缓冲事件中读取数据
        // var_dump($read);
    }
    fwrite($connection , date('Y-m-d H:i:s'));//往socket连接中写入数据

    ev_error($buffer , '' , $connection);
}







