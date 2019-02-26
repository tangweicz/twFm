

/*
 项目开发过程：将原本放在一个文件里面的代码 分隔到不同的文件中去
 
 一个头文件link.h 1、数据结构的定义 2、函数原型的声明 3、全局变量的引用和宏定义
 一个对链表进行操作的文件，linkTable.c
 一个存放文件进行操作函数的文件，file.c
 存放辅助函数的文件，assit.c
 存放主函数和全局变量，index.c。即这儿存放main函数
 
 */


#include <stdio.h>

#include <sys/file.h>

#include <stdlib.h>

#include <sys/malloc.h>

#include <string.h>

#define personSize sizeof(person)

#define linkNodeSize sizeof(linkNode)

#define statusSize sizeof(fileStatus)

#define filename "/Users/tangwei/WebServer/cstudy/cstudy/test.txt"

typedef struct{//用户信息存放的结构体
    
    char username[10];
    
    char mobile[12];
    
    char homeAddr[30];
    
}person;


typedef struct linkNodes{//双向链表结构体
    person personInfo;
    
    struct linkNodes *frontend, *backend;
    
    int pos;
    
}linkNode;


typedef struct{//存放用户信息的数据文件的状态结构体
    
    int totalNum;//当前联系人总数
    
    int rubbishNum;//当前文件中有多少的垃圾数据
    
}fileStatus;

linkNode *headNode;//存放linkNode拿到的头指针

fileStatus nowstatus;//存放结构体fileStatus数据

//链表处理函数

//插入一个链表节点
void insertOneNode(linkNode *newNode){//插入数据时，对要存储的用户进行姓名排序，从而决定他插入的位置
    
    if(headNode == NULL || strcmp(headNode->personInfo.username, newNode->personInfo.username) > 0){//要插到头指针的情况
        
        if(headNode == NULL){//在插入之前是个空链表
            
            headNode = newNode;
            
            headNode->backend = NULL;
            
            headNode->frontend = NULL;
            
        }else{//不是空链表
            
            newNode->backend = headNode;
            
            newNode->frontend = NULL;
            
            headNode->frontend = newNode;
            
            headNode = newNode;
            
        }
        
    }else{
        
        linkNode *currentNode;
        
        currentNode = headNode;
        
        while(currentNode->backend != NULL){//循环整个链表，查找插入点
            
            currentNode = currentNode->backend;
            
            if(strcmp(currentNode->personInfo.username, newNode->personInfo.username) >= 0){
                
                break;
                
            }
            
        }
        
        if(currentNode->backend == NULL){//如果没有寻找到插入点，那么肯定是插入到尾节点
            
            currentNode->backend = newNode;
            
            newNode->frontend = currentNode;
            
            newNode->backend = NULL;
            
        }else{
            
            linkNode *frontNode;
            
            frontNode = currentNode->frontend;
            
            frontNode->backend = newNode;
            
            newNode->frontend = frontNode;
            
            newNode->backend = currentNode;
            
            currentNode->frontend = newNode;
            
        }
        
    }
    
}

void delOneNode(linkNode *newNode, int isDelete)//该函数有3个功能，第一个功能是如果newNode为空，则删除所有的节点，第二个功能是如果isDelete为1则调用free()将该指针删除，第三个功能是如果isDelete为0则是将该指针脱离原有的顺序重新插位置。
{
    
    if(newNode == NULL){
        
        while(headNode != NULL){
            
            linkNode *tmpNode = headNode->backend;
            
            free(headNode);
            
            headNode = tmpNode;
            
        }
        
    }else if(newNode->frontend == NULL){//如果要删除的是头结点
        
        if(newNode->backend != NULL){
            
            headNode = newNode->backend;
            
            headNode->frontend = NULL;
            
        }
        
        if(isDelete == 1){
            
            free(newNode);
            
        }
        
        
    }else{
        
        if(newNode->backend != NULL){//不是尾节点
            
            newNode->frontend->backend = newNode->backend;
            
            newNode->backend->frontend = newNode->frontend;
            
            if(isDelete == 1){
                
                free(newNode);
                
            }
            
        }else{//是尾节点
            
            newNode->frontend->backend = NULL;
            
            if(isDelete == 1){
                
                free(newNode);
                
            }
            
            
        }
        
    }
    
}

