#!/Library/Frameworks/Python.framework/Versions/3.6/bin/python3.6
#! -*- coding=utf-8 -*-

import pymysql, datetime, time, sys, subprocess, re, os, shutil
from itertools import combinations
host = ""
port = 0
username = ""
password = ""
database = ""
phpVersionStr = subprocess.getoutput("php -v")
phpVersionList = phpVersionStr.split(" ")
phpVersionStr = str(phpVersionList[1])
if phpVersionStr.startswith("7"):
    phpVersion = 7
elif phpVersionStr.startswith("5"):
    phpVersion = 5
else:
    phpVersion = 0
if phpVersion == 0:
    print("系统不支持的php版本，请更新本地php的版本")
    exit()

with open("./Config/Config.php", "r") as configFile:#解析Config/Config.php文件，读取到 数据库的配置
    while True:
        data = configFile.readline()
        if data == "":
            break
        data = data.strip()
        if data.startswith('"host"') or data.startswith('"HOST"'):
            host = data.split("=>")[1].strip()
            leng = len(host)
            if host.endswith("\","):
                host = host[1:leng-2]
            else:
                host = host[1:leng-1]
        if data.startswith('"port"') or data.startswith('"PORT"'):
            port = data.split("=>")[1].strip()
            leng = len(port)
            if port.endswith("\","):
                port = port[1:leng-2]
            else:
                if port.endswith(","):
                    port = port[0:leng - 1]
                else:
                    port = port
        if data.startswith("\"user\"") or data.startswith("\"USER\""):
            username = data.split("=>")[1].strip()
            leng = len(username)
            if username.endswith("\","):
                username = username[1:leng - 2]
            else:
                username = username[1:leng - 1]
        if data.startswith("\"pass\"") or data.startswith("\"PASS\""):
            password = data.split("=>")[1].strip()
            leng = len(password)
            if password.endswith("\","):
                password = password[1:leng - 2]
            else:
                password = password[1:leng - 1]
        if data.startswith("\"dbname\"") or data.startswith("\"DBNAME\""):
            database = data.split("=>")[1].strip()
            leng = len(database)
            if database.endswith("\","):
                database = database[1:leng - 2]
            else:
                database = database[1:leng - 1]
