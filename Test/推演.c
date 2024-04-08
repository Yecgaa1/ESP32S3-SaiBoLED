#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define MAXBALL 63
#define Long 9
#define Deep 13
typedef unsigned char uint8_t;

typedef struct LedInfo {
	uint8_t R;
	uint8_t G;
	uint8_t B;
	uint8_t index;
} LedInfo;


LedInfo ChangeList[1000][100]; //第X轮推演，修改的第Y个灯的信息


struct shouldChange {
	uint8_t x;
	uint8_t y;
} ShouldChange[10], ShouldDrop[100];

int ShouldChangeI = 0;
int nCanChange = 0;
int ShouldDropI = 0;
int NowDropI = 0;

void LedOn(LedInfo *Info) {
	Info->R = 200;
	Info->G = 200;
	Info->B = 200;
}

void LedOff(LedInfo *Info) {
	Info->R = 0;
	Info->G = 0;
	Info->B = 0;
}

uint8_t GetIndexDown(const uint8_t x, const uint8_t y) {
	switch (y)
	{
		case 0:
			return 0;
		case 1:
			return 1;
		case 2:
			return 1 + x - 2;
		case 3:
			return 1 + 3 + x - 1;
		case 4:
			return 1 + 3 + 5 + x - 1;
		default:
			return 14 + (y - 5) * 7 + x;
	}
}

uint8_t GetIndexUp(const uint8_t x, const uint8_t y) {
	switch (y)
	{
		case 0:
			return 0;
		case 1:
			return x;
		case 2:
			return 7 + x;
		case 3:
			return 14 + x;
		case 4:
			return 21 + x;
		case 5:
			return 28 + x;
		case 6:
			return 35 + x;
		case 7:
			return 42 + x;
		case 8:
			return 49 + x - 1;
		case 9:
			return 49 + 5 + x - 1;
		case 10:
			return 49 + 10 + x - 1;
		case 11:
			return 49 + 10 + 3 + x - 1;
		case 12:
			return 0;
	}
}