linkNode *searchNode(char *name){
    
    linkNode *currentNode = headNode;
    
    int findIt = 0;
    
    while (currentNode != NULL) {
        
        if( strcmp(name, currentNode->personInfo.username) == 0){
            
            findIt = 1;
            
            break;
            
        }
        
        currentNode = currentNode->backend;
    }
    
    if(findIt == 0){
        
        currentNode = NULL;
        
    }
    
    return currentNode;
    
}






//文件处理函数

int insertIntoFile(FILE *file, linkNode *current){//插入数据到文件中
    
    fseek(file, (linkNodeSize*nowstatus.totalNum)+statusSize, SEEK_SET);
    
    current->pos = nowstatus.totalNum + 1;
    
    if(fwrite(current, linkNodeSize, 1, file) != 1){
        
        printf("文件写入失败");
        
        return 0;
        
    }
    
    setbuf(file,NULL);
    
    nowstatus.totalNum ++;
    
    fseek(file, 0, SEEK_SET);
    
    if(fwrite(&nowstatus, statusSize, 1, file) != 1){
        
        printf("写入文件失败");
        
        return 0;
        
    }
    
    setbuf(file,NULL);
    
    if(nowstatus.rubbishNum > 0)nowstatus.rubbishNum --;
    
    return 1;
}


int deleteOneFromFile(FILE *file, int delePos){//删除文件中的一条数据
    
    if(delePos == 0){//为0是删除所有的数据，那么只要把文件删掉，重新创建一个即可
        
        nowstatus.totalNum = 0;
        
        nowstatus.rubbishNum = 0;
        
        if((file = freopen(filename, "wb", file)) == NULL){
            
            printf("文件创建失败");
            
            return 0;
            
        }
        
        return 1;
        
    }else{
        
        if(delePos == nowstatus.totalNum){//如果要删掉的是文件最后一个数据
            
            
            //
            
            
        }else{//如果要删掉的 不是文件最后一个数据
            linkNode *lastNode = headNode;
            
            while(lastNode->backend != NULL){
                
                if(lastNode->pos == nowstatus.totalNum){
                    
                    break;
                    
                }
                
                
                lastNode = lastNode->backend;
                
                
            }
            
            
            fseek(file, (delePos -1)*linkNodeSize+statusSize, SEEK_SET);
            
            if( fwrite(lastNode, linkNodeSize, 1, file) != 1){
                
                printf("写入数据失败");
                
                return 0;
                
            }
            lastNode->pos = delePos;
            
            fseek(file, 0, SEEK_SET);
            
            nowstatus.totalNum --;
            
            nowstatus.rubbishNum ++;
            
            fwrite(&nowstatus, statusSize, 1, file);
            
            setbuf(file,NULL);
            
        }
        
    }
    
    return 1;
    
}

int modifyOne(FILE *file, linkNode *newNode){//其实在外面已经修改了，只要直接覆盖到相应的位置就行
    
    fseek(file, (newNode->pos - 1 ) * linkNodeSize + statusSize, SEEK_SET);
    
    if(fwrite(newNode, linkNodeSize, 1, file) != 1){
        
        printf("写入文件失败");
        
        return 0;
        
    }
    
    return 1;
    
}

int compareFile(FILE *file){
    
    linkNode *nowNode = headNode;
    
    FILE *newFile = freopen(filename, "wb", file);
    
    if(newFile != NULL){
        
        printf("创建文件失败");
        
        return 0;
        
    }
    
    nowstatus.rubbishNum = 0;
    
    if(fwrite(&nowstatus, statusSize, 1, file) == 1){
        
        while (nowNode != NULL) {
            fseek(newFile, statusSize+(linkNodeSize*(nowNode->pos-1)), SEEK_SET);
            
            fwrite(nowNode, linkNodeSize, 1, file);
            
            nowNode = nowNode->backend;
        }
        
    }else{
        
        printf("写入文件失败");
        
        return 0;
        
    }
    
    return 1;
    
}




