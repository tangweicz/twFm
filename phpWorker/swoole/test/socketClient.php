<?php
/**
 * Created by PhpStorm.
 * User: tangwei
 * Date: 16/10/21
 * Time: 11:07
 */

$client = new swoole_client(SWOOLE_TCP);

$client->connect("192.168.6.124", "9900");

$client->send("66666666");

var_dump($client->recv());