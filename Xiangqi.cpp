// Xiangqi.cpp : 定义控制台应用程序的入口点。
//

//#include "stdafx.h"
#include <math.h>		//abs()
#include <malloc.h>		//malloc()
#include <string.h>		//strcmp()

#include <algorithm>	//sort,lower_bound
#include <vector>
#include <iostream>

#include <string>	//getline,string
#include <sstream>	//stringstream

#include <ctime>	//clock_t,clock()
using namespace std;

#define DEBUG

//chessBoard 10*9
#define HEIGHT 12
#define WIDTH 11
int DX[2] = { -1,1 };
int DY[2] = { -1,1 };

typedef struct Chess
{
	int bx, by, rx, ry;		//(bx,by):black G pos
	char board[HEIGHT * WIDTH];

	//init ChessBoard
	Chess(int n,int _bx,int _by):bx(_bx),by(_by) 
	{
		memset(board, 0, sizeof(board));
		board[bx * WIDTH + by] = 'Y';	//black General
		char type;
		for (int i = 0; i < n; i++)
		{
			int x, y;
			scanf(" %c", &type);
			scanf("%d%d", &x, &y);
 			board[x * WIDTH + y] = type;
			if (type == 'G')
			{
				rx = x;
				ry = y;
			}
		}
	}

	//black G move
	bool go(int dx,int dy)
	{
		if (bx + dx < 1 || bx + dx > 3 || by + dy < 4 || bx + dy > 6)
			return false;
		else
		{
			board[bx * WIDTH + by] = 0;
			bx += dx;
			by += dy;
			board[bx * WIDTH + by] = 'Y';
			return true;
		}
	}

	//count chess between (x,y) ~ (bx,by)
	int betweenCnt(int x, int y)
	{
		int Cnt = 0;
		if (x == bx)
		{
			if (y > by)
			{
				for (int i = by + 1; i < y; i++)
				{
					if (board[x * WIDTH + i])
						Cnt++;
				}
				return Cnt;
			}
			if (y < by)
			{
				for (int i = y + 1; i < by; i++)
				{
					if (board[x * WIDTH + i])
						Cnt++;
				}
				return Cnt;
			}
		}
		else if (y == by)
		{
			if (x > bx)
			{
				for (int i = bx + 1; i < x; i++)
				{
					if (board[i * WIDTH + y])
						Cnt++;
				}
				return Cnt;
			}
			if (x < bx)
			{
				for (int i = x + 1; i < bx; i++)
				{
					if (board[i * WIDTH + y])
						Cnt++;
				}
				return Cnt;
			}
		}
		else
			return Cnt;
	
	}

	//check if horse can eat black G
	bool horseGo(int x, int y)
	{
		//note if horse is stumbled?
		if (abs(x - bx) * abs(y - by) == 2)
		{
			if (abs(x - bx) == 2)
			{
				if (x > bx)
				{
					if (!board[(x - 1) * WIDTH + y])
						return true;
				}
				else
				{
					if (!board[(x + 1) * WIDTH + y])
						return true;
				}
			}
			if (abs(y - by) == 2)
			{
				if (y > by)
				{
					if (!board[x * WIDTH + y - 1])
						return true;
				}
				else
				{
					if (!board[x * WIDTH + y + 1])
						return true;
				}
			}
		}
		return false;
	}

	//check is Black lost
	bool isBlackLost()
	{
		for (int i = 1; i < HEIGHT - 1; i++)
		{
			for (int j = 1; j < WIDTH - 1; j++)
			{
				switch (board[i * WIDTH + j])
				{
				case 'G' : 
				case 'R':	
					if (i == bx || j == by)
					{
						if (betweenCnt(i, j) == 0)
							return true;
					}
					break;
				case 'C' :
					if (i == bx || j == by)
					{
						if (betweenCnt(i, j) == 1)
							return true;
					}
					break;
				case 'H' : 
					if (horseGo(i, j))
						return true;
					break;
				default:
					break;
				}
			}
		}
		return false;
	}

	//output ChessBoard 
	void testOut()
	{
		for (int i = 1; i < HEIGHT - 1; i++)
		{
			for (int j = 1; j < WIDTH - 1; j++)
			{
				printf("%c ", board[i * WIDTH + j]? board[i * WIDTH + j]:'0');
			}
			printf("\n");
		}
	}
}Chess;