//辅助函数

char showMessage(char *str){//显示界面信息
    
    printf("\r\n%s", str);
    
    setbuf(stdin, NULL);//每一次调用之前，先把标准输入中的数据清空
    
    char a;
    
    scanf("%[^\n]", &a);
    
    return a;
    
}

int insertPerson(FILE *file)//添加联系人
{
    linkNode *newNode;
    
    newNode = (linkNode *)malloc(linkNodeSize);
    
    if(newNode == NULL){
        
        printf("初始化链表空间失败");
        
        return 0;
        
        
    }
    
    setbuf(stdin, NULL);
    
    printf("\r\n请输入用户姓名：");
    
    scanf("%s", newNode->personInfo.username);
    
    setbuf(stdin, NULL);
    
    printf("\r\n请输入用户家庭地址：");
    
    scanf("%s", newNode->personInfo.homeAddr);
    
    setbuf(stdin,NULL);
    
    printf("\r\n请输入用户手机号：");
    
    scanf("%s", newNode->personInfo.mobile);
    
    setbuf(stdin,NULL);
    
    
    char isSave = showMessage("\r\n是否存储数据，y/n");
    
    if( isSave != 'n'){
        
        int res = insertIntoFile(file, newNode);
        
        if(res){
            
            insertOneNode(newNode);
            
            showMessage("插入用户成功，按ENTER键返回");
            
        }else{
            
            showMessage("\r\n数据存储到文件失败");
            
            return 0;
            
        }
        
        
    }else{
        
        printf("\r\n您已取消存储");
        
        return 0;
        
    }
    
    return 1;
}

int deletePerson(FILE  *file, linkNode *nowNode){//删除联系人
    
    char *message;
    
    int pos;
    
    message = (char *)malloc(30 * sizeof(char));
    
    if(nowNode == NULL){//删除所有的，情况
        
        sprintf(message, "您确定要删除所有的数据么？");
        
        pos = 0;
        
    }else{
        
        sprintf(message, "您确定要删除%s位置的数据么？", nowNode->personInfo.username);
        
        pos = nowNode->pos;
        
    }
    
    if(showMessage(message) == 'y'){
        
        int res = deleteOneFromFile(file, pos);
        
        if(res){
            
            delOneNode(nowNode, 1);
            
            
            showMessage("删除成功，按ENTER键返回");
            
            return 1;
        }
        
    }else{
        
        printf("您取消了删除操作，按ENTER键返回");
        
        return 0;
        
    }
    
    return 1;
    
}


int modifyPerson(FILE *file, linkNode *nowNode){//修改联系人
    
    printf("\r\n---------******--------------");
    
    printf("\r\n当前用户信息：");
    
    printf("\r\n用户姓名：%s", nowNode->personInfo.username);
    
    printf("\r\n用户手机：%s", nowNode->personInfo.mobile);
    
    printf("\r\n用户地址：%s", nowNode->personInfo.homeAddr);
    
    printf("\r\n---------******--------------");
    
    setbuf(stdin, NULL);
    
    char r = showMessage("\r\n请输入您要修改的内容，1为用户姓名 2为手机号 3为家庭地址：");
    
    printf("%c", r);
    
    switch (r) {
        case '1':
            printf("\r\n用户姓名：");
            
            scanf("%s", nowNode->personInfo.username);
            
            setbuf(stdin, NULL);
            
            break;
            
        case '2':
            printf("\r\n用户手机号：");
            
            scanf("%s", nowNode->personInfo.mobile);
            
            setbuf(stdin, NULL);
            break;
            
        case '3':
            printf("\r\n用户地址：");
            
            setbuf(stdin, NULL);
            
            scanf("%s", nowNode->personInfo.homeAddr);
            break;
            
        default:
            break;
    }
    
    modifyOne(file, nowNode);
    
    if(r == '1'){
        
        delOneNode(nowNode, 0);
        
        insertOneNode(nowNode);
        
    }
    setbuf(stdin, NULL);
    
    printf("修改成功，按ENTER键返回");
    
    return 1;
    
}

