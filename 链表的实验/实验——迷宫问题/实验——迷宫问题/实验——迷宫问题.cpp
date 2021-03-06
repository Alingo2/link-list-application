#include "pch.h"
#include <iostream>
#include<stack>
#include<cstdlib>
#include<ctime>
#include <windows.h>
using namespace std;
const int n = 12;
struct Point
{
	int x, y;
};
void print(int a[n][n])
{
	for (int i = 0;i < n;i++)
	{
		for (int j = 0;j < n;j++)
		{
			if (a[i][j] == 0)
				cout << "▓";
			else if (a[i][j] == 1 || a[i][j] == -1)
				cout << "  ";
			else if (a[i][j] == 3)
				cout << "ㄖ";
			else if (a[i][j] == 4)
				cout << "♂";
		}
		cout << endl;
	}
}
void Maze(int arr[n][n], Point cur, const Point end)
{
	Point Stack[1024];
	int top = -1, condition(1);
	do
	{
		condition = 1;
		arr[cur.x][cur.y] = 4;					//设置可以走，设为4会打印出行走轨迹
		Stack[++top].x = cur.x;				//入栈
		Stack[top].y = cur.y;
		if (arr[cur.x][cur.y + 1] == 3 || arr[cur.x + 1][cur.y] == 3 || arr[cur.x][cur.y - 1] == 3 || arr[cur.x - 1][cur.y] == 3)//有出口直接out
			break;
		if (arr[cur.x][cur.y + 1] == 1)				//各个方向探索  还能再优化一下  先判断在上还是在下 调整 探索顺序 可以选择向上或是向下探索
			cur.y++;
		else if (arr[cur.x + 1][cur.y] == 1)
			cur.x++;
		else if (arr[cur.x - 1][cur.y] == 1)
			cur.x--;
		else if (arr[cur.x][cur.y - 1] == 1)
			cur.y--;
		else														//此路不同，修改标记，出栈，把打印调节置为否
		{
			arr[Stack[top].x][Stack[top].y] = -1;
			top--;
			cur.x = Stack[top].x;
			cur.y = Stack[top].y;
			top--;
			condition = 0;
		}
		if (condition)											//如果 走出了这一步，修改标记，然后清屏，再重新打印出新的地图
		{
			arr[Stack[top].x][Stack[top].y] = 4;
			system("cls");
			print(arr);
			//Sleep(500);
		}
	} while ((top != -1) && ((cur.x != end.x) || (cur.y != end.y)));
	cout << "out" << endl;
}
int main()
{
	int a[n][n];
	for (int i = 0;i < n;i++)
		for (int j = 0;j < n;j++)
			a[i][j] = 1;								//初始化
	for (int i = 0;i < n;i++)
		a[0][i] = a[n - 1][i] = a[i][0] = a[i][n - 1] = 0;//设置边界；
	srand((unsigned)time(NULL));
	for (int i = 0;i < n + 3;i++)						//设置路障
		a[rand() % (n - 2) + 1][rand() % (n - 2) + 1] = 0;
	Point start, end;										//随机化出口、入口
	start.x = rand() % (n - 2) + 1;
	start.y = rand() % (n - 2) + 1;
	end.x = rand() % (n - 2) + 1;
	end.y = rand() % (n - 2) + 1;
	const Point End = end;
	a[start.x][start.y] = 4;
	a[End.x][End.y] = 3;
	print(a);
	Sleep(500);
	system("cls");
	Maze(a, start, End);
	return 0;
}