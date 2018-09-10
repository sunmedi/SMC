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
	RenderWave();
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


void GLWidget::RenderWave()
{
	glPushMatrix();
	glTranslatef(0.0f, org_Y, 0.1f);

	///////////////////////////////draw thresh value line
	glLineWidth(1.0);


	if (name[1] == 'a')//raw
	{
		glColor4d(0.0, 0.0, 0.0, 1.0);
		//glColor4d(0.5,0.5,1.0,1.0);
	}
	else if (name[1] == 'o')//name =="Low Inhibit")
	{
		//glColor4d(1.0,0.75,0.0,1.0);
		glColor4d(236 / 255.0, 63 / 255.0, 49.0 / 255, 1.0);
	}
	else if (name[1] == 'e')//name =="Reward")
	{
		//glColor4d(1.0,0.5,0.0,1.0);
		glColor4d(30 / 255.0, 155 / 255.0, 151 / 255, 1.0);
	}
	else if (name[1] == 'i')//=="High Inhibit")
	{
		//glColor4d(1.0,0.25,0.0,1.0);
		glColor3d(76 / 255.0, 89 / 255.0, 158 / 255);
	}

	glBegin(GL_LINES);
	if (abs(0.5*ThresholdValue*scal_v) < 100)
	{
		glVertex2d(X_Array_Display[0], -0.5*ThresholdValue*scal_v);
		glVertex2d(X_Array_Display[2047], -0.5*ThresholdValue*scal_v);
		glVertex2d(X_Array_Display[0], 0.5*ThresholdValue*scal_v);
		glVertex2d(X_Array_Display[2047], 0.5*ThresholdValue*scal_v);
	}
	else
	{
		glVertex2d(X_Array_Display[0], -99);
		glVertex2d(X_Array_Display[2047], -99);
		glVertex2d(X_Array_Display[0], 99);
		glVertex2d(X_Array_Display[2047], 99);
	}
	glEnd();


	glLineWidth(1);
	//glHint(GL_LINE_SMOOTH_HINT,GL_NICEST);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glEnable(GL_LINE_SMOOTH);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//if(name[1] != 'i')
	//{
	glColor3d(0.0, 0.0, 0.0);
	glBegin(GL_LINE_STRIP);
	for (int i = 0; i < Current_sample_indx; i++)
		if (abs(Y_Array_Display[i] * scal_v) < 100)
			glVertex3d(X_Array_Display[i], Y_Array_Display[i] * scal_v, 0.05);
		else
		{
			if (Y_Array_Display[i] > 0)
				glVertex3d(X_Array_Display[i], 100, 0.05);
			else
				glVertex3d(X_Array_Display[i], -100, 0.05);
		}
	glEnd();


	glDisable(GL_BLEND);
	glDisable(GL_LINE_SMOOTH);
	glEnable(GL_DEPTH_TEST);

	if (IsInhibit)
		glColor3d(1.0, 0.0, 0.0);
	else
		glColor3d(0.0, 1.0, 0.0);


	glLineWidth(4);
	glBegin(GL_LINES);
	for (int i = 0; i < Current_sample_indx - 1; i++)
	{
		//if(IsOverFlowed[i]&&IsOverFlowed[i+1])
		if (IsOverFlowed[i])
		{
			glVertex3d(X_Array_Display[i], 0.0, 0.1);
			glVertex3d(X_Array_Display[i + 1], 0.0, 0.1);
		}
	}
	glEnd();


	glPopMatrix();

}