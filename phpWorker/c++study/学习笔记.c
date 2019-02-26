
知识点： 函(函数的重载、函数的默认参数)----->运(运算符重载 "=")------->内(内存管理，new、delete)-------->糠(const、cast)-------->数(数组 vector)------->字(字符串 string)-------->名(命名空间 namespace)------->引(引用 &)------->导(运算(a=b)=c)---->枚(枚举型数据)----->布(布尔值)-------->输(输入输出)

        模（STL）---------->文(文件读写、二进制文件读写)---------->虚（虚基类、虚函数）--------->运（运行时类型识别 typeid()、dynamic_cast()）-------->转（类类型转换，使用场景：  传参、赋值）---------->继（继承(函、友、赋、构、拷)）----->赋（赋值兼容）----->态（多态）------->smartypoint(智能指针)------>指(指向类成员的指针)------>友（友元）------>this------->赋(如果"="两边类相同、如果两边类不同，调用机制，具体写法，原理)------>数(调用机制)--->类(写法、存储)-------->函（如何把类当成函数一样使用）-------->const（const、static）-------->构(调用机制、具体写法、原理)----->重(重载)------->拷(调用机制、具体写法、原理)

        栈上的对象可以返回，但是栈上的引用不能返回(除非返回对象自己)

        string a = b + c;

        同类（拷贝、传入类函数中的同类）之间无隐私，异类之间有友元。

        给类函数的返回值加个const关键字，目的是什么？

        int i,j,k;

        (i + j) = k;//这样的写法不被允许，那么我们自己写的类，也不能有这样的操作，所以我们在类的operator+或者operator=等操作中，都需要对返回值进行const操作。

一、C++对C的扩展,C++的头文件没有.h结尾
    类型增强：
        ①、类型检查更严格：
                比如 在C中const的常量，把他的地址给一个const类型的指针，C中能编译通过，但是C++中不能通过

                eg：
                    const int a = 10;//因为const是常量是不能修改的，但是通过一个指针那就能修改

                    int *b = &a;

                    *b = 100;//在C中，此时a也是100了，证明const的常量也是能被修改的，但是 在C++中 这种写法不能编译通过

        ②、布尔类型（bool）：

                添加了 true、false

                eg:
                     bool a = true/false;

        ③、真正的枚举：在C++中，尽量不用宏定义，能用enum的就用enum、能用const就用const。
                在C中：
                      enum day{

                          mon, the, wes
                      };

                      enum day x;

                      x = 100;//在C语言中这样没有问题 ，但是在C++中这样是错的

                      printf("%d", x);

        ④、表达式被赋值：
                int a,b = 10;

                (a = b) = 100;//在C语言中 这儿是报错的，因为 C语言中不允许对表达式赋值，但是C++中允许，等价于 先把b的值给a，在把100给a，那么b的值没有改。

                printf("a = %d, b= %d", a , b);

                还有更复杂的：

                        ( a != b ? a : b ) = 1000;

        ⑤、C++的输入输出：
                cin、cout是类对象，跟C中scanf、printf类似，不过scanf、printf是函数

                eg：

                    char name[20];//但是 这儿会不安全，因为输入会超过预期。所以在C++中，最好用string name;

                    cin>>name;

                    cout<<name;//可以一次输出多个 cout<<xxx1<<xxx2





                    cin.get(buf, 10, 'i');//读取屏幕输入的9个字符，如果遇到'i'会提前结束，如果遇不到则读满9个，然后放入buf中，如果屏幕输入没有满9个，则无限等待

                    cout << buf << endl;



                    cin.getline(buf, 10, 'i')//等待屏幕输入9个字符，如果遇到回车直接输出,没有的话就随便输入，回车取前9个

                    cout << buf << endl;




                    cin.ignore(100, 'i');//100为忽略几个字符，'i'为 直到忽略到'i'字符为止。此时'i'就没有了

                    cin.putback('i');//这儿就是把上面多忽略掉的'i'还回来

                    char pe = cin.peek();//拿到数据看看，并不会到数据的指针





                cin >> a >> b 和 cout << a << "" << endl << b << endl   //endl相当于换行符 \n

                怎么设置输出的格式：(要用setw()等函数，需要引入iomanip这个类库)

                    cout << setw(10) << a//setw()是设置域宽，即输出的a的长度是10，如果a的时间数据没有超过10，那么前面补上空格，如果a的实际数据超过10，则按时间数据显示。

                    cout << a //如果a是浮点型数据，那么默认会输出，6个有效位数

                        eg：
                            float 207.456789

                            cout << a << endl; //结果为207.457

                            如果要修改输出的长度，可以强制修改为：cout << setw(10) << a << endl;//.也算一个长度，所以烧掉3个长度会用空格补上，默认补在左边。

                            如果想让空格默认补在右边的话，需要加上 setiosflags(ios::left)即可，即：cout << setiosflags(ios::left) << setw(10) << a << endl;

                            如果我强制要求只能输出小数点之后的2位，怎么办？
                                加上 setprecision(2) << setiosflags(ios::fixed) 即可 即 cout << setw(10) << setprecision(2) << setiosflags(ios::fixed) << a << endl;


                输出十六进制：

                    cout << hex << a << endl;

                输出八进制：

                    cout << oct << a << endl;

                setw()这个函数，会设置值域，如果不够长度会默认填充空格，但是不想填充空格，可以使用 setfill('x')，会用x来填充。这个setfill()是全局的，只要设定了，后续所有的输出空格都会被替换成x。



