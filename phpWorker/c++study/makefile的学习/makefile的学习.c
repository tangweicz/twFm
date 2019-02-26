
make -C 参数

g++ -MM xxx.hpp -std=c++11 -I../../include

目标一致，会合并目标依赖和命令，头文件的自动依赖就是用的这点

在makefile编译完后执行文件，遇到@rpath/xxxxx的错误的时候，需要到当前环境中，设置环境变量 export LD_LIBRARY_PATH=xxx/xxx/xx（mac为DYLD_LIBRARY_PATH）

makefile的执行，运行make命令，读取makefile的第一行命令，开启一个进程执行，执行完第一行命令。读取第二行命令，开启另一个进程，执行完第二行命令。。。。。。一直这样。那么如果我想其中两行用一个进程执行，那么在前面一行后面，加";\"这个就行

LD_LIBRARY_PATH与-L的关系，-L是在编译的时候指定动态库文件的位置 而可执行文件在执行的时候，它也不知道动态库文件在哪，所以需要用LD_LIBRARY_PATH去指定下！！！

make -j4

makefile有解析阶段和运行阶段。
    解析阶段是生成依赖关系树
    运行阶段是执行上面的依赖关系树




    #-----------------------------------c++ 先编译再链接成功案例（我们写的直播的代码）------------------------------------------------------#


    .PHONY:clean
    CC=g++
    BIN=main
    OBJS=main.o XAudioRecord.o XData.o XMedioEncode.o XRtmp.o XVedioRecord.o
    all:$(BIN) clean
    $(BIN):$(OBJS)
    	$(CC) $^ -o $@ -std=c++11 -L../../../lib -I../../../include -lQt5Core -lQt5Multimedia -lswresample -lavutil -lavcodec -lavformat -lswscale -lQt5Network -lswscale -lQt5Gui -lopencv_highgui -lopencv_core -lopencv_videoio -lavformat
    main.o:main.cpp
    	$(CC) $< -c -o $@ -std=c++11 -I../../../include
    XAudioRecord.o:XAudioRecord.cpp XAudioRecord.hpp
    	$(CC) $< -c -o $@ -std=c++11 -I../../../include
    XData.o:XData.cpp XData.hpp
    	$(CC) $< -c -o $@ -std=c++11 -I../../../include
    XMedioEncode.o:XMedioEncode.cpp XMedioEncode.hpp
    	$(CC) $< -c -o $@ -std=c++11 -I../../../include
    XRtmp.o:XRtmp.cpp XRtmp.hpp
    	$(CC) $< -c -o $@ -std=c++11 -I../../../include
    XVedioRecord.o:XVedioRecord.cpp XVedioRecord.hpp
    	$(CC) $< -c -o $@ -std=c++11 -I../../../include
    clean:
    	rm -f $(OBJS)
    install:
        cp xxx /usr/bin
        cp xxx /usr/local/lib




    对上面的简化版本：

    .PHONY:clean
    CC=g++
    BIN=main
    OBJS=$(wildcard *.cpp)
    COBJS=$(patsubst %.cpp, %.o, $(OBJS))
    $(BIN):$(COBJS)
    	$(CC) $^ -o $@ -std=c++11  -L../../../lib -I../../../include -lQt5Core -lQt5Multimedia -lswresample -lavutil -lavcodec -lavformat -lswscale -lQt5Network -lswscale -lQt5Gui -lopencv_highgui -lopencv_core -lopencv_videoio -lavformat
    %.o:%.cpp %.hpp
    	$(CC) $(filter %.cpp, $^) -c -o $@ -std=c++11 -I../../../include
    %.o:%.cpp
    	$(CC) $^ -c -o $@ -std=c++11 -I../../../include
    clean:
    	@echo $(COBJS)




    对上面自动添加头文件依赖，不至于我改了其中一个头文件，还不重新编译：

    .PHONY:all clean
    CC=g++
    BIN=main
    OBJS=$(wildcard *.cpp)
    COBJS=$(patsubst %.cpp, %.o, $(OBJS))
    DEPS=$(patsubst %.cpp, %.d, $(OBJS))
    all:$(DEPS) $(BIN) clean
    %.d:%.cpp
            g++ -MM $< -std=c++11 -I../../../include -I./ > $@
    ifneq ("$(wildcard *.d)", "")
    include $(DEPS)
    endif
    $(BIN):$(COBJS)
            $(CC) $^ -o $@ -std=c++11  -L../../../lib -I../../../include -lQt5Core -lQt5Multimedia -lswresample -lavutil -lavcodec -lavformat -lswscale -lQt5Network -lswscale -lQt5Gui -lopencv_highgui -lopencv_core -lopencv_videoio -lavformat
    %.o:%.cpp
            $(CC) $(filter %.cpp, $^) -c -o $@ -std=c++11 -I../../../include
    clean:
            rm -f $(COBJS) $(DEPS)






    使用目录管理源文件(即不同的.cpp文件放到不同的文件夹中)，makefile相同的部分，放到common.mk中，放在根目录，makefile不同的部分写到自己文件夹中的makefile中，然后根目录中再件一个makefile去控制整个的执行。

        那么：
            common.mk中如下：

                .PHONY:all clean
                CBJS=$(wildcard *.cpp)
                DBJS=$(patsubst %.cpp, %.d, $(CBJS))
                OBJS=$(patsubst %.cpp, %.o, $(CBJS))
                all:$(DBJS) $(OBJS) $(TARGET)
                %.d:%.cpp
                        g++ -MM $< -std=c++11 -I../include -I../../../../include > $@
                ifneq ("$(wildcard *.d)", "")
                include $(DBJS)
                endif
                %.o:%.cpp
                        g++ $< -c -o $@ -std=c++11 -I../include -I../../../../include
                $(TARGET):$(OBJS)
                        g++ $+ -o $@ ../XAudioRecord/XAudioRecord.o ../XData/XData.o ../XMedioEncode/XMedioEncode.o ../XRtmp/XRtmp.o ../XVedioRecord/XVedioRecord.o -std=c++11 -I../include -I../../../../include -L../../../../lib -lQt5Core -lQt5Multimedia -lswresample -lavutil -lavcodec -lavformat -lswscale -lQt5Network -lswscale -lQt5Gui -lopencv_highgui -lopencv_core -lopencv_videoio -lavformat


            具体某个.cpp文件中的makefile内容如下：
                TARGET=
                include ../common.mk


            要生成可执行文件的文件夹下的makefile的内容如下：
                TARGET=main
                include ../common.mk


            根目录中的makefile的内容：

                .PHONY:all clean
                all:
                        make -C XAudioRecord
                        make -C XData
                        make -C XMedioEncode
                        make -C XRtmp
                        make -C XVedioRecord
                        make -C app
                clean:
                        rm -f XAudioRecord/XAudioRecord.o XAudioRecord/XAudioRecord.d
                        rm -f XData/XData.o XData/XData.d
                        rm -f XMedioEncode/XMedioEncode.o XMedioEncode/XMedioEncode.d
                        rm -f XRtmp/XRtmp.o XRtmp/XRtmp.d
                        rm -f XVedioRecord/XVedioRecord.o XVedioRecord/XVedioRecord.d
                        rm -f app/main.o app/main.d app/main


    使用目录管理生成的.o文件（因为上面使用目录管理源文件，生成的.o文件放在各自的文件夹中，后面要使用很麻烦，所以 这儿就是为了以后方便使用.o文件）

        只要修改.o文件生成、使用、头文件依赖相关的地址！！
            修改为：
                .PHONY:all clean
                CBJS=$(wildcard *.cpp)                                  #获取当前路劲下的所有.cpp文件
                DBJS=$(patsubst %.cpp, %.d, $(CBJS))                    #有多少个.cpp文件就需要生成多少个.d文件
                OBJS=$(patsubst %.cpp, %.o, $(CBJS))                    #有多少个.cpp文件就需要生成多少个.o文件
                LINK_DIR=$(join ../, linkdir)                           #将.o文件存放到哪里
                SED_LINK_DIR=$(join ..\/, linkdir)
                OBJS:=$(addprefix $(LINK_DIR)/, $(OBJS))                #标识.o文件存放到哪里
                LINK_OBJS=$(wildcard $(addprefix $(LINK_DIR)/, *.o))    #拿到所有生成.o文件，为生成可执行文件做准备
                LINK_OBJS+=$(OBJS)                                      #拿到目前的.o文件
                ifeq ("$(wildcard $(LINK_DIR))", "")
                $(shell mkdir $(LINK_DIR))
                endif
                all:$(DBJS) $(OBJS) $(BIN)
                %.d:%.cpp                                               #生成.d文件，并将关联的.o文件的地址修改好
                        g++ -MM $^ -std=c++11 -I../include -I../../../../include | sed 's/\(.*\)\.o/$(SED_LINK_DIR)\/\1.o/g' > $@

                ifneq ("$(wildcard $(DBJS))", "")                       #引入.d文件
                include $(DBJS)
                endif

                $(addprefix $(LINK_DIR)/, %.o):%.cpp                    #生成.o文件
                        g++ $< -c -o $@ -std=c++11 -I../include -I../../../../include

                $(BIN):$(LINK_OBJS)                                     #生成可执行文件
                        g++ $^ -o $@ -std=c++11 -I../include -I../../../../include -L../../../../lib -lQt5Core -lQt5Multimedia -lswresample -lavutil -lavcodec -lavformat -lswscale -lQt5Network -lswscale -lQt5Gui -lopencv_highgui -lopencv_core -lopencv_videoio -lavformat





    #-----------------------------------c++先编译再链接成功案例------------------------------------------------------#





    #-----------------------------------c++ 使用动态库成功案例（我们写的直播的代码）------------------------------------------------------#

    CC=g++

    all:libXData.dylib libXVedioRecord.dylib libXMedioEncode.dylib libXAudioRecord.dylib libXRtmp.dylib main
    libXData.dylib:XData.cpp
            $(CC) $^ -o $@ -std=c++11 -fpic -shared -I../../../include -L../../../lib -lavutil
    libXVedioRecord.dylib:XVedioRecord.cpp
            $(CC) $^ -o $@ -std=c++11 -fpic -shared -I../../../include -L../../../lib -L./ -lopencv_highgui -lopencv_core -lopencv_videoio -lQt5Core -lXData
    libXAudioRecord.dylib:XAudioRecord.cpp
            $(CC) $^ -o $@ -std=c++11 -fpic -shared -I../../../include -L../../../lib -L./ -lXData -lQt5Core -lQt5Multimedia
    libXMedioEncode.dylib:XMedioEncode.cpp
            $(CC) $^ -o $@ -std=c++11 -fpic -shared -I../../../include -L../../../lib -L./ -lswresample -lavcodec -lswscale
    libXRtmp.dylib:XRtmp.cpp
            $(CC) $^ -o $@ -std=c++11 -fpic -shared -I../../../include -L../../../lib -L./ -lavformat
    main:main.cpp
            $(CC) $^ -o $@ -std=c++11 -I../../../include -L../../../lib -L./ -lQt5Core -lXVedioRecord -lXAudioRecord -lXMedioEncode -lXRtmp -lavutil -lXData


    或者


    CC=g++
    all:libXData.dylib libXVedioRecord.dylib libXMedioEncode.dylib libXAudioRecord.dylib libXRtmp.dylib main
    libXData.dylib:XData.o
            $(CC) $^ -o $@ -std=c++11 -fpic -shared -L../../../lib -lavutil
    XData.o:XData.cpp XData.hpp
            $(CC) $< -c -o $@ -std=c++11 -fpic -I../../../include
    libXVedioRecord.dylib:XVedioRecord.o
            $(CC) $^ -o $@ -std=c++11 -shared -L../../../lib -L./ -lopencv_highgui -lopencv_core -lopencv_videoio -lQt5Core -lXData
    XVedioRecord.o:XVedioRecord.cpp XVedioRecord.hpp
            $(CC) $< -c -o $@ -std=c++11 -fpic -I../../../include
    libXAudioRecord.dylib:XAudioRecord.o
            $(CC) $^ -o $@ -std=c++11 -shared -L../../../lib -L./ -lXData -lQt5Core -lQt5Multimedia
    XAudioRecord.o:XAudioRecord.cpp XAudioRecord.hpp
            $(CC) $< -c -o $@ -std=c++11 -fpic -I../../../include
    libXMedioEncode.dylib:XMedioEncode.o
            $(CC) $^ -o $@ -std=c++11 -shared -L../../../lib -L./ -lswresample -lavcodec -lswscale
    XMedioEncode.o:XMedioEncode.cpp XMedioEncode.hpp
            $(CC) $< -c -o $@ -std=c++11 -fpic -I../../../include
    libXRtmp.dylib:XRtmp.o
            $(CC) $^ -o $@ -std=c++11 -shared -L../../../lib -L./ -lavformat
    XRtmp.o:XRtmp.cpp XRtmp.hpp
            $(CC) $< -c -o $@ -std=c++11 -fpic -I../../../include
    main:main.o
            $(CC) $^ -o $@ -std=c++11 -L../../../lib -L./ -lQt5Core -lXVedioRecord -lXAudioRecord -lXMedioEncode -lXRtmp -lavutil -lXData
    main.o:main.cpp
            $(CC) $^ -c -o $@ -std=c++11 -I../../../include


    #-----------------------------------c++ 使用动态库成功案例------------------------------------------------------#


    #-----------------------------------c++ 使用静态库成功案例（我们写的直播的代码）------------------------------------------------------#


    .PHONY:all
    CC=g++
    all:libXData.a libXAudioRecord.a libXMedioEncode.a libXRtmp.a libXVedioRecord.a main
    libXData.a:XData.o
            ar rcs libXData.a XData.o
    XData.o:XData.cpp XData.hpp
            $(CC) $< -c -o $@ -std=c++11 -I../../../include
    libXAudioRecord.a:XAudioRecord.o
            ar rcs libXAudioRecord.a XAudioRecord.o
    XAudioRecord.o:XAudioRecord.cpp XAudioRecord.hpp
            $(CC) $< -c -o $@ -std=c++11 -I../../../include
    libXMedioEncode.a:XMedioEncode.o
            ar rcs libXMedioEncode.a XMedioEncode.o
    XMedioEncode.o:XMedioEncode.cpp XMedioEncode.hpp
            $(CC) $< -c -o $@ -std=c++11 -I../../../include
    libXRtmp.a:XRtmp.o
            ar rcs libXRtmp.a XRtmp.o
    XRtmp.o:XRtmp.cpp XRtmp.hpp
            $(CC) $< -c -o $@ -std=c++11 -I../../../include
    libXVedioRecord.a:XVedioRecord.o
            ar rcs libXVedioRecord.a XVedioRecord.o
    XVedioRecord.o:XVedioRecord.cpp XVedioRecord.hpp
            $(CC) $< -c -o $@ -std=c++11 -I../../../include
    main:main.cpp
            $(CC) $< -o $@ -std=c++11 -I../../../include -L../../../lib -L./ -lQt5Core -lQt5Multimedia -lswresample -lavutil -lavcodec -lavformat -lswscale -lQt5Network -lswscale -lQt5Gui -lopencv_highgui -lopencv_core -lopencv_videoio -lavformat -lXData -lXAudioRecord -lXMedioEncode -lXRtmp -lXVedioRecord

    #-----------------------------------c++ 使用静态库成功案例（我们写的直播的代码）------------------------------------------------------#




