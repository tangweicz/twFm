<html>
<head>
    <title>menu</title>
    <link rel="stylesheet" href="/css/base.css" type="text/css" />
    <link rel="stylesheet" href="/css/menu.css" type="text/css" />
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
    <script language='javascript'>var curopenItem = '1';</script>
    <script language="javascript" type="text/javascript" src="/js/menu.js"></script>
    <base target="main" />
</head>
<body target="main">
<table width='99%' height="100%" border='0' cellspacing='0' cellpadding='0'>
    <tr>
        <td style='padding-left:3px;padding-top:8px' valign="top">
            <!-- Item 1 Strat -->
            <dl class='bitem'>
                <dt onClick='showHide("items1_1")'><b>轮播图管理</b></dt>
                <dd style='display:block' class='sitem' id='items1_1'>
                    <ul class='sitemu'>
                        <!--<li><a href='./help' target='main'>帮助页内容编辑</a></li>-->
                        <li><a href="/admin/lunbo/listall/" target='main'>所有轮播图列表</a></li>

                        <li><a href="/admin/lunbo/add/" target='main'>新增轮播图</a></li>
                    </ul>
                </dd>
            </dl>
            <!-- Item 4 Strat -->
            <dl class='bitem'>
                <dt onClick='showHide("items7_1")'><b>微心愿管理</b></dt>
                <dd style='display:block' class='sitem' id='items7_1'>
                    <ul class='sitemu'>
                        <li><a href="/admin/xinyuan/listall/" target='main'>所有微心愿列表</a></li>

                        <li><a href="/admin/xinyuan/add/" target='main'>新增微心愿</a></li>

                        <li><a href="/admin/xinyuan/listunclaimant/" target='main'>未被领取的微心愿</a></li>

                        <li><a href="/admin/xinyuan/list/" target='main'>领取的待审核微心愿</a></li>

                        <li><a href="/admin/xinyuan/listsuccess/" target='main'>被成功领取的微心愿</a></li>
                    </ul>
                </dd>
            </dl>
            <!-- Item 4 End -->

        </td>
    </tr>
</table>
</body>
</html>