<?php

namespace Libs;

class Weichat
{
    const APPID = "wx1584e30131dc4900";//公众号或者小程序的APPID

    const APPSECRET = "9897c674c99006f90d319d30e1540ac6";//公众号或小程序的APPSECRET

    const CURL_TIMEOUT = 30;//CURL请求超时时间

    const MCHID = "1494699552";//商户ID号

    const KEY = "6Kg7c1Wm607DtIdS25ndTnPcKXF4l6zT";//商户KEY值

    const NOTIFY_URL = "http://xb.netfishing.cn/notifyWei";//支付成功回调的URL

    /*
     * @author tangwei
     *
     * @time 2016-01-23 12:41:05
     *
     * @function 网页授权登陆之前，需要获取用户的openid等信息
     */
    public static function getOauthAccessToken()
    {
        if(!isset($_GET["code"])) self::doLog("网页登陆获取用户access_token失败");

        $code = $_GET["code"];

        /*
         * 第三方类库处理http请求，暂时不用
         *
                $httpRequestFactory = new HttpRequestFactory();

                $http = new HttpRequest( $httpRequestFactory->newCurlRequest() );

                $httpRes = $http->makeHttpRequestTo("https://api.weixin.qq.com/sns/oauth2/access_token?appid=".self::APPID."&secret=".self::APPSECRET."&code=".$code."&grant_type=authorization_code");

                $httpRes = json_decode($httpRes);

                return $httpRes;
         */


        $url = "https://api.weixin.qq.com/sns/oauth2/access_token?appid=".self::APPID."&secret=".self::APPSECRET."&code=".$code."&grant_type=authorization_code";

        return self::doHttpRequest($url);

    }

    /*
     * @author tangwei
     *
     * @time 2016-01-23 12:41:30
     *
     * @function 网页授权登陆获取用户信息
     */
    public static function getOauthUserInfo($userAccessToken = "", $userOpenId = "")
    {
        if($userAccessToken== "" || $userOpenId == "") self::doLog("网页授权登陆获取用户信息，传参错误！");

        $url = "https://api.weixin.qq.com/sns/userinfo?access_token=$userAccessToken&openid=$userOpenId&lang=zh_CN";

        return self::doHttpRequest($url);

    }

    /*
     * @author tangwei
     *
     * @time 2016-01-23 14:27:29
     *
     * @function 接入微信，接入成功之后，这儿就不会再使用了
     */
    public static function joinWeiCat()
    {

        define("TOKEN","xinbeizhihuichengjian");//微信配置页面需要填写的

        $signature = $_GET["signature"];

        self::doLog("signature：".$signature);

        self::doLog("-----------------");

        $timestamp = $_GET["timestamp"];

        self::doLog("timestamp：".$timestamp);

        self::doLog("-----------------");

        $nonce = $_GET["nonce"];

        self::doLog("nonce：".$nonce);

        self::doLog("-----------------");

        $token = TOKEN;

        $tmpArr = array($token, $timestamp, $nonce);

        // use SORT_STRING rule
        sort($tmpArr, SORT_STRING);

        $tmpStr = implode( $tmpArr );

        $tmpStr = sha1( $tmpStr );

        self::doLog("tmpStr：".$tmpStr);

        self::doLog("-----------------");

        if( $tmpStr == $signature ){

            echo $_GET["echostr"];

        }else{

            echo  false;

        }
    }

    /*
     * @author tangwei
     *
     * @time 2016-01-23 22:29:39
     *
     * @function 微信接入成功之后，获取用户输入消息
     */
    public static function parseUserPutMessage()
    {
        $postStr = $GLOBALS["HTTP_RAW_POST_DATA"];//获取原生post数据

        if (!empty($postStr)){//如果原生post有数据

            libxml_disable_entity_loader(true);

            return $postObj = simplexml_load_string($postStr, 'SimpleXMLElement', LIBXML_NOCDATA);//解析xml数据

        }else {
            return "";
        }
    }

