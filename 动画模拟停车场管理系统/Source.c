
#define _CRT_SECURE_NO_WARNINGS

#include<stdio.h>
#include<stdlib.h>         //malloc
#include<time.h>           //��ȡϵͳʱ�����к���
#include<conio.h>          //_getch()
#include<Windows.h>        //���ù����Ϣ malloc

#define MaxSize 5                /*����ͣ����ջ����*/
#define PRICE 0.05               /*ÿ�����շ�*/
#define Baseprice 0.5            /*����ͣ����*/
#define Esc 27                  /*�˳�ϵͳ*/
#define Exit 3                 /*����*/
#define Stop 1                 /*ͣ��*/
#define Drive 2                 /*ȡ��*/


void gotoxy(int x, int y);
void qingping(int a);
void printstop(int a, int num, int x0, int y0);
void printleave(int a, int po, int num);




int jx = 0, jy = 39;           /*ȫ�ֱ��� ��־��ӡλ��*/

#pragma region �������ݽṹ
typedef struct
{
	int hour;
	int minute;
}Time, *Ptime;         /*ʱ��ڵ�*/

					   /*ͣ����Ϣ�ڵ�*/
typedef struct
{
	int num;     /*���ƺ�*/
	Time arrtime;  /*����ʱ����뿪ʱ��*/
}CarNode;

/*ģ��ͣ����*/
typedef struct
{
	CarNode stack[MaxSize];
	int top;
}SqStackCar;

/*������нڵ�����*/
typedef struct node
{
	int num;   /*���ƺ�*/
	struct node *next;
}QueueNode;

/*������У�ģ����*/
typedef struct
{
	QueueNode *front, *rear;
}LinkQueueCar;
#pragma endregion

void printflog(Time t, int n, int io, char ab, int po, double f);


/*��ʼ��ջ*/
void InitSeqstack(SqStackCar *s)
{
	s->top = -1;
}
/*��ջ*/
int push(SqStackCar *s, CarNode x)
{
	if (s->top == MaxSize - 1)
	{
		return 0; /*���ջ��������0*/
	}
	else
	{
		s->stack[++s->top] = x;/*ջ���������ﳵ����ջ*/
		return 1;
	}
}
/*��ջ*/
CarNode pop(SqStackCar *s)
{
	CarNode x;
	if (s->top < 0)
	{
		x.num = 0;
		x.arrtime.hour = 0;
		x.arrtime.minute = 0;
		return x;/*���ջΪ�գ����ؿ�ֵ*/
	}
	else
	{
		s->top--;
		return s->stack[s->top + 1];/*ջ��Ϊ�գ�����ջ��Ԫ��*/
	}
}

/*��ʼ������*/
void InitLinkQueue(LinkQueueCar *q)
{
	q->front = (QueueNode*)malloc(sizeof(QueueNode));
	if (q->front != NULL)
	{
		q->rear = q->front;
		q->front->next = NULL;
		q->front->num = 0;  /*ͷ����num���������Ԫ�صĸ���*/
	}
}
/*���*/
void EnLinkQueue(LinkQueueCar *q, int x)
{
	QueueNode *p;
	p = (QueueNode*)malloc(sizeof(QueueNode));
	p->num = x;
	p->next = NULL;
	q->rear->next = p;
	q->rear = p;
	q->front->num++;  /*����Ԫ�ظ�����1*/
}
/*����*/
int DeLinkQueue(LinkQueueCar *q)
{
	QueueNode *p;
	int n;
	if (q->front == q->rear)
	{
		return 0;/*�ӿշ���0*/
	}
	else
	{
		p = q->front->next;
		q->front->next = p->next;
		if (p->next == NULL)
		{
			q->rear = q->front;
		}
		n = p->num;
		free(p);
		q->front->num--;
		return n;/*���س�����Ϣ*/
	}
}

/*************************************************��������********************************************************/

//������ͣ��ջ��ͣ�����У�������Ϣ
//����ֵ����
//���ܣ��Դ���ĳ���������ջ ջ�����

