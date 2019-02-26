//
//  link.h
//  newBook
//
//  Created by tangwei on 17/7/31.
//  Copyright © 2017年 tangwei. All rights reserved.
//

#ifndef link_h
#define link_h


#endif /* link_h */

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

extern linkNode *headNode;//存放linkNode拿到的头指针

extern fileStatus nowstatus;//存放结构体fileStatus数据

extern void insertOneNode(linkNode *newNode);

extern void delOneNode(linkNode *newNode, int isDelete);

extern linkNode *searchNode(char *name);

extern int insertIntoFile(FILE *file, linkNode *current);

extern int deleteOneFromFile(FILE *file, int delePos);

extern int modifyOne(FILE *file, linkNode *newNode);

extern int compareFile(FILE *file);

extern char showMessage(char *str);

extern int insertPerson(FILE *file);

extern int deletePerson(FILE  *file, linkNode *nowNode);

extern int modifyPerson(FILE *file, linkNode *nowNode);

extern void showPerson(FILE *file, linkNode *nowNode);

extern void searchPerson(FILE *file);

extern void inital(FILE *file);

extern int menu();










