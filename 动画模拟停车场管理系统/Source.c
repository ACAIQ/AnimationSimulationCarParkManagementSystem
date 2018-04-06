
#define _CRT_SECURE_NO_WARNINGS

#include<stdio.h>
#include<stdlib.h>         //malloc
#include<time.h>           //获取系统时间所有函数
#include<conio.h>          //_getch()
#include<Windows.h>        //设置光标信息 malloc

#define MaxSize 5                /*定义停车场栈长度*/
#define PRICE 0.05               /*每分钟收费*/
#define Baseprice 0.5            /*基础停车费*/
#define Esc 27                  /*退出系统*/
#define Exit 3                 /*结束*/
#define Stop 1                 /*停车*/
#define Drive 2                 /*取车*/


void gotoxy(int x, int y);
void qingping(int a);
void printstop(int a, int num, int x0, int y0);
void printleave(int a, int po, int num);




int jx = 0, jy = 39;           /*全局变量 日志打印位置*/

#pragma region 定义数据结构
typedef struct
{
	int hour;
	int minute;
}Time, *Ptime;         /*时间节点*/

					   /*停车信息节点*/
typedef struct
{
	int num;     /*车牌号*/
	Time arrtime;  /*到达时间或离开时间*/
}CarNode;

/*模拟停车场*/
typedef struct
{
	CarNode stack[MaxSize];
	int top;
}SqStackCar;

/*定义队列节点类型*/
typedef struct node
{
	int num;   /*车牌号*/
	struct node *next;
}QueueNode;

/*定义队列，模拟便道*/
typedef struct
{
	QueueNode *front, *rear;
}LinkQueueCar;
#pragma endregion

void printflog(Time t, int n, int io, char ab, int po, double f);


/*初始化栈*/
void InitSeqstack(SqStackCar *s)
{
	s->top = -1;
}
/*入栈*/
int push(SqStackCar *s, CarNode x)
{
	if (s->top == MaxSize - 1)
	{
		return 0; /*如果栈满，返回0*/
	}
	else
	{
		s->stack[++s->top] = x;/*栈不满，到达车辆入栈*/
		return 1;
	}
}
/*出栈*/
CarNode pop(SqStackCar *s)
{
	CarNode x;
	if (s->top < 0)
	{
		x.num = 0;
		x.arrtime.hour = 0;
		x.arrtime.minute = 0;
		return x;/*如果栈为空，返回空值*/
	}
	else
	{
		s->top--;
		return s->stack[s->top + 1];/*栈不为空，返回栈顶元素*/
	}
}

/*初始化队列*/
void InitLinkQueue(LinkQueueCar *q)
{
	q->front = (QueueNode*)malloc(sizeof(QueueNode));
	if (q->front != NULL)
	{
		q->rear = q->front;
		q->front->next = NULL;
		q->front->num = 0;  /*头结点的num保存队列中元素的个数*/
	}
}
/*入队*/
void EnLinkQueue(LinkQueueCar *q, int x)
{
	QueueNode *p;
	p = (QueueNode*)malloc(sizeof(QueueNode));
	p->num = x;
	p->next = NULL;
	q->rear->next = p;
	q->rear = p;
	q->front->num++;  /*队列元素个数加1*/
}
/*出队*/
int DeLinkQueue(LinkQueueCar *q)
{
	QueueNode *p;
	int n;
	if (q->front == q->rear)
	{
		return 0;/*队空返回0*/
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
		return n;/*返回出队信息*/
	}
}

/*************************************************车辆到达********************************************************/

//参数：停车栈，停车队列，车辆信息
//返回值：空
//功能：对传入的车辆进行入栈 栈满入队

