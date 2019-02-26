<?php

$socket = stream_socket_client('tcp://192.168.6.102:8000', $errno, $errstr, 30);

//fwrite($socket, "tangwei");

//var_dump(fread($socket, 1024));

