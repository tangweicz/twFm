<?php
/**
 * Created by PhpStorm.
 * User: tangwei
 * Date: 16/12/5
 * Time: 14:26
 */



class webSocket
{
    public $sockets = array();

    public $master;//连接到socket server的client

    public $handshake = false;//是否完成握手
    /*
     * @author tangwei
     *
     * @time 2016-12-05 16:04:06
     *
     * @function 创建socket套接字
     */
    public function __construct()
    {
        $this->master = socket_create(AF_INET, SOCK_STREAM, SOL_TCP)
        or die("socket_create() failed");

        socket_set_option($this->master, SOL_SOCKET, SO_REUSEADDR, 1)
        or die("socket_option() failed");

        socket_bind($this->master, "0.0.0.0", "8000")
        or die("socket_bind() failed");

        socket_listen($this->master, 2)
        or die("socket_listen() failed");

        $this->sockets[] = $this->master;

        echo "socket创建成功：$this->master";

        while(true) {

            $write = NULL;

            $except = NULL;

            socket_select($this->sockets, $write, $except, NULL);

            foreach ($this->sockets as $socket) {

                //连接主机的 client
                if ($socket == $this->master){//普通socket的连接
                    $client = socket_accept($this->master);
                    if ($client < 0) {
                        // debug
                        echo "socket_accept() failed";
                        continue;
                    } else {
                        //connect($client);
                        array_push($this->sockets, $client);
                        echo "connect client\n";
                    }
                }else {//websocket连接
                    $bytes = @socket_recv($socket,$buffer,2048,0);
                    if($bytes == 0) return;
                    if (!$this->handshake) {
                        // 如果没有握手，先握手回应
                        $this->doHandShake($socket, $buffer);
                        //echo "shakeHands\n";
                    } else {
                        // 如果已经握手，直接接受数据，并处理
                        //$buffer = decode($buffer);
                        //process($socket, $buffer);
                        echo "send file\n";
                    }
                }

            }


        }
    }


    public function getHeaders($req)
    {
        $r = $h = $o = null;
        if(preg_match("/GET (.*) HTTP/"   , $req, $match))
          $r = $match[1];
        if(preg_match("/Host: (.*)\r\n/"  , $req, $match))
          $h = $match[1];
        if(preg_match("/Origin: (.*)\r\n/", $req, $match))
          $o = $match[1];
        if(preg_match("/Sec-WebSocket-Key: (.*)\r\n/", $req, $match))
         $key = $match[1];

        $m = array($r, $h, $o, $key);

        var_dump($m);

        echo "*******************\r\n";

        return $m;


    }
//
//     protected function wrap($msg="", $opcode = 0x1)
//     {
//         //默认控制帧为0x1（文本数据）
//         $firstByte = 0x80 | $opcode;
//         $encodedata = null;
//         $len = strlen($msg);
//
//         if (0 <= $len && $len <= 125)
//             $encodedata = chr(0x81) . chr($len) . $msg;
//         else if (126 <= $len && $len <= 0xFFFF)
//         {
//             $low = $len & 0x00FF;
//             $high = ($len & 0xFF00) >> 8;
//             $encodedata = chr($firstByte) . chr(0x7E) . chr($high) . chr($low) . $msg;
//         }
//
//         return $encodedata;
//     }
//
      private function doHandShake($user, $buffer)
      {
         echo ("\nRequesting handshake...");
         //var_dump($buffer);
          echo "------------------>\r\n";
          var_dump($user);

          echo "------------------>\r\n";
         list($resource, $host, $origin, $key) = $this->getHeaders($buffer);

         //websocket version 13
         $acceptKey = base64_encode(sha1($key . '258EAFA5-E914-47DA-95CA-C5AB0DC85B11', true));

         echo ("Handshaking...");
         $upgrade  = "HTTP/1.1 101 Switching Protocol\r\n" .
                     "Upgrade: websocket\r\n" .
                     "Connection: Upgrade\r\n" .
                     "Sec-WebSocket-Accept: " . $acceptKey . "\r\n\r\n";  //必须以两个回车结尾
         echo ($upgrade);
         $sent = socket_write($user, $upgrade, strlen($upgrade));
         $this->handshake=true;
         echo ("Done handshaking...");
         return true;
     }
//
//    protected function unwrap($clientSocket, $msg="")
//    {
//          $opcode = ord(substr($msg, 0, 1)) & 0x0F;
//          $payloadlen = ord(substr($msg, 1, 1)) & 0x7F;
//          $ismask = (ord(substr($msg, 1, 1)) & 0x80) >> 7;
//          $maskkey = null;
//          $oridata = null;
//          $decodedata = null;
//
//         //关闭连接
//         if ($ismask != 1 || $opcode == 0x8)
//         {
//             $this->disconnect($clientSocket);
//             return null;
//         }
//
//         //获取掩码密钥和原始数据
//         if ($payloadlen <= 125 && $payloadlen >= 0)
//             {
//                 $maskkey = substr($msg, 2, 4);
//             $oridata = substr($msg, 6);
//         }
//         else if ($payloadlen == 126)
//             {
//                 $maskkey = substr($msg, 4, 4);
//             $oridata = substr($msg, 8);
//         }
//         else if ($payloadlen == 127)
//             {
//                 $maskkey = substr($msg, 10, 4);
//             $oridata = substr($msg, 14);
//         }
//         $len = strlen($oridata);
//         for($i = 0; $i < $len; $i++)
//             {
//                 $decodedata .= $oridata[$i] ^ $maskkey[$i % 4];
//         }
//         return $decodedata;
//     }
//
//    public function disconnect($clientSocket)
//    {
//          $found = null;
//          $n = count($this->users);
//          for($i = 0; $i<$n; $i++)
//          {
//              if($this->users[$i]->socket == $clientSocket)
//              {
//                  $found = $i;
//                 break;
//             }
//         }
//         $index = array_search($clientSocket,$this->sockets);
//
//         if(!is_null($found))
//             {
//                 array_splice($this->users, $found, 1);
//                 array_splice($this->sockets, $index, 1);
//
//                 socket_close($clientSocket);
//                $this->say($clientSocket." DISCONNECTED!");
//         }
//     }
}

new webSocket();
