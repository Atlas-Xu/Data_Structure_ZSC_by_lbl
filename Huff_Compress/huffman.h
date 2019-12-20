#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <limits.h>

#define N 256
#define M 512

typedef struct {
	unsigned char weight;//字节符
	long count;          //字节频率
	long parent;         //父
	long lch, rch;       //左右孩子
	char bits[256];      //结点的huffman编码
}HuffNode;

HuffNode HuffmanTree[M];
typedef char **HuffmanCode;  //char**可以理解为字符串数组 haffman编码

void UnCompress();
void welcome();
void Compress();  //文件压缩