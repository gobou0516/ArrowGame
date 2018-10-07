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

//��ʃT�C�Y�p�p�����[�^
const int DEFAULT_WINDOW_W = 600;
const int DEFAULT_WINDOW_H = 400;
int WINDOW_W = DEFAULT_WINDOW_W;
int WINDOW_H = DEFAULT_WINDOW_H;

// ���̉�]�p�p�����[�^
bool rightPressing = false;
bool leftPressing = false;

// FPS����p�p�����[�^
const int FPS = 60;
const double INTERVAL = 1000.0 / FPS; // ms

// �����p�p�����[�^
const float light0pos[] = { 3.0, 4.0, 5.0, 1.0 };

//���Ɋւ���p�����[�^
vector<double> ArrowY;//����y���W
vector<double> ArrowDirection;//���̌���
int ArrowNumber = 0;//����󂪉����邩�𐔂���ϐ�

//�Q�[���Ɋւ���p�����[�^
int CrossKeyPush = 1;//���{�^���������ꂽ���𔻒f����B����������ĂȂ��Ƃ���1�A�����ꂽ�ꍇ�͖��̊p�x
int Combo = 0;//�R���{��
int Score = 0;//�X�R�A : Perfect-3 Great-2 Good-1 Miss-0
int Life = 20;//�~�X���邲�Ƃ�1���������Ă���0�ɂȂ�����Q�[���I�[�o�[
int Difficult;//��Փx��1~3�̒����炦��ԁB�������傫���Ȃ�قǓ���Ȃ�
int Speed = 0;//��󂪗���鑬�x�A���X�R�A���Ƃɑ����Ȃ��Ă���
string Judge;//�^�C�~���O���茋�� : Perfect > Great > Good > Miss



//����`�悷��֐�
void drawArrow(const float color[]) //����`��
{
	glColor3fv(color);//�\��
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

	glColor3fv(color);//����
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



bool fpsControll()//FPS�𐧌�
{
	static clock_t lastDisplayTime = -1;
	clock_t displayTime = clock();
	if (lastDisplayTime > 0.0 && (displayTime - lastDisplayTime)*1000.0 / CLOCKS_PER_SEC < INTERVAL) return false;
	lastDisplayTime = displayTime;
	return true;
}

//��ԌÂ����������֐�
void ArrowErase()
{
	ArrowY.erase(ArrowY.begin());
	ArrowDirection.erase(ArrowDirection.begin());
	ArrowNumber--;
}

//1/10�̊m���Ŗ��𐶐�����֐�
void ArrowCreate()
{
	if (rand() % (75 / Difficult) == 1)//���̐���
	{
		ArrowNumber++; //��󂪈��������
		ArrowY.push_back(-4.0);//ArrowY�Ƀf�[�^��ǉ�


		switch (rand() % 4)//�����ɂ���Ė��̌������w��
		{
		case 0:
			ArrowDirection.push_back(90.0); //��
			break;
		case 1:
			ArrowDirection.push_back(180.0); //��
			break;
		case 2:
			ArrowDirection.push_back(0.0); //��
			break;
		case 3:
			ArrowDirection.push_back(270.0); //��
			break;
		default:
			break;
		}
	}
}

//���̔��������֐�
void ArrowJudgement()
{
	for (int i = 0; i < ArrowNumber; i++)
	{
		if (ArrowY[i] > 2.2)//������󂪏�܂ōs������~�X����&����
		{
			ArrowErase();
			Judge = "Miss";
			Combo = 0;
			Life--;
		}
		else//�����łȂ���Ζ���������Ɉړ�������
		{
			ArrowY[i] += 0.1;
		}
	}

	//���x����(����^�C�~���O�v����)
	if (CrossKeyPush != 1 && ArrowNumber != 0)//�\���L�[��������Ă�����
	{
		if ((double)CrossKeyPush == ArrowDirection[0]) //�������L�[�������Ă����画�肷��
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
		else //�����Ă��Ȃ�������~�X����
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

//�w��̕�������w��̏ꏊ�ɕ`�悷��֐�
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
	// FPS����
	if (!fpsControll()) return;

	// ������
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// ���f���r���[�ϊ�
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// ���_�̕ύX
	gluLookAt(0.0, 6.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

	// �����̈ʒu�ݒ�
	glLightfv(GL_LIGHT0, GL_POSITION, light0pos);

	// ��������
	glMaterialfv(GL_FRONT, GL_AMBIENT, red);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, red);
	glMaterialfv(GL_FRONT, GL_SPECULAR, white);
	glMaterialf(GL_FRONT, GL_SHININESS, 5);


	//���C�t��0�ɂȂ����狭���I��
	if (Life <= 0)
	{
		PrintString(-0.9, -1, "GAME OVER", 9);
		PrintString(-0.9, -1.5, "Score:", 6);
		PrintString(0.1, -1.5, to_string(Score).c_str(), to_string(Score).size());
		glutSwapBuffers();
		return;
	}


	//����������蔻�肵����
	ArrowCreate();
	ArrowJudgement();

	//����`�悷��
	for (int i = 0; i < ArrowNumber; i++)
	{
		glPushMatrix();
		glTranslated(0, ArrowY[i], 0);
		glRotated(ArrowDirection[i], 0, 0, 1);
		drawArrow(red);
		glPopMatrix();
	}

	//���胉�C���`��
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

	//�����`��
	PrintString(2, 0, Judge.c_str(), Judge.size()); //(����)
	PrintString(1, -1.5,"Score:", 6); 
	PrintString(2, -1.5, to_string(Score).c_str(), to_string(Score).size());//(�X�R�A)
	PrintString(1.1, -2.2, "Life:", 5); 
	PrintString(2.1, -2.2, to_string(Life).c_str(), to_string(Life).size()); //(���C�t)
	PrintString(1, -2.9, "Combo:", 6);
	PrintString(2, -2.9, to_string(Combo).c_str(), to_string(Combo).size()); //(���C�t)

	glutSwapBuffers();// �`��
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
	// �r���[�|�[�g�̐ݒ�
	glViewport(0, 0, WINDOW_W, WINDOW_H);
	// �����ϊ�
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(30.0, (double)WINDOW_W / WINDOW_H, 1.0, 100.0);
}

//�\���L�[����
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
	// �B�ʏ����̗L��
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	
	// ���C�e�B���O�̗L��
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, white);

}

//�X�^�[�g�O�̊m�F
void SetUp()
{
	do
	{
		cout << "��Փx��1�`3�̒�����I��ł�������" << endl;
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