二、函数重载：（同一个函数表示好几个意思，具体什么意思，要由语境决定）就是静多态。

    1、搞清楚重载和重定义的区别，重定义到哪里都不行。重载是：两个函数，方法名相同，但参数列表不同（类型、个数、顺序），返回类型不影响。

    2、在C中，哪怕只是方法名相同都是不行的。在C++中只是重定义是不行的，重载没问题。

    3、ambiguous 二义性

    4、从多个重载的函数中，寻找合适的，先严格匹配，找到则调用，否则，先隐式转换寻找一个匹配，找到则调用。如果没找到就报错。如果找到两个，就会报二义性错误。

    5、函数重载原理：

            在c++中有个倾轧，会对定义的函数进行倾轧，比如，定义函数，void print(int a) 倾轧成 print_i 而我们调用的时候 print(a) 如果a是int类型，那么也是倾轧成 print_i或者print这两个具体根据头文件的定义去看到底是哪一个，然后去调用。
        但是，当我们在声明这个函数的时候指定它不倾轧，或者在实现函数的时候不倾轧，那么调用就会出错！

            eg：

                在head.h的头文件中，定义重载函数：
                    int print(int a);

                    char print(char b);

                在func.cpp文件中，实现这个重载函数：
                    int print(int a){

                        printf("%d", a);

                    }

                    char print(char b){

                        printf("%c", b);

                    }

                在main.cpp文件中，调用重载函数：

                    int a = 10;

                    char b = 'A';

                    print(a);

                    print(b);


                如何指定函数的声明和实现不倾轧呢？

                    就用 extern "C" int func(int a);//告诉func在实现时不需要倾轧

                    或者
                        extern "C" {//这里面就是C语言的语法

                               int func1(){

                               }

                               char func2(){

                               }

                        }

            为什么要用extern "C"{}这个东西呢？
                    因为，C++完全兼容C，在C的库函数的头文件中比如，stdio.h这个头文件中，代码明显有extern "C"这串代码，证明被C++引入，编译时不倾轧，所以，printf()函数在C++中也能调用

                    具体头文件中的写法为：
                        #ifdef __cplusplus //如果是C++编译器就要进行不倾轧设置，

                            extern "C"{

                                xxxxx

                            }

                        #endif

三、运算符的重载：
    1、就是对 + - * / 的重新定义，以便我们使用。因为，我们在调用运算符的时候，其实就是调用函数，比如 a+b 其实调用的是operator+(a, b)
        eg:
              struct info(){

                    int age;

                    float height;

              }

              info tangwei = {28, 1.83}, litong = {24, 1.65}

              info c = tangwei + litong//因为tangwei和litong都是两个结构体，那他们很明显是不能够相加的，那么我现在就必须让他们相加，那么我就要对+这个运算符进行重载，重载如下：

                    info operator+(info a, info b){

                            info c;

                            c.age = a.age + b.age;

                            c.heifht = a.height + b.height;

                            return c;
                    }

                    那么，现在的 info c = tangwei + litong的c就是相加之后的结构了！！


四、函数的默认参数：
    如果函数有一个默认参数，随便是哪个位置都行，但是多个默认参数，必须是连续有默认参数，默认参数之间不能夹杂不默认的参数，否则会报错！函数的默认参数，在声明中写，不在定义中写！

    有时候，函数的默认参数会和函数重载冲突：

        void print(int a){

            printf("first int a func");

        }

        void print(int a, int b = 20){

            printf("second int a int b func");

        }

        那么调用的时候，单单使用print(5)会出错！，所以能看出使用默认参数的函数，不要使用重载！



五、引用(C++中不使用指针，那么C++中如何传递地址？那就用到了引用。)

    int a = 500;

    a 如何传引给b呢？

    int &b = a; // 此时b就是a的别名，即b是a的引用

    引用必须初始化，即 = a 是必写的，类型必须与原来行保持一致。引用不开辟内存空间。一个变量只能接受一个地址且不能更改，可以对引用再进行引用。

        eg:

            如果 此时，有个int c = 600;再把int &b = c;这样是错误的。

            可以，int &m = b;这样没问题。

    对引用的应用：

         示例1：
             void swap(int &a, int &b){

                  a ^= b;

                  b ^= a;

                  a ^= b;

             }

             int a = 3, b = 5;

             swap(a, b);

         示例二：//在C语言中，数组或字符串，eg： char *str = "xxxx" str和&str是有区别的！！

                void swap(char *c, char *d){//使用指针，会开辟新的空间

                    char *tmp;

                    tmp = c;

                    c = d;

                    d = tmp;

                }

                char *a = "tagnwei";

                char *b = "litong";

                swap(a, b);

                printf("%s", a);

                printf("%s", b);

            这样并不能达到更换值的作用，我们如果换成引用来做下呢？！

            需要修改为：

                void swap(char **c, char **d){

                    char *tmp = *c;

                    *c = *d;

                    *d = tmp;

                }

                char *a = "tangwei";

                char *b = "litong";

                swap(&a, &b);


            而如果，在这儿换上引用，该怎么写？你会发现很简单！！！！

                void swap(char *&c, char *&d){

                    char *tmp;

                    tmp = c;

                    c = d;

                    d = tmp;

                }

                char *a = "tangwei";

                char *b = "litong";

                swap(a, b);

            但是在C++中，真正的使用是如下：

                swap(string &str1, string &str2){

                    string tmp;

                    tmp = str1;

                    sstr1 = str2;

                    str2 = tmp;

                }

                string a = "tangwei";

                string b = "litong";

                swap(a ,b);

    引用的提高：

        1、可以创建指针的引用，但是不能创建引用的引用（指针是可以建立指针的指针）

            char *str;

            char *&strp = str;//这个没问题

            int a;

            int &ra = a;

            int & & rra = ra;//这个是错误的，只能 int & rra = ra;


        2、可以定义指针数组，不可以定义引用数组，可以定义数组的引用

            int x,y,z;

            int *p[] = {&x, &y, &z};//这个是可以的

            int &p[] = {x, y, z};//这个是错误的，因为不能创建引用数组

            int a[] = {1,2,3,4};

            int (&xx)[4]  = a;//这样是没问题的