void Arrive(SqStackCar *stop, LinkQueueCar *lq, CarNode x)
{
	int f;
	f = push(stop, x);/*��ջ*/
	if (f == 0)/*ջ��*/
	{
		EnLinkQueue(lq, x.num);/*���*/
		printstop(0, lq->front->num, 0, 30);
		printflog(x.arrtime, x.num, 1, 'B', lq->front->num, 0);
		qingping(0);
		printf_s("���ĳ�ͣ�ڱ��%d�ų�λ��\n", lq->front->num);
	}
	else
	{
		printstop(1, stop->top + 1, 0, 30);
		printflog(x.arrtime, x.num, 1, 'P', stop->top + 1, 0);
		qingping(0);
		printf_s("���ĳ�ͣ��ͣ����%d�ų�λ��\n", stop->top + 1);
	}
	qingping(1);
	printf_s("�����������:");
	_getch();
}

/*************************************************�����뿪********************************************************/

//������ͣ��ջָ��s1���ݴ�ջָ��s2��ͣ���ж�ָ��p��������Ϣx
//����ֵ����
//���ܣ�����ջ��s1��x����ջ��ջ��û��������p�����ӣ���ӡ�뿪�շ���Ϣ

void Leave(SqStackCar *s1, SqStackCar *s2, LinkQueueCar *p, CarNode x)
{
	double fee = 0;
	int position = s1->top + 1;/*�������ڳ�λ*/
	int n, f = 0;//n=1��ʾ���ӣ������ǳ�ջ��
	CarNode y;
	QueueNode *q;
	while ((s1->top > -1) && (f != 1))/*��ջ������δ�ҵ�x*/
	{
		y = pop(s1);
		if (y.num != x.num)
		{
			n = push(s2, y);
			position--;
		}
		else
		{
			f = 1;
		}
	}
	if (y.num == x.num)/*�ҵ�x*/
	{
		gotoxy(34, 19);//ͣ��ʱ��
		printf_s("%d:%-2d", (x.arrtime.hour - y.arrtime.hour), (x.arrtime.minute - y.arrtime.minute));
		fee = ((x.arrtime.hour - y.arrtime.hour) * 60 + (x.arrtime.minute - y.arrtime.minute))*PRICE + Baseprice;
		gotoxy(62, 19);//ͣ����
		printf_s("%2.1fԪ\n", fee);
		qingping(0);
		printf_s("ȷ�����ĳ�����Ϣ");
		qingping(1);
		printf_s("�����������");
		_getch();
		while (s2->top > -1)
		{
			y = pop(s2);
			f = push(s1, y);
		}
		n = DeLinkQueue(p);
		if (n != 0)
		{
			y.num = n;
			y.arrtime = x.arrtime;
			f = push(s1, y);
			printleave(p->front->num + 1, position, s1->top + 1);//��ջ����
			printflog(x.arrtime, x.num, 0, 'p', position, fee);
		}
		else
		{
			printleave(0, position, s1->top + 2);
			printflog(x.arrtime, x.num, 0, 'P', position, fee);
		}
	}
	else  //��ջ����x
	{
		while (s2->top > -1)  //��ԭջ
		{
			y = pop(s2);
			f = push(s1, y);
		}
		q = p->front;
		f = 0;
		position = 1;
		while (f == 0 && q->next != NULL)  //���Ӳ�Ϊ����δ�ҵ�x
		{
			if (q->next->num != x.num)
			{
				q = q->next;
				position++;
			}
			else  //�ҵ�x
			{
				q->next = q->next->next;
				p->front->num--;
				if (q->next == NULL)
				{
					p->rear = p->front;
				}
				gotoxy(33, 77);
				printf_s("0:0");
				gotoxy(48, 17);
				printf_s("0Ԫ");
				qingping(0);
				printf_s("���ĳ�������");
				qingping(1);
				printf_s("�����������");
				_getch();
				printleave(-1, position, p->front->num + 1);//���Ӷ���
				printflog(x.arrtime, x.num, 0, 'B', position, 0);
				f = 1;
			}
			if (f == 0)//δ�ҵ�x
			{
				qingping(0);
				printf_s("û�ҵ����ĳ�");
				qingping(1);
				printf_s("�����������");
				_getch();
			}
			
		}
	}
}
/*��ȡϵͳʱ��*/
//����PTime����

Ptime get_timer()
{
	Time *t;
	time_t timer;
	struct tm *tblock;
	t = (Time*)malloc(sizeof(Time));

	timer = time(NULL);
	tblock = localtime(&timer);
	t->minute = tblock->tm_min;
	t->hour = tblock->tm_hour;
	return t;
}
//�ƶ���굽��x��y����
void gotoxy(int x, int y)
{
	COORD coord;
	coord.X = x;
	coord.Y = y+3;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}
/*��ͼ-����ϵͳ����*/
void panitPL()
{
	int i, j, x, y, a[2][4] = { 2,0,0,1,-2,0,0,-1 };
	gotoxy(20, 4);
	printf_s("***************************�Ի���****************************");
	x = 18, y = 6;//���
	for (i = 0; i < 2; i++)
	{
		for (j = 0; j < 30; j++)
		{
			x += a[i][0];
			y += a[i][1];
			gotoxy(x, y);
			printf_s("==");
		}
		x += a[i][0];
		y += a[i][1];
		gotoxy(x, y);
		if (i == 0)
		{
			printf_s("��");
			
		}
		else
		{
			printf_s("��");
		}

		for (j = 0; j < 15; j++)
		{
			x += a[i][2];
			y += a[i][3];
			gotoxy(x, y);
			printf_s("||");
		}
		x += a[i][2];
		y += a[i][3];
		gotoxy(x, y);

		if (i == 0)
		{
			printf_s("��");

		}
		else
		{
			printf_s("��");
		}
	}
	gotoxy(22, 8);
	printf_s("С��:");
	gotoxy(22, 11);
	printf_s("�˿�:");
	gotoxy(22, 15);
	printf_s("***********************ͣ����Ϣ***********************");
	gotoxy(22, 17);
	printf_s("���ƺ�:");
	gotoxy(57,17);
	printf_s("ʱ��:");
	gotoxy(22, 19);
	printf_s("ͣ��ʱ��:");
	gotoxy(57, 19);
	printf_s("�շ�:");
}

