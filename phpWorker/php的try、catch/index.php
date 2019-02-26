<?php
/**
 * Created by PhpStorm.
 * User: tangwei
 * Date: 17/2/17
 * Time: 16:00
 *
 * 注意点：
 *      1、一个try对应一个catch
 *      2、只有在try中throw出来的信息，才能被catch捕获
 *      3、try中的代码出错，try中的代码和外面的代码都不会执行
 *      4、
 *
 *
 */

try{

    throw new Exception("this is error");

    echo 456;

}catch (Exception $e){

    var_dump($e->getMessage());

}

echo 123;



/*
 * 如果有多个catch，catch会匹配catch中的对象与throw的对象，匹配就会走相应的catch
 */
class myException extends Exception
{
    /*
     * @author tangwei
     *
     * @time 2017-02-17 16:26:02
     *
     * @function 构造方法
     */
    public function __construct($message, $code = 0)
    {

        parent::__construct($message, $code);

    }

    /*
     * @author tangwei
     *
     * @time 2017-02-17 16:30:42
     *
     * @function 自己定义的方法
     */
    public function selfFunction()
    {
        echo "this is selfFunction\r\n";
    }

}

class myClass
{
    public $var;

    public function __construct($num)
    {
        switch($num){

            case 1:
                throw new myException("123");

                break;

            case 2:
                throw new Exception("456");

                break;

            default:
                $this->var = $num;

                break;

        }

    }
}

try{

    $my = new myClass(1);

    var_dump($my);

    echo "this is try \r\n";

}catch(myException $e){

    echo $e->getMessage()."\r\n";

    echo $e->selfFunction();

    exit;

}catch(Exception $e){

    echo $e->getMessage();

    exit;
}

echo 123456;



