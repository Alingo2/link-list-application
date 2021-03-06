#include "pch.h"
#include <iostream>
using namespace std;
struct node
{
	int data;
	node *next;
};
class circle
{
public:
	circle(int a[], int n)					//建立循环链表
	{
		front = new node;
		node *r = front;
		for (int i = 0;i < n;i++)			//采用尾插法插入
		{
			node *s = new node;
			s->data = a[i];
			r->next = s;
			r = s;
		}
		r->next = front->next;			//形成环链
	}
	void del(int n, int m)			//删除
	{
		node *mid = front->next;
		while (n)
		{
			for (int i = 0;i < m - 1;i++)			//移动m-1次（因为每隔m个人删一次）
				mid = mid->next;
			cout << mid->data << " ";		//先打印出队名
			node *q = mid->next;				//删除
			mid->data = q->data;
			mid->next = q->next;
			delete q;
			n--;
		}
	}
private:
	node *front;
};
int main()
{
	int n, m;
	cin >> n >> m;
	int *a = new int[n];
	for (int i = 0;i < n;i++)
		a[i] = i + 1;
	circle A(a, n);
	A.del(n, m);
	return 0;
}