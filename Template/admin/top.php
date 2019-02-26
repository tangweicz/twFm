<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
    <title>top</title>
    <link href="/css/base.css" rel="stylesheet" type="text/css" />
    <style type="text/css">
        body { padding:0px; margin:0px; }
        p {padding:0px; margin:0px; }
        .top {background:url(/images/top-Bg.gif) repeat-x bottom; height:130px; }
        .topT { height:97px;  }
        .topB { height:33px; overflow:hidden; }
        .loginInfo { float:right; padding-right:30px; color:#fff;line-height:33px; }
        .loginInfo img { float:left; margin:5px 5px 0px 0px; }
        .topR { float:right; margin:20px 30px 0px 0px; display:inline; }
        .topR li { width:70px; float:left; text-align:center; }
        .topR li a { color:#fff; }
    </style>
    <script>
        window.onload=function(){
            var date=new Date();
            var now=date.toLocaleDateString();
            var obj=document.getElementById("date");
            obj.innerHTML=now;
        }
    </script>
</head>
<body>
<div class="top">
    <div class="topT">
        <ul class="topR">

            <li>
                <a href="/admin/login/loginout" target="_top" target='main'>
                    <img src="/images/quit.png" alt="注销退出" />
                    <br />
                    <span>注销退出</span>
                </a>
            </li>
        </ul>
    </div>
    <div class="topB">
        <p class="loginInfo"><img src="/images/userIco.png" alt="" /> 当前用户：admin 欢迎您登陆管理后台系统！ 今天<span id="date"></span></p>
    </div>
</div>
</body>
</html>