六、const关键字的处理：
    1、const变量只能传给const指针、引用

    2、非const变量也能传递给const指针、引用

    3、将变量的引用给不同类型的数值，加个const就不会报错

    4、将常量，比如200的引用直接给一个变量，加上const也不会报错

    5、将表达式，比如a+200的引用直接传递给一个变量，加上const也不会报错

七、内存管理，new/delete 对应到C语言中的 malloc/free。

    malloc/free是C中的库函数，但是new/delete是C++中的关键字。

    能用malloc/free解决的，都能用new/delete解决，但是能用new/delete解决的，不一定能用malloc/free解决。

        1、开辟单变量空间：
            int *p = new int;//相当于 (int *)malloc(sizeof(int));

            delete p;

            int *p = new int(200);//给p分配内存空间，并设置默认值为200

            delete p;

            string *str = new string("tangwei");//字符串初始化

            delete str;

            struct stu{

                int age;

                float height;

            }

            stu *test = new stu({28,1.83});//结构体初始化

            delete stu;

        2、开辟数组空间：

            char *p = new char[40];

            memset(p, 0, sizeof(char[40]));//这儿是初始化p的值

            strcpy(p, "tangwei");

            cout << p;

            delete []p;

            //指针数组怎么使用new
            char *arr[3] = {&a, &b, &c};//首先指针数组

            char **ppc = new char *[3]{NULL};

            ppc[0] = "tangwei";

            ppc[1] = "kangkang";

            //多维数组怎么使用new
            int (*a)[4] = new int[3][4]{{0}};

            a[0][0] = 10;

            delete []a;

        3、如果new失败了，C++中会自动抛出一个异常。但是异常在C++中一般不用

            可以用:
                int *p = new (std::nothrow) int(9);

                if(p == NULL) return 0;


八、内联函数

    宏函数：//代码实际内嵌

        #define SQR(x) ((x)*(x))

    普通函数：//执行时调用

        int sqr(x){

            return x*x;

        }

    内联函数：//代码内嵌，其实在C++编译的过程中，是给编译器的建议。

        inline int sqr2(x){

            return x*x;

        }


九、强制类型转换cast：

    1、static_cast//对于隐式类型可以转化的，即可用此类型。

        eg：

            float a = 5.6;

            int b = 10;

            int c = static_cast<int>(a);//两个变量能相互转化的，用static_cast没问题

            float d = static_cast<float>(b);


            void *p;int *q;//此时，q能赋值给p，但是p不能赋值给q，如果要把p赋值给q，就需要用static_cast转换。

            q = static_cast<int *>(q);

            q = static_cast<int *>(malloc(100));



    2、reinterpret_cast//当双方隐式根本不可能通过的情况，就可以用

        eg:

            int a[5] = {1,2,3,4,5};

            int *p = reinterpret_cast<int *>(reinterpret_cast<int>((int)(a)+1))


    3、const_cast//脱常,只能应用于指针和引用。

        eg：
            class B
            {
                public:
                    B() { }
                public:
                    int m_iNum;
            };
            void foo()
            {
                const B b1;
                //b1.m_iNum = 100; //compile error

                // 可以做如下转换，体现出转换为指针类型
                B *b2 = const_cast<B*>(&b1);

                // 或者左侧也可以用引用类型，如果对b2或b3的数据成员做改变，就是对b1的值在做改变
                B &b3 = const_cast<B&>(b1);

                b2->m_iNum = 200;    //fine

                b3.m_iNum = 300;    //fine
            }
            int main( int argc, char * argv[] )
            {

                foo();

                return 0;

            }







