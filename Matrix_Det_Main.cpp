/* 
2018.10.17 
程序初版本基本框架建立  
？？限制：int，n<100 
0.90B

2018.10.17
读入模块（最大阶数固定）完成 
预计：错误代码模块 
0.91B

2018.10.17
读入模块
修正了
“在2-j列读入时temp1换行未清零
导致Mmain[i][j](i>2) 出现累加”的bug  
0.92B

2018.10.24
读入模块
添加了读取识别负数的功能
并且可以在部分语法上进行判断
定义了主计算矩阵为全局变量
0.93B 

2018.10.25
读入模块
添加了对于-0的语法判断句
！： 在第n（n！=1）行输入的非法数据k-会以errorcode3错误返回（0.99B更新） 
！！： 在第n（n！=1）行输入合法数据-k（不为最后一个元素）会以errorcode3错误返回 （已解决） 
0.931B 

2018.10.26
计算模块 
定义了处理矩阵和数个全局变量
删除了在程序中定义的局部变量 
将阶数为1/2的情况纳入了考虑
！：对全局变量使用的准确性有待确认 
读入模块
修正了0.931B中出现的合法数据报错的bug 
！：对于非法输入（阶数不符合初始stage的情况）无法判断，且尚不明确得出结果的具体计算过程。 
0.94B 

2018.10.29
计算模块
确定核心算法为初等行变换后对角累乘
计算模块基本框架完成
！！：fatal error-进行3*3矩阵测试时，Mmain_P[2][2]返回NaN，可能原因为uzero出错（返回inf） （已解决） 
！！：未对 uzero分母为0的情况进行规避 （已解决） 
读入模块
读入子程序名修改
根据算法需要重新定义矩阵为浮点型（输入仍限制为整数，读取算法限制）
0.95B 

2018.10.30
计算模块
修复了0.95B中出现的NaN,inf错误（系数分母定位失误，未能保持对角元素作为分母）
仍未规避uzero在初等行变换中遇到下一行元素为0的情况（0分母导致NaN） 
计划对code/errorcode进行调整，考虑使用全局变量
？计算模块是否有返回错误值的可能和必要
0.96B 

2018.10.30
计算模块
进一步修复了分母定位点错误的bug
修复了输出-0的bug
！！：116-119行  uzero=0; 算法存在问题 ，未能有效解决0分母的问题 （已解决） 
0.961B 

2018.10.31
计算模块
增加了行交换模块，有效避免了分母为0的情况
！对于零矩阵 输出错误 NaN（已解决） 
！！（1111/2222/3333/4444）输出错误  （已解决） 
？考虑开发循环计算模块，以省去system-pause语句 （已解决） 
0.98B 

2018.10.31
计算模块
修复了对于O矩阵输出NaN及类似情况的bug
？拓展功能能否实现输出每一步的运算过程 
读入模块 
！！： 0.931B中出现的k-问题，（n=1||<stage）程序无法停止读取 
！！！：对于大于100的单元素矩阵错误读取 （已解决） 
0.99B 

2018.10.31
读入模块
修复了0.99B中出现的读取bug（算法中错误使用10的阶乘 ） 
停用dec变量 
0.991B 

2018.11.01
计算循环控制模块
建立
添加了fflush函数，程序仅兼容Windows系统 
主执行
调整了函数的执行方式 
1.0 

2018.11.02
错误代码输出模块 
调整了部分提示语句
1.01 

2018.12.09
计算模块
修正了某些情况下计算结果仍输出-0的错误输出
暂时无法确定强制判断int操作是否会对正确结果造成影响 
1.10 (fatal error)

2018.12.19 
计算模块
修复了1.10中算法错误，修改了非整数结果的强制转换方法 
1.11 
*/

