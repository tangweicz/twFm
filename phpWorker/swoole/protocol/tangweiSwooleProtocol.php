<?php
/**
 * Created by PhpStorm.
 * User: tangwei
 * Date: 16/9/28
 * Time: 14:33
 *
 * 1、EOF协议：用一组固定的、不会在正常数据内出现的字符串作为分隔协议的标记，叫做EOF协议
 *
 * 2、固定包头协议：在数据包的头部放一个固定格式的数据做为协议头，称之为固定包头协议。协议头的格式必须固定，并且其中需要标明后续数据的长度。长度字段的格式只支持“S、L、N、V”和"s、l、n、v"
 *
 * 3、如何在一个swoole程序中监听其他端口，swoole_server::listen()，返回一个swoole_server_port对象，可以调用set、on方法，新创建的端口需要设置协议参数即需要set下，否则将会复用swoole_server的协议解析方式，创建的端口无法使用onRequest、onMessage回调
 *
 *
 * 不管是在代码中直接new还是通过listen进来的，都可以通过$this->ser->connection_info($fd)来拿到["server_port"]等信息
 */

//--------------------------EOF协议初试开始

class server
{
    private $ser;

    public function __construct()
    {
        $this->ser = new swoole_server('0.0.0.0', 9900);

        $this->ser->set(

            array(

                'worker_num'=>2,

                'package_max_length'=>8192,//swoole中有个数据缓存区，存放swoole所有读取的数据，这儿设定的大小是设定每个连接的缓存区的大小

                'open_eof_split'=>true,//开启eof检测

                'package_eof'=>"\r\n",//设定eof的标记

            )


        );

        $this->ser->on('');

        $this->ser->start();
    }
}

//--------------------------EOF协议初试完毕




//-------------------------固定包头协议初试开始
class serverC
{
    private $ser;

    public function __construct()
    {
        $this->ser = new swoole_server('0.0.0.0', 9900);

        $this->ser->set(

            array(

                'worker_num'=>2,

                'package_max_length'=>8192,//swoole中有个数据缓存区，存放swoole所有读取的数据，这儿设定的大小是设定每个连接的缓存区的大小

                'open_length_check'=>true,//打开固定包头协议

                'package_length_offset'=>0,//长度信息在头文件包中的偏移位置

                'package_body_offset'=>4,//头信息的内容长度

                'package_length_type'=>'N'//用什么方法去解包数据，对应客户端上的pack的打包方式

            )


        );

        $this->ser->on('receive',array($this, 'onReceive'));

        $this->ser->start();
    }


    public function onReceive(swoole_server $server, int $fd, int $from_id, string $data)
    {
        $upackedData = upack('N', $data);//用什么方法去解包数据，对应客户端上的pack的打包方式

        $length = $upackedData[1];

        $message = substr($data, -$length);

    }
}


$message = "this is a message";//client端的话，需要把send出来的数据pack一下

$message = pack("N", str_len($message)).$message;

//-------------------------固定包头协议初试完毕