//imitate black G 's movement ,and checkout 
bool isCheckmate(Chess &chessOri)
{
	for (int i = 0; i < 2; i++)
	{
		Chess tempBoard(chessOri);
		if (tempBoard.go(DX[i], 0) && !tempBoard.isBlackLost())
		{
			printf("NO\n");
			//tempBoard.testOut();
			return false;
		}
	}
	for (int i = 0; i < 2; i++)
	{
		Chess tempBoard(chessOri);
		if (tempBoard.go(0, DY[i]) && !tempBoard.isBlackLost())
		{
			printf("NO\n");
			//tempBoard.testOut();
			return false;
		}
	}
	printf("YES\n");
	return true;
}


typedef struct Squares
{
	int n;	//n*n dots
	int m;	//m  interconnecting line
	int *SH = (int*)malloc(n * (n - 1) * sizeof(int));
	int *SV = (int*)malloc((n - 1) * n * sizeof(int));
	int *Size = (int*)malloc(n * sizeof(int));

	//init Squares
	Squares(int _n,int _m):n(_n),m(_m)
	{
		memset(SH, 0, n * (n - 1) * sizeof(int));
		memset(SV, 0, (n - 1) * n * sizeof(int));
		memset(Size, 0, n * sizeof(int));
		for (int i = 0; i < m; i++)
		{
			char type;
			int x, y;
			scanf(" %c", &type);
			if (type == 'H')
			{
				scanf("%d %d", &y, &x);
				SH[(y - 1) * (n - 1) + (x - 1)] = 1;
			}
			else if (type == 'V')
			{
				scanf("%d %d", &x, &y);
				SV[(y - 1) * n + (x - 1)] = 1;
			}
		}
	}

	//~Squares()
	//{
	//	free(SH);		//corrupt!
	//	free(SV);
	//	free(Size);
	//}


	//count squares
	bool countSquares()
	{
		bool flag = false;
		//cycle each size   1~n-1
		for (int _size = 1; _size < n; _size++)
		{
			int cnt = 0;
			//cycle each SH(i,j)
			for (int i = 0; (i + _size) < n + 1 ; i++)
			{
				for (int j = 0; (j + _size) < (n - 1) + 1; j++)
				{
					int dEdge = 0;	//count 4 edge,dEdge == 4?
					
					//top
					if (SH[i * (n - 1) + j] == 1)
					{
						dEdge = 1;
						//find edge
						for (int _step = 1; _step < _size; _step++)
						{
							if (SH[i * (n - 1) + j + _step] == 1)
							{	
								dEdge = 1;
								continue;
							}
							else
							{
								dEdge = 0;
								break;
							}
						}
					}

					//bottom
					if (dEdge == 1)
					{
						if (SH[(i + _size) * (n - 1) + j] == 1)
						{
							dEdge = 2;
							for (int _step = 1; _step < _size; _step++)
							{
								if (SH[(i + _size) * (n - 1) + j + _step] == 1)
								{
									dEdge = 2;
									continue;
								}
								else
								{
									dEdge = 1;
									break;
								}
							}
						}
					}

					//left
					if (dEdge == 2)
					{
						if (SV[i * n + j] == 1)
						{
							dEdge = 3;
							for (int _step = 1; _step < _size; _step++)
							{
								if (SV[(i + _step) * n + j] == 1)
								{
									dEdge = 3;
									continue;
								}
								else
								{
									dEdge = 2;
									break;
								}
							}
						}
					}

					//right
					if (dEdge == 3)
					{
						if (SV[i * n + (j + _size)] == 1)
						{
							dEdge = 4;
							for (int _step = 1; _step < _size; _step++)
							{
								if (SV[(i + _step) * n + (j + _size)] == 1)
								{
									dEdge = 4;
									continue;
								}
								else
								{
									dEdge = 3;
									break;
								}
							}
						}
					}

					if (dEdge == 4)
					{
						cnt++;	//add num
					}
				}
			}
			Size[_size] = cnt;
			if (cnt)
				flag = true;
		}
		return flag;
	}

	//output Squares
	void testOut(int kase)
	{	
		////SH
		//printf("SH\n");
		//for (int i = 0; i < n; i++)
		//{
		//	for (int j = 0; j < n - 1; j++)
		//	{
		//		printf("%d ", SH[i * (n - 1) + j]);
		//	}
		//	printf("\n");
		//}

		////SV
		//printf("SV\n");
		//for (int i = 0; i < n - 1; i++)
		//{
		//	for (int j = 0; j < n; j++)
		//	{
		//		printf("%d ", SV[i * n + j]);
		//	}
		//	printf("\n");
		//}

		//answer
		if (countSquares())
		{
			for (int i = 1; i < n; i++)
			{
				if (Size[i])
				{
					printf("%d square (s) of size %d\n", Size[i], i);
				}
			}
		}
		else
		{
			printf("No completed squares can be found.\n");
		}
	}

	//other 's code
	// 接下来判断(x1,y1)-(x2,y2)能否连环  
	//int isSqu(int x1, int y1, int x2, int y2) {
	//	int px = x1, py = y1;
	//	while (py < y2) { if (!h[px][py]) return 0; py++; }
	//	while (px < x2) { if (!v[px][py]) return 0; px++; }
	//	while (py > y1) { if (!h[px][py - 1]) return 0; py--; }
	//	while (px > x1) { if (!v[px - 1][py]) return 0; px--; }
	//	return 1;
	//}

}Squares;


