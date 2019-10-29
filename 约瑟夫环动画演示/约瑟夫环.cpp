#include <stdlib.h>
#include <malloc.h>
#include <iostream>
#include <graphics.h>
#include <conio.h>
#include <math.h>


#define R1 50
#define R2 30 

#define PI 3.14159
using namespace std;


typedef struct Node    /*结点类型定义*/
{
	int num;//编号
	int x;//圆心的x坐标
	int y;//圆心的y坐标
	int r;//当前圆的半径大小
	Node* next;
}Node, * link;  /* LinkList为结构指针类型*/

Node* initLink(int n)
{
	Node* head = new Node;//定义首元节点
	head->num = 1;
	head->next = NULL;
	head->x = 280;//第一个点的横坐标恒为280
	head->y = 0;//第一个点的纵坐标恒为0
	Node* temp = head;
	for (int i = 1; i <= n; i++) //定义后面n-1个节点
	{
		Node* body = new Node;
		body->num = i;
		body->x = 280 * cos(2 * PI / n * body->num);
		body->y = 280 * sin(2 * PI / n * body->num);
		body->next = NULL;
		temp->next = body;
		temp = temp->next;
	}
	temp->next = head;//首尾相连
	return head;
}

void nchoose_circle(Node* choose)
{
	setfillcolor(RED);
	fillcircle(choose->x, choose->y, R2);
	Sleep(400);
	setfillcolor(BLUE);
	fillcircle(choose->x, choose->y, R2);
}//选择过程动画，需要添加到FindAndDelete

void circle_narror(Node* choose)
{
	setfillcolor(RED);
	fillcircle(choose->x, choose->y, R2);
	Sleep(200);
	clearcircle(choose->x, choose->y, R2 + 1);
	Sleep(100);
	fillcircle(choose->x, choose->y, R2);
	Sleep(200);
	clearcircle(choose->x, choose->y, R2 + 1);
	fillcircle(0, 0, R1);
	Sleep(200);
	clearcircle(0, 0, R1 + 1);
	for (int i = 1; i <= 16; i++)
	{
		fillcircle(0, 0, pow(0.8, i) * R1);
		Sleep(200);
		clearcircle(0, 0, pow(0.8, i) * R1 + 1);
	}

}//被选中的消失动画，需要添加到FindAndDelete。被选中的效果是闪烁两下，然后突出到中间位置后缩小到无，

void init_circle(Node* head, int n)//输入总数后，画出那一圈圆
{
	Node* temp = head;
	BeginBatchDraw();
	setfillcolor(BLUE);
	for (int i = 0; i < n; i++)
	{
		fillcircle(temp->x, temp->y, R2);
		temp = temp->next;
	}
	EndBatchDraw();
}


void FindAndDelete(Node* head, int n, int num)
{
	Node* temp = head;
	while (temp->next != head)
	{//定位到首元节点前一节点
		temp = temp->next;
	}
	head = initLink(n);
	Node* temp2 = head;
	while (temp2->num != n)
	{//定一个指针指向起始节点，一个定位到起始节点前一节点，为删除节点准备
		temp = temp2;
		temp2 = temp2->next;
	}
	while (temp2->next != temp2)
	{//当一个循环链表的节点->next指向自身，说明只剩一个节点
		for (int i = 1; i < num; ++i)
		{//一个节点定位到数到num的节点，一个定位到前一节点
			temp = temp2;
			temp2 = temp->next;
			nchoose_circle(temp2);
		}

		circle_narror(temp2);
		temp->next = temp2->next;//删除num节点
		delete temp2;
		temp2 = temp->next;//从下一节点继续开始
	}
	
	char fin_num = (char)('0' + temp2->num);
	getchar();
	delete temp2;
	outtextxy(0, 0, "最后消失的是"+fin_num);
}

void init_num(int n)
{
	float x = 0, y = 0;
	
	for (int i = 1; i <= n; i++)
	{
		x = 350 * cos(2 * PI / n * i);
		y = 350 * sin(2 * PI / n * i);
		char s = (char)('0' + i);
		outtextxy(x, y, s);
	}
}



int main()
{
	int n, m;
	cout << "请输入总人数n和报道数m ";
	cin >> n >> m;
	Node* head = initLink(n);
	initgraph(800, 800);//创建画布
	setorigin(400, 400);//定义画布中心为坐标原点
	init_circle(head, n);
	init_num(n);
	FindAndDelete(head, n, m);
	_getch();
	closegraph();          // 关闭绘图窗口
	return 0;
}
