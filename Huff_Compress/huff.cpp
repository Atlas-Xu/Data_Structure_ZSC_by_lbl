#define _CRT_SECURE_NO_WARNINGS
#include "huffman.h"
#include<conio.h>//控制台的输入输出读取
#include<iostream>
using namespace std;

void Compress()
{
	int i, j;
	FILE *ifile,*ofp;        //指向FILE对象指针
	char filename[N];
	cout << "\t输入欲压缩图片地址及文件名：";
	gets_s(filename);        //获取用户输入的文件路径
	cout << endl;
	ifile = fopen(filename, "rb"); //二进制流打开文件

	if (ifile == NULL)       //出错处理--找不到文件
	{
		cout << "\t打开文件失败" << endl << "请重新输入：";
		scanf("%s", filename);
		fflush(stdin);		// 清空标准输入流，防止干扰gets函数读取文件名
		ifile = fopen(filename, "rb");
		if (ifile == NULL)
			welcome();
	}

	unsigned char Bmp_char; //二进制读取中文件【0-255的】字符
	int length = 0; //长度（统计用）
	int flength; //文件长度值
	//读取文件并统计文件各类型字符出现次数
	while (!feof(ifile)) //feof检测到文件结束符，返回0
	{
		fread(&Bmp_char, 1, 1, ifile);//逐字节读入文件 ，指向Bmp_char的内存地址
		HuffmanTree[Bmp_char].count++;//该Bmp_char的字节频率+1
		length++; //当前读取文件长度
	}//读取文件
	length--;   //最后一个字节多计算了一次，'\0'再算一次
	flength = length;  //文件总长
	HuffmanTree[Bmp_char].count--;  //最后一个字节多计算了一次

	//初始化二叉树
	for (i = 0; i < M; i++) 
	{
		if(HuffmanTree[i].count !=0)
			HuffmanTree[i].weight = (unsigned char)i;//如果有权值，设置叶子结点的字节符
		else     
			HuffmanTree[i].weight = 0;
		//对结点进行初始化，所有结点的父结点都不存在，左右孩子都不存在
		HuffmanTree[i].parent = -1;
		HuffmanTree[i].lch = -1;
		HuffmanTree[i].rch = -1;
	}
	//将前256个元素根据[weight（权值）]大小排序
	//在此处运用排序将结点按权值从大到小排序(简单选择排序)
	HuffNode temp;//临时变量结构体
	for (i = 0; i < N; i++)
	{
		for (j = i + 1; j < N; j++)//当a=1，a[1]与a[2~n]比较，另a[1]最小
		{
			if (HuffmanTree[i].count < HuffmanTree[j].count)
			{
				temp = HuffmanTree[i];
				HuffmanTree[i] = HuffmanTree[j];
				HuffmanTree[j] = temp;
			}
		}
	}
	
	int n, m;
	for (i = 0; i < N; i++)
	{
		if (HuffmanTree[i].count == 0)//读入文件时该字符出现频率为零的话，去掉
			break;
	}
	n = i;          //n个叶子节点
	m = 2 * n - 1;  //m为哈夫曼树的总结点数
	//n = i;          //第n个叶子节点下标
	//m = 2 * n + 1;  //m为哈夫曼树的总结点数2*（n+1）-1 

	//生成哈夫曼树
	long maxt, pt;
	cout << "输出节点信息:\n";
	for (i = n; i < m; i++)  //构建哈夫曼树的剩下的非叶子结点
	{
		maxt = LONG_MAX;    //预设的最大权值，即结点出现的最大次数
		for (j = 0; j < i; j++)    //选出权值最小的结点
		{
			//if (HuffmanTree[j].parent != -1)  //parent!=-1说明该结点已在哈夫曼树中（初始的都是-1），跳出循环选择下一个节点 
				//continue;
			//下面都是parent= -1
			if (HuffmanTree[j].parent == -1 && maxt > HuffmanTree[j].count)   //迷
			{
				pt = j;
				maxt = HuffmanTree[j].count;
				//continue;
			}
		}
		//用上面取出最小的做为哈夫曼树的左结点，设置好相关信息
		cout << "第 " << j<< " 节点";
		HuffmanTree[i].count = HuffmanTree[pt].count;
		HuffmanTree[pt].parent = i;    //设置第i个结点左孩子的父结点信息
		HuffmanTree[i].lch = pt;       //设置第i个结点的左孩子
		cout << "\t左孩子：" << HuffmanTree[i].lch ;
		maxt = LONG_MAX;
		for (j = 0; j < i; j++)   //同样的方式求出次小值，此时最小值的parent已不为-1
		{
			if (HuffmanTree[j].parent == -1 && maxt > HuffmanTree[j].count)   //迷
			{
				pt = j;
				maxt = HuffmanTree[j].count;
				//continue;
			}
		}
		
		HuffmanTree[i].count += HuffmanTree[pt].count;
		HuffmanTree[pt].parent = i;  //设置第i个结点右孩子的父结点信息
		HuffmanTree[i].rch = pt;     //设置第i个结点的右孩子
		cout << "\t右孩子：" << HuffmanTree[i].rch << endl;

}//循环结束=建树完成 QAQ
	cout << "\n\n输出结点信息\n";
 //Huffman编码
 int f;
 for (i = 0; i < n; i++)
 {
	 f = i;
	 HuffmanTree[i].bits[0] = 0;  //ASCll码为0的字符，即为\0结束符
	 while (HuffmanTree[f].parent != -1)//当有父亲节点的时候
	 {
		 j = f;  //此时f等于循环i
		 f = HuffmanTree[f].parent;  //判断当前结点与父节点的关系
		 if (HuffmanTree[f].lch == j)   //置分支编码0
		 {
			 j = strlen(HuffmanTree[i].bits);
			 //拷贝，留出位置放当前的编码
			 //j+1意味着拷贝时把\0复制，menmove 不出现重叠部分被覆盖
			 memmove(HuffmanTree[i].bits + 1, HuffmanTree[i].bits, j + 1);//j+1是HuffmanTree[i].bits + 1长度，HuffmanTree[i].bits+1向后移一位，空出
			 //依次存储连接“0”“1”编码
			 HuffmanTree[i].bits[0] = '0';//左分支记为0
		 }
		 else //置右分支编码1
		 {
			 j = strlen(HuffmanTree[i].bits);
			 memmove(HuffmanTree[i].bits + 1, HuffmanTree[i].bits, j + 1);
			 HuffmanTree[i].bits[0] = '1';
			 //结合左分支的处理，完成右分支的处理
		 }
	 }

 }
/*
	HuffmanCode hc;
	int start, c, f;
	hc = (HuffmanCode)malloc((n + 1) * sizeof(char*));
	char* cd = (char*)malloc(n * sizeof(char));
	cd[n - 1] = '\0';
	for (i = 0; i < n; i++)
	{
		start = n - 1;
		for (c = i, f = HuffmanTree[i].parent; f != -1; c = f, f = HuffmanTree[f].parent)
		{
			--start;
			if (HuffmanTree[f].lch == c)
				cd[start] = '0';
			else
				cd[start] = '1';
		}
		hc[i] = (char*)malloc((n - start) * sizeof(char));
		//strcpy(hc[i], &cd[start]);
		strcpy(HuffmanTree[i].bits, &cd[start]);
	}
	*/
	for (int i = 0; i < n; i++)
		cout << hex << "0x" << (unsigned int)(HuffmanTree[i].weight) << "\t" << dec << HuffmanTree[i].count << "\t" << dec << HuffmanTree[i].parent << "\t" << dec <<HuffmanTree[i].lch << "\t" << dec << HuffmanTree[i].rch << "\t" << HuffmanTree[i].bits << endl;
	//cout << hex << "0x" << (unsigned int)(HuffmanTree[i].weight) << "\t" << hc[i] << endl;
	//cout << hex << "0x" << (unsigned int)(HuffmanTree[i].weight) << "\t" << HuffmanTree[i].bits << endl;
	
	//free(cd);
	
	//FILE *ifile;

	//压缩文件
	char wenjian[M];//定义输出文件名
	printf("\n输入压缩后的文件名:");
	fflush(stdin);		// 清空标准输入流，防止干扰gets函数读取文件名
	scanf("%s", wenjian);
	ofp = fopen(wenjian, "wb");//打开文件
	while (ofp == NULL)
	{
		printf("\t重新输入压缩后的文件");
		ofp = fopen(wenjian, "wb");

	}
	fseek(ifile, 0, SEEK_SET);//SEEK_SET指文件头，将文件指针指向待压缩文件的开始位置
	fwrite(&length, sizeof(int), 1, ofp);//在压缩文件头写入原文件的总长度，占2个字节
	fseek(ofp, 8, SEEK_SET);//重定位压缩文件指针，从头偏移8个字节，留出空间写其它信息，并为写入哈夫曼编码准备
	char buf[512];//定义缓冲区，保存字节的Huffman编码
	buf[0] = 0;//初始为'\0'，'\0'表示的是结尾
	f = 0;//统计文件的长度，哈夫曼编码从第8个字节开始写入
	pt = 8;
	while (!feof(ifile))//扫描原文件
	{
		
		//从文件中读取一个字符，读取一个字符后，光标位置后移一个字节
		fread(&Bmp_char, 1, 1, ifile);//逐字节读入文件 ，指向Bmp_char的内存地址
		f++;//计字节
		for (i = 0; i < n; i++)
		{
			//找到取出字符对应哈夫曼树中叶子结点，并得到相应的下标去找相应的编码
			if (Bmp_char == HuffmanTree[i].weight)
				break;
		}
		strcat(buf, HuffmanTree[i].bits);//找到当前字符的哈夫曼编码，连接到buf中
										 //接下
										 //若长度大于8，进行拆分写入，若小于8，则继续取下一个字符的哈夫曼码凑一个字节，凑满写入
		j = strlen(buf);//统计该字符编码的长度
		Bmp_char = 0;
		while (j >= 8)//若当前编码的长度大于等于8，则进行拆分，分两个字节存，如1100101010拆分成11001010，10
		{
			for (i = 0; i < 8; i++)
			{
				if (buf[i] == '1')
					Bmp_char = (Bmp_char << 1) | 1;
				else Bmp_char = Bmp_char << 1;
			}//每8个组成一个c
			fwrite(&Bmp_char, 1, 1, ofp);
			//把凑好的一个字节编码写入文件，写入文件头发盘（写之前记得把文件以二级制的方式打开），
			//一个字节，一个数据，值为c的内容，请用上面介绍的fwrite函数完成
			pt++;//文件长度
			strcpy(buf, buf + 8);//把buf后一个字节起的所有内容复制到buf中，即一个字节一个字节取
			j = strlen(buf);//（j=j-8）
							//把
		}

		if (f == flength)//若源文件所有的字符取完，结束
			break;
	}
	//统计压缩后文件长度（）
	if (j > 0)
	{
		strcat(buf, "00000000");
		for (i = 0; i < 8; i++)
		{
			if (buf[i] == '1')
				Bmp_char = (Bmp_char << 1) | 1;
			else
				Bmp_char = Bmp_char << 1;
		}
		fwrite(&Bmp_char, 1, 1, ofp);//把最后一个字节写入文件中
		pt++;
	}
	//接上
	fseek(ofp, 4, SEEK_SET);//移动文件指针位置到第4个字节
	fwrite(&pt, sizeof(long), 1, ofp);//写入统计压缩后文件的长度，4个字节
	fseek(ofp, pt, SEEK_SET);//移动文件指针到压缩后文件尾
	fwrite(&n, sizeof(long), 1, ofp);//写入节点数目，即总的不同字节的个数

	for (i = 0; i < n; i++)
	{
		fwrite(&(HuffmanTree[i].weight), 1, 1, ofp);//写入每个节点的代表的字符
		Bmp_char = strlen(HuffmanTree[i].bits);
		fwrite(&Bmp_char, 1, 1, ofp);//写入每个字符哈夫曼编码的长度
		j = strlen(HuffmanTree[i].bits);//统计哈夫曼长度
		if (j % 8 != 0)//若存储的位数不是8的倍数，则补0
		{
			for (f = j % 8; f < 8; f++)
				strcat(HuffmanTree[i].bits, "0");//001 00000    01010101 10000000

		}
		//将哈夫曼编码字符串字符串变成二进制
		while (HuffmanTree[i].bits[0] != 0)//这里检查是否到了字符串末尾
		{
			Bmp_char = 0;
			for (j = 0; j < 8; j++)//字符的有效存储不超过8位，则对有效位数左移实现补0
			{
				if (HuffmanTree[i].bits[j] == '1')
					Bmp_char = (Bmp_char << 1) | 1;
				else
					Bmp_char = Bmp_char << 1;
			}
			strcpy(HuffmanTree[i].bits, HuffmanTree[i].bits + 8);//继续转换后面的字符串
			fwrite(&Bmp_char, 1, 1, ofp);
		}
	}

	long length2 = pt--;//压缩后的文件大小
	double div = (double)length2 / (double)flength;//计算文件的压缩率
	fclose(ifile);//关闭文件
	fclose(ofp);
	printf("\n压缩文件成功\n");
	printf("压缩率为 %f%%\n", div * 100);
}

	/*ifile = fopen(filename, "rb");
	while (!feof(ifile)) //feof检测到文件结束符，返回0
	{

		fread(&Bmp_char, 1, 1, ifile);//逐字节读入文件 ，指向Bmp_char的内存地址
		cout<<HuffmanTree[Bmp_char].bits;//该Bmp_char的字节频率+1
		
	}

	fclose(ifile);

	
}*/

