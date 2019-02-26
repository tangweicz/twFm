<?php
/**
 * Created by PhpStorm.
 * User: tangwei
 * Date: 16/9/22
 * Time: 10:17
 *
 * swoole process方便我们在php中进行多进程的编程，内置了管道和消息队列的接口，可以很方便的实现进程间通信。
 *
 * swoole process提供了自定义的信号管理。
 *
 * 父子进程间，子进程复制父进程的上下文的内容，如果父进程在操作文件，子进程同样会复制文件句柄，这样就涉及到了进程锁的问题。
 *
 * 进程间的通信方式：1、管道：
 *                         进程之间相互通信是通过管道进行的。在父进程中创建一个管道，这个管道会创建2个描述符，一个用来读一个用来写。父进程创建了这个2个描述符，子进程也同时拥有这两个描述符。当父进程在写这个描述符的时候，子进程通过读描述符就能读到父进程写的内容。
 *                         管道需要在fork之前创建，如果某一端主动关闭管道，另一端的读取操作会直接返回0
 *
 *                2、消息队列：
 *                          消息队列独立于进程之外的空间，用过指定的key值创建一个消息队列。
 *                          在消息队列中传递的数据有大小限制。
 *                          一个进程中创建了一个消息队列，该队列会一直保留，直到被主动关闭。
 *
 *
 * SIGCHLD SIGTERM SIGALAM三种信号
 */

class baseProcess
{
    private $process;

    public function __construct()
    {
        $this->process = new swoole_process(array($this, 'run'), false, true);

        $this->process->start();

        swoole_event_add($this->process->pipe, function($pipe){

            echo "RECV:".$this->process->read();

        });
    }

    public function run()
    {
        swoole_timer_tick(1000, function($timer_id){

            static $index = 0;

            $index = $index + 1;

            $this->process->write("tangwei");

            if($index >= 10){

                swoole_timer_clear($timer_id);
            }

        });
    }
}

new baseProcess();

swoole_process::signal(SIGTERM, function($sig){

    while($res = swoole_process::wait(false)){//父进程回收子进程

        echo "PID:".$res["pid"];
    }

});