int main() {
	printf("Hello, World!\n");

	uint8_t down[13][9] = {
		{2, 2, 2, 2, 2, 2, 2, 2, 2},
		/*  初始化索引号为 0 的行 */
		{2, 2, 2, 2, 0, 2, 2, 2, 2},
		/*  初始化索引号为 1 的行 */
		{2, 2, 2, 0, 0, 0, 2, 2, 2},
		/*  初始化索引号为 2 的行 */
		{2, 2, 0, 0, 0, 0, 0, 2, 2},
		/*  初始化索引号为 3 的行 */
		{2, 2, 0, 0, 0, 0, 0, 2, 2},
		/*  初始化索引号为 4 的行 */
		{2, 0, 0, 0, 0, 0, 0, 0, 2},
		/*  初始化索引号为 5 的行 */
		{2, 0, 0, 0, 0, 0, 0, 0, 2},
		/*  初始化索引号为 6 的行 */
		{2, 0, 0, 0, 0, 0, 0, 0, 2},
		/*  初始化索引号为 7 的行 */
		{2, 0, 0, 0, 0, 0, 0, 0, 2},
		/*  初始化索引号为 8 的行 */
		{2, 0, 0, 0, 0, 0, 0, 0, 2},
		/*  初始化索引号为 9 的行 */
		{2, 0, 0, 0, 0, 0, 0, 0, 2},
		/*  初始化索引号为 10 的行 */
		{2, 0, 0, 0, 0, 0, 0, 0, 2},
		/*  初始化索引号为 11 的行 */
		{2, 2, 2, 2, 2, 2, 2, 2, 2},
		/*  初始化索引号为 12 的行 */
	};

	uint8_t up[13][9] = {
		{2, 2, 2, 2, 2, 2, 2, 2, 2},
		/*  初始化索引号为 0 的行 */
		{2, 1, 1, 1, 1, 1, 1, 1, 2},
		/*  初始化索引号为 1 的行 */
		{2, 1, 1, 1, 1, 1, 1, 1, 2},
		/*  初始化索引号为 2 的行 */
		{2, 1, 1, 1, 1, 1, 1, 1, 2},
		/*  初始化索引号为 3 的行 */
		{2, 1, 1, 1, 1, 1, 1, 1, 2},
		/*  初始化索引号为 4 的行 */
		{2, 1, 1, 1, 1, 1, 1, 1, 2},
		/*  初始化索引号为 5 的行 */
		{2, 1, 1, 1, 1, 1, 1, 1, 2},
		/*  初始化索引号为 6 的行 */
		{2, 1, 1, 1, 1, 1, 1, 1, 2},
		/*  初始化索引号为 7 的行 */
		{2, 2, 1, 1, 1, 1, 1, 2, 2},
		/*  初始化索引号为 8 的行 */
		{2, 2, 1, 1, 1, 1, 1, 2, 2},
		/*  初始化索引号为 9 的行 */
		{2, 2, 2, 1, 1, 1, 2, 2, 2},
		/*  初始化索引号为 11 的行 */
		{2, 2, 2, 2, 1, 2, 2, 2, 2},
		/*  初始化索引号为 11 的行 */
		{2, 2, 2, 2, 2, 2, 2, 2, 2},
		/*  初始化索引号为 12 的行 */
	};

	uint8_t upForPrint[13][9] = {
		{2, 2, 2, 2, 2, 2, 2, 2, 2},
		/*  初始化索引号为 0 的行 */
		{2, 1, 1, 1, 1, 1, 1, 1, 2},
		/*  初始化索引号为 1 的行 */
		{2, 1, 1, 1, 1, 1, 1, 1, 2},
		/*  初始化索引号为 2 的行 */
		{2, 1, 1, 1, 1, 1, 1, 1, 2},
		/*  初始化索引号为 3 的行 */
		{2, 1, 1, 1, 1, 1, 1, 1, 2},
		/*  初始化索引号为 4 的行 */
		{2, 1, 1, 1, 1, 1, 1, 1, 2},
		/*  初始化索引号为 5 的行 */
		{2, 1, 1, 1, 1, 1, 1, 1, 2},
		/*  初始化索引号为 6 的行 */
		{2, 1, 1, 1, 1, 1, 1, 1, 2},
		/*  初始化索引号为 7 的行 */
		{2, 2, 1, 1, 1, 1, 1, 2, 2},
		/*  初始化索引号为 8 的行 */
		{2, 2, 1, 1, 1, 1, 1, 2, 2},
		/*  初始化索引号为 9 的行 */
		{2, 2, 2, 1, 1, 1, 2, 2, 2},
		/*  初始化索引号为 11 的行 */
		{2, 2, 2, 2, 1, 2, 2, 2, 2},
		/*  初始化索引号为 11 的行 */
		{2, 2, 2, 2, 2, 2, 2, 2, 2},
		/*  初始化索引号为 12 的行 */
	};

	uint8_t maxDeep = 2;
	for (int ball = 0; ball < 63; ball++)
	{
		struct shouldChange CanChange[20];
		int nCanChange = 0;

		//按深度查找可以被剔除的球
		for (int deep = 1; deep <= maxDeep; deep++)
		{
			for (int j = 1; j < Long; j++)
			{
				if (up[deep][j] == 1) //判断这个球可否被移除而不影响其他
				{
					if (up[deep - 1][j - 1] != 1 &&
					    up[deep - 1][j] != 1 &&
					    up[deep - 1][j + 1] != 1)
					{
						CanChange[nCanChange].x = j;
						CanChange[nCanChange].y = deep;
						nCanChange++;

						//更新maxDeep
						if (maxDeep == deep)
							maxDeep++;
					}
				}
			}
		}

		if (nCanChange == 0)
			break;
		//随机移除一个球
		const int a = rand() % nCanChange;
		//记录
		ShouldDrop[ShouldDropI].x = CanChange[a].x;
		ShouldDrop[ShouldDropI].y = CanChange[a].y;
		ShouldDropI++;
		up[CanChange[a].y][CanChange[a].x] = 0;
		//打印现在的表
		//遍历打印up
		for (int i = 0; i < 13; i++)
		{
			for (int j = 0; j < 9; j++)
			{
				if (up[i][j] == 2)
				{
					printf("  ");
				} else
				{
					printf("%d ", up[i][j]);
				}
			}
			printf("\n");
		}
	}

	//清空推演数组
	for (int i = 0; i < 10; i++)
	{
		ShouldChange[i].x = 0;
		ShouldChange[i].y = 0;
	}

	for (int range = 0; range < 500; range++) //推演下落1000轮
	{
		int doRange = 0;


		//是否已经没有需要推演的
		if (ShouldChangeI == 0)
		{
			//是否已经结束
			if (NowDropI == ShouldDropI)
				return 0;
			//添加要推演的存在
			upForPrint[ShouldDrop[NowDropI].y][ShouldDrop[NowDropI].
				x] = 0;
			NowDropI++;

			down[1][4] = 1;
			ShouldChange[0].x = 4;
			ShouldChange[0].y = 1;
			ShouldChangeI = 1;
			LedOn(&ChangeList[range][doRange]);
			ChangeList[range][doRange].index = GetIndexDown(
				4, 1);
			doRange++;
		}

		//遍历打印up down
		for (int i = 0; i < 12; i++)
		{
			for (int j = 0; j < 9; j++)
			{
				if (upForPrint[i][j] == 2)
				{
					printf("  ");
				} else
				{
					printf("%d ", upForPrint[i][j]);
				}
			}
			printf("\n");
		}
		for (int i = 0; i < 13; i++)
		{
			for (int j = 0; j < 9; j++)
			{
				if (down[i][j] == 2)
				{
					printf("  ");
				} else
				{
					printf("%d ", down[i][j]);
				}
			}
			printf("\n");
		}

		if (ShouldChangeI != 0)
		{
			for (int i = 0; i < 10; i++)
			{
				//确定为被推演对象
				if (ShouldChange[i].x != 0)
				{
					switch (down[ShouldChange[i].y + 1][
						ShouldChange[i].x])
					{
						case 0: //下落
							down[ShouldChange[i].y + 1][
								ShouldChange[i].x] = 1;
							down[ShouldChange[i].y][
								ShouldChange[i].x] = 0;
							LedOff(
								&ChangeList[range][
									doRange]);
							ChangeList[range][doRange].index
									= GetIndexDown(
										ShouldChange[i].x,
										ShouldChange[i].y);
							doRange++;
							ShouldChange[i].y++;
							LedOn(
								&ChangeList[range][
									doRange]);
							ChangeList[range][doRange].index
									= GetIndexDown(
										ShouldChange[i].x,
										ShouldChange[i].y);
							doRange++;
							break;;
						case 1: //有东西
							bool R =
									down[ShouldChange[i].y +
										1][ShouldChange[i].x
										+ 1] == 0;
							bool L =
									down[ShouldChange[i].y +
										1][ShouldChange[i].x
										- 1] == 0;
							if (R && L) //左右均有位置
							{
								if (rand() % 2 == 0) //右
								{
									R = true;
									L = false;
								} else
								{
									R = false;
									L = true;
								}
							}
							if (R) //左落
							{
								down[ShouldChange[i].y + 1][
											ShouldChange[i].
											x + 1] =
										1;
								down[ShouldChange[i].y][
									ShouldChange[i].x] = 0;
								LedOff(
									&ChangeList[range][
										doRange]);
								ChangeList[range][doRange].
										index =
										GetIndexDown(
											ShouldChange[i].
											x, ShouldChange[
												i].y);
								doRange++;
								ShouldChange[i].y++;
								ShouldChange[i].x++;
								LedOff(
									&ChangeList[range][
										doRange]);
								ChangeList[range][doRange].
										index =
										GetIndexDown(
											ShouldChange[i].
											x, ShouldChange[
												i].y);
								doRange++;
								break;
							}
							if (L) //右落
							{
								down[ShouldChange[i].y + 1][
											ShouldChange[i].
											x - 1] =
										1;
								down[ShouldChange[i].y][
									ShouldChange[i].x] = 0;
								LedOff(
									&ChangeList[range][
										doRange]);
								ChangeList[range][doRange].
										index =
										GetIndexDown(
											ShouldChange[i].
											x, ShouldChange[
												i].y);
								doRange++;
								ShouldChange[i].y++;
								ShouldChange[i].x--;
								LedOff(
									&ChangeList[range][
										doRange]);
								ChangeList[range][doRange].
										index =
										GetIndexDown(
											ShouldChange[i].
											x, ShouldChange[
												i].y);
								doRange++;
								break;
							}
						//允许左右都没位置下滑
						case 2:
						{
							ShouldChange[i].x = 0;
							ShouldChange[i].y = 0;
							ShouldChangeI -= 1;
							if (doRange == 0)
							{
								//相当于这一轮啥都没干，
								range--;
							}
							break;
						}
					}
				}
			}
		}
	}

	return 0;
}
