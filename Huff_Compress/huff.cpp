#define _CRT_SECURE_NO_WARNINGS
#include "huffman.h"
#include<conio.h>//����̨�����������ȡ
#include<iostream>
using namespace std;

void Compress()
{
	int i, j;
	FILE *ifile,*ofp;        //ָ��FILE����ָ��
	char filename[N];
	cout << "\t������ѹ��ͼƬ��ַ���ļ�����";
	gets_s(filename);        //��ȡ�û�������ļ�·��
	cout << endl;
	ifile = fopen(filename, "rb"); //�����������ļ�

	if (ifile == NULL)       //������--�Ҳ����ļ�
	{
		cout << "\t���ļ�ʧ��" << endl << "���������룺";
		scanf("%s", filename);
		fflush(stdin);		// ��ձ�׼����������ֹ����gets������ȡ�ļ���
		ifile = fopen(filename, "rb");
		if (ifile == NULL)
			welcome();
	}

	unsigned char Bmp_char; //�����ƶ�ȡ���ļ���0-255�ġ��ַ�
	int length = 0; //���ȣ�ͳ���ã�
	int flength; //�ļ�����ֵ
	//��ȡ�ļ���ͳ���ļ��������ַ����ִ���
	while (!feof(ifile)) //feof��⵽�ļ�������������0
	{
		fread(&Bmp_char, 1, 1, ifile);//���ֽڶ����ļ� ��ָ��Bmp_char���ڴ��ַ
		HuffmanTree[Bmp_char].count++;//��Bmp_char���ֽ�Ƶ��+1
		length++; //��ǰ��ȡ�ļ�����
	}//��ȡ�ļ�
	length--;   //���һ���ֽڶ������һ�Σ�'\0'����һ��
	flength = length;  //�ļ��ܳ�
	HuffmanTree[Bmp_char].count--;  //���һ���ֽڶ������һ��

	//��ʼ��������
	for (i = 0; i < M; i++) 
	{
		if(HuffmanTree[i].count !=0)
			HuffmanTree[i].weight = (unsigned char)i;//�����Ȩֵ������Ҷ�ӽ����ֽڷ�
		else     
			HuffmanTree[i].weight = 0;
		//�Խ����г�ʼ�������н��ĸ���㶼�����ڣ����Һ��Ӷ�������
		HuffmanTree[i].parent = -1;
		HuffmanTree[i].lch = -1;
		HuffmanTree[i].rch = -1;
	}
	//��ǰ256��Ԫ�ظ���[weight��Ȩֵ��]��С����
	//�ڴ˴��������򽫽�㰴Ȩֵ�Ӵ�С����(��ѡ������)
	HuffNode temp;//��ʱ�����ṹ��
	for (i = 0; i < N; i++)
	{
		for (j = i + 1; j < N; j++)//��a=1��a[1]��a[2~n]�Ƚϣ���a[1]��С
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
		if (HuffmanTree[i].count == 0)//�����ļ�ʱ���ַ�����Ƶ��Ϊ��Ļ���ȥ��
			break;
	}
	n = i;          //n��Ҷ�ӽڵ�
	m = 2 * n - 1;  //mΪ�����������ܽ����
	//n = i;          //��n��Ҷ�ӽڵ��±�
	//m = 2 * n + 1;  //mΪ�����������ܽ����2*��n+1��-1 

	//���ɹ�������
	long maxt, pt;
	cout << "����ڵ���Ϣ:\n";
	for (i = n; i < m; i++)  //��������������ʣ�µķ�Ҷ�ӽ��
	{
		maxt = LONG_MAX;    //Ԥ������Ȩֵ���������ֵ�������
		for (j = 0; j < i; j++)    //ѡ��Ȩֵ��С�Ľ��
		{
			//if (HuffmanTree[j].parent != -1)  //parent!=-1˵���ý�����ڹ��������У���ʼ�Ķ���-1��������ѭ��ѡ����һ���ڵ� 
				//continue;
			//���涼��parent= -1
			if (HuffmanTree[j].parent == -1 && maxt > HuffmanTree[j].count)   //��
			{
				pt = j;
				maxt = HuffmanTree[j].count;
				//continue;
			}
		}
		//������ȡ����С����Ϊ�������������㣬���ú������Ϣ
		cout << "�� " << j<< " �ڵ�";
		HuffmanTree[i].count = HuffmanTree[pt].count;
		HuffmanTree[pt].parent = i;    //���õ�i��������ӵĸ������Ϣ
		HuffmanTree[i].lch = pt;       //���õ�i����������
		cout << "\t���ӣ�" << HuffmanTree[i].lch ;
		maxt = LONG_MAX;
		for (j = 0; j < i; j++)   //ͬ���ķ�ʽ�����Сֵ����ʱ��Сֵ��parent�Ѳ�Ϊ-1
		{
			if (HuffmanTree[j].parent == -1 && maxt > HuffmanTree[j].count)   //��
			{
				pt = j;
				maxt = HuffmanTree[j].count;
				//continue;
			}
		}
		
		HuffmanTree[i].count += HuffmanTree[pt].count;
		HuffmanTree[pt].parent = i;  //���õ�i������Һ��ӵĸ������Ϣ
		HuffmanTree[i].rch = pt;     //���õ�i�������Һ���
		cout << "\t�Һ��ӣ�" << HuffmanTree[i].rch << endl;

}//ѭ������=������� QAQ
	cout << "\n\n��������Ϣ\n";
 //Huffman����
 int f;
 for (i = 0; i < n; i++)
 {
	 f = i;
	 HuffmanTree[i].bits[0] = 0;  //ASCll��Ϊ0���ַ�����Ϊ\0������
	 while (HuffmanTree[f].parent != -1)//���и��׽ڵ��ʱ��
	 {
		 j = f;  //��ʱf����ѭ��i
		 f = HuffmanTree[f].parent;  //�жϵ�ǰ����븸�ڵ�Ĺ�ϵ
		 if (HuffmanTree[f].lch == j)   //�÷�֧����0
		 {
			 j = strlen(HuffmanTree[i].bits);
			 //����������λ�÷ŵ�ǰ�ı���
			 //j+1��ζ�ſ���ʱ��\0���ƣ�menmove �������ص����ֱ�����
			 memmove(HuffmanTree[i].bits + 1, HuffmanTree[i].bits, j + 1);//j+1��HuffmanTree[i].bits + 1���ȣ�HuffmanTree[i].bits+1�����һλ���ճ�
			 //���δ洢���ӡ�0����1������
			 HuffmanTree[i].bits[0] = '0';//���֧��Ϊ0
		 }
		 else //���ҷ�֧����1
		 {
			 j = strlen(HuffmanTree[i].bits);
			 memmove(HuffmanTree[i].bits + 1, HuffmanTree[i].bits, j + 1);
			 HuffmanTree[i].bits[0] = '1';
			 //������֧�Ĵ�������ҷ�֧�Ĵ���
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

	//ѹ���ļ�
	char wenjian[M];//��������ļ���
	printf("\n����ѹ������ļ���:");
	fflush(stdin);		// ��ձ�׼����������ֹ����gets������ȡ�ļ���
	scanf("%s", wenjian);
	ofp = fopen(wenjian, "wb");//���ļ�
	while (ofp == NULL)
	{
		printf("\t��������ѹ������ļ�");
		ofp = fopen(wenjian, "wb");

	}
	fseek(ifile, 0, SEEK_SET);//SEEK_SETָ�ļ�ͷ�����ļ�ָ��ָ���ѹ���ļ��Ŀ�ʼλ��
	fwrite(&length, sizeof(int), 1, ofp);//��ѹ���ļ�ͷд��ԭ�ļ����ܳ��ȣ�ռ2���ֽ�
	fseek(ofp, 8, SEEK_SET);//�ض�λѹ���ļ�ָ�룬��ͷƫ��8���ֽڣ������ռ�д������Ϣ����Ϊд�����������׼��
	char buf[512];//���建�����������ֽڵ�Huffman����
	buf[0] = 0;//��ʼΪ'\0'��'\0'��ʾ���ǽ�β
	f = 0;//ͳ���ļ��ĳ��ȣ�����������ӵ�8���ֽڿ�ʼд��
	pt = 8;
	while (!feof(ifile))//ɨ��ԭ�ļ�
	{
		
		//���ļ��ж�ȡһ���ַ�����ȡһ���ַ��󣬹��λ�ú���һ���ֽ�
		fread(&Bmp_char, 1, 1, ifile);//���ֽڶ����ļ� ��ָ��Bmp_char���ڴ��ַ
		f++;//���ֽ�
		for (i = 0; i < n; i++)
		{
			//�ҵ�ȡ���ַ���Ӧ����������Ҷ�ӽ�㣬���õ���Ӧ���±�ȥ����Ӧ�ı���
			if (Bmp_char == HuffmanTree[i].weight)
				break;
		}
		strcat(buf, HuffmanTree[i].bits);//�ҵ���ǰ�ַ��Ĺ��������룬���ӵ�buf��
										 //����
										 //�����ȴ���8�����в��д�룬��С��8�������ȡ��һ���ַ��Ĺ��������һ���ֽڣ�����д��
		j = strlen(buf);//ͳ�Ƹ��ַ�����ĳ���
		Bmp_char = 0;
		while (j >= 8)//����ǰ����ĳ��ȴ��ڵ���8������в�֣��������ֽڴ棬��1100101010��ֳ�11001010��10
		{
			for (i = 0; i < 8; i++)
			{
				if (buf[i] == '1')
					Bmp_char = (Bmp_char << 1) | 1;
				else Bmp_char = Bmp_char << 1;
			}//ÿ8�����һ��c
			fwrite(&Bmp_char, 1, 1, ofp);
			//�Ѵպõ�һ���ֽڱ���д���ļ���д���ļ�ͷ���̣�д֮ǰ�ǵð��ļ��Զ����Ƶķ�ʽ�򿪣���
			//һ���ֽڣ�һ�����ݣ�ֵΪc�����ݣ�����������ܵ�fwrite�������
			pt++;//�ļ�����
			strcpy(buf, buf + 8);//��buf��һ���ֽ�����������ݸ��Ƶ�buf�У���һ���ֽ�һ���ֽ�ȡ
			j = strlen(buf);//��j=j-8��
							//��
		}

		if (f == flength)//��Դ�ļ����е��ַ�ȡ�꣬����
			break;
	}
	//ͳ��ѹ�����ļ����ȣ���
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
		fwrite(&Bmp_char, 1, 1, ofp);//�����һ���ֽ�д���ļ���
		pt++;
	}
	//����
	fseek(ofp, 4, SEEK_SET);//�ƶ��ļ�ָ��λ�õ���4���ֽ�
	fwrite(&pt, sizeof(long), 1, ofp);//д��ͳ��ѹ�����ļ��ĳ��ȣ�4���ֽ�
	fseek(ofp, pt, SEEK_SET);//�ƶ��ļ�ָ�뵽ѹ�����ļ�β
	fwrite(&n, sizeof(long), 1, ofp);//д��ڵ���Ŀ�����ܵĲ�ͬ�ֽڵĸ���

	for (i = 0; i < n; i++)
	{
		fwrite(&(HuffmanTree[i].weight), 1, 1, ofp);//д��ÿ���ڵ�Ĵ�����ַ�
		Bmp_char = strlen(HuffmanTree[i].bits);
		fwrite(&Bmp_char, 1, 1, ofp);//д��ÿ���ַ�����������ĳ���
		j = strlen(HuffmanTree[i].bits);//ͳ�ƹ���������
		if (j % 8 != 0)//���洢��λ������8�ı�������0
		{
			for (f = j % 8; f < 8; f++)
				strcat(HuffmanTree[i].bits, "0");//001 00000    01010101 10000000

		}
		//�������������ַ����ַ�����ɶ�����
		while (HuffmanTree[i].bits[0] != 0)//�������Ƿ����ַ���ĩβ
		{
			Bmp_char = 0;
			for (j = 0; j < 8; j++)//�ַ�����Ч�洢������8λ�������Чλ������ʵ�ֲ�0
			{
				if (HuffmanTree[i].bits[j] == '1')
					Bmp_char = (Bmp_char << 1) | 1;
				else
					Bmp_char = Bmp_char << 1;
			}
			strcpy(HuffmanTree[i].bits, HuffmanTree[i].bits + 8);//����ת��������ַ���
			fwrite(&Bmp_char, 1, 1, ofp);
		}
	}

	long length2 = pt--;//ѹ������ļ���С
	double div = (double)length2 / (double)flength;//�����ļ���ѹ����
	fclose(ifile);//�ر��ļ�
	fclose(ofp);
	printf("\nѹ���ļ��ɹ�\n");
	printf("ѹ����Ϊ %f%%\n", div * 100);
}

	/*ifile = fopen(filename, "rb");
	while (!feof(ifile)) //feof��⵽�ļ�������������0
	{

		fread(&Bmp_char, 1, 1, ifile);//���ֽڶ����ļ� ��ָ��Bmp_char���ڴ��ַ
		cout<<HuffmanTree[Bmp_char].bits;//��Bmp_char���ֽ�Ƶ��+1
		
	}

	fclose(ifile);

	
}*/

//��ѹ��
void UnCompress()
{
	char filename[255], outpufile[255], buf[255], bx[255];
	//bx����������01�����п�����һ���ַ���Ҳ�п����Ƕ���ַ�
	//��fת��Ϊ�����Ʊ�ʾ���Ը�������10011
	unsigned char c;
	HuffNode tmp;
	long i, j, m, n, f, p, l;
	long flength;
	FILE *ifp, *ofp;
	printf("\t����������Ҫ��ѹ�����ļ���:");
	gets_s(filename);
	ifp = fopen(filename, "rb");
	if (ifp == NULL)
	{
		printf("\n\t�ļ���ʧ��\n");
		return;
	}
	printf("\t���ڵ�ǰĿ¼�½�ѹ�����������ѹ������ļ���:");
	gets_s(outpufile);
	ofp = fopen(outpufile, "wb");
	if (ofp == NULL)
	{
		printf("\t��ѹ���ļ�ʧ��");
		return;
	}


	//����Huffman����Huffman�����
	fread(&flength, sizeof(long), 1, ifp);//��ȡδѹ��ʱԭ�ļ�����
	fread(&f, sizeof(long), 1, ifp);//��ȡѹ���ļ��ĳ��ȣ�λ�ڵ�4���ֽڴ�
	fseek(ifp, f, SEEK_SET);//���ļ�ָ�붨λ���洢�ڵ�����
	fread(&n, sizeof(long), 1, ifp);//��ȡ�ڵ���
	for (i = 0; i < n; i++)//����Huffman����n��Ҷ�ӽڵ�
	{
		fread(&HuffmanTree[i].weight, 1, 1, ifp);//��ȡһ���ֽڣ��õ�Huffman����һ�����
		fread(&c, 1, 1, ifp);//��ȡ�ַ���Ӧ�Ĺ��������볤��
		p = (long)c;
		HuffmanTree[i].count = p;//count�ɱ�����Ȩֵ��Ϊ������ı��볤��
		HuffmanTree[i].bits[0] = 0;
		if (p % 8 > 0)
			m = p / 8 + 1;//�ֽ���
		else
			m = p / 8;
		for (j = 0; j < m; j++)
		{
			fread(&c, 1, 1, ifp);//ÿ��ȡ��һ���ֽ�
			f = c;//16����תʮ����
			_itoa(f, buf, 2);//��fת��Ϊ�����Ʊ�ʾ���ַ�������10011��������->�ַ���
			f = strlen(buf);//long��ɶ���ʱ���粻��8λ��������8λ��0��
			for (l = 8; l > f; l--)
			{
				strcat(HuffmanTree[i].bits, "0");//���ڹ������������벹0
			}
			strcat(HuffmanTree[i].bits, buf);//����0��������ת�õ�01�ַ���
		}
		HuffmanTree[i].bits[p] = 0;//���ý�����
	}
	for (i = 0; i < n; i++)//���ݹ���������ĳ��̣��Խ��������򣬱���̵���ǰ��
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
	p = strlen(HuffmanTree[n - 1].bits);//�������󳤶�

	fseek(ifp, 8, SEEK_SET);//��λ�ļ�ָ�������ԭ�ļ������������λ��
	m = 0;
	bx[0] = 0;//ÿ�δ���ı�����ַ���
	while (1)//ͨ������������ĳ��̣�һ�α��룬��ԭ����λ�洢��ԭ���ֽڴ洢��Ҳ�п���ʱ����ַ�
	{
		while (strlen(bx) < (unsigned int)p)//bx����������01�����п�����һ���ַ���Ҳ�п����Ƕ���ַ�
		{
			fread(&c, 1, 1, ifp);//ȡһ���ֽڣ�ת���ɶ�����01,ͬ��
			f = c;
			_itoa(f, buf, 2);
			f = strlen(buf);
			for (l = 8; l > f; l--)//�ڵ��ֽ�����Ӧλ�ò�0
			{
				strcat(bx, "0");
			}
			strcat(bx, buf);
		}


		for (i = 0; i < n; i++)
		{
			if (memcmp(HuffmanTree[i].bits, bx, HuffmanTree[i].count) == 0)//�Ƚ�bits��bxǰcount���ֽ�
				break;
		}
		//�Ƚϳɹ�������������ж�bx��Ӧ�������ַ�
		strcpy(bx, bx + HuffmanTree[i].count);
		c = HuffmanTree[i].weight;//�õ�ƥ���Ĺ����������Ӧ���ַ�
		fwrite(&c, 1, 1, ofp);//���õ����ַ�д��Ŀ���ļ�
		m++;//�����Ƚϣ�ͳ�ƽ�ѹ�����ļ��ĳ���
		if (m == flength)//flength��ԭ�ļ�����
			break;
	}

	fclose(ifp);//�ر�
	fclose(ofp);
	printf("\n\t�ļ���ѹ���ɹ���\n");
	if (m == flength)//�Խ�ѹ�����ļ���ԭ�ļ���ͬ�ԱȽϽ����ж�(�����ļ���С)
		printf("\t��ѹ���ļ���ԭ�ļ���ͬ��\n\n");
	else
		printf("\t��ѹ���ļ���ԭ�ļ���ͬ��\n\n");
	return;
}

void welcome() 
{
	system("color f0");//д����ʱ�����
	int c;
	while (1)
	{
		system("cls");
		
		cout << "        ��ѡ����Ĳ�����1.ѹ�� 2.��ѹ 0.�˳���";
		do     
		{
			printf("\n\t��ѡ����Ӧ����(0-2)");
			c = _getch();
			printf("%c\n", c);
			if (c != '0'&&c != '1'&&c != '2')
			{
				printf("QAQ�����������������0-2֮�䣡\n");
				printf("\t��������һ�飡\n");
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