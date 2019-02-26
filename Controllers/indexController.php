<?php

namespace Controllers;
use Libs\Controller;
use Libs\Weichat;
use Libs\CTemplate;

/**
 * Created by PhpStorm.
 * User: tangwei
 * Date: 16/1/23
 * Time: 11:20
 */
class indexController extends Controller
{
    /*
     * @author tangwei
     *
     * @time 2018-03-08 10:13:37
     *
     * @function 引到用户授权登录拿到用户信息
     */
    public function index()
    {
        $oauthAccessToken = Weichat::getOauthAccessToken();
        $openId = $oauthAccessToken->openid;
        $access_token = $oauthAccessToken->access_token;
        $userInfo = Weichat::getOauthUserInfo($access_token, $openId);
        var_dump($userInfo);
    }

    /*
     * @author tangwei
     *
     * @time 2018-03-08 10:13:09
     *
     * @function 接入微信服务，并且也是微信开发的入口
     */
    public function joinWeiChat()
    {
        $re = Weichat::joinWeiChat();//接入微信，接入成功即可屏蔽
        echo $re;exit;
        $result = Weichat::parseUserPutMessage();
        if($result != ""){
            $fromUsername = $result->FromUserName;
            $toUsername = $result->ToUserName;
            $time = time();
            $msgType = $result->MsgType;//获取用户输入的信息类型
            switch($msgType){
                case "text"://如果输入的是纯文本
                    $content = trim($result->Content);//这儿获取到用户输入的内容，好做判断
                    $reportContent = "Welcome to wechat world!!12!";//设定回复内容
                    echo Weichat::reportTextXml($fromUsername, $toUsername, $time, $reportContent);
                    break;
                case "event"://如果输入的是事件
                    $event = $result->Event;//拿到具体的时间类型
                    if($event == "LOCATION"){//为上报地理位置的事件
                        $longitude = $result->Longitude;
                        $latitude = $result->Latitude;
                        $content = "$longitude,$latitude";//设定回复内容
                        echo Weichat::reportTextXml($fromUsername, $toUsername, $time, $content);
                    }
                    if($event == "SCAN"){
                        $content = $result->EventKey;
                        echo Weichat::reportTextXml($fromUsername, $toUsername, $time, $content);
                    }
                    if($event == "subscribe"){//关注事件
                    }
                    if($event == "unsubscribe"){//取消关注事件
                    }
                    break;
                default:
                    break;

            }
        }
    }

    /*
     * @author tangwei
     *
     * @time 2016-01-24 11:06:13
     *
     * @function 增加微信的js-sdk的功能，以便以后的分享操作、调起微信拍照等一系列的微信js-sdk的功能
     */
    public function addJsSdk()
    {
        $result = Weichat::getAccessToken();//获取普通access_token，需要存数据库，并且查看是否过期
        $re = Weichat::getJsApiTicket($result);//获取api-ticket，需要存数据库，并且查看是否过期
        $r = Weichat::getSignPackage($re);//获取到签名包，这个需要注入页面中
        $ctemplate=new CTemplate("weiChat.php","dafault",__DIR__."/../Template");
        $ctemplate->render(array('result'=>$r));
    }

    /*
     * @author tangwei
     *
     * @time 2016-01-24 13:34:37
     *
     * @function 微信支付流程，首先微信要开通微信商家，进入微信支付--开发配置，配置支付目录以及测试白名单。
     */
    public function weiChatPay()
    {
        if(!isset($_GET["code"])){//我们自己的支付URL没有code，没办法获取用户的信息，所以这儿加个判断，如果没有code，交给微信生成了带code的URL回来继续处理
            $result = Weichat::createUrlWithCode("http://shop.taoljt.com/weiChatPay/");//这儿填写的地址，会拼接上code=xxx然后在下面跳转
            Header("Location: $result");
        }
        $oauthAccessToken = Weichat::getOauthAccessToken();//上面的跳转回来，获取用户的信息
        $openId = $oauthAccessToken->openid;
        $goods_name = "付钱吧";
        $total_money = 2;
        $perpay_id = Weichat::getPrepayId($openId, $goods_name, $total_money);//获取perpay_id，最复杂的一步操作，实际的内容为将要生成的订单参数通过xml格式发送给微信端，微信端需要提前生成订单，然后告知我们订单号是多少！
        $jsApiParameters = Weichat::getParameters($perpay_id);//生成支付页面js-sdk中所需要的参数
        $ctemplate=new CTemplate("weiChatPay.php","dafault",__DIR__."/../Template");
        $ctemplate->render(array("jsApiParameters"=>$jsApiParameters));
    }