void Arrive(SqStackCar *stop, LinkQueueCar *lq, CarNode x)
{
	int f;
	f = push(stop, x);/*入栈*/
	if (f == 0)/*栈满*/
	{
		EnLinkQueue(lq, x.num);/*入队*/
		printstop(0, lq->front->num, 0, 30);
		printflog(x.arrtime, x.num, 1, 'B', lq->front->num, 0);
		qingping(0);
		printf_s("您的车停在便道%d号车位上\n", lq->front->num);
	}
	else
	{
		printstop(1, stop->top + 1, 0, 30);
		printflog(x.arrtime, x.num, 1, 'P', stop->top + 1, 0);
		qingping(0);
		printf_s("您的车停在停车场%d号车位上\n", stop->top + 1);
	}
	qingping(1);
	printf_s("按任意键继续:");
	_getch();
}

/*************************************************车辆离开********************************************************/

//参数：停车栈指针s1，暂存栈指针s2，停车列队指针p，车辆信息x
//返回值：空
//功能：查找栈中s1的x并出栈，栈中没有则查队列p并出队，打印离开收费信息

void Leave(SqStackCar *s1, SqStackCar *s2, LinkQueueCar *p, CarNode x)
{
	double fee = 0;
	int position = s1->top + 1;/*车辆所在车位*/
	int n, f = 0;//n=1表示出队，否则是出栈；
	CarNode y;
	QueueNode *q;
	while ((s1->top > -1) && (f != 1))/*当栈不空且未找到x*/
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
	if (y.num == x.num)/*找到x*/
	{
		gotoxy(34, 19);//停车时长
		printf_s("%d:%-2d", (x.arrtime.hour - y.arrtime.hour), (x.arrtime.minute - y.arrtime.minute));
		fee = ((x.arrtime.hour - y.arrtime.hour) * 60 + (x.arrtime.minute - y.arrtime.minute))*PRICE + Baseprice;
		gotoxy(62, 19);//停车费
		printf_s("%2.1f元\n", fee);
		qingping(0);
		printf_s("确认您的车辆信息");
		qingping(1);
		printf_s("按任意键继续");
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
			printleave(p->front->num + 1, position, s1->top + 1);//出栈动画
			printflog(x.arrtime, x.num, 0, 'p', position, fee);
		}
		else
		{
			printleave(0, position, s1->top + 2);
			printflog(x.arrtime, x.num, 0, 'P', position, fee);
		}
	}
	else  //若栈中无x
	{
		while (s2->top > -1)  //还原栈
		{
			y = pop(s2);
			f = push(s1, y);
		}
		q = p->front;
		f = 0;
		position = 1;
		while (f == 0 && q->next != NULL)  //当队不为空且未找到x
		{
			if (q->next->num != x.num)
			{
				q = q->next;
				position++;
			}
			else  //找到x
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
				printf_s("0元");
				qingping(0);
				printf_s("您的车将离便道");
				qingping(1);
				printf_s("按任意键继续");
				_getch();
				printleave(-1, position, p->front->num + 1);//出队动画
				printflog(x.arrtime, x.num, 0, 'B', position, 0);
				f = 1;
			}
			if (f == 0)//未找到x
			{
				qingping(0);
				printf_s("没找到您的车");
				qingping(1);
				printf_s("按任意键继续");
				_getch();
			}
			
		}
	}
}
/*获取系统时间*/
//返回PTime类型

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
//移动光标到（x，y）点
void gotoxy(int x, int y)
{
	COORD coord;
	coord.X = x;
	coord.Y = y+3;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}
/*画图-画出系统界面*/
void panitPL()
{
	int i, j, x, y, a[2][4] = { 2,0,0,1,-2,0,0,-1 };
	gotoxy(20, 4);
	printf_s("***************************对话框****************************");
	x = 18, y = 6;//起点
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
			printf_s("》");
			
		}
		else
		{
			printf_s("《");
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
			printf_s("》");

		}
		else
		{
			printf_s("《");
		}
	}
	gotoxy(22, 8);
	printf_s("小张:");
	gotoxy(22, 11);
	printf_s("顾客:");
	gotoxy(22, 15);
	printf_s("***********************停车信息***********************");
	gotoxy(22, 17);
	printf_s("车牌号:");
	gotoxy(57,17);
	printf_s("时间:");
	gotoxy(22, 19);
	printf_s("停车时长:");
	gotoxy(57, 19);
	printf_s("收费:");
}

