<?php
/**
 * Created by PhpStorm.
 * User: tangwei
 * Date: 16/9/18
 * Time: 09:19
 */
//$socket = socket_create(AF_INET, SOCK_STREAM, SOL_TCP);//创建一个socket连接
//
//if (!socket_connect($socket, "120.26.229.170", 9000))
//{
//    die('Socket error : '.socket_strerror(socket_last_error()));
//}
//
//$in = "first|get|package.xml";
//
//if(!socket_write($socket, $in, strlen($in))) {
//
//    echo "socket_write() failed: reason: " . socket_strerror($socket) . "\n";
//
//}else {
//
//    sleep(3);
//    socket_write($socket, "secend|get|ready");
//
//    echo "发送到服务器信息成功！\n";
//
//    echo "发送的内容为:<font color='red'>$in</font> <br>";
//
//}

//以上为客户端简单示例，下面开始真实应用


class ftpSocket
{

    /*
     * @author tangwei
     *
     * @time 2016-09-18 13:49:04
     *
     * @function 告知用户真正的使用方式
     */
    public function usage()
    {
        echo "the correct usage is `php xxx.php get/send xxx.txt` \r\n";

        exit;
    }

    /*
     * @author tagnwei
     *
     * @time 2016-09-18 14:16:13
     *
     * @function 从服务器端拉一个文件到本地
     */
    public function getFile($filename)
    {
        $fp = fopen($filename, "a+");

        $socket = socket_create(AF_INET, SOCK_STREAM, SOL_TCP);//创建一个socket连接

        socket_connect($socket, "120.26.229.170", 9000);

        socket_write($socket, "first|get|package.xml" );

        sleep(2);

        $firstRecv = socket_read($socket, 1024);

        sleep(2);

        $firstRecvData = explode("|", $firstRecv);

        if($firstRecvData[1] != "filesize"){

            echo "客户端接受文件大小出错！";

            exit;

        }else{

            $fileSize = $firstRecvData[2];

            $size = 0;

            socket_write($socket, "secend|get|ready");

            sleep(1);

            while($fileSize+2048 >= $size){

                echo $size."\r\n";

                fwrite($fp, socket_read($socket, 1024));

                $size +=1024;

                sleep(1);

            }

            fclose($fp);

            socket_close($socket);

            exit;

        }
    }

    /*
     * @author tangwei
     *
     * @time 2016-09-18 14:17:07
     *
     * @function 发送文件
     */
    public function sendFile()
    {
        return;

    }
}

$ftp = new ftpSocket();

if (count($argv) < 2) {

    $ftp->usage();

    exit(0);

}

switch($argv[1]){

    case "get":

        $ftp->getFile($argv[2]);
        break;

    case "send":

        $ftp->sendFile();
        break;

    default:

        $ftp->usage();
        break;

}