#pragma once
#include <QGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QMatrix4x4>




class GLWidget :
	public QGLWidget, protected QOpenGLFunctions
{
	Q_OBJECT
public:
	GLWidget(QWidget *parent);
	~GLWidget();

	void InitValue(char* name_, double ThreshValue_, double width, double Org_Y, bool IsRawWave_, bool Isinhibit_, double scal_v_);
	void DrawString(const char * str);

	void RenderWave();

	void SetXArrayDisplay(double data[]) { for (int i = 0; i < 2048; i++) X_Array_Display[i] = data[i]; }
	void SetYArrayDisplay(double data[]) { for (int i = 0; i < 2048; i++) Y_Array_Display[i] = data[i]; }

protected:
	void initializeGL() override;
	void paintGL() override;
	void resizeGL(int width, int height) override;


public slots:

private:
	double org_Y;
	char name[20];
	double ThresholdValue;
	double scal_v;
	bool IsInhibit;
	int Current_sample_indx;
	
	double X_Array_Display[2048];
	double Y_Array_Display[2048];
	bool IsOverFlowed[2048];//data "amp" >thresholdValue ? true,false
	
	bool IsThreshVChange;
	int CountForThreshChange;
	bool IsRawWave;
	double width_;
};

