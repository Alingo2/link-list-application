#include "pch.h"
#include <iostream>
#include<cstdlib>
#include<ctime>
using namespace std;
struct term
{
	float coef;
	int expn;
	term(float c = 0, int e = 0)
	{
		coef = c;
		expn = e;
	}
};
struct Node
{
	term data;
	Node* next;
};
term* bubble(term*A,int n)								//冒泡排序（在打印前、多项式相乘之前进行升幂调整）
{
	for (int i=0;i<n-1;i++)
	{
		for (int j=0;j<n-1-i;j++)
		{
			if (A[j].expn > A[j + 1].expn)
			{
				term temp = A[j];
				A[j] = A[j + 1];
				A[j + 1] = temp;
			}
		}
	}
	return A;
}
class Polynomial
{
public:
	Polynomial(void)
	{
		front = new Node;
		front->next = NULL;
	}
	Polynomial(term listbefore[],int n)
	{
		num = n;
		term *list = bubble(listbefore, n);
		front = new Node;
		Node *temp = front;
		for (int i = 0;i < n;i++)
		{
			Node*p = new Node;
			p->data = list[i];
			temp->next = p;
			temp = p;
		}
		temp->next = NULL;
	}
	~Polynomial()
	{}
	void selfdestroy()			//自动的析构函数不太适用与循环 生存期太短 另外写了一个手动调用的析构函数
	{		
		Node*temp = front;
		while (front)
		{
			temp= front;
			front = front->next;
			delete temp;
		}
		front = NULL;
		cout << "clear has benn done" << endl;
			return;
	}
	Node* Getfirst()
	{
		return front;
	}
	Polynomial operator +(Polynomial &B)
	{
		Node*p_prior = front;
		Node*pA = front->next;
		Node*pB = B.Getfirst()->next;
		while (pA&&pB)
		{
			if (pA->data.expn < pB->data.expn)
			{
				p_prior = pA;
				pA = pA->next;
			}
			else if (pA->data.expn > pB->data.expn)
			{
				p_prior->next = pB;
				p_prior = pB;
				pB = pB->next;
				p_prior->next = pA;
			}
			else
			{
				pA->data.coef += pB->data.coef;
				if (fabs(pA->data.coef) < 1e-7)
				{
					p_prior->next = pA->next;
					delete pA;
					pA = p_prior->next;
				}
				else
				{
					p_prior = pA;
					pA = p_prior->next;
				}
				Node*temp = pB;
				pB = pB->next;
				delete temp;
			}
		}
		if (pB)
			p_prior->next = pB;
		B.Getfirst()->next = NULL;
		return *this;
	}
	Polynomial operator -(Polynomial &B)
	{
		Node*p_prior = front;
		Node*pA = front->next;
		Node*pB = B.Getfirst()->next;
		while (pA&&pB)
		{
			if (pA->data.expn < pB->data.expn)
			{
				p_prior = pA;
				pA = pA->next;
			}
			else if (pA->data.expn > pB->data.expn)
			{
				p_prior->next = pB;
				p_prior = pB;
				pB = pB->next;
				p_prior->next = pA;
			}
			else
			{
				pA->data.coef -= pB->data.coef;
				if (fabs(pA->data.coef) < 1e-7)
				{
					p_prior->next = pA->next;
					delete pA;
					pA = p_prior->next;
				}
				else
				{
					p_prior = pA;
					pA = p_prior->next;
				}
				Node*temp = pB;
				pB = pB->next;
				delete temp;
			}
		}
		if (pB)
			p_prior->next = pB;
		B.Getfirst()->next = NULL;
		return *this;
	}
	Polynomial operator *(Polynomial &B)
	{
		int tempnum = this->num*B.num;			//获得最大项数
		term *temp=new term[tempnum];
		Node*pA = front->next;
		int pos = 0;
		while (pA)								//大循环套小循环 逐项求乘积
		{
			Node*pB = B.Getfirst()->next;
			while (pB)
			{
				temp[pos].coef = pA->data.coef*pB->data.coef;					//将乘后的结果单独拿出储存
				temp[pos].expn = pA->data.expn+pB->data.expn;
				pos++;
				pB = pB->next;
			}
			pA = pA->next;
		}
		Polynomial out(temp, tempnum);		
		B.selfdestroy();							//乘法需要自己手动析构 加减法都是会析构掉链表的 传参数的时候也都是传的引用 不存在需要析构
		return out;
	}
	void Calculatex()						//计算该点的函数值，通过累加实现
	{
		double x;
		cin >> x;
		Node*p = front->next;
		double sum = 0;
		while (p)
		{
			sum += p->data.coef*pow(x, p->data.expn);
			p = p->next;
		}
		cout << sum << endl;
	}
	void differential()				//求导
	{
		int n;
		cin >> n;
		while (n--)
		{
			Node*p = front->next;
			Node*backup = front;				//跟进指针
			while (p)
			{
				if (fabs(p->data.expn) < 1e-7)
				{
					backup->next = p->next;			//删除 求导为零的项
					delete p;
					p = backup->next;
				}
				else
				{																//改变指数和系数
					p->data.coef *= p->data.expn;
					p->data.expn -= 1;
					p = p->next;
					backup = backup->next;
				}
			}
		}
	}
	void PrintLinst()							//打印函数，其中包含了对多项式的整理
	{
		Node *temp = front;
		while (temp)
		{
			if (temp->next)
			{
				if (temp->next->data.coef == 0)				//删去系数为0的项
				{
					Node*mid = temp->next;
					temp->next = mid->next;
					delete mid;
					continue;
				}
				if (temp->data.expn == temp->next->data.expn)			//合并同类项
				{
					temp->data.coef += temp->next->data.coef;
					Node* mid = temp->next;
					temp->next = mid->next;
					delete mid;
				}
			}
			temp = temp->next;
		}
		temp = front->next;
		while (temp)
		{
			if (temp != front->next&&temp->data.coef > 0)				//将第一项与后面项区分开（第一项为正不用带正号）
				cout << "+";																//正数项加个正号
			if (temp->data.coef != 0)
			{
				if (temp->data.expn != 0)
					cout << temp->data.coef << "x" << "^" << temp->data.expn << " ";
				else
					cout << temp->data.coef << " ";
			}
			temp = temp->next;
		}
			cout<<endl;
	}
private:
	Node* front;
	Node*rear;
	int num;
};
term* randonize(int testsize)
{
	term* test=new term[testsize];
	for (int i = 0;i < testsize;i++)
	{
		test[i].expn = rand() % 10;			//生成随机整数，需要在main函数中设置随机数种子
		float m = rand() % 100, n = rand() % 10;		//这里不写成float后面不会生成小数
		test[i].coef = rand() % 10 + m / 10 + n / 100;	//因为自带的随机函数生成的小数位数不易控制，所以自己对数进行除法再相加得小数
		if (rand() % 2)
			test[i].coef *= -1;				//生成随机正负的浮点数 ，随机正负号
	}
	return test;
}
Polynomial make()					//录入多项式各项参数
{
	int num;
	cin >> num;
	term* test = new term[num];
	for (int i = 0;i < num;i++)
	{
		int coef, expn;
		cin >> coef >> expn;
		test[i].coef = coef;
		test[i].expn = expn;
	}
	Polynomial Test(test, num);
	return Test;
}
void operation(Polynomial Test1)								//主调用函数（便于处理异常时返回程序）
{
	int num = NULL;
	bool judge = false;
	while (num != 6)
	{
		cout << "Now please enter the front number of your wanted operation" << endl;
		cin >> num;
		if (num >= 1 && num <= 3)
		{
			Polynomial Test2 = make();
			if (num == 1)
				Test1 = Test1 + Test2;
			else if (num == 2)
				Test1 = Test1 - Test2;
			else if (num == 3)
				Test1 = Test1 * Test2;
			Test1.PrintLinst();
		}
		else if (num == 4)
			Test1.Calculatex();
		else if (num == 5)
		{
			Test1.differential();
			Test1.PrintLinst();
		}
		else if (num != 1 && num != 2 && num != 3 && num != 4 && num != 5 && num != 6)
		{
			judge = true;
			break;
		}
	}
	if(judge)
		throw"Please give the correct number!";
}
void loop(Polynomial Test1)							//处理异常之后重新进入程序
{
	cout << "you have seen your stuff,would you like to operate?" << endl;
	cout << "here's the options you get:   " << endl;
	cout << "1.add an another polynomial" << endl;
	cout << "2.subtract an another polynomial" << endl;
	cout << "3.multiply an another polynomial" << endl;
	cout << "4.calculate the exact value of the polynomial 1" << endl;
	cout << "5.get the derivative of the polynomial 1" << endl;
	cout << "6.quit this program" << endl;
	try
	{
		operation(Test1);
	}
	catch (const char*error2)
	{
		cout << error2 << endl;
		operation(Test1);
	}
		Test1.selfdestroy();
		cout << "bye" << endl;
	return;
}
void selfputin()					//进入手动输入模式（打包成函数也是便于处理异常后再次调用）
{
	char putin2;
	cout << "OK,now you can write down a polynomial with following rules:" << endl;
	cout << "1.the length of your polynomial" << endl;
	cout << "2.the coefficient and the exponent for each one(with a space to separate)" << endl;
	Polynomial Test1 = make();
	cout << "perfect,so,would you like to see your polynomial 1?[y/n]" << endl;
	cin >> putin2;
	if (putin2 == 'y')
		Test1.PrintLinst();
	else
		cout << "fine......" << endl;
	loop(Test1);
	return;
}
int main()
{
	srand((unsigned)time(NULL));
	cout << "would you like to enter auto mode?[y/n]" << endl;
	char putin;
	try {
		cin >> putin;
		if (putin == 'y')				//调用随机函数  便于展示实例
		{
			cout << "This is the random listA:" << endl;
			term* testA = randonize(2);
			Polynomial listA(testA, 2);
			listA.PrintLinst();
			cout << "This is the random listB:" << endl;
			term*testB = randonize(1);
			Polynomial listB(testB, 1);
			listB.PrintLinst();
			cout << "This is the random listC:" << endl;
			term*testC = randonize(2);
			Polynomial listC(testC, 2);
			listC.PrintLinst();
			cout << "This is the outcome of the listA+listB" << endl;
			listA = listA + listB;
			listA.PrintLinst();
			cout << "This is the outcome of the listA*listC" << endl;
			listA = listA * listC;
			listA.PrintLinst();
			loop(listA);
		}
		else if (putin == 'n')
			selfputin();
		else
			throw"Please just chose one of the 'y' and 'n' !";
	}
	catch (const char *error)
	{
		cout << error << endl;
		selfputin();
	}
	return 0;
}