// Xiangqi.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <math.h>
#include <malloc.h>

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
			printf("one solution: \n");
			tempBoard.testOut();
			return false;
		}
	}
	for (int i = 0; i < 2; i++)
	{
		Chess tempBoard(chessOri);
		if (tempBoard.go(0, DY[i]) && !tempBoard.isBlackLost())
		{
			printf("one solution: \n");
			tempBoard.testOut();
			return false;
		}
	}
	printf("black is lost\n");
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
		//SH
		printf("SH\n");
		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < n - 1; j++)
			{
				printf("%d ", SH[i * (n - 1) + j]);
			}
			printf("\n");
		}

		//SV
		printf("SV\n");
		for (int i = 0; i < n - 1; i++)
		{
			for (int j = 0; j < n; j++)
			{
				printf("%d ", SV[i * n + j]);
			}
			printf("\n");
		}

		//answer
		printf("Problem #%d\n\n", kase);
		if (countSquares())
		{
			for (int i = 1; i < n; i++)
			{
				if (Size[i])
				{
					printf("%d squares (s) of size %d\n", Size[i], i);
				}
			}
			printf("\n**********************\n");
		}
		else
		{
			printf("No completed squares can be found.\n\n**********************\n");
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


int main()
{	
	//char c;
	//scanf("%c ", &c);
	//putchar('0');

	//					Chess				///////////////
	//int n, bx, by;
	//while (scanf("%d%d%d", &n, &bx, &by) && n != 0)
	//{
	//	Chess chess(n, bx, by);
	//	chess.testOut();
	//	isCheckmate(chess);
	//}
	///////////////////////////////////////////////////////

	//					Squares				///////////////
#ifdef DEBUG  
	freopen("Squares.in", "r", stdin);
#endif // DEBUG
	int n, m, kase = 0;;
	while (scanf("%d%d",&n,&m)!= EOF )
	{
		kase++;
		scanf("%d%d", &n, &m);
		Squares squares(n, m);
		squares.testOut(kase);
	}
	///////////////////////////////////////////////////////

}

