<?php
/**
 * Created by PhpStorm.
 * User: tangwei
 * Date: 16/9/20
 * Time: 13:58
 */

$socket = stream_socket_client('tcp://121.41.9.17:8000', $errno, $errstr, 30);

fwrite($socket, "tangwei");

var_dump(fread($socket, 1024));

