#include "GLWidget.h"
#include <qopenglshaderprogram.h>
#include <qcoreapplication.h>
#include <QMouseEvent>
#include <gl/GLU.h>

/**
*
* @brief EEG-Training에서 OpenGL로 그려준다.
* @details 뇌파의 데이터를 받아와서 OpenGL로 파형을 그려준다.
* @author 이보규
* @date 2018-09-03
* @version 0.0.1
*
*/
GLWidget::GLWidget(QWidget *parent)
	: QGLWidget(parent)
{
	IsThreshVChange = false;
	CountForThreshChange = 0;
	ThresholdValue = 60;
	scal_v = 5.0; 
	for (int i = 0; i< 2048; i++)
	{
		X_Array_Display[i] = 200 + (double)(i*(1645 - 200)) / 2048.0f;
		Y_Array_Display[i] = 0;
		IsOverFlowed[i] = false;//data "amp" >thresh value ? true,false;
	}

}


GLWidget::~GLWidget()
{
}

/**
*
*        @brief OpenGL 초기화 메서드
*        @details OpenGL을 사용하기위해 초기화를 한다. 다만 어떻게 초기화를 해야하는지 모르겠다.
*        @throws ValidException 
*
*/
void GLWidget::initializeGL()
{
	initializeOpenGLFunctions();
	glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_COLOR_MATERIAL);
	glEnable(GL_BLEND);
	glEnable(GL_POLYGON_SMOOTH);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
}

/**
*
*        @brief resize 메서드
*        @details OpenGL의 사이즈를 다시 정한다.
*        @throws ValidException
*
*/
void GLWidget::resizeGL(int w, int h)
{ 
	// Currently we are not handling width/height changes.
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, w, 0, h); // set origin to bottom left corner
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}


/**
*
*        @brief 그리기 메서드
*        @details OpenGL로 지정된 공간 안에서 그려준다.
*        @throws ValidException
*
*/
void GLWidget::paintGL()
{
	int width = 1645;
	int height = 1019;
	double WSPos = 270.0*width / 1920.0;//WaveStartPos
	double WTopPos = 0.77*height - 1.0;
	//int m_width_ = width+20;
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, width, 0, height, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glBegin(GL_QUADS);
	glColor3d(0.96, 0.96, 0.96);
	glVertex3d(0, height, -0.11);
	glVertex3d(0, WTopPos, -0.11);
	glVertex3d(width, WTopPos, -0.11);
	glVertex3d(width, height, -0.11);
	glEnd();
	


	///////////////////draw grid////////////
	glPushMatrix();
	glTranslatef(WSPos, 0, 0);
	glColor3d(0.8, 0.8, 0.8);

	glLineWidth(1.0);
	glBegin(GL_LINES);
	glVertex3d(-0.2*width, WTopPos, 0);
	glVertex3d(width, WTopPos, 0);
	glVertex3d(-0.2*width, 0.576*height, 0);
	glVertex3d(width, 0.576*height, 0);
	glVertex3d(-0.2*width, 0.382*height, 0);
	glVertex3d(width, 0.382*height, 0);
	glVertex3d(-0.2*width, 0.192*height, 0);
	glVertex3d(width, 0.192*height, 0);
	glEnd();

	for (int i = 0; i < 17; i++)
	{
		if (i % 2 == 0)
		{
			glLineWidth(1.0);
			glBegin(GL_LINES);
			glVertex3d(i*(width - WSPos) / 16.0 + 2, 0, -0.1);
			//glVertex3d(i*(width-200)/16.0 +2,height*0.97,-0.1);
			glVertex3d(i*(width - WSPos) / 16.0 + 2, WTopPos, -0.1);
			glEnd();
		}
		else
		{
			glEnable(GL_LINE_STIPPLE);
			glLineWidth(1.0);
			glLineStipple(1, 0x0F0F);
			glBegin(GL_LINES);
			glVertex3d(i*(width - WSPos) / 16.0 + 2, 0, -0.1);
			glVertex3d(i*(width - WSPos) / 16.0 + 2, WTopPos, -0.1);
			glEnd();
			glDisable(GL_LINE_STIPPLE);
		}
	}
	glPopMatrix();
	///////////////////draw grid///////////


	////just for raw wave spectialGreen
	glPushMatrix();

	glLineWidth(5.0);
}


void GLWidget::InitValue(char * name_, double ThresholdValue_, double width, double Org_Y, bool IsRawWave_, bool Isinhibit_, double scal_v_)
{
	for (int i = 0; i < 20; i++)
	{
		name[i] = name_[i];
	}
	scal_v = scal_v_;
	IsInhibit = Isinhibit_;
	Current_sample_indx = 0;
	org_Y = Org_Y;
	width_ = width;
	IsRawWave = IsRawWave_;
	for (int i = 0; i< 2048; i++)
	{
		X_Array_Display[i] = 200 + (double)(i*(width - 200)) / 2048.0;
		Y_Array_Display[i] = 0;
		IsOverFlowed[i] = false;//data "amp" >thresh value ? true,false;
	}
}

void GLWidget::DrawString(const char* str)
{
	int len, i;
	wchar_t* wstring;
	HDC hDC = wglGetCurrentDC();
	GLuint list = glGenLists(1);

	len = 0;
	for (i = 0; str[i] != '\0'; ++i)
	{
		if (IsDBCSLeadByte(str[i]))
			++i;
		++len;
	}

	wstring = (wchar_t*)malloc((len + 1) * sizeof(wchar_t));
	MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, str, -1, wstring, len);
	wstring[len] = L'\0';

	for (i = 0; i<len; ++i)
	{
		wglUseFontBitmapsW(hDC, wstring[i], 1, list);
		glCallList(list);
	}

	free(wstring);
	glDeleteLists(list, 1);
}