    /*
     * @author tangwei
     *
     * @time 2016-01-23 22:44:36
     *
     * @function 返回文字类型的xml对象给用户
     */
    public static function reportTextXml($fromUsername, $toUsername, $time, $content)
    {
        $textTpl = "<xml>
							<ToUserName><![CDATA[%s]]></ToUserName>
							<FromUserName><![CDATA[%s]]></FromUserName>
							<CreateTime>%s</CreateTime>
							<MsgType><![CDATA[%s]]></MsgType>
							<Content><![CDATA[%s]]></Content>
							<FuncFlag>0</FuncFlag>
							</xml>";//返回数据的XML格式

        /*
         * 这儿匹配用户输入内容，对应不同的响应
         */

        $msgType = "text";

        $contentStr = $content;

        $resultStr = sprintf($textTpl, $fromUsername, $toUsername, $time, $msgType, $contentStr);

        return $resultStr;
    }

    /*
     * @author tangwei
     *
     * @time 2016-01-24 11:35:03
     *
     * @function 获取普通access_token，在调用js-sdk等地需要使用
     */
    public static function getAccessToken()
    {
        // access_token 应该存储与更新到数据库中，因为每日的调用是有限制的，所以建议每次获取前，判断是否还在有效期以内

        // 如果是企业号用以下URL获取access_token
        // $url = "https://qyapi.weixin.qq.com/cgi-bin/gettoken?corpid=$this->appId&corpsecret=$this->appSecret";

        $url = "https://api.weixin.qq.com/cgi-bin/token?grant_type=client_credential&appid=".self::APPID."&secret=".self::APPSECRET;

        return self::doHttpRequest($url);

    }

    /*
     * @author tangwei
     *
     * @time 2016-01-24 14:25:46
     *
     * @function 生成带code的url，在支付的时候，因为我们自己的url不带code所以不能通过认证，这儿生成一下，将我们的URL带上code
     */
    public static function createUrlWithCode($url)
    {
        $urlObj["appid"] = self::APPID;

        $urlObj["redirect_uri"] = "$url";

        $urlObj["response_type"] = "code";

        $urlObj["scope"] = "snsapi_base";

        $urlObj["state"] = "STATE"."#wechat_redirect";

        $reqPar = self::formatBizQueryParaMap($urlObj, false);

        return "https://open.weixin.qq.com/connect/oauth2/authorize?".$reqPar;

    }

    /*
     * @author tangwei
     *
     * @time 2016-01-24 11:43:45
     *
     * @function 根据获取到的普通access_token，再去获取jsapi-ticket，在调用js-sdk等地需要使用
     */
    public static function getJsApiTicket($accessToken)
    {
        // jsapi-ticket 应该存储与更新到数据库中，因为每日的调用是有限制的，所以建议每次获取前，判断是否还在有效期以内

        $accessToken = $accessToken->access_token;
        // 如果是企业号用以下 URL 获取 ticket
        // $url = "https://qyapi.weixin.qq.com/cgi-bin/get_jsapi_ticket?access_token=$accessToken";
        $url = "https://api.weixin.qq.com/cgi-bin/ticket/getticket?type=jsapi&access_token=$accessToken";

        return self::doHttpRequest($url);
    }

    /*
     * @author tangwei
     *
     * @time 2016-01-24 12:05:39
     *
     * @function 获取签名包，config接口注入权限验证配置时使用
     */
    public static function getSignPackage($ticket) {
        $jsapiTicket = $ticket->ticket;

        // 注意 URL 一定要动态获取，不能 hardcode.
        $protocol = (!empty($_SERVER['HTTPS']) && $_SERVER['HTTPS'] !== 'off' || $_SERVER['SERVER_PORT'] == 443) ? "https://" : "http://";

        $url = "$protocol$_SERVER[HTTP_HOST]$_SERVER[REQUEST_URI]";

        $timestamp = time();

        $length = 16;

        $chars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

        $nonceStr = "";

        for ($i = 0; $i < $length; $i++) {

            $nonceStr .= substr($chars, mt_rand(0, strlen($chars) - 1), 1);

        }


        // 这里参数的顺序要按照 key 值 ASCII 码升序排序
        $string = "jsapi_ticket=$jsapiTicket&noncestr=$nonceStr&timestamp=$timestamp&url=$url";

        $signature = sha1($string);

        $signPackage = array(
            "appId"     => self::APPID,
            "nonceStr"  => $nonceStr,
            "timestamp" => $timestamp,
            "url"       => $url,
            "signature" => $signature,
            "rawString" => $string
        );
        return $signPackage;
    }

