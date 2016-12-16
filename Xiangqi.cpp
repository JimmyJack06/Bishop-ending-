// Xiangqi.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <math.h>

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

int main()
{	
	//char c;
	//scanf("%c ", &c);
	//putchar('0');
	int n, bx, by;
	while (scanf("%d%d%d", &n, &bx, &by) && n != 0)
	{
		Chess chess(n, bx, by);
		chess.testOut();
		isCheckmate(chess);
	}
}

