# twFm
Php rapid development framework built with composer \r
部署到本地，根目录指到Public中。\r
Config目录中：Config.php为配置文件，数据库连接配置在这儿。Route.php为访问路径文件，所有的路由配置在这儿。
Controllers目录中：各个逻辑层的代码
Libs目录中：所有的工具类
Log目录中：记录日志
Models目录中：数据库Model层的代码，配置好数据库的连接，运行autoCreateModel.py文件，会自动生成所有的model文件。
Public目录中：项目公共文件夹，存放css、图片、js等
test目录中：针对Model层的代码，挨个生成的phpunit的单元测试代码，也由autoCreateModel.py自动生成，自动运行测试，测试的日志记录在，unitTestFinishedLog.txt中
vendor目录中：composer安装的类库