    /*
     * @author tangwei
     *
     * @time 2016-01-24 14:54:07
     *
     * @function 统一支付接口，获取prepay_id
     */
    public static function getPrepayId($openId, $goods_name, $total_money)
    {


        $url = "https://api.mch.weixin.qq.com/pay/unifiedorder";
        //设置curl超时时间
        $curl_timeout = self::CURL_TIMEOUT;

        $param["openid"] = $openId;

        $param["body"] = $goods_name;

        echo $param["out_trade_no"] = self::APPID.time();

        $param["total_fee"] = $total_money;

        $param["notify_url"] = self::NOTIFY_URL;

        $param["trade_type"] ="JSAPI";

        $param["appid"] = self::APPID;//公众账号ID

        $param["mch_id"] = self::MCHID;//商户号

        $param["spbill_create_ip"] = $_SERVER['REMOTE_ADDR'];//终端ip

        $param["nonce_str"] = self::createNoncestr();//随机字符串

        $param["sign"] = self::getSign($param);//签名

        $result = self::arrayToXml($param);

        $xml = self::postXmlCurl($result, $url, $curl_timeout);

        $array_data = json_decode(json_encode(simplexml_load_string($xml, 'SimpleXMLElement', LIBXML_NOCDATA)), true);

        return $array_data["prepay_id"];

    }

    /*
     * @author tangwei
     *
     * @time 2016-01-24 20:07:42
     *
     * @function 获取支付所需参数（jsapi中使用）
     */
    public static function getParameters($prepay_id)
    {
        $arr["appId"] = self::APPID;

        $timeStamp = time();

        $arr["timeStamp"] = "$timeStamp";

        $arr["nonceStr"] = self::createNoncestr();

        $arr["package"] = "prepay_id=$prepay_id";

        $arr["signType"] = "MD5";

        $arr["paySign"] = self::getSign($arr);

        return json_encode($arr);
    }

    /*
     * @author tangwei
     *
     * @time 2016-01-25 10:52:04
     *
     * @function 申请退款，需要使用
     */
    public static function applyRefund($out_trade_no, $refund_fee, $total_money)
    {

        $url = "https://api.mch.weixin.qq.com/secapi/pay/refund";

        $curl_timeout = self::CURL_TIMEOUT;

        $param["out_trade_no"] = $out_trade_no;

        $param["out_refund_no"] = $out_trade_no."refund";

        $param["total_fee"] = $total_money;

        $param["refund_fee"] = $refund_fee;

        $param["op_user_id"] = self::MCHID;

        $param["appid"] = self::APPID;//公众账号ID

        $param["mch_id"] = self::MCHID;//商户号

        $param["nonce_str"] = self::createNoncestr();//随机字符串

        $param["sign"] = self::getSign($param);//签名

        $xml = self::arrayToXml($param);//将数组参数组成xml

        $xml_result = self::postXmlSSLCurl($xml, $url, $curl_timeout);//将xml数据交给curl处理

        $array_data = json_decode(json_encode(simplexml_load_string($xml_result, 'SimpleXMLElement', LIBXML_NOCDATA)), true);//将返回的xml数据转化为数组

    }

