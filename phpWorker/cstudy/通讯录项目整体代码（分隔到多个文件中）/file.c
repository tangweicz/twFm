//
//  file.c
//  newBook
//
//  Created by tangwei on 17/7/31.
//  Copyright © 2017年 tangwei. All rights reserved.
//

#include "link.h"

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