1、Makefile是干什么用的？
    我们写好了makefile这个脚本，那么我们接下来就能在makefile的当前路径下，运行make命令，来将我们的源代码文件按照我们写的makefile的规则，编译成可执行文件。




2、helloworld:main.c
        g++ $+ -o $@ -std=c++11 -L../../../lib -I../../../include -l/xx/xx/xx/xx -l/xx/xx/xx/xx
   clean:(这儿是运行make clean的命令的时候，会执行)
        rm helloworld



3、然而，我们真正的编写makefile是这样写的！

    rtmp:main.o
            g++ $+ -o $@ -std=c++11 -I../../../include -L../../../lib -lopencv_videoio -lopencv_highgui -lopencv_core -lavutil -lavformat -lswscale -lavcodec
    main.o:main.cpp
            g++ $+ -c -o $@ -std=c++11
    clean:
            rm rtmp main.o



4、静态库和动态库

    动态库 .so      动态库的应用类似类似函数调用，执行代码的时候，动态库中的代码会加入到内存中，供调用 ！
        参数 必须加上 "-fpic -shared"
            什么是-fpic？ 用在编译阶段，只用位置无关代码！因为我们在生成共享库的代码中，肯定也会用到绝对路径和相对路径，如果不加-fpic的话，那么就需要修改共享库中的路径，而加上的话，就不需要修改路径。
            什么是-share？ 用在链接阶段。

            eg:
                g++ xxx.cpp -c -o xxx.o -fpic

                g++ xxx.o -o libxxx.so -shared

             或者：
                    g++ xxx.cpp -o libxxx.so -fpic -shared (实际上与上面分2次写是一致的)





    静态库 .a       静态库的应用，我们代码中用到了静态库，其实就是把静态库中的代码添加到了我们的代码中。调用的此时的越多，代码的体积越大。

            如何生成静态库？
                g++ xxx.cpp -c -o xxx.o

                ar rcs xxxx.a xxx.o