    /*
     * @author tangwei
     *
     * @time 2016-01-25 10:52:04
     *
     * @function 微信端，申请退款
     */
    public function weiChatRefund()
    {
        if (!isset($_POST["out_trade_no"]) || !isset($_POST["refund_fee"]))//传递过来的订单号和退款金额
        {
            $ctemplate=new CTemplate("weiChatRefund.php","dafault",__DIR__."/../Template");//获取订单信息
            $ctemplate->render(array("jsApiParameters"=>""));
        }else{
            $out_trade_no = $_POST["out_trade_no"];//需要退款的订单号
            $refund_fee = $_POST["refund_fee"];//需要退款的金额
            $total_money = 1;//订单总金额
            $result = Weichat::applyRefund($out_trade_no, $refund_fee, $total_money);//申请退款
            if($result["return_code"] == "FAIL"){//如果申请失败
            }else{//申请成功
            }
        }
    }

    /*
     * @author tangwei
     *
     * @time 2016-01-26 10:36:46
     *
     * @function 微信生成带参数的临时二维码
     */
    public function weiChatCreateTmp()
    {
        $token = Weichat::getAccessToken();
        $token = $token->access_token;
        $param["expire_seconds"] = 604800;
        $param["action_name"] = "QR_SCENE";
        $param["action_info"] = array("scene"=>array("scene_id"=>600));//scene_id这个参数，自己设置，一般在做微分销的时候会用来区分
        $param = json_encode($param);
        $result = Weichat::createTmpCode($param, $token);//生成二维码
        Header("Location:$result");//这儿就生成了二维码，然后扫描进入之后，就会触发scan事件，交给消息处理那边
    }


    /*
     * @author tangwei
     *
     * @time 2016-01-26 13:36:03
     *
     * @function 微信创建自定义菜单
     */
    public function weiChatCreateMenu()
    {
        $token = Weichat::getAccessToken();

        $token = $token->access_token;

        $data = '{
             "button":[
             {
                  "type":"click",
                  "name":"今日歌曲",
                  "key":"V1001_TODAY_MUSIC"
              },
              {
                   "name":"菜单",
                   "sub_button":[
                   {
                       "type":"view",
                       "name":"搜索",
                       "url":"http://www.soso.com/"
                    },
                    {
                       "type":"view",
                       "name":"视频",
                       "url":"http://v.qq.com/"
                    },
                    {
                       "type":"click",
                       "name":"赞一下我们",
                       "key":"V1001_GOOD"
                    }]
               }]
         }';

        $re = Weichat::createMenu($data, $token);

