<?php
/**
 * Created by PhpStorm.
 * User: tangwei
 * Date: 17/1/22
 * Time: 16:05
 */

/*
            SIGHUP 本信号在用户终端连接(正常或非正常)结束时发出, 通常是在终端的控制进程结束时, 通知同一session内的各个作业, 这时它们与控制终端不再关联。

            SIGINT 程序终止(interrupt)信号, 在用户键入INTR字符(通常是Ctrl-C)时发出。

            SIGQUIT 和SIGINT类似, 但由QUIT字符(通常是Ctrl-)来控制. 进程在因收到SIGQUIT退出时会产生core文件, 在这个意义上类似于一个程序错误信号。

            SIGILL 执行了非法指令. 通常是因为可执行文件本身出现错误, 或者试图执行数据段. 堆栈溢出时也有可能产生这个信号。

            SIGTRAP 由断点指令或其它trap指令产生. 由debugger使用。

            SIGABRT 程序自己发现错误并调用abort时产生。

            SIGIOT 在PDP-11上由iot指令产生, 在其它机器上和SIGABRT一样。

            SIGBUS 非法地址, 包括内存地址对齐(alignment)出错. eg: 访问一个四个字长的整数, 但其地址不是4的倍数。

            SIGFPE 在发生致命的算术运算错误时发出. 不仅包括浮点运算错误, 还包括溢出及除数为0等其它所有的算术的错误。

            SIGKILL 用来立即结束程序的运行. 本信号不能被阻塞, 处理和忽略。

            SIGUSR1 留给用户使用。

            SIGSEGV 试图访问未分配给自己的内存, 或试图往没有写权限的内存地址写数据。

            SIGUSR2 留给用户使用。

            SIGPIPE 向一个没有读进程的管道写数据会出现该信号    即：Broken pipe

            SIGALRM 时钟定时信号, 计算的是实际的时间或时钟时间. alarm函数使用该信号。

            SIGTERM 程序结束(terminate)信号, 与SIGKILL不同的是该信号可以被阻塞和处理. 通常在程序自己正常退出exit 或者 用户发出kill命令的时候回出现这个信号，kill -9 不会有这个信号. shell命令kill缺省产生这个信号。

            SIGCHLD 子进程结束时，比如子进程运行exit、子进程遇到错误崩溃, 父进程都会收到这个信号。

            SIGCONT 让一个停止(stopped)的进程继续执行. 本信号不能被阻塞. 可以用一个handler来让程序在由stopped状态变为继续执行时完成特定的工作. 例如, 重新显示提示符

            SIGSTOP 停止(stopped)进程的执行. 注意它和terminate以及interrupt的区别: 该进程还未结束, 只是暂停执行. 本信号不能被阻塞, 处理或忽略。

            SIGTSTP 停止进程的运行, 但该信号可以被处理和忽略. 用户键入SUSP字符时(通常是Ctrl-Z)发出这个信号。

            SIGTTIN 当后台作业要从用户终端读数据时, 该作业中的所有进程会收到SIGTTIN信号. 缺省时这些进程会停止执行。

            SIGTTOU 类似于SIGTTIN, 但在写终端(或修改终端模式)时收到。

            SIGURG 有紧急数据或out-of-band数据到达socket时产生.

            SIGXCPU 超过CPU时间资源限制. 这个限制可以由getrlimit/setrlimit来读取/改变。

            SIGXFSZ 超过文件大小资源限制。

            SIGVTALRM 虚拟时钟信号. 类似于SIGALRM, 但是计算的是该进程占用的CPU时间。

            SIGPROF 类似于SIGALRM/SIGVTALRM, 但包括该进程用的CPU时间以及系统调用的时间.

            SIGWINCH 窗口大小改变时发出。

            SIGIO 文件描述符准备就绪, 可以开始进行输入/输出操作。

            3SIGPWR Power failure 。
 */


/*
 * @author tangwei
 *
 * @time 2017-01-24 15:32:00
 *
 * @function 进程自己捕捉ctrl+c判断进程是否退出
 */
function test()
{
    echo "this porcess is exit";

    exit;//必须要

}

declare(ticks = 1);//ticks = 1 表示，每执行一行代码，就去检查有没有被pcntl_signal()注册的信号回调，性能极差。一般使用了这个 就不要在使用pcntl_signal_dispatch()这个函数，如果没有使用这个，需要循环使用pcntl_signal_dispatch()函数

pcntl_signal(SIGINT, "test", false);//要想使安装的信号，能顺利捕捉到用户发出的信号，就必须要使用 declare(ticks = 1); 或者 pcntl_signal_dispatch();

pcntl_signal_dispatch();//每个等待信号通过pcntl_signal() 安装的处理器，一般要循环处理。一般使用这个，就不需要再使用 declare(ticks = 1);


//进程自己捕捉ctrl+c判断进程是否退出完毕







posix_kill(posix_getpid(), SIGINT);//向进程发送一个信号

posix_kill("pid", 0);//检查一个进程是否还在运行中

posix_kill("pid", 9);//给一个进程发送强制结束的信号









//研究pcntl_alarm()开始
/*
 *   同一时间只能设置一个闹钟，如果你设置了一个闹钟，在它还没闹之前又设置了一个闹钟，那么新的闹钟会取代旧的闹钟，然后函数的返回值是上一个闹钟还剩下的秒数。
 *   如果设置一个新的闹钟时候把参数设为0，就可以清除所有闹钟，函数的返回值也是上一个闹钟还剩下的秒数。
 *   闹钟信号传到进程的时候，进程的任何工作都会被忽略掉，包括sleep()。
 */


class test
{
    public $arr = array(1,2,3,4,5,6,7,8,9,0);

    public function signalHandle($signal)
    {

            echo "6666666\r\n";

            pcntl_alarm(1);

            if(empty($this->arr)){

                pcntl_alarm(0);

                echo "arr数组已经取完！！";

                exit;

            }

            foreach($this->arr as $key=>$value){

                if($key > 2){

                    sleep(2);

                }
                echo "value值为：".$value."\r\n";

                unset($this->arr[$key]);

            }

    }

    public function __construct()
    {
        $pid = pcntl_fork();

        if ($pid == -1) {

            die('could not fork');

        } else if ($pid) {

            echo "父进程退出\r\n";

            exit;

        } else {

            echo "进入子进程\r\n";

            $sid=posix_setsid();//使进程摆脱界面的控制

            if ($sid == -1) {//使进程摆脱界面的控制，操作失败

                die("could not detach from terminal");

            }elseif($sid < 0){//使进程摆脱界面的控制，操作失败

                exit;

            }


            if(is_callable(array($this, 'signalHandle'))){

                echo "有回调函数\r\n";

                pcntl_signal(SIGALRM, array($this, 'signalHandle'), false);

            }else{

                echo "无回调函数\r\n";

                exit;

            }

            posix_kill(posix_getpid(), SIGALRM);

            while(1){

                pcntl_signal_dispatch();

            }

        }
    }

}

new test();
//研究pcntl_alarm()完毕