//for Windows Series Systems 
#define _CRT_SECURE_NO_WARNINGS		//VS环境兼容性语句 
#include<stdio.h>
#include<math.h>
//#include<stdlib.h> 				//可能需要加入system("pause");语句，视后续功能开发情况而定 
#define minusone -1					//-1常量 
#define zero 0						//0常量 
#define dec 10						//10常量 
double Mmain[100][100];				//定义主计算矩阵为全局变量，便于子程序修改 ，限定n阶最高为100，数据类型为整形 ，Mmain为主计算矩阵  
double Mmain_process[100][100];		//定义主处理矩阵 
int stage;							//定义全局阶数变量 ,初始阶数stage为0
double det=1;						//det（限制为int） 
/* int Matrix_Det_Aleft(int a[100],int stageleft)		//行列式脱壳程序 （停用） 
{
	for (int i=0;i<stageleft-1;i++)
	{
		for (int j=0;j<stageleft-1;j++)
		{
			a[i][j]=a[i+1][j+1]; 						//存在语法错误，指针与int冲突 
		}
	}
	stageleft--;
	return a[100],stageleft;
} */
void Matrix_Det_Exc(int n);			//行列式初等变换程序（i->j）
int Matrix_Det_Cal()				//行列式计算程序 
{
	//int det;
	//extern int Mmain[100][100];
	det=1;							//清空上一次计算的数据 
	switch (stage)					//两种特殊情况 
	{
		case 1:
			{
				det=Mmain[0][0];
				return 0;
			}
		case 2:										//考虑到计算效率，保留2阶矩阵的直接算法 
			{
				det=Mmain[0][0]*Mmain[1][1]-Mmain[1][0]*Mmain[0][1];
				return 0;
			}
	};
	//printf("%d %d\n%d %d",Mmain[0][0],Mmain[0][1],Mmain[1][0],Mmain[1][1]);			//for test only 18/10/26 
	double uzero=0;				//系数控制变量     
	for (int j=0;j<stage-1;j++)					//核心算法 
	{
		for (int i=j;i<stage-1;i++)
		{
			if  (Mmain_process[j][j]==0)
			{
				Matrix_Det_Exc(j);
			}
			uzero=Mmain_process[i+1][j]/Mmain_process[j][j]*minusone;			//确定系数 
			Mmain_process[i+1][j]=0;											//划为行阶梯型 
			for (int deduct=j+1;deduct<stage;deduct++)
			{
				Mmain_process[i+1][deduct]=Mmain_process[i+1][deduct]+uzero*Mmain_process[j][deduct];		//初等行变换 ki+j 
			}
		}
	}
	force:if (det==0)								//规避NaN错误，强制输出0，label force未使用 
	{
		return det;
	 } 
	for (int i=0;i<stage;i++)
	{
		det=det*Mmain_process[i][i];					//行列式对角累乘 
	}
	return det;
 } 
 
void Matrix_Det_Exc(int n)						//行列式初等变换程序（i->j） 
{
	double Mtemp[100];
	for (int i=1;i<stage-n;i++)					//此层循环用于定位不为0的列 
	{
		if (Mmain_process[n+i][n]!=0)
		{
			for (int exc=0;exc<stage;exc++)			//exc为列定位点，用于两行数据交换 
			{
				Mtemp[exc]=Mmain_process[n+i][exc];
				Mmain_process[n+i][exc]=Mmain_process[n][exc];
				Mmain_process[n][exc]=Mtemp[exc];
			}
			det=det*(-1);
			break;
		}
		if (n+i+1==stage)					//如果始终无法找到非零同列元素，则说明det=0 
		{
			det=0;							//对于NaN情况，该赋值语句不够有效 ，所以添加了force语句 
		}
	}
}
 
