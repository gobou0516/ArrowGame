#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <GL/glut.h>
#include <windows.h>
#include <iostream>
#include <vector>
#include <string>
#include "color.h"

using namespace std;

//画面サイズ用パラメータ
const int DEFAULT_WINDOW_W = 600;
const int DEFAULT_WINDOW_H = 400;
int WINDOW_W = DEFAULT_WINDOW_W;
int WINDOW_H = DEFAULT_WINDOW_H;

// 物体回転用パラメータ
bool rightPressing = false;
bool leftPressing = false;

// FPS制御用パラメータ
const int FPS = 60;
const double INTERVAL = 1000.0 / FPS; // ms

// 光源用パラメータ
const float light0pos[] = { 3.0, 4.0, 5.0, 1.0 };

//矢印に関するパラメータ
vector<double> ArrowY;//矢印のy座標
vector<double> ArrowDirection;//矢印の向き
int ArrowNumber = 0;//今矢印が何個あるかを数える変数

//ゲームに関するパラメータ
int CrossKeyPush = 1;//何ボタンが押されたかを判断する。何も押されてないときは1、押された場合は矢印の角度
int Combo = 0;//コンボ数
int Score = 0;//スコア : Perfect-3 Great-2 Good-1 Miss-0
int Life = 20;//ミスするごとに1ずつ減少していき0になったらゲームオーバー
int Difficult;//難易度を1~3の中からえらぶ。数字が大きくなるほど難しくなる
int Speed = 0;//矢印が流れる速度、一定スコアごとに早くなっていく
string Judge;//タイミング判定結果 : Perfect > Great > Good > Miss



//矢印を描画する関数
void drawArrow(const float color[]) //矢印を描画
{
	glColor3fv(color);//表面
	glNormal3d(0.0, 0.0, 1.0);
	glBegin(GL_POLYGON);
	glVertex3d(0, 0.4, 0.05);
	glVertex3d(-0.3, 0.1, 0.05);
	glVertex3d(-0.3, -0.1, 0.05);
	glVertex3d(-0.1, 0.1, 0.05);
	glVertex3d(-0.1, -0.4, 0.05);
	glVertex3d(0.1, -0.4, 0.05);
	glVertex3d(0.1, 0.1, 0.05);
	glVertex3d(0.3, -0.1, 0.05);
	glVertex3d(0.3, 0.1, 0.05);
	glEnd();

	glColor3fv(color);//裏面
	glNormal3d(0.0, 0.0, -1.0);
	glBegin(GL_POLYGON);
	glVertex3d(0, 0.4, -0.05);
	glVertex3d(0.3, 0.1, -0.05);
	glVertex3d(0.3, -0.1, -0.05);
	glVertex3d(0.1, 0.1, -0.05);
	glVertex3d(0.1, -0.4, -0.05);
	glVertex3d(-0.1, -0.4, -0.05);
	glVertex3d(-0.1, 0.1, -0.05);
	glVertex3d(-0.3, -0.1, -0.05);
	glVertex3d(-0.3, 0.1, -0.05);
	glEnd();

	glColor3fv(color);
	glNormal3d(-1.0, 1.0, 0.0);
	glBegin(GL_QUADS);
	glVertex3d(0, 0.4, 0.05);
	glVertex3d(0, 0.4, -0.05);
	glVertex3d(-0.3, 0.1, -0.05);
	glVertex3d(-0.3, 0.1, 0.05);
	glEnd();

	glColor3fv(color);
	glNormal3d(-1.0, 0.0, 0.0);
	glBegin(GL_QUADS);
	glVertex3d(-0.3, 0.1, 0.05);
	glVertex3d(-0.3, 0.1, -0.05);
	glVertex3d(-0.3, -0.1, -0.05);
	glVertex3d(-0.3, -0.1, 0.05);
	glEnd();

	glColor3fv(color);
	glNormal3d(1.0, -1.0, 0.0);
	glBegin(GL_QUADS);
	glVertex3d(-0.3, -0.1, 0.05);
	glVertex3d(-0.3, -0.1, -0.05);
	glVertex3d(-0.1, 0.1, -0.05);
	glVertex3d(-0.1, 0.1, 0.05);
	glEnd();

	glColor3fv(color);
	glNormal3d(-1.0, 0.0, 0.0);
	glBegin(GL_QUADS);
	glVertex3d(-0.1, 0.1, 0.05);
	glVertex3d(-0.1, 0.1, -0.05);
	glVertex3d(-0.1, -0.4, -0.05);
	glVertex3d(-0.1, -0.4, 0.05);
	glEnd();

	glColor3fv(color);
	glNormal3d(0.0, -1.0, 0.0);
	glBegin(GL_QUADS);
	glVertex3d(-0.1, -0.4, 0.05);
	glVertex3d(-0.1, -0.4, -0.05);
	glVertex3d(0.1, -0.4, -0.05);
	glVertex3d(0.1, -0.4, 0.05);
	glEnd();

	glColor3fv(color);
	glNormal3d(1.0, 0.0, 0.0);
	glBegin(GL_QUADS);
	glVertex3d(0.1, 0.1, 0.05);
	glVertex3d(0.1, -0.4, 0.05);
	glVertex3d(0.1, -0.4, -0.05);
	glVertex3d(0.1, 0.1, -0.05);
	glEnd();

	glColor3fv(color);
	glNormal3d(-1.0, -1.0, 0.0);
	glBegin(GL_QUADS);
	glVertex3d(0.3, -0.1, -0.05);
	glVertex3d(0.3, -0.1, 0.05);
	glVertex3d(0.1, 0.1, 0.05);
	glVertex3d(0.1, 0.1, -0.05);
	glEnd();

	glColor3fv(color);
glNormal3d(1.0, 0.0, 0.0);
glBegin(GL_QUADS);
glVertex3d(0.3, 0.1, 0.05);
glVertex3d(0.3, -0.1, 0.05);
glVertex3d(0.3, -0.1, -0.05);
glVertex3d(0.3, 0.1, -0.05);
glEnd();

glColor3fv(color);
glNormal3d(1.0, 1.0, 0.0);
glBegin(GL_QUADS);
glVertex3d(0.3, 0.1, 0.05);
glVertex3d(0.3, 0.1, -0.05);
glVertex3d(0, 0.4, -0.05);
glVertex3d(0, 0.4, 0.05);
glEnd();
}



