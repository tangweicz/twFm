<?php

namespace Libs;

use Monolog\Logger;

use Monolog\Handler\StreamHandler;

/**
 * Created by PhpStorm.
 * User: tangwei
 * Date: 15/10/4
 * Time: 12:28
 */

class Controller
{
    static $PAGESIZE=10;

    public $http;

    public $log;

    public function __construct()
    {
        $this->log = new Logger('name');

        $this->log->pushHandler(new StreamHandler(__DIR__.'/../Log/System.log', Logger::WARNING));

    }

    public function jump($message, $url){

        echo "<script>alert('".$message."');location.href='".$url."'</script>";

        exit;

    }

    public function errorBack($message)
    {
        echo "<script>alert('".$message."');history.go(-1);</script>";

        exit;

    }

    public function isMobile()

    {

        // 如果有HTTP_X_WAP_PROFILE则一定是移动设备

        if (isset ($_SERVER['HTTP_X_WAP_PROFILE']))

        {

            return true;

        }

        // 如果via信息含有wap则一定是移动设备,部分服务商会屏蔽该信息

        if (isset ($_SERVER['HTTP_VIA']))

        {

            // 找不到为flase,否则为true

            return stristr($_SERVER['HTTP_VIA'], "wap") ? true : false;

        }

        // 判断手机发送的客户端标志,兼容性有待提高

        if (isset ($_SERVER['HTTP_USER_AGENT']))

        {

            $clientkeywords = array ('nokia',

                'sony',

                'ericsson',

                'mot',

                'samsung',

                'htc',

                'sgh',

                'lg',

                'sharp',

                'sie-',

                'philips',

                'panasonic',

                'alcatel',

                'lenovo',

                'iphone',

                'ipod',

                'blackberry',

                'meizu',

                'android',

                'netfront',

                'symbian',

                'ucweb',

                'windowsce',

                'palm',

                'operamini',

                'operamobi',

                'openwave',

                'nexusone',

                'cldc',

                'midp',

                'wap',

                'mobile'

            );

            // 从HTTP_USER_AGENT中查找手机浏览器的关键字

            if (preg_match("/(" . implode('|', $clientkeywords) . ")/i", strtolower($_SERVER['HTTP_USER_AGENT'])))

            {

                return true;

            }

        }

        // 协议法，因为有可能不准确，放到最后判断

        if (isset ($_SERVER['HTTP_ACCEPT']))

        {

            // 如果只支持wml并且不支持html那一定是移动设备

            // 如果支持wml和html但是wml在html之前则是移动设备

            if ((strpos($_SERVER['HTTP_ACCEPT'], 'vnd.wap.wml') !== false) && (strpos($_SERVER['HTTP_ACCEPT'], 'text/html') === false || (strpos($_SERVER['HTTP_ACCEPT'], 'vnd.wap.wml') < strpos($_SERVER['HTTP_ACCEPT'], 'text/html'))))

            {

                return true;

            }

        }

        return false;

    }



    /**
     * 判断手机号的类型
     * 更新时间 2016-11-07
     * @param $mobile
     * @return int  1:电信 2:移动 3:联通 4:虚拟运营商 10:未知
     */
    public function getMobileType($mobile){

        $prefix = substr($mobile,0,3);

        if (in_array($prefix, array('133','153','154','181','180','189','177'))) {

            return 1;

        } elseif (in_array($prefix, array('134','135','136','137','138','139','147','150','151','152','157','158','159','178','182','183','184','187','188'))) {

            return 2;

        } elseif (in_array($prefix, array('130','131','132','155','156','185','186','145','176', '166'))) {

            return 3;

        } elseif (in_array($prefix, array('170','177'))) {

            return 4;

        } else {

            return 10;

        }
    }


    /*
     * @author tangwei
     *
     * @time 2018-10-17 17:07:24
     *
     * @function 判断用户手机号归属地
     */
    function getMobileArea($mobile){

        $url = "https://tcc.taobao.com/cc/json/mobile_tel_segment.htm?tel=".$mobile;

        $ch = curl_init();

        $header = array(
            "content-type: application/x-www-form-urlencoded; charset=UTF-8"
        );

        //设置选项，包括URL

        curl_setopt($ch, CURLOPT_URL, $url);

        curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);

        curl_setopt($ch, CURLOPT_HEADER, 0);

        //执行并获取HTML文档内容

        $output = curl_exec($ch);

        //释放curl句柄

        curl_close($ch);

        $output = mb_convert_encoding($output, 'UTF-8', 'UTF-8,GBK,GB2312,BIG5');

        preg_match('/\{.*\}/ismU', $output, $match);

        return $match[0];

    }

    /*
     * @author tangwei
     *
     * @time 2018-10-28 10:03:57
     *
     * @function 过滤数据
     */
    public function filter($info)
    {

        return strip_tags(trim($info));

    }

}
