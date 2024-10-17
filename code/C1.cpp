#define _CRT_SECURE_NO_WARNINGS
//依赖文件mark.tet，EasyX图形库才能正常运行
//且要将设置中的”终端“改为”windows控制台主机“
#include<graphics.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include<time.h>
#include<conio.h>
#include<Windows.h>
#define width 800
#define height 800

int emptyflag = 1, collideflag = 0, defaultflag = 0, blocktype = 0;
int mark_old, mark_now;
int old_state = 0, new_state = 0;
int arr[10][22][2];
void roundbox(int x, int y, int size_x, int size_y);
void puttexttobox(int x, int y, int sizex, int sizey, const char* text);
void showUI();
void putblock(int x, int y, int z);
void createbutton(int x, int y, int sizex, int sizey, const char* text);
int showmark();
int getselect();
int changecolor(int x, int y, int w, int h);
void gameloop(int gamestate);
void playgame();
void pausegame();
void continuegame();
void restartgame();
int collide();
int isempty();
void eliminate();
void blockdrop();
int createblock();
int isdefault();
void initialize();
void showblock();
void leftmove();
void rightmove();
void accelerate();
void revolve();
int revolveblock(int blocktype, int blockstate);
int judgeisable(int blocktype, int blockstate);
int getblockstate(int blocktype);

