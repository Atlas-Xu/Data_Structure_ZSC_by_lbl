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
	unsigned char weight;//�ֽڷ�
	long count;          //�ֽ�Ƶ��
	long parent;         //��
	long lch, rch;       //���Һ���
	char bits[256];      //����huffman����
}HuffNode;

HuffNode HuffmanTree[M];
typedef char **HuffmanCode;  //char**�������Ϊ�ַ������� haffman����

void UnCompress();
void welcome();
void Compress();  //�ļ�ѹ��