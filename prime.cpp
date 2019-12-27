#include <graphics.h>      // 引用图形库头文件
#include<string.h>
#include<algorithm>
#include<ctime>
#include<math.h>
#include <stdio.h>

using namespace std;

#define MAX 0x003f3f3f

int win_width = 640;
int win_height = 560;
int origin_x = 0;// 坐标原点位置x轴
int origin_y = 0;// 坐标原点位置y轴
const int circle_count = 30;
int next_c[circle_count][circle_count] = { 0 };
int R = 5;

typedef struct Circle
{
	double x;
	double y;
	double r;
}Circle;

typedef struct Closeedge
{
	int end_ver;
	double lowcost;
	int flag;
}Closeedge;

Closeedge closeedges[circle_count];//最小生成树的边
Circle circles[circle_count];
//测试值：{ {150, 138}, { 350,257 }, { 78,96 }, { 521,235 }, { 193,269 }, { 220,89 }, { 89,390 }, { 552,78 }, { 400,222 }, { 129,500 }};
void init();//初始化窗体
void draw_cirle(Circle C);
void draw_cirles(Circle circles[], int n);
void draw_lines(Circle circles[], int n);
void init_circles(Circle circles[], int n);//初始化窗口大小，窗口坐标中心点/画板背景
int find_center_circle(Circle circles[], int n);//查找到达其它圆的距离最小的圆（类似质心）
void paint_center_circle_line(Circle circles[], int n, int center);//突出绘制中心点以及到其余各个圆的线
void find_min_tree(Circle circles[], int n);
void paint_min_tree(Circle circles[], int n);
void flush(Circle circles[], int n);
int get_clickcirclenum(double x, double y);
void move_circle(MOUSEMSG m);

int main()
{
	init();
	init_circles(circles, circle_count);
	flush(circles, circle_count);
	MOUSEMSG m_down;//定义一个鼠标消息	
	while (1)
	{
		m_down = GetMouseMsg();
		if (m_down.uMsg == WM_LBUTTONDOWN)
		{
			move_circle(m_down);
		}
		if (m_down.uMsg == WM_RBUTTONDOWN)
		{
			break;
		}
	}
	Sleep(500);
	int center = find_center_circle(circles, circle_count);
	paint_center_circle_line(circles, circle_count, center);
	Sleep(2000);
	find_min_tree(circles, circle_count);
	paint_min_tree(circles, circle_count);
	getchar();
	closegraph();
}

void init()//初始化窗口大小，窗口坐标中心点/画板背景
{
	initgraph(win_width, win_height);   // 创建绘图窗口，大小为 640x480 像素		
	setorigin(origin_x, origin_y);//设置绘图的中心点为窗体的中心点	
	setbkcolor(WHITE);
	cleardevice();//清理窗口
	return;
}

void init_circles(Circle circles[], int n)//利用随机数，初始化圆的位置
{
	srand(time(NULL));
	for (int i = 0; i <= n; i++)
	{
		circles[i].x = rand() % (win_width - R) + 10;
		circles[i].y = rand() % (win_height - R) + 10;
		circles[i].r = R;
	}
	return;
}

void draw_cirle(Circle C)//绘制一个圆
{
	setfillcolor(BLUE);
	fillcircle(C.x, C.y, C.r);
}

void draw_circles(Circle circles[], int n)//绘制圆
{
	setfillcolor(BLUE);
	for (int i = 0; i < n; i++)
		draw_cirle(circles[i]);
}



void draw_lines(Circle circles[], int n)//绘制圆之间的连线
{
	setlinecolor(RED);
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < i; j++)
		{
			line(circles[i].x, circles[i].y, circles[j].x, circles[j].y);
		}
	}
}

void flush(Circle circles[], int n)//绘制圆和圆之间的连线
{
	draw_lines(circles, circle_count);
	draw_circles(circles, circle_count);
}

int get_clickcirclenum(double x, double y)//获取所点击圆的编号
{
	for (int i = 0; i < circle_count; i++)
	{
		if ((circles[i].x - x)* (circles[i].x - x) + (circles[i].y - y)*(circles[i].y - y) <= circles[i].r* circles[i].r)
			return i;
	}
	return -1;
}

void move_circle(MOUSEMSG m_down)//可以利用鼠标拖动圆，调整圆的位置
{
	MOUSEMSG m;//定义一个鼠标消息	
	int circle_num;
	//WM_MOUSEMOVE	
	circle_num = get_clickcirclenum(m_down.x, m_down.y);
	if (circle_num >= 0)
	{
		while (true)
		{
			m = GetMouseMsg();
			if (m.uMsg == WM_LBUTTONUP)
			{
				settextcolor(RED);
				circles[circle_num].x = circles[circle_num].x + (m.x - m_down.x);
				circles[circle_num].y = circles[circle_num].y + (m.y - m_down.y);
				cleardevice();
				flush(circles, circle_count);
				break;
			}
		}
	}
	return;
}

int find_center_circle(Circle circles[], int n)//查找到达其它圆的距离最小的圆
{
	int v = 0, k = 0;
	int mv = MAX;
	for (int i = 0; i < circle_count; i++)
	{
		v = 0;
		for (int j = 0; j < circle_count; j++)
		{
			v += abs(circles[i].x - circles[j].x);
			v += abs(circles[i].y - circles[j].y);
		}
		if (v < mv)
		{
			mv = v;
			k = i;
		}
	}
	return k;
}

void paint_center_circle_line(Circle circles[], int n, int center)
{
	setlinecolor(GREEN);
	setlinestyle(PS_SOLID | PS_JOIN_BEVEL, 3);
	for (int j = 0; j < circle_count; j++)
	{
		line(circles[center].x, circles[center].y, circles[j].x, circles[j].y);
	}
}

void find_min_tree(Circle circles[], int n)// 求解最小生成树
{
	int lowcost[circle_count];
	int min, tmp;
	int closest[circle_count], i, j, k = 0;
	for (i = 0; i < circle_count; i++)
	{
		lowcost[i] = abs(circles[0].x - circles[i].x) + abs(circles[0].y - circles[i].y);
		closest[i] = 0;
	}

	for (i = 1; i < circle_count; i++)
	{
		min = MAX;
		for (j = 0; j < circle_count; j++)
		{
			if (lowcost[j] != 0 && lowcost[j]<min)
			{
				min = lowcost[j];
				k = j;
			}
		}
		next_c[closest[k]][k] = 1;
		lowcost[k] = 0;
		for (j = 0; j < circle_count; j++)
		{
			tmp = abs(circles[k].x - circles[j].x) + abs(circles[j].y - circles[i].y);
			if (lowcost[j] != 0 && tmp < lowcost[j])
			{
				lowcost[j] = tmp;
				closest[j] = k;
			}
		}
	}
	printf("1");
}

void paint_min_tree(Circle circles[], int n)//绘制最小生成树
{
	cleardevice();
	draw_circles(circles, n);
	setlinecolor(GREEN);
	setlinestyle(PS_SOLID | PS_JOIN_BEVEL, 3);
	for (int j = 0; j < circle_count; j++)
	{
		for (int i = 0; i < circle_count; i++)
		{
			if (next_c[j][i])
			{
				line(circles[i].x, circles[i].y, circles[j].x, circles[j].y);
			}
		}
	}
}