十、命名空间(给大型项目用的)，连个明明空间重名，系统自动给你分配到一块去！头文件和函数库都需要被同样的namespace包括才有效!
    1、命名空间的定义：
        namespace 空间名{

            xxxx;

            xxxx;

        }

        eg：

            namespace test{

                int a;

                void func(){

                    cout << endl;

                }

                struct te{

                    int b;

                    float c;


                };

                namespace kangkang{

                    int a;//注意这儿

                }
            }


    2、调用:
        方法一：

            test::a = 200;

            cout << test::a << endl;//找到test下的a

            test::kangkang::a = 100;

            cout << test::kangkang::a << endl;//找到test--->kangkang下的a



        方法二：

            using test::a;//这儿就是把命名空间中的变量解包到当前位置，就能正常使用了，但是当前不能有与其相同的变量

            a = 100;

        方法三：

            using namespace test;

            a = 100;

            kangkang::a = 200;


        如果在调用命名空间的时候，发生冲突了，那么就用{}把相对的代码括起来，形成局域代码就行。


十一、字符串(string 不是关键字，是一个类)

    1、定义一个字符串：
        string str = "tangwei";

        或者

        string str("tangwei");


    2、字符串输入：

        string str;

        cin >> str;

        cout << str << endl;

    3、字符串长度：
        string str = "tangwei";

        str.length();或者 str.size();

    4、字符串赋值：
        string str1 = "tangwei";

        string str;

        str = str1;

    5、字符串拼接：
        string str = "tangwei";

        string str2 = "good";

        string str3;

        str3 = str1 + str2;


    6、字符串比较：

        string str1 = "tangwei";

        string str2 = "tangwei";

        if(str1 == str2){

            xxxx

        }else{

            if(str1 > str2){

                xxxxx

            }else{

                xxx

            }

        }

    7、修改具体下标：

        string str = "kangkang";

        str[2] = "m";

        cout << str << endl;

    8、将字符串转化成C语言能用的字符串：

        string str = "tangwei";

        const char *s = str.c_str();

        cout << s << endl;


    9、值的交换：
        string str1 = "tangwei";

        string str2 = "kangkang";

        str.swap(str2);

    10、字符串查找
        int find(char c, int pos = 0);//方法定义,pos是要从哪个下标开始,找不到为-1

        long find(char *c, int pos = 0);//方法定义,pos是要从哪个下标开始,找不到为-1

        eg：

            string str1 = "tangwei";

            int num = str1.find('t', 0);


            string str1 = "tangwei";

            long num = str1.find("ang", 0);

    11、字符串数组
        string str[10] = {"a", "ab", "abc", "tangwei", "kangkang"};

        for(int i=0; i < 10; i++){

            cout << str[i] << endl;

        }

    12、字符串去空格

        string str = "tangwei";

        str.erase(0, str.find_first_not_of(' '));//从str字符的头开始找到第一个不为空格的位置，然后通过erase删掉其位置上的内容。

        str.erase(str.find_last_not_of(' ')+1);//从str字符的尾开始找到第一个不为空格的位置，然后通过erase删掉其位置上的内容。

十二、数组

    引入vector代码库

    vector <string> ve;

    ve.push_back(str1);

    ve.size();

    ve[i];

十三、休息一秒怎么做？

    #include <unistd.h>

    sleep(1);


