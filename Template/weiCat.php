<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <title></title>
</head>
<body>

</body>
<script src="http://res.wx.qq.com/open/js/jweixin-1.0.0.js"></script>
<script>
    /*
     * 注意：
     * 1. 所有的JS接口只能在公众号绑定的域名下调用，公众号开发者需要先登录微信公众平台进入“公众号设置”的“功能设置”里填写“JS接口安全域名”。
     * 2. 如果发现在 Android 不能分享自定义内容，请到官网下载最新的包覆盖安装，Android 自定义分享接口需升级至 6.0.2.58 版本及以上。
     * 3. 常见问题及完整 JS-SDK 文档地址：http://mp.weixin.qq.com/wiki/7/aaa137b55fb2e0456bf8dd9148dd613f.html
     *
     * 开发中遇到问题详见文档“附录5-常见错误及解决办法”解决，如仍未能解决可通过以下渠道反馈：
     * 邮箱地址：weixin-open@qq.com
     * 邮件主题：【微信JS-SDK反馈】具体问题
     * 邮件内容说明：用简明的语言描述问题所在，并交代清楚遇到该问题的场景，可附上截屏图片，微信团队会尽快处理你的反馈。
     */
    wx.config({
        debug: true,
        appId: '<?php echo $result["appId"];?>',
        timestamp: <?php echo $result["timestamp"];?>,
    nonceStr: '<?php echo $result["nonceStr"];?>',
            signature: '<?php echo $result["signature"];?>',
            jsApiList: [
                'chooseImage',
                'uploadImage',
                'onMenuShareQQ',
                'onMenuShareAppMessage',
                'scanQRCode',
                'hideMenuItems'
        // 所有要调用的 API 都要加到这个列表中
    ]
    });
    wx.ready(function () {
        wx.hideMenuItems({
            menuList: ['menuItem:share:timeline'] // 要隐藏的菜单项，只能隐藏“传播类”和“保护类”按钮，所有menu项见附录3
        });

        // 在这里调用 API
        wx.onMenuShareQQ({//分享给QQ好友
            title: '测试分享', // 分享标题
            desc: '测试一下', // 分享描述
            link: 'http://www.baidu.com', // 分享链接
            imgUrl: 'http://image.baidu.com/search/detail?ct=503316480&z=0&ipn=d&word=%E5%9B%BE%E7%89%87&pn=1&spn=0&di=112989922650&pi=&rn=1&tn=baiduimagedetail&ie=utf-8&oe=utf-8&cl=2&lm=-1&cs=1183223528%2C3058066243&os=123530706%2C110939732&simid=4263705247%2C758731806&adpicid=0&ln=30&fr=ala&sme=&cg=&bdtype=0&objurl=http%3A%2F%2Fpic14.nipic.com%2F20110522%2F7411759_164157418126_2.jpg&fromurl=ippr_z2C%24qAzdH3FAzdH3Fooo_z%26e3Bgtrtv_z%26e3Bv54AzdH3Ffi5oAzdH3FnAzdH3F8cAzdH3F9m8c9bahjj01b8dv_z%26e3Bip4s&gsm=0', // 分享图标
            success: function () {
                // 用户确认分享后执行的回调函数
            },
            cancel: function () {
                // 用户取消分享后执行的回调函数
            }
        });

        wx.onMenuShareAppMessage({//分享给微信好友
            title: '测试123', // 分享标题
            desc: '123456', // 分享描述
            link: 'http://www.baidu.com', // 分享链接
            imgUrl: '123', // 分享图标
            type: '', // 分享类型,music、video或link，不填默认为link
            dataUrl: '', // 如果type是music或video，则要提供数据链接，默认为空
            success: function () {
                // 用户确认分享后执行的回调函数
            },
            cancel: function () {
                // 用户取消分享后执行的回调函数
            }
        });
    });


</script>

<button id="imgs">点击拍照</button>

<button id="upload">点击上传拍照</button>

<br/><br/><br/><br/>
<button id="scan">扫一扫</button>

<script>
    window.onload=function(){
        var localIds = [];//存放拍照或者选取相册成功之后的数据

        var img = document.getElementById("imgs");

        img.onclick=function(){//选择照片或拍照
            //选择照片，拍照或者选择相册都可以
            wx.chooseImage({

                count: 2, // 默认9，选择照片的最多数量

                sizeType: ['original', 'compressed'], // 可以指定是原图还是压缩图，默认二者都有

                sourceType: ['album', 'camera'], // 可以指定来源是相册还是相机，默认二者都有

                success: function (res) {

                    localIds = res.localIds; // 返回选定照片的本地ID列表，localId可以作为img标签的src属性显示图片，此时数据格式为数组
                }
            });
        }

        var upload = document.getElementById("upload");

        upload.onclick=function(){//上传照片

            var i = 0,len = localIds.length;//获取需要上传图片的数据

            function up(){
                wx.uploadImage({

                    localId: localIds[i], // 需要上传的图片的本地ID，由chooseImage接口获得

                    isShowProgressTips: 1, // 默认为1，显示进度提示

                    success: function (res) {
                        i++;
                        //var serverId = res.serverId; // 返回图片的服务器端ID
                        if(i<len){
                            up();
                        }

                    }
                });
            }

            up();

        }

        var scan = document.getElementById("scan");

        scan.onclick=function(){
            wx.scanQRCode({//扫一扫开始

                needResult: 0, // 默认为0，扫描结果由微信处理，1则直接返回扫描结果，

                scanType: ["qrCode","barCode"], // 可以指定扫二维码还是一维码，默认二者都有

                success: function (res) {

                    var result = res.resultStr; // 当needResult 为 1 时，扫码返回的结果

                }

            });
        }




    }

</script>
</html>