    /*
     *
     * @author tangwei
     *
     * @time 2018-03-01 11:03:03
     *
     * @function 使用证书，以post方式提交xml到对应的接口url
     *
     */
    public function postXmlSSLCurl($xml,$url)
    {

        $ch = curl_init();
        //超时时间
        curl_setopt($ch,CURLOPT_TIMEOUT,self::CURL_TIMEOUT);
        //这里设置代理，如果有的话
        //curl_setopt($ch,CURLOPT_PROXY, '8.8.8.8');
        //curl_setopt($ch,CURLOPT_PROXYPORT, 8080);
        curl_setopt($ch,CURLOPT_URL, $url);
        curl_setopt($ch,CURLOPT_SSL_VERIFYPEER,FALSE);
        curl_setopt($ch,CURLOPT_SSL_VERIFYHOST,FALSE);
        //设置header
        curl_setopt($ch,CURLOPT_HEADER,FALSE);
        //要求结果为字符串且输出到屏幕上
        curl_setopt($ch,CURLOPT_RETURNTRANSFER,TRUE);
        //设置证书
        //使用证书：cert 与 key 分别属于两个.pem文件
        //默认格式为PEM，可以注释
        curl_setopt($ch,CURLOPT_SSLCERTTYPE,'PEM');
        curl_setopt($ch,CURLOPT_SSLCERT,getcwd().'/apiclient_cert.pem');
        //默认格式为PEM，可以注释
        curl_setopt($ch,CURLOPT_SSLKEYTYPE,'PEM');
        curl_setopt($ch,CURLOPT_SSLKEY, getcwd().'/apiclient_key.pem');
        //post提交方式
        curl_setopt($ch,CURLOPT_POST, true);
        curl_setopt($ch,CURLOPT_POSTFIELDS,$xml);
        $data = curl_exec($ch);

        //var_dump($data);
        //返回结果
        if($data){
            curl_close($ch);
            return $data;
        }
        else {
            $error = curl_errno($ch);
            echo "curl出错，错误码:$error"."<br>";
            echo "<a href='http://curl.haxx.se/libcurl/c/libcurl-errors.html'>错误原因查询</a></br>";
            curl_close($ch);
            return false;
        }
    }

    /*
     * @author tangwei
     *
     * @time 2016-01-26 10:41:49
     *
     * @function 制作临时的带参数的二维码
     */
    public static function createTmpCode($data, $token)
    {
        $url = "https://api.weixin.qq.com/cgi-bin/qrcode/create?access_token=$token";

        $ticket = self::postCurl($data, $url, self::CURL_TIMEOUT);//发送curl请求

        $ticket = json_decode($ticket);

        $ticket = $ticket->ticket;//拿到ticket依据

        return self::accordTickerReturn($ticket);
    }

    /*
     * @author tangwei
     *
     * @time 2016-01-26 10:53:40
     *
     * @function 根据需要制作二维码的ticket票据，获取二维码，并返回URL
     */
    private static function accordTickerReturn($ticket)
    {
        $ticket = urlencode($ticket);

        return "https://mp.weixin.qq.com/cgi-bin/showqrcode?ticket=$ticket";
    }

    /*
     * @author tangwei
     *
     * @time 2016-01-26 13:37:19
     *
     * @function 创建自定义菜单
     */
    public static function createMenu($data, $token)
    {
        $url = "https://api.weixin.qq.com/cgi-bin/menu/create?access_token=$token";

        return self::postCurl($data, $url);
    }
    /*
     * @author tangwei
     *
     * @time 2016-01-26 10:31:42
     *
     * @function curl的post发送数据
     */
    public static function postCurl($data, $url, $second = 30)
    {
        //初始化curl
        $ch = curl_init();
        //设置超时
        curl_setopt($ch, CURLOPT_TIMEOUT, $second);
        //这里设置代理，如果有的话
        //curl_setopt($ch,CURLOPT_PROXY, '8.8.8.8');
        //curl_setopt($ch,CURLOPT_PROXYPORT, 8080);
        curl_setopt($ch,CURLOPT_URL, $url);
        curl_setopt($ch,CURLOPT_SSL_VERIFYPEER,FALSE);
        curl_setopt($ch,CURLOPT_SSL_VERIFYHOST,FALSE);
        //设置header
        curl_setopt($ch, CURLOPT_HEADER, FALSE);
        //要求结果为字符串且输出到屏幕上
        curl_setopt($ch, CURLOPT_RETURNTRANSFER, TRUE);
        //post提交方式
        curl_setopt($ch, CURLOPT_POST, TRUE);
        curl_setopt($ch, CURLOPT_POSTFIELDS, $data);
        //运行curl
        $data = curl_exec($ch);
        curl_close($ch);
        //返回结果
        if($data)
        {
            //curl_close($ch);
            return $data;
        }else{

            return false;
        }
    }