/*��������*/
void qingping(int a)
{
	if (a == 0)
	{
		gotoxy(28, 8);
		printf_s(" ");
		gotoxy(28, 9);
		printf_s(" ");
		gotoxy(27, 8);//�����һ�ŵ���Ϣ
		printf_s("                                   ");
		gotoxy(27, 8);
	}
	else if(a==1)
	{
		gotoxy(28,11);
		printf_s("   ");
		gotoxy(28, 12);
		printf_s("   ");
		gotoxy(28, 13);
		printf_s("   ");
		gotoxy(27, 11);//�����һ�ŵ���Ϣ
		printf_s("                            ");
		gotoxy(27, 11);
	}
	else
	{
		//gotoxy(31, 17);//���복�ƺ�
		//printf_s(" ");
		//gotoxy(48, 17);//��ʾʱ��
		//printf_s(" ");
		//gotoxy(33, 19);
		//printf_s(" ");
		//gotoxy(48, 19);
		//printf_s(" ");
		gotoxy(35, 21);//��Esc�˳�ϵͳ
	}
}
/*�����¼�ѡ��*/
int getkey()
{
	char c;
	int x = 59,y=11;
	while (1)
	{
		gotoxy(x, 11);
		printf_s("  ");
		gotoxy(x, 12);
		printf_s("  ");
		gotoxy(x, 13);
		printf_s("  ");
		gotoxy(x, y);
		printf_s(">>");
		c = _getch();
		if (c == 13)/*��Enter�����ص�ǰֵ*/
		{
			return y - 10;
		}
		
		if (c != -32)/*���Ƿ���������´�ѭ��*/
			continue;
		
		c = _getch();
		if (c == 72)/*��*/
		{			
			if (y == 11)
			{
				y = 13;
			}				
			else
			{
				y--;
			}
		}
		if (c == 80)/*��*/
		{
			if (y == 13)
				y = 11;
			else
				y++;
		}
	}
}
/*���복����Ϣ*/
CarNode getcarInfo()
{
	Ptime T;
	CarNode x;
	qingping(0);
	printf_s("�����복�ƺţ�\n");
	qingping(1);
	printf_s("�����������복����Ϣ��\n");
	//qingping(2);
	gotoxy(30, 17);//��ʾ���ƺ�
	scanf_s("%d", &(x.num));
	T = get_timer();
	x.arrtime = *T;
	gotoxy(62, 17);//��ʾʱ��
	printf_s("%d:%d", x.arrtime.hour, x.arrtime.minute);
	_getch();
	return x;
}
/*��ӡͣ����*/
void printcar()
{
	gotoxy(0, 25);
	printf_s("========================================================================================��\n");
	printf_s("                 �����ݷ��� ||    1       2      3       4      5                        ||\n");
	printf_s("----------------------------------------------------------------------------             ||\n");
	printf_s("                         ������                                                          ||\n");
	printf_s("----------------------------------------------------------------------------             ||\n");
	printf_s(" 12    11    10     9     8     7     6     5       4      3      2       1              ||\n");
	printf_s("=============================================================================�[           ||\n");
	printf_s("   ��           ��===========================================================��          || \n");
	printf_s("  ��� ͣ���� ��|| 1       2         3         4          5          6          7        ||\n");
	printf_s("                �^========================================================================�a\n\n");
	printf_s("                         ������־\n\n");
	printf_s("ʱ��    ���ƺ�     ����1��/ ����0����λ��B���Pͣ�������շѣ�Ԫ��");
	
}
/*��ӡ��־*/
void printflog(Time t, int n, int io, char ab, int po, double f)
{
	jy++;
	gotoxy(jx, jy);
	if (io == 0)
		printf_s("                                                    %2.1f", f);
	gotoxy(jx, jy);
	printf_s("%d:%d    %d             %d             %c:%d", t.hour, t.minute, n, io, ab, po);
	/*gotoxy(jx, jy);
	printf_s("%d:%d / %d", t.hour, t.minute, n);*/
}
void printstop(int a, int num, int x0, int y0)
{
	static char *car = "������";
	/*x0 = 0; 
	y0 = 27;*/
	int x = 0, y = 28;
	if (a == 0)//��
	{
		x = (num + 6) * 6;
		for (; x0 < 72; x0++)
		{
			gotoxy(x0, y0);
			printf_s("%s", car);
			Sleep(30);
			gotoxy(x0, y0);
			printf_s("  ");
		}
		for (; y0 < y; y0++)
		{
			gotoxy(x0, y0);
			printf_s("%s", car);
			Sleep(100);
			gotoxy(x0, y0);
			printf_s("  ");
		}
		for (; x0 > x; x0--)
		{
			gotoxy(x0, y0);
			printf_s("%s", car);
			Sleep(50);
			gotoxy(x0, y0);
			printf_s("  ");
		}
		gotoxy(x, y);
		printf_s("%s", car);
	}
	else
	{
		x = (14 - num) * 6;
		y = y + 5;
		for (; x0 < x; x0++)
		{
			gotoxy(x0, y0);
			printf_s("%s", car);
			Sleep(30);
			gotoxy(x0, y0);
			printf_s("  ");
		}
		gotoxy(x, y);
		printf_s("%s", car);
	}
}
void printleave(int a, int po, int num)
{
	static char *car = "������";
	int x0 = 0;
	int y0 = 23;
	int x = 0, y = 28;
	int i;
	if (a == -1)
	{
		x = (14-po) * 6;
		y = y - 3;
		gotoxy(x, y);
		printf_s("  ");

		gotoxy(x, y-2);
		printf_s("%s", car);
		Sleep(100);

		if (12 > num)
		{
			gotoxy((12-num)*6, y);
			printf_s(" ");
		}
		gotoxy(x, y);
		printf_s("%s", car);

		for (; x > x0; x--)
		{
			gotoxy(x, y - 2);
			printf_s("%s", car);
			Sleep(30);
			gotoxy(x, y - 2);
			printf_s(" ");
		}
	}
	else
	{
		i = num + 1;

		for (;num>po;num--)
		{
			x = (num + 6) * 6;
			y = 28;
			for (; x < 72; x++)
			{
				gotoxy(x, y);
				printf("%s", car);
				Sleep(30);
				gotoxy(x, y);
				printf_s("  ");
			}
			for (; y>21; y--)
			{
				gotoxy(x, y);
				printf("%s", car);
				Sleep(50);
				gotoxy(x, y);
				printf_s("  ");
			}
			for (; x>(i-num+6)*6;x--)
			{
				gotoxy(x, y);
				printf("%s", car);
				Sleep(30);
				gotoxy(x, y);
				printf_s("  ");
			}
			gotoxy(x, y);
			printf_s("%s", car);
		}
		x = (po + 6) * 6;
		y = 28;
		for (; x < 72; x++)
		{
			gotoxy(x, y);
			printf("%s", car);
			Sleep(30);
			gotoxy(x, y);
			printf_s("  ");
		}
		for (; y>23; y--)
		{
			gotoxy(x, y);
			printf("%s", car);
			Sleep(50);
			gotoxy(x, y);
			printf_s("  ");
		}
		for (; x>0; x--)
		{
			gotoxy(x, y);
			printf("%s", car);
			Sleep(30);
			gotoxy(x, y);
			printf_s("  ");
		}
		num++;

		for (; i-num>0; num++)
		{
			x = (i - num + 6) * 6;
			y = 21;
			for (; x < 72; x++)
			{
				gotoxy(x, y);
				printf("%s", car);
				Sleep(30);
				gotoxy(x, y);
				printf_s("  ");
			}
			for (; y>28; y--)
			{
				gotoxy(x, y);
				printf("%s", car);
				Sleep(50);
				gotoxy(x, y);
				printf_s("  ");
			}
			for (; x>(num+5)*6; x--)
			{
				gotoxy(x, y);
				printf("%s", car);
				Sleep(30);
				gotoxy(x, y);
				printf_s("  ");
			}
			gotoxy(x, y);
			printf("%s", car);
		}
		if (a > 0)
		{
			x = 66;
			y = 25;
			gotoxy(x, y);
			printf("%s", car);

			gotoxy(x, y-2);
			printf("%s", car);
			Sleep(100);
			if (12>a)
			{

				gotoxy((12-a)*6, y);
				printf("  ");
			}
			if (a > 1)
			{

				gotoxy(x, y);
				printf("%s", car);
			}
			printstop(0, i - 1, x, y - 2);
		}
	}
}