@的妙用，不会显示运行的是什么命令，而只显示运行的结果！！！！！

$@的意义，目标文件

$^的意义，所有的依赖文件

$<的意义，第一个依赖文件

%的意义，在目标和目标依赖上，匹配任意字符

make hello 是指定编译hello这个目标！！！

5、makefile的基本语法

        规（规则） ----->变（变量） ------>条（条件执行） ------>文（文本、文件名处理函数、文件包含）


        一、规则：（一个makefile中可以有多个规则，默认第一个为默认的目标规则，以它为目的。）

            规则的组成：

                eg:
                    目标:目标依赖
                            命令

                1、目标（必不可少，可以有多个目标。）

                2、目标依赖（一个规则中，可以没有目标依赖，也可以有多个目标依赖。）目标依赖中不需要写头文件！！

                3、命令（一个规则中，可以没有命令）

            什么是伪目标？

                1、伪目标，我们在执行make xxx的时候，会先看后面的依赖文件，如果后面的依赖文件没有改变（无依赖文件，默认为没有改变），那么命令就不会被执行。但是指定为伪目标之后，每次执行都不会去看依赖文件，而直接执行命令。

                2、all:xxx1 xxx2 指明要先后编译 xxx1、xxx2这些 all也是伪目标。

                eg:
                    .PHONY:clean all

            隐式规则：
                当我们的依赖文件不存在，那么我们会去找对应的写的规则，而我们没有写这个规则，那么就会调用隐式规则：

                     test:xxx.o//依赖xxx.o文件，但是xxx.o文件不存在，又没有xxx.o的编译命令，那么就会调用隐式规则。
                            xxxxx


        二、变量：(在命令中不要创建变量)  传（变量传递） ------>遍历（立即展开变量）---------->条（条件变量）-------->目（目标变量）

              cc = gcc//声明

              $(cc)//调用

              ①、变量的拼接：

                cc += "xxx"

              ②、变量如果没有被赋值，那么赋值，如果已经赋值，就不要再赋值

                cc ?= "xxx"

              ③、立即展开变量

                mm = kang

                g = ceshi

                cc := $(mm)$(g)//如果换成"="，不要":"这个试试

                g = haha

                test:
                        echo $(cc)

              ④、目标变量
                    目标名称：变量名称=值

                    eg：
                        N=1

                        kang.o:N=2
                        kang.o:
                            echo $(N)

                        te:
                            echo $(N)


              ⑤、变量传递

                    因为 我们的项目文档不可能只有一个文件夹，自然，也就不可能只有一个makefile，如何在根目录下运行make，然后会将子目录下的makefile也执行呢？

                    eg：
                        在根目录下有lib目录，其中根目录和lib目录中都有makefile文件

                        make -C lib（这个命令就是进入到lib中，运行lib中的makefile）涉及到变量传递，只要定义在根目录下的变量是export形容的就行。

        三、条件执行：

            ifeq ($(DEBUG), true)//这个ifeq后面必须加个空格，然后是()

            cc = gcc -g

            else

            cc = gcc

            endif

        四、目录函数：（参考 GNU_Makefile中文手册.pdf中的文本处理函数、文件名处理函数）

                1、$(subst FROM,TO,TEXT) 把字串“TEXT”中的“FROM”字符替换为“TO”

                2、$(patsubst PATTERN,REPLACEMENT,TEXT)

                        eg:$(patsubst %.c,%.o,x.c.c bar.c)

                3、$(strip STRINT)取消string中的空格

                        eg：
                            STR = a b c
                            LOSTR = $(strip $(STR))

                4、$(findstring FIND,IN) 在字串“IN”中查找“FIND”字串

                5、$(filter PATTERN ,TEXT)过滤掉字串“TEXT”中所有不符合模式“PATTERN”的单词,保留所有符合此模式的单词。可以使用多个模式。模式中一般需要包含模式字符“%”。存在多个模式时,模式表达式之间使用空格分割。
















