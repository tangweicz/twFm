//
//  文件操作.c
//  cstudy
//
//  Created by tangwei on 17/7/19.
//  Copyright © 2017年 tangwei. All rights reserved.
//

#include <stdio.h>

#include <stdlib.h>

#include <sys/file.h>

#include <sys/malloc.h>

#define true 1

int main(){
    
    //写入单个字符到文件
    
    char a = 'A';
    
    char *filename = "/etc/hahaha.txt";
    
    FILE *fd = fopen(filename, "w");
    
    int res = fputc(a, fd);
    
    if(res == -1){
    
        printf("写入数据失败");
        
        return 0;
        
    }
    
    printf("%d", res);
    
    
    //文件单个字符读入到内存
    
    char *filename = "/etc/hahaha.txt";
    
    FILE *file;
    
    file = fopen(filename, "r");
    
    while(true){
        
        char a = fgetc(file);
        
        if(a == -1){
        
            printf("到达文件末尾\r\n");
            
            break;
            
        }
    
        printf("%c \r\n", a);
        
    }
    
    //将字符串追加写入文件
    
    char *filename = "/etc/hahaha.txt";
    
    FILE *file;
    
    char *str[] = {"tangweikang", "kangkang", "hahaha"};
    
    file = fopen(filename, "a");
    
    for(int i = 0; i<=2; i++){
    
        int b = fputs(str[i], file);
        
        if(b != 0){
        
            printf("循环到第%d正常", i+1);
            
        }else{
        
            printf("循环到第%d个出错", i+1);
            
            break;
            
        }
        
    }
    
    //从文件读取一个字符串到内存
    
    char *filename = "/etc/hahaha.txt";
    
    FILE *file;
    
    char *str = (char *)malloc(8*sizeof(char));
    
    file = fopen(filename, "r");
    
    if(file == NULL){
        printf("打开文件失败");
        
        exit(0);
    }
    
    while(fgets(str, 8*sizeof(char), file) != NULL){
        
        printf("%s \r\n", str);
        
    }
    
    //将数据块(数组)写入到文件中并读出
    
    int a[] = {1,2,3,4,5};
    
    FILE *file;
    
    char *filename = "/Users/tangwei/WebServer/cstudy/cstudy/hahaha.txt";
    
    file = fopen(filename, "w");
    
    long num = fwrite(a, sizeof(a), 1, file);
    
    if(num == 0){
    
        printf("数据写入错误");
        
        exit(0);
        
    }
    
    int b[5];
    
    fclose(file);
    
    file = fopen(filename, "r");
    
    fread(b, sizeof(b), 1, file);
    
    for(int i = 0; i<=4; i++){
    
        printf("%d \r\n", b[i]);
        
    }
    
    fclose(file);
    
    //将数据块（结构体）写入到文件中并读出
    
    typedef struct tangwei{
        
        char *name;
        
        int age;
        
        float score;
        
    }tw;
    
    tw *kangkang = (tw *)malloc(sizeof(tw));
    
    if(kangkang == NULL){
        
        printf("初始化构造类型失败");
        
        exit(0);
        
    }
    
    kangkang->name = "tangwei";
    
    kangkang->age = 28;
    
    kangkang->score = 99.5;
    
    FILE *file;
    
    char *filename = "/Users/tangwei/WebServer/cstudy/cstudy/hahaha.txt";
    
    file = fopen(filename, "w");
    
    long num = fwrite(kangkang, sizeof(tw), 1, file);
    
    if(num <= 0){
        
        printf("写入文件失败");
        
        exit(0);
        
    }
    
    fclose(file);
    
    FILE *file2;
    
    file2 = fopen(filename, "r");
    
    tw *p = (tw *)malloc(sizeof(tw));
    
    fread(p, sizeof(tw), 1, file2);
    
    printf("%d", p->age);
    
    printf("%s", p->name);
    
    printf("%f", p->score);
    
    
    
    
    
}
