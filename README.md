# twFm
Php rapid development framework built with composer \
用composer搭建起来的一个很简单的php开发框架，自己用这个框架开发了2年多，很多项目都是在此框架上开发，所以分享给大家。要求版本php >= 5.6。最好要本地安装了python环境且3.0以上,否则无法使用自动生成Model层的功能。\
部署到本地，根目录指到Public中。\
Config目录中：Config.php为配置文件，数据库连接配置在这儿。Route.php为访问路径文件，所有的路由配置在这儿。\
Controllers目录中：各个逻辑层的代码, 运行autoCreateModel.py文件，会自动生成所有的controller文件，内部代码有对应的增删改查操作\
Libs目录中：所有的工具类\
Log目录中：记录日志\
Models目录中：数据库Model层的代码，配置好数据库的连接，运行autoCreateModel.py文件，会自动生成所有的model文件。\
Public目录中：项目公共文件夹，存放css、图片、js等\
test目录中：针对Model层的代码，挨个生成的phpunit的单元测试代码，也由autoCreateModel.py自动生成，自动运行测试，测试的日志记录在，unitTestFinishedLog.txt中\
vendor目录中：composer安装的类库
Template目录为静态模板目录，对应controller中的目录，挨个对比，每个文件夹中有add.html、list.html、edit.html\
\

autoCreateModel.py的使用方法:
>>>>eg: autoCreateModel.py 
>>>>>>>>-t 表名 如果不指定-t 参数全表生成 \
                       -h 查看脚本的使用规则\