        var_dump($re);

    }

    /*-----------------------微信摇一摇红包,开始----------------------*/

    /*
     * @author tangwei
     *
     * @time 2016-03-08 09:22:09
     *
     * @function 红包预下单接口,商户创建红包活动之前操作
     */
    public function perPay()
    {
        $data = array(

            "mch_billno"        =>Weichat::MCHID.date("yyyymmis"),//商户记录订单号，每个订单号都不能相同

            "mch_id"            =>Weichat::MCHID,//红包提供者的商户号

            "wxappid"           =>Weichat::APPID,//红包提供者公众号的appid

            "send_name"         =>"测试摇一摇",//红包提供者名称，展示在红包页面

            "hb_type"           =>"NORMAL",//红包类型NORMAL为正常红包，GROUP为裂变红包

            "auth_mchid"        =>1000052601,//摇一摇商家后台固定值

            "auth_appid"        =>"wxbf42bd79c4391863",//摇一摇商家APPID固定值

            "total_amount"      =>115,//总金额，单位分

            "amt_type"          =>"ALL_RAND",//红包金额设置方式，只对裂变红包生效

            "total_num"         =>1,//红包发放总人数，即总共有多少人可以领到该组红包（包括分享者）。普通红包填1，裂变红包必须大于1。

            "wishing"           =>"抢枪抢",//红包祝福语，展示在红包页面

            "act_name"          =>"抢枪抢",//活动名称

            "remark"            =>"抢枪抢",//备注信息

            "risk_cntl"         =>"NORMAL",//用于管控接口风险

            "nonce_str"         =>Weichat::createNoncestr(),//随机字符串不能长于32位

        );

        $data["sign"] = Weichat::getSign($data);//请求签名

        $xmlData = Weichat::arrayToXml($data);

        $url = "https://api.mch.weixin.qq.com/mmpaymkttransfers/hbpreorder";

        $result = Weichat::postXmlSSLCurl($xmlData, $url);

        var_dump($result);
    }
    /*
     * @author tangwei
     *
     * @time 2016-03-07 15:43:11
     *
     * @function 摇一摇红包第一步，创建红包活动
     */
    public function createRedActivity()
    {
        $ctemplate=new CTemplate("createRedActivity.php","dafault",__DIR__."/../Template");

        $ctemplate->render(array());
    }

    /*
     * @author tangwei
     *
     * @time 2016-03-07 16:17:32
     *
     * @function 新增一个红包的逻辑操作
     */
    public function postCreateRedActivity()
    {

        $isUseTemplate = (isset($_POST["isdefault"]) && ($_POST["isdefault"] ==1 || $_POST["isdefault"] == 0)) ? $_POST["isdefault"] : 1;//是否使用微信抢红包的原生模板

        $token = Weichat::getAccessToken();

        $token = $token->access_token;//获取请求的access_token

        $url = "https://api.weixin.qq.com/shakearound/lottery/addlotteryinfo?access_token=".$token."&use_template=".$isUseTemplate."&logo_url=LOGO_URL";

        $title = (isset($_POST["title"]) && $_POST["title"] != "") ? strip_tags(trim($_POST["title"])) : "";

        if($title == "") $this->errorBack("活动名称不能为空！");

        $desc = (isset($_POST["des"]) && $_POST["des"] != "") ? strip_tags(trim($_POST["des"])) : "";

        $total = (isset($_POST["total"]) && is_numeric($_POST["total"]) && $_POST["total"] >=0) ? $_POST["total"] : 0;

        if($total == 0) $this->errorBack("红包总数不能为0");

        $data = array(

             "title"        => $title,

             "desc"         => $desc,

             "onoff"        => 1,

             "begin_time"   => time(),

             "expire_time"  => time()+4*24*60*60,

             "sponsor_appid"=>Weichat::APPID,

             "total"        => $total,

             "jump_url"     => ""
        );

        $data["key"] = Weichat::getSign($data);

        $data = json_encode($data);

        $result = Weichat::postCurl($data, $url);

        var_dump($result);

    }

    /*
     * @author tangwei
     *
     * @time 2016-03-10 08:43:21
     *
     * @function 录入红包信息,就是往一个红包活动中录入红包
     */
    public function addRedInfo()
    {
        //这儿需要判断如果录入的红包数量如果大于新增红包中的total值，那么抛出错误
        $token = Weichat::getAccessToken();

        $token = $token->access_token;

        $url = "https://api.weixin.qq.com/shakearound/lottery/setprizebucket?access_token=".$token;//如果，一次往一个红包活动中录入的数量小于100，调用一次该接口就行，如果大于100个需要循环调用

        $data = array(
            "lottery_id"        => "hJbsSXYAz4Wlk2TvUTV_Zw",

            "mchid"             => Weichat::MCHID,

            "sponsor_appid"     => Weichat::APPID,

            "prize_info_list"   =>array(

                array(

                    "ticket"    =>  "v1|x6Vgvf5Dpc/TEJrZ74BDV2GILLa9JcW2RZ7nDLsMSf5LktAPyjiXS1a/10v7DmiQIaorUmsJyDTVrcviJhUHFW8dEgcUEagYPMYI6fEqyzgkX0Wq4/3SS0pDNxdcxEFYU0xYcTSrvGiOAK2dhhE1Ug=="

                ),

                array(

                    "ticket"    =>  "v1|x6Vgvf5Dpc/TEJrZ74BDV6B+aAbKqcYJLCpKlZHYFiRLktAPyjiXS1a/10v7DmiQIaorUmsJyDTVrcviJhUHFWQPVumN1kLJRQMyv1Lbi1YuI64qZuOgfx8Gef7f6+YFMDJ4suEgKwMzaPDdFM+oWg=="

                ),

                array(

                    "ticket"    =>  "v1|x6Vgvf5Dpc/TEJrZ74BDV8hHHBbeHg5fC67GVTg3liZLktAPyjiXS1a/10v7DmiQIaorUmsJyDTVrcviJhUHFabrmmbFZDc21jRCTpmC88tEKvYzHFR6p7/FVIuk2VsByJ8W9awMV7lJbBl6+Dj/pA=="

                ),

                array(

                    "ticket"    =>  "v1|x6Vgvf5Dpc/TEJrZ74BDV2FUOwich1zy5WnWaZJFn8hLktAPyjiXS1a/10v7DmiQIaorUmsJyDTVrcviJhUHFVmKn9zKtfxIp/j4og0LzPDKSHp8m88bODuwZQhXBwsMQFYOJRx0jRweeDC58dVr2g=="

                ),

                array(

                    "ticket"    =>  "v1|x6Vgvf5Dpc/TEJrZ74BDV3crUGFQWHSfTVYshHS8echLktAPyjiXS1a/10v7DmiQIaorUmsJyDTVrcviJhUHFY98qyEfRU26TyEIzMZyVKyhvoS+HFTCB08508o4WjhZNe0SoL4vqcQTIsmF2znErA=="

                )



            )
        );

        $data = json_encode($data);

        $result = Weichat::postCurl($data, $url);

        var_dump($result);

    }
    /*-----------------------微信摇一摇红包，结束----------------------*/


    /*
     * @author tangwei
     *
     * @time 2018-03-08 10:02:43
     *
     * @function 商家向用户打款，只需调用下面方法即可
     */
    public function payToUser()
    {
        $weichat = new Weichat();

        $weichat->payToUser();//这儿需要加参数
    }

    /*
     * @author tangwei
     *
     * @time 2018-01-17 09:17:10
     *
     * @function 向小程序调起微信支付提供参数
     */
    public function apipay()
    {
        $openId = $_GET["openid"];

        $goods_name = "付钱吧";

        $total_money = 2;

        $prepay_id = Weichat::getPrepayId($openId, $goods_name, $total_money);

        echo Weichat::getParameters($prepay_id);
    }
}