十一、类
    1、public、private、protected

    2、类怎么写 怎么实例化
        class personInfo{

            private:
                int gae;

                float height;

            public:
                string username;

            public:
                void print(){

                    cout << age << height << username << endl;

                }

        };

        personInfo tangwei;

        tangwei.username = "tangwei";

        tangwei.print();

    3、如果写了一个类，那么头文件应该如何声明？声明和实现也是需要分开的！

        在头文件xxxx.h中：
            class personinfo{

                public:
                    int age;

                    float height;

                private:
                    string username;

                public:

                    void init();

                    void print();

            };

        在库文件中：

            #include "xxxx.h"

            void personinfo::init(){

                cin >> age;

                cin >> height;

                cin >> username;

            }

            void personinfo::print(){

                cout << age << endl;

            }

        调用：
            #include "xxxx.h"

            personinfo test;

            test.init();

            test.print();


        或者：

            #include "xxxx.h"

            personinfo* test = new personinfo;//这个不会调用析构函数

            test->age = 20;

            delete personinfo;

        类成员方法的存储：
            类的大小由成员属性决定，成员函数是独立于类存储的，在对象调用成员函数的时候，向该函数传递了该对象的指针，即this

        构造器：constructor
            1、与类名相同,一定是public,如果份文件，那么头文件中也需要将这个方法声明好。

            2、没有返回值

            3、被系统生成对象时自动调用，用于初始化

            4、可以有参数，可以重载，可以有默认参数。

            5、如果我们没有提供任何构造器，系统默认生成一个无参构造器。若提供了，则不再生成系统默认构造器。

            eg:

                class test{

                    test()//这儿就是个constructor
                    :param1(xxx),param2(xxxx)
                    {



                    }

                    test(param1){//这儿就是个constructor



                    }

                    test(param1=20, param2){//这儿就是个constructor



                    }

                };

                调用的时候：
                    test te;//走test()

                    test te(123);//走test(param1)


            析构器：destructor
                1、类名相同，前面加个~

                2、无参、无返回

                3、用于对象销毁时自动被调用。一般处理内存释放。

                eg:

                    class test{

                        ~test(){

                            delete xxxx;

                        }

                    };

            拷贝构造器：
                1、public类型

                2、无返回类型

                3、与类名相同

                4、参数为本类的一个引用

                5、用于函数传参，对象直接赋值给对象。

                6、如果自己不定义，那么系统会自己提供一个拷贝构造器，但是这个默认的就只是浅拷贝（他们指向同一段内存地址，那么在析构时会出错）。如何深拷贝呢？看下面的代码

                7、类拷贝之后，同类之间private也是可以访问的。

                eg:

                    class str{

                        public:

                            str(const str &s){

                                str = s.str//这个就是浅拷贝

                                //深拷贝开始
                                str = new char[strlen(s.str) +1];

                                strcpy(str, s.str);

                                //深拷贝完成



                            }

                    }

                    函数传参触发拷贝构造：
                        void func(str ss){

                        }
                        str tt("tangwei");

                        func(tt);

                    函数返回触发构造器：
                        str func(str &ss){

                            return ss;

                        }

                        str tt("tangwei");

                        func(tt);

            this指针：
                只能在类内部使用，在对象创建的时候被创建。

                this->name;

                this->age;

                this->display();

                return this;//在方法中实现

                stu * group(){

                    return this;

                }

                stu & group(){

                    return *this;

                }


            赋值运算符重载：系统默认的还是浅赋值，会导致内存泄漏，内存发生重析构，所以一定要注意。
                格式：
                    class 类名{

                        类名 & operator = (const 类名 &原对象){

                            return *this;

                        }

                    }

                    eg:
                        class myString{

                            myString & operator = (const myString &tmp){

                                if(this = &tmp)

                                    return *this;

                                delete []this->str;//先清除自己的内部数据

                                this->str = new char[strlen(tmp.str) + 1];

                                strcpy(this->str, tmp.str);

                                return *this;

                            }

                        }

                对象数组：
                    myString test[2] = {"xxx", "xxx"};//会走2次类的析构函数，同时每次会带上值

                    或者：

                    myString *te = new myString[2]{"xx", "xx"};//会走2次类的析构函数，同时每次会带上值

            类中const关键字：
                1、常成员变量：
                    const int a;//一经定义值不能改变，初始化值只能通过类的初始化成员列表进行。

                2、常成员函数：//能访问类中的任何成员属性，但是不能修改任何成员属性。但是如果非要修改普通成员属性的值，那么可以在前面加个mutable关键字。只能访问类中的常成员函数，不能访问类中普通的成员函数。

                    声明时：
                        void func(param1, param2) const;

                    实现时：

                        void func(param1, param2) const{

                            xxxx

                        }

                3、常对象：
                    使用：
                        const myString te("xxxx");//能访问类中的任何成员属性，但是不能修改任何成员属性。但是如果非要修改普通成员属性的值，那么可以在前面加个mutable关键字。只能访问类中的常成员函数，不能访问类中普通的成员函数。

                4、对象常引用。
                    使用：
                        const myString &another;


            类中static关键字：

                1、静态成员属性：       static类对象必须要在类外进行初始化,不能写在main函数中
                    ①：声明：

                        class A{

                            static int a;

                        }

                        初始化：

                        int person::a = 10;//不能写在main函数中

                        int main(){

                            cout << person::a << endl;

                        }

                    ②：static成员属性，存放到公共区域被所有的类对象共用，在对象没有的时候就已经存在，可以被调用。

                        调用：
                            person::a;

                            person.a;


                2、静态成员方法：
                    ①：定义在类声明中。

                        static void run();

                        初始化：

                        void person::run(){

                            xxxx

                        }

                    ②：因为static的成员方法，在对象没有的时候已经可以调用，所以，static的成员方法只能够调用static的成员属性和static的成员方法，不能调用普通成员属性和成员方法。

                        普通成员方法，因为是在对象生成之后才能调用，此时static的成员方法还在，所以普通成员方法能调动static成员方法和static成员属性。

                        调用：
                            person::func();

                            person.func();


            指向类成员函数的指针：

                class myString{

                    public:
                        int a;

                        string name;

                        void func(param1, param2){



                        }

                };

                指向类成员属性：

                    int myString::*a = &myString::a//这儿是取myString类的int a的指针。

                    myString str;

                    str.*a;

                指向类成员函数：

                    void (myString::*fun)(int a, const string &cc) = &myString::func

                    ①：
                        myString str;

                        (str.*fun)();

                    ②：

                        myString *p = new myString();

                        (p->*fun)();

                    ③：如果是数组怎么办？（就是存放多个类函数的指针）

                        void (myString::func[10])();

                        func[0] = &myString::a;

            友元：
                只为打破类的封装性。异类之间有友元！

                友元可以有：友元函数、友元类。

                举个例子：
                    class A{

                        private:
                            int a;

                    }

                    class B{

                        public:
                            void display(const A &tmp){

                                cout << tmp.a;

                            }

                    }

                    A a;

                    B b;

                    b.display(a);//这儿要访问，会走b的display方法，而在方法内部，调用的是A的int a属性是private，所以这儿是必错的。那么如何借助友元来搞定？

                    在类中声明的时候，加个：
                        友元函数：

                            friend void display(const A & tmp);

                            friend void B::display(const A & tmp);//当声明这个友元的时候，会发现，在A中声明时，B类还没有声明，所以把B类拿到A类的前面去，但是B中又调用了A，那么B在前A又没定义，那怎么办？这儿就用到了"泛定义"，只能定义引用或者指针。

                            在两个类中，其中一个类，需要声明另一个类的友元的时候，会发现，一个类并没有出现

                        友元类：

                            就直接：

                                friend class B;

