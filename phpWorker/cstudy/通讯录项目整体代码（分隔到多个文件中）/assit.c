//
//  assit.c
//  newBook
//
//  Created by tangwei on 17/7/31.
//  Copyright © 2017年 tangwei. All rights reserved.
//

#include "link.h"

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

