//
//  main.c
//  cstudy
//
//  Created by tangwei on 17/7/5.
//  Copyright © 2017年 tangwei. All rights reserved.
//

/*
 *          C语言的代码到运行结果其实有4步
 *   1、代码编写
 *   2、编译代码成机器能识别的代码，由.c文件到.o文件
 *   3、连接文件，将编译后的.o文件与系统内置提供的函数结合，生成可执行文件
 *   4、运行连接之后的文件
 
 数据类型：
    基本类型
        |-----------数值类型
                        |-------整型
                        |-------浮点型
        |-----------字符类型
 
    构造类型
        |-----------数组
        |-----------共用体
        |-----------枚举型
        |-----------结构体
 
 空类型
 
 指针类型
 
 
 一字节==8位
二进制：
    1或0就占1位
    10就占2位 即整型2就占2位
    100就占3位
 
 
 int型数据：系统自动分配4个字节 最大值就是2的32次方-1
 
 有无符号，是指最高位有没有符号位，无符号就指占满符号位，有符号就少占一个位
 
 有符号短整型：short 占用字节数：2 取值范围：-32767 到 32767
 无符号短整型：short 占用字节数：2 取值范围：0到65524
 有符号整型：int 占用字节数：4 取值范围：-2147483647 到 2147483647
 无符号整型：int 占用字节数：4 取值范围：0到4294967295
 有符号长整型：long 占用字节数：4 取值范围：-2147483647 到 2147483647
 无符号长整形：long 占用字节数：4 取值返回：0到4294967295
 
 在定义一个整型的时候，如果是有符号的，signed是可以省略的，而无符号的unsigned是不可以省略的
 
 浮点型数据：
 单精度浮点型：float，不在乎取值范围，只需知道他精确到小数点后7位
 双精度浮点型：double，不在乎取值范围，只需知道他精确到小数点后15位
 
 字符型数据：用''单型号包含的单个字符都是字符型数据，如：'A', 'T', '3',注意一定是''不能用 ""，实际存储的是其ASCII码值（数字），可以直接参与算数运算
 有符号的字符型：char 占用字节数：1 取值范围： -128到127
 无符号的字符型：char 占用字节数：1 取值范围：0到255
 
 --------------------------------*******************************************************----------------------------------------------------------------------------------
 
 用sizeof查看各种类型的存储长度
 
 常量一经定义不能改变  变量在程序运行过程中可以改变
 
 定义一个常量：
    #define xxx 20
 
    const float xxx=3.14;
 
 判断是否定义了一个常量
    #ifdef xxxx
        执行
    #else
        执行
    #endif
 
 
    #ifndef xxx
        执行
    #else
        执行
    #endif
 
 位运算符：
            & | ~  ^ >>  <<
 
 求字节运算符：
            sizeof(变量名或者类型名)
 
 指针运算符：
            * &
 
 求一个数的绝对值：
                fabs()
 
 --------------------------------*******************************************************----------------------------------------------------------------------------------
 
 if的写法：
    if(xxx){
        xxx
    }else if(xxxx){
        xxxx
    }else{
        xxxx
    }
 
 
 
 switch...case的写法：
            switch(表达式){
                case 1:
                break;
 
                case 2:
                break;
 
                default:
                break;
            }
 
 while的写法：
            while(表达式){
 
                xxxxx
 
            }
 
 do...while的写法：
            do{
                xxxxx
            }while(表达式);
 
 for的写法：
            for(int i =5; i <= 10; i++){
                print(i)
            }
 
 
 break;continue;
 
 
 强制类型转换
 (float)(变量)
 
 (int)(变量)
 
 (int)x
 
 (int)(x+y)
 
 
 标准的输入输出：C语言本身不提供输入输出语句，都是靠系统库函数来实现的即"stdio.h"
 1、putchar(参数) eg: putchar('A')或者 putchar('A'+32) 如果成功返回字符的ASCII值，否则返回EOF即-1
 
 2、getchar()从接盘接受一个字符,以回车结束输入。成功返回字符的ASCII值，否则返回EOF即-1
 
 3、printf()打印东西到屏幕
 
 4、scanf("格式化控制字符", 地址列表) eg:scanf("%d %f", &num, &f); 从键盘输入一个整数一个浮点数，将其存入变量num、f中去,当然num、f都需要提前定义好！！
 
 5、gets(变量或者指针);将用户输入存放到变量或者指针中，scanf不能接受空格、制表符Tab、回车等；而gets能够接受空格、制表符Tab和回车等
 
 --------------------------------*******************************************************----------------------------------------------------------------------------------
 
 
 C语言函数：
 自己定义一个函数：
            int test(参数){
                xxxx
                return 0;
            }
 调用 test();
 调用函数要在被调用之前进行声明：
 声明：int test(int);
 
 全局变量一经定义在哪个函数中都能调用！
 
 将函数写入到一个文件中去，只要将这个文件#include xxxx.c进来，然后照样可以调用函数！！
 
 extern关键字，是证明，这个函数或者变量，在别的文件中被定义，一般用在头文件中。
 
 返回一个指针的函数
 
 int *xxx(){//xxx为函数名，返回一个int类型指针
 
 }
 
 --------------------------------*******************************************************----------------------------------------------------------------------------------
 
 数组学习：
    一维数组：
    定义一个一维数组：
            存储类型 数据类型 数组名[数组长度] eg:int arr[5];
            一维数组进行赋值：
                arr[x] = 20;
 
            或者可以一次性定义好：
                int arr[] = {1,2,3,4,5};
 
            调用一维数组的数据：
                arr[x]
 
    多维数组：
    定义一个多维数组：
            存储类型 数据类型 数组名[数组长度][数组长度] eg:int arr[2][3]
            对多维数组进行赋值：
                arr[2][3] = 10;
            一次性定义好对维数组：
                int arr[2][3] = {{1,2,3}, {4,5,6}};
 
            对多维数组的调用：
 
                arr[1][2];
 
    一维字符数组：
    定义一个一维字符数组：
            char arr[10];
 
    一维数组进行赋值：
            arr[3] = 'A';
 
    一次性定义一个一维数组：
            char arr[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J'};
 
    调用一维字符数组：
            arr[9];
 
    二维字符数组：
        定义一个一维字符数组：
 
            char arr[2][3];
 
    二维数组进行赋值：
            arr[1][2] = 'A';
 
    一次性定义一个一维数组：
            char arr[][] = {{'A', 'B', 'C'}, {'D', 'E', 'F'}};
 
    调用一维字符数组：
            arr[1][2];
 
 数组名已经是数据的存储地址了，所以我们在需要引用的地方就只要填数组名，不需要&这个地址符
 
 --------------------------------*******************************************************----------------------------------------------------------------------------------
 
 字符串学习：
    字符串如何存储？是将字符串的字母挨个存入每个字节中，字符串最后的一个字节是"\0"做为分隔
 
 用""括起来的数据
 
 不能将一个字符串赋值给字符型变量，即 char ch = "xxxxxx" 是错误的
 
 定义一个字符串：
    char carr[] = "tangweikang";
 
 如何从屏幕输入字符串:(不推荐使用)
    char sss[];
 
 scanf("%s", sss);
 
 printf("%s", sss);
 
 字符串处理函数：一定要先引入string.h
 strcpy(字符串1, 字符串2); 将字符串2的内容复制给字符串1     将字符串2直接复制给字符串1
 
 strcmp(字符串1, 字符串2);//返回比较结果                  若str1==str2，则返回零；若str1<str2，则返回负数；若str1>str2，则返回正数
 
 strcat(字符串1, 字符串2);//拼接字符串                   字符串1的容量一定要足够，不然会报错
 
 strlen(字符串1);//获取字符串长度                        返回值为long类型
 
 把一个数组传给函数，函数内部这个数组改变，那么外面这个数组确确实实改变了！！（传地址）
 int test(int a[]){
 
    for(int i = 0; i <=4; i ++){
 
        printf("%d \r\n", a[i]);
 
    }
 
    return 0;
 }
 
 int xxx[] = {1,2,3,4,5};
 
 test(xxx);这儿是调用一个函数
 
 
 
 
 先理解变量是如何在内存中存储的！！
 以 int a = 5;为例
 声明一个int类型的a变量，会在内存中分配一个4字节的连续空间，此时a存放是的这4字节的首空间地址，通过这个首空间地址即a就能访问到整个数据。用&a就会获取到这个地址值。
 
 
 
 
 
 --------------------------------*******************************************************----------------------------------------------------------------------------------
 
 指针的学习：
    指针就是地址，指针变量就是存放其他变量地址的变量。指针就是 pl = &a;
 
 指针变量定义：
    数据类型 *指针变量名     数据类型必须与给他地址的变量的数据类型一致 比如 int a = 10; int *pl = &a; a是int，pl这个指针也要是int，不然会报错
        eg: 
            int *pl
 
            float *pl2
 
            double *pl3
 
            char *pl4
 
 运算的时候: *号是指去解析地址中的数据
 带*号
    eg：
        int a = 5, b=6;
 
        int *pl = &a, *pl2 = &b;
 
        int c = *pl + *pl2;
 
        printf("%d", c);
 
 不带*号：
    eg:
        int a[] = {1,2,3,4}
 
        int *pl = a;
 
        int *pl2 = pl + 3; +3就是地址位向下移动3位，-3就是地址位向上移动3位
 
        printf("%d", *pl2);
 定义一个空指针：
    double *pl = NULL;
 
 将一个一维数组的首地址给一个指针，那么就会有以下效果：
    int a[] = {1,2,3,4};
 
    int *m = a;
 
    m[0] == a[0] 效果一致
 
    m[1] == a[1] 效果一致
 
 将一个一维字符串的首地址给一个指针，那么就会有一下效果：
    char a[] = "tangwei";
 
    char *b = a;
 
    printf("%s", b);
 
    printf("%c", b[0]);
 
    所以 我们定义一个字串何不简单一点：
 
    char *str = "tangwei";
 
    printf("%s", str);
 
    printf("%c", str[0]);
 
 将一个二维数字数组的地址给一个指针，那么像要获取所有的数据，就要按如下要求：
 
    int a[2][3] = {{1,2,3}, {4,5,6}};
 
    for(int i = 0; i <= 1; i ++){
 
        int *c = a[i];
 
        printf("%d", c[0]);
 
        printf("%d", c[1]);
 
    }
 
 将一个二维字符串数组的地址给一个指针，那么想要获取所有的数据，就要按如下要求：
 
    char str[5][10] = {"tangwei", "kangkang", "haha", "litong", "666"};
 
    for(int i=0;i<=4;i++){
 
        char *m = str[i];
 
        printf("%s \r\n", m);
 
        printf("%c \r\n", m[0]);
 
    }
 
 那么我们可以找个简单的写法，来写一个二维的字符串数组：
 
    char *str[] = {"tangwei", "kangkang", "haha", "litong", "666"}
 
    printf("%s", str[0]);
 
    printf("%c", str[0][0]);
 
 
 
 
 怎么把一个指针传递给一个函数
 
    int *test(int *a);
 
    int *test(int *a){
        return a;
    }
 
    int b = 10;
 
    int *c = test(&b);
 
    printf("%d \r\n", c);
 
 
 --------------------------------*******************************************************----------------------------------------------------------------------------------
 
 
 结构体、共用体、枚举型数据：
    所谓的 数值类型、指针类型、数组类型 都是系统内部定义好的，存储空间等信息 我们都不能修改，
    而构造类型，是我们可以自己定义存储空间等信息，不受系统条件限制，我们自己做主！！！！比如，数组，只能存放同一类型的数据，但是，我们一个数据中要同时存放整型、浮点型数据咋办？就要依靠我们的结构体、共用体、枚举型数据！
 
 结构体类型：
    概念： 将一些不同类型的、有一定关联的数据组合到一起，形成一个新的数据类型，就叫结构体。
    声明：
        struct 名称{
            类型1 成员名1;
 
            类型2 成员名2;
 
            .
            .
            .
            类型N 成员名N;
 
        };
 
    eg:
        struct student{
            int age;
 
            int sex;
 
            char *name;
 
            float score;
        };
 定义结构体变量：
    方法一：
        struct student student1, student2; 定义了student1、student2两个变量！
 
 
    方法二：
        struct student{
            int age;
 
            int sex;
 
            char *name;
 
            float score;
        }student1,student2;
 
 
 结构体变量的初始化：
    方法一：等变量定义好了，单独初始化：

        student1.name = "tangwei";
 
    方法二：初始化struct的时候整体初始化：
        struct student{
 
            int age;
 
            int sex;
 
            char *name;
 
            float score;
 
        }student={28,1,"tangwei", 100};
 
    结构体变量的使用：
            student1.age;
 
            sstudent1.name;
 
    如果两个变量都属于同一个结构体类型，那么其中一个可以直接赋值给另一个：
            struct student student2;
 
            student2 = student1;
 
 对结构体变量的输入输出，只能挨个进行，不能整体一次性操作：
    scanf("%d,%d,%s,%f", &student1.age, &student1.sex, student1.name, &student1.score);
 
 
 结构体数组：
    有一个结构体student，而如果一个班有50个学生，那么需要定义50个结构体变量？肯定不现实！，那么就有了结构体数组
 
    struct Student{
 
        int age;
 
        char *name;
 
        float score;
 
    }student[45];
 
 上面就定义了一个结构体student的数组，该数组有45个结构体元素。
 
 调用么：student[0].name即可
 
 结构体指针变量：
 
    结构体指针变量指向的是结构体变量的内存首地址。
 
    结构体指针变量也可以用来指向结构体数组中的元素
 
    结构体指针变量的定义：
            struct student *pstudent;
 
            pstudent = &student1;
 
            pstudent = &student2;
 
    访问结构体指针变量对应变量中的变量时：
 
            pstudent->name;  这个最常用
 
    或者：
 
            (*pstudent).name;
 
    结构体与函数：
        1、将结构体变量传递给函数，那么函数内部也是一样使用。
            struct Student{
 
                int age;
 
                char *name;
 
                float socre;
 
            }student;
 
            int test(struct Student stu){
 
                return stu.age;
 
            }
 
            student.age = 20;
 
            int a = test(student);
 
            printf("%d", a);
 
 2、将结构体指针变量传递给函数，那么函数内部也是一样使用。
    struct Student{
 
        int age;
 
        char *name;
 
        float score;
 
    }student;
 
    student.age = 20;
 
    struct Student *stu = &student;
 
    int test(struct Student *stu){
 
        return stu->age;
 
    }
 
    int a = test(stu);
 
    printf("%d", a);
 
 
 
 3、返回结构体的函数。
    struct Student{
 
        int age;
 
        char *name;
 
        float score;
 
    }student;
 
    struct Student test(struct Student stu){
 
        stu.age = 30;
 
        return stu;
    }
 
 
    struct Student s = test(student);
 
    printf("%d", s.age);
 
 --------------------------------*******************************************************----------------------------------------------------------------------------------
 
 共用体的学习：(使用方法参考struct)
 
 定义一个共用体：
    union xxx{
        类型1 数据1;
 
        类型2 数据2;
    };
 
 共用体变量：
    union xxxx tangwei;
    或者：
    union xxx{
 
        类型1 数据1;
 
        类型2 数据2;
 
    }tangwei1,tangwei2;
 
 --------------------------------*******************************************************----------------------------------------------------------------------------------
 
 枚举型的学习：
 概念：一个变量的取值只限于事前一一列举出来的取值范围，枚举型就是为这种变量服务的。
 
 定义一个枚举型：
    enum xxx{
 
        枚举常量1,
 
        枚举常量2,
 
        枚举常量3,
 
        .
 
        .
 
        枚举常量N
 
    };
 经枚举型定义之后，枚举常量1的值为0，其后的值都是依次+1
    eg:
        printf("%d", 枚举常量1);
 
 --------------------------------*******************************************************----------------------------------------------------------------------------------
 
 typedef定义类型：
    概念：对已有的类型名起个别名
    eg：
        typedef int tangweikang; 给int定义了一个tangweikang这个别名,int也可以使用。
 
 
 --------------------------------*******************************************************----------------------------------------------------------------------------------
 
 链表学习：
    数组的弊端，数组只能存放同一种类型的数据，并且在使用之前就需要提前声明好数组的个数，在程序中没法灵活的调整数组中数据的个数！！如果按照最大的个数来定义数组的个数，那么又会造成系统资源的浪费！！那咋办？链表应运而生！！
 概念：
    在程序执行过程中，根据需求，当有数据需要存储时就向系统申请存储空间，实现动态数组，不造成存储区的浪费。
 
 分类：单链表、循环链表、双向链表。
 
 单链表的构造：
    是指数据节点单项排列，一个单链表节点，起结构类型分为：数据域、指针域。数据域存放数据，比如1、2、3、4等，指针域存放指向下一个节点的地址。
 eg：
    节点1---------->节点2----------->节点3
    数据1|节点1------->数据2|节点2------->数据3|节点3
 
 节点的定义：
    typedef struct tangwei{
 
        char *name;
 
        struct tangwei *plink;
 
        }tw;
 注意点：
    1、tw是 struct tangwei这个结构体的别名，不是结构体类型数据。需要定义结构体变量，需要tw xxxx才行。
    2、char *name 是数据域，用来存放数据。
    3、struct tangwei *plink 是指针域，用来存放下一个节点的地址的。
 
 定义好了链表的节点基本机构之后，在程序运行时，在数据域中存储适当的数据，如有后继节点，则把指针域指向其后续节点的地址，若没有则置为NULL。
 
 链表的创建：
    eg：
        #include <stdio.h>
 
        #include <stdlib.h>
 
        #include <sys/malloc.h> 动态分配内存的库，包含函数：malloc()动态分配内存函数。
 
        #define N 10
 
        typedef struct kang{//定义好节点
 
            char *name;
 
            struct kang *plink;
 
        }tw;
 
        tw *ceshi(int n){//创建一个链表,定义一个名为测试的函数。返回值是第一只节点的地址。n是定义的要创建多少个节点
 
        tw *front(当前指针的前一个), *head（头结点）, *current（当前节点）;
 
        if(head = (tw *)(malloc(sizeof(tw))) == NULL){
 
            printf("不能分配内存空间");
 
            exit;
 
 
        }
 
        head->name = "\0";头结点的数据域不存储任何内容！
 
        head->plink = NULL;
 
        front = head;
 
        int i = 0;
 
        for(i <= n; i++){
 
            if(current = (tw *)malloc(sizeof(tw)) == null){
                printf("不能分配内存空间");
 
                exit;
 
            }
 
            front->plink = current;
 
            current->plink = null;
 
            front = current;
 
        }
 
        return head;
 
 
    }
 
 创建链表方法的使用：
    tw *haha = Creat(10);
 
 
 单链表的使用：
 
    1、查找：
        tw *search(tw *head, char *str){
 
            tw *current;
 
            current = head->link;
 
            while(current->plink != NULL){
 
                if(strcam(current->name, str) == 0){
 
                    return current;
 
                    break;
 
                }
 
            current = current->plink;
 
            }
 
    }
 
 2、插入
 
    tw *insert(tw *p){//在给定的节点后插入一个
 
        if(tw *now = (tw *)malloc(sizeof(tw)) == NULL){
 
            printf("分配内存失败");
 
            exit;
 
        }
 
        now->plink = p->link;
 
        p->plink = now;
 
    }
 
 3、删除：
    void del(tw *x,tw *y){//删除x之后的y节点，并把这个的内存释放 free(指针名称)
 
        x->plink = y->plink;
 
        free(y);
 
 
    }
 
 --------------------------------*******************************************************----------------------------------------------------------------------------------
 
 文件操作学习：
    1、文件的打开：
        FILE *tangwei;
        tangwei = fopen("/etc/php.ini", "r");
 
 模式：
    r 打开只读文件，该文件必须存在。
 
 　　r+ 打开可读写的文件，该文件必须存在。
 
 　　rb+ 读写打开一个二进制文件，只允许读写数据。
 
 　　rt+ 读写打开一个文本文件，允许读和写。
 
 　　w 打开只写文件，若文件存在则文件长度清为0，即该文件内容会消失。若文件不存在则建立该文件。
 
 　　w+ 打开可读写文件，若文件存在则文件长度清为零，即该文件内容会消失。若文件不存在则建立该文件。
 
 　　a 以附加的方式打开只写文件。若文件不存在，则会建立该文件，如果文件存在，写入的数据会被加到文件尾，即文件原先的内容会被保留。（EOF符保留）
 
 　　a+ 以附加方式打开可读写的文件。若文件不存在，则会建立该文件，如果文件存在，写入的数据会被加到文件尾后，即文件原先的内容会被保留。 （原来的EOF符不保留）
 
 　　wb 只写打开或新建一个二进制文件；只允许写数据。
 
 　　wb+ 读写打开或建立一个二进制文件，若文件存在则文件长度清为零，即该文件内容会消失，允许读和写。
 
 　　wt+ 读写打开或着建立一个文本文件，若文件存在则文件长度清为零，即该文件内容会消失；允许读写。
 
 　　at+ 读写打开一个文本文件，允许读或在文本末追加数据。
 
 　　ab+ 读写打开一个二进制文件，允许读或在文件末追加数据。
 
 2、文件的关闭：
    fclose(FILE *xxxx);关闭成功返回0，如果非0，则证明关闭失败,可以用ferror来测试
    eg:
        fclose(tangwei);
 
 3、写入一个字符到文件：
    fputc(char a, FILE *xxx);写入成功，就返回写入字符的ASCII值，失败则返回EOF，EOF是stdio.h中定义的常量，值为-1
    eg:
        fputc('a', tangwei);
 
 4、从文件中读出一个字符到内存：
    fgetc(FILE *xxx);返回读取到的内容，遇到文件结束符EOF时，返回EOF
    eg:
        fgetc(tangwei);
 
 5、写入一个字符串到文件：
    fputs(char *str, FILE *xxx);失败返回0，成功返回非0
    eg:
        char *str = (char *)malloc(8*sizeof(char));
 
        str = "tangwei";
 
        FILE *file = fopen("/Users/tangwei/WebServer/cstudy/cstudy/hahaha.txt", "a");
 
        fputs(str, file);
 
        char *str1 = (char *)malloc(8*sizeof(char));
 
 6、从文件中读取一个字符串到内存：
    fgets(char *str, int *BUFFSIZE, FILE *xxx);读取成功返回字串到str，失败整体返回一个空指针NULL。
    eg：
        char *str1 = (char *)malloc(8*sizeof(char));
 
        FILE *file2 = fopen("/Users/tangwei/WebServer/cstudy/cstudy/hahaha.txt", "r");
 
        while(fgets(str1, 8*sizeof(char), file2) != NULL){这儿的意思是每8个字节循环读取一次
 
            printf("%s \r\n", str1);
 
        }
 
        fclose(file2);
 
 7、数据块的读取：什么叫数据块，char *str、struct tangwei{}等都是数据块
    fread(void *str, unsigned size, unsigned count, FILE *xxx); 从xxx文件中，读取count次数据，每次读取大小为size，并一起存入到str中。str一定要是指针，返回读取的count数
 
    eg：
        ①：
            int buffer[5];
 
            FILE *file2 = fopen("/Users/tangwei/WebServer/cstudy/cstudy/hahaha.txt", "rb");
 
            fread(buffer, sizeof(int), 1, file2);
 
            for (int i =0; i <= 4; i ++ ) {
 
                printf("%d \r\n", buffer[i]);
 
            }
 
            fclose(file2);
        ②：
            struct kang{
 
                char *name;
 
                int age;
 
            }ceshi;
 
            fread(&ceshi, sizeof(struct kang), 1, tangwei);
 
 
 8、数据块写入到文件,成功返回写入的count数（所有的结构体只要1次写入）
    fwrite(void *str, unsigned size, unsigned count, FILE *xxx); 将str数据，分count次，每次大小为size地写入到文件中。str一定要是指针
 
    eg:
        ①：
            int a[5] = {1,2,3,4,5};
 
            FILE *file = fopen("/Users/tangwei/WebServer/cstudy/cstudy/hahaha.txt", "wb");
 
            if(file == NULL){
 
                printf("打开文件失败！");
 
                exit(0);
 
            }
 
            long res = fwrite(a, sizeof(int), 1, file);
 
            printf("%ld \r\n", res);
 
            fclose(file);
 
 
        ②：
 
            typedef struct tangwei{
 
                char *name;
 
                int age;
 
                float score;
 
            }tw;
 
            tw *kang = (tw *)malloc(sizeof(tw));
 
            if(kang == NULL){
 
                printf("分配构造类型内存空间失败");
 
                exit(0);
 
            }
 
            kang->age = 28;
 
            kang->name= "tangwei";
 
            kang->score = 99.5;
 
            FILE *file = fopen("/Users/tangwei/WebServer/cstudy/cstudy/hahaha.txt", "w");
 
            long res = fwrite(kang, sizeof(tw), 1, file);
 
            if(res != 1){
 
                printf("写入文件失败");
 
                exit(0);
 
            }
 
            fclose(file);
 
 9、从文件格式化读取数据到内存：（一行一行读的）
    fscanf(FILE *xxx, 格式化控制字串, 地址表列);成功返回数据，失败返回EOF
 
    eg:
        while(fscanf(xxx, "%d,%d", &a, &b) != -1){
 
            .......
 
        }
 
 10、从内容格式化写入到文件：（一行一行写）
    fprintf(FILE *xxx, 格式化控制字符串， 地址表列);
    eg:
 
        fprintf(xxx, "%d", &c);
 
 11、fseek(FILE *file, int offset, int origin);//file为打开的文件指针 origin是起始位置，SEEK_SET就是拿到默认的头位置 offset操作的偏移量，即向前或向后移动的字节数
 
 12、fflush(FILE *file)//立即同步数据到文件，如果不用这个，那么会在每次fseek的时候才会将数据写入文件，调用函数fclose关闭该文件，同样可以达到将缓冲区的内容写到文件中的目的，但是那样系统开销较大
 
 13、FILE *file = freopen("xxx.txt", "wb", FILE *file)//覆盖当前file的资源流，重新创建一个新的xxx.txt文件并创建新的资源流
 
 
 
 --------------------------------*******************************************************----------------------------------------------------------------------------------
 
 位运算学习：
 使用范围，只能用在整型和字符型数据。
 
 十进制数与二进制数之前的相互运算。此规则可以被任何进制沿用！只是基数不同
 
 运算符：
 1、~ 按位取反 就是所有的位数据全部取反。eg：~a
 2、<< 左移 所有的位数据全部左移，被移除的忽略，被移动的位置补0。eg： a<<2 a全部向左移2位，移动位置补0
 3、>> 右移 所有的位数据全部右移，被移除的忽略，被移动的位置补什么，取决与符号位，如果符号位为0，则补0，符号位为1，则补1。如果无符号，则直接补0.eg： a>>2 a全部向右移2位
 4、& 按位与 所有的位数据全部与运算，即 两者为1结果才为1。eg: a & b
 5、| 按位或 所有的位数据全部或运算，即 两者中只要其中有一个是1结果就为1， eg： a | b
 6、^ 按位异或 所有的数据位全部异或运算，即 两者相同就为0，两者不同就为1，eg: a^b
 
 
 --------------------------------*******************************************************----------------------------------------------------------------------------------
 
 普通变量加上static之后，就变成了，只能在本文件内调用。
 
 双向链表：
    结构体中有两个指针，分别指向前后节点的地址。
    
    头节点，前驱节点没有指向。
    尾节点，后驱节点没有指向。
 
 
 *
 */