重载：
    不能重载的五个运算符：
        1、 "."

        2、".*"

        3、"::"

        4、"?:"

        5、sizeof()

    具体的能重写的运算符，怎么重写法：

        1、=

        2、+

        3、+=     包括： a += b、 a += b += c、(a += b) += c

        4、-(负数) 包括：-a、-(-a)、 -a = 10（要这个不成立）

        5、<< 和 >> 输入输出流重载。

        6、new、new[]、delete、 delete[]（一般很少用，故这儿不记）

        7、->、*（一般很少用，故这儿不记）
            eg:
                class A{

                };

                A a;

                cout << a << endl;//我们的理解是，cout << a;可以看做是 cout.operator<< (a);是cout的成员函数，但是 我们不能去修改它里面的东西。那么只能用友元重载它。即operator<<(cout, a);

                如何修改？

                    输出：


                        ostream & operator<<(ostream &os, const A &c){

                            os << "(" << c._x << "," << c._y << ")"<< endl;

                            return os;

                        }

                        因为用到了A中的私有变量，所以 要到A类内部中将其置为友元函数

                    输入：
                        istream & operator>>(istream &is, A &c){

                            is >> c._x;

                            is >> c._y;

                            return is;

                        }

                        因为用到了A中的私有变量，所以 要到A类内部中将其置为友元函数


                    到了声明中，因为用到了ostream、istream两个类，所以需要事先#include <iostream>下

类类型的转换：
    在函数传参、赋值、运算等场景下，一个类不能够进行某一个操作的时候，就会用到类类型转换。

    eg:
        class point2D{
            public:
                point2D(int x, int y):_x(x),_y(y){}

                void dis(){

                    cout << "(" << this->_x << "," << this->_y << ")" << endl;

                }

            private:
                int _x;

                int _y;

        };

        class point3D{
            public:
                point3D(int x, int y, int z):_x(x),_y(y),_z(z){}

                void dis(){

                    cout << "(" << this->_x << "," << this->_y << "," << this->_z << ")" << endl;

                }

            private:
                int _x;

                int _y;

                int _z;

        };

        使用时：

            point2D p2(1,2);

            point3D p3(3,4,5);

            p3 = p2;//这儿p2转化成p3，现有的知识是不行的，那么这儿就用到了类类型转化

            1、到P3中写方法转化：（类转化构造函数）

                point3D(const point2D &p2){

                    this->_x = p2._x;

                    this->_y = p2._y;

                    this->_z = 0;

                }


            2、到P2中写方法：

                operator point3D(){

                    return point3D(this->_x, this->_y, 0);

                }

如何把类当成函数一样使用：
    class tmp{


    }

    调用：
        tmp t;

        int i = 10;

        cout << t(i) << endl;//这儿是把类当成函数一样使用，那么类中需要做什么呢？需要重载"()"这个运算符！！！

        类中添加代码：

            int operator()(int i){

                return i*i;

            }

智能指针：

    #include <memory>

    class A{
        public:

            void func(){
                cout << "xxxxx" << endl;
            }

    }

    void fun(){

        auto_ptr<A> ptr(new A);//new A之后被ptr托管，不需要再关心delete的问题了，delete在ptr离开fun这个函数的时候发生

        ptr->func();
    }

    func();


    自己如何实现智能指针？

    class A{

        A(B *tmpb){

            this->b = tmpb;

        }

        B * & operator->(){

            return this->b;

        }

        B & operator*(){

            reutrn *(this->b);

        }

        private:

            B *b;

    };

    class B{

        void func(){

            cout << "xxxxx" << endl;

        }

    };

    A a(new B);

    a->func();

    (*a).func();



类的继承：

    class B{

        public:
            B(int tmpb):b(tmpb){}

            int b;

    }

    class A:[public/protected/private] B,[public/protected/private]C{//A类继承了B类，默认的继承方式为private

        //public的继承方式，对被继承的类，public继承过来还是public、protected继承过来还是protected、private继承不过来



    }

    A中的构造函数怎么写：

        A(int tmpb):B(tmpb){//声明的时候不需要去写:B(tmpb)，要在实现的时候去写。

                            //父类中构造器没有任何参数，或者参数都有默认参数，那么在子类中可以不用去调用父类的构造器，系统自动调用。



        }

    A中的拷贝构造器怎么写：

        B(const B &another){

            this->xxx = another.xxx;

        }

        A(const A & another):B(another){//子类未实现拷贝构造时，会默认调用父类的拷贝构造器(无论父类实现未实现)

                                        //子类但凡实现了自己的拷贝构造，则必须显式地调用父类的拷贝构造器



        }

    子类的赋值运算符重载：
        B &operator=(const B & another){



        }

        A &operator=(const A & another){//子类未实现赋值重载时，会默认调用父类的赋值运算符重载（无论父类实现未实现）

                                        //子类但凡实现赋值重载，必须显示地调用父类的赋值重载。

            B::operator=(another);

            return this;

        }

    //如果子类中有个跟父类中相同名称的方法，那么子类中的方法会把父类中的shadow掉，然后自己调用自己，进入死循环，所以需要加以区分。A::xxx();

    继承的友元：
        通过友元的友元来调用父类中的私有属性。

    函数shadow行为：
        重载、shadow