void showPerson(FILE *file, linkNode *nowNode)
{
    printf("\r\n---------******--------------");
    
    printf("\r\n当前用户信息：");
    
    printf("\r\n总共有%d位联系人，您是第%d位联系人：", nowstatus.totalNum, nowNode->pos);
    
    printf("\r\n用户姓名：%s", nowNode->personInfo.username);
    
    printf("\r\n用户手机：%s", nowNode->personInfo.mobile);
    
    printf("\r\n用户地址：%s", nowNode->personInfo.homeAddr);
    
    printf("\r\n---------******--------------");
    
    char res = showMessage("\r\n 1删除 2修改 3返回 4上一位 任意键下一位 请选择：");
    
    switch (res) {
        case '1':
            if(deleteOneFromFile(file, nowNode->pos)){
                
                deletePerson(file, nowNode);
                
                if(nowNode->frontend != NULL){
                    
                    showPerson(file, nowNode->frontend);
                    
                }else{
                    
                    if(nowNode->backend != NULL){
                        
                        showPerson(file, nowNode->backend);
                        
                    }
                    
                }
                
            }else{
                
                
                showPerson(file, nowNode);
                
            }
            
            
            break;
            
            
        case '2':
            
            setbuf(stdin, NULL);
            
            modifyPerson(file, nowNode);
            
            showPerson(file, nowNode);
            
        case '3':
            
            return;
            
        case '4':
            if(nowNode->frontend != NULL){
                
                showPerson(file, nowNode->frontend);
                
            }
            
            break;
            
            
        default:
            if(nowNode->backend != NULL){
                
                showPerson(file, nowNode->backend);
                
            }
            
            break;
    }
    
    
}

void searchPerson(FILE *file){
    
    printf("\r\n请输入要查找的用户名：");
    
    setbuf(stdin,NULL);
    
    char username[20];
    
    scanf("%s", username);
    
    linkNode *newNode = searchNode(username);
    
    if(newNode != NULL){
        
        showPerson(file, newNode);
        
    }else{
        
        printf("没有找到该联系人");
        
    }
    
}

void inital(FILE *file){//当系统重启之后，内存中的数据没有了，那么我需要把文件中的数据加载到内存中
    long res = fread(&nowstatus, statusSize, 1, file);
    
    if(res == 1){
        
        if (nowstatus.totalNum != 0) {
            
            for (int i=1; i <=nowstatus.totalNum; i++) {
                
                linkNode *node = (linkNode *)malloc(linkNodeSize);
                
                if(node == NULL){
                    
                    printf("初始化内存空间失败");
                    
                    exit(0);
                    
                }
                
                long r = fread(node, linkNodeSize, 1, file);
                
                if(r == 1){//读取成功
                    
                    node->pos = i;
                    
                    insertOneNode(node);
                    
                }else{
                    
                    printf("文件读取失败！");
                    
                    free(node);
                    
                    break;
                    
                }
                
            }
            
        }
        
    }else{
        
        printf("读取文件状态信息失败");
        
        exit(0);
    }
    
}

//主功能函数，负责调用以上函数
int menu(){
    int a;
    
    setbuf(stdin,NULL);
    
    while (1) {
        
        printf("\r\n--------------****************------------------");
        
        printf("\r\n--------------****电话本软件****------------------");
        
        printf("\r\n\t\t 1.添加姓名      2.删除全部姓名\r\n\r\n");
        
        printf("\t\t 3.查找姓名      4.显示姓名\r\n\r\n");
        
        printf("\t\t 5.数据整理      6.退出系统\r\n\r\n");
        
        printf("\r\n--------------****************------------------\r\n");
        
        printf("请选择：");
        
        scanf("%d", &a);
        
        if(a >6 || a < 1){
            
            showMessage("您的输入不正确，按ENTER键返回");
            
        }else{
            
            break;
            
        }
        
    }
    
    
    return a;
    
    
}

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





























