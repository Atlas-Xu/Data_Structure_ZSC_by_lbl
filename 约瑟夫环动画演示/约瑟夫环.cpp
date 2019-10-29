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


typedef struct Node    /*������Ͷ���*/
{
	int num;//���
	int x;//Բ�ĵ�x����
	int y;//Բ�ĵ�y����
	int r;//��ǰԲ�İ뾶��С
	Node* next;
}Node, * link;  /* LinkListΪ�ṹָ������*/

Node* initLink(int n)
{
	Node* head = new Node;//������Ԫ�ڵ�
	head->num = 1;
	head->next = NULL;
	head->x = 280;//��һ����ĺ������Ϊ280
	head->y = 0;//��һ������������Ϊ0
	Node* temp = head;
	for (int i = 1; i <= n; i++) //�������n-1���ڵ�
	{
		Node* body = new Node;
		body->num = i;
		body->x = 280 * cos(2 * PI / n * body->num);
		body->y = 280 * sin(2 * PI / n * body->num);
		body->next = NULL;
		temp->next = body;
		temp = temp->next;
	}
	temp->next = head;//��β����
	return head;
}

void nchoose_circle(Node* choose)
{
	setfillcolor(RED);
	fillcircle(choose->x, choose->y, R2);
	Sleep(400);
	setfillcolor(BLUE);
	fillcircle(choose->x, choose->y, R2);
}//ѡ����̶�������Ҫ��ӵ�FindAndDelete

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

}//��ѡ�е���ʧ��������Ҫ��ӵ�FindAndDelete����ѡ�е�Ч������˸���£�Ȼ��ͻ�����м�λ�ú���С���ޣ�

void init_circle(Node* head, int n)//���������󣬻�����һȦԲ
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
	{//��λ����Ԫ�ڵ�ǰһ�ڵ�
		temp = temp->next;
	}
	head = initLink(n);
	Node* temp2 = head;
	while (temp2->num != n)
	{//��һ��ָ��ָ����ʼ�ڵ㣬һ����λ����ʼ�ڵ�ǰһ�ڵ㣬Ϊɾ���ڵ�׼��
		temp = temp2;
		temp2 = temp2->next;
	}
	while (temp2->next != temp2)
	{//��һ��ѭ������Ľڵ�->nextָ������˵��ֻʣһ���ڵ�
		for (int i = 1; i < num; ++i)
		{//һ���ڵ㶨λ������num�Ľڵ㣬һ����λ��ǰһ�ڵ�
			temp = temp2;
			temp2 = temp->next;
			nchoose_circle(temp2);
		}

		circle_narror(temp2);
		temp->next = temp2->next;//ɾ��num�ڵ�
		delete temp2;
		temp2 = temp->next;//����һ�ڵ������ʼ
	}
	
	char fin_num = (char)('0' + temp2->num);
	getchar();
	delete temp2;
	outtextxy(0, 0, "�����ʧ����"+fin_num);
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
	cout << "������������n�ͱ�����m ";
	cin >> n >> m;
	Node* head = initLink(n);
	initgraph(800, 800);//��������
	setorigin(400, 400);//���廭������Ϊ����ԭ��
	init_circle(head, n);
	init_num(n);
	FindAndDelete(head, n, m);
	_getch();
	closegraph();          // �رջ�ͼ����
	return 0;
}