类对象：
    class birthday{

        public:

            birthday(int y, int m, int d):year(y),month(m),day(d){}

            void getbirth(){

                cout << y << m << d << endl;

            }

            int year;

            int month;

            int day;
    }

    class person{

        public:

            person(int y, int m, int d):birth(y,m,d){

            }

            birthday birth;

    }


虚基类：
    该类内的成员是从虚继承该类的基类中提取出来的公共成员。他是类设计和提取的结果。

    注意点：
        虚基类的构造器，需要被继承的基类调用、也需要被真正实现的类调用，真正起作用的是，实现的类中调用的过程。

        虚继承之后，在真正实现的类中，就可以直接使用虚基类中的成员。

        eg:
            基类有：沙发、床（其中都有dis方法）

            派生类：沙发床继承了 沙发、床 那么沙发床中就有两个dis方法，那么具体调用哪个呢？就会带来二义性的问题，所以 此时将 dis方法调到虚基类中去，让沙发、床两个基类虚继承之，就可以实现。

        class base{

            public:
                int b;

                base(int tmpb):b(tmpb){}

        }

        class act:virtual public base{

            public:
                act(int c):base(c){}

        }

        class dos:public act{

            public:

                dos(int d):act(d),base(d){}

                void dis(){

                    cout << b << endl

                }

        }

        dos d(10);

        d.dis();


赋值兼容：

    只能运用在public的继承方式中。

    使用情况：
        子类对象赋给父类对象 A a = b;

        子类对象赋给父类对象的引用 A & a = b;

        子类对象的地址赋给赋给父类对象的指针 A *a = &b;



多态：
    实现要求：
        1、父类中有虚函数。（virtual 修饰的函数，在声明中要带着，在实现时去掉）

        2、子类覆写（同名、同参、同返回）父类中的虚函数，覆写具体使用public、protected、private都无所谓，只要父类中是public即可。不管是直接继承，还是覆写，子类还可以被其他子类继承再覆写。

        3、子类的地址赋给父类的指针，并调用虚函数。

        class testvirA{

            public:
                virtual void test(){

                    cout << "aaaaa" << endl;

                }

        };

        class testvirB:public testvirA{

            private:

                virtual void test(){

                    cout << "bbbb" << endl;

                }

        };

        testvirB b;

        testvirA *a = &b;

        a->test();


    什么是纯虚函数：

        虚函数不需要实现，只需要在定义时加个 "= 0"

        eg:
            virtual void func() = 0;

        拥有纯虚函数的类，不能被实例化，只能被继承，在子类中实现，如果子类中也不实现，那么子类继承纯虚函数，子类也就不能实例化，只能被继承。



运行时身份识别：
    typeid、dynamic_cast 只能用在父子类中执行

    1、typeid()使用
        使用typeid()之后，返回一个type_info的对象，里面有个name()，可以拿到信息

        class A{

            public:

                virtual ~A(){};

        };

        class B:public A{



        }

        B b;

        cout << typeid(b).name() << endl;//结果 1 B

        A &a = b;

        if(typeid(a) == typeid(b)){

            cout << "xxx" << endl;

        }


        cout << typeid(a).name() << endl;// 结果 1 B


        A *a= &b;

        if(typeid(*a) == typeid(b)){

            B *tmpb = danmic_cast <B *>(a);

            cout << "yyy" << endl;

        }

        cout << typeid(*a).name() << endl;// 结果 1 B


    2、dynamic_cast 用于在父子类中，将父类转换成子类。


STL：

    int a = 4, b =5;

    double a = 4, b = 5;

    long a = 4, b = 5;



    如果要将a和b的值互相换掉，那么以我们之前学的东西，需要写3个函数重载下去转换，如果用了STL就没有这么多问题了！

    函数模板：（使用前提，函数的参数个数相同，参数类型不同，函数体也相同）

        template<typename T>

        void swap(T &a, T & b){

            T tmp = a;

            a = b;

            b = tmp;

        }

    类模板：(类模板---->模板类---->类对象) 声明和定义不能分到2个文件中去！！！！C++要求模板的声明和实现对引用者必须都可见，模板的声明和实现要放到一个文件里，都写在.h文件里

        template <typename T>

        class test{

            public:
                template <typename M>//如果写在类内，就不用写这句，如果是类外，只要用到T的地方都需要写这句
                void push(M s){//如果写在类内，这样写没问题，如果是类外，需要改为： void test<T>::push(T s)



                }

                template <typename L>
                L pop(){

                    return xxx;

                }

        };

        使用：

            test<xxxx> s("tangwei");

            eg:

                test<int> s("tangwei");



