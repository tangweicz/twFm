先来说说什么是信号：

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

            SIGTERM 程序结束(terminate)信号, 与SIGKILL不同的是该信号可以被阻塞和处理. 通常用来要求程序自己正常退出. shell命令kill缺省产生这个信号。

            SIGCHLD 子进程结束时, 父进程会收到这个信号。

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





怎么进行处理？

        利用C++中提供的函数：signal(信号常量，一个指向信号处理函数的指针即函数名)；//通常第二个参数，是放个函数名。也可以放置常量，如：SIG_IGN 是忽略信号的处理程序。


        eg:

            using namespace std;

            int keepRunning = 1;

            void sig_handler( int sig )
            {
                if ( sig == SIGINT)
                {
                    keepRunning = 0;
                }
            }

            int main( )
            {

                signal( SIGINT, sig_handler );// 进程中随时随地都会去捕获这个信号，然后进行相关操作。

                while( keepRunning )
                {
                  cout << "Running" << endl;
                }

                cout << "Terminated by Ctrl+C signal." << endl;

                cout << "Finishes data saving or some other work, and then exits."

                return 0;
            }