/*清屏函数*/
void qingping(int a)
{
	if (a == 0)
	{
		gotoxy(28, 8);
		printf_s(" ");
		gotoxy(28, 9);
		printf_s(" ");
		gotoxy(27, 8);//清除这一排的信息
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
		gotoxy(27, 11);//清除这一排的信息
		printf_s("                            ");
		gotoxy(27, 11);
	}
	else
	{
		//gotoxy(31, 17);//输入车牌号
		//printf_s(" ");
		//gotoxy(48, 17);//显示时间
		//printf_s(" ");
		//gotoxy(33, 19);
		//printf_s(" ");
		//gotoxy(48, 19);
		//printf_s(" ");
		gotoxy(35, 21);//按Esc退出系统
	}
}
/*用上下键选择*/
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
		if (c == 13)/*按Enter键返回当前值*/
		{
			return y - 10;
		}
		
		if (c != -32)/*不是方向键进行下次循环*/
			continue;
		
		c = _getch();
		if (c == 72)/*上*/
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
		if (c == 80)/*下*/
		{
			if (y == 13)
				y = 11;
			else
				y++;
		}
	}
}
/*输入车辆信息*/
CarNode getcarInfo()
{
	Ptime T;
	CarNode x;
	qingping(0);
	printf_s("请输入车牌号！\n");
	qingping(1);
	printf_s("请在下面输入车辆信息！\n");
	//qingping(2);
	gotoxy(30, 17);//显示车牌号
	scanf_s("%d", &(x.num));
	T = get_timer();
	x.arrtime = *T;
	gotoxy(62, 17);//显示时间
	printf_s("%d:%d", x.arrtime.hour, x.arrtime.minute);
	_getch();
	return x;
}
/*打印停车场*/
void printcar()
{
	gotoxy(0, 25);
	printf_s("========================================================================================┓\n");
	printf_s("                 出场暂放区 ||    1       2      3       4      5                        ||\n");
	printf_s("----------------------------------------------------------------------------             ||\n");
	printf_s("                         主车道                                                          ||\n");
	printf_s("----------------------------------------------------------------------------             ||\n");
	printf_s(" 12    11    10     9     8     7     6     5       4      3      2       1              ||\n");
	printf_s("=============================================================================╗           ||\n");
	printf_s("   ↑           『===========================================================』          || \n");
	printf_s("  便道 停车区 →|| 1       2         3         4          5          6          7        ||\n");
	printf_s("                ╚========================================================================╝\n\n");
	printf_s("                         管理日志\n\n");
	printf_s("时间    车牌号     进（1）/ 出（0）车位（B便道P停车场）收费（元）");
	
}
/*打印日志*/
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
	static char *car = "【■】";
	/*x0 = 0; 
	y0 = 27;*/
	int x = 0, y = 28;
	if (a == 0)//入
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
	static char *car = "【■】";
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
	printf_s("                                        停车场管理系统\n\n");
	printf_s("    ******************************************欢迎光临************************************************\n\n");
	printf_s("收费标准：基础费0.5元，每分钟收费0.8元，收费精确到0.1元\n");
	panitPL();
	printcar();
	gotoxy(0, -3);
	c = 0;
	while (1)
	{
		for (i = 2; i > -1; i--)
		{
			qingping(i);
			printf_s("按Esc退出系统");
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
				printf_s("        欢迎光临！");

				gotoxy(49, 9);
				printf_s("    请按↑↓选择需要的服务！  ");
				gotoxy(58, 11);
				printf_s("    1、我要停车");
				gotoxy(58, 12);
				printf_s("    2、我要取车  ");
				gotoxy(58, 13);
				printf_s("    3、结束  ");

				a = getkey();
				if (a == Exit)
				{
					printf_s("   结束服务。");
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