const int N = 20;
typedef struct Cube
{
	
	char str[N] = { 0 }, str1[N] = { 0 }, str2[N] = {0};		//init 0!!!
	int dir[6][6] = { {0,1,2,3,4,5},{1,5,2,3,0,4},{2,1,5,0,4,3},{3,1,0,5,4,2},{4,0,2,3,5,1},{5,4,2,3,1,0} };

	void readData()
	{
		while (scanf("%s",str) != EOF)
		{
			for (int i = 0; i < 6; i++)
				str1[i] = str[i];
			for (int i = 0; i < 6; i++)
				str2[i] = str[i + 6];
			if (this->judge())
				puts("TRUE");
			else
				puts("FALSE");
		}
	}

	bool judge()
	{
		char temp[N] = {0};
		//for 6 position
		for (int i = 0; i < 6; i++)
		{
			for (int j = 0; j < 6; j++)
			{
				temp[j] = str1[dir[i][j]];	//reset pos
			}
			//rotate 4 times,'0' and '5' insist
			for (int j = 0; j < 4; j++)
			{
				char cha;
				cha = temp[1];
				temp[1] = temp[2];
				temp[2] = temp[4];
				temp[4] = temp[3];
				temp[3] = cha;
				int t = strcmp(temp, str2);
				if (strcmp(temp, str2) == 0)
				{
					return true;
				}
			}
		}
		return false;
	}

}Cube;

const int Tmax = 10000;
typedef struct Stu
{
	int n;	//n students
	int *a = (int*)malloc(n * sizeof(int));
	int *b = (int*)malloc(n * sizeof(int));
	int *c = (int*)malloc(n * sizeof(int));
	int t = 1;	//cur_time

	Stu(int _n):n(_n)
	{
		memset(a, 0, n * sizeof(int));
		memset(b, 0, n * sizeof(int));
		memset(c, 0, n * sizeof(int));
		for (int i = 0; i < n;i++)
		{
			scanf("%d%d%d", a + i, b + i, c + i);
		}
		//for (int i = 0; i < n; i++)
		//{
		//	printf("%d %d %d\n", a[i], b[i], c[i]);
		//}
	}

	int process()
	{
		for (t; t < Tmax; t++)
		{
			int cnt_awake = 0;
			for (int i = 0; i < n; i++)
			{
				if (a[i] >= c[i])
					cnt_awake++;
			}
			if (cnt_awake == n)		//success
			{
				return t;
			}

			//use c[i] to save new state
			for (int i = 0; i < n; i++)
			{
				if (a[i] == c[i])
				{
					if (n - cnt_awake <= cnt_awake)
						c[i] = 0;
				}
				if (c[i] == a[i] + b[i])
					c[i] = 0;

				c[i]++;
			}
		}
		if (t == Tmax)
			return -1;
	}

}Stu;


typedef struct Pallet
{
	int row;
	int col;
}Pallet;
typedef struct Cuboid
{
	int width, length, height;
	int wl = 0, wh = 0, lh = 0;
	Pallet *pallet = (Pallet*)malloc(sizeof(Pallet) * 6);
	Cuboid()
	{
		for (int i = 0; i < 6; i++)
		{
			scanf("%d %d", &(pallet[i].row), &(pallet[i].col));
			if (pallet[i].row > pallet[i].col)
			{
				int temp = pallet[i].row;
				pallet[i].row = pallet[i].col;
				pallet[i].col = temp;
			}
		}
	}

	bool check()
	{
		width = pallet[0].row;
		length = pallet[0].col;
		if(pallet[1].row == width)
		return false;
	}
}Cuboid;