void main(void)
{
	int i, a;
	char c;
	SqStackCar s1, s2;
	LinkQueueCar p;
	InitSeqstack(&s1);
	InitSeqstack(&s2);
	InitLinkQueue(&p);
	printf_s("                                        ͣ��������ϵͳ\n\n");
	printf_s("    ******************************************��ӭ����************************************************\n\n");
	printf_s("�շѱ�׼��������0.5Ԫ��ÿ�����շ�0.8Ԫ���շѾ�ȷ��0.1Ԫ\n");
	panitPL();
	printcar();
	gotoxy(0, -3);
	c = 0;
	while (1)
	{
		for (i = 2; i > -1; i--)
		{
			qingping(i);
			printf_s("��Esc�˳�ϵͳ");
			c = _getch();
			if (c == Esc)
			{
				qingping(0);
				break;
			}
			while (1)
			{
				qingping(2);
				gotoxy(36, 7);
				printf_s("        ��ӭ���٣�");

				gotoxy(49, 9);
				printf_s("    �밴����ѡ����Ҫ�ķ���  ");
				gotoxy(58, 11);
				printf_s("    1����Ҫͣ��");
				gotoxy(58, 12);
				printf_s("    2����Ҫȡ��  ");
				gotoxy(58, 13);
				printf_s("    3������  ");

				a = getkey();
				if (a == Exit)
				{
					printf_s("   ��������");
					break;
				}
				switch (a)
				{
				case Stop:
					Arrive(&s1, &p, getcarInfo());
					break;
				case Drive:
					Leave(&s1, &s2, &p, getcarInfo());
					break;
				default:
					break;
				}
			}
		}
	}
}