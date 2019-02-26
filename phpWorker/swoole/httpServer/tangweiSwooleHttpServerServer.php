<?php
/**
 * Created by PhpStorm.
 * User: tangwei
 * Date: 16/9/26
 * Time: 09:09
 *
 * 继承自swoole的server
 *
 * swoole的HttpServer本质是swoole_server，其协议解析部分固定使用Http协议解析
 *
 * 完整的Httd协议请求会被解析并封装在swoole_http_request对象内
 *
 * 所有的Http相应都通过swoole_http_resopnse对象进行封装和发送。
 *
 * 1、swoole_http_server()函数，新增回调函数onRequest，在on的时候可以绑定。
 *      swoole_http_server不接受onReceive回调，因为有了onRequest回调了 ，但是可以使用Task Worker和定时器
 *
 *      swoole_http_serverr可以使用诸如onStart、onWorkerStart等回调
 *
 *      $response/$request对象传递给其他函数时，不要加&引用符号
 *
 * 2、swoole_http_request()函数，
 *      $header是Http请求的头部信息。类型为数组，所有key均为小写
 *
 *      $server是Http请求相关的服务器信息,$request->server["path_info"]
 *
 *      $get是请求的get参数，相当于php中的$_GET
 *
 *      $post是Http的post参数，相当于php中的$_POST，content-type限定为application/x-www-form-urlencoded
 *
 *      $cookie是Http的COOKIE参数，相当于php中的$_COOKIE
 *
 *      $files是Http上传文件的信息，相当于php中的$_FILES
 *
 *      rawContent()是原生的Http POST参数，用于非application/x-www-form-urlencoded格式的请求
 *
 * 3、swoole_http_response()函数
 *
 *      swoole_http_response::header($key, $value)设置http响应的头信息，需要在调用end函数之前调用
 *
 *      swoole_http_response::cookie($key, $value, $expire = 0, $path = '/', $domain = '', $secure = false, $httponly = fase)设置http响应的COOKIE信息，等效于php的setcookie函数，需要在调用end函数之前调用
 *
 *      swoole_http_response::status($code)设置http响应的状态码，如200，404，503等，需要在调用end函数之前调用
 *
 *      swoole_http_response::gzip($level = 1)开启gzip压缩，需要在调用end函数之前调用
 *
 *      swoole_http_response::write($data)启用Http Chunk分段向浏览器发送相应内容，不需要再次调用end函数
 *
 *      swoole_http_response::sendFile($filename)发送文件到浏览器，不需要再次调用end函数
 *
 *      swoole_http_response::end($data)发送Http相应
 */

//---------------------------------swoole_http_server简单用例开始

$server = new swoole_http_server('0.0.0.0', "9900");

$server->on('request', function($request, $response){

    var_dump($request->get);

    var_dump($request->post);

    var_dump($request->cookie);

    var_dump($request->files);

    var_dump($request->header);

    var_dump($request->server);

    $response->cookie("username", "tangwei");

    $response->header("X-Server", "tangweiSwoole");

    $response->end("welcome");


});

$server->start();

//---------------------------------swoole_http_server简单用例完毕



//---------------------------------swoole_http_server稍复杂用例开始

class server
{
    private $ser;

    public function __construct()
    {
        $this->ser = new swoole_http_server('0.0.0.0', 9900);

        $this->ser->set(

            array(

                "worker_num"=>1,

            )

        );

        $this->ser->on("start", array($this, "onStart"));

        $this->ser->on('managerStart', array($this, 'onManagerStart'));

        $this->ser->on("workerStart", array($this, "onWorkerStart"));

        $this->ser->on("request", array($this, "onRequest"));

        $this->ser->start();
    }

    public function onStart()
    {
        swoole_set_process_name("tangwei_master");
    }

    public function onManagerStart()
    {
        swoole_set_process_name("tangwei_manage");
    }

    public function onWorkerStart()
    {
        swoole_set_process_name("tangwei_worker");

    }

    public function onRequest($request, $response)
    {

        $requestArr = explode("/", $request->server["path_info"]);

        $class = $requestArr[1];

        $action = $requestArr[2];

        //下面实现逻辑部分

    }


}

new server();

//---------------------------------swoole_http_server稍复杂用例完毕

//---------------------------------swoole_http_server上传文件部分开始
class serverC
{
    private $ser;

    public function __construct()
    {
        $this->ser = new swoole_http_server('0.0.0.0', 9900);

        $this->ser->set(

            array(

                "max_package_length"=>200000000,

            )

        );

        $this->ser->on("request", array($this, "onRequest"));

        $this->ser->start();
    }

    public function onRequest($request, $response){

        if($request->server["request_method"] == "GET"){

            return;
        }
        //var_dump($request);

        $file = $request->files["files"];

        $filename = $file["name"];

        $file_tmp_name = $file["tmp_name"];

        $upload_dir = __DIR__."/upload";

        if(!file_exists($upload_dir)){

            mkdir($upload_dir);

        }

        move_uploaded_file($file_tmp_name, $upload_dir."/".$filename);

        $response->end("upload success");

    }


}

new serverC();
//---------------------------------swoole_http_server上传文件部分完毕

