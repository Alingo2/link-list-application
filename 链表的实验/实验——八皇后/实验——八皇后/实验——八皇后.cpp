#include "pch.h"
#include <iostream>
#include<Windows.h>
using namespace std;
int finalnum = 0;
void print(int**maze, int n)				//打印棋盘
{
	for (int i = 0;i < n;i++)
	{
		for (int j = 0;j < n;j++)
		{
			if (maze[j][i] == 0)
				cout << "▓";
			else if (maze[j][i] == 1)
				cout << "♀";
		}
		cout << endl;
	}
}
bool findplace(int **maze,int n,int x,int y)			//判断该点能否放置皇后
{
	for (int i = 0;i < y;i++)
	{
		if (maze[x][i]==1)							//检查该列是否有皇后（不用检查行，因为一行放一个就会去下一行）
			return false;
		if (x - 1 - i >= 0 && maze[x - 1 - i][y - 1 - i]==1)			//检查该点的左上方对角线是否有皇后 （同样不用往下方检查）
			return false;
		if (x + 1 + i < n&&maze[x + 1 + i][y - 1 - i]==1)			//检查该点的右上方对角线是否有皇后（同样不用往下方检查）
			return false;
	}
	return true;
}
bool putqueen(int **maze,int n,int row)
{
	if (row == n)								//如果达到了n行，就打印，并把总计数增加，因为要找出全部路径，所以又设置此格不行，回溯
	{
		print(maze, n);
		cout << endl << endl;
		finalnum++;
		return false;
	}
	for (int i = 0;i < n;i++)
	{
		for (int x = 0;x < n;x++)
			maze[x][row] = 0;				//将有可能存在的上一次赋值清空
		if (findplace(maze, n, i,row))
		{
			maze[i][row] = 1;				//如果该位置可以放  就置为1，再看下一步
			putqueen(maze, n, row + 1);
		}
	}
	return false;
}
int main()
{
	int n;
	cin >> n;
	int **maze = new int*[n];			//动态申请二维数组
	for (int i = 0;i < n;i++)
		maze[i] = new int[n];
	for (int i = 0;i < n;i++)
		for (int j = 0;j < n;j++)
			maze[i][j] = 0;
	putqueen(maze, n, 0);
	cout << finalnum;
    return 0; 
}