//int main()
//{	
//	//char c;
//	//scanf("%c ", &c);
//	//putchar('0');
//
//	//		uva1589			Chess				///////////////
//	//int n, bx, by;
//	//while (scanf("%d%d%d", &n, &bx, &by) && n != 0)
//	//{
//	//	Chess chess(n, bx, by);
//	//	//chess.testOut();
//	//	isCheckmate(chess);
//	//}
//	///////////////////////////////////////////////////////
//
//	//			uva201		Squares				///////////////
////#ifdef DEBUG  
////	freopen("Squares.in", "r", stdin);
////#endif // DEBUG
////	int n, m, kase = 0;;
////	while (scanf("%d%d",&n,&m)!= EOF )
////	{
////		if (kase) puts("\n**********************************\n");
////		printf("Problem #%d\n\n", ++kase);
////		Squares squares(n, m);
////		squares.testOut(kase);
////	}
//	///////////////////////////////////////////////////////
//
//	//          uva253  Cube Painting          ///////////////  
////#ifdef DEBUG  
////	freopen("Cube.in", "r", stdin);
////#endif // DEBUG
////	Cube cube;
////	cube.readData();
//	
//	//////////////////////////////////////////////////////////
//
//	//			uva12108 Extraordinarily Tired Student////////
//#ifdef DEBUG  
//	freopen("Student.in", "r", stdin);
//#endif // DEBUG
//	int n, k = 1;
//	while (scanf("%d",&n) && n)
//	{
//		Stu stu(n);
//		printf("Case %d: %d\n", k++, stu.process());
//	}
//	//////////////////////////////////////////////////////////
//}