文件读写：
    #include <fstream>

    ①：
        ifstream ifs("abx.txt");//只能从abx.txt中读取数据,读取到第一个空格或者是换行为止。

        或者也可以这样：

        ifstream ifs;

        ifs.open("abx,txt");//自动忽略 ios::in、ios::out等参数

        if(!ifs){

            cout << "open error" << endl;

            exit(0);

        }

        char *a = new char[10];//如果在10个字符以内遇到空格，那么就不满10个字符，自动会在后面加上\0

        ifs >> a;//读取数据

        cout << a << endl;

        ifs << 12 << 13;//不能这么做



    ②：
        ofstream ofs("abx.txt");//可以往abx.txt文件中写数据，文件不存在会默认新建文件，如果文件存在，那么会把文件内的内容清除掉重新写入要写入的东西

        或者也可以这样：

        ofstream ifs;

        ofs.open("abx,txt");//自动忽略 ios::in、ios::out等参数

        if(!ofs){

            cout << "open error" << endl;

        }

        ofs << 12 << 34 << 56 << endl;

        int ab;

        ofs >> ab >> endl;



    ③：
        fstream fs;

        fs.open("abx,txt", ios::in|ios::out);//读写方式打开一个文件，如果没有该文件现在会报错，
                                             //加个ios::in是读取数据
                                             //加个ios::out是写入数据
                                             //加个ios::trunc就创建创建一个空文件，这个参数，如果文件本来就存在，那么会清空里面的数据
                                             //加个ios::app 也会创建一个空文件，如果文件本来就存在，不会清空内容

        if(!fs){

            cout << "open error" <<  endl;

        }

        fs << "xxxx  yyyyyy" << endl;//写入数据到文件

        fs << "xxxx  yyyyyy" << endl;

        fs << "xxxx  yyyyyy" << endl;

        fs << "xxxx  yyyyyy" << endl;

        fs << "xxxx  yyyyyy" << endl;

        读取写入文件方式一：// 读取会遇到空格和换行就截断 写入不会清空数据,在最后一行写入数据。文件不存在不会主动创建。

            fs.seekg(0, ios::beg);

            char buf[1024];

            fs >> buf;//读取数据

            cout << buf << endl;

            fs >> buf;//读取数据

            cout << buf << endl;

            fs >> buf;//读取数据

            cout << buf << endl;

            fs << "ceshiceshi";

            fs.close();


        读取文件方式二：

            fs.seekg(0, ios::beg);

            fs.put();//挨个字符写入到文件

            fs.get();//挨个字符读入到内存

            eg:

                std::fstream fsr("/Users/tangwei/Desktop/twsocket/abc.txt", std::ios::in | std::ios::app);

                std::fstream fsw("/Users/tangwei/Desktop/twsocket/test.txt",  std::ios::out | std::ios::trunc);

                if(!fsw)
                    std::cout << "文件打开失败" << std::endl;

                if(!fsr)
                    std::cout << "文件打开失败1" << std::endl;

                fsr.seekg(0, std::ios::beg);

                char tmp;

                while(!fsr.eof()){

                    fsr.get(tmp);

                    fsw.put(tmp);

                }

                fsw.close();

                fsr.close();



        读取文件方式三：

            fs.seekg(0, ios::beg);

            fsr.getline(tmp, 1024, '\n');

            eg:
                std::fstream fsr("/Users/tangwei/Desktop/twsocket/abc.txt", std::ios::in | std::ios::app);

                std::fstream fsw("/Users/tangwei/Desktop/twsocket/test.txt", std::ios::out | std::ios::trunc);

                if(!fsw)
                    std::cout << "文件打开失败" << std::endl;

                if(!fsr)
                    std::cout << "文件打开失败1" << std::endl;

                fsr.seekg(0, std::ios::beg);

                char tmp[1024];

                while(fsr.getline(tmp, 1024, '\n'), !fsr.eof()){

                    fsw << tmp << std::endl;

                }

                fsw.close();

                fsr.close();

        读取文件方式四：//一块一块地读取文件，用在二进制文件上

            fstream fsw("/Users/tangwei/Desktop/小火车/test/test/test.txt", ios::out | ios::trunc | ios::binary);

                struct te{

                    char *username;

                    int age;

                    float socre;

                };

                te t[3] = {

                    {"tangwei", 28, 99.5},

                    {"litong", 24, 100.0},

                    {"fanbing", 28, 90.5}

                };

                cout << t << endl;

                cout << &t[0] << endl;

                fsw.write((char *)t, sizeof(t));

                fsw.close();

                fstream fsr("/Users/tangwei/Desktop/小火车/test/test/test.txt", ios::in | ios::binary);

                fsr.seekg(0, ios::beg);

                te content;

                while(true){

                    fsr.read((char *)&content, sizeof(te));

                    if(fsr.eof())break;

                    std::cout << content.username << std::endl;

                    std::cout << content.age << std::endl;

                    std::cout << content.socre << std::endl;

                }


                fsr.close();



















                /*
                 * 这儿是获取文件大小的方法
                 */

                std::fstream fsr(middleFile, std::ios::in | std::ios::binary );

                fsr.seekg(0, std::ios::end);

                fileSize = fsr.tellg();

                std::cout << "文件大小：" << fileSize << std::endl;

                /*
                 * 这儿是获取文件大小的方法
                 */






                while(true){//最好是这样去读取文件内容

                    if(fs.eof())break;

                    char tmpbuf[1024] = {0};

                    fs.read(tmpbuf, sizeof(tmpbuf));

                }






















