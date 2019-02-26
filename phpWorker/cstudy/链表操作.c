//
//  链表操作.c
//  cstudy
//
//  Created by tangwei on 17/7/17.
//  Copyright © 2017年 tangwei. All rights reserved.
//

#include <stdio.h>

#include <stdlib.h>

#include <sys/malloc.h>

#include <string.h>

#define tangwei 20

typedef struct kang
{
    char name[30];
    
    struct kang *link;
}tw;

tw *create(n)
{
    if(n < 2){
        
        printf("需要创建的链表的节点数不能少于2");
        
    }
    
    int i = 0;
    
    tw *head, *front, *current;
    
    head = (tw *)malloc(sizeof(tw));
    
    if(head == NULL){
        
        printf("分配给头指针的内存失败");
        
        exit(0);
        
    }
    
    strcpy(head->name, "\0");
    
    head->link = NULL;
    
    front = head;
    
    for (; i < n; i ++) {
        
        current = (tw *)malloc(sizeof(tw));
        
        if(current == NULL){
            
            printf("循环分配链表节点内存失败");
            
            exit(0);
            
        }
        
        printf("请输入第%d位用户的姓名：", i+1);
        
        scanf("%s", current->name);
        
        front->link = current;
        
        current->link = NULL;
        
        front = current;
        
    }
    
    return head;
}

tw *search(tw *head, char *name)
{
    tw *current, *res;
    
    current = head->link;
    
    while (current->link != NULL){
        
        if(strcmp(current->name, name) == 0){
            
            res = current;
            
            break;
            
        }
        
        current = current->link;
        
    }
    
    if(res == NULL){
        
        printf("没有查找到相关姓名");
        
        exit(0);
        
    }
    
    return res;
}


tw *insert(tw *x){
    
    
    tw *new;
    
    new = (tw *)malloc(sizeof(tw));
    
    if(new == NULL){
        
        printf("新增节点分配内存空间失败");
        
        exit(0);
        
    }
    
    printf("请输入要插入的学生的姓名：");
    
    scanf("%s", new->name);
    
    new->link = x->link;
    
    x->link = new;
    
    return new;
    
}

void del(tw *x, tw *y)
{
    x->link = y->link;
    
    free(y);
}

int main(int argc, const char * argv[]) {
    
    tw *head = create(4);
    
    tw *p = search(head, "tangwei");
    
    insert(p);
    
    tw *m = search(head, "hahahahaha");
    
    printf("%s", m->name);
    
}
