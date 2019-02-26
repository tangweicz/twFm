<?php
/**
 * Created by PhpStorm.
 * User: tangwei
 * Date: 16/9/22
 * Time: 11:27
 */

$sock = stream_socket_client("tcp://127.0.0.1:9900", $errio, $errstr,30);

function onRead()
{
    global $sock;

    $getData = stream_socket_recvfrom($sock,1024);

    if(!$getData){

        echo "this socket is shutdown\r\n";

        swoole_event_del($sock);
    }

    echo "Recv:$getData";

    fwrite(STDOUT, "请输入：");

}

function onWrite()
{
    global $sock;
}

function onInput()
{
    global $sock;

    $userInput = trim(fgets(STDOUT));

    if($userInput == "exit"){

        swoole_event_exit();

    }

    swoole_event_write($sock, $userInput);

    fwrite(STDOUT, "请输入：");
}

swoole_event_add($sock, 'onRead', 'onWrite');

swoole_event_add(STDOUT, 'onInput');

fwrite(STDOUT, "请输入：");