int main()
{
	initgraph(width, height, SHOWCONSOLE);
	showUI();
	while (true) {
		showUI();
		new_state = getselect();
		if (new_state) {
			old_state = new_state;
			gameloop(new_state);
		}
		else gameloop(old_state);
	}
	closegraph();
	system("pause");
	return 0;
}
void roundbox(int x, int y, int size_x, int size_y) {
	if (changecolor(x, y, size_x, size_y)) {
		setfillcolor(RGB(126, 234, 209));
	}
	else setfillcolor(WHITE);
	fillroundrect(x, y, x + size_x, y + size_y, 10, 10);
}
void puttexttobox(int x, int y, int sizex, int sizey, const char* text) {
	settextcolor(BLACK);
	settextstyle(25, 0, "楷体");
	setbkmode(TRANSPARENT);
	outtextxy(x + sizex / 2 - textwidth(text) / 2, y + sizey / 2 - textheight(text) / 2, text);
}
void showUI() {
	BeginBatchDraw();//解决屏闪
	HWND hnd = GetHWnd();
	SetWindowText(hnd, "俄罗斯方块");//是由于字符集导致的，更改字符集
	setbkcolor(WHITE);
	cleardevice();
	setlinecolor(RGB(135, 206, 250));
	line(200, 0, 200, 800);
	line(600, 0, 600, 800);
	for (int i = 200; i <= 600; i += 40) {
		line(i, 0, i, 800);
	}
	for (int j = 0; j < 800; j += 40) {
		line(200, j, 600, j);
	}
	setfillcolor(RGB(135, 206, 250));
	fillrectangle(0, 0, 200, 800);
	fillrectangle(600, 0, 800, 800);
	createbutton(635, 100, 130, 60, "开始游戏");
	createbutton(635, 220, 130, 60, "暂停游戏");
	createbutton(635, 340, 130, 60, "继续游戏");
	createbutton(635, 460, 130, 60, "重新开始");
	createbutton(635, 580, 130, 60, "退出游戏");
	showmark();
	showblock();
	EndBatchDraw();//解决屏闪
}
void putblock(int x, int y, int z) {
	switch (z) {
	case 1:
		setfillcolor(RGB(255, 0, 0));
		fillroundrect(x, y, x + 40, y + 40, 10, 10);
		break;
	case 2:
		setfillcolor(RGB(0, 0, 255));
		fillroundrect(x, y, x + 40, y + 40, 10, 10);
		break;
	case 3:
		setfillcolor(RGB(255, 255, 0));
		fillroundrect(x, y, x + 40, y + 40, 10, 10);
		break;
	case 4:
		setfillcolor(RGB(0, 255, 0));
		fillroundrect(x, y, x + 40, y + 40, 10, 10);
		break;
	case 5:
		setfillcolor(RGB(128, 0, 128));
		fillroundrect(x, y, x + 40, y + 40, 10, 10);
		break;
	case 6:
		setfillcolor(RGB(255, 165, 0));
		fillroundrect(x, y, x + 40, y + 40, 10, 10);
		break;
	}
}
void createbutton(int x, int y, int sizex, int sizey, const char* text) {
	roundbox(x, y, sizex, sizey);
	puttexttobox(x, y, sizex, sizey, text);
}
int showmark() {
	int flag = 0;
	FILE* fp;
	fp = fopen("mark.txt", "r");
	if (!fp) {
		printf("无法打开文件");
		return 1;
	}
	fscanf(fp, "%d", &mark_old);
	if (mark_now > mark_old) {
		flag = 1;
		mark_old = mark_now;
	}
	char str[20];
	sprintf(str, "%d", mark_old);//
	fclose(fp);
	createbutton(35, 100, 130, 60, "最高分数");
	createbutton(35, 260, 130, 60, str);
	sprintf(str, "%d", mark_now);//
	createbutton(35, 420, 130, 60, "当前分数");
	createbutton(35, 580, 130, 60, str);
	if (flag) {
		fp = fopen("mark.txt", "w");
		if (!fp) {
			printf("无法打开文件");
			return 1;
		}
		fprintf(fp, "%d", mark_old);
		fclose(fp);
	}
}
int getselect() {
	if (MouseHit()) {
		MOUSEMSG mm = GetMouseMsg();
		if (mm.uMsg == WM_LBUTTONDOWN) {//获取鼠标点击消息
			if (mm.x >= 635 && mm.x <= 765 && mm.y >= 100 && mm.y <= 160) {
				return 1;
			}
			else if (mm.x >= 635 && mm.x <= 765 && mm.y >= 220 && mm.y <= 280) {
				return 2;
			}
			else if (mm.x >= 635 && mm.x <= 765 && mm.y >= 340 && mm.y <= 400) {
				return 3;
			}
			else if (mm.x >= 635 && mm.x <= 765 && mm.y >= 460 && mm.y <= 520) {
				return 4;
			}
			else if (mm.x >= 635 && mm.x <= 765 && mm.y >= 580 && mm.y <= 640) {
				return 5;
			}
			else return 0;
		}
		else return 0;
	}
	else return 0;
}
int changecolor(int x, int y, int w, int h) {
	//如果用GetMouseMsg()加MOUSEMSG，在鼠标无操作时会造成获取到的鼠标信息不正确
	HWND hwnd = GetHWnd();  // 获取 EasyX 窗口的句柄
	POINT cursorPos, clientPos;
	GetCursorPos(&cursorPos);  // 获取鼠标屏幕坐标
	ScreenToClient(hwnd, &cursorPos);  // 转换为窗口坐标
	if (cursorPos.x >= x && cursorPos.x <= x + w && cursorPos.y >= y && cursorPos.y <= y + h) {
		return 1;
	}
	return 0;
}
void gameloop(int gamestate) {
	switch (gamestate) {
	case 1:playgame();
		break;
	case 2:pausegame();
		break;
	case 3:continuegame();
		break;
	case 4:restartgame();
		break;
	case 5:exit(1);
		break;
	}
}
void playgame() {
	char ch;
	if (collide())collideflag = 1;
	else collideflag = 0;
	if (!defaultflag) {
		if (emptyflag || collideflag) {
			eliminate();
			emptyflag = 0;
			blocktype = createblock();
			if (!collide())mark_now += 10;
			if (isdefault()) {
				defaultflag = 1;
			}
		}
		else {
			blockdrop();
		}
	}

	cleardevice();
	showUI();
	for (int i = 0; i < 50; i++) {//用于下落时间间隔设置
		showUI();
		new_state = getselect();
		if (new_state && new_state != 1 && new_state != 3) {
			old_state = new_state; break;
		}
		//用这个会卡顿
		//ExMessage m;
		//m = getmessage();
		//默认终端为让Windows决定时时只能控制台窗口在前才能检测到键盘输入，除非更改终端为Windows控制台主机
		if (!defaultflag) {
			if (_kbhit()) {
				ch = _getch();
				switch (ch) {
				case 'a':
				case 'A':leftmove();
					break;
				case 's':
				case 'S':accelerate();
					break;
				case 'd':
				case 'D':rightmove();
					break;
				case 'w':
				case 'W':revolve();
					break;
				}
			}
		}
		//过于灵敏
		/*	ch=getkey();
			switch (ch) {
			case 'a':printf("left\n");
				break;
			case 's':printf("quick\n");
				break;
			case 'd':printf("right\n");
				break;
			case 'w':printf("revolve\n");
				break;
			}*/
	}

}
int collide() {
	for (int i = 0; i < 10; i++) {
		for (int j = 21; j > 0; j--) {
			if (arr[i][j][0] == 1 && arr[i][j - 1][0] == 2 || arr[i][21][0] == 2) {
				for (int i = 0; i < 10; i++) {
					for (int j = 0; j < 22; j++) {
						if (arr[i][j][0])arr[i][j][0] = 1;
					}
				}
				return 1;
			}
		}
	}
	return 0;
}
int isdefault() {
	if (collideflag) {
		for (int i = 0; i < 10; i++) {
			for (int j = 0; j < 2; j++) {
				if (arr[i][j][0] == 1)return 1;
			}
		}
	}
	return 0;
}
int createblock() {
	srand((unsigned int)time(NULL));
	switch (rand() % 7) {
	case 0:
		arr[3][1][0] = arr[4][1][0] = arr[5][1][0] = arr[6][1][0] = 2;
		arr[3][1][1] = arr[4][1][1] = arr[5][1][1] = arr[6][1][1] = rand() % 6 + 1;
		return 0;//I类
	case 1:
		arr[4][0][0] = arr[4][1][0] = arr[5][1][0] = arr[6][1][0] = 2;
		arr[4][0][1] = arr[4][1][1] = arr[5][1][1] = arr[6][1][1] = rand() % 6 + 1;
		return 1;//L类
	case 2:
		arr[6][0][0] = arr[4][1][0] = arr[5][1][0] = arr[6][1][0] = 2;
		arr[6][0][1] = arr[4][1][1] = arr[5][1][1] = arr[6][1][1] = rand() % 6 + 1;
		return 2;//J类
	case 3:
		arr[4][0][0] = arr[5][0][0] = arr[5][1][0] = arr[6][1][0] = 2;
		arr[4][0][1] = arr[5][0][1] = arr[5][1][1] = arr[6][1][1] = rand() % 6 + 1;
		return 3;//Z类
	case 4:
		arr[4][1][0] = arr[5][1][0] = arr[5][0][0] = arr[6][0][0] = 2;
		arr[4][1][1] = arr[5][1][1] = arr[5][0][1] = arr[6][0][1] = rand() % 6 + 1;
		return 4;//S类
	case 5:
		arr[4][1][0] = arr[5][1][0] = arr[5][0][0] = arr[6][1][0] = 2;
		arr[4][1][1] = arr[5][1][1] = arr[5][0][1] = arr[6][1][1] = rand() % 6 + 1;
		return 5;//T类
	case 6:
		arr[4][0][0] = arr[4][1][0] = arr[5][0][0] = arr[5][1][0] = 2;
		arr[4][0][1] = arr[4][1][1] = arr[5][0][1] = arr[5][1][1] = rand() % 6 + 1;
		return 6;//O类
	}
}
void blockdrop() {
	if (!collideflag) {
		for (int i = 0; i < 10; i++) {
			for (int j = 21; j > -1; j--) {
				if (arr[i][j][0] == 2) {
					arr[i][j + 1][0] = 2;
					arr[i][j][0] = 0;
					arr[i][j + 1][1] = arr[i][j][1];
				}
			}
		}
	}
}
void eliminate() {
	int flag;
	int line = 0;
	for (int i = 21; i > -1; i--) {
		flag = 1;
		for (int j = 0; j < 10; j++) {
			if (arr[j][i][0] == 0 || arr[j][i][0] == 2) {
				flag = 0; break;
			}
		}
		if (flag) {
			line++;
			for (int k = i; k > 0; k--) {
				for (int l = 0; l < 10; l++) {
					arr[l][k][0] = arr[l][k - 1][0];
					arr[l][k][1] = arr[l][k - 1][1];
				}
			}
			for (int m = 0; m < 10; m++) {
				arr[m][0][0] = 0;
				arr[m][0][1] = 0;
			}
			i++;
		}
	}
	if (line) mark_now += pow(2, line - 1) * 100;
	cleardevice();
	showUI();
}
void pausegame() {
	while (true) {
		showUI();
		new_state = getselect();
		if (new_state) {
			old_state = new_state; break;
		}
	}

}
void continuegame() {
	Sleep(100);//制造停顿
	old_state = 1;
}
void restartgame() {
	initialize();
	old_state = 1;
	defaultflag = 0;
}
void showblock() {
	for (int i = 0; i < 10; i++) {
		for (int j = 2; j < 22; j++) {
			if (arr[i][j][0])putblock(200 + i * 40, (j - 2) * 40, arr[i][j][1]);
		}
	}
}
void initialize() {
	emptyflag = 1;
	mark_now = 0;
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 22; j++) {
			arr[i][j][0] = 0;
			arr[i][j][1] = 0;
		}
	}
}
void leftmove() {
	int flag = 1;
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 22; j++) {
			if (arr[0][j][0] == 2 || (arr[i][j][0] == 2 && arr[i - 1][j][0] == 1))flag = 0;
		}
	}
	if (flag) {
		for (int i = 0; i < 22; i++) {
			for (int j = 0; j < 10; j++) {
				if (arr[j][i][0] == 2) {
					arr[j - 1][i][0] = 2;
					arr[j][i][0] = 0;
					arr[j - 1][i][1] = arr[j][i][1];
				}
			}
		}
	}
	cleardevice();
	showUI();
}
void rightmove() {
	int flag = 1;
	for (int i = 10; i > -1; i--) {
		for (int j = 0; j < 22; j++) {
			if (arr[9][j][0] == 2 || (arr[i][j][0] == 2 && arr[i + 1][j][0] == 1))flag = 0;
		}
	}
	if (flag) {
		for (int i = 0; i < 22; i++) {
			for (int j = 10; j > -1; j--) {
				if (arr[j][i][0] == 2) {
					arr[j + 1][i][0] = 2;
					arr[j][i][0] = 0;
					arr[j + 1][i][1] = arr[j][i][1];
				}
			}
		}
	}
	cleardevice();
	showUI();
}
void accelerate() {
	if (collide()) {
		collideflag = 1;
	}
	else collideflag = 0;
	if (!collideflag) {
		blockdrop();
	}
	else {
		eliminate();
		blocktype = createblock();
		if (!collide())mark_now += 10;
	}
	if (isdefault()) {
		defaultflag = 1;
	}
	cleardevice();
	showUI();
}
void revolve() {
	if (blocktype != 6) {
		int blockstate = getblockstate(blocktype);
		if (judgeisable(blocktype, blockstate)) {
			revolveblock(blocktype, blockstate);
			cleardevice();
			showUI();
		}
	}
}
int getblockstate(int blocktype) {
	int line = 0, row = 0;
	switch (blocktype) {
	case 0:
		for (int i = 0; i < 10; i++) {
			for (int j = 0; j < 22; j++) {
				if (arr[i][j][0] == 2) {
					for (int k = i; k < 10; k++)if (arr[k][j][0] == 2)line++;
					for (int l = j; l < 22; l++)if (arr[i][l][0] == 2)row++;
					if (line == 4)return 1;
					else return 2;
				}
			}
		}
		break;
	case 1:
		for (int i = 0; i < 10; i++) {
			for (int j = 0; j < 22; j++) {
				if (arr[i][j][0] == 2) {
					for (int k = i; k < 10; k++)if (arr[k][j][0] == 2)line++;
					for (int l = j; l < 22; l++)if (arr[i][l][0] == 2)row++;
					if (line == 1)return 11;
					else if (line == 3)return 13;
					else if (row == 1)return 12;
					else return 14;
				}
			}
		}
		break;
	case 2:
		for (int i = 0; i < 10; i++) {
			for (int j = 0; j < 22; j++) {
				if (arr[i][j][0] == 2) {
					for (int k = i; k < 10; k++)if (arr[k][j][0] == 2)line++;
					for (int l = j; l < 22; l++)if (arr[i][l][0] == 2)row++;
					if (line == 1)return 24;
					else if (line == 2) return 22;
					else if (row == 1)return 21;
					else return 23;
				}
			}
		}
		break;
	case 3:
		for (int i = 0; i < 10; i++) {
			for (int j = 0; j < 22; j++) {
				if (arr[i][j][0] == 2) {
					for (int k = i; k < 10; k++)if (arr[k][j][0] == 2)line++;
					for (int l = j; l < 22; l++)if (arr[i][l][0] == 2)row++;
					if (row == 1)return 31;
					else return 32;
				}
			}
		}
		break;
	case 4:
		for (int i = 0; i < 10; i++) {
			for (int j = 0; j < 22; j++) {
				if (arr[i][j][0] == 2) {
					for (int k = i; k < 10; k++)if (arr[k][j][0] == 2)line++;
					for (int l = j; l < 22; l++)if (arr[i][l][0] == 2)row++;
					if (row == 1)return 41;
					else return 42;
				}
			}
		}
		break;
	case 5:
		for (int i = 0; i < 10; i++) {
			for (int j = 0; j < 22; j++) {
				if (arr[i][j][0] == 2) {
					for (int k = i; k < 10; k++)if (arr[k][j][0] == 2)line++;
					for (int l = j; l < 22; l++)if (arr[i][l][0] == 2)row++;
					if (line == 2)return 52;
					else if (line == 1)return 54;
					else if (j == 21)return 51;
					else if (arr[i + 1][j + 1][0] == 2)return 53;
					else return 51;
				}

			}
		}
		break;
	}
	return 0;
}
int judgeisable(int blocktype, int blockstate) {
	switch (blocktype) {
	case 0:
		switch (blockstate) {
		case 1:
			for (int i = 0; i < 10; i++) {
				for (int j = 0; j < 22; j++) {
					if (arr[i][j][0] == 2) {
						if (j <= 1 || j == 21)return 0;
						else if (arr[i + 1][j + 1][0] == 0 && arr[i + 1][j - 1][0] == 0 && arr[i + 1][j - 2][0] == 0)return 1;
						else return 0;
					}
				}
			}
			break;
		case 2:
			for (int i = 0; i < 10; i++) {
				for (int j = 0; j < 22; j++) {
					if (arr[i][j][0] == 2) {
						if (i == 0 || i >= 8)return 0;
						else if (arr[i - 1][j + 2][0] == 0 && arr[i + 1][j + 2][0] == 0 && arr[i + 2][j + 2][0] == 0)return 1;
						else return 0;
					}
				}
			}
			break;
		}
		break;
	case 1:
		switch (blockstate) {
		case 11:
			for (int i = 0; i < 10; i++) {
				for (int j = 0; j < 22; j++) {
					if (arr[i][j][0] == 2) {
						if (j == 0)return 0;
						else if (arr[i + 1][j][0] == 0 && arr[i + 1][j - 1][0] == 0)return 1;
						else return 0;
					}
				}
			}
			break;
		case 12:
			for (int i = 0; i < 10; i++) {
				for (int j = 0; j < 22; j++) {
					if (arr[i][j][0] == 2) {
						if (i == 8)return 0;
						else if (arr[i + 2][j][0] == 0 && arr[i + 2][j - 1][0] == 0 && arr[i][j - 1][0] == 0)return 1;
						else return 0;
					}
				}
			}
			break;
		case 13:
			for (int i = 0; i < 10; i++) {
				for (int j = 0; j < 22; j++) {
					if (arr[i][j][0] == 2) {
						if (j == 0)return 0;
						else if (arr[i][j + 1][0] == 0 && arr[i][j - 1][0] == 0 && arr[i + 1][j - 1][0] == 0)return 1;
						else return 1;
					}
				}
			}
			break;
		case 14:
			for (int i = 0; i < 10; i++) {
				for (int j = 0; j < 22; j++) {
					if (arr[i][j][0] == 2) {
						if (i == 8)return 0;
						else if (arr[i + 1][j + 2][0] == 0 && arr[i + 2][j + 2][0] == 0)return 1;
						else return 0;
					}
				}
			}
			break;
		}
		break;
	case 2:
		switch (blockstate) {
		case 21:
			for (int i = 0; i < 10; i++) {
				for (int j = 0; j < 22; j++) {
					if (arr[i][j][0] == 2) {
						if (j <= 1)return 0;
						else if (arr[i][j - 2][0] == 0 && arr[i + 1][j - 2][0] == 0 && arr[i + 1][j - 1][0] == 0)return 1;
						else return 0;
					}
				}
			}
			break;
		case 22:
			for (int i = 0; i < 10; i++) {
				for (int j = 0; j < 22; j++) {
					if (arr[i][j][0] == 2) {
						if (i == 8)return 0;
						else if (arr[i][j + 2][0] == 0 && arr[i][j + 1][0] == 0 && arr[i + 2][j + 1][0] == 0)return 1;
						else return 0;
					}
				}
			}
			break;
		case 23:
			for (int i = 0; i < 10; i++) {
				for (int j = 0; j < 22; j++) {
					if (arr[i][j][0] == 2) {
						if (j == 0)return 0;
						else if (arr[i + 1][j + 1][0] == 0 && arr[i][j - 1][0] == 0)return 1;
						else return 0;
					}
				}
			}
			break;
		case 24:
			for (int i = 0; i < 10; i++) {
				for (int j = 0; j < 22; j++) {
					if (arr[i][j][0] == 2) {
						if (i == 8)return 0;
						else if (arr[i + 2][j + 1][0] == 0 && arr[i + 2][j + 2][0] == 0)return 1;
						else return 0;
					}
				}
			}
			break;
		}
		break;
	case 3:
		switch (blockstate) {
		case 31:
			for (int i = 0; i < 10; i++) {
				for (int j = 0; j < 22; j++) {
					if (arr[i][j][0] == 2) {
						if (j == 0)return 0;
						else if (arr[i][j + 1][0] == 0 && arr[i + 1][j - 1][0] == 0)return 1;
						else return 0;
					}
				}
			}
			break;
		case 32:
			for (int i = 0; i < 10; i++) {
				for (int j = 0; j < 22; j++) {
					if (arr[i][j][0] == 2) {
						if (i == 8)return 0;
						else if (arr[i + 1][j + 1][0] == 0 && arr[i + 2][j + 1][0] == 0)return 1;
						else return 0;
					}
				}
			}
			break;
		}
		break;
	case 4:
		switch (blockstate) {
		case 41:
			for (int i = 0; i < 10; i++) {
				for (int j = 0; j < 22; j++) {
					if (arr[i][j][0] == 2) {
						if (j == 1)return 0;
						else if (arr[i][j - 1][0] == 0 && arr[i][j - 2][0] == 0)return 1;
						else return 0;
					}
				}
			}
			break;
		case 42:
			for (int i = 0; i < 10; i++) {
				for (int j = 0; j < 22; j++) {
					if (arr[i][j][0] == 2) {
						if (i == 8)return 0;
						else if (arr[i][j + 2][0] == 0 && arr[i + 2][j + 1][0] == 0)return 1;
						else return 0;
					}
				}
			}
			break;
		}
		break;
	case 5:
		switch (blockstate) {
		case 51:
			for (int i = 0; i < 10; i++) {
				for (int j = 0; j < 22; j++) {
					if (arr[i][j][0] == 2) {
						if (j == 1)return 0;
						else if (arr[i][j - 1][0] == 0 && arr[i + 1][j - 2][0] == 0)return 1;
						else return 0;
					}
				}
			}
			break;
		case 52:
			for (int i = 0; i < 10; i++) {
				for (int j = 0; j < 22; j++) {
					if (arr[i][j][0] == 2) {
						if (i == 8)return 0;
						else if (arr[i + 2][j][0] == 0)return 1;
						else return 0;
					}
				}
			}
			break;
		case 53:
			for (int i = 0; i < 10; i++) {
				for (int j = 0; j < 22; j++) {
					if (arr[i][j][0] == 2) {
						if (j == 0)return 0;
						else if (arr[i + 1][j - 1][0] == 0)return 1;
						else return 0;
					}
				}
			}
			break;
		case 54:
			for (int i = 0; i < 10; i++) {
				for (int j = 0; j < 22; j++) {
					if (arr[i][j][0] == 2) {
						if (i == 0)return 0;
						else if (arr[i - 1][j + 2][0] == 0 && arr[i + 1][j + 2][0] == 0)return 1;
						else return 0;
					}
				}
			}
			break;
		}
		break;
	}
}
int revolveblock(int blocktype, int blockstate) {
	switch (blocktype) {
	case 0:
		switch (blockstate) {
		case 1:
			for (int i = 0; i < 10; i++) {
				for (int j = 0; j < 22; j++) {
					if (arr[i][j][0] == 2) {
						arr[i][j][0] = arr[i + 2][j][0] = arr[i + 3][j][0] = 0;
						arr[i][j][1] = arr[i + 2][j][1] = arr[i + 3][j][1] = 0;
						arr[i + 1][j - 1][0] = arr[i + 1][j - 2][0] = arr[i + 1][j + 1][0] = 2;
						arr[i + 1][j - 1][1] = arr[i + 1][j - 2][1] = arr[i + 1][j + 1][1] = arr[i + 1][j][1];
						return 0;
					}
				}
			}
			break;
		case 2:
			for (int i = 0; i < 10; i++) {
				for (int j = 0; j < 22; j++) {
					if (arr[i][j][0] == 2) {
						arr[i][j][0] = arr[i][j + 1][0] = arr[i][j + 3][0] = 0;
						arr[i][j][1] = arr[i][j + 1][1] = arr[i][j + 3][1] = 0;
						arr[i - 1][j + 2][0] = arr[i + 1][j + 2][0] = arr[i + 2][j + 2][0] = 2;
						arr[i - 1][j + 2][1] = arr[i + 1][j + 2][1] = arr[i + 2][j + 2][1] = arr[i][j + 2][1];
						return 0;

					}
				}
			}
			break;
		}
		break;
	case 1:
		switch (blockstate) {
		case 11:
			for (int i = 0; i < 10; i++) {
				for (int j = 0; j < 22; j++) {
					if (arr[i][j][0] == 2) {
						arr[i][j][0] = arr[i + 2][j + 1][0] = 0;
						arr[i][j][1] = arr[i + 2][j + 1][1] = 0;
						arr[i + 1][j][0] = arr[i + 1][j - 1][0] = 2;
						arr[i + 1][j][1] = arr[i + 1][j - 1][1] = arr[i][j + 1][1];
						return 0;

					}
				}
			}
			break;
		case 12:
			for (int i = 0; i < 10; i++) {
				for (int j = 0; j < 22; j++) {
					if (arr[i][j][0] == 2) {
						arr[i][j][0] = arr[i + 1][j][0] = arr[i + 1][j - 2][0] = 0;
						arr[i][j][1] = arr[i + 1][j][1] = arr[i + 1][j - 2][1] = 0;
						arr[i][j - 1][0] = arr[i + 2][j - 1][0] = arr[i + 2][j][0] = 2;
						arr[i][j - 1][1] = arr[i + 2][j - 1][1] = arr[i + 2][j][1] = arr[i + 1][j - 1][1];
						return 0;
					}
				}
			}
			break;
		case 13:
			for (int i = 0; i < 10; i++) {
				for (int j = 0; j < 22; j++) {
					if (arr[i][j][0] == 2) {
						arr[i + 1][j][0] = arr[i + 2][j][0] = arr[i + 2][j + 1][0] = 0;
						arr[i + 1][j][1] = arr[i + 2][j][1] = arr[i + 2][j + 1][1] = 0;
						arr[i][j + 1][0] = arr[i][j - 1][0] = arr[i + 1][j - 1][0] = 2;
						arr[i][j + 1][1] = arr[i][j - 1][1] = arr[i + 1][j - 1][1] = arr[i][j][1];
						return 0;
					}
				}
			}
			break;
		case 14:
			for (int i = 0; i < 10; i++) {
				for (int j = 0; j < 22; j++) {
					if (arr[i][j][0] == 2) {
						arr[i][j][0] = arr[i + 1][j][0] = 0;
						arr[i][j][1] = arr[i + 1][j][1] = 0;
						arr[i + 1][j + 2][0] = arr[i + 2][j + 2][0] = 2;
						arr[i + 1][j + 2][1] = arr[i + 2][j + 2][1] = arr[i][j + 1][1];
						return 0;
					}
				}
			}
			break;
		}
		break;
	case 2:
		switch (blockstate) {
		case 21:
			for (int i = 0; i < 10; i++) {
				for (int j = 0; j < 22; j++) {
					if (arr[i][j][0] == 2) {
						arr[i][j][0] = arr[i + 2][j][0] = arr[i + 2][j - 1][0] = 0;
						arr[i][j][1] = arr[i + 2][j][1] = arr[i + 2][j - 1][1] = 0;
						arr[i + 1][j - 1][0] = arr[i + 1][j - 2][0] = arr[i][j - 2][0] = 2;
						arr[i + 1][j - 1][1] = arr[i + 1][j - 2][1] = arr[i][j - 2][1] = arr[i + 1][j][1];
						return 0;
					}
				}
			}
			break;
		case 22:
			for (int i = 0; i < 10; i++) {
				for (int j = 0; j < 22; j++) {
					if (arr[i][j][0] == 2) {
						arr[i][j][0] = arr[i + 1][j][0] = arr[i + 1][j + 2][0] = 0;
						arr[i][j][1] = arr[i + 1][j][1] = arr[i + 1][j + 2][1] = 0;
						arr[i][j + 1][0] = arr[i][j + 2][0] = arr[i + 2][j + 1][0] = 2;
						arr[i][j + 1][1] = arr[i][j + 2][1] = arr[i + 2][j + 1][1] = arr[i + 1][j + 1][1];
						return 0;
					}
				}
			}
			break;
		case 23:
			for (int i = 0; i < 10; i++) {
				for (int j = 0; j < 22; j++) {
					if (arr[i][j][0] == 2) {
						arr[i + 1][j][0] = arr[i + 2][j][0] = 0;
						arr[i + 1][j][1] = arr[i + 2][j][1] = 0;
						arr[i][j - 1][0] = arr[i + 1][j + 1][0] = 2;
						arr[i][j - 1][1] = arr[i + 1][j + 1][1] = arr[i][j][1];
						return 0;
					}
				}
			}
			break;
		case 24:
			for (int i = 0; i < 10; i++) {
				for (int j = 0; j < 22; j++) {
					if (arr[i][j][0] == 2) {
						arr[i][j][0] = arr[i][j + 1][0] = 0;
						arr[i][j][1] = arr[i][j + 1][1] = 0;
						arr[i + 2][j + 1][0] = arr[i + 2][j + 2][0] = 2;
						arr[i + 2][j + 1][1] = arr[i + 2][j + 2][1] = arr[i][j + 2][1];
						return 0;
					}
				}
			}
			break;
		}
		break;
	case 3:
		switch (blockstate) {
		case 31:
			for (int i = 0; i < 10; i++) {
				for (int j = 0; j < 22; j++) {
					if (arr[i][j][0] == 2) {
						arr[i + 1][j + 1][0] = arr[i + 2][j + 1][0] = 0;
						arr[i + 1][j + 1][1] = arr[i + 2][j + 1][1] = 0;
						arr[i][j + 1][0] = arr[i + 1][j - 1][0] = 2;
						arr[i][j + 1][1] = arr[i + 1][j - 1][1] = arr[i][j][1];
						return 0;
					}
				}
			}
			break;
		case 32:
			for (int i = 0; i < 10; i++) {
				for (int j = 0; j < 22; j++) {
					if (arr[i][j][0] == 2) {
						arr[i][j + 1][0] = arr[i + 1][j - 1][0] = 0;
						arr[i][j + 1][1] = arr[i + 1][j - 1][1] = 0;
						arr[i + 1][j + 1][0] = arr[i + 2][j + 1][0] = 2;
						arr[i + 1][j + 1][1] = arr[i + 2][j + 1][1] = arr[i][j][1];
						return 0;
					}
				}
			}
			break;
		}
		break;
	case 4:
		switch (blockstate) {
		case 41:
			for (int i = 0; i < 10; i++) {
				for (int j = 0; j < 22; j++) {
					if (arr[i][j][0] == 2) {
						arr[i][j][0] = arr[i + 2][j - 1][0] = 0;
						arr[i][j][1] = arr[i + 2][j - 1][1] = 0;
						arr[i][j - 1][0] = arr[i][j - 2][0] = 2;
						arr[i][j - 1][1] = arr[i][j - 2][1] = arr[i + 1][j][1];
						return 0;
					}
				}
			}
			break;
		case 42:
			for (int i = 0; i < 10; i++) {
				for (int j = 0; j < 22; j++) {
					if (arr[i][j][0] == 2) {
						arr[i][j][0] = arr[i][j + 1][0] = 0;
						arr[i][j][1] = arr[i][j + 1][1] = 0;
						arr[i][j + 2][0] = arr[i + 2][j + 1][0] = 2;
						arr[i][j + 2][1] = arr[i + 2][j + 1][1] = arr[i + 1][j + 1][1];
						return 0;
					}
				}
			}
			break;
		}
		break;
	case 5:
		switch (blockstate) {
		case 51:
			for (int i = 0; i < 10; i++) {
				for (int j = 0; j < 22; j++) {
					if (arr[i][j][0] == 2) {
						arr[i][j][0] = arr[i + 2][j][0] = 0;
						arr[i][j][1] = arr[i + 2][j][1] = 0;
						arr[i][j - 1][0] = arr[i + 1][j - 2][0] = 2;
						arr[i][j - 1][1] = arr[i + 1][j - 2][1] = arr[i + 1][j][1];
						return 0;
					}
				}
			}
			break;
		case 52:
			for (int i = 0; i < 10; i++) {
				for (int j = 0; j < 22; j++) {
					if (arr[i][j][0] == 2) {
						arr[i + 1][j - 1][0] = 0;
						arr[i + 1][j - 1][1] = 0;
						arr[i + 2][j][0] = 2;
						arr[i + 2][j][1] = arr[i][j][1];
						return 0;
					}
				}
			}
			break;
		case 53:
			for (int i = 0; i < 10; i++) {
				for (int j = 0; j < 22; j++) {
					if (arr[i][j][0] == 2) {
						arr[i][j][0] = 0;
						arr[i][j][1] = 0;
						arr[i + 1][j - 1][0] = 2;
						arr[i + 1][j - 1][1] = arr[i + 1][j][1];
						return 0;
					}
				}
			}
			break;
		case 54:
			for (int i = 0; i < 10; i++) {
				for (int j = 0; j < 22; j++) {
					if (arr[i][j][0] == 2) {
						arr[i][j][0] = arr[i + 1][j + 1][0] = 0;
						arr[i][j][1] = arr[i + 1][j + 1][1] = 0;
						arr[i - 1][j + 2][0] = arr[i + 1][j + 2][0] = 2;
						arr[i - 1][j + 2][1] = arr[i + 1][j + 2][1] = arr[i][j + 1][1];
						return 0;
					}
				}
			}
			break;
		}
		break;
	}
}