在实际的开发过程中，当项目非常庞大的时候，我们不可能自己去写makefile，所以，我们下面要学的，就是，利用工具自动生成makefile。

    autotools中包含 Autoconf、Automake、Libtool 一般情况下，这三个工具已经安装在系统中了，如果没有安装，请各自安装下即可。 yum Autoconf、Automake、Libtool -y


    Autotools自动创建makefile流程：

        1、生成配置脚本configure

                ①：autoscan，生成configure.scan修改为configure.ac     //autoscan会扫描源码，并生成一些通用的宏调用、输入的声明以及输出的声明

                ②：修改、配置configure.ac  添加一个常量 AM_INIT_AUTOMAKE（告诉autoconf需要配置和调用automake）、行末AC_OUTPUT修改为AC_OUTPUT(Makefile)

                ③：aclocal，生成aclocal.m4,存放autoconf运行需要的宏    //configure.ac实际是依靠宏展开来得到configure的。因此，能否成功生成取决于，宏定义能否找到。autoconf会从自身安装路径下来寻找事先定义好了宏。然而对于像automake、libtool和gettext等第三方扩展宏，甚至是开发者自行编写的宏就一无所知了。于是，存在这个工具aclocal，将在configure.ac同一目录下生成aclocal.m4，在扫描configure.ac的过程中，将第三方扩展和开发者自己编写的宏定义复制进去。这样，autoconf在遇到不认识的宏时，就会从aclocal.m4中查找

                ④：autoheader 生成config.h.in                       //autoheader命令扫描configure.ac中的内容，并确定需要如何生成config.h.in。每当configure.ac有所变化，都可以通过再次执行autoheader更新config.h.in。在执行./configure的时候，会联合生成的config.h.in文件，一起生成config.h文件！！！！在configure.ac通过AC_CONFIG_HEADERS([config.h])告诉autoheader应当生成config.h.in的路径。

                ④：autoconf，将configure.ac生成configure             //如果修改了configure.ac，就只要执行autoreconf就会重新生成configure文件。

        2、生成makefile的通用规则文件makefile.in

                ①：手工生成、编写makefile.am文件

                    内容为：

                        AUTOMAKE_OPTIONS = foreign

                        SUBDIRS = a b //这里通过SUBDIRS声明了两个子目录，子目录的中的构建需要靠a/Makefile.am和b/Makefile.am来进行，这样多目录组织起来就方便多了。

                        bin_PROGRAMS    = st   //bin_PROGRAMS声明一个可执行文件目标

                        st_SOURCES      = xxxx //st_SOURCES指定这个目标所依赖的源代码文件

                        st_LDADD        = $(top_builddir)/src/a/liba.la $(top_builddir)/src/b/libb.la //st_LDADD声明了可执行文件在连接时，需要依赖的Libtool库文件。

                ②：automake(需要先有configure.ac、aclocal.m4、config.h.in),将makefile.am生成makefile.in  // --add-missing这条命令,这条命令应该会自动给我们创建缺少的文件

        3、通过configure生成makefile

            ①：./configure 行./configure会连接上面的makefile.in去执行，生成makefile文件。

            ②：make && make install




    automake.am的宏、变量：

        AUTOMAKE_OPTIONS    值一般为 foreign

        bin     生成可执行文件

        lib     生成库文件

        noinst  不安装                     //本目录下的代码编译成libhello.a库。不需要发布。如果需要发布，则写成bin_LIBRARIES

        _PROGRAMS 表示生成目标是可执行文件

        _LIBRARIES 表示目标为库文件

        _LTLIBRARIES 使用LIBTOOL生成库文件

        _HEADERS 头文件                    //会把=后面的，头文件拷贝到系统公共的头文件目录中去，供其他开发者调用

        _DATA 数据文件、配置文件等           //会把=后面的，数据文件拷贝到系统公共的数据文件目录中去，供其他开发者调用

        _SOURCES 目标依赖的源文件

        _LIBADD 生成库时需要链接的其它库

        _LDADD 需要的静态库文件

        _LDFLAGS    _LDFLAGS=-L/usr/lib -L/path/to/your/lib -shared -fPIC

        _CFLAGS     C语言头文件路径 -I....

        _CXXFLAGS     c++头文件路径 _CXXFLAGS=-I/usr/include -I/path/include

        _LIBTOOLFLAGS   Libtool编译选项

        bindir  $(prefix)/bin

        libdir  $(prefix)/lib

        datadir $(prefix)/share

        sysconfdir  $(prefix)/etc

        includedir  $(prefix)/include

        $(top_srcdir)   工程顶层目录

        $(top_builddir) 目标文件顶层目录

        SUBDIRS     递归处理子目录,运行automake的时候，递归生成Makefile.in文件，运行make的时候，递归到各个目录中，找到生成每个makefile.am中的目标

        EXTRA_DIST  打包文件或目录

        CXXFLAGS= "-std=c++11"

        AM_CXXFLAGS = -I../../../include -I./ -L./

        LIBS 需要的动态库文件 eg: LIBS = -lpthread -liconv


        使用案例：

                一、生成可执行文件

                    bin_PROGRAMS = foo

                    foo_SOURCES = xxx.cpp

                    foo_LDADD =

                    foo_LDFLAGS =

                    foo_LIBTOOLFLAGS =

                二、生成静态库文件

                    lib_LIBRARIES = foo.a

                    foo_a_SOURCES =

                    foo_a_LDADD =

                    foo_a_LIBADD =

                    foo_a_LDFLAGS =

                三、头文件

                    include_HEADERS=foo.h  //会把=后面的，头文件拷贝到系统公共的头文件目录中去，供其他开发者调用


                四、数据文件

                    data_DATA = data1 data2 //会把=后面的，数据文件拷贝到系统公共的数据文件目录中去，供其他开发者调用



    configure.ac的语法：
        一般情况下，在configure.ac中
            AC_INIT     一般以此开头          //AC_INIT(可执行文件名, 版本号, 联系人邮箱)

            AC_PROG_CC  检测c编译器，c项目需要设置此宏

            AC_CONFIG_SRCDIR    项目源文件   //AC_CONFIG_SRCDIR([xxxx.cpp])

            AC_CONFIG_HEADERS   配置头文件

            AC_CHECK_LIB        检测lib库中是否有指定函数

            AC_PROG_LIBTOOL     使用libtool生成动态共享库

            AC_PROG_RANLIB      启用静态库(要把某一部分编译成静态库)

            AC_OUTPUT           设置configure输出的Makefile文件，一般以此结尾，如果需要多个目录生成makefile，需要写过个 //AC_OUTPUT(Makefile, XData/Makefile, XAudioRecord/Makefile, ......)

            AM_INIT_AUTOMAKE    运行automake生成makefile.in必需的宏



    头文件的自动依赖，已经被集成到了automake中去了，我们不用管！！！



    项目下，每一个跟项目相关的目录，都必须要有一个makefile.am而生成makefile.in文件，这样在文件打包的时候才能被打上。

    针对不同的项目目录结构，如何使用autotools创建makefile
        1、flat目录结构（所有的文件都在同一目录下）
            第一步：autoscan 扫描得到 configure.scan，重命名为 configure.ac，修改其中内容，AC_INIT()这个参数, 特别注意，AC_OUTPUT需要修改为：AC_OUTPUT(Makefile)

            第二步：修改configure.ac，添加宏，AM_INIT_AUTOMAKE

            第三步：运行aclocal，生成aclocal.m4文件。运行autoheader，生成config.h.in文件

            第四步：运行autoconf，生成configure的脚本

            第五步：需要在各个目录编写Makefile.am文件

                    Makefile.am内容：

                                AUTOMAKE_OPTIONS=foreign

                                bin_PROGRAMS=main

                                main_SOURCES=main.cpp XAudioRecord.cpp XData.cpp XMedioEncode.cpp XRtmp.cpp XVedioRecord.cpp

                                AM_CXXFLAGS=-I../../../include -I./

                                CXXFLAGS =  -fPIC -std=c++11

                                main_LDFLAGS=-L../../../lib

                                LIBS=-lQt5Core -lQt5Multimedia -lswresample -lavutil -lavcodec -lavformat -lswscale -lQt5Network -lswscale -lQt5Gui -lopencv_highgui -lopencv_core -lopencv_videoio -lavformat

            第六步：到根目录中运行 automake --add-missing

            第七步：运行./configure



        2、shallow目录结构（主程序源文件放在顶层目录中，各个模块文件放在各个子目录中）(头文件存放的文件夹也需要建立makefile.am文件，这样在项目打包的时候才能被管理到)

            第一步：autoscan 扫描得到 configure.scan，重命名为 configure.ac，修改其中内容AC_INIT()，特别注意，AC_OUTPUT需要修改为：AC_OUTPUT(Makefile, XData/Makefile, XAudioRecord/Makefile, ......, include/Makefile)，每个子目录下都需要生成自己的Makefile

            第二步：修改configure.ac，添加宏，AM_INIT_AUTOMAKE,表示在运行configure的时候，会解析Makefile.am中的常量

            第三步：运行aclocal，生成aclocal.m4文件。

            第四步：运行autoheader

            第五步：运行autoconf，生成configure的脚本

            第六步：需要在各个目录编写Makefile.am文件

                    方法一：项目下，各个文件，生成静态库文件，参与编译生成可执行文件


                            修改configure.ac，添加AC_PROG_RANLIB 表示启用静态库

                            根目录的Makefile.am内容：

                                                    AUTOMAKE_OPTIONS = foreign

                                                    SUBDIRS = XAudioRecord XData XMedioEncode XRtmp XVedioRecord include

                                                    bin_PROGRAMS = main

                                                    main_SOURCES = main.cpp

                                                    main_CXXFLAGS   = -I../../../include -I./include

                                                    main_LDFLAGS=-L../../../lib -LXData -LXAudioRecord -LXMedioEncode -LXRtmp -LXVedioRecord

                                                    LIBS= -lQt5Core -lQt5Multimedia -lswresample -lavutil -lavcodec -lavformat -lswscale -lQt5Network -lswscale -lQt5Gui -lopencv_highgui -lopencv_core -lopencv_videoio -lavformat

                                                    main_LDADD = XData/libxdata.a XAudioRecord/libxaudiorecord.a XMedioEncode/libxmedioencode.a XRtmp/libxrtmp.a XVedioRecord/libxvediorecord.a

                                                    CXXFLAGS =  -fPIC -std=c++11


                            XData目录的Makefile.am内容：

                                                    AUTOMAKE_OPTIONS = foreign

                                                    noinst_LIBRARIES = libxdata.a

                                                    libxdata_a_SOURCES = XData.cpp

                                                    libxdata_a_CXXFLAGS        = -I../../../../include -I../include

                                                    libxdata_a_LDFLAGS=-L../../../../lib

                                                    LIBS = -lavutil

                                                    CXXFLAGS =  -fPIC -std=c++11

                            XAudioRecord目录的Makefile.am内容：

                                                    AUTOMAKE_OPTIONS = foreign

                                                    noinst_LIBRARIES = libxaudiorecord.a

                                                    libxaudiorecord_a_SOURCES = XAudioRecord.cpp

                                                    libxaudiorecord_a_CXXFLAGS        = -I../../../../include -I../include

                                                    libxaudiorecord_a_LDFLAGS=-L../../../../lib

                                                    LIBS = -lQt5Core -lQt5Multimedia -lswresample -lavutil -lavcodec -lavformat -lswscale -lQt5Network -lswscale -lQt5Gui -lopencv_highgui -lopencv_core -lopencv_videoio -lavformat

                                                    CXXFLAGS =  -fPIC -std=c++11


                            include目录的Makefile.am内容：

                                                    AUTOMAKE_OPTIONS = foreign

                                                    noinst_HEADERS = XData.hpp XMedioEncode.hpp .....(因为上面的静态库文件没有安装到系统中去，所以，头文件也不需要安装到系统中去)、如果要安装到系统中去，那么修改为：include_HEADERS 即为安装到系统的/usr/local/include中




                    方法二：项目下，在配置生成静态文件的基础下，各个文件，不生成静态库文件，只生成.o文件，然后结合所有的.o文件生成可执行文件

                            根目录的Makefile.am内容：

                                                    AUTOMAKE_OPTIONS = foreign

                                                    SUBDIRS = XData XAudioRecord ..... include(头文件这个文件夹，也需要写个Makefile.am，因为在后期打包项目的时候，也需要把include这个文件夹包含进去，如果不写，那么include在后期打包，默认不会包含。)

                                                    bin_PROGRAMS = main

                                                    main_SOURCES = main.cpp

                                                    main_CXXFLAGS   = -I../../../../include -I./include

                                                    main_LDFLAGS=-L../../../lib -L./XData -L./XAudioRecord

                                                    LIBS =  -lQt5Core -lQt5Multimedia -lswresample -lavutil -lavcodec -lavformat -lswscale -lQt5Network -lswscale -lQt5Gui -lopencv_highgui -lopencv_core -lopencv_videoio -lavformat

                                                    main_LDADD= XData/XData.o XAudioRecord/libxaudiorecord.a XData/libxdata.a

                                                    CXXFLAGS =  -fPIC -std=c++11


                            XData目录的Makefile.am内容：

                                                    AUTOMAKE_OPTIONS = foreign

                                                    noinst_LIBRARIES = libxdata.a

                                                    libxdata_a_SOURCES = XData.cpp

                                                    libxdata_a_CXXFLAGS        = -I../../../../include -I../include

                                                    CXXFLAGS =  -fPIC -std=c++11



                                                    运行automake -a生成Makefile.in之后，修改Makefile.in文件，找到生成libxdata.a的地方，注释掉生成libxdata.a的代码即可


                            XAudioRecord目录的Makefile.am内容：

                                                    AUTOMAKE_OPTIONS = foreign

                                                    noinst_LIBRARIES = libxaudiorecord.a

                                                    libxaudiorecord_a_SOURCES = XData.cpp

                                                    libxaudiorecord_a_CXXFLAGS        = -I../../../../include -I./include

                                                    libxaudiorecord_a_LDFLAGS=-L../../../lib

                                                    LIBS = -lQt5Core -lQt5Multimedia -lswresample -lavutil -lavcodec -lavformat -lswscale -lQt5Network -lswscale -lQt5Gui -lopencv_highgui -lopencv_core -lopencv_videoio -lavformat

                                                    CXXFLAGS =  -fPIC -std=c++11


                            include目录的Makefile.am内容：

                                                    AUTOMAKE_OPTIONS = foreign

                                                    noinst_HEADERS = XData.hpp XMedioEncode.hpp .....(因为上面的静态库文件没有安装到系统中去，所以，头文件也不需要安装到系统中去)、如果要安装到系统中去，那么修改为：include_HEADERS 即为安装到系统的/usr/local/include中


                    方法三：项目下，各个文件，生成动态库文件，参与编译生成可执行文件

                            为什么要用libtool生成动态库？

                                因为，libtool对库进行抽象，生成.la形式的文件，支持不同平台！！

                            使用libtool之前，先使用libtoolize，生成本地化的libtool。

                                            ①：命令行使用libtool：（一般不使用）

                                                        libtool --tag = CXX(c++)

                                                                        CC(c语言)

                                                                        GO(go语言)

                                                                        GCJ(java语言)

                                                                --mode = compile(将一个源文件编译为libtool目标文件)

                                                                         link(生成一个库或可执行文件)

                                                                         install(安装一个库或可执行文件)

                                                                         execute(运行可执行文件，并自动设置库路径)

                                                                         uninstall(卸载库)

                                                                         clean(从编译目录清除生成的文件)

                                                                         finish(完成libtool库文件的安装)



                                                        eg:

                                                            libtool --tag=CXX --mode=compile g++ xxx.cpp -c -o xxx.lo //生成xxx.lo

                                                            libtool --tag=CXX --mode=link g++ xxx.lo -o libxxx.la     //根据xxx.lo生成共享库libxxx.la

                                                            libtool --mode=install install -c xxx(可执行文件) /usr/local/bin(安装的路径)

                                                            libtool --mode=uninstall remove /usr/local/bin/xxx(删除一个安装的可执行文件)




                                            ②：配合autoconf命令生成动态库：

                                                        在需要的项目的根目录下，运行 libtoolize //Mac为glibtoolize

                                                        修改configure.ac中需要添加上：AC_PROG_LIBTOOL

                                                        根目录的Makefile.am内容：

                                                                                 AUTOMAKE_OPTIONS = foreign

                                                                                 SUBDIRS = XAudioRecord XData XMedioEncode XRtmp XVedioRecord include

                                                                                 bin_PROGRAMS = main

                                                                                 main_SOURCES = main.cpp

                                                                                 main_LDADD = XAudioRecord/libxaudiorecord.a XMedioEncode/libxmedioencode.a XRtmp/libxrtmp.a XVedioRecord/libxvediorecord.a

                                                                                 main_CXXFLAGS   = -I../../../include -I./include

                                                                                 main_LDFLAGS=-L../../../lib -LXData

                                                                                 LIBS = -lxdata -lQt5Core -lQt5Multimedia -lswresample -lavutil -lavcodec -lavformat -lswscale -lQt5Network -lswscale -lQt5Gui -lopencv_highgui -lopencv_core -lopencv_videoio -lavformat

                                                                                 CXXFLAGS =  -fPIC -std=c++11

                                                        XData目录的Makefile.am内容：

                                                                                 AUTOMAKE_OPTIONS = foreign

                                                                                 lib_LTLIBRARIES = libxdata.la

                                                                                 libxdata_la_SOURCES = XData.cpp

                                                                                 libxdata_la_CXXFLAGS        = -I../../../../include -I../include

                                                                                 libxdata_la_LDFLAGS=-L../../../../lib

                                                                                 LIBS = -lavutil

                                                                                 CXXFLAGS = -fPIC -std=c++11


            第七步：到根目录中运行 automake -a

            第八步：运行./configure

            第九步：运行 make 即可编译

        3、deep目录结构（所有的源文件都存放在各个子目录中）

                非常类似shallow目录结构，只要注意SUBDIRS的设置即可




        当文件在编译的时候，我们通过-L可以指定库文件的位置，但是，当我们的可执行文件在运行的时候，动态库文件也需要加载，那么就需要改系统的链接库的目录，在/etc/ld.so.conf.d/xxx.conf文件