    /*
     * @author tangwei
     *
     * @time 2018-03-08 09:54:49
     *
     * @function 将数组组换成XML
     */
    public static function arrayToXml($arr)
    {
        $xml = "<xml>";
        foreach ($arr as $key=>$val)
        {
            if (is_numeric($val))
            {
                $xml.="<".$key.">".$val."</".$key.">";

            }
            else
                $xml.="<".$key."><![CDATA[".$val."]]></".$key.">";
        }
        $xml.="</xml>";
        return $xml;
    }

    /*
     * @author tangwei
     *
     * @time 2018-03-08 09:54:25
     *
     * @function 获取32随机字符串
     */
    public static function createNoncestr( $length = 32 )
    {
        $chars = "abcdefghijklmnopqrstuvwxyz0123456789";
        $str ="";
        for ( $i = 0; $i < $length; $i++ )  {
            $str.= substr($chars, mt_rand(0, strlen($chars)-1), 1);
        }
        return $str;
    }

    /*
     * @author tangwei
     *
     * @time 2018-03-08 09:53:57
     *
     * @function 生成签名  到处都用
     */
    public static function getSign($Obj)
    {
        foreach ($Obj as $k => $v)
        {
            $Parameters[$k] = $v;
        }
        //签名步骤一：按字典序排序参数
        ksort($Parameters);
        $String = self::formatBizQueryParaMap($Parameters, false);
        //echo '【string1】'.$String.'</br>';
        //签名步骤二：在string后加入KEY
        $String = $String."&key=".self::KEY;
        //echo "【string2】".$String."</br>";
        //签名步骤三：MD5加密
        $String = md5($String);
        //echo "【string3】 ".$String."</br>";
        //签名步骤四：所有字符转为大写
        $result_ = strtoupper($String);
        //echo "【result】 ".$result_."</br>";
        return $result_;
    }

    /*
     * @author tangwei
     *
     * @time 2016-01-24 15:27:41
     *
     * @function 以post方式提交xml到对应的接口url
     */
    public static function postXmlCurl($xml,$url,$second=30)
    {
        //初始化curl
        $ch = curl_init();
        //设置超时
        curl_setopt($ch, CURLOPT_TIMEOUT, $second);
        //这里设置代理，如果有的话
        //curl_setopt($ch,CURLOPT_PROXY, '8.8.8.8');
        //curl_setopt($ch,CURLOPT_PROXYPORT, 8080);
        curl_setopt($ch,CURLOPT_URL, $url);
        curl_setopt($ch,CURLOPT_SSL_VERIFYPEER,FALSE);
        curl_setopt($ch,CURLOPT_SSL_VERIFYHOST,FALSE);
        //设置header
        curl_setopt($ch, CURLOPT_HEADER, FALSE);
        //要求结果为字符串且输出到屏幕上
        curl_setopt($ch, CURLOPT_RETURNTRANSFER, TRUE);
        //post提交方式
        curl_setopt($ch, CURLOPT_POST, TRUE);
        curl_setopt($ch, CURLOPT_POSTFIELDS, $xml);
        //运行curl
        $data = curl_exec($ch);
        curl_close($ch);
        //返回结果
        if($data)
        {
            //curl_close($ch);
            return $data;
        }
        else
        {
            $error = curl_errno($ch);
            echo "curl出错，错误码:$error"."<br>";
            echo "<a href='http://curl.haxx.se/libcurl/c/libcurl-errors.html'>错误原因查询</a></br>";
            curl_close($ch);
            return false;
        }
    }

