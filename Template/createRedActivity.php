<html>

    <head>
        <title>创建摇一摇红包活动</title>
    </head>

    <body>

        <form method="post" action="">
            红包活动名称：<input type="text" name="title" /><br />

            是否使用微信模板页面：<input type="radio" value="1" checked="checked" name="isdefault"> 是 <input type="radio" value="0" name="isdefault"> 否 <br />

<!--            使用模板页面的图片URL：<input type="file" name="myfile[]" /> <br />-->

            红包活动描述： <input type="text" name="des" /> <br />

            红包总数：<input type="text" name="total" /> <br />

            <input type="submit" value="提交" />
        </form>

    </body>
</html>