bool fpsControll()//FPSを制御
{
	static clock_t lastDisplayTime = -1;
	clock_t displayTime = clock();
	if (lastDisplayTime > 0.0 && (displayTime - lastDisplayTime)*1000.0 / CLOCKS_PER_SEC < INTERVAL) return false;
	lastDisplayTime = displayTime;
	return true;
}

//一番古い矢印を消す関数
void ArrowErase()
{
	ArrowY.erase(ArrowY.begin());
	ArrowDirection.erase(ArrowDirection.begin());
	ArrowNumber--;
}

//1/10の確率で矢印を生成する関数
void ArrowCreate()
{
	if (rand() % (75 / Difficult) == 1)//矢印の生成
	{
		ArrowNumber++; //矢印が一つ増えたよ
		ArrowY.push_back(-4.0);//ArrowYにデータを追加


		switch (rand() % 4)//乱数によって矢印の向きを指定
		{
		case 0:
			ArrowDirection.push_back(90.0); //←
			break;
		case 1:
			ArrowDirection.push_back(180.0); //↓
			break;
		case 2:
			ArrowDirection.push_back(0.0); //↑
			break;
		case 3:
			ArrowDirection.push_back(270.0); //→
			break;
		default:
			break;
		}
	}
}

//矢印の判定をする関数
void ArrowJudgement()
{
	for (int i = 0; i < ArrowNumber; i++)
	{
		if (ArrowY[i] > 2.2)//もし矢印が上まで行ったらミス判定&消す
		{
			ArrowErase();
			Judge = "Miss";
			Combo = 0;
			Life--;
		}
		else//そうでなければ矢印を上方向に移動させる
		{
			ArrowY[i] += 0.1;
		}
	}

	//精度判定(判定タイミング要検証)
	if (CrossKeyPush != 1 && ArrowNumber != 0)//十字キーが押されていたら
	{
		if ((double)CrossKeyPush == ArrowDirection[0]) //押したキーが合っていたら判定する
		{
			Combo++;
			if (1.7 <= ArrowY[0] && ArrowY[0] <= 1.9)
			{
				Judge = "Perfect";
				Score += 3;
			}
			else if (1.6 <= ArrowY[0] && ArrowY[0] <= 2.0)
			{
				Judge = "Great";
				Score += 2;
			}
			else if (1.4 <= ArrowY[0] && ArrowY[0] <= 2.2)
			{
				Judge = "Good";
				Score++;
			}
			else
			{
				Judge = "Miss";
				Combo = 0;
				Life--;
			}
		}
		else //合っていなかったらミス判定
		{
			Judge = "Miss";
			Combo = 0;
			Life--;
		}

		ArrowErase();
		cout << Judge << endl;
		CrossKeyPush = 1;
	}
}

