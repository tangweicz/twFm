

/*
 项目开发过程：将原本放在一个文件里面的代码 分隔到不同的文件中去
 
 一个头文件link.h 1、数据结构的定义 2、函数原型的声明 3、全局变量的引用和宏定义
 一个对链表进行操作的文件，linkTable.c
 一个存放文件进行操作函数的文件，file.c
 存放辅助函数的文件，assit.c
 存放主函数和全局变量，index.c。即这儿存放main函数
 
 */
#include "link.h"

linkNode *headNode;//存放linkNode拿到的头指针

fileStatus nowstatus;//存放结构体fileStatus数据

int main(int argc, const char * argv[]) {
    
    
    FILE *file = fopen(filename, "rb+");
    
    if(file == NULL){
        
        printf("打开文件失败\r\n");
        
        exit(0);
        
    }
    
    inital(file);
    
    //FILE *file = fopen(filename, "w+");
    
    while(1){
        
        char res = menu();
        
        switch (res) {
            case 1:
                insertPerson(file);
                break;
                
            case 2:
                deletePerson(file, NULL);
                break;
                
            case 3:
                searchPerson(file);
                break;
                
            case 6:
                return 0;
                break;
                
            default:
                break;
        }
        
    }
    
}





