//解压缩
void UnCompress()
{
	char filename[255], outpufile[255], buf[255], bx[255];
	//bx保存最长编码的01串，有可能是一个字符，也有可能是多个字符
	//将f转换为二进制表示的自负串，如10011
	unsigned char c;
	HuffNode tmp;
	long i, j, m, n, f, p, l;
	long flength;
	FILE *ifp, *ofp;
	printf("\t请你输入需要解压缩的文件名:");
	gets_s(filename);
	ifp = fopen(filename, "rb");
	if (ifp == NULL)
	{
		printf("\n\t文件打开失败\n");
		return;
	}
	printf("\t将在当前目录下解压，请您输入解压缩后的文件名:");
	gets_s(outpufile);
	ofp = fopen(outpufile, "wb");
	if (ofp == NULL)
	{
		printf("\t解压缩文件失败");
		return;
	}


	//生成Huffman树和Huffman编码表
	fread(&flength, sizeof(long), 1, ifp);//读取未压缩时原文件长度
	fread(&f, sizeof(long), 1, ifp);//读取压缩文件的长度，位于第4个字节处
	fseek(ifp, f, SEEK_SET);//将文件指针定位到存储节点总数
	fread(&n, sizeof(long), 1, ifp);//读取节点数
	for (i = 0; i < n; i++)//构造Huffman树的n个叶子节点
	{
		fread(&HuffmanTree[i].weight, 1, 1, ifp);//读取一个字节，得到Huffman树的一个结点
		fread(&c, 1, 1, ifp);//读取字符对应的哈夫曼编码长度
		p = (long)c;
		HuffmanTree[i].count = p;//count由保存结点权值改为保存结点的编码长度
		HuffmanTree[i].bits[0] = 0;
		if (p % 8 > 0)
			m = p / 8 + 1;//字节数
		else
			m = p / 8;
		for (j = 0; j < m; j++)
		{
			fread(&c, 1, 1, ifp);//每次取出一次字节
			f = c;//16进制转十进制
			_itoa(f, buf, 2);//将f转换为二进制表示的字符串，如10011，即整数->字符串
			f = strlen(buf);//long变成二制时，如不如8位，而不足8位则补0；
			for (l = 8; l > f; l--)
			{
				strcat(HuffmanTree[i].bits, "0");//先在哈夫曼树结点编码补0
			}
			strcat(HuffmanTree[i].bits, buf);//补足0后连接已转好的01字符串
		}
		HuffmanTree[i].bits[p] = 0;//设置结束符
	}
	for (i = 0; i < n; i++)//根据哈夫曼编码的长短，对结点进行排序，编码短的在前面
	{
		for (j = i + 1; j < n; j++)
		{
			if (strlen(HuffmanTree[i].bits)>strlen(HuffmanTree[j].bits))
			{
				tmp = HuffmanTree[i];
				HuffmanTree[i] = HuffmanTree[j];
				HuffmanTree[j] = tmp;
			}
		}
	}
	p = strlen(HuffmanTree[n - 1].bits);//编码的最大长度

	fseek(ifp, 8, SEEK_SET);//定位文件指针至存放原文件哈夫曼编码的位置
	m = 0;
	bx[0] = 0;//每次处理的编码的字符串
	while (1)//通过哈夫曼编码的长短，一次编码，从原来的位存储还原到字节存储，也有可能时多个字符
	{
		while (strlen(bx) < (unsigned int)p)//bx保存最长编码的01串，有可能是一个字符，也有可能是多个字符
		{
			fread(&c, 1, 1, ifp);//取一个字节，转换成二进制01,同上
			f = c;
			_itoa(f, buf, 2);
			f = strlen(buf);
			for (l = 8; l > f; l--)//在单字节内相应位置补0
			{
				strcat(bx, "0");
			}
			strcat(bx, buf);
		}


		for (i = 0; i < n; i++)
		{
			if (memcmp(HuffmanTree[i].bits, bx, HuffmanTree[i].count) == 0)//比较bits，bx前count个字节
				break;
		}
		//比较成功后，需继续往后判断bx对应的其他字符
		strcpy(bx, bx + HuffmanTree[i].count);
		c = HuffmanTree[i].weight;//得到匹配后的哈夫曼编码对应的字符
		fwrite(&c, 1, 1, ofp);//将得到的字符写入目标文件
		m++;//继续比较，统计解压缩后文件的长度
		if (m == flength)//flength是原文件长度
			break;
	}

	fclose(ifp);//关闭
	fclose(ofp);
	printf("\n\t文件解压缩成功！\n");
	if (m == flength)//对解压缩后文件和原文件相同性比较进行判断(根据文件大小)
		printf("\t解压缩文件与原文件相同！\n\n");
	else
		printf("\t解压缩文件与原文件不同！\n\n");
	return;
}

void welcome() 
{
	system("color f0");//写报告时较清楚
	int c;
	while (1)
	{
		system("cls");
		
		cout << "        请选择你的操作（1.压缩 2.解压 0.退出）";
		do     
		{
			printf("\n\t请选择相应功能(0-2)");
			c = _getch();
			printf("%c\n", c);
			if (c != '0'&&c != '1'&&c != '2')
			{
				printf("QAQ请检查您输入的数字在0-2之间！\n");
				printf("\t请再输入一遍！\n");
			}
		} while (c != '0'&&c != '1'&&c != '2');
		if (c == '1')
			Compress();
		else if (c == '2')
			UnCompress();
		else
		{
			system("cls");
			
			system("pause");
			exit(0);
		}
		system("pause");
	}
}
void main()
{

	welcome();
	system("pause");
}