//	uva 101	The blocks problem //////////////////////////////
//typedef struct Block
//{
//	int n;
//	vector < vector <int> > block;
//	string command;
//	string comA, comB;
//	int a, b;
//	
//	bool readData()
//	{
//		getline(cin, command);
//		if (command.compare("quit") == 0)
//			return false;
//		stringstream ss(command);
//		ss >> comA >> a >> comB >> b;
//		return true;
//	}
//
//	void init()
//	{
//		scanf("%d", &n);
//		block.resize(n);
//		for (int i = 0; i < n; i++)
//		{	
//			block[i].push_back(i);
//		}
//	}
//
//	bool moveOnto()
//	{
//		int posA = block[a].front();
//		int posB = block[b].front();
//		//checkout
//		for (int i = 0; i < block[posA].size(); i++)
//		{
//			if (block[posA][i] == b)
//				return false;
//		}
//		for (int i = 0; i < block[posB].size(); i++)
//		{
//			if (block[posB][i] == a)
//				return false;
//		}
//		//reset block[a]
//		for (int i = block[posA].size() - 1; i >= 0; i--)
//		{	
//			if (block[posA][i] == a)
//				break;
//			//reset!!!!!!!!!!!!!
//			block[block[posA][i]].clear();
//			block[block[posA][i]].push_back(block[posA][i]);//0:0,1:1  ....
//			block[posA].pop_back();
//		}
//		//reset block[b]
//		for (int i = block[posB].size() - 1; i >= 0; i--)
//		{
//			if (block[posB][i] == b)
//				break;
//			//reset!!!!!!!!!!!!!
//			block[block[posB][i]].clear();
//			block[block[posB][i]].push_back(block[posB][i]);//0:0,1:1  ....
//			block[posB].pop_back();
//		}
//		//move a onto b
//		block[posB].push_back(a);
//		block[posA].pop_back();
//		
//		//reset posA
//		block[a].clear();
//		block[a].push_back(posB);
//		return true;
//	}
//
//	bool moveOver()
//	{
//		int posA = block[a].front();
//		int posB = block[b].front();
//		//checkout
//		for (int i = 0; i < block[posA].size(); i++)
//		{
//			if (block[posA][i] == b)
//				return false;
//		}
//		for (int i = 0; i < block[posB].size(); i++)
//		{
//			if (block[posB][i] == a)
//				return false;
//		}
//		//reset block[a]
//		for (int i = block[posA].size() - 1; i >= 0; i--)
//		{
//			if (block[posA][i] == a)
//				break;
//			//reset!!!!!!!!!!!!!
//			block[block[posA][i]].clear();
//			block[block[posA][i]].push_back(block[posA][i]);//0:0,1:1  ....
//			block[posA].pop_back();
//		}
//		//move a over b
//		block[posB].push_back(a);
//		block[posA].pop_back();
//
//		//reset posA
//		block[a].clear();
//		block[a].push_back(posB);
//		return true;
//	}
//
//	bool pileOnto()
//	{
//		int posA = block[a].front();
//		int posB = block[b].front();
//		//checkout
//		for (int i = 0; i < block[posA].size(); i++)
//		{
//			if (block[posA][i] == b)
//				return false;
//		}
//		for (int i = 0; i < block[posB].size(); i++)
//		{
//			if (block[posB][i] == a)
//				return false;
//		}
//		//reset block[b]
//		for (int i = block[posB].size() - 1; i >= 0; i--)
//		{
//			if (block[posB][i] == b)
//				break;
//			//reset!!!!!!!!!!!!!
//			block[block[posB][i]].clear();
//			block[block[posB][i]].push_back(block[posB][i]);//0:0,1:1  ....
//			block[posB].pop_back();
//		}
//		//pile a onto b
//		vector <int> tempVec;
//		for (int i = block[posA].size() - 1; i >= 0; i--)
//		{
//			if (block[posA][i] == a)
//			{
//				block[posB].push_back(a);
//				block[posA].pop_back();
//				//reset posA
//				block[a].clear();
//				block[a].push_back(posB);
//
//				for (int i = tempVec.size() - 1; i >= 0; i--)
//				{
//					block[posB].push_back(tempVec[i]);
//
//					//reset pos !!!!!!!!!!!!!!
//					block[tempVec[i]].clear();
//					block[tempVec[i]].push_back(posB);
//
//					tempVec.pop_back();
//				}
//				break;
//			}
//			else
//			{
//				tempVec.push_back(block[posA][i]);
//				block[posA].pop_back();
//			}
//		}
//
//		return true;
//	}
//
//	bool pileOver()
//	{
//		int posA = block[a].front();
//		int posB = block[b].front();
//		//checkout
//		for (int i = 0; i < block[posA].size(); i++)
//		{
//			if (block[posA][i] == b)
//				return false;
//		}
//		for (int i = 0; i < block[posB].size(); i++)
//		{
//			if (block[posB][i] == a)
//				return false;
//		}
//		//pile a over b
//		vector <int> tempVec;
//		for (int i = block[posA].size() - 1; i >= 0; i--)
//		{
//			if (block[posA][i] == a)
//			{
//				block[posB].push_back(a);
//				block[posA].pop_back();
//				//reset posA
//				block[a].clear();
//				block[a].push_back(posB);
//
//				for (int i = tempVec.size() - 1; i >= 0; i--)
//				{
//					block[posB].push_back(tempVec[i]);
//
//					//reset pos !!!!!!!!!!!!!!
//					block[tempVec[i]].clear();
//					block[tempVec[i]].push_back(posB);
//
//					tempVec.pop_back();
//				}
//				break;
//			}
//			else
//			{
//				tempVec.push_back(block[posA][i]);
//				block[posA].pop_back();
//			}
//		}
//		return true;
//	}
//
//	bool process()
//	{
//		if (comA.compare("move") == 0)
//		{
//			if (comB.compare("onto") == 0)
//			{
//				moveOnto();
//			}
//			else if (comB.compare("over") == 0)
//			{
//				moveOver();
//			}
//		}
//		else if(comA.compare("pile") == 0)
//		{
//			if (comB.compare("onto") == 0)
//			{
//				pileOnto();
//			}
//			else if (comB.compare("over") == 0)
//			{
//				pileOver();
//			}
//		}
//		return true;
//	}
//
//	void print()
//	{
//		for (int i = 0; i < n; i++)
//		{
//			printf("%d:", i);
//			//clear flag
//			if (block[i].front() != i)
//				block[i].pop_back();
//			for (int j = 0; j < block[i].size(); j++)
//			{
//				printf(" %d", block[i][j]);
//			}
//			printf("\n");
//		}
//	}
//
//}Block;
//
//int main()
//{
//#ifdef DEBUG  
//	freopen("Block.in", "r", stdin);
//#endif // DEBUG
//	Block block1;
//	block1.init();
//	while (block1.readData())
//	{
//		block1.process();
//	}
//	block1.print();
//	return 0;
//}
/////////////////////////////////////////////////////////////


