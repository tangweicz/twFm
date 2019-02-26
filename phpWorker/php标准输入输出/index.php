<?php
/**
 * Created by PhpStorm.
 * User: tangwei
 * Date: 17/2/8
 * Time: 14:39
 */

/*
 * 标准输入
 *
 * STDIN 其实就是 fopen("php://stdin", "w") 那么既然是fopen的句柄，那么久可以用fread()、fclose()等去处理
 */

$fh = fopen('php://stdin', 'w');
echo "[php://stdin]请输入任意字符：";
$str = fread($fh, 1000);
echo "[php://stdin]你输入的是：".$str;
fclose($fh);


echo "[STDIN]请输入任意字符：";
$str = fread(STDIN, 1000);
echo "[STDIN]你输入的是：".$str;



/*
 * 标准输出
 *
 * STUOUT 其实就是fopen("php://stdout", "w")
 */

$fh = fopen('php://stdout', 'w');
fwrite($fh, "标准输出php://stdout/n");
fclose($fh);
fwrite(STDOUT, "标准输出STDOUT/n");


/*
 * 标准错误，默认情况下会发送至用户终端
 *
 * STDERR 其实就是fopen("php://stderr", 'w')
 */
$fh = fopen('php://stderr', 'w');
fwrite($fh, "标准错误php://stderr/n");
fclose($fh);
fwrite(STDERR, "标准错误STDERR/n");




/*
 * @author tangwei
 *
 * @time 2017-02-08 15:13:44
 *
 * @function 重定向标准输入输出到文件
 */
var_dump(STDOUT);

var_dump(STDERR);

$path = "/dev/null";

@fclose(STDOUT);//关闭掉标准输出

@fclose(STDERR);//关闭掉标准错误输出

$stdout = fopen($path, "a");

$stderr = fopen($path, "a");

var_dump($stdout);//这儿已经不会显示到屏幕，而是写入到/dev/null中去

var_dump($stderr);//这儿已经不会显示到屏幕，而是写入到/dev/null中去





