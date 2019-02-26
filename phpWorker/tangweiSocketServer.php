<?php
/**
 * Created by PhpStorm.
 * User: tangwei
 * Date: 16/9/18
 * Time: 09:00
 */

$socket = socket_create(AF_INET, SOCK_STREAM, SOL_TCP);//创建一个socket连接

socket_bind($socket, "0.0.0.0" , 9000);//绑定一个socket到本地服务器,0.0.0.0是表示绑定外网网卡

socket_listen($socket, 2);//监听由指定socket的所有连接

while(true)
{
    if(($newc = socket_accept($socket)) !== false)
    {
        $buf = socket_read($newc,1024);

        $talkback = "收到的信息:$buf\n";

        echo $talkback;
    }
}

socket_close($socket);

//以上为服务器端简单示例，下面开始真实应用

class ftpSocket
{
    public $pidFile = "/tmp/socketServer/server.pid";//存放后台用于标记进程是否启动的文件

    public $logFile = "/tmp/socketServer/logfile.txt";

    /*
     * @author tangwei
     *
     * @time 2016-09-18 13:49:04
     *
     * @function 告知用户真正的使用方式
     */
    public function usage()
    {
        echo "the correct usage is `php xxx.php start/stop` \r\n";

        exit;
    }

    /*
     * @author tagnwei
     *
     * @time 2016-09-18 14:16:13
     *
     * @function 启动一个后台的socket进程，将服务器一直开在后台
     */
    public function startDae()
    {

        if(file_exists($this->pidFile)) die("进程已经启动无法重复启动\r\n");

        $pid = pcntl_fork();

        if ($pid == -1) {

            die('could not fork');

        } else if ($pid) {

            exit;//fork成功，父进程退出

        } else {
            $childPid = posix_getpid();//获取子进程的pid号，写入文件

            $path = dirname($this->pidFile);//获取子进程的pid号文件的目录部分

            if(!file_exists($path))mkdir($path, 0777);//如果目录部分不存在，那么创建这个目录

            $fp = fopen($this->pidFile, "w+");//以读写的方式打开子进程的pid号文件

            fwrite($fp, $childPid);//写入pid号

            fclose($fp);//关闭文件句柄

            $socket = socket_create(AF_INET, SOCK_STREAM, SOL_TCP);//创建一个socket连接

            socket_bind($socket, "0.0.0.0" , 9000);//绑定一个socket到本地服务器,0.0.0.0是表示绑定外网网卡

            socket_listen($socket, 1);//监听由指定socket的所有连接

            while(true)
            {
                if(($newc = socket_accept($socket)) !== false)//接受一个socket连接进入，如果没有接收到连接请求，会阻塞在这儿
                {
                    $firstRecv = socket_read($newc,1024);//获取到用户的指令，形式为“first|get|文件名 意思为要从服务器上拉取某个文件”，如果没有接收到指令，会阻塞在这儿

                    $firstRecvs = explode("|", $firstRecv);//解析第一次获取的信号是什么

                    $log = fopen($this->logFile, "a+");//打开日志文件，准备写入日志

                    fwrite($log, "接受到的指令为：".$firstRecv."\r\n");//写入日志

                    switch($firstRecvs[1]){//判断用户第一次给的指令是什么

                        case "get":

                            $file = $firstRecvs[2];

                            if(!file_exists($file)){//需要拉取的文件不存在

                                fwrite($log, "该".$file."文件不存在\r\n");

                            }else{//文件是存在的

                                $size = filesize($file);

                                fwrite($log, "告诉客户端".$file."大小为".$size."\r\n");

                                socket_write($newc, "back|filesize|$size");//告诉客户端需要接收的文件的大小，格式为”back|filesize|10000“

                                sleep(1);

                                $secendRecv = socket_read($newc, 1024);//第二次获取到用户指令，形式为”secend|get|ready 意思为客户端已经准备好了接收文件“，如果没有接收到指令，会阻塞在这儿

                                sleep(2);

                                fwrite($log, "第二次接收到客户端回复，指令为：".$secendRecv."\r\n");

                                $secendRecv = explode("|", $secendRecv);

                                if($secendRecv[2] == "ready"){//准备开始传输文件

                                    fwrite($log, $file."开始传输文件"."\r\n");

                                    $startSize = 0;

                                    while($size+2048 >= $startSize){

                                        $content = file_get_contents($file, false, NULL, $startSize, 1024);

                                        $startSize += 1024;

                                        fwrite($log, $file."文件目前传输大小为：$startSize \r\n");

                                        socket_write($newc, $content);

                                        sleep(2);

                                    }

                                    fwrite($log, $file."文件传输完毕\r\n");

                                    socket_close($newc);//确保文件传输完毕，需要干掉这个socket连接

                                }
                            }

                            break;

                        default:

                            fwrite($log, "指令不存在!\r\n");
                            break;

                    }

                    fclose($log);


                }
            }


        }

    }

    /*
     * @author tangwei
     *
     * @time 2016-09-18 14:17:07
     *
     * @function 杀死该后台进程
     */
    public function stopDae()
    {
        if(!file_exists($this->pidFile)){

            exit(0);

        }else{

            $pidfile_content=file_get_contents($this->pidFile);

            if($pidfile_content == FALSE || !is_numeric( $pidfile_content)){

                exit(0);

            }else{

                posix_kill($pidfile_content,  SIGKILL);

                unlink($this->pidFile);
            }


        }


    }
}

$ftp = new ftpSocket();

if (count($argv) < 2) {

    $ftp->usage();

    exit(0);

}

switch($argv[1]){

    case "start":

        $ftp->startDae();
        break;

    case "stop":

        $ftp->stopDae();
        break;

    default:

        $ftp->usage();
        break;

}

