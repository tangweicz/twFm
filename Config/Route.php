<?php
/**
 * Created by PhpStorm.
 * User: tangwei
 * Date: 15/10/1
 * Time: 14:50
 */

//
use NoahBuscher\Macaw\Macaw as route;

//route::get("/","Controllers\\taskController@UpdateUserName");
//
//route::get("/index","Controllers\\taskController@UpdateUserName");
//
//route::get("/test/tangwei/route","Controllers\\taskController@UpdateUserName");
//

route::get("/test", "Controllers\\adminIndexController@index");

route::get("/index/","Controllers\\indexController@index");//微信网页授权登陆

route::get("/joinWeiCat/","Controllers\\indexController@joinWeiCat");//接入微信的时候打开

route::post("/joinWeiCat/","Controllers\\indexController@joinWeiCat");//接入微信，并且成为微信开发的入口，用于接收回复用户消息

route::get("/addJsSdk/","Controllers\\indexController@addJsSdk");//增加微信的js-sdk的功能，以便以后的分享操作、调起微信拍照等一系列的微信js-sdk的功能

route::get("/weiCatPay/","Controllers\\indexController@weiCatPay");//微信H5网页支付

route::get("/weiCatRefund/","Controllers\\indexController@weiCatRefund");//微信申请退款

route::post("/weiCatRefund/","Controllers\\indexController@weiCatRefund");//微信申请退款

route::get("/weiCatCreateTmp/","Controllers\\indexController@weiCatCreateTmp");//微信生成带参数的临时的二维码

route::get("/weiCatCreateMenu/","Controllers\\indexController@weiCatCreateMenu");

route::get("/prepay/","Controllers\\indexController@perPay");//红包预下单

route::get("/addred/","Controllers\\indexController@createRedActivity");//新增红包活动

route::post("/addred/","Controllers\\indexController@postCreateRedActivity");//Post新增红包活动

route::get("/addredinfo/","Controllers\\indexController@addRedInfo");//Post新增红包活动

route::get("/admin/index/","Controllers\\adminIndexController@index");//后台管理登陆之后的首页

route::get("/admin/index/top/","Controllers\\adminIndexController@top");//后台管理登陆之后的首页，需要调用的子页面

route::get("/admin/index/menu/","Controllers\\adminIndexController@menu");//后台管理登陆之后的首页，需要调用的子页面


route::dispatch();

