#include "EEGWave.h"
#include <iomanip>
//#include "DataController.h"
#include "../User/include/FIRWINFunction.h"
#include<algorithm>
#include <QOpenGLFunctions>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QMatrix4x4>
//vector<EEGWave *>EEG_CONTROL_arry;
//extern DataController DataContrl;
//int countttt =0;


EEGWave::EEGWave(void)
{
	IsNeedComputeThreshold = false;
	IsSingleChanel = true;
	MinAMP = 100000;
	MaxAMP = 0;
	Total_AVG_AMP =0;
	SDAMP = 0;
	IsLockedUSBGetting = false;
	IsThresholdComputeEndding = true;
	CountForThreshChange = 0;
	IsThreshVChange = false;
	AutoThreshOffset = 0.1;
	FlagNeedNewThreshold = false;
	CountOfRecodeSamples = 0;
	Recoded_Data_Change_flag2 = false;
	Recoded_Data_Change_flag = false;
	TargetOverPercent = OverVpercent = OverVpercent_8sec = Total_amp = Avg_amp = 0;
	Absolute_Thresh = 100;
	//CountofOverSample_8sec_start=CountofOverSample_8sec= 
	CountofOverSample = 0;
	IsRecoding_notrawdata = false;
	isHalfAutoProcessing = false;
	offset_Target = 5;
	Duration_for_auto = 4;

	T_success = 0;
	T_SCount = 0;
}
void EEGWave::SetRecodeArraySize()
{
	Recoded_Array.reserve(Recode_size_Capacity);
	Recoded_Array_IsOver.reserve(Recode_size_Capacity);
	Recoded_Array_2.reserve(Recode_size_Capacity);
	Recoded_Array_IsOver_2.reserve(Recode_size_Capacity);
	Avg_amp_8sec_Arry.reserve(Recode_size_Capacity/32);
}
void EEGWave::ReSetDataMemery()
{
	IsNeedComputeThreshold = false;
	MinAMP = 100000;
	MaxAMP = 0;
	SDAMP = 0;
	Total_AVG_AMP = 0;
	IsLockedUSBGetting = false;
	T_success = 0;
	T_SCount = 0;

	IsThresholdComputeEndding = true;
	CountForThreshChange = 0;
	IsThreshVChange = false;
	/*ClearVector(Recoded_Array);
	ClearVector(Recoded_Array_IsOver);
	ClearVector(Recoded_Array_2);
	ClearVector(Recoded_Array_IsOver_2);
	ClearVector(Avg_amp_8sec_Arry);*/
	Recoded_Array.swap(vector<double>());//better to clear
	Recoded_Array_IsOver.swap(vector<bool>());
	Recoded_Array_2.swap(vector<double>());
	Recoded_Array_IsOver_2.swap(vector<bool>());
	Avg_amp_8sec_Arry.swap(vector<double>());

	Current_sample_indx = 0;
	CountOfRecodeSamples = 0;
	Recoded_Data_Change_flag2 = false;
	Recoded_Data_Change_flag = false;
	FlagNeedNewThreshold = false;

	OverVpercent = OverVpercent_8sec = Total_amp = Avg_amp = 0;
	
	//CountofOverSample_8sec_start=CountofOverSample_8sec= 
	CountofOverSample = 0;
	IsRecoding_notrawdata = false;
	isHalfAutoProcessing = false;

	//Recode_size_Capacity = RECODE_SIZE_25+1;
	//Recode_size_Capacity = 2048+1;
	//Recoded_Array.reserve(Recode_size_Capacity);
	//Recoded_Array_IsOver.reserve(Recode_size_Capacity);
	SetRecodeArraySize();
	for (int i = 0;i< 2048;i++)
	{
		Y_Array_Display[i] = 0;
		Y_Array_Display_2[i] = 0;

		IsOverFlowed[i] = false;//data "amp" >thresh value ? true,false;
	}
	for (int i = 0;i < 256;i++)
	{
		Buf_1sec[i]=0.0;
	}
	for (int i = 0;i < 512;i++)
	{
		Buf_2sec[i]=0.0;
	}
	Avg_amp_1sec = 0.0;
	Avg_amp_8sec = 0.0;
	Max_amp_8sec = 0.0;

}
void EEGWave::InitValue(bool issinglec,char* name_,double ThreshValue_,double width,double Org_Y,bool IsRawWave_,IIRProcDataStruct iirDataSet_,bool Isinhibit_,double scal_v_,double TargetOverPercent_,double Absolute_Thresh_,double offset_Target_,int Duration_for_auto_,double RawThreshValueBuf_)
{
	IsSingleChanel = issinglec;
	Total_AVG_AMP = 0;
	MinAMP = 100000;
	MaxAMP = 0;
	SDAMP = 0;
	IsLockedUSBGetting = false;
	IsThresholdComputeEndding = true;
	RawThreshValueBuf = RawThreshValueBuf_;
	offset_Target = offset_Target_;
	Duration_for_auto = Duration_for_auto_;
	CountForThreshChange = 0;
	IsThreshVChange = false;
	for (int i = 0;i < 256;i++)
	{
		Buf_1sec[i]=0.0;
	}
	for (int i = 0;i < 512;i++)
	{
		Buf_2sec[i]=0.0;
	}
	//EEG_CONTROL_arry.push_back(this);
	Avg_amp_1sec = 0.0;
	Avg_amp_8sec = 0.0;
	Max_amp_8sec = 0.0;
	Absolute_Thresh = Absolute_Thresh_;
	TargetOverPercent = TargetOverPercent_;
	for (int i = 0;i < 20;i++)
	{
		name[i] = name_[i];
	}

	scal_v = scal_v_;
	//scal_FIR = scal_F;
	IsInhibit = Isinhibit_;
	Current_sample_indx = 0;
	org_Y = Org_Y;
	width_ =  width;
	IsRawWave = IsRawWave_;

	//ThreshValue = ThreshValue_/scal_FIR;

	ThreshValue_buf =ThreshValue = ThreshValue_;
	//ThreshValue_display = 2*ThreshValue_*scal_FIR;
	//ThresholdChange temp;
	//temp.CurrentSuccessPerc = 0.0;
	//temp.sampleInx = 0;
	//temp.thresholdV = ThreshValue;
	//ThreshVArr.push_back(temp);
	for (int i = 0;i< 2048;i++)
	{
		X_Array_Display[i] = 200+(double)(i*(width-200))/2048.0;
		Y_Array_Display[i] = 0;
		Y_Array_Display_2[i] = 0;
		IsOverFlowed[i] = false;//data "amp" >thresh value ? true,false;
	}

	IirDataSet.PassType = iirDataSet_.PassType;
	IirDataSet.IIRType = iirDataSet_.IIRType;
	if (IsRawWave)
	{
		IirDataSet.OmegaC = iirDataSet_.OmegaC;
		IirDataSet.BandWidth = iirDataSet_.BandWidth;

		start_hz = 128.0*(IirDataSet.OmegaC - 0.5*IirDataSet.BandWidth);
		end_hz = 128.0*(IirDataSet.OmegaC + 0.5*IirDataSet.BandWidth);

		//filterController_.ComputIIRFilter(iirDataSet_.PassType,iirDataSet_.IIRType,iirDataSet_.OmegaC,iirDataSet_.BandWidth);
		IirDataSet.OmegaC *= 128.0;
		IirDataSet.BandWidth *= 128.0;
		New_IIR_filter.initFilter(256, IirDataSet.OmegaC, IirDataSet.BandWidth, 0.8);
	}
}
void EEGWave::ReSetXArray(double width,double Org_Y)
{
	for (int i = 0;i< 2048;i++)
		X_Array_Display[i] = 200+(double)(i*(width-200))/2048.0;
	org_Y = Org_Y;

}
EEGWave::~EEGWave(void)
{

}
void EEGWave::ReSetFIR()
{
	double h1[2048];

	FirWin(FirDataSet.lf,FirDataSet.hf,FirDataSet.fs,
		FirDataSet.FIR_LEN, FirDataSet.Filter_type,FirDataSet.Window_type,h1);

	for(int i = 0; i < FIR_TAP; i++)
		FIR[i] = h1[i];
}
void EEGWave::ReSetIIR()
{
	start_hz = 128.0*(IirDataSet.OmegaC - 0.5*IirDataSet.BandWidth);	
	end_hz = 128.0*(IirDataSet.OmegaC + 0.5*IirDataSet.BandWidth);

	/*if(name[1]=='o')
		filterController_.ComputIIRFilter(IirDataSet.PassType,IirDataSet.IIRType,IirDataSet.OmegaC,IirDataSet.BandWidth + 1.8/128.0);
	else if(name[1]=='e')
		filterController_.ComputIIRFilter(IirDataSet.PassType,IirDataSet.IIRType,IirDataSet.OmegaC,IirDataSet.BandWidth + 0.8/128.0);
	else if(name[1]=='i')
		filterController_.ComputIIRFilter(IirDataSet.PassType,IirDataSet.IIRType,IirDataSet.OmegaC - 1.0/128.0,IirDataSet.BandWidth + 1.0/128.0);
	else
		filterController_.ComputIIRFilter(IirDataSet.PassType,IirDataSet.IIRType,IirDataSet.OmegaC,IirDataSet.BandWidth);*/

	IirDataSet.OmegaC *= 128.0;
	IirDataSet.BandWidth *= 128.0;
	New_IIR_filter.initFilter(256,IirDataSet.OmegaC,IirDataSet.BandWidth,0.8);

}
void EEGWave::RenderWave()
{
	glPushMatrix();
	glTranslatef(0.0f,org_Y,0.1f);
	
	///////////////////////////////draw thresh value line
	glLineWidth(1.0);


	if(name[1] == 'a')//raw
	{
		glColor4d(0.0,0.0,0.0,1.0);
		//glColor4d(0.5,0.5,1.0,1.0);
	}
	else if(name[1] == 'o')//name =="Low Inhibit")
	{
		//glColor4d(1.0,0.75,0.0,1.0);
		glColor4d(236/255.0,63/255.0,49.0/255,1.0);
	}
	else if(name[1] == 'e')//name =="Reward")
	{
		//glColor4d(1.0,0.5,0.0,1.0);
		glColor4d(30/255.0,155/255.0,151/255,1.0);
	}
	else if(name[1] == 'i')//=="High Inhibit")
	{
		//glColor4d(1.0,0.25,0.0,1.0);
		glColor3d(76/255.0,89/255.0,158/255);
	}

	glBegin(GL_LINES);
	if(abs(0.5*ThreshValue*scal_v) < 100)
	{
		glVertex2d(X_Array_Display[0],-0.5*ThreshValue*scal_v);
		glVertex2d(X_Array_Display[2047],-0.5*ThreshValue*scal_v);
		glVertex2d(X_Array_Display[0],0.5*ThreshValue*scal_v);
		glVertex2d(X_Array_Display[2047],0.5*ThreshValue*scal_v);
	}
	else
	{
		glVertex2d(X_Array_Display[0],-99 );
		glVertex2d(X_Array_Display[2047],-99 );
		glVertex2d(X_Array_Display[0],99 );
		glVertex2d(X_Array_Display[2047],99 );
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
		glColor3d(0.0,0.0,0.0);
		glBegin(GL_LINE_STRIP);
		for (int i = 0;i < Current_sample_indx;i++)
			if(abs(Y_Array_Display[i]*scal_v) < 100)
				glVertex3d(X_Array_Display[i],Y_Array_Display[i]*scal_v,0.05);
			else
			{
				if(Y_Array_Display[i] > 0)
					glVertex3d(X_Array_Display[i],100,0.05);
				else
					glVertex3d(X_Array_Display[i],-100,0.05);
			}
		glEnd();
		
	
		glDisable(GL_BLEND);
		glDisable(GL_LINE_SMOOTH);
		glEnable(GL_DEPTH_TEST);

		if(IsInhibit)
			glColor3d(1.0,0.0,0.0);
		else
			glColor3d(0.0,1.0,0.0);


		glLineWidth(4);
		glBegin(GL_LINES);
		for (int i = 0;i < Current_sample_indx -1;i++)
		{
			//if(IsOverFlowed[i]&&IsOverFlowed[i+1])
			if(IsOverFlowed[i])
			{
				glVertex3d(X_Array_Display[i],0.0,0.1);
				glVertex3d(X_Array_Display[i+1],0.0,0.1);
			}
		}
		glEnd();
		

	glPopMatrix();
	
}


void EEGWave::RenderWave_2()
{
	if(name[1] == 'a')//raw
		glColor4d(0.5,0.5,1.0,1.0);
	else if(name[1] == 'o')//name =="Low Inhibit")
		glColor4d(0.5,0.5,0.9,1.0);
	else if(name[1] == 'e')//name =="Reward")
		glColor4d(0.5,0.5,0.8,1.0);
	else if(name[1] == 'i')//=="High Inhibit")
		glColor4d(0.5,0.5,0.7,1.0);

#pragma region CH1
	glPushMatrix();
	glTranslatef(0.0f,org_Y,0.1f);
	///////////////////draw grid////////////
	double g_w = 190;//grid_width
	double g_h = 140;//gird_height
	double d_h = 140/7.0;
	glPushMatrix();
	{
		glTranslated(5,0.5*g_h,0);

		glColor3d(1.0,1.0,1.0);
		glBegin(GL_QUADS);
		glVertex3d(0,0,-0.2);
		glVertex3d(g_w,0,-0.2);
		glVertex3d(g_w,-g_h,-0.2);
		glVertex3d(0,-g_h,-0.2);
		glEnd();

		glColor3d(0.96,0.96,0.96);
		glBegin(GL_QUADS);
		glVertex3d(0,0,-0.19);
		glVertex3d(g_w,0,-0.19);
		glVertex3d(g_w,-d_h,-0.19);
		glVertex3d(0,-d_h,-0.19);

		glVertex3d(0,-2*d_h,-0.19);
		glVertex3d(g_w,-2*d_h,-0.19);
		glVertex3d(g_w,-3*d_h,-0.19);
		glVertex3d(0,-3*d_h,-0.19);

		glVertex3d(0,-4*d_h,-0.19);
		glVertex3d(g_w,-4*d_h,-0.19);
		glVertex3d(g_w,-5*d_h,-0.19);
		glVertex3d(0,-5*d_h,-0.19);

		glVertex3d(0,-6*d_h,-0.19);
		glVertex3d(g_w,-6*d_h,-0.19);
		glVertex3d(g_w,-7*d_h,-0.19);
		glVertex3d(0,-7*d_h,-0.19);

		glEnd();



		glColor3d(0,0,0);
		glLineWidth(2.0);
		glBegin(GL_LINE_STRIP);
		glVertex2d(0,0);
		glVertex2d(g_w,0);
		glVertex2d(g_w,-g_h);
		glVertex2d(0,-g_h);
		glVertex2d(0,0);
		glEnd();

		glLineWidth(1.0);
		glBegin(GL_LINES);
		for(int i =0;i<6;i++)
		{
			glVertex2d(0,-(i+1)*d_h);
			glVertex2d(g_w,-(i+1)*d_h);
		}
		glVertex2d(0.5*g_w,0);
		glVertex2d(0.5*g_w,-g_h);
		glEnd();


	}
	glPopMatrix();
	///////////////////draw grid////////////

	glPushMatrix();
	glTranslated(0,70,0);
	//stringstream ss_2 (stringstream::in | stringstream::out);
	//ss_2<<std::fixed<<std::setprecision(1)<<start_hz<<" ~ "<<end_hz<<"Hz";
	glPushMatrix();
	glColor3d(0.0,0.0,0.0);

	if(name[1] == 'a')//raw
	{
		DrawString(" ");
		glRasterPos3d(g_w*0.25*(1.0 - 1.0/7.0), -17.0, 0.0);
	}
	else if(name[1] == 'e')
		glRasterPos3d(g_w*0.25*(1.0 - 2.5/7.0), -17.0, 0.0);
	else if(name[1] == 'o')
		glRasterPos3d(g_w*0.25*(1.0 - 4.0/7.0), -17.0, 0.0);
	else if(name[1] == 'i')
		glRasterPos3d(g_w*0.25*(1.0 - 4.0/7.0), -17.0, 0.0);

	DrawString(name);
	if(name[1] == 'a')//raw
		glRasterPos3d(g_w*0.75*(1.0 - 3.0/21.0), -17.0, 0.0);
	else if(name[1] == 'e')
		glRasterPos3d(g_w*0.75*(1.0 - 3.0/21.0), -17.0, 0.0);
	else if(name[1] == 'o')
		glRasterPos3d(g_w*0.75*(1.0 - 2.5/21.0), -17.0, 0.0);
	else if(name[1] == 'i')
		glRasterPos3d(g_w*0.75*(1.0 - 3.5/21.0), -17.0, 0.0);
	//glRasterPos3d(110.0, -17.0, 0.0);
	//DrawString((ss_2.str()).c_str());
	glPopMatrix();
	//ss_2.str("");


	//ss_2<<"Scale                 "<<std::fixed<<std::setprecision(1)<<scal_v;
	glPushMatrix();
	glColor3d(0.0,0.0,0.0);
	glRasterPos3d(20.0, -37.0, 0.0);

	glRasterPos3d(g_w*0.25*(1.0 - 2/7.0), -37.0, 0.0);

	//DrawString((ss_2.str()).c_str());
	glPopMatrix();
	//ss_2.str("");

	//ss_2<<"AMP Thresh       "<<std::fixed<<std::setprecision(1)<<ThreshValue_display;
	//ss_2<<"AMP Thresh           "<<std::fixed<<std::setprecision(1)<<0.5*ThreshValue;
	//ss_2<<"AMP Thresh       "<<std::fixed<<std::setprecision(1)<<Recoded_Array.size();
	glPushMatrix();
	if(!IsThreshVChange)
		glColor3d(0.0,0.0,0.0);
	else
	{
		glColor3d(1.0,0.0,0.0);
		if(CountForThreshChange<6)
			CountForThreshChange++;
		else
		{
			CountForThreshChange = 0;
			IsThreshVChange = false;
		}
	}


	glRasterPos3d(20.0, -57.0, 0.0);

	//DrawString((ss_2.str()).c_str());
	glPopMatrix();
	//ss_2.str("");

	//ss_2<<"AMP AVG              "<<std::fixed<<std::setprecision(1)<<Avg_amp;
	//ss_2<<"OTC            "<<std::fixed<<std::setprecision(1)<<CountofOverSample;
	glPushMatrix();
	glColor3d(0.0,0.0,0.0);
	glRasterPos3d(20.0, -77.0, 0.0);

	//DrawString((ss_2.str()).c_str());
	glPopMatrix();
	//ss_2.str("");


	/*if(IsInhibit)
		ss_2<<"< Target%             "<<std::fixed<<std::setprecision(2)<<(100.0-TargetOverPercent);
	else
		ss_2<<"> Target%             "<<std::fixed<<std::setprecision(2)<<TargetOverPercent;*/
	glPushMatrix();
	if(IsInhibit)
		glColor3d(1.0,0.0,0.0);
	else
		glColor3d(0.0,0.5,0.0);

	glRasterPos3d(20.0, -137.0, 0.0);

	//DrawString((ss_2.str()).c_str());
	glPopMatrix();
	//ss_2.str("");

	if(!IsRawWave)
	{	
		//ss_2<<"AMP                  +/- "<<std::fixed<<std::setprecision(1)<<Avg_amp_1sec;
	//	ss_2<<"AMP 8sec              "<<std::fixed<<std::setprecision(1)<<Avg_amp_8sec;
		//ss_2<<"OTC2            "<<std::fixed<<std::setprecision(1)<<CountofOverSample_;
		glPushMatrix();
		glColor3d(0.0,0.0,0.0);
		glRasterPos3d(20.0, -97.0, 0.0);

		//DrawString((ss_2.str()).c_str());
		glPopMatrix();
		//ss_2.str("");

		/*if(IsInhibit)
			ss_2<<name<<"%          "<<std::fixed<<std::setprecision(2)<<(OverVpercent_8sec)*100;
		else
			ss_2<<name<<"%               "<<std::fixed<<std::setprecision(2)<<(OverVpercent_8sec)*100;*/

		glPushMatrix();
		glColor3d(0.0,0.0,0.0);
		glRasterPos3d(20.0, -117.0, 0.0);

		//DrawString((ss_2.str()).c_str());
		glPopMatrix();
		//ss_2.str("");
	}
	else
	{	
		//ss_2<<name<<"%                 "<<std::fixed<<std::setprecision(2)<<(OverVpercent_8sec)*100;
		//ss_2<<name<<"%                    "<<std::fixed<<std::setprecision(2)<<(OverVpercent)*100;
		//ss_2<<name<<"%                 "<<std::fixed<<std::setprecision(2)<<CountofOverSample;
		glPushMatrix();
		glColor3d(0.0,0.0,0.0);
		glRasterPos3d(20.0, -117.0, 0.0);

		//DrawString((ss_2.str()).c_str());
		glPopMatrix();
		//ss_2.str("");
	}
	glPopMatrix();
	//////////////END GRID//////////

	///////////////////////////////draw thresh value line
	glLineWidth(1.0);
	glPushMatrix();
	if(name[1] == 'a')//raw
		glColor4d(0.5,0.5,1.0,1.0);
	else if(name[1] == 'o')//name =="Low Inhibit")
		glColor4d(0.5,0.5,0.9,1.0);
	else if(name[1] == 'e')//name =="Reward")
		glColor4d(0.5,0.5,0.8,1.0);
	else if(name[1] == 'i')//=="High Inhibit")
		glColor4d(0.5,0.5,0.7,1.0);

	glBegin(GL_LINES);
	glVertex2d(X_Array_Display[0],-0.5*ThreshValue*scal_v);
	glVertex2d(X_Array_Display[2047],-0.5*ThreshValue*scal_v);
	glVertex2d(X_Array_Display[0],0.5*ThreshValue*scal_v);
	glVertex2d(X_Array_Display[2047],0.5*ThreshValue*scal_v);
	glEnd();
	glPopMatrix();

	glLineWidth(1);
	//glHint(GL_LINE_SMOOTH_HINT,GL_NICEST);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glEnable(GL_LINE_SMOOTH);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	
	glColor3d(0.0,0.0,0.0);
	glBegin(GL_LINE_STRIP);
	for (int i = 0;i < Current_sample_indx;i++)
		glVertex3d(X_Array_Display[i],Y_Array_Display[i]*scal_v,0.05);
	glEnd();

	glDisable(GL_BLEND);
	glDisable(GL_LINE_SMOOTH);
	glEnable(GL_DEPTH_TEST);

	if(IsInhibit)
		glColor3d(1.0,0.0,0.0);
	else
		glColor3d(0.0,1.0,0.0);


	glLineWidth(4);
	glBegin(GL_LINES);
	for (int i = 0;i < Current_sample_indx -1;i++)
	{
		if(IsOverFlowed[i])
		{
			glVertex3d(X_Array_Display[i],0.0,0.1);
			glVertex3d(X_Array_Display[i+1],0.0,0.1);
		}
	}
	glEnd();
	glPopMatrix();
#pragma endregion CH1

	if(name[1] == 'a')//raw
		glColor4d(0.5,0.5,1.0,1.0);
	else if(name[1] == 'o')//name =="Low Inhibit")
		glColor4d(0.5,0.5,0.9,1.0);
	else if(name[1] == 'e')//name =="Reward")
		glColor4d(0.5,0.5,0.8,1.0);
	else if(name[1] == 'i')//=="High Inhibit")
		glColor4d(0.5,0.5,0.7,1.0);

#pragma region CH2
	glPushMatrix();
	glTranslatef(width_,org_Y,0.1f);
	///////////////////draw grid////////////

	glPushMatrix();
	{
		glTranslated(5,0.5*g_h,0);

		glColor3d(1.0,1.0,1.0);
		glBegin(GL_QUADS);
		glVertex3d(0,0,-0.2);
		glVertex3d(g_w,0,-0.2);
		glVertex3d(g_w,-g_h,-0.2);
		glVertex3d(0,-g_h,-0.2);
		glEnd();

		glColor3d(0.96,0.96,0.96);
		glBegin(GL_QUADS);
		glVertex3d(0,0,-0.19);
		glVertex3d(g_w,0,-0.19);
		glVertex3d(g_w,-d_h,-0.19);
		glVertex3d(0,-d_h,-0.19);

		glVertex3d(0,-2*d_h,-0.19);
		glVertex3d(g_w,-2*d_h,-0.19);
		glVertex3d(g_w,-3*d_h,-0.19);
		glVertex3d(0,-3*d_h,-0.19);

		glVertex3d(0,-4*d_h,-0.19);
		glVertex3d(g_w,-4*d_h,-0.19);
		glVertex3d(g_w,-5*d_h,-0.19);
		glVertex3d(0,-5*d_h,-0.19);

		glVertex3d(0,-6*d_h,-0.19);
		glVertex3d(g_w,-6*d_h,-0.19);
		glVertex3d(g_w,-7*d_h,-0.19);
		glVertex3d(0,-7*d_h,-0.19);

		glEnd();



		glColor3d(0,0,0);
		glLineWidth(2.0);
		glBegin(GL_LINE_STRIP);
		glVertex2d(0,0);
		glVertex2d(g_w,0);
		glVertex2d(g_w,-g_h);
		glVertex2d(0,-g_h);
		glVertex2d(0,0);
		glEnd();

		glLineWidth(1.0);
		glBegin(GL_LINES);
		for(int i =0;i<6;i++)
		{
			glVertex2d(0,-(i+1)*d_h);
			glVertex2d(g_w,-(i+1)*d_h);
		}
		glVertex2d(0.5*g_w,0);
		glVertex2d(0.5*g_w,-g_h);
		glEnd();


	}
	glPopMatrix();
	///////////////////draw grid////////////

	glPushMatrix();
	glTranslated(0,70,0);
	
	//ss_2<<std::fixed<<std::setprecision(1)<<start_hz<<" ~ "<<end_hz<<"Hz";
	glPushMatrix();
	glColor3d(0.0,0.0,0.0);

	if(name[1] == 'a')//raw
	{
		DrawString(" ");
		glRasterPos3d(g_w*0.25*(1.0 - 1.0/7.0), -17.0, 0.0);
	}
	else if(name[1] == 'e')
		glRasterPos3d(g_w*0.25*(1.0 - 2.5/7.0), -17.0, 0.0);
	else if(name[1] == 'o')
		glRasterPos3d(g_w*0.25*(1.0 - 4.0/7.0), -17.0, 0.0);
	else if(name[1] == 'i')
		glRasterPos3d(g_w*0.25*(1.0 - 4.0/7.0), -17.0, 0.0);

	DrawString(name);
	if(name[1] == 'a')//raw
		glRasterPos3d(g_w*0.75*(1.0 - 3.0/21.0), -17.0, 0.0);
	else if(name[1] == 'e')
		glRasterPos3d(g_w*0.75*(1.0 - 3.0/21.0), -17.0, 0.0);
	else if(name[1] == 'o')
		glRasterPos3d(g_w*0.75*(1.0 - 2.5/21.0), -17.0, 0.0);
	else if(name[1] == 'i')
		glRasterPos3d(g_w*0.75*(1.0 - 3.5/21.0), -17.0, 0.0);
	//glRasterPos3d(110.0, -17.0, 0.0);
	//DrawString((ss_2.str()).c_str());
	glPopMatrix();
	//ss_2.str("");


	//ss_2<<"Scale                 "<<std::fixed<<std::setprecision(1)<<scal_v;
	glPushMatrix();
	glColor3d(0.0,0.0,0.0);
	glRasterPos3d(20.0, -37.0, 0.0);

	glRasterPos3d(g_w*0.25*(1.0 - 2/7.0), -37.0, 0.0);

	//DrawString((ss_2.str()).c_str());
	glPopMatrix();
	//ss_2.str("");

	//ss_2<<"AMP Thresh       "<<std::fixed<<std::setprecision(1)<<ThreshValue_display;
	//ss_2<<"AMP Thresh           "<<std::fixed<<std::setprecision(1)<<ThreshValue;
	//ss_2<<"AMP Thresh       "<<std::fixed<<std::setprecision(1)<<Recoded_Array.size();
	glPushMatrix();
	if(!IsThreshVChange)
		glColor3d(0.0,0.0,0.0);
	else
	{
		glColor3d(1.0,0.0,0.0);
		if(CountForThreshChange<6)
			CountForThreshChange++;
		else
		{
			CountForThreshChange = 0;
			IsThreshVChange = false;
		}
	}


	glRasterPos3d(20.0, -57.0, 0.0);

	//DrawString((ss_2.str()).c_str());
	glPopMatrix();
	//ss_2.str("");

	//ss_2<<"AMP AVG              "<<std::fixed<<std::setprecision(1)<<Avg_amp;
	//ss_2<<"OTC            "<<std::fixed<<std::setprecision(1)<<CountofOverSample;
	glPushMatrix();
	glColor3d(0.0,0.0,0.0);
	glRasterPos3d(20.0, -77.0, 0.0);

	//DrawString((ss_2.str()).c_str());
	glPopMatrix();
	//ss_2.str("");


	/*if(IsInhibit)
		ss_2<<"< Target%             "<<std::fixed<<std::setprecision(2)<<(100.0-TargetOverPercent);
	else
		ss_2<<"> Target%             "<<std::fixed<<std::setprecision(2)<<TargetOverPercent;*/
	glPushMatrix();
	if(IsInhibit)
		glColor3d(1.0,0.0,0.0);
	else
		glColor3d(0.0,0.5,0.0);

	glRasterPos3d(20.0, -137.0, 0.0);

	//DrawString((ss_2.str()).c_str());
	glPopMatrix();
	//ss_2.str("");

	if(!IsRawWave)
	{	
		//ss_2<<"AMP                  +/- "<<std::fixed<<std::setprecision(1)<<Avg_amp_1sec;
		//ss_2<<"AMP 8sec              "<<std::fixed<<std::setprecision(1)<<Avg_amp_8sec;
		//ss_2<<"OTC2            "<<std::fixed<<std::setprecision(1)<<CountofOverSample_;
		glPushMatrix();
		glColor3d(0.0,0.0,0.0);
		glRasterPos3d(20.0, -97.0, 0.0);

		//DrawString((ss_2.str()).c_str());
		glPopMatrix();
		//ss_2.str("");

		/*if(IsInhibit)
			ss_2<<name<<"%          "<<std::fixed<<std::setprecision(2)<<(OverVpercent_8sec)*100;
		else
			ss_2<<name<<"%               "<<std::fixed<<std::setprecision(2)<<(OverVpercent_8sec)*100;*/
		/*if(IsInhibit)
			ss_2<<name<<"%          "<<std::fixed<<std::setprecision(2)<<(OverVpercent)*100;
		else
			ss_2<<name<<"%               "<<std::fixed<<std::setprecision(2)<<(OverVpercent)*100;*/

		glPushMatrix();
		glColor3d(0.0,0.0,0.0);
		glRasterPos3d(20.0, -117.0, 0.0);

		//DrawString((ss_2.str()).c_str());
		glPopMatrix();
		//ss_2.str("");
	}
	else
	{	
		//ss_2<<name<<"%                 "<<std::fixed<<std::setprecision(2)<<(OverVpercent_8sec)*100;
		//ss_2<<name<<"%                    "<<std::fixed<<std::setprecision(2)<<(OverVpercent)*100;
		//ss_2<<name<<"%                 "<<std::fixed<<std::setprecision(2)<<CountofOverSample;
		glPushMatrix();
		glColor3d(0.0,0.0,0.0);
		glRasterPos3d(20.0, -117.0, 0.0);

		//DrawString((ss_2.str()).c_str());
		glPopMatrix();
		//ss_2.str("");
	}
	glPopMatrix();
	//////////////END GRID//////////

	///////////////////////////////draw thresh value line
	glLineWidth(1.0);
	glPushMatrix();
			if(name[1] == 'a')//raw
		{
			glColor4d(0.0,0.0,0.0,1.0);
			//glColor4d(0.5,0.5,1.0,1.0);
		}
		else if(name[1] == 'o')//name =="Low Inhibit")
		{
			//glColor4d(1.0,0.75,0.0,1.0);
			glColor4d(1.0,0.0,0.0,1.0);
		}
		else if(name[1] == 'e')//name =="Reward")
		{
			//glColor4d(1.0,0.5,0.0,1.0);
			glColor4d(0.0,0.75,0.0,1.0);
		}
		else if(name[1] == 'i')//=="High Inhibit")
		{
			//glColor4d(1.0,0.25,0.0,1.0);
			glColor4d(0.0,0.0,0.75,1.0);
		}

		glBegin(GL_LINES);
		if(abs(0.5*ThreshValue*scal_v) < 100)
		{
			glVertex2d(X_Array_Display[0],-0.5*ThreshValue*scal_v);
			glVertex2d(X_Array_Display[2047],-0.5*ThreshValue*scal_v);
			glVertex2d(X_Array_Display[0],0.5*ThreshValue*scal_v);
			glVertex2d(X_Array_Display[2047],0.5*ThreshValue*scal_v);
		}
		else
		{
			glVertex2d(X_Array_Display[0],-99 );
			glVertex2d(X_Array_Display[2047],-99 );
			glVertex2d(X_Array_Display[0],99 );
			glVertex2d(X_Array_Display[2047],99 );
		}
		glEnd();
	glPopMatrix();

	glLineWidth(1);
	//glHint(GL_LINE_SMOOTH_HINT,GL_NICEST);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glEnable(GL_LINE_SMOOTH);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	glColor3d(0.0,0.0,0.0);
	glBegin(GL_LINE_STRIP);
	for (int i = 0;i < Current_sample_indx;i++)
		if(abs(Y_Array_Display[i]*scal_v) < 100)
			glVertex3d(X_Array_Display[i],Y_Array_Display[i]*scal_v,0.05);
		else
		{
			if(Y_Array_Display[i] > 0)
				glVertex3d(X_Array_Display[i],100,0.05);
			else
				glVertex3d(X_Array_Display[i],-100,0.05);
		}

		glEnd();
	

	glDisable(GL_BLEND);
	glDisable(GL_LINE_SMOOTH);
	glEnable(GL_DEPTH_TEST);

	if(IsInhibit)
		glColor3d(1.0,0.0,0.0);
	else
		glColor3d(0.0,1.0,0.0);


	glLineWidth(4);
	glBegin(GL_LINES);
	
	for (int i = 0;i < Current_sample_indx -1;i++)
	{
		if(IsOverFlowed[i])
		{
			glVertex3d(X_Array_Display[i],0.0,0.1);
			glVertex3d(X_Array_Display[i+1],0.0,0.1);
		}
	}
	glEnd();
	glPopMatrix();
#pragma endregion CH2
	
	
}


void EEGWave::UpdateWave(double rawwave[],bool isRecoding,double FFT_AS[])
{
	IsRecoding_notrawdata = isRecoding;

	if(Current_sample_indx != 0&&Current_sample_indx<=2048)
	{
		//Recoded_Data_Change_flag = true;

		//Convolution(FIR,FIR_TAP,rawwave->Y_Array_Display,FirDataSet.Window_width,Y_Array_Display);//2048-32 = window width->2016->1984
#pragma region FILTER_PROCESSING3
		//double array_buf_in[512];
		//double array_buf_out[512];
		//int sec_ = (int)(Current_sample_indx/256);
		//if(sec_ >=1 && sec_ < 8)//1-7sec
		//{
		//	int j = 0;
		//	for (int i = (sec_ - 1)*256;i < (sec_ + 1)*256;i++,j++)
		//	{
		//		array_buf_in[j] = rawwave[i];
		//	}
		//}
		//else if(sec_==8)//sec==8sec
		//{
		//	int j = 0;
		//	for (int i = 6*256;i < 8*256;i++,j++)
		//	{
		//		array_buf_in[j] = rawwave[i];
		//	}
		//	for (int i = 0;i < 256;i++)
		//	{
		//		Buf_1sec[i] = rawwave[i + 256*7];
		//		//Buf_1sec[i] = Y_Array_Display[i + 256*7];
		//	}
		//}
		//else if(sec_ == 0)
		//{
		//	int j = 0;
		//	for (int i = 0;i < 256;i++,j++)
		//	{
		//		array_buf_in[j] = Buf_1sec[i];
		//	}
		//	for (int i = 0;i < 256;i++,j++)
		//	{
		//		array_buf_in[j] = rawwave[i];
		//	}
		//}

		////filterController_.IIRFilterProcessing(rawwave->Y_Array_Display,Y_Array_Display,2048);
		//filterController_.IIRFilterProcessing(array_buf_in,array_buf_out,512);
		////filterController_.IIRFilterProcessing(array_buf_in,array_buf_out,768);

		//if(sec_ <= 7)//1-7sec
		//{
		//	int j = 256;
		//	for (int i = sec_*256;i < (sec_ + 1)*256;i++,j++)
		//	{
		//		Y_Array_Display[i] = array_buf_out[j];
		//	}
		//}
		//else if(sec_==8)
		//{
		//	int j = 256;
		//	for (int i = 7*256;i < 8*256;i++,j++)
		//	{
		//		Y_Array_Display[i] = array_buf_out[j];
		//	}
		//}
#pragma endregion FILTER_PROCESSING3
#pragma region FILTER_PROCESSING2
		//double array_buf_in[512];
		//double array_buf_out[512];
	 //   int sec_ = (int)(Current_sample_indx/256);
		//if(sec_ >=1 && sec_ < 8)//1-7sec
		//{
		//	int j = 0;
		//	for (int i = (sec_ - 1)*256;i < (sec_)*256;i++,j++)
		//	{
		//		array_buf_in[j] = Y_Array_Display[i];
		//	}
		//	for (int i = (sec_)*256;i < (sec_ + 1)*256;i++,j++)
		//	{
		//		array_buf_in[j] = rawwave[i];
		//	}
		//}
		//else if(sec_==8)//sec==8sec
		//{
		//	int j = 0;
		//	for (int i = 6*256;i < 7*256;i++,j++)
		//	{
		//		array_buf_in[j] = Y_Array_Display[i];
		//	}
		//	for (int i = 7*256;i < 8*256;i++,j++)
		//	{
		//		array_buf_in[j] = rawwave[i];
		//	}
		//	for (int i = 0;i < 256;i++)
		//	{
		//		//Buf_1sec[i] = rawwave[i + 256*7];
		//		Buf_1sec[i] = Y_Array_Display[i + 256*7];
		//	}
		//}
		//else if(sec_ == 0)
		//{
		//	int j = 0;
		//	for (int i = 0;i < 256;i++,j++)
		//	{
		//		array_buf_in[j] = Buf_1sec[i];
		//	}
		//	for (int i = 0;i < 256;i++,j++)
		//	{
		//		array_buf_in[j] = rawwave[i];
		//	}
		//}
		//
	 //   //filterController_.IIRFilterProcessing(rawwave->Y_Array_Display,Y_Array_Display,2048);
		//filterController_.IIRFilterProcessing(array_buf_in,array_buf_out,512);
		////filterController_.IIRFilterProcessing(array_buf_in,array_buf_out,768);

		//if(sec_ <= 7)//1-7sec
		//{
		//	int j = 256;
		//	for (int i = sec_*256;i < (sec_ + 1)*256;i++,j++)
		//	{
		//		Y_Array_Display[i] = array_buf_out[j];
		//	}
		//}
		//else if(sec_==8)
		//{
		//	int j = 256;
		//	for (int i = 7*256;i < 8*256;i++,j++)
		//	{
		//		Y_Array_Display[i] = array_buf_out[j];
		//	}
		//}
#pragma endregion FILTER_PROCESSING2
#pragma region FILTER_PROCESSING1

		//double array_buf_in[2304];//256+2048
		//double array_buf_out[2304];
		////int sec_ = (int)(Current_sample_indx/256);//new method Current_sample_indx max at 2048-32
		//int jjj = 0;
		//for (int i = 0;i < 256;i++,jjj++)
		//{
		//	//if(abs(Buf_1sec[i]) > RawThreshValueBuf)//15.0= 60.0*0.5*scal(0.5) is the thresold of raw
		//	//{
		//	//	if(Buf_1sec[i] > 0)
		//	//		array_buf_in[jjj] = RawThreshValueBuf;
		//	//	else
		//	//		array_buf_in[jjj] = -1*RawThreshValueBuf;
		//	//}
		//	//else
		//		array_buf_in[jjj] = Buf_1sec[i];
		//}
		//for (int i = 0;i < 2048;i++,jjj++)
		//{
		//	//if(abs(rawwave[i]) > 15.0)//60.0 is the thresold of raw
		//	//{
		//	//	if(rawwave[i]>0)
		//	//		array_buf_in[jjj] = RawThreshValueBuf;
		//	//	else
		//	//		array_buf_in[jjj] = -1 * RawThreshValueBuf;
		//	//}
		//	//else 
		//		array_buf_in[jjj] = rawwave[i];
		//}
		////if(sec_%7 == 0 && sec_ != 0)//new method Current_sample_indx max at 2048-32
		//if(Current_sample_indx == 2048)//2048-32
		//{
		//	//countttt++;
		//	for (int i = 0;i < 256;i++)
		//	{
		//		/*if(abs(rawwave[i + 256*7]) >= 15.0)
		//		{
		//			if(rawwave[i + 256*7]>0)
		//				Buf_1sec[i] = RawThreshValueBuf;
		//			else
		//				Buf_1sec[i] = -1 * RawThreshValueBuf;
		//		}		
		//		else */
		//			Buf_1sec[i] = rawwave[i + 256*7];
		//	}
		//}
		//filterController_.IIRFilterProcessing(array_buf_in,array_buf_out,2304);
		//if(name[1] == 'o')//name =="Low Inhibit"
		//{
		//	for (int i = 0;i < 2048;i++)
		//	{
		//		double gain = 0.92;
		//		if(i > 0 && i <= 64)
		//			Y_Array_Display[i] = 1.01 * array_buf_out[256+i];
		//		else if(i > 64 && i <= 128)
		//			Y_Array_Display[i] = 0.92 * array_buf_out[256+i];
		//		else if(i > 128 && i <= 192)
		//			Y_Array_Display[i] = 0.94 * array_buf_out[256+i];
		//		else if(i > 192 && i <= 224)
		//			Y_Array_Display[i] = 0.94 * array_buf_out[256+i];
		//		else
		//			Y_Array_Display[i] = array_buf_out[256+i];
		//	}
		//}
		//else if(name[1] == 'e')//name =="Reward"
		//{
		//	for (int i = 0;i < 2048;i++)
		//	{
		//		/*double gain = 0.92;
		//		if(i > 0 && i <= 64)
		//			Y_Array_Display[i] = 1.01 * array_buf_out[256+i];
		//		else if(i > 64 && i <= 128)
		//			Y_Array_Display[i] = 0.92 * array_buf_out[256+i];
		//		else if(i > 128 && i <= 192)
		//			Y_Array_Display[i] = 0.94 * array_buf_out[256+i];
		//		else if(i > 192 && i <= 224)
		//			Y_Array_Display[i] = 0.94 * array_buf_out[256+i];
		//		else*/
		//			Y_Array_Display[i] = array_buf_out[256+i];
		//	}
		//}
		//else 
		//{
		//	for (int i = 0;i < 2048;i++)
		//	{
		//		/*double gain = 0.92;
		//		if(i > 0 && i <= 64)
		//			Y_Array_Display[i] = 1.01 * array_buf_out[256+i];
		//		else if(i > 64 && i <= 128)
		//			Y_Array_Display[i] = 0.92 * array_buf_out[256+i];
		//		else if(i > 128 && i <= 192)
		//			Y_Array_Display[i] = 0.94 * array_buf_out[256+i];
		//		else if(i > 192 && i <= 224)
		//			Y_Array_Display[i] = 0.94 * array_buf_out[256+i];
		//		else*/
		//			Y_Array_Display[i] = array_buf_out[256+i];
		//	}
		//}

#pragma endregion FILTER_PROCESSING1

#pragma region FILTER_PROCESSING1_1
		//double array_buf_in[544];//512 + 32
		//double array_buf_out[544];
		//for (int i = 0;i < 544; i++)
		//{
		//	//if(abs(rawwave[i]) > 15.0)//60.0 is the thresold of raw
		//	//{
		//	//	if(rawwave[i]>0)
		//	//		array_buf_in[i] = RawThreshValueBuf;
		//	//	else
		//	//		array_buf_in[i] = -1 * RawThreshValueBuf;
		//	//}
		//	//else 
		//		array_buf_in[i] = rawwave[i];
		//		array_buf_out[i] = New_IIR_filter.FilterProcess(array_buf_in[i]);
		//}
		//
		////filterController_.IIRFilterProcessing(array_buf_in,array_buf_out,544);
		///*for (int i = Current_sample_indx - 32,j = 0; i < Current_sample_indx; i++,j++)
		//{
		//	Y_Array_Display[i] = array_buf_out[512 + j];
		//}*/
		//if(Current_sample_indx < 32)
		//{
		//	for (int i = 0; i < Current_sample_indx; i++)
		//	{
		//		Y_Array_Display[i] = array_buf_out[544 - Current_sample_indx + i];
		//	}
		//}
		//else
		//{
		//	for (int i = Current_sample_indx - 32,j = 0; i < Current_sample_indx; i++,j++)
		//	{
		//		//Y_Array_Display[i] = array_buf_out[256+128+64+32+j];
		//		Y_Array_Display[i] = array_buf_out[512+j];
		//	}
		//}
		//

#pragma endregion FILTER_PROCESSING1_1
#pragma region FILTER_PROCESSING1_2
		/*for (int i = 512,k=0;i < 2560; i++,k++)
		{
			Y_Array_Display[k] = New_IIR_filter.FilterProcess(rawwave[i]);
		}*/ 
		double buf2560[2560];
		for (int i = 0; i < 2560; i++)
			buf2560[i] = rawwave[i];// New_IIR_filter.FilterProcess(rawwave[i]);
		for (int k = Current_sample_indx -32;k < Current_sample_indx;k++)
		{
			Y_Array_Display[k] = buf2560[k];
		}
#pragma endregion FILTER_PROCESSING1_2
		if(isRecoding)
		{
			//Recoded_Data_Change_flag = true;//lock
			for (int i = Current_sample_indx - BUF_SIZE;i < Current_sample_indx;i++)
			{
				CountOfRecodeSamples++;
				bool isover = abs(Y_Array_Display[i])>0.5*ThreshValue?true:false;
				
				Recoded_Array.push_back(Y_Array_Display[i]);
				Recoded_Array_IsOver.push_back(isover);
				if(isover)
					CountofOverSample++;
				IsOverFlowed[i] = isover;
				//Total_amp += abs(Y_Array_Display[i]);
			}
			//Recoded_Data_Change_flag = false;//unlock
		}
#pragma region NEW_METHOD_OF_PROCESS_WAVE_OVERFLOW
		//int pluscount = 0,minuscount = 0,fisrt_plusconut =0;
		//int step_ = 0;
		//bool plusflag = false;
		//bool minusflag = false;
		//bool IsFinish = false;
		//if(Current_sample_indx >= 2.0*BUF_SIZE)
		//for (int i = Current_sample_indx-2.0*BUF_SIZE;i< Current_sample_indx;i++)
		//{
		//	if (!IsFinish)
		//	{
		//		//if (abs(Y_Array_Display[i])>0.5*ThreshValue)
		//		if(IsOverFlowed[i])
		//		{
		//			if(!plusflag)step_++;
		//			if(step_==3)
		//			{
		//				IsFinish = true;
		//				i--;
		//			}
		//			plusflag = true;
		//			minusflag = false;
		//			pluscount++;
		//			//IsOverFlowed[i] = true;
		//			//if(!IsOverFlowed[i])
		//			//{
		//			//	int derta = Current_sample_indx - i;
		//			//	
		//			//	if(CountOfRecodeSamples >= derta && CountOfRecodeSamples>0)
		//			//	{
		//			//		if(!Recoded_Array_IsOver[CountOfRecodeSamples - derta])
		//			//		{
		//			//			Recoded_Array_IsOver[CountOfRecodeSamples - derta] = true;//the new Is Over Flowed result
		//			//			CountofOverSample++;
		//			//			IsOverFlowed[i]= true;
		//			//		}
		//			//	}
		//			//}
		//		}
		//		else
		//		{
		//			if(step_ != 0)
		//			{
		//				if(!minusflag) step_++;
		//				if((double)minuscount > 128.0/(double)start_hz)
		//				{
		//					IsFinish = true;
		//					i--;
		//				}
		//				minusflag = true;
		//				plusflag = false;
		//				minuscount++;
		//			}
		//		}
		//	}
		//	else
		//	{
		//		/*if(step_ == 2)
		//		{
		//			for(int j = i - pluscount - minuscount;j < i - minuscount;j++)
		//			{
		//				IsOverFlowed[j] = false;		
		//			}
		//		}*/
		//		if(step_ == 3)
		//		{
		//			//if((pluscount > OffsetOverThresh * minuscount) && plusflag&&i>=minuscount)
		//			if(pluscount > 0)
		//			{
		//				for(int j  = i - (minuscount+pluscount);j < i;j++)
		//				//for(int j = i - minuscount;j < i ;j++)
		//				//for(int j = i - minuscount;j < i ;j++)
		//				{
		//					if(!IsOverFlowed[j])//&&(j+BUF_SIZE > Current_sample_indx))
		//					{
		//						IsOverFlowed[j]= true;
		//						if(isRecoding && (CountOfRecodeSamples >= Current_sample_indx - j))
		//						{
		//							int derta = Current_sample_indx - j;
		//							if(!Recoded_Array_IsOver[CountOfRecodeSamples - derta])
		//							{
		//								Recoded_Array_IsOver[CountOfRecodeSamples - derta] = true;//the new Is Over Flowed result
		//								CountofOverSample++;
		//							}
		//						}
		//					}
		//				}
		//			}
		//		}
		//		step_ = 0;
		//		minuscount = 0;
		//		pluscount = 0;
		//		IsFinish = false;
		//		plusflag = false;
		//		minusflag = false;
		//	}
		//}

#pragma endregion NEW_METHOD_OF_PROCESS_WAVE_OVERFLOW
#pragma region WAVE_OVERFLOW_USING_PEAK
		if(isRecoding)
		{
			/*if(start_hz < 6)
			{
				if((CountOfRecodeSamples >= 2.0*BUF_SIZE)
					&&(Current_sample_indx >= 2.0*BUF_SIZE))
					AMPThresholdIsOverfByPeakMethod(end_hz,2.0*BUF_SIZE);
			}*/
			if(start_hz >= 12)
			{
				if((CountOfRecodeSamples >= BUF_SIZE)
					&&(Current_sample_indx >= BUF_SIZE))
					AMPThresholdIsOverfByPeakMethod(end_hz,BUF_SIZE);
			}
			else
			{
				if((CountOfRecodeSamples >= 2.0*BUF_SIZE)
					&&(Current_sample_indx >= 2.0*BUF_SIZE))
					AMPThresholdIsOverfByPeakMethod(end_hz,2.0*BUF_SIZE);
			}
			#pragma region FILTERSHORTLINE
			//////////////////////////////////////////////filter the short line
			if(!IsRawWave)
			{
				int len_threshold;
				if(name[1] == 'o')
					len_threshold = 16;
				else if(name[1] == 'e')
					len_threshold = 8;
				else if(name[1] == 'i')
					len_threshold = 4;

				int lenth = 0;
				for (int i = 0;i< 2048;i++)
				{
					if(IsOverFlowed[i])
					{
						lenth++;
					}
					else
					{
						if(lenth < len_threshold && lenth > 0)
						{
							int j = i - 1;
							while(lenth != 0)
							{
								lenth--;
								IsOverFlowed[j--] = false;
							}
						}
						lenth = 0;	
					}
				}
			}
#pragma endregion FILTERSHORTLINE
#pragma region combine_short_line
			if(!IsRawWave)
			{	
				////////////////////////////////////////////////combine short line
				int len_ = 0;
				int len_threshold;
				if(name[1] == 'o')
					len_threshold = 10;
				else if(name[1] == 'e')
					len_threshold = 8;
				else if(name[1] == 'i')
					len_threshold = 7;//256/36hz = 7.001


				for (int i = 0;i< 2048;i++)
				{
					if(!IsOverFlowed[i])
					{
						len_++;
					}
					else
					{
						if(len_ < len_threshold && len_ > 0)
						{
							int j = i - 1;
							while(len_ != 0)
							{
								len_--;
								IsOverFlowed[j--] = true;
							}
						}
						len_ = 0;
					}
				}
				////////////////////////////////////////////////
			}
#pragma endregion combine_short_line
		}
			
		
#pragma endregion WAVE_OVERFLOW_USING_PEAK
#pragma region For_Avg_amp_OverVpercen_8secOverP
		if(isRecoding)
		{
			/////////////new Avg_amp////////////
			//if(rawwave->Recoded_Array.size() >= 2048)
			{
				//int start_hz = FirDataSet.lf;
				//int end_hz = FirDataSet.hf;
				//int start_hz = 128.0*(IirDataSet.OmegaC - 0.5*IirDataSet.BandWidth);
		//		if(start_hz == 22&&rawwave->Recoded_Array.size() >= 2600)
		//			start_hz=start_hz;
				//end_hz = 128.0*(IirDataSet.OmegaC + 0.5*IirDataSet.BandWidth);
				double sum_as = 0;
				double max_as = 0;
				double min_as = 10000;
				int count_unzero =0;
				
				//if(name[1] == 'i'&&Recoded_Array.size()>2548)
				//	count_unzero = count_unzero;
				for (int i = start_hz;i <= end_hz;i++)
				{
					//if(abs(FFT_AS[i]) > 0.001)
					if(abs(FFT_AS[i]) > 0.01)
					{
						sum_as += abs(FFT_AS[i]);//*scal_;//*scal_v;//0.4-0.5?
						count_unzero++;
					}
					
					if(max_as < abs(FFT_AS[i]))
					{
						max_as = abs(FFT_AS[i]);
					}
					if(min_as > abs(FFT_AS[i]))
					{
						min_as = abs(FFT_AS[i]);
					}
					amphz[i-start_hz]= FFT_AS[i];
				}
				////////////////////////////////////////
				/*if(IsNeedComputeThreshold)
				{
					ThreshValue_buf = 0.0;
					for (int i = start_hz;i <= end_hz;i++)
					{
						if(!IsInhibit)
							ThreshValue_buf += cos(0.5*PI*(0.01*TargetOverPercent))*abs(FFT_AS[i])*abs(FFT_AS[i])/sum_as;
						else
							ThreshValue_buf += 2.0*cos(0.5*PI*(1.0 - 0.01*TargetOverPercent))*abs(FFT_AS[i])*abs(FFT_AS[i])/sum_as;
					}
					IsNeedComputeThreshold = false;
				}*/
				
				////////////////////////////////////
				//Avg_amp_8sec = max_as;
				
				//Avg_amp_8sec = sum_as/(end_hz - start_hz+1);
				//Avg_amp_8sec = scal_v*sum_as/count_unzero;
				
				//Avg_amp_8sec = sum_as/((end_hz - start_hz)*8+1);
				//Avg_amp_8sec = sum_as/((end_hz - start_hz)*8);
				
				//Avg_amp_8sec = max_as +min_as;
				//if(count_unzero != 0)
				//	Avg_amp_8sec = 2.0*sum_as/(double)(count_unzero-1);
				//	//Avg_amp_8sec = sum_as;
				//else
				//	Avg_amp_8sec = 0;

				if(count_unzero != 0)
					Avg_amp_8sec = sum_as/count_unzero;
				else
					Avg_amp_8sec = 0.0;

				Avg_amp_8sec *= 2.0;

				

				/*if(!Peak_8sec_Arry.empty())
				{
					double sum_ = 0;
					for (int i =0;i<Peak_8sec_Arry.size();i++)
					{
						sum_ += Peak_8sec_Arry[i];
					}
					sum_ /= Peak_8sec_Arry.size();
					Avg_amp_8sec = sum_;
				}
				else
				{
					Avg_amp_8sec = 0;
				}*/
				
				Avg_amp_8sec_Arry.push_back(Avg_amp_8sec);
				if(Avg_amp_8sec_Arry.size() >= 64)//8*8  
				{
					if(Avg_amp_8sec < MinAMP)
						MinAMP = Avg_amp_8sec;
					if(Avg_amp_8sec > MaxAMP)
						MaxAMP = Avg_amp_8sec;
				}
			
				//////////////////////////////Std dev
				//double sum_sd = 0;
				//for (int i = start_hz;i < end_hz;i++)
				//{
				//	sum_sd += (FFT_AS[i] - Avg_amp_8sec)*(FFT_AS[i] - Avg_amp_8sec);
				//}
				//sum_sd = sqrt(sum_sd /(double)(end_hz - start_hz));
				Total_AVG_AMP += Avg_amp_8sec;
				//////////////////////////////Std dev

				//if(Avg_amp_8sec > 2.0*RawThreshValueBuf)//NO HALF AMP.SO...*2.0
				//	Avg_amp_8sec = 2.0*RawThreshValueBuf;

				Max_amp_8sec = max_as;
				//if(CountOfRecodeSamples>=2048)
					Total_amp += abs(Avg_amp_8sec);
			}
			//if(Recoded_Array.size() > 3000)
			//	Max_amp_8sec = Max_amp_8sec;
			/////////////new Avg_amp////////////
			
			if(CountOfRecodeSamples > 0)
			{
				int countofupdate = int(CountOfRecodeSamples/32);
				Avg_amp = Total_amp/countofupdate;
				//SDAMP = Total_AVG_AMP/countofupdate;
				if(!IsRawWave)
				{
					if(IsInhibit)
						OverVpercent = (double)CountofOverSample/CountOfRecodeSamples;
					else
						OverVpercent = 1.0-(double)CountofOverSample/CountOfRecodeSamples;
				}
				//rawwave OverVpercent is computed in GetRawDataFromUSB
			

				if(CountOfRecodeSamples >= 2048)
				{
					int count_ = 0;
					int count_32 = 0;
					//double sum = 0;
					for (int i = CountOfRecodeSamples -2048;i<CountOfRecodeSamples;i++)
					//for (unsigned int i = 0;i < 2048;i++)
					{
						//sum += abs(Recoded_Array[i].vol);
						if(Recoded_Array_IsOver[i])
							count_++;
						//if(i >= CountOfRecodeSamples - 96 
						//	&& i < CountOfRecodeSamples - 64)
						if(i >= CountOfRecodeSamples - 64 
							&& i < CountOfRecodeSamples - 32)
						{
							if(Recoded_Array_IsOver[i])count_32++;
						}
					}
					OverVpercent_8sec = (double)count_/2048;
					//Avg_amp_8sec = sum/2048;
					//if(count_32 > 32)
					//	count_32 = 32;
					//if(start_hz == 22)
					T_SCount += count_32;
					T_success = (double)T_SCount/CountOfRecodeSamples;
				}
				else
				{
					//double sum = 0;
					int count_ = 0;
					for (int i = 0;i< CountOfRecodeSamples;i++)
					{
						//sum += abs(Recoded_Array[i].vol);
						if(Recoded_Array_IsOver[i])
							count_++;
					}
					OverVpercent_8sec = (double)count_/CountOfRecodeSamples;
					//Avg_amp_8sec = sum/len_2;
					T_SCount = count_;
					T_success = (double)T_SCount/CountOfRecodeSamples;
				}
			}
			/*if(IsInhibit)
				IsInhibit = IsInhibit;*/
			if(IsInhibit)
			{
				if(abs(OverVpercent_8sec*100 -(100.0 - TargetOverPercent)) > offset_Target)
					FlagNeedNewThreshold = true;
				else
					FlagNeedNewThreshold = false;
			}
			else
			{
				if(abs(OverVpercent_8sec*100 - TargetOverPercent)> offset_Target)
					FlagNeedNewThreshold = true;
				else
					FlagNeedNewThreshold = false;
			}
		
		}
#pragma endregion For_Avg_amp_OverVpercen_8secOverP
	}
	/*else
	{
		AfxMessageBox(L"EEGWAVE_Update_Err!!!!!!!!!", MB_OK);
	}*/
}
void EEGWave::UpdateWave_2(double rawwave[],double rawwave2[],bool isRecoding,double FFT_AS[],double FFT_AS2[])
{
	IsRecoding_notrawdata = isRecoding;

	if(Current_sample_indx != 0&&Current_sample_indx<=2048)
	{
#pragma region FILTER_PROCESSING1
		double array_buf_in[2304];//256+2048
		double array_buf_out[2304];
		double array_buf_in_2[2304];//256+2048
		double array_buf_out_2[2304];
		//int sec_ = (int)(Current_sample_indx/256);//new method Current_sample_indx max at 2048-32
		int jjj = 0;
		for (int i = 0;i < 256;i++,jjj++)
		{
			if(abs(Buf_1sec[i]) > RawThreshValueBuf)//15.0= 60.0*0.5*scal(0.5) is the thresold of raw
			{
				if(Buf_1sec[i] > 0)
					array_buf_in[jjj] = RawThreshValueBuf;
				else
					array_buf_in[jjj] = -1*RawThreshValueBuf;
			}
			else
				array_buf_in[jjj] = Buf_1sec[i];

			if(abs(Buf_1sec_2[i]) > RawThreshValueBuf)//15.0= 60.0*0.5*scal(0.5) is the thresold of raw
			{
				if(Buf_1sec_2[i] > 0)
					array_buf_in_2[jjj] = RawThreshValueBuf;
				else
					array_buf_in_2[jjj] = -1*RawThreshValueBuf;
			}
			else
				array_buf_in_2[jjj] = Buf_1sec_2[i];
		}
		for (int i = 0;i < 2048;i++,jjj++)
		{
			if(abs(rawwave[i]) > 15.0)//60.0 is the thresold of raw
			{
				if(rawwave[i]>0)
					array_buf_in[jjj] = RawThreshValueBuf;
				else
					array_buf_in[jjj] = -1 * RawThreshValueBuf;
			}
			else 
				array_buf_in[jjj] = rawwave[i];

			if(abs(rawwave2[i]) > 15.0)//60.0 is the thresold of raw
			{
				if(rawwave2[i]>0)
					array_buf_in_2[jjj] = RawThreshValueBuf;
				else
					array_buf_in_2[jjj] = -1 * RawThreshValueBuf;
			}
			else 
				array_buf_in_2[jjj] = rawwave2[i];
		}
		//if(sec_%7 == 0 && sec_ != 0)//new method Current_sample_indx max at 2048-32
		if(Current_sample_indx == 2048)//2048-32
		{
			//countttt++;
			for (int i = 0;i < 256;i++)
			{
				if(abs(rawwave[i + 256*7]) >= 15.0)
				{
					if(rawwave[i + 256*7]>0)
						Buf_1sec[i] = RawThreshValueBuf;
					else
						Buf_1sec[i] = -1 * RawThreshValueBuf;
				}		
				else 
					Buf_1sec[i] = rawwave[i + 256*7];
			}

			for (int i = 0;i < 256;i++)
			{
				if(abs(rawwave2[i + 256*7]) >= 15.0)
				{
					if(rawwave2[i + 256*7]>0)
						Buf_1sec_2[i] = RawThreshValueBuf;
					else
						Buf_1sec_2[i] = -1 * RawThreshValueBuf;
				}		
				else 
					Buf_1sec_2[i] = rawwave2[i + 256*7];
			}
		}
		//filterController_.IIRFilterProcessing(array_buf_in,array_buf_out,2304);
		//filterController_.IIRFilterProcessing(array_buf_in_2,array_buf_out_2,2304);
		for (int i = 0;i < 2048;i++)
		{
			Y_Array_Display[i] = array_buf_out[256+i];
			Y_Array_Display_2[i] = array_buf_out_2[256+i];
		}
#pragma endregion FILTER_PROCESSING1
		if(isRecoding)
		{
			//Recoded_Data_Change_flag = true;//lock
		
			for (int i = Current_sample_indx - BUF_SIZE;i < Current_sample_indx;i++)
			{
				CountOfRecodeSamples++;
				bool isover = abs(Y_Array_Display[i])>0.5*ThreshValue?true:false;
				bool isover_ = abs(Y_Array_Display_2[i])>0.5*ThreshValue?true:false;

				Recoded_Array.push_back(Y_Array_Display[i]);
				Recoded_Array_IsOver.push_back(isover);

				Recoded_Array_2.push_back(Y_Array_Display_2[i]);
				Recoded_Array_IsOver_2.push_back(isover_);

				if(isover)
					CountofOverSample++;
				IsOverFlowed[i] = isover;
				//Total_amp += abs(Y_Array_Display[i]);
			}
			
			//Recoded_Data_Change_flag = false;//unlock
		}
#pragma region NEW_METHOD_OF_PROCESS_WAVE_OVERFLOW
		int pluscount = 0,minuscount = 0,fisrt_plusconut =0;
		int step_ = 0;
		bool plusflag = false;
		bool minusflag = false;
		bool IsFinish = false;
		if(Current_sample_indx >= 2.0*BUF_SIZE)
		for (int i = Current_sample_indx-2.0*BUF_SIZE;i< Current_sample_indx;i++)
		{
			if (!IsFinish)
			{
				//if (abs(Y_Array_Display[i])>0.5*ThreshValue)
				if(IsOverFlowed[i])
				{
					if(!plusflag)step_++;
					if(step_==3)
					{
						IsFinish = true;
						i--;
					}
					plusflag = true;
					minusflag = false;
					pluscount++;
					//IsOverFlowed[i] = true;
					//if(!IsOverFlowed[i])
					//{
					//	int derta = Current_sample_indx - i;
					//	
					//	if(CountOfRecodeSamples >= derta && CountOfRecodeSamples>0)
					//	{
					//		if(!Recoded_Array_IsOver[CountOfRecodeSamples - derta])
					//		{
					//			Recoded_Array_IsOver[CountOfRecodeSamples - derta] = true;//the new Is Over Flowed result
					//			CountofOverSample++;
					//			IsOverFlowed[i]= true;
					//		}
					//	}
					//}
				}
				else
				{
					if(step_ != 0)
					{
						if(!minusflag) step_++;
						if((double)minuscount > 128.0/(double)start_hz)
						{
							IsFinish = true;
							i--;
						}
						minusflag = true;
						plusflag = false;
						minuscount++;
					}
				}
			}
			else
			{
				/*if(step_ == 2)
				{
					for(int j = i - pluscount - minuscount;j < i - minuscount;j++)
					{
						IsOverFlowed[j] = false;		
					}
				}*/
				if(step_ == 3)
				{
					//if((pluscount > OffsetOverThresh * minuscount) && plusflag&&i>=minuscount)
					if(pluscount > 0)
					{
						//for(int j  = i - (minuscount+pluscount);j <= i;j++)
						for(int j = i - minuscount;j < i ;j++)
						{
							if(!IsOverFlowed[j])//&&(j+BUF_SIZE > Current_sample_indx))
							{
								IsOverFlowed[j]= true;
								if(isRecoding && (CountOfRecodeSamples >= Current_sample_indx - j))
								{
									int derta = Current_sample_indx - j;
									if(!Recoded_Array_IsOver[CountOfRecodeSamples - derta])
									{
										Recoded_Array_IsOver[CountOfRecodeSamples - derta] = true;//the new Is Over Flowed result
										CountofOverSample++;
									}
								}
							}
						}
					}
				}
				step_ = 0;
				minuscount = 0;
				pluscount = 0;
				IsFinish = false;
				plusflag = false;
				minusflag = false;
			}
		}

#pragma endregion NEW_METHOD_OF_PROCESS_WAVE_OVERFLOW
#pragma region For_Avg_amp_OverVpercen_8secOverP
		if(isRecoding)
		{
			/////////////new Avg_amp////////////
			//if(rawwave->Recoded_Array.size() >= 2048)
			{
				//int start_hz = FirDataSet.lf;
				//int end_hz = FirDataSet.hf;
				//int start_hz = 128.0*(IirDataSet.OmegaC - 0.5*IirDataSet.BandWidth);
		//		if(start_hz == 22&&rawwave->Recoded_Array.size() >= 2600)
		//			start_hz=start_hz;
				//end_hz = 128.0*(IirDataSet.OmegaC + 0.5*IirDataSet.BandWidth);
				double sum_as = 0;
				double max_as = 0;
				//double min_as = 10000;
				int count_unzero =0;
				for (int i = start_hz;i < end_hz;i++)
				{
					
					if(abs(FFT_AS[i]) > 0.001)
					{
						sum_as += abs(FFT_AS[i]);//*scal_;//*scal_v;//0.4-0.5?
						count_unzero++;
					}
					if(max_as < abs(FFT_AS[i]))
					{
						max_as = abs(FFT_AS[i]);
					}
					/*if(min_as > abs(FFT_AS[i]))
					{
						min_as = abs(FFT_AS[i]);
					}*/
					amphz[i-start_hz]= FFT_AS[i];
				}

				//Avg_amp_8sec = sum_as/(end_hz - start_hz+1);
				//Avg_amp_8sec = scal_v*sum_as/count_unzero;
				//Avg_amp_8sec = sum_as/count_unzero;
				//Avg_amp_8sec = sum_as/((end_hz - start_hz)*8+1);
				//Avg_amp_8sec = sum_as/((end_hz - start_hz)*8);
				Avg_amp_8sec = sum_as/8;

				Avg_amp_8sec_Arry.push_back(Avg_amp_8sec);
				if(Avg_amp_8sec_Arry.size() >= 64)//8*8  
				{
					if(Avg_amp_8sec < MinAMP)
						MinAMP = Avg_amp_8sec;
					if(Avg_amp_8sec > MaxAMP)
						MaxAMP = Avg_amp_8sec;
				}
			
				//////////////////////////////Std dev
				//double sum_sd = 0;
				//for (int i = start_hz;i < end_hz;i++)
				//{
				//	sum_sd += (FFT_AS[i] - Avg_amp_8sec)*(FFT_AS[i] - Avg_amp_8sec);
				//}
				//sum_sd = sqrt(sum_sd /(double)(end_hz - start_hz));
				Total_AVG_AMP += Avg_amp_8sec;
				//////////////////////////////Std dev

				//if(Avg_amp_8sec > 2.0*RawThreshValueBuf)//NO HALF AMP.SO...*2.0
				//	Avg_amp_8sec = 2.0*RawThreshValueBuf;

				Max_amp_8sec = max_as;
				//if(CountOfRecodeSamples>=2048)
					Total_amp += abs(Avg_amp_8sec);
			}
			//if(Recoded_Array.size() > 3000)
			//	Max_amp_8sec = Max_amp_8sec;
			/////////////new Avg_amp////////////
			
			if(CountOfRecodeSamples > 0)
			{
				int countofupdate = int(CountOfRecodeSamples/32);
				Avg_amp = Total_amp/countofupdate;
				//SDAMP = Total_AVG_AMP/countofupdate;
				if(!IsRawWave)
				{
					if(IsInhibit)
						OverVpercent = (double)CountofOverSample/CountOfRecodeSamples;
					else
						OverVpercent = 1.0-(double)CountofOverSample/CountOfRecodeSamples;
				}
				//rawwave OverVpercent is computed in GetRawDataFromUSB
			

				if(CountOfRecodeSamples >= 2048)
				{
					int count_ = 0;
					int count_32 = 0;
					//double sum = 0;
					for (int i = CountOfRecodeSamples -2048;i<CountOfRecodeSamples;i++)
					//for (unsigned int i = 0;i < 2048;i++)
					{
						//sum += abs(Recoded_Array[i].vol);
						if(Recoded_Array_IsOver[i])
							count_++;
						if(i >= CountOfRecodeSamples - 96 
							&& i < CountOfRecodeSamples - 64)
							/*if(i >= CountOfRecodeSamples - 64 
								&& i < CountOfRecodeSamples - 32)*/
						{
							if(Recoded_Array_IsOver[i])
								count_32++;
						}
					}
					OverVpercent_8sec = (double)count_/2048;
					//Avg_amp_8sec = sum/2048;
					//if(count_32 > 32)
					//	count_32 = 32;
					T_SCount += count_32;
					T_success = (double)T_SCount/CountOfRecodeSamples;
				}
				else
				{
					//double sum = 0;
					int count_ = 0;
					for (int i = 0;i<CountOfRecodeSamples;i++)
					{
						//sum += abs(Recoded_Array[i].vol);
						if(Recoded_Array_IsOver[i])
							count_++;
					}
					OverVpercent_8sec = (double)count_/CountOfRecodeSamples;
					//Avg_amp_8sec = sum/len_2;
					T_SCount = count_;
					T_success = (double)T_SCount/CountOfRecodeSamples;
				}
			}
			/*if(IsInhibit)
				IsInhibit = IsInhibit;*/
			if(IsInhibit)
			{
				if(abs(OverVpercent_8sec*100 -(100.0 - TargetOverPercent)) > offset_Target)
					FlagNeedNewThreshold = true;
				else
					FlagNeedNewThreshold = false;
			}
			else
			{
				if(abs(OverVpercent_8sec*100 - TargetOverPercent)> offset_Target)
					FlagNeedNewThreshold = true;
				else
					FlagNeedNewThreshold = false;
			}
		
		}
#pragma endregion For_Avg_amp_OverVpercen_8secOverP
	}
	/*else
	{
		AfxMessageBox(L"EEGWAVE_Update_Err!!!!!!!!!", MB_OK);
	}*/
}
void EEGWave::ComputeWaveAMPStdDev()
{
	int countofupdate = Avg_amp_8sec_Arry.size();
	double AVG_ =  Total_AVG_AMP/countofupdate; 
	double sum_b = 0.0;
	for (int i = 0;i< countofupdate;i++)
	{
		sum_b += (Avg_amp_8sec_Arry[i] - AVG_)*(Avg_amp_8sec_Arry[i] - AVG_);
	}
	sum_b /= countofupdate;
	SDAMP = sqrt(sum_b);
}
bool EEGWave::GetRawDataFromUSB(bool isRecoding)//RUNNING IN THREAD
{
	//if((int)Recoded_Array.size() < Recode_size_Capacity )
	{
	//	if( !IsLockedUSBGetting)
		{
			if(Current_sample_indx <= 2048)
			{
				IsLockedUSBGetting = true;
				if(Current_sample_indx == 2048)
				{
					for (int i = 1536,k = 0;i < 2048;k++,i++)
					{
						Buf_2sec[k] = Y_Array_Display[i];
					}
					TurnPage();
				}


				int csi = Current_sample_indx + BUF_SIZE;//스레드 밖에도 indx공유하고있기때문.머저 ...다음...
				
				
					Y_Array_Display[Current_sample_indx] = serialPort.GetChannel1Data();
					qDebug() << "EEGWave : " << Y_Array_Display[Current_sample_indx];
				

				//if(!serialPort.GetChannel1Data())
				//{
				//	//AfxMessageBox(L"EEGWAVE ReadFromComm_NeuroFlex_2 ERROR", MB_OK);
				//	return false;
				//}
				//serialPort.ReadFromComm_NeuroFlex_2(Y_Array_Display,BUF_SIZE,csi);
				Current_sample_indx += BUF_SIZE;

				//for (int i = Current_sample_indx - BUF_SIZE;i < Current_sample_indx;i++)
				//	Y_Array_Display[i] = Y_Array_SM_WAVE[i];

				if(Buf_2sec[510] == 0 && Buf_2sec[511] == 0)
				{
					for (int i = Current_sample_indx - BUF_SIZE;i < Current_sample_indx;i++)
						Buf_2sec[i] = Y_Array_Display[i];
				}
				/*else
				{
					double Buf_2sec_[544];
					for(int i = 0;i < 512-32; i++)
						Buf_2sec_[i] = Buf_2sec[32 + i];
					for(int i = 0;i < 32; i++)
						Buf_2sec_[512 - 32 + i] = Y_Array_Display[Current_sample_indx - 32 + i];
					for(int i = 0;i < 512; i++)
						Buf_2sec[i] = Buf_2sec_[i];
				}*/

				
				int kk = 0;
				for (int i = Current_sample_indx - BUF_SIZE;i < Current_sample_indx;i++,kk++)
				{
					//just for raw data
					bool isover = false;
					//Y_Array_Display[i] *= 1.8;
					if (abs(Y_Array_Display[i]) > 0.5*ThreshValue)
					{
						IsOverFlowed[i] = true;
						isover = true;
					}
				/*	if (abs(Y_Array_Display_2[i]) > 0.5*ThreshValue)// proccess later
					{
						IsOverFlowed[i] = true;
						isover = true;
					}*/
					//Y_Array_Display[i] = Y_Array_SM_WAVE[i];
					/*if((Current_sample_indx/32)%2==0)
							Y_Array_Display[i] = 20.0*sin(PI*kk/BUF_SIZE);
						else
							Y_Array_Display[i] = -20.0*sin(PI*kk/BUF_SIZE);*/
					if(isRecoding)
					{
						/*if((Current_sample_indx/32)%2==0)
							Y_Array_Display[i] = 20.0*sin(PI*kk/BUF_SIZE);
						else
							Y_Array_Display[i] = -20.0*sin(PI*kk/BUF_SIZE);*/

						//int r_size = Recoded_Array.size();
						//if((r_size/2048)%2 == 0)
							//Y_Array_Display[i] = Y_Array_SM_WAVE[i];
						//else
						//	Y_Array_Display[i] = Y_Array_SM_WAVE_[i];

						Total_amp += abs(Y_Array_Display[i]);
						Recoded_Array.push_back(Y_Array_Display[i]);

						if(isover)
						{
							CountofOverSample++;
						}
						Recoded_Array_IsOver.push_back(IsOverFlowed[i]);
					}
				}
				double recode_size = Recoded_Array.size();
				if( recode_size != 0)
				{
					Avg_amp = Total_amp/recode_size;
					//Avg_amp /= 0.707;
					//Avg_amp *= 1.414;
					//Avg_amp *= 1.623;
					Avg_amp *= 1.5748;
					Avg_amp *= 2.0;
					OverVpercent = (double)CountofOverSample/recode_size;
				}

				IsLockedUSBGetting = false;
			}
			
		}
		return true;
	}
}
bool EEGWave::GetRawDataFromUSB_2(bool isRecoding)//RUNNING IN THREAD
{
	if((int)Recoded_Array.size() < Recode_size_Capacity )
	{
		if(!IsLockedUSBGetting)
		{
			if(Current_sample_indx <= 2048)
			{
				IsLockedUSBGetting = true;
				if(Current_sample_indx == 2048)
				{
					for (int i = 1536,k = 0;i < 2048;k++,i++)
					{
						Buf_2sec[k] = Y_Array_Display[i];
					}
					TurnPage();
				}
					

				int csi = Current_sample_indx + BUF_SIZE;//스레드 밖에도 indx공유하고있기때문.머저 ...다음...


				for (int i = csi - BUF_SIZE; i < csi; i++)
				{
					Y_Array_Display[i] = serialPort.GetChannel1Data();
					Y_Array_Display_2[i] = serialPort.GetChannel2Data();
				}

				//if(!serialPort.ReadFromNeuronFlex2(Y_Array_Display,Y_Array_Display_2,BUF_SIZE,csi))
				//{
				//	//AfxMessageBox(L"EEGWAVE ReadFromComm_NeuroFlex_3 ERROR", MB_OK);
				//	return false;
				//}
				//serialPort.ReadFromComm_NeuroFlex_2(Y_Array_Display,BUF_SIZE,csi);
				Current_sample_indx += BUF_SIZE;
			
				for (int i = Current_sample_indx - BUF_SIZE;i < Current_sample_indx;i++)
				{
					//just for raw data
					bool isover = false;
					Y_Array_Display[i] *= 2.0;
					Y_Array_Display_2[i] *= 2.0;
					if (abs(Y_Array_Display[i]) > 0.5*ThreshValue)
					{
						IsOverFlowed[i] = true;
						isover = true;
					}
				/*	if (abs(Y_Array_Display_2[i]) > 0.5*ThreshValue)// proccess later
					{
						IsOverFlowed[i] = true;
						isover = true;
					}*/
					if(isRecoding)
					{
						Total_amp += abs(Y_Array_Display[i]);
						Recoded_Array.push_back(Y_Array_Display[i]);

						Recoded_Array_2.push_back(Y_Array_Display_2[i]);

						if(isover)
						{
							CountofOverSample++;
						}
						Recoded_Array_IsOver.push_back(IsOverFlowed[i]);
					}
				}
				double recode_size = Recoded_Array.size();
				if( recode_size != 0)
				{
					Avg_amp = Total_amp/recode_size;
					OverVpercent = (double)CountofOverSample/recode_size;
				}
			}
			
		}
		return true;
	}
	else
	{
		//AfxMessageBox(L"EEGWAVE capacity overflow resize...error", MB_OK);
		return false;//capacity overflow resize...error...
	}
		
	
}
void EEGWave::TurnPage()
{
	for (int i = 0;i< 2048;i++)
	{
		IsOverFlowed[i] = false;//data "amp" >thresh value ? true,false;
		Y_Array_Display[i] = 0;
		Y_Array_Display_2[i] = 0;
	}
	Current_sample_indx = 0;
}
bool EEGWave::OpenUsbComm()
{
	if(!serialPort.InitThread()) return false;
	return true;
}
void EEGWave::CloseUsbComm()
{
	serialPort.exit();
}
void EEGWave::start()
{
	serialPort.SetBoolPause(false);
	serialPort.SetBoolStop(false);
	serialPort.start();
}

void EEGWave::pause()
{
	serialPort.Pause();
}

void EEGWave::resume()
{
	serialPort.Resume();
}

void EEGWave::stop()
{
	serialPort.Stop(); 
}


void EEGWave::DrawString(const char* str)
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
void EEGWave::HalfAuto()
{
	if(!IsRawWave&&IsRecoding_notrawdata&&Recoded_Array.size()>2048)
	{
		if(IsInhibit)
		{
			bool isHigh = false;
			//if(IirDataSet.OmegaC >(15.0/128.0))isHigh = true;
			if(name[0] =='H')isHigh = true;
			//if(ThreshValue < Absolute_Thresh)//can change
			//{
				if(!isHigh)
				{
	
					double derta = abs(OverVpercent_8sec - (1.0-(double)TargetOverPercent/100));
					if(derta >= 0.3)
					{
						//if(OverVpercent_8sec > (1.0-(double)TargetOverPercent/100))
						//	ThreshValue = 2.0*Max_amp_8sec*(double)TargetOverPercent/100;
						//else
							ThreshValue = 2.0*Max_amp_8sec*(double)TargetOverPercent/100;//half
					}
					else
						ThreshValue = 2.0 * Max_amp_8sec*((double)TargetOverPercent/100)*((double)TargetOverPercent/100);
					
				/*	ThreshValue = 2.0*Avg_amp_8sec*(double)TargetOverPercent/100*(double)TargetOverPercent/100;
					if(ThreshValue > Absolute_Thresh)
						ThreshValue = Absolute_Thresh;*/
				}
				else//high
				{
					//ThreshValue = 2.0*Avg_amp_8sec*(double)TargetOverPercent/100*(double)TargetOverPercent/100;
					
					double derta = abs(OverVpercent_8sec - (1.0-(double)TargetOverPercent/100));
					if(derta >= 0.3)
					{
						//if(OverVpercent_8sec > (1.0-(double)TargetOverPercent/100))
						//	ThreshValue = 2.0*Max_amp_8sec*(double)TargetOverPercent/100;
						//else
						ThreshValue = 2.0*Max_amp_8sec*(double)TargetOverPercent/100;//half
					}
					else
						ThreshValue = 2.0 * Max_amp_8sec*((double)TargetOverPercent/100)*((double)TargetOverPercent/100);
					if(ThreshValue >Absolute_Thresh)
						ThreshValue = Absolute_Thresh;
				}
			//}
			if(ThreshValue > Absolute_Thresh)
				ThreshValue = Absolute_Thresh;
			if(ThreshValue < 0.01)ThreshValue = 0.01;
		}
		else//Reward
		{
			//double kk = (399 - TargetOverPercent)*0.01;//225
			//ThreshValue = kk*Avg_amp_8sec*(double)(100-TargetOverPercent)/100;
			//ThreshValue = Avg_amp_8sec*(1.2-(double)TargetOverPercent/100);//* ;
			//ThreshValue = Avg_amp_8sec*(1.85 - (double)TargetOverPercent/100);//* ;
			//ThreshValue = Avg_amp_8sec*((double)TargetOverPercent/100);//* ;
			//double derta = abs(OverVpercent_8sec - (double)TargetOverPercent/100);
			//if(derta > 0.2)
			//{
				//if(OverVpercent_8sec > (double)TargetOverPercent/100)
				//	ThreshValue = 2.0 * Max_amp_8sec*(double)TargetOverPercent/100;
				//else
					//ThreshValue = Avg_amp_8sec*0.85;//half
			//		ThreshValue = 2.3*Avg_amp_8sec*(1.0-(double)TargetOverPercent/100);
			//}
			//else
				ThreshValue = 2.3*Avg_amp_8sec*(1.0-(double)TargetOverPercent/100);
				

			if(ThreshValue > Absolute_Thresh)
				ThreshValue = Absolute_Thresh;
			
			//if(ThreshValue > 2.0*Absolute_Thresh)
			//	ThreshValue = 2.0*Absolute_Thresh;
			if(ThreshValue < 0.01)
				ThreshValue = 0.01;
		}
		//ThreshValue_display = 2*ThreshValue*scal_FIR;
		//ReProcess8secData();
	}

}
double EEGWave::MaxAMPInRange(int s_,int range)
{
	int max_ = 0;
	for(int i = s_ ;i < s_+range;i++)
	{
		if(abs(Y_Array_Display[i])>max_)
			max_ = Y_Array_Display[i];
	}
	return max_;
}
void EEGWave::ReProcess8secData()
{
//	bool IsOverFlowed_[2048];
//	int jjj = 0;
//	for(int i = 0;i< 2048;i++)
//	{	
//		Recoded_Array_IsOver[CountOfRecodeSamples - 2048 + i] = abs(Recoded_Array[CountOfRecodeSamples - 2048 + i])>0.5*ThreshValue?true:false;
//		//Recoded_Array_IsOver[CountOfRecodeSamples - 2048 + i] = recode_array_buf[i];
//
//		if(i >= 2048 - Current_sample_indx)
//			IsOverFlowed_[jjj++] = abs(Recoded_Array[CountOfRecodeSamples - 2048 + i])>0.5*ThreshValue?true:false;
//		//IsOverFlowed[jjj++] = recode_array_buf[i];
//	}
//	for (int i = Current_sample_indx-1;i < 2048;i++)
//	{
//		IsOverFlowed_[i] = false;
//	}
//#pragma region NEW_METHOD_OF_PROCESS_WAVE_OVERFLOW_from_0_to_currentindx
//	if(Current_sample_indx >= 64)
//	for (int all = 64; all < Current_sample_indx; all += 32)
//	{
//		int pluscount = 0,minuscount = 0;
//		int step_ = 0;
//		bool plusflag = false;
//		bool minusflag = false;
//		bool IsFinish = false;
//		for (int i = all-64;i< all;i++)
//		{
//			if (!IsFinish)
//			{
//				//if (Recoded_Array_IsOver[CountOfRecodeSamples - all + i])
//				if (IsOverFlowed_[i])
//				{
//					if(!plusflag)step_++;
//					if(step_==3)
//					{
//						IsFinish = true;
//						i--;
//					}
//					plusflag = true;
//					minusflag = false;
//					pluscount++;
//				}
//				else
//				{
//					if(step_ != 0)
//					{
//						if(!minusflag) step_++;
//						if((double)minuscount > 128.0/(double)start_hz)
//						{
//							IsFinish = true;
//							i--;
//						}
//						minusflag = true;
//						plusflag = false;
//						minuscount++;
//					}
//				}
//			}
//			else
//			{
//				if(pluscount > 0&&step_==3)
//				{
//					for(int j = i - minuscount;j < i;j++)
//					{
//						if(!IsOverFlowed_[j])
//						{
//							IsOverFlowed_[j]= true;
//							//int derta = Current_sample_indx - i;
//							//if(CountOfRecodeSamples >= derta)
//							//{
//							//	if(!Recoded_Array_IsOver[CountOfRecodeSamples - derta])
//							//	{
//							//		Recoded_Array_IsOver[CountOfRecodeSamples - derta] = true;//the new Is Over Flowed result		
//							//	}
//							//}
//						}
//
//					}
//				}
//				//if(first_pulus==0)first_pulus =i;
//				step_ = 0;
//				minuscount = 0;
//				pluscount = 0;
//				IsFinish = false;
//				plusflag = false;
//				minusflag = false;
//			}
//		}
//	}
	
//#pragma endregion NEW_METHOD_OF_PROCESS_WAVE_OVERFLOW_from_0_to_currentindx

	//
//#pragma region NEW_METHOD_OF_PROCESS_WAVE_OVERFLOW_RecodeArry

	//int sss = CountOfRecodeSamples - 2048;
	//int eee = CountOfRecodeSamples - 2048 + abs(Current_sample_indx - 2048);
	//if(sss >= 64)
	//for (int all = sss; all < eee; all += 32)
	//{
	//	int pluscount = 0;
	//	int minuscount = 0;
	//	int step_ = 0;
	//	bool plusflag = false;
	//	bool minusflag = false;
	//	bool IsFinish = false;
	//	for (int i = all- 64;i < all;i ++)
	//	{
	//		if (!IsFinish)
	//		{
	//			if (Recoded_Array_IsOver[i])
	//			{
	//				if(!plusflag)step_++;
	//				if(step_==3)
	//				{
	//					IsFinish = true;
	//					i--;
	//				}
	//				plusflag = true;
	//				minusflag = false;
	//				pluscount++;

	//			}
	//			else
	//			{
	//				if(step_ != 0)
	//				{
	//					if(!minusflag) step_++;
	//					if((double)minuscount > 128.0/(double)start_hz)
	//					{
	//						IsFinish = true;
	//						i--;
	//					}
	//					minusflag = true;
	//					plusflag = false;
	//					minuscount++;
	//				}
	//			}
	//		}
	//		else
	//		{
	//			if(pluscount > 0 && step_==3)
	//			{
	//				for(int j = i - minuscount;j < i;j++)
	//				{
	//					if(!Recoded_Array_IsOver[j])
	//					{
	//						Recoded_Array_IsOver[j] = true;//the new Is Over Flowed result
	//					}
	//				}
	//			}
	//			step_ = 0;
	//			minuscount = 0;
	//			pluscount = 0;
	//			IsFinish = false;
	//			plusflag = false;
	//			minusflag = false;
	//		}
	//	}
	//}
	
//#pragma endregion NEW_METHOD_OF_PROCESS_WAVE_OVERFLOW_RecodeArry

	
	//if(CountOfRecodeSamples >= 2048)
	//	AMPThresholdIsOverfByPeakMethod(end_hz,2048);
	int ctt_=0;
	//Recoded_Data_Change_flag2 = true;
	int ss_= 2047 - Current_sample_indx;
	for(int i = 0;i< 2048;i++)
	{	
		if(Recoded_Array_IsOver[CountOfRecodeSamples - 2048 + i])
		{
			if(i >= ss_)
			{
				int idx = i - ss_;
				if(idx >= 0 && idx <=2047)
				IsOverFlowed[idx]= true;
			}
				
			ctt_++;
		}
		else 
		{
			if(i >= ss_)
			{
				int idx = i - ss_;
				if(idx >= 0 && idx <=2047)
					IsOverFlowed[idx]= false;
			}
		}
			
	}

	OverVpercent_8sec = (double)ctt_/2048;

	//Recoded_Data_Change_flag2 = false;
}

float EEGWave::ReProcess8secData_1(double ThreshValue_)//for min value method
{
	if(ThreshValue_ >= 0.1
		//ThreshValue_ >= 2.0 * Avg_amp_8sec * TargetOverPercent/100.0
		//&&ThreshValue_ <= 2.0*Avg_amp_8sec * TargetOverPercent/100.0*2.0)
		&&ThreshValue_ <= 2.0*Max_amp_8sec)
	{
		//recoded size >= 2048////////////////
		//int Current_sample_indx_ = Current_sample_indx;
		for (int i = 0;i< 2048;i++)
		{
			recode_array_buf[i] = Recoded_Array[CountOfRecodeSamples - 2048+i]>0.5*ThreshValue_?true:false;
		}
#pragma region NEW_METHOD_OF_PROCESS_WAVE_OVERFLOW
		
		for (int all = 64; all < 2048;all += 32)
		{
			int pluscount = 0,minuscount = 0;
			int step_ = 0;
			bool plusflag = false;
			bool minusflag = false;
			bool IsFinish = false;
		//	int first_pluscount = 0;
			for(int i = all - 64;i< all;i++)
			{
				if (!IsFinish)
				{
					if (recode_array_buf[i])
					{
						if(!plusflag)step_++;
						if(step_==3)
						{
							IsFinish = true;
							i--;
						}
						plusflag = true;
						minusflag = false;
						pluscount++;
					}
					else
					{
						if(step_ != 0)
						{
							if(!minusflag) step_++;		
							if((double)minuscount > 128.0/(double)start_hz)
							{
								IsFinish = true;
								i--;
							}
							minusflag = true;
							plusflag = false;
							minuscount++;
						}
					}
				}
				else
				{
					if((step_==3)&&(pluscount > 0))				
					{
						for(int j = i - minuscount;j < i;j++)
						{
							if(!recode_array_buf[j])
							{
								recode_array_buf[j]= true;	
							}		
						}
					}

					step_ = 0;
					minuscount = 0;
					pluscount = 0;
					IsFinish = false;
					plusflag = false;
					minusflag = false;
				}
			}//for64
		}//for2048
		
		
	
#pragma endregion NEW_METHOD_OF_PROCESS_WAVE_OVERFLOW
#pragma region For_Avg_amp_OverVpercen_8secOverP

		int count_ = 0;
		for (int i = 0;i< 2048;i++)
		{
			if(recode_array_buf[i])
				count_++;
		}
	/*	count_*=1.2;
		if(count_>2048)count_=2048;*/
		return (double)count_/2048;

#pragma endregion For_Avg_amp_OverVpercen_8secOverP
	}
	else
	{
		return 2.0;
	}
}	
float EEGWave::ReProcess8secData_2(double ThreshValue_)//for min value method
{
	if(//ThreshValue_ >= 2.0 * Avg_amp_8sec * (1.0 - TargetOverPercent/100.0)*0.1
		ThreshValue_ >= 0.1
		&&ThreshValue_ <= 2.0*Avg_amp_8sec)// * (1.0 - TargetOverPercent/100.0)*2)
		//&&ThreshValue_ <= Absolute_Thresh)
	{
		//recoded size >= 2048////////////////
		int Current_sample_indx_ = Current_sample_indx;
		//bool recode_array_buf[2048];


		for (int i = 0;i< 2048;i++)
		{
			recode_array_buf[i] = Recoded_Array[CountOfRecodeSamples - 2048+i]>0.5*ThreshValue_?true:false;
		}
#pragma region NEW_METHOD_OF_PROCESS_WAVE_OVERFLOW

		for (int all = 64;all < 2048;all += 32)
		{
			int pluscount = 0,minuscount = 0;
			int step_ = 0;
			bool plusflag = false;
			bool minusflag = false;
			bool IsFinish = false;
			//	int first_pluscount = 0;
			for(int i = all - 64;i< all;i++)
			{
				if (!IsFinish)
				{
					if (recode_array_buf[i])
					{
						if(!plusflag)step_++;
						if(step_==3)
						{
							IsFinish = true;
							i--;
						}
						plusflag = true;
						minusflag = false;
						pluscount++;
					}
					else
					{
						if(step_ != 0)
						{
							if(!minusflag) step_++;		
							if((double)minuscount > 128.0/(double)start_hz)
							{
								IsFinish = true;
								i--;
							}
							minusflag = true;
							plusflag = false;
							minuscount++;
						}
					}
				}
				else
				{
					if((step_==3)&&(pluscount > 0))				
					{
						for(int j = i - minuscount;j < i;j++)
						{
							if(!recode_array_buf[j])
							{
								recode_array_buf[j]= true;	
							}		
						}
					}

					step_ = 0;
					minuscount = 0;
					pluscount = 0;
					IsFinish = false;
					plusflag = false;
					minusflag = false;
				}
			}//for64
		}//for2048



#pragma endregion NEW_METHOD_OF_PROCESS_WAVE_OVERFLOW
#pragma region For_Avg_amp_OverVpercen_8secOverP

		int count_ = 0;
		for (int i = 0;i< 2048;i++)
		{
			if(recode_array_buf[i])
				count_++;
		}
		return (double)count_/2048;

#pragma endregion For_Avg_amp_OverVpercen_8secOverP
	}
	else
	{
		return 2.0;
	}
	//return 1;
}	
float EEGWave::ReProcess8secData_3(double ThreshValue_)//for min value method
{
	//if(ThreshValue_ >= 2.0 * Avg_amp_8sec * 2.5
	//	&&ThreshValue_ <= 2.0*Avg_amp_8sec * TargetOverPercent/100.0*4.0)
		//&&ThreshValue_ <= Absolute_Thresh)
	if(ThreshValue_ >= 0.1 && ThreshValue_ <= 3.0*Max_amp_8sec)
	{
		//recoded size >= 2048////////////////
		int Current_sample_indx_ = Current_sample_indx;
		//bool recode_array_buf[2048];

		for (int i = 0;i< 2048;i++)
		{
			recode_array_buf[i] = Recoded_Array[CountOfRecodeSamples - 2048+i]>0.5*ThreshValue_?true:false;
		}
#pragma region NEW_METHOD_OF_PROCESS_WAVE_OVERFLOW
		int pluscount = 0,minuscount = 0;
		int step_ = 0;
		bool plusflag = false;
		bool minusflag = false;
		bool IsFinish = false;
		int first_pluscount = 0;
		for (int i = 0;i < 2048-32;i++)
		{
			if (!IsFinish)
			{
				if (recode_array_buf[i])
				{
					if(!plusflag)step_++;
					if(step_==3)
					{
						IsFinish = true;
						i--;
					}
					plusflag = true;
					minusflag = false;
					pluscount++;
				}
				else
				{
					if(step_ != 0)
					{
						if(!minusflag) step_++;		
						if((double)minuscount > 128.0/(double)start_hz)
						{
							IsFinish = true;
							i--;
						}
						minusflag = true;
						plusflag = false;
						minuscount++;
					}
				}
			}
			else
			{
				if((step_==3)&&(pluscount > 0))				
				{
					for(int j = i - minuscount;j < i;j++)
					{
						if(!recode_array_buf[j])
						{
							recode_array_buf[j]= true;	
						}		
					}
				}
				if(first_pluscount == 0)
					first_pluscount = i;
				step_ = 0;
				minuscount = 0;
				pluscount = 0;
				IsFinish = false;
				plusflag = false;
				minusflag = false;
			}
		}
		pluscount = 0,minuscount = 0;
		step_ = 0;
		plusflag = false;
		minusflag = false;
		IsFinish = false;
		for (int i = first_pluscount;i < 2048;i++)
		{
			if (!IsFinish)
			{
				if (abs(Recoded_Array[CountOfRecodeSamples - 2048+i])>0.5*ThreshValue_)
				{
					if(!plusflag)step_++;
					if(step_==3)
					{
						IsFinish = true;
						i--;
					}
					plusflag = true;
					minusflag = false;
					pluscount++;
				}
				else
				{
					if(step_ != 0)
					{
						if(!minusflag) step_++;		
						if((double)minuscount > 128.0/(double)start_hz)
						{
							IsFinish = true;
							i--;
						}
						minusflag = true;
						plusflag = false;
						minuscount++;
					}
				}
			}
			else
			{
				if((step_==3)&&(pluscount > 0))				
				{
					for(int j = i - minuscount;j < i;j++)
					{
						if(!recode_array_buf[j])
						{
							recode_array_buf[j]= true;	
						}		
					}
				}
				step_ = 0;
				minuscount = 0;
				pluscount = 0;
				IsFinish = false;
				plusflag = false;
				minusflag = false;
			}
		}
#pragma endregion NEW_METHOD_OF_PROCESS_WAVE_OVERFLOW
#pragma region For_Avg_amp_OverVpercen_8secOverP

		int count_ = 0;
		for (int i = 0;i< 2048;i++)
		{
			if(recode_array_buf[i])
				count_++;
		}
		return (double)count_/2048;

#pragma endregion For_Avg_amp_OverVpercen_8secOverP
	}
	else
	{
		return 2.0;
	}
}	
//float OverPercentErroFunc1(float x_[])
//{
//	//float OverVpercen_8sec_ = EEG_CONTROL_arry[1]->ReProcess8secData_1(x_[1]);
//	//float err = abs(OverVpercen_8sec_ - (1.0 - EEG_CONTROL_arry[1]->TargetOverPercent/100.0));
//	//return err;
//}
//float OverPercentErroFunc2(float x_[])
//{
//	//float OverVpercen_8sec_ = EEG_CONTROL_arry[2]->ReProcess8secData_2(x_[1]);
//	//float err = abs(OverVpercen_8sec_ - (EEG_CONTROL_arry[2]->TargetOverPercent/100.0));
//	//return err;
//}
//float OverPercentErroFunc3(float x_[])
//{
//	//float OverVpercen_8sec_ = EEG_CONTROL_arry[3]->ReProcess8secData_3(x_[1]);
//	//float err = abs(OverVpercen_8sec_ - (1.0 - EEG_CONTROL_arry[3]->TargetOverPercent/100.0));
//	//return err;
//}

bool EEGWave::NewHalfAuto1()
{
#pragma region M3
	//IsThresholdComputeEndding = false;
	//int         emi_peaks[56];//if only 7hz , MAX PEAK = 2048 * 7hz / 256 = 56 
	//int         absorp_peaks[56];
	//int         emi_count = 0;
	//int         absorp_count = 0;
	//double DataForPeakDetec[2048];
	//double      delta = 1e-6;
	//int         emission_first = 0;

	//for (int i = 0;i< 2048;i++)
	//{
	//	DataForPeakDetec[i] = Recoded_Array[CountOfRecodeSamples - 2048+i];
	//}
	//detect_peak(DataForPeakDetec, 2048,
	//	emi_peaks, &emi_count, 56,
	//	absorp_peaks, &absorp_count, 56,
	//	delta, emission_first);

	//if(emi_count != 0)
	//{
	//	vector<double> emi_peaks_AMP;//not index, means AMP
	//	vector<double> emi_peaks_AMP_;
	//	//double sum_ = 0;
	//	for (int i = 0;i < emi_count;i++)
	//	{
	//		emi_peaks_AMP.push_back(DataForPeakDetec[emi_peaks[i]]);
	//		emi_peaks_AMP_.push_back(DataForPeakDetec[emi_peaks[i]]);
	//		//sum_ += abs(DataForPeakDetec[emi_peaks[i]]);
	//	}
	//	//ThreshValue_buf = (0.01*TargetOverPercent + 0.2)*2.15*sum_/emi_count;//2.15
	//	sort(emi_peaks_AMP.begin(),emi_peaks_AMP.end());
	//	if(TargetOverPercent == 0)
	//	{
	//		ThreshValue_buf = 2.0*emi_peaks_AMP[0] + 0.1;
	//	}
	//	else if(TargetOverPercent == 100)
	//	{
	//		ThreshValue_buf = 2.0*emi_peaks_AMP[emi_count - 1] - 0.1;
	//	}
	//	else
	//	{
	//		double ind_buf = (0.01*TargetOverPercent)*emi_count;
	//		if(ind_buf >= 0 && ind_buf <= 1)
	//			ind_buf = 1;
	//		int indx_ = (int)ind_buf; 
	//		if(indx_ == 0)indx_ = 1;
	//			ThreshValue_buf = emi_peaks_AMP[indx_ - 1]*2.0;
	//	}
	//	ThreshValue = ThreshValue_buf;

	//	int bandcount = (int)(2048/(double)emi_count);
	//	int halfbandcount = (int)(1024/(double)emi_count);
	//	for (int i = 0;i < emi_count -1;i++)
	//	{
	//		if(DataForPeakDetec[emi_peaks[i]] >= 0.5*ThreshValue)
	//		{
	//			//for(int j = -1*(i+1)*halfbandcount;j < (i+1)*halfbandcount;j++)
	//				for(int j = (i+1)*bandcount;j < (i+2)*bandcount;j++)
	//			{
	//				int iidx = CountOfRecodeSamples - 2048 + j;
	//				if(iidx < 0)
	//					iidx = 0;
	//				if(!Recoded_Array_IsOver[iidx])
	//				{
	//					Recoded_Array_IsOver[iidx] = true;
	//					CountofOverSample++;
	//				}
	//			}
	//		}
	//		else
	//		{
	//			//for(int j = -1*(i+1)*halfbandcount;j < (i+1)*halfbandcount;j++)
	//			//for(int j = i*bandcount;j < (i+1)*bandcount;j++)
	//				for(int j = (i+1)*bandcount;j < (i+2)*bandcount;j++)
	//			{
	//				int iidx = CountOfRecodeSamples - 2048 + j;
	//				if(iidx < 0)
	//					iidx = 0;
	//				if(Recoded_Array_IsOver[iidx])
	//				{
	//					Recoded_Array_IsOver[iidx] = false;
	//					CountofOverSample--;
	//				}
	//			}
	//		}
	//	}		
	//}
	//IsThresholdComputeEndding = true;
	//return true;
#pragma endregion M3
#pragma region M4
	//IsThresholdComputeEndding = false;
	//int emi_count = Peak_8sec_Arry.size();
	//if(emi_count != 0)
	//{
	//	vector<double> emi_peaks_AMP;//not index, means AMP
	//	vector<double> emi_peaks_AMP_;
	//	for (int i = 0;i < emi_count;i++)
	//	{
	//		emi_peaks_AMP.push_back(Peak_8sec_Arry[i]);
	//		emi_peaks_AMP_.push_back(Peak_8sec_Arry[i]);
	//	}
	//	int rec_size_ = Recoded_Array_IsOver.size();
	//	sort(emi_peaks_AMP.begin(),emi_peaks_AMP.end());
	//	if(TargetOverPercent == 0)
	//	{
	//		ThreshValue_buf = emi_peaks_AMP[0] + 0.1;
	//	}
	//	else if(TargetOverPercent == 100)
	//	{
	//		ThreshValue_buf = emi_peaks_AMP[emi_count - 1] - 0.1;
	//	}
	//	else
	//	{
	//		double ind_buf = (0.01*TargetOverPercent)*emi_count;
	//		if(ind_buf >= 0 && ind_buf <= 1)
	//			ind_buf = 1;
	//		int indx_ = (int)ind_buf; 
	//		if(indx_ == 0)indx_ = 1;
	//		if(indx_ >= emi_count)emi_count - 1;
	//		ThreshValue_buf = emi_peaks_AMP[indx_];
	//	}
	//	ThreshValue = ThreshValue_buf;

	//	int bandcount = (int)(2048/(double)emi_count);
	//	int halfbandcount = (int)(1024/(double)emi_count);
	//	int rec_size = Recoded_Array_IsOver.size();

	//	for (int i = Peak_8sec_Arry.size() - 1;i > 0;i--)
	//	{
	//		if(Peak_8sec_Arry[i] >= ThreshValue)
	//		{
	//			for(int j = Peak_8sec_IDX_Arry[i] - halfbandcount;j < Peak_8sec_IDX_Arry[i]+ halfbandcount;j++)
	//			{
	//				//int iidx = rec_size -  j;
	//				int iidx = j;
	//				if(iidx < 0)
	//					iidx = 0;
	//				if(!Recoded_Array_IsOver[iidx])
	//				{
	//					Recoded_Array_IsOver[iidx] = true;
	//					CountofOverSample++;
	//				}
	//			}

	//		}
	//		else
	//		{
	//			for(int j = Peak_8sec_IDX_Arry[i] - halfbandcount;j < Peak_8sec_IDX_Arry[i]+ halfbandcount;j++)
	//			//for(int j = Peak_8sec_IDX_Arry[i]*bandcount;j < (i+1)*bandcount;j++)
	//			{
	//				//int iidx = rec_size - j;
	//				int iidx = j;
	//				if(iidx < 0)
	//					iidx = 0;
	//				if(Recoded_Array_IsOver[iidx])
	//				{
	//					Recoded_Array_IsOver[iidx] = false;
	//					CountofOverSample--;
	//				}
	//			}
	//		}
	//	}	
	//	//KK= KK;
	//}
	//IsThresholdComputeEndding = true;
	//return true;
#pragma endregion M4
#pragma region M5
	IsThresholdComputeEndding = false;
	int emi_count = Peak_8sec_Arry.size();
	if(emi_count != 0)
	{
		vector<double> emi_peaks_AMP;//not index, means AMP
		vector<double> emi_peaks_AMP_;
		for (int i = 0;i < emi_count;i++)
		{
			emi_peaks_AMP.push_back(Peak_8sec_Arry[i]);
			emi_peaks_AMP_.push_back(Peak_8sec_Arry[i]);
		}
		int rec_size_ = Recoded_Array_IsOver.size();
		sort(emi_peaks_AMP.begin(),emi_peaks_AMP.end());
		if(TargetOverPercent == 0)
		{
			ThreshValue_buf = emi_peaks_AMP[0] + 0.1;
		}
		else if(TargetOverPercent == 100)
		{
			ThreshValue_buf = emi_peaks_AMP[emi_count - 1] - 0.1;
		}
		else
		{
			double ind_buf = (0.01*TargetOverPercent)*emi_count;
			if(ind_buf >= 0 && ind_buf <= 1)
				ind_buf = 1;
			int indx_ = (int)ind_buf; 
			if(indx_ == 0)indx_ = 1;
			if(indx_ + 2 >= emi_count)indx_ = emi_count - 3;
			ThreshValue_buf = emi_peaks_AMP[indx_ + 2];
		}
		ThreshValue = ThreshValue_buf;

		int bandcount = (int)(2048.0/(double)emi_count);

		for (int i = 0;i < emi_count;i++)
		{
			int start_idx = Peak_8sec_IDX_Arry[i] - (int)(0.25*bandcount);
			if(start_idx < 0) 
				start_idx = 0;
			int end_idx = start_idx + bandcount ;
			if(end_idx > CountOfRecodeSamples)
				end_idx = CountOfRecodeSamples;
			if(Peak_8sec_Arry[i] >= ThreshValue)
			{
				for(int j = start_idx;j < end_idx;j++)
				{
					if(!Recoded_Array_IsOver[j])
					{
						int idx_2048 = Current_sample_indx - (CountOfRecodeSamples - j);
						if(idx_2048 >= 0 && idx_2048 < 2048)
						{
							IsOverFlowed[idx_2048] = true;
						}
						Recoded_Array_IsOver[j] = true;
						
						CountofOverSample++;
					}
				}
			}
			else
			{
				for(int j = start_idx;j < end_idx;j++)
				{
					if(Recoded_Array_IsOver[j])
					{
						int idx_2048 = Current_sample_indx - (CountOfRecodeSamples - j);
						if(idx_2048 >= 0 && idx_2048 < 2048)
						{
							IsOverFlowed[idx_2048] = false;
						}
					
						Recoded_Array_IsOver[j] = false;
						CountofOverSample--;
					}
				}
			}
		}	
		
	}

	IsThresholdComputeEndding = true;
	return true;
#pragma endregion M5
}
bool EEGWave::NewHalfAuto2()
{
#pragma region M3

	//IsThresholdComputeEndding = false;
	//int         emi_peaks[120];//if only 15hz , MAX PEAK = 2048 * 15hz / 256 = 120 
	//int         absorp_peaks[120];
	//int         emi_count = 0;
	//int         absorp_count = 0;
	//double DataForPeakDetec[2048];
	//double      delta = 1e-6;
	//int         emission_first = 0;

	//for (int i = 0;i< 2048;i++)
	//{
	//	DataForPeakDetec[i] = Recoded_Array[CountOfRecodeSamples - 2048+i];
	//}
	//detect_peak(DataForPeakDetec, 2048,
	//	emi_peaks, &emi_count, 120,
	//	absorp_peaks, &absorp_count, 120,
	//	delta, emission_first);


	//if(emi_count != 0)
	//{
	//	vector<double> emi_peaks_AMP;//not index, means AMP
	//	
	//	for (int i = 0;i < emi_count;i++)
	//	{
	//		emi_peaks_AMP.push_back(DataForPeakDetec[emi_peaks[i]]);
	//	}
	//	sort(emi_peaks_AMP.begin(),emi_peaks_AMP.end());
	//	if(TargetOverPercent == 0)
	//	{
	//		ThreshValue_buf = 2.0*emi_peaks_AMP[0] + 0.1;
	//	}
	//	else if(TargetOverPercent == 100)
	//	{
	//		ThreshValue_buf = 2.0*emi_peaks_AMP[emi_count - 1] - 0.1;
	//	}
	//	else
	//	{
	//		double ind_buf = (1.0-0.01*TargetOverPercent)*emi_count;
	//		if(ind_buf >= 0 && ind_buf <= 1)
	//			ind_buf = 1;
	//		int indx_ = (int)ind_buf; 
	//		if(indx_ == 0)indx_ = 1;


	//		//if(indx_ >= 25)
	//		//{
	//		//	//ThreshValue_buf = 0.1*emi_peaks_AMP[indx_ - 1]*2.0 + 0.1*emi_peaks_AMP[indx_ - 2]*2.0
	//		//	//	+0.15*emi_peaks_AMP[indx_ - 3]*2.0 + 0.75*emi_peaks_AMP[indx_ - 4]*2.0;
	//		//	ThreshValue_buf = 0.5*emi_peaks_AMP[indx_ - 25]*2.0 + 0.5*emi_peaks_AMP[indx_ - 24]*2.0;
	//		//}
	//		//else 
	//		{
	//			ThreshValue_buf = emi_peaks_AMP[indx_ - 1]*2.0;
	//		}
	//	}
	//	ThreshValue = ThreshValue_buf;

	//	int bandcount = (int)(2048/(double)emi_count);
	//	for (int i = 0;i < emi_count;i++)
	//	{
	//		if(DataForPeakDetec[emi_peaks[i]] >= 0.5*ThreshValue)
	//		{
	//			for(int j = i*bandcount;j < (i+1)*bandcount;j++)
	//			{
	//				if(!Recoded_Array_IsOver[CountOfRecodeSamples - 2048 + j])
	//				{
	//					Recoded_Array_IsOver[CountOfRecodeSamples - 2048 + j] = true;
	//					CountofOverSample++;
	//				}
	//			}
	//		}
	//		else
	//		{
	//			for(int j = i*bandcount;j < (i+1)*bandcount;j++)
	//			{
	//				if(Recoded_Array_IsOver[CountOfRecodeSamples - 2048 + j])
	//				{
	//					Recoded_Array_IsOver[CountOfRecodeSamples - 2048 + j] = false;
	//					CountofOverSample--;
	//				}
	//			}
	//		}
	//	}
	//}

	//IsThresholdComputeEndding = true;
	
	//return true;
#pragma endregion M3
#pragma region M4
	//IsThresholdComputeEndding = false;
	//int emi_count = Peak_8sec_Arry.size();
	//if(emi_count != 0)
	//{
	//	vector<double> emi_peaks_AMP;//not index, means AMP
	//	vector<double> emi_peaks_AMP_;
	//	for (int i = 0;i < emi_count;i++)
	//	{
	//		emi_peaks_AMP.push_back(Peak_8sec_Arry[i]);
	//		emi_peaks_AMP_.push_back(Peak_8sec_Arry[i]);
	//	}
	//	//int rec_size_ = Recoded_Array_IsOver.size();
	//	sort(emi_peaks_AMP.begin(),emi_peaks_AMP.end());
	//	if(TargetOverPercent == 0)
	//	{
	//		ThreshValue_buf = emi_peaks_AMP[0] + 0.1;
	//	}
	//	else if(TargetOverPercent == 100)
	//	{
	//		ThreshValue_buf = emi_peaks_AMP[emi_count - 1] - 0.1;
	//	}
	//	else
	//	{
	//		double ind_buf = (1.0 - 0.01*TargetOverPercent)*emi_count;
	//		if(ind_buf >= 0 && ind_buf <= 1)
	//			ind_buf = 1;
	//		int indx_ = (int)ind_buf; 
	//		if(indx_ == 0)indx_ = 1;
	//		if(indx_ >= emi_count)emi_count - 1;
	//		ThreshValue_buf = emi_peaks_AMP[indx_];
	//	}
	//	ThreshValue = ThreshValue_buf;

	//	int bandcount = (int)(2048/(double)emi_count);
	//	int halfbandcount = (int)(1024/(double)emi_count);
	//	int rec_size = Recoded_Array_IsOver.size();


	//	for (int i = Peak_8sec_Arry.size() - 1;i > 0;i--)
	//	{
	//		if(Peak_8sec_Arry[i] >= ThreshValue)
	//		{
	//			for(int j = Peak_8sec_IDX_Arry[i] - halfbandcount;j < Peak_8sec_IDX_Arry[i]+ halfbandcount;j++)
	//			{
	//				//int iidx = rec_size -  j;
	//				int iidx = j;
	//				if(iidx < 0)
	//					iidx = 0;
	//				if(!Recoded_Array_IsOver[iidx])
	//				{
	//					Recoded_Array_IsOver[iidx] = true;
	//					CountofOverSample++;
	//				}
	//			}

	//		}
	//		else
	//		{
	//			for(int j = Peak_8sec_IDX_Arry[i] - halfbandcount;j < Peak_8sec_IDX_Arry[i]+ halfbandcount;j++)
	//				//for(int j = Peak_8sec_IDX_Arry[i]*bandcount;j < (i+1)*bandcount;j++)
	//			{
	//				//int iidx = rec_size - j;
	//				int iidx =j;
	//				if(iidx < 0)
	//					iidx = 0;
	//				if(Recoded_Array_IsOver[iidx])
	//				{
	//					Recoded_Array_IsOver[iidx] = false;
	//					CountofOverSample--;
	//				}
	//			}
	//		}
	//	}	
	//	//KK= KK;
	//}
	//IsThresholdComputeEndding = true;
	//return true;
#pragma endregion M4
#pragma region M5
	IsThresholdComputeEndding = false;
	int emi_count = Peak_8sec_Arry.size();
	if(emi_count != 0)
	{
		vector<double> emi_peaks_AMP;//not index, means AMP
		vector<double> emi_peaks_AMP_;
		for (int i = 0;i < emi_count;i++)
		{
			emi_peaks_AMP.push_back(Peak_8sec_Arry[i]);
			emi_peaks_AMP_.push_back(Peak_8sec_Arry[i]);
		}
		int rec_size_ = Recoded_Array_IsOver.size();
		sort(emi_peaks_AMP.begin(),emi_peaks_AMP.end());
		if(TargetOverPercent == 0)
		{
			ThreshValue_buf = emi_peaks_AMP[0] + 0.1;
		}
		else if(TargetOverPercent == 100)
		{
			ThreshValue_buf = emi_peaks_AMP[emi_count - 1] - 0.1;
		}
		else
		{
			double ind_buf = (1.0 - 0.01*TargetOverPercent)*emi_count;
			if(ind_buf >= 0 && ind_buf <= 1)
				ind_buf = 1;
			int indx_ = (int)ind_buf; 
			if(indx_ - 2 <= 0)indx_ = 2;
			//if(indx_ + 2>= emi_count)indx_ = emi_count - 3;
			//ThreshValue_buf = emi_peaks_AMP[indx_ + 2];
			if(indx_ - 2 >= emi_count)indx_ = emi_count - 1 + 2;

			ThreshValue_buf = emi_peaks_AMP[indx_ - 2];
		}
		ThreshValue = ThreshValue_buf;

		int bandcount = (int)(2048/(double)emi_count);
		for (int i = 0;i < emi_count;i++)
		{
			int start_idx = Peak_8sec_IDX_Arry[i] - (int)(0.25*bandcount);
			if(start_idx < 0) 
				start_idx = 0;
			int end_idx = start_idx + bandcount + 2;
			if(end_idx > CountOfRecodeSamples)
				end_idx = CountOfRecodeSamples;
			if(Peak_8sec_Arry[i] >= ThreshValue)
			{
				for(int j = start_idx;j < end_idx;j++)
				{
					if(!Recoded_Array_IsOver[j])
					{
						int idx_2048 = Current_sample_indx - (CountOfRecodeSamples - j);
						if(idx_2048 >= 0 && idx_2048 < 2048)
						{
							IsOverFlowed[idx_2048] = true;
						}
						
						Recoded_Array_IsOver[j] = true;

						CountofOverSample++;
					}
				}
			}
			else
			{
				for(int j = start_idx;j < end_idx;j++)
				{
					if(Recoded_Array_IsOver[j])
					{
						int idx_2048 = Current_sample_indx - (CountOfRecodeSamples - j);
						if(idx_2048 >= 0 && idx_2048 < 2048)
						{
							IsOverFlowed[idx_2048] = false;
						}
						
						Recoded_Array_IsOver[j] = false;
						CountofOverSample--;
					}
				}
			}
		}	
	}
	IsThresholdComputeEndding = true;
	return true;
#pragma endregion M5
}

bool EEGWave::NewHalfAuto3()
{
#pragma region M3
	//IsThresholdComputeEndding = false;
	//int         emi_peaks[288];//if only 36hz , MAX PEAK = 2048 * 36hz / 256 = 288 
	//int         absorp_peaks[288];
	//int         emi_count = 0;
	//int         absorp_count = 0;
	//double DataForPeakDetec[2048];
	//double      delta = 1e-6;
	//int         emission_first = 0;

	//for (int i = 0;i< 2048;i++)
	//{
	//	DataForPeakDetec[i] = Recoded_Array[CountOfRecodeSamples - 2048+i];
	//}
	//detect_peak(DataForPeakDetec, 2048,
	//	emi_peaks, &emi_count, 288,
	//	absorp_peaks, &absorp_count, 288,
	//	delta, emission_first);

	//if(emi_count != 0)
	//{
	//	vector<double> emi_peaks_AMP;//not index, means AMP
	//	
	//	for (int i = 0;i < emi_count;i++)
	//	{
	//		emi_peaks_AMP.push_back(DataForPeakDetec[emi_peaks[i]]);
	//	}
	//	sort(emi_peaks_AMP.begin(),emi_peaks_AMP.end());
	//	if(TargetOverPercent == 0)
	//	{
	//		ThreshValue_buf = 2.0*emi_peaks_AMP[0] + 0.1;
	//	}
	//	else if(TargetOverPercent == 100)
	//	{
	//		ThreshValue_buf = 2.0*emi_peaks_AMP[emi_count - 1] - 0.1;
	//	}
	//	else
	//	{
	//		double ind_buf = (0.01*TargetOverPercent)*emi_count;
	//		if(ind_buf >= 0 && ind_buf <= 1)
	//			ind_buf = 1;
	//		int indx_ = (int)ind_buf; 
	//		if(indx_ == 0)indx_ = 1;


	//		//if(indx_ >= 45)
	//		//{
	//		//	//ThreshValue_buf = 0.1*emi_peaks_AMP[indx_ - 1]*2.0 + 0.1*emi_peaks_AMP[indx_ - 2]*2.0
	//		//	//	+0.15*emi_peaks_AMP[indx_ - 3]*2.0 + 0.75*emi_peaks_AMP[indx_ - 4]*2.0;
	//		//	ThreshValue_buf = 0.5*emi_peaks_AMP[indx_ - 45]*2.0 + 0.5*emi_peaks_AMP[indx_ - 44]*2.0;
	//		//}
	//		//else 
	//		{
	//			ThreshValue_buf = emi_peaks_AMP[indx_ - 1]*2.0;
	//		}
	//	}
	//	ThreshValue = ThreshValue_buf;
	//	int bandcount = (int)(2048/(double)emi_count);
	//	for (int i = 0;i < emi_count;i++)
	//	{
	//		if(DataForPeakDetec[emi_peaks[i]] >= 0.5*ThreshValue)
	//		{
	//			for(int j = i*bandcount;j < (i+1)*bandcount;j++)
	//			{
	//				if(!Recoded_Array_IsOver[CountOfRecodeSamples - 2048 + j])
	//				{
	//					Recoded_Array_IsOver[CountOfRecodeSamples - 2048 + j] = true;
	//					CountofOverSample++;
	//				}
	//			}
	//		}
	//		else
	//		{
	//			for(int j = i*bandcount;j < (i+1)*bandcount;j++)
	//			{
	//				if(Recoded_Array_IsOver[CountOfRecodeSamples - 2048 + j])
	//				{
	//					Recoded_Array_IsOver[CountOfRecodeSamples - 2048 + j] = false;
	//					CountofOverSample--;
	//				}
	//			}
	//		}
	//	}
	//}
	//IsThresholdComputeEndding = true;
//return true;
#pragma endregion M3
#pragma region M4
	//IsThresholdComputeEndding = false;
	//int emi_count = Peak_8sec_Arry.size();
	//if(emi_count != 0)
	//{
	//	vector<double> emi_peaks_AMP;//not index, means AMP
	//	vector<double> emi_peaks_AMP_;
	//	for (int i = 0;i < emi_count;i++)
	//	{
	//		emi_peaks_AMP.push_back(Peak_8sec_Arry[i]);
	//		emi_peaks_AMP_.push_back(Peak_8sec_Arry[i]);
	//	}
	//	int rec_size_ = Recoded_Array_IsOver.size();
	//	sort(emi_peaks_AMP.begin(),emi_peaks_AMP.end());
	//	if(TargetOverPercent == 0)
	//	{
	//		ThreshValue_buf = emi_peaks_AMP[0] + 0.1;
	//	}
	//	else if(TargetOverPercent == 100)
	//	{
	//		ThreshValue_buf = emi_peaks_AMP[emi_count - 1] - 0.1;
	//	}
	//	else
	//	{
	//		double ind_buf = (0.01*TargetOverPercent)*emi_count;
	//		if(ind_buf >= 0 && ind_buf <= 1)
	//			ind_buf = 1;
	//		int indx_ = (int)ind_buf; 
	//		if(indx_ == 0)indx_ = 1;
	//		if(indx_ >= emi_count)emi_count - 1;
	//		ThreshValue_buf = emi_peaks_AMP[indx_];
	//	}
	//	ThreshValue = ThreshValue_buf;

	//	int bandcount = (int)(2048/(double)emi_count);
	//	int halfbandcount = (int)(1024/(double)emi_count);
	//	int rec_size = Recoded_Array_IsOver.size();


	//	for (int i = Peak_8sec_Arry.size() - 1;i > 0;i--)
	//	{
	//		if(Peak_8sec_Arry[i] >= ThreshValue)
	//		{
	//			for(int j = Peak_8sec_IDX_Arry[i] - halfbandcount;j < Peak_8sec_IDX_Arry[i]+ halfbandcount;j++)
	//			{
	//				//int iidx = rec_size -  j;
	//				int iidx = j;
	//				if(iidx < 0)
	//					iidx = 0;
	//				if(!Recoded_Array_IsOver[iidx])
	//				{
	//					Recoded_Array_IsOver[iidx] = true;
	//					CountofOverSample++;
	//				}
	//			}

	//		}
	//		else
	//		{
	//			for(int j = Peak_8sec_IDX_Arry[i] - halfbandcount;j < Peak_8sec_IDX_Arry[i]+ halfbandcount;j++)
	//				//for(int j = Peak_8sec_IDX_Arry[i]*bandcount;j < (i+1)*bandcount;j++)
	//			{
	//				//int iidx = rec_size - j;
	//				int iidx =j;
	//				if(iidx < 0)
	//					iidx = 0;
	//				if(Recoded_Array_IsOver[iidx])
	//				{
	//					Recoded_Array_IsOver[iidx] = false;
	//					CountofOverSample--;
	//				}
	//			}
	//		}
	//	}	
	//	//KK= KK;
	//}
	//IsThresholdComputeEndding = true;
	//return true;
#pragma endregion M4	
#pragma region M5
	IsThresholdComputeEndding = false;
	int emi_count = Peak_8sec_Arry.size();
	if(emi_count != 0)
	{
		vector<double> emi_peaks_AMP;//not index, means AMP
		vector<double> emi_peaks_AMP_;
		for (int i = 0;i < emi_count;i++)
		{
			emi_peaks_AMP.push_back(Peak_8sec_Arry[i]);
			emi_peaks_AMP_.push_back(Peak_8sec_Arry[i]);
		}
		int rec_size_ = Recoded_Array_IsOver.size();
		sort(emi_peaks_AMP.begin(),emi_peaks_AMP.end());
		if(TargetOverPercent == 0)
		{
			ThreshValue_buf = emi_peaks_AMP[0] + 0.1;
		}
		else if(TargetOverPercent == 100)
		{
			ThreshValue_buf = emi_peaks_AMP[emi_count - 1] - 0.1;
		}
		else
		{
			double ind_buf = (0.01*TargetOverPercent)*emi_count;
			if(ind_buf >= 0 && ind_buf <= 1)
				ind_buf = 1;
			int indx_ = (int)ind_buf; 
			if(indx_ == 0)indx_ = 1;
			if(indx_ +2>= emi_count)indx_ = emi_count - 3;
			ThreshValue_buf = emi_peaks_AMP[indx_ +2];
		}
		ThreshValue = ThreshValue_buf;

		int bandcount = (int)(2048/(double)emi_count);
		for (int i = 0;i < emi_count;i++)
		{
			int start_idx = Peak_8sec_IDX_Arry[i] - (int)(0.25*bandcount);
			if(start_idx < 0) 
				start_idx = 0;
			int end_idx = start_idx + bandcount + 2;
			if(end_idx > CountOfRecodeSamples)
				end_idx = CountOfRecodeSamples;
			if(Peak_8sec_Arry[i] >= ThreshValue)
			{
				for(int j = start_idx;j < end_idx;j++)
				{
					if(!Recoded_Array_IsOver[j])
					{
						int idx_2048 = Current_sample_indx - (CountOfRecodeSamples - j);
						if(idx_2048 >= 0 && idx_2048 < 2048)
						{
							IsOverFlowed[idx_2048] = true;
						}
					
						Recoded_Array_IsOver[j] = true;

						CountofOverSample++;
					}
				}
			}
			else
			{
				for(int j = start_idx;j < end_idx;j++)
				{
					if(Recoded_Array_IsOver[j])
					{
						int idx_2048 = Current_sample_indx - (CountOfRecodeSamples - j);
						if(idx_2048 >= 0 && idx_2048 < 2048)
						{
							IsOverFlowed[idx_2048] = false;
						}
						
						Recoded_Array_IsOver[j] = false;
						CountofOverSample--;
					}
				}
			}
		}	
	}
	IsThresholdComputeEndding = true;
	return true;
#pragma endregion M5
	
}

float EEGWave::PreHalfAuto()
{
	float ThreshValue_ = 0;
	if(IsInhibit)
	{
		bool isHigh = false;
		
		if(name[0] =='H')isHigh = true;
	
		if(!isHigh)
			ThreshValue_ = 2.0*Avg_amp_8sec*TargetOverPercent/100.0;
		else//high
			ThreshValue_ = 2.0*Avg_amp_8sec*TargetOverPercent/100.0;//*(double)TargetOverPercent/100;
	}
	else//Reward
		ThreshValue_ = 2.0*Avg_amp_8sec*(1.0-TargetOverPercent/100.0);
	
	if(ThreshValue_ > Absolute_Thresh)
		ThreshValue_ = Absolute_Thresh;
	if(ThreshValue_ < 0.01)
		ThreshValue_ = 0.01;
	return ThreshValue_;
}
int EEGWave::detect_peak(
        const double*   data, /* the data */ 
        int             data_count, /* row count of data */ 
        int*            emi_peaks, /* emission peaks will be put here */ 
        int*            num_emi_peaks, /* number of emission peaks found */
        int             max_emi_peaks, /* maximum number of emission peaks */ 
        int*            absop_peaks, /* absorption peaks will be put here */ 
        int*            num_absop_peaks, /* number of absorption peaks found */
        int             max_absop_peaks, /* maximum number of absorption peaks
                                            */ 
        double          delta, /* delta used for distinguishing peaks */
        int             emi_first /* should we search emission peak first of
                                     absorption peak first? */
        )
{
    int     i;
    double  mx;
    double  mn;
    int     mx_pos = 0;
    int     mn_pos = 0;
    int     is_detecting_emi = emi_first;


    mx = data[0];
    mn = data[0];

    *num_emi_peaks = 0;
    *num_absop_peaks = 0;

    for(i = 1; i < data_count; ++i)
    {
        if(data[i] > mx)
        {
            mx_pos = i;
            mx = data[i];
        }
        if(data[i] < mn)
        {
            mn_pos = i;
            mn = data[i];
        }

        if(is_detecting_emi &&
                data[i] < mx - delta)
        {
            if(*num_emi_peaks >= max_emi_peaks) /* not enough spaces */
                return 1;

            emi_peaks[*num_emi_peaks] = mx_pos;
            ++ (*num_emi_peaks);

            is_detecting_emi = 0;

            i = mx_pos - 1;

            mn = data[mx_pos];
            mn_pos = mx_pos;
        }
        else if((!is_detecting_emi) &&
                data[i] > mn + delta)
        {
            if(*num_absop_peaks >= max_absop_peaks)
                return 2;

            absop_peaks[*num_absop_peaks] = mn_pos;
            ++ (*num_absop_peaks);

            is_detecting_emi = 1;
            
            i = mn_pos - 1;

            mx = data[mn_pos];
            mx_pos = mn_pos;
        }
    }

    return 0;
}
void EEGWave::AMPThresholdIsOverfByPeakMethod(int Band_Max_hz,int size_)
{
	int         *emi_peaks;//if only 36hz , MAX PEAK = 2048 * 36hz / 256 = 288 
	int maxpeaknum = (int)(size_ * Band_Max_hz /256.0);
	emi_peaks = new int[maxpeaknum];
	int *absorp_peaks = new int[maxpeaknum];
	int         emi_count = 0;
	int         absorp_count = 0;
	double *DataForPeakDetec = new double[size_];
	double      delta = 1e-6;
	int         emission_first = 0;

	for (int i = 0;i< size_;i++)
	{
		DataForPeakDetec[i] = Recoded_Array[CountOfRecodeSamples - size_ +i];
	}
	detect_peak(DataForPeakDetec, size_,
		emi_peaks, &emi_count,maxpeaknum,
		absorp_peaks, &absorp_count, maxpeaknum,
		delta, emission_first);

	if(emi_count != 0)
	{
		int bandcount = (int)(size_/(double)emi_count);
		int halfbandcount = (int)(0.5*bandcount);
		/*if(size_ == 64)
			size_ = 64;*/
		for (int i = 0;i < emi_count;i++)
		{
			int start_idx = emi_peaks[i] + CountOfRecodeSamples - size_ - (int)(0.25*bandcount);//real indx
			int end_idx = (start_idx + bandcount) > CountOfRecodeSamples ? CountOfRecodeSamples:(start_idx + bandcount);
			if(start_idx < 0) start_idx = 0;
			
			if(DataForPeakDetec[emi_peaks[i]] >= 0.5*ThreshValue)
			{
				for(int j = start_idx;j < end_idx;j++)
				{
					if(!Recoded_Array_IsOver[j])
					{
						int idx_2048 = Current_sample_indx - (CountOfRecodeSamples - j);
						if(idx_2048 >= 0 && idx_2048 < 2048)
						{
							IsOverFlowed[idx_2048] = true;
						}

						/*if(Current_sample_indx >= 64)
						{
							int idx_2048 = Current_sample_indx - (CountOfRecodeSamples - j);
							IsOverFlowed[idx_2048] = true;
						}
						else
						{
							for(int k = 0;k< 32;k++)
								IsOverFlowed[k] = true;
						}*/
						
						Recoded_Array_IsOver[j] = true;
						CountofOverSample++;
					}
				}
			}
			else
			{
				for(int j = start_idx;j < end_idx;j++)
				{
					if(Recoded_Array_IsOver[j])
					{
						int idx_2048 = Current_sample_indx - (CountOfRecodeSamples - j);
						if(idx_2048 >= 0 && idx_2048 < 2048)
						{
							IsOverFlowed[idx_2048] = false;
						}
						/*if(Current_sample_indx >= 64)
						{
							int idx_2048 = Current_sample_indx - (CountOfRecodeSamples - j);
							IsOverFlowed[idx_2048] = false;
						}
						else
						{
							for(int k = 0;k< 32;k++)
							IsOverFlowed[k] = false;
						}*/
						Recoded_Array_IsOver[j] = false;
						CountofOverSample--;
					}
				}
			}
			
			if((int)Peak_8sec_Arry.size() < end_hz * 8)//8sec
			{
				Peak_8sec_Arry.push_back(DataForPeakDetec[emi_peaks[i]]*2.0);
				Peak_8sec_IDX_Arry.push_back(emi_peaks[i] + CountOfRecodeSamples - size_);
			}
			else
			{
				Peak_8sec_Arry.erase(Peak_8sec_Arry.begin());
				Peak_8sec_Arry.push_back(DataForPeakDetec[emi_peaks[i]]*2.0);

				Peak_8sec_IDX_Arry.erase(Peak_8sec_IDX_Arry.begin());
				Peak_8sec_IDX_Arry.push_back(emi_peaks[i] + CountOfRecodeSamples - size_);
			}
		}
	}
	delete emi_peaks;
	delete absorp_peaks;
	delete DataForPeakDetec;
}