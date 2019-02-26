//
//  linkNode.c
//  newBook
//
//  Created by tangwei on 17/7/31.
//  Copyright © 2017年 tangwei. All rights reserved.
//

#include "link.h"

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