//	 156   Ananagrams////////////////////////////////////
//#include <map>
//map <string, int> cnt;		//count words times
//vector <string> words;		//save origin words
//
//string stdize(const string &s)
//{
//	string st = s;
//	for (int i = 0; i < st.length(); i++)
//	{
//		st[i] = tolower(st[i]);
//	}
//	sort( st.begin(), st.end() );
//	return st;
//}
//
//int main()
//{
//#ifdef DEBUG  
//	freopen("Ananagrams.in", "r", stdin);
//#endif // DEBUG
//	string s;
//	while (cin >> s)
//	{
//		if(s[0] == '#')	break;
//		words.push_back(s);
//		string st;
//		st = stdize(s);	//standardize
//		if (!cnt[st])
//		{
//			cnt[st]=0;
//		}
//		cnt[st]++;
//	}
//
//	vector <string> ans;	//因为输出要按顺序所以用vector
//	for (int i = 0; i < words.size(); i++)
//	{
//		if (cnt[stdize(words[i])] == 1)
//			ans.push_back(words[i]);
//	}
//	sort(ans.begin(), ans.end());
//	for (int i = 0; i < ans.size(); i++)
//	{
//		cout << ans[i] << endl;
//	}
//
//	map<int, int> num_score;
//	num_score[1] = 100;
//	num_score[2] = 99;
//	cout << num_score[1] << endl;
//}
////////////////////////////////////////////////////////////

// 210 Concurrency Simulator	////////////////////////////
//#include <queue>
//
//const int maxn = 1000;
//int n, Q;
//int var[26], t[5],ip[maxn];
//char prog[maxn][10];
//bool lock = false;
//
//queue <int> blockQ;
//deque <int>	readyQ;
//
//void run(int pid)
//{
//	int q = Q;	//each process time occupy
//
//	while (q > 0)
//	{
//		char *p = prog[ip[pid]];
//		switch (p[2])
//		{
//		case '=':	
//			var[p[0] - 'a'] = isdigit(p[5]) ? (p[4] - '0') * 10 + p[5] - '0' : p[4] - '0';
//			q -= t[0];
//			break;
//		case 'i':	//print
//			printf("%d: %d\n", pid+1, var[p[6] - 'a']);
//			q -= t[1];
//			break;
//		case 'c':  //lock
//			if (lock)
//			{
//				blockQ.push(pid);	//hang on
//				return;
//			}
//			lock = true;
//			q -= t[2];
//			break;
//		case 'l':  //unlock
//			lock = false;
//			if (!blockQ.empty())
//			{
//				int pid2 = blockQ.front();
//				blockQ.pop();
//				readyQ.push_front(pid2);	//thrust into readyQ 1st
//			}
//			q -= t[3];
//			break;
//		case 'd':	//end
//			return;	
//
//		}
//
//		ip[pid]++;	//next command
//	}
//	readyQ.push_back(pid);
//}
//
//int main()
//{
////#ifdef DEBUG  
////	freopen("Concurrency.in", "r", stdin);
////#endif // DEBUG
//
//	int T;
//	scanf("%d", &T);
//	while (T > 0)
//	{
//		scanf("%d %d %d %d %d %d %d", &n, &t[0], &t[1], &t[2], &t[3], &t[4], &Q);
//		memset(var, 0, sizeof(var));
//		getchar();
//
//		int line = 0;	//lineNo,global
//		//set commmand
//		for (int i = 0; i < n; i++)
//		{
//			fgets(prog[line], maxn, stdin);
//			ip[i] = line;
//			while (prog[line][2] != 'd')		//prog[line][0] != 'e'  wrong!!!!!!!!!!
//			{
//				line++;
//				fgets(prog[line], maxn, stdin);
//			}
//			line++;
//			readyQ.push_back(i);	//waiting line
//		}
//
//		//read commmand
//		while (!readyQ.empty())
//		{
//			int pid = readyQ.front();
//			readyQ.pop_front();
//			run(pid);
//		}
//		if (--T)
//			printf("\n");
//	}
//	return 0;
//}
///////////////////////////////////////////////////////////

