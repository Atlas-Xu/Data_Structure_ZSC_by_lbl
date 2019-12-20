#include <stdio.h>
#include <graphics.h>
#include <iostream>
#include <conio.h>
#include<stack>
#include <queue>
using namespace std;
IMAGE* images[10];
MOUSEMSG m;
int status[10] = { 1,5,6,7,3,2,0,4,8,9 };
int current_status[10];
int next_num[10][4] ={ { -1, -1, -1, 1},{ -1,  0,  2,  4},{  1, -1,  3,  5},{  2,- 1, -1,  6},{ -1,  1,  5, 7},
                       { 4,  2,  6,  8},{  5,  3, -1,  9},{  -1,  4, 8, -1},{  7,  5,  9, -1},{  8,  6, -1, -1} };
int coordinate[10][2] = { {0,-76},{0,0},{100,0},{200,0},{0,76},{100,76},{200,76},{0,152},{100,152},{200,152} };
int f[11] = { 1,1,2,6,24,120,720,5040,40320,362880,3628800 };

int sum = 0;//�Ʋ�
int kangtuo_status[3628801]={0};
int current_kangtuo;
stack<int>my_stack;
queue<int>my_queue;
int kangtuo(int a[]);//����չ��ֵ
void set_image();//����ͼƬ
void flush();//���ͼƬ
void show();
int EndPosition(int stu[]);//�жϽ�������
void Showgame();//��Ϸģʽ
int  Find_zeroPlace();
void backward();//����
int forward ();//ǰ��
void auto_deep_play();//��������Զ�ִ��
void swap_iamge(int i,int j, int str[]);
int main()
{
	int userChoice;
	show();
	cin>>userChoice;
	initgraph(640, 640);//���ý���
    setorigin(200,200);//����ԭ��Ϊ��0,0��
	switch(userChoice)
	{
	case 1:
		setbkcolor(0xFFFFFF);//���ñ���Ϊ��ɫ
		cleardevice();
	    set_image();
		Showgame();
		system("pause");
		break;
	case 2:
		set_image();
		auto_deep_play();
		break;
	}
	_getch();
	closegraph();
	return 0;
}

int kangtuo(int a[])
{
	int ans=0,n=10;
	for(int i=0;i<n;i++)
	{
		int x=0;int c=1,m=1;
		for(int j=i+1;j<n;j++)
		{
			if(a[j]<a[i])x++;
			m*=c;c++;
		}
		ans+=x*m;
	}
	return ans;
}

void set_image()//����ͼƬ
{
	TCHAR str[10]=L"0.jpg";
	for (int i = 0; i <=10; i++)
	{
	    images[i]= new IMAGE;
		str[0]='0'+i;
		loadimage(images[i],str);
	}
	for (int i = 0; i <10; i++)
    {
	  putimage(coordinate[i][0], coordinate[i][1], images[status[i]]);
    }
}

void flush()//���ͼƬ
{ 
	  TCHAR s[6];
	  cleardevice();
	  for (int i = 0; i < 10; i++)
	  {
		 putimage(coordinate[i][0], coordinate[i][1], images[status[i]]);
	  }
      settextcolor(BLUE);
	   _stprintf(s, _T("%d"),sum);
	   outtextxy(-200, -100, s);
}

void show()
{
	system("cls");
	cout<<"--------------------------------"<<endl;
	cout<<"\t1.�˹���Ϸģʽ"<<endl;
	cout<<"\t2.��������Զ�ִ��ģʽ"<<endl;
	cout<<"--------------------------------"<<endl;
	cout<<"��ѡ����Ϸģʽ:"<<endl;
}

int EndPosition(int stu[])//�жϽ�������
{
	for (int i = 0; i < 10; i++)
	{
		if (stu[i]!= i)
		return 0;
	}
		return 1;
}

void Showgame()
{
	while (1)
	{
		m = GetMouseMsg();
		if (m.uMsg == WM_LBUTTONDOWN)
		{
			for(int i=0;i<10;i++)
			{
				if(m.x-200>=coordinate[i][0] && m.x-200<=coordinate[i][0]+100 
				&& m.y-200>=coordinate[i][1] && m.y-200<=coordinate[i][1]+76)//�ж�������Ƿ���ͼƬ����
				{
					for(int j=0;j<4;j++)
					{
						if(next_num[i][j]!=-1 && status[next_num[i][j]]==0)
						{
							status[next_num[i][j]]=status[i];
							status[i]=0;
							sum++;
							flush();
							break;
						}
					}
				}
			}
			if (EndPosition(status) == 1)
			{
				cout <<"��ϲͨ�أ����ܹ�����"<< sum<<"��"<<endl;
				break;
			}
		}
	}
}

void swap_iamge(int i,int j, int str[])
{
	int temp;
	temp=str[i];
	str[i]=str[j];
	str[j]=temp;

}

void backward()//����
{
	int temp=my_stack.top();
	my_stack.pop();
	swap(status[temp], status[Find_zeroPlace()]);
}

int Find_zeroPlace()
{
	for (int i = 0; i < 10; i++)
	{
		if (status[i] == 0)
			return i;
	}
}	

int forward ()
{
	int num;
	int temp=Find_zeroPlace();
	for (int d = 0; d < 4; d++)//���ݵ�ǰ״̬���ҵ�0��ͼƬ����λ�ã��������d�����µ��ڽ�λ�ã�
	{
		num=next_num[temp][d];
		if (num>=0)//d�����ǺϷ���ͼƬλ��,������ǰ��
		{
			swap_iamge(temp,num,status);
			if(kangtuo_status[kangtuo(status)]==1)
			{
				swap_iamge(temp,num,status);
				continue;
			}
			else
			{
				my_stack.push(d);
				temp=num;
				kangtuo_status[kangtuo(status)]=1;
				return 1;
			}
		}
	}
	return 0;//û��һ��������ߣ�ǰ��ʧ��
}

void auto_deep_play()//��������Զ�����ģʽ
{
	memset(kangtuo_status, 0, sizeof(kangtuo_status));
	current_kangtuo = kangtuo(status);
	while (current_kangtuo!= 1)
	{
		if (forward()==1)//���������ǰ
		{   
			sum++;
			current_kangtuo = kangtuo(status);
			flush();
			if (current_kangtuo==1)
				break;
		}
		else//��ǰʧ�ܣ������
		{
			if(my_stack.empty()==0)
			{
				 backward();
				 sum++;
				 flush();
			}
			else
			{
				settextcolor(BLUE);
				outtextxy(-200,-200,L"��Ϸ�޽⣡����");
			}
		}
		Sleep(100); //��ͣ��ʾ
	}
}

void auto_width_play()
{
	for(int i=0;i<10;i++)
	{
        my_queue.push(status[i]);
	}
	current_kangtuo=kangtuo(status);
	kangtuo_status[current_kangtuo]=1;
}