    /*
     * @author tanagwei
     *
     * @time 2016-01-24 15:20:06
     *
     * @function 格式化请求字串
     */
    private static function formatBizQueryParaMap($paraMap, $urlencode)
    {
        $buff = "";
        ksort($paraMap);
        foreach ($paraMap as $k => $v)
        {
            if($urlencode)
            {
                $v = urlencode($v);
            }
            //$buff .= strtolower($k) . "=" . $v . "&";
            $buff .= $k . "=" . $v . "&";
        }
        $reqPar = "";
        if (strlen($buff) > 0)
        {
            $reqPar = substr($buff, 0, strlen($buff)-1);
        }
        return $reqPar;
    }
    /*
     * @author tangwei
     *
     * @time 2016-01-23 12:53:22
     *
     * @function 处理url请求的curl操作
     */
    private static function doHttpRequest($url = "")
    {
        if($url == "") self::doLog("处理url请求的curl操作");


        $request = curl_init();

        curl_setopt($request, CURLOPT_URL, $url);

        curl_setopt($request, CURLOPT_TIMEOUT, 30);

        curl_setopt($request, CURLOPT_RETURNTRANSFER, true);

        $result = curl_exec($request);

        return $result = json_decode($result);
    }

    /*
     * @author tangwei
     *
     * @time 2016-01-23 13:04:42
     *
     * @function 操作日志的方法
     */

    private static function doLog($message)
    {
        $log = new Logger('name');//初始化log类，需要往文件里面写入错误信息

        $log->pushHandler(new StreamHandler(__DIR__.'/../Log/System.log', Logger::WARNING));

        $log->addWarning($message);//写入log信息
    }




    /*
     * @author tangwei
     *
     * @time 2018年03月01日10:23:28
     *
     * @function 微信商户向微信个人打款
     *
     * @param $partner_trade_no 订单编码
     *
     *        $openid 收款方openid
     *
     *        $amount 收款方金额，精确到分
     *
     *        $desc   打款描述
     */
    public function payToUser($partner_trade_no = "", $openid = "", $amount = "", $desc = "")
    {

        $url = "https://api.mch.weixin.qq.com/mmpaymkttransfers/promotion/transfers";

        $param["mch_appid"] = self::APPID;//公众账号ID

        $param["mchid"] = self::MCHID;//商户号

        $param["nonce_str"] = self::createNoncestr();//随机字符串

        $param["partner_trade_no"] = $partner_trade_no;//self::APPID.time();

        $param["openid"] = $openid;//"ol4D54nXmST6JfujjN-fBaeK8lbs";

        $param["check_name"] = "NO_CHECK";

        $param["amount"] = $amount;//100;

        $param["desc"] = $desc;//"答题红包";

        $param["spbill_create_ip"] = $_SERVER['REMOTE_ADDR'];

        $param["sign"] = self::getSign($param);//签名

        $xml = self::arrayToXml($param);//将数组参数组成xml

        return $result = $this->postXmlSSLCurl($xml, $url);

    }

    /*
     * @author tangwei
     *
     * @time 2018-01-17 14:34:05
     *
     * @function 小程序中获取用户信息
     */
    public static function getUserInfo()
    {
        if(!isset($_GET["code"])) self::doLog("小程序中获取用户信息失败");

        $code = $_GET["code"];

        $url = "https://api.weixin.qq.com/sns/jscode2session?appid=".self::XCXAPPID."&secret=".self::XCXAPPSECRET."&js_code=".$code."&grant_type=authorization_code";

        return self::doHttpRequest($url);

    }

}