//	11988 Broken KeyBoard	///////////////////////////////
//const int maxn = 100000 + 5;
//int linkNext[maxn];
//char s[maxn];
//int main()
//{
////#ifdef DEBUG
////	freopen("Broken_KeyBoard.in", "r", stdin);
////#endif // DEBUG
//	while (scanf("%s", s + 1) == 1)
//	{
//		memset(linkNext, 0, sizeof(linkNext));
//		int cur = 0, last = 0;
//		int n = strlen(s + 1);
//		for (int i = 1; i <= n; i++)
//		{
//			if (s[i] == '[')	cur = 0;
//			else if (s[i] == ']')	cur = last;
//			else 
//			{
//				linkNext[i] = linkNext[cur];
//				linkNext[cur] = i;
//				if (cur == last)	last = i;
//				cur = i;
//			}
//		}
//
//		for (int i = linkNext[0]; i != 0; i = linkNext[i])
//		{
//			printf("%c", s[i]);
//		}
//		printf("\n");
//	}
//	return 0;
//}
///////////////////////////////////////////////////////////

//	12657 Boxes in a line	///////////////////////////////
//#include <algorithm>
//const int maxn = 100000 + 5;
//int lkRight[maxn], lkLeft[maxn];
//
//void linkLR(int LX, int RX)
//{
//	lkRight[LX] = RX;
//	lkLeft[RX] = LX;
//}
//
//int main()
//{
////#ifdef DEBUG
////	freopen("Boxes_in_line.in", "r", stdin);
////	freopen("Boxes_in_line.out", "w", stdout);
////#endif // DEBUG
//	
//
//	int kase = 1;
//	int n, m;
//	while (scanf("%d%d\n", &n, &m) == 2)
//	{
//		//printf("m = %d\n", m);
//		//init
//		memset(lkLeft, 0, sizeof(lkLeft));
//		memset(lkRight, 0, sizeof(lkRight));
//		lkRight[0] = 1;
//		lkLeft[0] = n;
//		for (int i = 1; i <= n; i++)
//		{
//			lkLeft[i] = i - 1;
//			if(i != n)
//				lkRight[i] = i + 1;
//		}
//
//		int op, x, y;
//		bool inv = false;
//		for (int i = 0; i < m; i++)
//		{
//			scanf("%d", &op);
//			if (op == 4)	inv = !inv;
//			else
//			{
//				scanf("%d%d\n", &x, &y);
//				//if (op == 3 && )
//				if (op != 3  && inv)	op = 3 - op;	//for 1,2
//				if (op == 1 && lkRight[x] == y)	continue;
//				if(op == 2 && lkLeft[x] == y)	continue;
//
//				int LX = lkLeft[x], RX = lkRight[x], 
//					LY = lkLeft[y], RY = lkRight[y];
//				if (op == 3)
//				{
//					if (RX == y)
//					{
//						lkLeft[x] = y;
//						lkRight[x] = RY;
//						lkLeft[y] = LX;
//						lkRight[y] = x;
//						//yicuo!!!
//						lkRight[LX] = y;
//						lkLeft[RY] = x;
//						//continue;		//too more		(don't break !!!!!!!!!)
//					}
//					else if (LX == y)
//					{
//						lkLeft[x] = LY;
//						lkRight[x] = y;
//						lkLeft[y] = x;
//						lkRight[y] = RX;
//						//yicuo!!!
//						lkRight[LY] = x;
//						lkLeft[RX] = y;
//						//continue;
//					}
//					else
//					{
//						linkLR(LY, x); linkLR(x, RY);
//						linkLR(LX, y); linkLR(y, RX);
//					}
//				}
//				if (op == 1)
//				{
//					linkLR(LX, RX); linkLR(LY, x); linkLR(x, y);
//				}
//				if (op == 2)
//				{
//					linkLR(LX, RX); linkLR(y, x); linkLR(x, RY);
//				}
//			}
//			//test
//			/*if (!inv)
//			{
//				for (int i = lkRight[0]; i != 0; i = lkRight[i])
//				{
//					printf("%d ", i);
//				}
//				printf("\n");
//			}
//			else
//			{
//				for (int i = lkLeft[0]; i != 0; i = lkLeft[i])
//				{
//					printf("%d ", i);
//				}
//				printf("\n");
//			}*/
//		}
//
//		//output
//		long long sum = 0;		//"int" not enough
//		if (!inv)
//		{
//			int pos = 1;
//			for (int i = lkRight[0]; i != 0; i = lkRight[i])
//			{
//				if (pos % 2 == 1)
//					sum += i;
//				pos++;
//			}
//		}
//		else
//		{
//			int pos = 1;
//			for (int i = lkLeft[0]; i != 0; i = lkLeft[i])
//			{
//				if (pos % 2 == 1)
//					sum += i;
//				pos++;
//			}
//		}
//		printf("Case %d: %lld\n", kase++, sum);
//	}
//	return 0;
//}
///////////////////////////////////////////////////////////