db = pymysql.connect(host=host, user=username, password=password, db=database, port=int(port))#连击额数据库
curfd = db.cursor()
sql = "select table_name from information_schema.tables where table_schema='"+database+"'"#读取所有表的sql语句
curfd.execute(sql)#执行sql语句
results = curfd.fetchall()
totalNum = len(results)
nowNum = 0
unitTestFinishedFileFd = open("./unitTestFinishedLog.txt", "w+")#所有的单元测试跑完，写入到日志文件中去
for row in results:#循环出所有的表名
    everyTableName = row[0]
    sql = "show create table `"+everyTableName+"`"#开始查看每张表的字段信息
    curfd.execute(sql)
    createTableInfo = curfd.fetchall()
    createTableInfoStr = createTableInfo[0][1]#拿到表信息，eg:create table `xxxx`(`id` int(11))engine=innodb default charset=utf8;
    createTableInfoStrDetail = createTableInfoStr.split("\n")#将上面拿到的字符串信息用\n去切片，保存到一个列表中去
    tableColumnList = list()#用来存放当前表字段信息
    for detail in createTableInfoStrDetail:#循环整个列表
        tableListTmp = dict()#用来存放当前字段的信息
        detail = detail.strip()
        if detail.startswith("`"):#如果一行字符串是以`开头，那么证明使我们要的信息
            detailList = detail.split(" ")
            if not detailList[0][1:-1] == "id":
                tableListTmp["name"] = detailList[0][1:-1]#拿到诸如：id、name、等字段名称
                if "int" in detailList[1]:#如果int在字符串中，那么我默认类型就为int型
                    tableListTmp["type"] = "int"
                elif "char" in detailList[1]:#如果char在字符中，那么我默认类型为varchar型
                    tableListTmp["type"] = "varchar"
                elif "text" in detailList[1]:#如果text在字符中，那么我默认类型为text型
                    tableListTmp["type"] = "text"
                elif "date" in detailList[1]:#如果text在字符中，那么我默认类型为datetime型
                    tableListTmp["type"] = "datetime"
                elif "float" in detailList[1]:#如果float在字符中，那么我默认类型为int型
                    tableListTmp["type"] = "int"
                elif "decimal" in detailList[1]:#如果decimal在字符中，那么我默认类型为int型
                    tableListTmp["type"] = "int"

                if tableListTmp["type"] == "int":
                    tableListTmp["other"] = detailList[2]
                else:
                    tableListTmp["other"] = "not null"
        if len(tableListTmp) > 0:
            tableColumnList.append(tableListTmp)#将数据写入到列表中
    modelFileName = ""
    controllerFileName = ""
    modelTestFileName = ""
    noExtensionModelFileName = ""
    count = 0
    if "_" in everyTableName:#如果有下划线，那么将下划线后面的首字母大写
        compos = everyTableName.split("_")
        for tmp in compos:
            if not tmp == "":
                if not count == 0:
                    tmp = tmp.capitalize()
                noExtensionModelFileName = noExtensionModelFileName + tmp
                count += 1
        modelFileName = noExtensionModelFileName + "Model.php"
        controllerFileName = noExtensionModelFileName + "Controller.php"
        modelTestFileName = noExtensionModelFileName + "ModelTest.php"
    else:
        noExtensionModelFileName = everyTableName
        modelFileName = noExtensionModelFileName+"Model.php"
        controllerFileName = noExtensionModelFileName + "Controller.php"
        modelTestFileName = noExtensionModelFileName + "ModelTest.php"

    unitTestCommandList = list()  # 存放所有的单元测试命令
    allColumnWithIdList = list()
    hasIsDeleted = False  # 判断是否有isDeleted字段，即软删除字段
    hasSort = False  # 判断是否有sort字段，排序字段
    for column in tableColumnList:
        if "id" in column["name"].lower():
            allColumnWithIdList.append(column["name"])
        if column["name"] == "isDeleted":
            hasIsDeleted = True
        if column["name"] == "sort":
            hasSort = True


    zhushiStr = "    /**\r     * User: tangwei\r     * Date: " + time.strftime('%Y.%m.%d', time.localtime(time.time())) + "\r     * @param {params}\r     * @return\r     * Function:{usage}\r     */\r"  # 注释文本
    with open("./Controllers/"+controllerFileName, "w+") as fd:#写入Controller文件
        fd.write("<?php \r")
        fd.write("namespace Controllers;\r")
        fd.write("use Libs\Controller;\r")
        fd.write("use Libs\CTemplate;\r")
        fd.write("use Libs\Page;\r")
        fd.write("use Models\\"+noExtensionModelFileName+"Model;\r")
        fd.write("class " + noExtensionModelFileName + "Controller extends Controller\r{\r")

        #写入 新增数据的界面显示Controller方法
        addStr = zhushiStr.format(params="", usage="新增数据的界面显示")
        addStr += "    public function add"+noExtensionModelFileName+"()\r"
        addStr += "    {\r"
        addStr += "        $ctemplate=new CTemplate(\""+noExtensionModelFileName+"/add.html\",\"dafault\",__DIR__.\"/../Template\");\r"
        addStr += "        $ctemplate->render(array());\r"
        addStr += "    }\r"
        fd.write(addStr)
        if not os.path.exists("./Template/"+noExtensionModelFileName):#判断Template下是否有文件夹，如果没有创建之
            os.mkdir("./Template/"+noExtensionModelFileName)
        shutil.copy("./Template/add.html", "./Template/"+noExtensionModelFileName+"/add.html")#拷贝add.html到上面的文件夹中


        #写入 新增数据的逻辑操作的Controller方法
        addStr = zhushiStr.format(params="", usage="新增数据的逻辑操作")
        addStr += "    public function doadd"+noExtensionModelFileName+"()\r"
        addStr += "    {\r"
        if len(tableColumnList) > 0:#循环表中字段，判断controller层中的post数据判断
            for item in tableColumnList:
                if item["name"] != "isDeleted" and item["name"] != "addTime" and item["name"] != "sort":#如果字段名为isDeleted为0时，字段是不需要post的提交，然后新增数据的。所以这儿判断，不为isDleted等字段就不需要。
                    if not item["type"] == "int":
                        addStr += "        if(!isset($_POST[\""+item["name"]+"\"]) || trim($_POST[\""+item["name"]+"\"]) == \"\")$this->errorBack(\""+item["name"]+" must upload\");\r"
                    else:
                        addStr += "        if(!isset($_POST[\"" + item["name"] + "\"]) || !is_numeric($_POST[\""+item["name"]+"\"]) || $_POST[\"" + item["name"] + "\"] <= 0 )$this->errorBack(\"" + item["name"] + " correct type\");\r"
                    addStr += "        $"+item["name"]+" = $this->filter($_POST[\""+item["name"]+"\"]);\r"
        addStr += "        $" + noExtensionModelFileName + "Model = new " + noExtensionModelFileName + "Model();\r"
        if len(tableColumnList) > 0:
            for item in tableColumnList:
                if item["name"] == "isDeleted" or item["name"] == "sort":
                    addStr += "        $" + noExtensionModelFileName + "Model->"+item["name"]+" = 0;\r"
                elif "time" in item["name"].lower():
                    addStr += "        $" + noExtensionModelFileName + "Model->"+item["name"]+" = date(\"Y-m-d H:i:s\");\r"
                else:
                    addStr += "        $" + noExtensionModelFileName + "Model->"+item["name"]+" = $"+item["name"]+";\r"
        addStr += "        $res = $"+noExtensionModelFileName+"Model->addOne();\r"
        addStr += "        if(!empty($res))$this->jump(\"新增成功\", \"/"+noExtensionModelFileName+"List\");\r"
        addStr += "        $this->jump(\"新增失败\", \"/add"+noExtensionModelFileName+"\");\r"
        addStr += "    }\r"
        fd.write(addStr)

        #写入 编辑数据的界面显示的Controller方法
        editStr = zhushiStr.format(params="", usage="编辑数据的界面显示")
        editStr += "    public function edit"+noExtensionModelFileName+"()\r"
        editStr += "    {\r"
        editStr += "        if (!isset($_GET[\"id\"]) || !is_numeric($_GET[\"id\"]) || $_GET[\"id\"] <= 0) $this->errorBack(\"参数错误\");\r"
        editStr += "        $id = $this->filter($_GET[\"id\"]);\r"
        editStr += "        $"+noExtensionModelFileName+"Date = "+noExtensionModelFileName+"Model::getOneById($id);\r"
        editStr += "        if(empty($"+noExtensionModelFileName+"Date))$this->errorBack(\"不存在该数据或已被删除\");\r"
        editStr += "        $"+noExtensionModelFileName+"Date = $"+noExtensionModelFileName+"Date[0];\r"
        editStr += "        $ctemplate=new CTemplate(\""+noExtensionModelFileName+"/edit.html\",\"dafault\",__DIR__.\"/../Template\");\r"
        editStr += "        $ctemplate->render(array(\""+noExtensionModelFileName+"Date\"=>$"+noExtensionModelFileName+"Date));\r"
        editStr += "    }\r"
        fd.write(editStr)
        if not os.path.exists("./Template/"+noExtensionModelFileName):#判断Template下是否有文件夹，如果没有创建之
            os.mkdir("./Template/"+noExtensionModelFileName)
        shutil.copy("./Template/edit.html", "./Template/"+noExtensionModelFileName+"/edit.html")#拷贝add.html到上面的文件夹中

        #写入 编辑数据的逻辑操作的Controller方法
        editStr = zhushiStr.format(params="", usage="编辑数据的逻辑操作")
        editStr += "    public function doedit" + noExtensionModelFileName + "()\r"
        editStr += "    {\r"
        editStr += "        if (!isset($_GET[\"id\"]) || !is_numeric($_GET[\"id\"]) || $_GET[\"id\"] <= 0) $this->errorBack(\"参数错误\");\r"
        editStr += "        $id = $this->filter($_GET[\"id\"]);\r"
        if len(tableColumnList) > 0:#循环表中字段，判断controller层中的post数据判断
            for item in tableColumnList:
                if item["name"] != "isDeleted" and item["name"] != "addTime" and item["name"] != "sort":#如果字段名为isDeleted为0时，字段是不需要post的提交，然后新增数据的。所以这儿判断，不为isDleted等字段就不需要。
                    if not item["type"] == "int":
                        editStr += "        if(!isset($_POST[\""+item["name"]+"\"]) || trim($_POST[\""+item["name"]+"\"]) == \"\")$this->errorBack(\""+item["name"]+" must upload\");\r"
                    else:
                        editStr += "        if(!isset($_POST[\"" + item["name"] + "\"]) || !is_numeric($_POST[\""+item["name"]+"\"]) || $_POST[\"" + item["name"] + "\"] <= 0 )$this->errorBack(\"" + item["name"] + " correct type\");\r"
                    editStr += "        $"+item["name"]+" = $this->filter($_POST[\""+item["name"]+"\"]);\r"
        editStr += "        $" + noExtensionModelFileName + "Date = " + noExtensionModelFileName + "Model::getOneById($id);\r"
        editStr += "        if(empty($" + noExtensionModelFileName + "Date))$this->errorBack(\"不存在该数据或已被删除\");\r"
        editStr += "        $" + noExtensionModelFileName + "Date = $" + noExtensionModelFileName + "Date[0];\r"
        if len(tableColumnList) > 0:
            for item in tableColumnList:
                if item["name"] == "isDeleted" or item["name"] == "sort":
                    editStr += ""
                elif item["name"] == "addTime":
                    editStr += ""
                else:
                    editStr += "        $" + noExtensionModelFileName + "Date->"+item["name"]+" = $"+item["name"]+";\r"
        editStr += "        $res = $"+noExtensionModelFileName+"Date->editOne();\r"
        editStr += "        $this->jump(\"编辑成功\", \"/"+noExtensionModelFileName+"List\");\r"
        editStr += "    }\r"
        fd.write(editStr)

        #写入 删除一条记录的逻辑操作的Controller方法
        deleteStr = zhushiStr.format(params="", usage="删除一条数据的逻辑操作")
        deleteStr += "    public function delete" + noExtensionModelFileName + "()\r"
        deleteStr += "    {\r"
        deleteStr += "        if (!isset($_GET[\"id\"]) || !is_numeric($_GET[\"id\"]) || $_GET[\"id\"] <= 0) $this->errorBack(\"参数错误\");\r"
        deleteStr += "        $id = $this->filter($_GET[\"id\"]);\r"
        deleteStr += "        $" + noExtensionModelFileName + "Date = " + noExtensionModelFileName + "Model::getOneById($id);\r"
        deleteStr += "        if(empty($" + noExtensionModelFileName + "Date))$this->errorBack(\"不存在该数据或已被删除\");\r"
        deleteStr += "        $" + noExtensionModelFileName + "Date = $" + noExtensionModelFileName + "Date[0];\r"
        if hasIsDeleted:
            deleteStr += "        $res = $" + noExtensionModelFileName + "Date->deleteUpdateOne();\r"
        else:
            deleteStr += "        $res = $" + noExtensionModelFileName + "Date->deleteOne();\r"
        deleteStr += "        if(!empty($res))$this->jump(\"操作成功\", \"/" + noExtensionModelFileName + "List\");\r"
        deleteStr += "        $this->jump(\"操作失败\", \"/" + noExtensionModelFileName + "List\");\r"
        deleteStr += "    }\r"
        fd.write(deleteStr)

        # 写入 数据的列表展示页面的Controller方法
        listStr = zhushiStr.format(params="", usage="显示列表页面")
        listStr += "    public function " + noExtensionModelFileName + "List()\r"
        listStr += "    {\r"
        listStr += "        $all"+noExtensionModelFileName+" = "+noExtensionModelFileName+"Model::getAllUndeletedWithOutLimit();\r"
        listStr += "        $allNum = count($all"+noExtensionModelFileName+");\r"
        listStr += "        $page = new Page($allNum,self::$PAGESIZE);\r"
        listStr += "        $all"+noExtensionModelFileName+"Date = "+noExtensionModelFileName+"Model::getAllUndeletedWithLimit($page->limit);\r"
        listStr += "        $ctemplate=new CTemplate(\"articleCate/list.html\",\"dafault\",__DIR__.\"/../Template\");\r"
        listStr += "        $all" + noExtensionModelFileName + "Date = $all" + noExtensionModelFileName + "Date[0];\r"
        listStr += "        $ctemplate->render(array(\"all"+noExtensionModelFileName+"Date\"=>$all"+noExtensionModelFileName+"Date, \"page\"=>$page->showpage()));\r"
        listStr += "    }\r"
        fd.write(listStr)
        if not os.path.exists("./Template/"+noExtensionModelFileName):#判断Template下是否有文件夹，如果没有创建之
            os.mkdir("./Template/"+noExtensionModelFileName)
        shutil.copy("./Template/list.html", "./Template/"+noExtensionModelFileName+"/list.html")#拷贝add.html到上面的文件夹中

        fd.write("\r}")
        fd.close()

    testFileFd = open("./test/"+modelTestFileName, "w+")
    testFileFd.write("<?php\r")
    testFileFd.write("use Models\\"+modelTestFileName[0:-8]+";\r")
    testFileFd.write("class "+modelTestFileName[0:-4]+" extends PHPUnit_Framework_TestCase {\r")
    with open("./Models/" + modelFileName, "w+") as fd:#创建表名对应的数据库model文件和单元测试文件，下面就是往文件中写入数据
        fd.write("<?php \r")
        fd.write("namespace Models;\r")
        fd.write("use Libs\CObject;\r")
        fd.write("class "+noExtensionModelFileName+"Model extends CObject\r{\r")
        fd.write("    static $table = \""+everyTableName+"\"; \r")
        #针对表的增删改查的代码都从这儿开始写

        #insert into xxx () values ()

        addStr = zhushiStr.format(params="", usage="新增一条记录")+"    public function addOne()"
        if phpVersion == 7:
            addStr += " : array "
        addStr +="\r    {\r"
        columnStr = ""
        questionStr = ""
        columnArrayStr = ""
        for column in tableColumnList:
            if column["type"] == "int":#如果字段是int类型：
                addStr += "        assert(isset($this->" + column["name"] + ") && is_numeric($this->"+column["name"]+") && $this->"+column["name"]+" >= 0);\r"
            else:
                addStr += "        assert(isset($this->"+column["name"]+") && $this->"+column["name"]+" != '' );\r"
            columnStr += "`"+column["name"]+"`,"
            questionStr += " ?,"
            columnArrayStr += "            $this->"+column["name"]+",\r"
        addStr += "        $sql = \"INSERT INTO \".self::$table.\" ({columns}) VALUES ({question})\";\r".format(columns=columnStr[0:-1], question=questionStr[0:-1])
        addStr += "        $sqlParam = array(\r{array});\r".format(array=columnArrayStr[0:-1])
        addStr += "        return self::query($sql, $sqlParam, false);"
        addStr += "\r    }\r"#新增的方法
        fd.write(addStr)
        #phpunit测试insert方法相关代码
        addStr = zhushiStr.format(params="", usage="测试新增一条记录") + "    public function testaddOne()\r    {\r"
        addStr += "        $model = new "+modelFileName[0:-4]+"();\r"
        for column in tableColumnList:
            if "delete" in column["name"].lower():
                addStr += "        $model->" + column["name"] + " = 0;\r"
            else:
                if column["type"] == "int":  # 如果字段是int类型：
                    addStr += "        $model->"+column["name"]+" = 10;\r"
                elif column["type"] == "datetime":
                    addStr += "        $model->" + column["name"] + " = \"2019-02-12\";\r"
                else:
                    addStr += "        $model->"+column["name"]+" = \"xxxx\";\r"
        addStr += "        $res = $model->addOne();\r"
        addStr += "        var_dump($res);\r"
        addStr += "        #./vendor/bin/phpunit --filter testaddOne ./test/"+modelTestFileName
        addStr += "\r    }\r"
        testFileFd.write(addStr)
        unitTestCommandList.append("./vendor/bin/phpunit --filter testaddOne ./test/"+modelTestFileName)


        #delete from xxx where id = xx
        deleteStr = zhushiStr.format(params="", usage="删除记录的方法，不进回收站，直接删数据库的记录")+"    public function deleteOne()"
        if phpVersion == 7:
            deleteStr += " : array "
        deleteStr += "\r    {\r"
        deleteStr += "        $sql = \"DELETE FROM \".self::$table.\" WHERE `id` = ?\";\r"
        deleteStr += "        $sqlParam = array($this->id);\r"
        deleteStr += "        return self::query($sql, $sqlParam, false);"
        deleteStr += "\r    }\r"#删除的方法
        fd.write(deleteStr)
        #phpunit测试delete方法祥光代码
        deleteStr = zhushiStr.format(params="", usage="测试删除记录的方法，不进回收站，直接删数据库的记录") + "    public function testdeleteOne()\r    {\r"
        deleteStr += "        $record = "+modelFileName[0:-4]+"::getOneById(10);\r"
        deleteStr += "        if(empty($record)){\r            echo \"搜索无数据\";\r            exit;\r        }\r"
        deleteStr += "        $record = $record[0];\r"
        deleteStr += "        $res = $record->deleteOne();\r"
        deleteStr += "        var_dump($res);\r"
        deleteStr += "        #./vendor/bin/phpunit --filter testdeleteOne ./test/" + modelTestFileName
        deleteStr += "\r    }\r"  # 删除的方法
        testFileFd.write(deleteStr)
        unitTestCommandList.append("./vendor/bin/phpunit --filter testdeleteOne ./test/" + modelTestFileName)




        #select * from xxx=xxx
        for column in tableColumnList:
            if "id" in column["name"].lower() or "status" in column["name"].lower():#如果字段中带有Id、ID、id等字眼的，一般都是跟外表关联的都需要单独生成获取的方法
                if hasIsDeleted:
                    if not "status" in column["name"].lower():
                        tmpGetStr = zhushiStr.format(params="", usage="根据" + column["name"] + "获取一条不在回收站内的数据") + "    public static function getOneUndeletedBy" + column["name"].capitalize() + "($" + column["name"] + ")"
                        if phpVersion == 7:
                            tmpGetStr += " : array"
                        tmpGetStr += "\r    {\r"
                        tmpGetStr += "        $sql = \" SELECT * from \".self::$table.\" WHERE `isDeleted` = 0 AND " + column["name"] + " = ?  order by "
                        if hasSort:
                            tmpGetStr += "sort"
                        else:
                            tmpGetStr += "id"
                        tmpGetStr += " desc limit 0,1\";\r"
                        tmpGetStr += "        $sqlParam = array($" + column["name"] + ");\r"
                        tmpGetStr += "        return self::query($sql, $sqlParam, true);"
                        tmpGetStr += "\r    }\r"
                        fd.write(tmpGetStr)
                        # phpunit测试获取一条不在回收站内的数据
                        deleteStr = zhushiStr.format(params="", usage="获取一条不在回收站内的数据") + "    public function testgetOneUndeletedBy" + column["name"].capitalize() + "()\r    {\r"
                        deleteStr += "        $record = " + modelFileName[0:-4] + "::getOneUndeletedBy" + column["name"].capitalize() + "(10);\r"
                        deleteStr += "        var_dump($record);\r"
                        deleteStr += "        #./vendor/bin/phpunit --filter testgetOneUndeletedBy" + column["name"].capitalize() + " ./test/" + modelTestFileName
                        deleteStr += "\r    }\r"
                        testFileFd.write(deleteStr)
                        unitTestCommandList.append("./vendor/bin/phpunit --filter testgetOneUndeletedBy" + column["name"].capitalize() + " ./test/" + modelTestFileName)


                        tmpGetStr = zhushiStr.format(params="", usage="根据" + column["name"] + "获取一条在回收站内的数据") + "    public static function getOneDeletedBy" + column["name"].capitalize() + "($" + column["name"] + ")"
                        if phpVersion == 7:
                            tmpGetStr += " : array"
                        tmpGetStr += "\r    {\r"
                        tmpGetStr += "        $sql = \" SELECT * from \".self::$table.\" WHERE `isDeleted` = 1 AND " + column["name"] + " = ?  order by "
                        if hasSort:
                            tmpGetStr += "sort"
                        else:
                            tmpGetStr += "id"
                        tmpGetStr += " desc limit 0,1\";\r"
                        tmpGetStr += "        $sqlParam = array($" + column["name"] + ");\r"
                        tmpGetStr += "        return self::query($sql, $sqlParam, true);"
                        tmpGetStr += "\r    }\r"
                        fd.write(tmpGetStr)
                        # phpunit测试获取一条在回收站内的数据
                        deleteStr = zhushiStr.format(params="", usage="获取一条在回收站内的数据") + "    public function testgetOneDeletedBy" + column["name"].capitalize() + "()\r    {\r"
                        deleteStr += "        $record = " + modelFileName[0:-4] + "::getOneDeletedBy" + column["name"].capitalize() + "(10);\r"
                        deleteStr += "        var_dump($record);\r"
                        deleteStr += "        #./vendor/bin/phpunit --filter testgetOneDeletedBy" + column["name"].capitalize() + " ./test/" + modelTestFileName
                        deleteStr += "\r    }\r"
                        testFileFd.write(deleteStr)
                        unitTestCommandList.append("./vendor/bin/phpunit --filter testgetOneDeletedBy" + column["name"].capitalize() + " ./test/" + modelTestFileName)

                        #根据ID生成IN查询
                        tmpGetStr = zhushiStr.format(params="", usage="IN查询，根据" + column["name"] + "获取不在回收站内的数据") + "    public static function getAllUndeletedBy" + column["name"].capitalize() + "UseIn($" + column["name"] + "Values)"
                        if phpVersion == 7:
                            tmpGetStr += " : array"
                        tmpGetStr += "\r    {\r"
                        tmpGetStr += "        $sql = \" SELECT * from \".self::$table.\" WHERE `isDeleted` = 0 AND find_in_set (`" + column["name"] + "`,\'$"+column["name"]+"Values \') order by "
                        if hasSort:
                            tmpGetStr += "sort"
                        else:
                            tmpGetStr += "id"
                        tmpGetStr += " desc\";\r"
                        tmpGetStr += "        $sqlParam = array();\r"
                        tmpGetStr += "        return self::query($sql, $sqlParam, true);"
                        tmpGetStr += "\r    }\r"
                        fd.write(tmpGetStr)
                        # phpunit测试获取一条不在回收站内的数据
                        deleteStr = zhushiStr.format(params="",usage="IN查询， 获取一条不在回收站内的数据") + "    public function testgetAllUndeletedBy" + column["name"].capitalize() + "UseIn()\r    {\r"
                        deleteStr += "        $record = " + modelFileName[0:-4] + "::getAllUndeletedBy" + column["name"].capitalize() + "UseIn(\'1,2,3,4\');\r"
                        deleteStr += "        var_dump($record);\r"
                        deleteStr += "        #./vendor/bin/phpunit --filter testgetAllUndeletedBy" + column["name"].capitalize() + "UseIn ./test/" + modelTestFileName
                        deleteStr += "\r    }\r"
                        testFileFd.write(deleteStr)
                        unitTestCommandList.append("./vendor/bin/phpunit --filter testgetAllUndeletedBy" + column["name"].capitalize() + "UseIn ./test/" + modelTestFileName)


                    tmpGetStr = zhushiStr.format(params="", usage="根据"+column["name"]+"不带分页获取所有的不在回收站内的数据")+"    public static function getAllUndeletedBy"+column["name"].capitalize()+"WithOutLimit($"+column["name"]+")"
                    if phpVersion == 7:
                        tmpGetStr += " : array"
                    tmpGetStr += "\r    {\r"
                    tmpGetStr += "        $sql = \" SELECT * from \".self::$table.\" WHERE `isDeleted` = 0 AND "+column["name"]+" = ?  order by "
                    if hasSort:
                        tmpGetStr += "sort"
                    else:
                        tmpGetStr += "id"
                    tmpGetStr += " desc \";\r"
                    tmpGetStr += "        $sqlParam = array($"+column["name"]+");\r"
                    tmpGetStr += "        return self::query($sql, $sqlParam, true);"
                    tmpGetStr += "\r    }\r"
                    fd.write(tmpGetStr)
                    #phpunit测试不带分页获取所有的不在回收站内的数据
                    deleteStr = zhushiStr.format(params="", usage="测试不带分页获取所有的不在回收站内的数据") + "    public function testgetAllUndeletedBy"+column["name"].capitalize()+"WithOutLimit()\r    {\r"
                    deleteStr += "        $record = " + modelFileName[0:-4] + "::getAllUndeletedBy"+column["name"].capitalize()+"WithOutLimit(10);\r"
                    deleteStr += "        var_dump($record);\r"
                    deleteStr += "        #./vendor/bin/phpunit --filter testgetAllUndeletedBy"+column["name"].capitalize()+"WithOutLimit ./test/" + modelTestFileName
                    deleteStr += "\r    }\r"
                    testFileFd.write(deleteStr)
                    unitTestCommandList.append("./vendor/bin/phpunit --filter testgetAllUndeletedBy"+column["name"].capitalize()+"WithOutLimit ./test/" + modelTestFileName)


                    tmpGetStr = zhushiStr.format(params="", usage="根据" + column["name"] + "带分页获取所有的不在回收站内的数据") + "    public static function getAllUndeletedBy" + column["name"].capitalize() + "WithLimit($" + column["name"] + ", $limit)"
                    if phpVersion == 7:
                        tmpGetStr += " : array"
                    tmpGetStr += "\r    {\r"
                    tmpGetStr += "        assert($limit = \"\");\r"#这儿后期需要做调整，对$limit的判断有点问题
                    tmpGetStr += "        $sql = \" SELECT * from \".self::$table.\" WHERE `isDeleted` = 0 AND " + column["name"] + " = ? order by "
                    if hasSort:
                        tmpGetStr += "sort"
                    else:
                        tmpGetStr += "id"
                    tmpGetStr += " desc \".$limit;\r"
                    tmpGetStr += "        $sqlParam = array($" + column["name"] + ");\r"
                    tmpGetStr += "        return self::query($sql, $sqlParam, true);"
                    tmpGetStr += "\r    }\r"
                    fd.write(tmpGetStr)
                    #phpunit测试带分页获取所有的不在回收站内的数据
                    deleteStr = zhushiStr.format(params="", usage="测试带分页获取所有的不在回收站内的数据") + "    public function testgetAllUndeletedBy" + column["name"].capitalize() + "WithLimit()\r    {\r"
                    deleteStr += "        $record = " + modelFileName[0:-4] + "::getAllUndeletedBy" + column["name"].capitalize() + "WithLimit(10, \"limit 0,1\");\r"
                    deleteStr += "        var_dump($record);\r"
                    deleteStr += "        #./vendor/bin/phpunit --filter testgetAllUndeletedBy" + column["name"].capitalize() + "WithLimit ./test/" + modelTestFileName
                    deleteStr += "\r    }\r"
                    testFileFd.write(deleteStr)
                    unitTestCommandList.append("./vendor/bin/phpunit --filter testgetAllUndeletedBy" + column["name"].capitalize() + "WithLimit ./test/" + modelTestFileName)


                    tmpGetStr = zhushiStr.format(params="", usage="根据" + column["name"] + "不带分页获取所有的在回收站内的数据") + "    public static function getAllDeletedBy" + column["name"].capitalize() + "WithOutLimit($" + column["name"] + ")"
                    if phpVersion == 7:
                        tmpGetStr += " : array"
                    tmpGetStr += "\r    {\r"
                    tmpGetStr += "        $sql = \" SELECT * from \".self::$table.\" WHERE `isDeleted` = 1 AND " + column["name"] + " = ? order by "
                    if hasSort:
                        tmpGetStr += "sort"
                    else:
                        tmpGetStr += "id"
                    tmpGetStr += " desc \";\r"
                    tmpGetStr += "        $sqlParam = array($" + column["name"] + ");\r"
                    tmpGetStr += "        return self::query($sql, $sqlParam, true);"
                    tmpGetStr += "\r    }\r"
                    fd.write(tmpGetStr)
                    #phpunit测试不带分页获取所有的在回收站内的数据
                    deleteStr = zhushiStr.format(params="", usage="测试带分页获取所有的不在回收站内的数据") + "    public function testgetAllDeletedBy" + column["name"].capitalize() + "WithOutLimit()\r    {\r"
                    deleteStr += "        $record = " + modelFileName[0:-4] + "::getAllDeletedBy" + column["name"].capitalize() + "WithOutLimit(10);\r"
                    deleteStr += "        var_dump($record);\r"
                    deleteStr += "        #./vendor/bin/phpunit --filter testgetAllDeletedBy" + column["name"].capitalize() + "WithOutLimit ./test/" + modelTestFileName
                    deleteStr += "\r    }\r"
                    testFileFd.write(deleteStr)
                    unitTestCommandList.append("./vendor/bin/phpunit --filter testgetAllDeletedBy" + column["name"].capitalize() + "WithOutLimit ./test/" + modelTestFileName)

                    tmpGetStr = zhushiStr.format(params="", usage="根据" + column["name"] + "带分页获取所有的在回收站内的数据") + "    public static function getAllDeletedBy" + column["name"].capitalize() + "WithLimit($" + column["name"] + ", $limit)"
                    if phpVersion == 7:
                        tmpGetStr += " : array"
                    tmpGetStr += "\r    {\r"
                    tmpGetStr += "        assert($limit = \"\");\r"  # 这儿后期需要做调整，对$limit的判断有点问题
                    tmpGetStr += "        $sql = \" SELECT * from \".self::$table.\" WHERE `isDeleted` = 1 AND " + column["name"] + " = ? order by "
                    if hasSort:
                        tmpGetStr += "sort"
                    else:
                        tmpGetStr += "id"
                    tmpGetStr += " desc \".$limit;\r"
                    tmpGetStr += "        $sqlParam = array($" + column["name"] + ");\r"
                    tmpGetStr += "        return self::query($sql, $sqlParam, true);"
                    tmpGetStr += "\r    }\r"
                    fd.write(tmpGetStr)
                    #phpunit测试带分页获取所有的在回收站内的数据
                    deleteStr = zhushiStr.format(params="", usage="测试带分页获取所有的不在回收站内的数据") + "    public function testgetAllDeletedBy" + column["name"].capitalize() + "WithLimit()\r    {\r"
                    deleteStr += "        $record = " + modelFileName[0:-4] + "::getAllDeletedBy" + column["name"].capitalize() + "WithLimit(10, \"limit 0,1\");\r"
                    deleteStr += "        var_dump($record);\r"
                    deleteStr += "        #./vendor/bin/phpunit --filter testgetAllDeletedBy" + column["name"].capitalize() + "WithLimit ./test/" + modelTestFileName
                    deleteStr += "\r    }\r"
                    testFileFd.write(deleteStr)
                    unitTestCommandList.append("./vendor/bin/phpunit --filter testgetAllDeletedBy" + column["name"].capitalize() + "WithLimit ./test/" + modelTestFileName)



                else:
                    tmpGetStr = zhushiStr.format(params="", usage="根据" + column["name"] + "不带分页获取所有的数据") + "    public static function getAllBy" + column["name"].capitalize() + "WithOutLimit($" + column["name"] + ")"
                    if phpVersion == 7:
                        tmpGetStr += " : array"
                    tmpGetStr += "\r    {\r"
                    tmpGetStr += "        $sql = \" SELECT * from \".self::$table.\" WHERE " +column["name"] + " = ? order by "
                    if hasSort:
                        tmpGetStr += "sort"
                    else:
                        tmpGetStr += "id"
                    tmpGetStr += " desc \";\r"
                    tmpGetStr += "        $sqlParam = array($" + column["name"] + ");\r"
                    tmpGetStr += "        return self::query($sql, $sqlParam, true);"
                    tmpGetStr += "\r    }\r"
                    fd.write(tmpGetStr)
                    #phpunit测试不带分页获取所有的数据
                    deleteStr = zhushiStr.format(params="", usage="测试带分页获取所有的不在回收站内的数据") + "    public function testgetAllBy" + column["name"].capitalize() + "WithOutLimit()\r    {\r"
                    deleteStr += "        $record = " + modelFileName[0:-4] + "::getAllBy" + column["name"].capitalize() + "WithOutLimit(10);\r"
                    deleteStr += "        var_dump($record);\r"
                    deleteStr += "        #./vendor/bin/phpunit --filter testgetAllBy" + column["name"].capitalize() + "WithOutLimit ./test/" + modelTestFileName
                    deleteStr += "\r    }\r"
                    testFileFd.write(deleteStr)
                    unitTestCommandList.append("./vendor/bin/phpunit --filter testgetAllBy" + column["name"].capitalize() + "WithOutLimit ./test/" + modelTestFileName)

                    tmpGetStr = zhushiStr.format(params="", usage="根据" + column["name"] + "带分页获取所有的数据") + "    public static function getAllBy" + column["name"].capitalize() + "WithLimit($" + column["name"] + ", $limit)"
                    if phpVersion == 7:
                        tmpGetStr += " : array"
                    tmpGetStr += "\r    {\r"
                    tmpGetStr += "        assert($limit = \"\");\r"  # 这儿后期需要做调整，对$limit的判断有点问题
                    tmpGetStr += "        $sql = \" SELECT * from \".self::$table.\" WHERE " + column["name"] + " = ? order by "
                    if hasSort:
                        tmpGetStr += "sort"
                    else:
                        tmpGetStr += "id"
                    tmpGetStr += " desc \".$limit;\r"
                    tmpGetStr += "        $sqlParam = array($" + column["name"] + ");\r"
                    tmpGetStr += "        return self::query($sql, $sqlParam, true);"
                    tmpGetStr += "\r    }\r"
                    fd.write(tmpGetStr)
                    #phpunit测试带分页获取所有的数据
                    deleteStr = zhushiStr.format(params="", usage="测试带分页获取所有的不在回收站内的数据") + "    public function testgetAllBy" + column["name"].capitalize() + "WithLimit()\r    {\r"
                    deleteStr += "        $record = " + modelFileName[0:-4] + "::getAllBy" + column["name"].capitalize() + "WithLimit(10, \"limit 0,1\");\r"
                    deleteStr += "        var_dump($record);\r"
                    deleteStr += "        #./vendor/bin/phpunit --filter testgetAllBy" + column["name"].capitalize() + "WithLimit ./test/" + modelTestFileName
                    deleteStr += "\r    }\r"
                    testFileFd.write(deleteStr)
                    unitTestCommandList.append("./vendor/bin/phpunit --filter testgetAllBy" + column["name"].capitalize() + "WithLimit ./test/" + modelTestFileName)

                    if not "status" in column["name"].lower():
                        tmpGetStr = zhushiStr.format(params="", usage="根据" + column["name"] + "获取一条数据") + "    public static function getOneBy" + column["name"].capitalize() + "($" + column["name"] + ")"
                        if phpVersion == 7:
                            tmpGetStr += " : array"
                        tmpGetStr += "\r    {\r"
                        tmpGetStr += "        $sql = \" SELECT * from \".self::$table.\" WHERE " + column["name"] + " = ?  order by "
                        if hasSort:
                            tmpGetStr += "sort"
                        else:
                            tmpGetStr += "id"
                        tmpGetStr += " desc limit 0,1\";\r"
                        tmpGetStr += "        $sqlParam = array($" + column["name"] + ");\r"
                        tmpGetStr += "        return self::query($sql, $sqlParam, true);"
                        tmpGetStr += "\r    }\r"
                        fd.write(tmpGetStr)
                        # phpunit测试获取一条数据
                        deleteStr = zhushiStr.format(params="", usage="获取一条数据") + "    public function testgetOneBy" + column["name"].capitalize() + "()\r    {\r"
                        deleteStr += "        $record = " + modelFileName[0:-4] + "::getOneBy" + column["name"].capitalize() + "(10);\r"
                        deleteStr += "        var_dump($record);\r"
                        deleteStr += "        #./vendor/bin/phpunit --filter testgetOneBy" + column["name"].capitalize() + " ./test/" + modelTestFileName
                        deleteStr += "\r    }\r"
                        testFileFd.write(deleteStr)
                        unitTestCommandList.append("./vendor/bin/phpunit --filter testgetOneBy" + column["name"].capitalize() + " ./test/" + modelTestFileName)

                        # 根据ID生成IN查询
                        tmpGetStr = zhushiStr.format(params="", usage="IN查询，根据" + column["name"] + "获取不在回收站内的数据") + "    public static function getAllUndeletedBy" + column["name"].capitalize() + "UseIn($" + column["name"] + "Values)"
                        if phpVersion == 7:
                            tmpGetStr += " : array"
                        tmpGetStr += "\r    {\r"
                        tmpGetStr += "        $sql = \" SELECT * from \".self::$table.\" WHERE find_in_set(`" + column["name"] + "`, \'$" + column["name"] + "Values \') order by "
                        if hasSort:
                            tmpGetStr += "sort"
                        else:
                            tmpGetStr += "id"
                        tmpGetStr += " desc\";\r"
                        tmpGetStr += "        $sqlParam = array();\r"
                        tmpGetStr += "        return self::query($sql, $sqlParam, true);"
                        tmpGetStr += "\r    }\r"
                        fd.write(tmpGetStr)
                        # phpunit测试获取一条不在回收站内的数据
                        deleteStr = zhushiStr.format(params="",usage="获取一条不在回收站内的数据") + "    public function testgetOneUndeletedBy" + column["name"].capitalize() + "UseIn()\r    {\r"
                        deleteStr += "        $record = " + modelFileName[0:-4] + "::getOneUndeletedBy" + column["name"].capitalize() + "UseIn(\'1,2,3,4\');\r"
                        deleteStr += "        var_dump($record);\r"
                        deleteStr += "        #./vendor/bin/phpunit --filter testgetOneUndeletedBy" + column["name"].capitalize() + "UseIn ./test/" + modelTestFileName
                        deleteStr += "\r    }\r"
                        testFileFd.write(deleteStr)
                        unitTestCommandList.append("./vendor/bin/phpunit --filter testgetOneUndeletedBy" + column["name"].capitalize() + "UseIn ./test/" + modelTestFileName)
        #select * from xxxx where xxx=xxx and xxx=xxx and xxx=xxx
        if len(allColumnWithIdList) > 1:#如果含有`id`关键词的字段有2个及2个以上，那么需要组合生成他们的搜索方法
            startPoint = 2
            while True:
                if startPoint > len(allColumnWithIdList):
                    break
                tmpList = list(combinations(allColumnWithIdList, startPoint))
                for tmpCom in tmpList:#循环这个list
                    itemStr = ""
                    itemSqlStr = ""
                    itemThisStr = ""
                    itemAssertStr = ""
                    findNumStr = ""
                    for item in tmpCom:#因为list中每个元素中都是一个tuple，所以还要循环
                        itemThisStr += item.capitalize()+"And"
                        itemStr += "$"+item+", "
                        itemSqlStr += item+" = ? AND "
                        itemAssertStr += "        assert(is_numeric($"+item+") && $"+item+" > 0);\r"
                        findNumStr += "10, "
                    if hasIsDeleted:

                        getAllStr = zhushiStr.format(params="", usage="根据" + itemStr[0:-2] + "获取一条没被删除的数据") + "    public static function getOneUndeletedBy" + itemThisStr[0:-3] + "(" + itemStr[0:-2] + ")"
                        if phpVersion == 7:
                            getAllStr += " : array"
                        getAllStr += "\r    {\r"
                        getAllStr += itemAssertStr
                        getAllStr += "        $sql = \"SELECT * FROM \".self::$table.\" WHERE `isDeleted` = 0 AND " + itemSqlStr[0:-5] + " order by "
                        if hasSort:
                            getAllStr += "sort"
                        else:
                            getAllStr += "id"
                        getAllStr += " desc limit 0,1\";\r"
                        getAllStr += "        $sqlParam = array(" + itemStr[0:-2] + ");\r"
                        getAllStr += "        return self::query($sql, $sqlParam, true);"
                        getAllStr += "\r    }\r"
                        fd.write(getAllStr)
                        # phpunit测试测试获取一条没被删除的数据
                        deleteStr = zhushiStr.format(params="", usage="测试获取一条没被删除的数据") + "    public function testgetOneUndeletedBy" + itemThisStr[0:-3] + "()\r    {\r"
                        deleteStr += "        $record = " + modelFileName[0:-4] + "::getOneUndeletedBy" + itemThisStr[0:-3] + "("+findNumStr[0:-2]+");\r"
                        deleteStr += "        var_dump($record);\r"
                        deleteStr += "        #./vendor/bin/phpunit --filter testgetOneUndeletedBy" + itemThisStr[0:-3] + " ./test/" + modelTestFileName
                        deleteStr += "\r    }\r"
                        testFileFd.write(deleteStr)
                        unitTestCommandList.append("./vendor/bin/phpunit --filter testgetOneUndeletedBy" + itemThisStr[0:-3] + " ./test/" + modelTestFileName)

                        getAllStr = zhushiStr.format(params="", usage="根据" + itemStr[0:-2] + "获取一条被删除的数据") + "    public static function getOneDeletedBy" + itemThisStr[0:-3] + "(" + itemStr[0:-2] + ")"
                        if phpVersion == 7:
                            getAllStr += " : array"
                        getAllStr += "\r    {\r"
                        getAllStr += itemAssertStr
                        getAllStr += "        $sql = \"SELECT * FROM \".self::$table.\" WHERE `isDeleted` = 1 AND " + itemSqlStr[0:-5] + " order by "
                        if hasSort:
                            getAllStr += "sort"
                        else:
                            getAllStr += "id"
                        getAllStr += " desc limit 0,1\";\r"
                        getAllStr += "        $sqlParam = array(" + itemStr[0:-2] + ");\r"
                        getAllStr += "        return self::query($sql, $sqlParam, true);"
                        getAllStr += "\r    }\r"
                        fd.write(getAllStr)
                        # phpunit测试测试获取一条被删除的数据
                        deleteStr = zhushiStr.format(params="", usage="测试获取一条被删除的数据") + "    public function testgetOneDeletedBy" + itemThisStr[0:-3] + "()\r    {\r"
                        deleteStr += "        $record = " + modelFileName[0:-4] + "::getOneDeletedBy" + itemThisStr[0:-3] + "("+findNumStr[0:-2]+");\r"
                        deleteStr += "        var_dump($record);\r"
                        deleteStr += "        #./vendor/bin/phpunit --filter testgetOneDeletedBy" + itemThisStr[0:-3] + " ./test/" + modelTestFileName
                        deleteStr += "\r    }\r"
                        testFileFd.write(deleteStr)
                        unitTestCommandList.append("./vendor/bin/phpunit --filter testgetOneDeletedBy" + itemThisStr[0:-3] + " ./test/" + modelTestFileName)

                        getAllStr = zhushiStr.format(params="", usage="根据"+itemStr[0:-2]+"不带limit的获取所有的没被删除的数据") + "    public static function getAllUndeletedBy"+itemThisStr[0:-3]+"WithOutLimit("+itemStr[0:-2]+")"
                        if phpVersion == 7:
                            getAllStr += " : array"
                        getAllStr += "\r    {\r"
                        getAllStr += itemAssertStr
                        getAllStr += "        $sql = \"SELECT * FROM \".self::$table.\" WHERE `isDeleted` = 0 AND "+itemSqlStr[0:-5]+" order by "
                        if hasSort:
                            getAllStr += "sort"
                        else:
                            getAllStr += "id"
                        getAllStr += " desc \";\r"
                        getAllStr += "        $sqlParam = array("+itemStr[0:-2]+");\r"
                        getAllStr += "        return self::query($sql, $sqlParam, true);"
                        getAllStr += "\r    }\r"  # 不带limit的获取所有的数据
                        fd.write(getAllStr)
                        #phpunit测试不带limit的获取所有的没被删除的数据
                        deleteStr = zhushiStr.format(params="", usage="测试不带limit的获取所有的没被删除的数据") + "    public function testgetAllUndeletedBy"+itemThisStr[0:-3]+"WithOutLimit()\r    {\r"
                        deleteStr += "        $record = " + modelFileName[0:-4] + "::getAllUndeletedBy"+itemThisStr[0:-3]+"WithOutLimit("+findNumStr[0:-2]+");\r"
                        deleteStr += "        var_dump($record);\r"
                        deleteStr += "        #./vendor/bin/phpunit --filter testgetAllUndeletedBy"+itemThisStr[0:-3]+"WithOutLimit ./test/" + modelTestFileName
                        deleteStr += "\r    }\r"
                        testFileFd.write(deleteStr)
                        unitTestCommandList.append("./vendor/bin/phpunit --filter testgetAllUndeletedBy"+itemThisStr[0:-3]+"WithOutLimit ./test/" + modelTestFileName)

                        getAllStr = zhushiStr.format(params="", usage="根据" + itemStr[0:-2] + "不带limit的获取所有的被删除的数据") + "    public static function getAllDeletedBy" + itemThisStr[0:-3] + "WithOutLimit(" + itemStr[0:-2] + ")"
                        if phpVersion == 7:
                            getAllStr += " : array"
                        getAllStr += "\r    {\r"
                        getAllStr += itemAssertStr
                        getAllStr += "        $sql = \"SELECT * FROM \".self::$table.\" WHERE `isDeleted` = 1 AND " + itemSqlStr[0:-5] + " order by "
                        if hasSort:
                            getAllStr += "sort"
                        else:
                            getAllStr += "id"
                        getAllStr += " desc \";\r"
                        getAllStr += "        $sqlParam = array(" + itemStr[0:-2] + ");\r"
                        getAllStr += "        return self::query($sql, $sqlParam, true);"
                        getAllStr += "\r    }\r"  # 不带limit的获取所有的数据
                        fd.write(getAllStr)
                        #phpunit测试不带limit的获取所有的被删除的数据
                        deleteStr = zhushiStr.format(params="", usage="测试不带limit的获取所有的没被删除的数据") + "    public function testgetAllDeletedBy" + itemThisStr[0:-3] + "WithOutLimit()\r    {\r"
                        deleteStr += "        $record = " + modelFileName[0:-4] + "::getAllDeletedBy" + itemThisStr[0:-3] + "WithOutLimit("+findNumStr[0:-2]+");\r"
                        deleteStr += "        var_dump($record);\r"
                        deleteStr += "        #./vendor/bin/phpunit --filter testgetAllDeletedBy" + itemThisStr[0:-3] + "WithOutLimit ./test/" + modelTestFileName
                        deleteStr += "\r    }\r"
                        testFileFd.write(deleteStr)
                        unitTestCommandList.append("./vendor/bin/phpunit --filter testgetAllDeletedBy" + itemThisStr[0:-3] + "WithOutLimit ./test/" + modelTestFileName)

                        getAllStr = zhushiStr.format(params="", usage="根据" + itemStr[0:-2] + "带limit的获取所有的没被删除的数据") + "    public static function getAllUndeletedBy" + itemThisStr[0:-3] + "WithLimit(" + itemStr[0:-2] + ", $limit)"
                        if phpVersion == 7:
                            getAllStr += " : array"
                        getAllStr += "\r    {\r"
                        getAllStr += itemAssertStr
                        getAllStr += "        assert($limit != \"\");\r"
                        getAllStr += "        $sql = \"SELECT * FROM \".self::$table.\" WHERE `isDeleted` = 0 AND " + itemSqlStr[0:-5] + " order by "
                        if hasSort:
                            getAllStr += "sort"
                        else:
                            getAllStr += "id"
                        getAllStr += " desc \".$limit;\r"
                        getAllStr += "        $sqlParam = array(" + itemStr[0:-2] + ");\r"
                        getAllStr += "        return self::query($sql, $sqlParam, true);"
                        getAllStr += "\r    }\r"  # 不带limit的获取所有的数据
                        fd.write(getAllStr)
                        #phpunit测试带limit的获取所有的没被删除的数据
                        deleteStr = zhushiStr.format(params="", usage="测试带limit的获取所有的没被删除的数据") + "    public function testgetAllUndeletedBy" + itemThisStr[0:-3] + "WithLimit()\r    {\r"
                        deleteStr += "        $record = " + modelFileName[0:-4] + "::getAllUndeletedBy" + itemThisStr[0:-3] + "WithLimit("+findNumStr[0:-2]+", \"limit 0,1\");\r"
                        deleteStr += "        var_dump($record);\r"
                        deleteStr += "        #./vendor/bin/phpunit --filter testgetAllUndeletedBy" + itemThisStr[0:-3] + "WithLimit ./test/" + modelTestFileName
                        deleteStr += "\r    }\r"
                        testFileFd.write(deleteStr)
                        unitTestCommandList.append("./vendor/bin/phpunit --filter testgetAllUndeletedBy" + itemThisStr[0:-3] + "WithLimit ./test/" + modelTestFileName)

                        getAllStr = zhushiStr.format(params="", usage="根据" + itemStr[0:-2] + "带limit的获取所有的被删除的数据") + "    public static function getAllDeletedBy" + itemThisStr[0:-3] + "WithLimit(" + itemStr[0:-2] + ", $limit)"
                        if phpVersion == 7:
                            getAllStr += " : array"
                        getAllStr += "\r    {\r"
                        getAllStr += itemAssertStr
                        getAllStr += "        assert($limit != \"\");\r"
                        getAllStr += "        $sql = \"SELECT * FROM \".self::$table.\" WHERE `isDeleted` = 1 AND " + itemSqlStr[0:-5] + " order by "
                        if hasSort:
                            getAllStr += "sort"
                        else:
                            getAllStr += "id"
                        getAllStr += " desc \".$limit;\r"
                        getAllStr += "        $sqlParam = array(" + itemStr[0:-2] + ");\r"
                        getAllStr += "        return self::query($sql, $sqlParam, true);"
                        getAllStr += "\r    }\r"  # 不带limit的获取所有的数据
                        fd.write(getAllStr)
                        #phpunit测试带limit的获取所有的被删除的数据
                        deleteStr = zhushiStr.format(params="", usage="测试带limit的获取所有的没被删除的数据") + "    public function testgetAllDeletedBy" + itemThisStr[0:-3] + "WithLimit()\r    {\r"
                        deleteStr += "        $record = " + modelFileName[0:-4] + "::getAllDeletedBy" + itemThisStr[0:-3] + "WithLimit("+findNumStr[0:-2]+", \"limit 0,1\");\r"
                        deleteStr += "        var_dump($record);\r"
                        deleteStr += "        #./vendor/bin/phpunit --filter testgetAllDeletedBy" + itemThisStr[0:-3] + "WithLimit ./test/" + modelTestFileName
                        deleteStr += "\r    }\r"
                        testFileFd.write(deleteStr)
                        unitTestCommandList.append("./vendor/bin/phpunit --filter testgetAllDeletedBy" + itemThisStr[0:-3] + "WithLimit ./test/" + modelTestFileName)

                    else:
                        getAllStr = zhushiStr.format(params="", usage="根据" + itemStr[0:-2] + "不带limit的获取所有的没被删除的数据") + "    public static function getAllBy" + itemThisStr[0:-3] + "WithOutLimit(" + itemStr[0:-2] + ")"
                        if phpVersion == 7:
                            getAllStr += " : array"
                        getAllStr += "\r    {\r"
                        getAllStr += itemAssertStr
                        getAllStr += "        $sql = \"SELECT * FROM \".self::$table.\" WHERE " + itemSqlStr[0:-5] + " order by "
                        if hasSort:
                            getAllStr += "sort"
                        else:
                            getAllStr += "id"
                        getAllStr += " desc \";\r"
                        getAllStr += "        $sqlParam = array(" + itemStr[0:-2] + ");\r"
                        getAllStr += "        return self::query($sql, $sqlParam, true);"
                        getAllStr += "\r    }\r"  # 不带limit的获取所有的数据
                        fd.write(getAllStr)
                        #phpunit测试不带limit的获取所有的数据
                        deleteStr = zhushiStr.format(params="", usage="测试带limit的获取所有的没被删除的数据") + "    public function testgetAllBy" + itemThisStr[0:-3] + "WithOutLimit()\r    {\r"
                        deleteStr += "        $record = " + modelFileName[0:-4] + "::getAllBy" + itemThisStr[0:-3] + "WithOutLimit("+findNumStr[0:-2]+");\r"
                        deleteStr += "        var_dump($record);\r"
                        deleteStr += "        #./vendor/bin/phpunit --filter testgetAllBy" + itemThisStr[0:-3] + "WithOutLimit ./test/" + modelTestFileName
                        deleteStr += "\r    }\r"
                        testFileFd.write(deleteStr)
                        unitTestCommandList.append("./vendor/bin/phpunit --filter testgetAllBy" + itemThisStr[0:-3] + "WithOutLimit ./test/" + modelTestFileName)

                        getAllStr = zhushiStr.format(params="", usage="根据" + itemStr[0:-2] + "不带limit的获取所有的被删除的数据") + "    public static function getAllBy" + itemThisStr[0:-3] + "WithLimit(" + itemStr[0:-2] + ", $limit)"
                        if phpVersion == 7:
                            getAllStr += " : array"
                        getAllStr += "\r    {\r"
                        getAllStr += itemAssertStr
                        getAllStr += "        assert($limit != \"\");\r"
                        getAllStr += "        $sql = \"SELECT * FROM \".self::$table.\" WHERE " + itemSqlStr[0:-5] + " order by "
                        if hasSort:
                            getAllStr += "sort"
                        else:
                            getAllStr += "id"
                        getAllStr += " desc \".$limit;\r"
                        getAllStr += "        $sqlParam = array(" + itemStr[0:-2] + ");\r"
                        getAllStr += "        return self::query($sql, $sqlParam, true);"
                        getAllStr += "\r    }\r"  # 不带limit的获取所有的数据
                        fd.write(getAllStr)
                        #phpunit测试带limit的获取所有的数据
                        deleteStr = zhushiStr.format(params="", usage="测试带limit的获取所有的没被删除的数据") + "    public function testgetAllBy" + itemThisStr[0:-3] + "WithLimit()\r    {\r"
                        deleteStr += "        $record = " + modelFileName[0:-4] + "::getAllBy" + itemThisStr[0:-3] + "WithLimit("+findNumStr[0:-2]+", \"limit 0,1\");\r"
                        deleteStr += "        var_dump($record);\r"
                        deleteStr += "        #./vendor/bin/phpunit --filter testgetAllBy" + itemThisStr[0:-3] + "WithLimit ./test/" + modelTestFileName
                        deleteStr += "\r    }\r"
                        testFileFd.write(deleteStr)
                        unitTestCommandList.append("./vendor/bin/phpunit --filter testgetAllBy" + itemThisStr[0:-3] + "WithLimit ./test/" + modelTestFileName)

                        getAllStr = zhushiStr.format(params="", usage="根据" + itemStr[0:-2] + "获取一条数据") + "    public static function getOneBy" + itemThisStr[0:-3] + "(" + itemStr[0:-2] + ")"
                        if phpVersion == 7:
                            getAllStr += " : array"
                        getAllStr += "\r    {\r"
                        getAllStr += itemAssertStr
                        getAllStr += "        $sql = \"SELECT * FROM \".self::$table.\" WHERE " + itemSqlStr[0:-5] + " order by "
                        if hasSort:
                            getAllStr += "sort"
                        else:
                            getAllStr += "id"
                        getAllStr += " desc limit 0,1\";\r"
                        getAllStr += "        $sqlParam = array(" + itemStr[0:-2] + ");\r"
                        getAllStr += "        return self::query($sql, $sqlParam, true);"
                        getAllStr += "\r    }\r"  # 不带limit的获取所有的数据
                        fd.write(getAllStr)
                        # phpunit测试不带limit的获取所有的数据
                        deleteStr = zhushiStr.format(params="", usage="测试获取一条数据") + "    public function testgetOneBy" + itemThisStr[0:-3] + "()\r    {\r"
                        deleteStr += "        $record = " + modelFileName[0:-4] + "::getOneBy" + itemThisStr[0:-3] + "("+findNumStr[0:-2]+");\r"
                        deleteStr += "        var_dump($record);\r"
                        deleteStr += "        #./vendor/bin/phpunit --filter testgetOneBy" + itemThisStr[0:-3] + " ./test/" + modelTestFileName
                        deleteStr += "\r    }\r"
                        testFileFd.write(deleteStr)
                        unitTestCommandList.append("./vendor/bin/phpunit --filter testgetOneBy" + itemThisStr[0:-3] + " ./test/" + modelTestFileName)

                startPoint += 1
        # select * from xxx where id = xx
        if hasIsDeleted:
            getOneStr = zhushiStr.format(params="", usage="根据ID获取一条记录") + "    public static function getOneById($id)"
            if phpVersion == 7:
                getOneStr += " : array"
            getOneStr += "\r    {\r"
            getOneStr += "        assert(is_numeric($id) && $id > 0);\r"
            getOneStr += "        $sql = \"SELECT * FROM \".self::$table.\" WHERE `id` = ? AND isDeleted = 0\";\r"
            getOneStr += "        $sqlParam = array($id);\r"
            getOneStr += "        return self::query($sql, $sqlParam, true);"
            getOneStr += "\r    }\r"  # 根据ID获取一条的记录
            fd.write(getOneStr)
            #phpunit测试根据ID获取一条记录
            deleteStr = zhushiStr.format(params="",usage="根据ID获取一条记录") + "    public function testgetOneById()\r    {\r"
            deleteStr += "        $record = " + modelFileName[0:-4] + "::getOneById(1);\r"
            deleteStr += "        var_dump($record);\r"
            deleteStr += "        #./vendor/bin/phpunit --filter testgetOneById ./test/" + modelTestFileName
            deleteStr += "\r    }\r"
            testFileFd.write(deleteStr)
            unitTestCommandList.append("./vendor/bin/phpunit --filter testgetOneById ./test/" + modelTestFileName)

            #IN查询
            getOneStr = zhushiStr.format(params="", usage="IN查询，根据ID获取所有的记录") + "    public static function getAllUndeletedByIdUseIn($ids)"
            if phpVersion == 7:
                getOneStr += " : array"
            getOneStr += "\r    {\r"
            getOneStr += "        assert($ids != \"\");\r"
            getOneStr += "        $sql = \"SELECT * FROM \".self::$table.\" WHERE find_in_set(`id`, \'$ids\') AND isDeleted = 0\";\r"
            getOneStr += "        $sqlParam = array();\r"
            getOneStr += "        return self::query($sql, $sqlParam, true);"
            getOneStr += "\r    }\r"  # 根据ID获取一条的记录
            fd.write(getOneStr)
            # phpunit测试根据ID获取一条记录
            deleteStr = zhushiStr.format(params="", usage="IN查询，根据ID获取一条记录") + "    public function testgetAllUndeletedByIdUseIn()\r    {\r"
            deleteStr += "        $record = " + modelFileName[0:-4] + "::getAllUndeletedByIdUseIn(\'1,2,3,4\');\r"
            deleteStr += "        var_dump($record);\r"
            deleteStr += "        #./vendor/bin/phpunit --filter testgetAllUndeletedByIdUseIn ./test/" + modelTestFileName
            deleteStr += "\r    }\r"
            testFileFd.write(deleteStr)
            unitTestCommandList.append("./vendor/bin/phpunit --filter testgetAllUndeletedByIdUseIn ./test/" + modelTestFileName)

        else:
            getOneStr = zhushiStr.format(params="", usage="根据ID获取一条记录") + "    public static function getOneById($id)"
            if phpVersion == 7:
                getOneStr += " : array"
            getOneStr += "\r    {\r"
            getOneStr += "        assert(is_numeric($id) && $id > 0);\r"
            getOneStr += "        $sql = \"SELECT * FROM \".self::$table.\" WHERE `id` = ?\";\r"
            getOneStr += "        $sqlParam = array($id);\r"
            getOneStr += "        return self::query($sql, $sqlParam, true);"
            getOneStr +="\r    }\r"#根据ID获取一条的记录
            fd.write(getOneStr)
            # phpunit测试根据ID获取一条记录
            deleteStr = zhushiStr.format(params="",usage="根据ID获取一条记录") + "    public function testgetOneById()\r    {\r"
            deleteStr += "        $record = " + modelFileName[0:-4] + "::getOneById(1);\r"
            deleteStr += "        var_dump($record);\r"
            deleteStr += "        #./vendor/bin/phpunit --filter testgetOneById ./test/" + modelTestFileName
            deleteStr += "\r    }\r"
            testFileFd.write(deleteStr)
            unitTestCommandList.append("./vendor/bin/phpunit --filter testgetOneById ./test/" + modelTestFileName)

            # IN查询
            getOneStr = zhushiStr.format(params="", usage="IN查询，根据ID获取所有的记录") + "    public static function getAllByIdUseIn($ids)"
            if phpVersion == 7:
                getOneStr += " : array"
            getOneStr += "\r    {\r"
            getOneStr += "        assert($id != \"\");\r"
            getOneStr += "        $sql = \"SELECT * FROM \".self::$table.\" WHERE find_in_set(`id`, \'$ids\')\";\r"
            getOneStr += "        $sqlParam = array();\r"
            getOneStr += "        return self::query($sql, $sqlParam, true);"
            getOneStr += "\r    }\r"  # 根据ID获取一条的记录
            fd.write(getOneStr)
            # phpunit测试根据ID获取一条记录
            deleteStr = zhushiStr.format(params="", usage="IN查询，根据ID获取一条记录") + "    public function testgetAllByIdUseIn()\r    {\r"
            deleteStr += "        $record = " + modelFileName[0:-4] + "::getAllByIdUseIn(\'1,2,3,4\');\r"
            deleteStr += "        var_dump($record);\r"
            deleteStr += "        #./vendor/bin/phpunit --filter testgetAllByIdUseIn ./test/" + modelTestFileName
            deleteStr += "\r    }\r"
            testFileFd.write(deleteStr)
            unitTestCommandList.append("./vendor/bin/phpunit --filter testgetAllByIdUseIn ./test/" + modelTestFileName)

        #select * from xxx where id = xx for update
        if hasIsDeleted:
            getOneStr = zhushiStr.format(params="", usage="根据ID获取一条没被删除的记录") + "    public static function getOneByIdForUpdate($id)"
            if phpVersion == 7:
                getOneStr += " : array"
            getOneStr += "\r    {\r"
            getOneStr += "        assert(is_numeric($id) && $id > 0);\r"
            getOneStr += "        $sql = \"SELECT * FROM \".self::$table.\" WHERE `id` = ? AND isDeleted = 0 FOR UPDATE\";\r"
            getOneStr += "        $sqlParam = array($id);\r"
            getOneStr += "        return self::query($sql, $sqlParam, true);"
            getOneStr += "\r    }\r"  # 根据ID获取一条的记录
            fd.write(getOneStr)
            #phpunit测试根据ID获取一条记录
            deleteStr = zhushiStr.format(params="",usage="根据ID获取一条记录") + "    public function testgetOneByIdForUpdate()\r    {\r"
            deleteStr += "        $record = " + modelFileName[0:-4] + "::getOneByIdForUpdate(1);\r"
            deleteStr += "        var_dump($record);\r"
            deleteStr += "        #./vendor/bin/phpunit --filter testgetOneByIdForUpdate ./test/" + modelTestFileName
            deleteStr += "\r    }\r"
            testFileFd.write(deleteStr)
            unitTestCommandList.append("./vendor/bin/phpunit --filter testgetOneByIdForUpdate ./test/" + modelTestFileName)
        else:
            getOneStr = zhushiStr.format(params="", usage="根据ID获取一条记录") + "    public static function getOneByIdForUpdate($id)"
            if phpVersion == 7:
                getOneStr += " : array"
            getOneStr += "\r    {\r"
            getOneStr += "        assert(is_numeric($id) && $id > 0);\r"
            getOneStr += "        $sql = \"SELECT * FROM \".self::$table.\" WHERE `id` = ? FOR UPDATE\";\r"
            getOneStr += "        $sqlParam = array($id);\r"
            getOneStr += "        return self::query($sql, $sqlParam, true);"
            getOneStr +="\r    }\r"#根据ID获取一条的记录
            fd.write(getOneStr)
            #phpunit测试根据ID获取一条记录
            deleteStr = zhushiStr.format(params="",usage="根据ID获取一条记录") + "    public function testgetOneByIdForUpdate()\r    {\r"
            deleteStr += "        $record = " + modelFileName[0:-4] + "::getOneByIdForUpdate(1);\r"
            deleteStr += "        var_dump($record);\r"
            deleteStr += "        #./vendor/bin/phpunit --filter testgetOneByIdForUpdate ./test/" + modelTestFileName
            deleteStr += "\r    }\r"
            testFileFd.write(deleteStr)
            unitTestCommandList.append("./vendor/bin/phpunit --filter testgetOneByIdForUpdate ./test/" + modelTestFileName)

        #update xxx set isDeleted=1 where id = xx
        if hasIsDeleted:#软删除数据，即将数据放入回收站
            deleteUpdateStr = zhushiStr.format(params="", usage="删除记录的方法，进回收站，不是直接删数据库的记录") + "    public function deleteUpdateOne()"
            if phpVersion == 7:
                deleteUpdateStr += " : array"
            deleteUpdateStr += "\r    {\r"
            deleteUpdateStr += "        $sql = \"UPDATE \".self::$table.\" SET `isDeleted` = 1 WHERE `id` = ?\";\r"
            deleteUpdateStr += "        $sqlParam = array($this->id);\r"
            deleteUpdateStr += "        return self::query($sql, $sqlParam, false);"
            deleteUpdateStr += "\r    }\r"  # 删除的方法
            fd.write(deleteUpdateStr)
            #phpunit测试根据ID删除记录的方法，进回收站，不是直接删数据库的记录
            deleteStr = zhushiStr.format(params="",usage="删除记录的方法，进回收站，不是直接删数据库的记录") + "    public function testdeleteUpdateOne()\r    {\r"
            deleteStr += "        $record = " + modelFileName[0:-4] + "::getOneById(1);\r"
            deleteStr += "        if(empty($record)){\r            echo \"搜索无数据\";\r            exit;\r        }\r"
            deleteStr += "        $record = $record[0];\r"
            deleteStr += "        $res= $record->deleteUpdateOne();\r"
            deleteStr += "        var_dump($res);\r"
            deleteStr += "        #./vendor/bin/phpunit --filter testdeleteUpdateOne ./test/" + modelTestFileName
            deleteStr += "\r    }\r"
            testFileFd.write(deleteStr)
            unitTestCommandList.append("./vendor/bin/phpunit --filter testdeleteUpdateOne ./test/" + modelTestFileName)

        if hasSort:
            editSortStr = zhushiStr.format(params="", usage="编辑排序功能") + "    public function editOneSort()"
            if phpVersion == 7:
                editSortStr += " : array"
            editSortStr += "\r    {\r"
            editSortStr += "        $sql = \"UPDATE \".self::$table.\" SET `sort` = ? WHERE `id` = ?\";\r"
            editSortStr += "        $sqlParam = array($this->sort, $this->id);\r"
            editSortStr += "        return self::query($sql, $sqlParam, false);"
            editSortStr += "\r    }\r"  # 编辑排序的方法
            fd.write(editSortStr)
            # phpunit测试根据ID删除记录的方法，进回收站，不是直接删数据库的记录
            editSortStr = zhushiStr.format(params="",
                                         usage="编辑排序功能") + "    public function testeditOneSort()\r    {\r"
            editSortStr += "        $record = " + modelFileName[0:-4] + "::getOneById(1);\r"
            editSortStr += "        if(empty($record)){\r            echo \"搜索无数据\";\r            exit;\r        }\r"
            editSortStr += "        $record = $record[0];\r"
            editSortStr += "        $record->sort = 10;\r"
            editSortStr += "        $res= $record->editOneSort();\r"
            editSortStr += "        var_dump($res);\r"
            editSortStr += "        #./vendor/bin/phpunit --filter testeditOneSort ./test/" + modelTestFileName
            editSortStr += "\r    }\r"
            testFileFd.write(editSortStr)
            unitTestCommandList.append("./vendor/bin/phpunit --filter testeditOneSort ./test/" + modelTestFileName)

        #select * from xxx where isDeleted = 0
        if hasIsDeleted:
            getAllStr = zhushiStr.format(params="", usage="不带limit的获取所有的没被删除的数据")+"    public static function getAllUndeletedWithOutLimit()"
            if phpVersion == 7:
                getAllStr += " : array"
            getAllStr += "\r    {\r"
            getAllStr += "        $sql = \"SELECT * FROM \".self::$table.\" WHERE `isDeleted` = 0 order by "
            if hasSort:
                getAllStr += "sort"
            else:
                getAllStr += "id"
            getAllStr += " desc \";\r"
            getAllStr += "        $sqlParam = array();\r"
            getAllStr += "        return self::query($sql, $sqlParam, true);"
            getAllStr += "\r    }\r"#不带limit的获取所有的数据
            fd.write(getAllStr)
            #phpunit测试不带limit的获取所有的没被删除的数据
            deleteStr = zhushiStr.format(params="", usage="不带limit的获取所有的没被删除的数据") + "    public function testgetAllUndeletedWithOutLimit()\r    {\r"
            deleteStr += "        $record = " + modelFileName[0:-4] + "::getAllUndeletedWithOutLimit();\r"
            deleteStr += "        var_dump($record);\r"
            deleteStr += "        #./vendor/bin/phpunit --filter testgetAllUndeletedWithOutLimit ./test/" + modelTestFileName
            deleteStr += "\r    }\r"
            testFileFd.write(deleteStr)
            unitTestCommandList.append("./vendor/bin/phpunit --filter testgetAllUndeletedWithOutLimit ./test/" + modelTestFileName)

            getAllStr = zhushiStr.format(params="", usage="带limit的获取所有的没被删除的数据") + "    public static function getAllUndeletedWithLimit($limit)"
            if phpVersion == 7:
                getAllStr += " : array"
            getAllStr += "\r    {\r"
            getAllStr += "        assert($limit != \"\");\r"
            getAllStr += "        $sql = \"SELECT * FROM \".self::$table.\" WHERE `isDeleted` = 0  order by "
            if hasSort:
                getAllStr += "sort"
            else:
                getAllStr += "id"
            getAllStr += " desc \".$limit;\r"
            getAllStr += "        $sqlParam = array();\r"
            getAllStr += "        return self::query($sql, $sqlParam, true);"
            getAllStr += "\r    }\r"  # 不带limit的获取所有的数据
            fd.write(getAllStr)
            #phpunit测试带limit的获取所有的没被删除的数据
            deleteStr = zhushiStr.format(params="",usage="带limit的获取所有的没被删除的数据") + "    public function testgetAllUndeletedWithLimit()\r    {\r"
            deleteStr += "        $record = " + modelFileName[0:-4] + "::getAllUndeletedWithLimit(\"limit 0,1\");\r"
            deleteStr += "        var_dump($record);\r"
            deleteStr += "        #./vendor/bin/phpunit --filter testgetAllUndeletedWithLimit ./test/" + modelTestFileName
            deleteStr += "\r    }\r"
            testFileFd.write(deleteStr)
            unitTestCommandList.append("./vendor/bin/phpunit --filter testgetAllUndeletedWithLimit ./test/" + modelTestFileName)

            getAllStr = zhushiStr.format(params="", usage="不带limit的获取所有的被删除的数据") + "    public static function getAllDeletedWithOutLimit()"
            if phpVersion == 7:
                getAllStr += " : array"
            getAllStr += "\r    {\r"
            getAllStr += "        $sql = \"SELECT * FROM \".self::$table.\" WHERE `isDeleted` = 1 order by "
            if hasSort:
                getAllStr += "sort"
            else:
                getAllStr += "id"
            getAllStr += " desc \";\r"
            getAllStr += "        $sqlParam = array();\r"
            getAllStr += "        return self::query($sql, $sqlParam, true);"
            getAllStr += "\r    }\r"  # 不带limit的获取所有的数据
            fd.write(getAllStr)
            #phpunit测试不带limit的获取所有的被删除的数据
            deleteStr = zhushiStr.format(params="", usage="不带limit的获取所有的被删除的数据") + "    public function testgetAllDeletedWithOutLimit()\r    {\r"
            deleteStr += "        $record = " + modelFileName[0:-4] + "::getAllDeletedWithOutLimit();\r"
            deleteStr += "        var_dump($record);\r"
            deleteStr += "        #./vendor/bin/phpunit --filter testgetAllDeletedWithOutLimit ./test/" + modelTestFileName
            deleteStr += "\r    }\r"
            testFileFd.write(deleteStr)
            unitTestCommandList.append("./vendor/bin/phpunit --filter testgetAllDeletedWithOutLimit ./test/" + modelTestFileName)

            getAllStr = zhushiStr.format(params="", usage="带limit的获取所有的被删除的数据") + "    public static function getAllDeletedWithLimit($limit)"
            if phpVersion == 7:
                getAllStr += " : array"
            getAllStr += "\r    {\r"
            getAllStr += "        assert($limit != \"\");\r"
            getAllStr += "        $sql = \"SELECT * FROM \".self::$table.\" WHERE `isDeleted` = 1 order by "
            if hasSort:
                getAllStr += "sort"
            else:
                getAllStr += "id"
            getAllStr += " desc \".$limit;\r"
            getAllStr += "        $sqlParam = array();\r"
            getAllStr += "        return self::query($sql, $sqlParam, true);"
            getAllStr += "\r    }\r"  # 不带limit的获取所有的数据
            fd.write(getAllStr)
            #phpunit测试带limit的获取所有的被删除的数据
            deleteStr = zhushiStr.format(params="", usage="带limit的获取所有的被删除的数据") + "    public function testgetAllDeletedWithLimit()\r    {\r"
            deleteStr += "        $record = " + modelFileName[0:-4] + "::getAllDeletedWithLimit(\"limit 0,1\");\r"
            deleteStr += "        var_dump($record);\r"
            deleteStr += "        #./vendor/bin/phpunit --filter testgetAllDeletedWithLimit ./test/" + modelTestFileName
            deleteStr += "\r    }\r"
            testFileFd.write(deleteStr)
            unitTestCommandList.append("./vendor/bin/phpunit --filter testgetAllDeletedWithLimit ./test/" + modelTestFileName)
        else:
            getAllStr = zhushiStr.format(params="", usage="不带limit的获取所有的数据") + "    public static function getAllWithOutLimit()"
            if phpVersion == 7:
                getAllStr += " : array"
            getAllStr += "\r    {\r"
            getAllStr += "        $sql = \"SELECT * FROM \".self::$table.\" order by "
            if hasSort:
                getAllStr += "sort"
            else:
                getAllStr += "id"
            getAllStr += " desc \";\r"
            getAllStr += "        $sqlParam = array();\r"
            getAllStr += "        return self::query($sql, $sqlParam, true);"
            getAllStr += "\r    }\r"  # 不带limit的获取所有的数据
            fd.write(getAllStr)
            # phpunit测试不带limit的获取所有的数据
            deleteStr = zhushiStr.format(params="", usage="不带limit的获取所有的数据") + "    public function testgetAllWithOutLimit()\r    {\r"
            deleteStr += "        $record = " + modelFileName[0:-4] + "::getAllWithOutLimit();\r"
            deleteStr += "        var_dump($record);\r"
            deleteStr += "        #./vendor/bin/phpunit --filter testgetAllWithOutLimit ./test/" + modelTestFileName
            deleteStr += "\r    }\r"
            testFileFd.write(deleteStr)
            unitTestCommandList.append("./vendor/bin/phpunit --filter testgetAllWithOutLimit ./test/" + modelTestFileName)

            getAllWithLimitStr = zhushiStr.format(params="$limit",usage="带limit获取所有的记录") + "    public static function getAllWithLimit($limit)"
            if phpVersion == 7:
                getAllWithLimitStr += " : array"
            getAllWithLimitStr += "\r    {\r"
            getAllWithLimitStr += "        assert($limit != \"\");\r"
            getAllWithLimitStr += "        $sql = \"SELECT * from \".self::$table.\" order by "
            if hasSort:
                getAllWithLimitStr += "sort"
            else:
                getAllWithLimitStr += "id"
            getAllWithLimitStr += " desc \".$limit;\r"
            getAllWithLimitStr += "        $sqlParam = array();\r"
            getAllWithLimitStr += "        return self::query($sql, $sqlParam, true);"
            getAllWithLimitStr += "\r    }\r"  # 带limit的获取所有的数据
            fd.write(getAllWithLimitStr)
            # phpunit测试不带limit的获取所有的数据
            deleteStr = zhushiStr.format(params="",usage="不带limit的获取所有的数据") + "    public function testgetAllWithLimit()\r    {\r"
            deleteStr += "        $record = " + modelFileName[0:-4] + "::getAllWithLimit(\"limit 0,1\");\r"
            deleteStr += "        var_dump($record);\r"
            deleteStr += "        #./vendor/bin/phpunit --filter testgetAllWithLimit ./test/" + modelTestFileName
            deleteStr += "\r    }\r"
            testFileFd.write(deleteStr)
            unitTestCommandList.append("./vendor/bin/phpunit --filter testgetAllWithLimit ./test/" + modelTestFileName)

        #update xxx set = xxx where id = xxx
        editStr = zhushiStr.format(params="", usage="编辑一条记录")+"    public function editOne()"
        if phpVersion == 7:
            editStr += " : array"
        editStr += "\r    {\r"
        columnStr = ""
        columnArrayStr = ""
        for column in tableColumnList:
            if not column["name"] == "addTime":
                if column["type"] == "int":  # 如果字段是int类型：
                    editStr += "        assert(isset($this->" + column["name"] + ") && is_numeric($this->" + column["name"] + ") && $this->" + column["name"] + " >= 0);\r"
                else:
                    editStr += "        assert(isset($this->" + column["name"] + ") && $this->" + column["name"] + " != '' );\r"
                columnStr += "`" + column["name"] + "` = ?,"
                columnArrayStr += "            $this->" + column["name"] + ",\r"
        editStr += "        $sql = \"UPDATE \".self::$table.\" SET {columns} WHERE `id` = ? \";\r".format(columns=columnStr[0:-1])
        editStr += "        $sqlParam = array(\r{array}\r            $this->id\r        );\r".format(array=columnArrayStr[0:-1])
        editStr += "        return self::query($sql, $sqlParam, false);"
        editStr += "\r    }\r"#编辑一条记录
        fd.write(editStr)
        # phpunit测试编辑一条记录
        addStr = zhushiStr.format(params="", usage="测试编辑一条记录") + "    public function testeditOne()\r    {\r"
        addStr += "        $record = " + modelFileName[0:-4] + "::getOneById(1);\r"
        addStr += "        if(empty($record)){\r            echo \"搜索无数据\";\r            exit;\r        }\r"
        addStr += "        $record = $record[0];\r"
        for column in tableColumnList:
            if column["type"] == "int":  # 如果字段是int类型：
                addStr += "        $record->" + column["name"] + " = 10;\r"
            elif column["type"] == "datetime":
                addStr += "        $record->" + column["name"] + " = \"2019-02-12\";\r"
            else:
                addStr += "        $record->" + column["name"] + " = \"xxxx\";\r"
        addStr += "        $res = $record->editOne();\r"
        addStr += "        var_dump($res);\r"
        addStr += "        #./vendor/bin/phpunit --filter testeditOne ./test/" + modelTestFileName
        addStr += "\r    }\r"
        testFileFd.write(addStr)
        unitTestCommandList.append("./vendor/bin/phpunit --filter testeditOne ./test/" + modelTestFileName)

        fd.write("\r}")
        fd.close()

    testFileFd.write("}\r")
    testFileFd.close()
    if len(unitTestCommandList) > 0:
        for item in unitTestCommandList:#开始运行单元测试，将运行结果写入到testFinishedTest.txt文件中去
            unitRes = subprocess.getoutput(item)
            unitResList = unitRes.split("\n")
            res = unitResList[2]
            if res.startswith(".int"):#可以判断为新增、更新、删除操作
                num = re.search('\d+', res).group()
                if int(num) > 0:
                    unitTestFinishedFileFd.write(item+"\t\t\t True \r")
                else:
                    unitTestFinishedFileFd.write(item + "\t\t\t False \r")
            if res.strip() == "E":
                unitTestFinishedFileFd.write(item + "\t\t\t False \r")
            else:
                if res.strip() == "搜索无数据":
                    unitTestFinishedFileFd.write(item + "\t\t\t"+res.strip()+"\t\t\t True \r")
                else:
                    unitTestFinishedFileFd.write(item + "\t\t\t True \r")

            unitTestFinishedFileFd.flush()

    nowNum = nowNum + 1
    precent = int(nowNum / totalNum * 100)
    sys.stdout.flush()
    sys.stdout.write("正在生成文件" + "=" * precent + ">" + str(precent) + "%    [" + str(nowNum) + "/" + str(totalNum) + "]\r")
    time.sleep(0.2)

db.close()#关闭数据库连接