int Matrix_Det_Input()				//行列式主程序 （读入） 
{
	//int Mmain[100][100];			//限定n阶最高为100，数据类型为整形 ，Mmain为主计算矩阵 
	//int Mmain_process[100][100];		//主处理矩阵 
	fflush(stdin);					//清空缓冲区 
	int temp=0,temp1=0;				//temp1,temp=读入数组临时变量，使用temp1=temp1*10^x+temp 方法读入 (ASCII-48)
	int /*stage=0,*/minus=0;					// 初始阶数stage为0,负数控制变量minus 
	stage=0;
	for (int i=0/*dec=0*/;;i++)		//第一行矩阵输入，获取矩阵阶数stage，i为输入次数计数变量，dec表示幂计数 
	{
		scanf("%c",&temp);			//temp为输入读取区域，仅在temp为数字的情况下，方读入temp1中 
		if ((temp>57||temp<48)&&temp!=' '&&temp!='\n'&&temp!='-')			//程序结构优化的可能：使用switch函数替换for函数 
		{
			return 1;
		}
		else
		{
			if (temp!='\n')
			{
				if (temp==' ')							//改进：利用枚举结构判断是否合法 
				{
					if (minus>1)						//判断负数输入语法 
					{
						return 2;
					}
					Mmain[0][stage]=temp1*pow(minusone,minus);		//遇到空格将temp1赋值进入数组 
					if (Mmain[0][stage]==0&&minus!=0)
					{
						return 3;					//负数语法判别语句，-0非法 
					} 
					//dec=0;
					temp1=0;
					minus=0;					//释放临时数据 dec，temp1，minus 
					stage++;					//遇到空格说明某数输入完全，此时阶数+1 
				}
				else
				{
					if (temp=='-') 					//合法输入-应该出现在空格之后 
					{
						minus++;
					}
					else
					{	
						temp1=temp1*dec+(temp-48);
						//dec++;
					}
				}
			}
			else
			{
				if (minus>1)						//判断负数输入语法 
				{
					return 2;
				}
				Mmain[0][stage]=temp1*pow(minusone,minus);			//如果遇到回车即将temp1的值赋予最后一个数组数值，并当即停止读取 
				if (Mmain[0][stage]==0&&minus!=0)
				{
					return 3;									//负数语法判别语句，-0非法 
				} 
				stage++;
				temp=0;
				temp1=0;
				minus=0; 
				break;
			}
		}
	}
	for (int i=1;i<=stage-1;i++)				//读取2-stage阶的矩阵 ,原理同上 
	{
		for (int j=0/*,dec=0*/;;)
		{
			scanf("%c",&temp);
			if ((temp>57||temp<48)&&temp!=' '&&temp!='\n'&&temp!='-')					//考虑使用switch语句 
			{
				return 1;
			}
			else
			{
				if (temp!='\n')
				{
					if (temp==' ')
					{
						if (minus>1)
						{
							return 2;
						}
						Mmain[i][j]=temp1*pow(minusone,minus);	
						if (Mmain[i][j]==0&&minus!=0)
						{
							return 3;													//负数语法判别语句，-0非法 
					 	} 	
						//dec=0;
						temp1=0;
						minus=0;
						j++;
					}
					else
					{
						if (temp=='-')
						{
							minus++;
						}
						else
						{
							temp1=temp1*dec+(temp-48);
							//dec++;
						}
					}
				} 
				else 
				{
					if (minus>1)						//判断负数输入语法 
					{
						return 2;
					}
					Mmain[i][stage-1]=temp1*pow(minusone,minus);
					if (Mmain[i][stage-1]==0&&minus!=0)
					{
						return 3;									//负数语法判别语句，-0非法 
					 } 
					temp1=0;
					minus=0;
					break;
				}
			}
		}
	}
	//printf("%d %d\n%d %d",Mmain[0][0],Mmain[0][1],Mmain[1][0],Mmain[1][1]);    //for test only 18/10/17
	//printf("%d",stage); 	//for test only 18/10/17
	
	/*for (int i=0;i<stage;i++)			//for test only 18/10/17 
	{
		for (int j=0;j<stage;j++)
		{
			printf("%d ",Mmain[i][j]);
		}
		printf("\n");
	}  */ 
	
	for (int i=0;i<stage;i++)						//主矩阵备份 
	{
		for (int j=0;j<stage;j++)
		{
			Mmain_process[i][j]=Mmain[i][j];
		}
	}
	Matrix_Det_Cal();
	if (det==-0||abs(det)<1)							//避免输出-0（输出-0）的情况有多种，1是本身即为-0，而是为一个极小的负浮点数（double）无法被完全储存 
	{
		det=0;
	}
	printf("det=%.0lf\n",det);
	return 0;
 } 
 
void error_code(int errorcode)					//错误代码输出模块 
 {
 	switch(errorcode)
 	{
 		case 1:
 			{
 				printf("Illegal Input, Please only input numbers(integer).\n");
 				break;
			 }
		case 2:
			{
				printf("Illeagal Input, code (--).\n");						//-- 
				break;
			}
		case 3:
			{
				printf("Illegal Input, code -0. \n");					//-0
				break;
			}
		default:
			{
				printf("Unexpected Error.\n");
				break;
			}
	 };
 }

int Re_Cal(char s);							//计算循环控制模块 
int main()
 {
 	char ds='y'; 
 	int code;
 	printf("Version Info: 1.11\nRelease Date: 2018/12/19\n\n");
 	for (;ds=='y';)
 	{
 		printf("Input Matrix(100*100 Maximum):\n");
 		code=Matrix_Det_Input();					
		if (code!=0)
 		{
 			error_code(code);
	 	}
	 	printf("Press y for next calculation, n to exit.\n");
	 	do
	 	{
	 		fflush(stdin);			//覆盖回车符 
	 		ds=getchar();					//直到正确输入为止 
		 }
		 while (Re_Cal(ds)=='n');
	 }
 	return 0;
 }
 
int Re_Cal(char s)
{
	switch (s)
	{
		case 'y':
			{
				return 1;
			}
		case 'n':
			{
				return 0;
			}
		default:
			{
				printf("Illegal Input, Press y for next calculation, n to exit.\n");
				return 'n';
			}
	};
}
 