// 122 trees on the level	//////////////////////////////
#include<vector>
#include<queue>
const int maxn = 300;
typedef struct Node
{
	bool has_val;
	Node *leftNode, *rightNode;
	int value;

}Node;

bool addNode(Node *root ,int v, char *s)
{
	int n = strlen(s);
	Node* curNode = root;
	for (int i = 0; i < n; i++)		//i < n -1 ?
	{
		if (s[i] == 'L')
		{
			if (curNode->leftNode == NULL)
			{
				curNode->leftNode = (Node*)malloc(sizeof(Node));
				curNode->leftNode->has_val = false;
				curNode->leftNode->leftNode = NULL;
				curNode->leftNode->rightNode = NULL;
			}
			curNode = curNode->leftNode;
		}
		else if (s[i] == 'R')
		{
			if (curNode->rightNode == NULL)
			{
				curNode->rightNode = (Node*)malloc(sizeof(Node));
				curNode->rightNode->has_val = false;
				curNode->rightNode->leftNode = NULL;
				curNode->rightNode->rightNode = NULL;
			}
			curNode = curNode->rightNode;
		}
	}
	if (curNode->has_val)
		return false;
	curNode->value = v;
	curNode->has_val = true;
	return true;
}

bool readInput(Node * &root)
{
	root = (Node*)malloc(sizeof(Node));
	root->has_val = false;
	root->leftNode = NULL;
	root->rightNode = NULL;
	char s[maxn];
	while (scanf("%s", s) != EOF)
	{
		if (strcmp(s, "()") == 0)
			return true;
		char *start = strchr(s, ',') + 1;
		int v;
		sscanf(&s[1], "%d", &v);
		if (!addNode(root, v, start))
			return false;
	}
	return false;
}

bool bfs(Node *curNode,vector <int> &ans)
{
	queue <Node*> q;
	if (!curNode->has_val)
	{
		return false;
	}
	q.push(curNode);
	while (!q.empty())
	{
		Node *u = q.front();
		q.pop();
		if (u->leftNode != NULL)
			q.push(u->leftNode);
		if (u->rightNode != NULL)
			q.push(u->rightNode);
		if (!u->has_val)
			return false;
		ans.push_back(u->value);
	}
	return true;
}

int main()
{
//#ifdef DEBUG
//	freopen("trees_on_the_level.in", "r", stdin);
//	freopen("trees_on_the_level.out", "w", stdout);
//#endif // DEBUG
	Node* root;
	while (readInput(root))
	{
		vector <int> ans;
		if (!bfs(root, ans))
			printf("not complete\n");
		else
		{
			for (int i = 0; i < ans.size(); i++)
				printf("%d ", ans[i]);
			printf("\n");
		}
	}
	return 0;
}

//////////////////////////////////////////////////////////

//	linear solve test	//////////////////////////////////
//#include <mkl.h>
//float A[16] = { 2,3,11,5,1,1,5,2,2,1,3,2,1,1,3,3 };
//float B[4] = { 2,1,-3,-2 };
//int main()
//{
//	MKL_INT ipiv[4];
//	LAPACKE_sgesv(LAPACK_ROW_MAJOR, 4, 1, A, 4, ipiv,
//		B, 1);
//	//LAPACKE_sgels(LAPACK_ROW_MAJOR, 'N', 4, 4, 1, A, 4, B, 1);
//	for (int i = 0; i < 4; i++)
//		printf("%f\n", B[i]);
//	float c = 2 * B[0] + 3 * B[1] + 11 * B[2] + 5 * B[3];
//	printf("%f\n", c);
//	return 0;
//}

//////////////////////////////////////////////////////////