//指定の文字列を指定の場所に描画する関数
void PrintString(float x, float y, const char* str, int length) 
{
	float z = -1.0f;
	glRasterPos3f(x, y, z);

	for (int i = 0; i < length; i++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[i]);
	}
}



void display(void)
{
	// FPS制御
	if (!fpsControll()) return;

	// 初期化
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// モデルビュー変換
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// 視点の変更
	gluLookAt(0.0, 6.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

	// 光源の位置設定
	glLightfv(GL_LIGHT0, GL_POSITION, light0pos);

	// 光源処理
	glMaterialfv(GL_FRONT, GL_AMBIENT, red);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, red);
	glMaterialfv(GL_FRONT, GL_SPECULAR, white);
	glMaterialf(GL_FRONT, GL_SHININESS, 5);


	//ライフが0になったら強制終了
	if (Life <= 0)
	{
		PrintString(-0.9, -1, "GAME OVER", 9);
		PrintString(-0.9, -1.5, "Score:", 6);
		PrintString(0.1, -1.5, to_string(Score).c_str(), to_string(Score).size());
		glutSwapBuffers();
		return;
	}


	//矢印を作ったり判定したり
	ArrowCreate();
	ArrowJudgement();

	//矢印を描画する
	for (int i = 0; i < ArrowNumber; i++)
	{
		glPushMatrix();
		glTranslated(0, ArrowY[i], 0);
		glRotated(ArrowDirection[i], 0, 0, 1);
		drawArrow(red);
		glPopMatrix();
	}

	//判定ライン描画
	glPushMatrix();
	glTranslated(0, 2.0, 0);
	glColor3fv(red);
	glBegin(GL_LINES);
	glVertex2d(-2, 0);
	glVertex2d(2, 0);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glTranslated(0, 1.6, 0);
	glColor3fv(red);
	glBegin(GL_LINES);
	glVertex2d(-2, 0);
	glVertex2d(2, 0);
	glEnd();
	glPopMatrix();

	//文字描画
	PrintString(2, 0, Judge.c_str(), Judge.size()); //(判定)
	PrintString(1, -1.5,"Score:", 6); 
	PrintString(2, -1.5, to_string(Score).c_str(), to_string(Score).size());//(スコア)
	PrintString(1.1, -2.2, "Life:", 5); 
	PrintString(2.1, -2.2, to_string(Life).c_str(), to_string(Life).size()); //(ライフ)
	PrintString(1, -2.9, "Combo:", 6);
	PrintString(2, -2.9, to_string(Combo).c_str(), to_string(Combo).size()); //(ライフ)

	glutSwapBuffers();// 描画
}

void idle()
{
	glutPostRedisplay();
}

void resize(int w, int h)
{
	printf("resize called\n");
	WINDOW_W = w;
	WINDOW_H = h;
	// ビューポートの設定
	glViewport(0, 0, WINDOW_W, WINDOW_H);
	// 透視変換
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(30.0, (double)WINDOW_W / WINDOW_H, 1.0, 100.0);
}

//十字キー判定
void specialKey(int key, int x, int y)
{
	switch (key) {
	case GLUT_KEY_LEFT:
		printf("key LEFT pressed\n");
		CrossKeyPush = 90;
		break;
	case GLUT_KEY_DOWN:
		printf("key DOWN pressed\n");
		CrossKeyPush = 180;
		break;
	case GLUT_KEY_UP:
		printf("key UP pressed\n");
		CrossKeyPush = 0;
		break;
	case GLUT_KEY_RIGHT:
		printf("key RIGHT pressed\n");
		CrossKeyPush = 270;
		break;
	default:
		break;
	}
}

void init(void)
{
	glClearColor(0.0, 0.0, 0.0, 1.0);
	// 隠面処理の有効
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	
	// ライティングの有効
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, white);

}

//スタート前の確認
void SetUp()
{
	do
	{
		cout << "難易度を1～3の中から選んでください" << endl;
		cin >> Difficult;
	} while (1 > Difficult || Difficult > 3);
}


int main(int argc, char *argv[])
{
	SetUp();
	srand(time(NULL));
	glutInitWindowSize(WINDOW_W, WINDOW_H);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutCreateWindow("ArrowGame");
	glutDisplayFunc(display);
	glutReshapeFunc(resize);
	glutSpecialFunc(specialKey);
	glutIdleFunc(idle);
	init();
	glutMainLoop();
	return 0;
}