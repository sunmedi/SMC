#include "stdAfx.h"
#include "Spectrum.h"
#define AMPSCAL 2
Spectrum::Spectrum(void)
{
	width = 0;
	height = 0;
	IsChanel2 = false;
	out_array = NULL;
	out_array256 = NULL;
	IsFull2048 = false;
	Low_success = 0;
	Reward_success = 0;
	High_success = 0;

	Low_TCount = 0;
	Low_SCount = 0;
	Rew_TCount = 0;
	Rew_SCount = 0;
	Hig_TCount = 0;
	Hig_SCount = 0;
	Low_success = 0.0;
	Reward_success = 0.0;
	High_success = 0.0;
}

Spectrum::~Spectrum(void)
{
}
void Spectrum::DestroySpectrum()
{
	fftw_destroy_plan(fft_p); 
	fftw_free(out_array);
	fftw_destroy_plan(fft_p256); 
	fftw_free(out_array256);
}
void Spectrum::ResetSpectrum()
{
	for(int i = 0;i < 50;i++)
	{
		X_Array_50[i] = i * 16*(double)width/1920.0;
		Y_Array_AS_50[i] = 0;
		Y_Array_PS_50[i] = 0;
		Y_Array_Phase_50[i] =0;
	}
	for( int i=0; i < PROCESSING_FFT_BUF_SIZE; i++)
		in_array_double[i] = 0.0;
	for( int i=0; i < 256; i++)
		in_array_double_256[i] = 0.0;

	/*for( int i=0; i <= PROCESSING_FFT_BUF_SIZE*0.5 + 1; i++)
	{
		out_array[i][0] = 0.0;
		out_array[i][1] = 0.0;
	}
		
	for( int i=0; i <= 129; i++)
	{
		out_array256[i][0] = 0.0;
		out_array256[i][1] = 0.0;
	}*/

	Low_success = 0;
	Reward_success = 0;
	High_success = 0;

	Low_TCount = 0;
	Low_SCount = 0;
	Rew_TCount = 0;
	Rew_SCount = 0;
	Hig_TCount = 0;
	Hig_SCount = 0;

	derta_sum = 0;
	theta_sum = 0;
	alpha_sum = 0;
	beta_sum = 0;
	gamma_sum = 0;

	SumOfAS = 0;
	SumOfPS = 0;
	IsFull2048 = false;
}
void Spectrum::HammingWin(int windowLength, float *buffer)
{
	for(int i = 0; i < windowLength; i++) 
	{
		buffer[i] = 0.54 - (0.46 * cos( 2 * Pi * (i / ((windowLength - 1) * 1.0))));
	}
}
void Spectrum::InitSpectrum(int SamplesHz,string SensorPosName_,int width_,int height_)
{
	width = (double)width_;
	height = (double)height_;
	derta_sum = 0;
	theta_sum = 0;
	alpha_sum = 0;
	beta_sum = 0;
	gamma_sum = 0;

	SumOfAS = 0;
	SumOfPS = 0;
	Samples_Hz = SamplesHz;
	for(int i = 0;i < SamplesHz;i++)
	{
		//X_Array.push_back(x_);
		//Y_Array_PS.push_back(0.0);
		//Y_Array_AS.push_back(0.0);
		X_Array_50[i] = i * 16*width/1920.0;
		Y_Array_AS_50[i] = 0;
		Y_Array_PS_50[i] = 0;
		Y_Array_Phase_50[i] =0;
	}
	SensorPosName = SensorPosName_;
	//in_array =(fftw_complex*) fftw_malloc(sizeof(fftw_complex)* PROCESSING_FFT_BUF_SIZE);
	out_array =(fftw_complex*) fftw_malloc(sizeof(fftw_complex)* (0.5*PROCESSING_FFT_BUF_SIZE + 1));
	out_array256 =(fftw_complex*) fftw_malloc(sizeof(fftw_complex)* 129);
	/*out_array8 =(fftw_complex*) fftw_malloc(sizeof(fftw_complex)* 5);
	out_array16 =(fftw_complex*) fftw_malloc(sizeof(fftw_complex)* 9);
	out_array32 =(fftw_complex*) fftw_malloc(sizeof(fftw_complex)* 17);
	out_array64 =(fftw_complex*) fftw_malloc(sizeof(fftw_complex)* 33);
	out_array128 =(fftw_complex*) fftw_malloc(sizeof(fftw_complex)* 65);*/
	for( int i=0; i < PROCESSING_FFT_BUF_SIZE; i++)
		in_array_double[i] = 0.0;
	for( int i=0; i < 256; i++)
		in_array_double_256[i] = 0.0;
	/*for( int i=0; i < 128; i++)
		in_array_double_128[i] = 0.0;
	for( int i=0; i < 64; i++)
		in_array_double_64[i] = 0.0;
	for( int i=0; i < 32; i++)
		in_array_double_32[i] = 0.0;
	for( int i=0; i < 16; i++)
		in_array_double_16[i] = 0.0;
	for( int i=0; i < 8; i++)
		in_array_double_8[i] = 0.0;*/
	
	
	//fft_p = fftw_plan_dft_1d(PROCESSING_FFT_BUF_SIZE,
	//	in_array,out_array,FFTW_FORWARD,FFTW_ESTIMATE);
	//fft_p = fftw_plan_dft_1d(PROCESSING_FFT_BUF_SIZE,
	//	in_array,out_array,FFTW_FORWARD,FFTW_MEASURE);
	
	//fft_p = fftw_plan_dft_r2c_1d(PROCESSING_FFT_BUF_SIZE,
	//	in_array_double,out_array,FFTW_MEASURE);
	fft_p = fftw_plan_dft_r2c_1d(PROCESSING_FFT_BUF_SIZE,in_array_double,out_array,FFTW_ESTIMATE);
	fft_p256 = fftw_plan_dft_r2c_1d(256,in_array_double_256,out_array256,FFTW_ESTIMATE);
	//fft_p128 = fftw_plan_dft_r2c_1d(128,in_array_double_128,out_array256,FFTW_ESTIMATE);
	
	HammingWin(256,HammingWin256);
	HammingWin(2048,HammingWin2048);
}
void Spectrum::SetSpectrumOrg(float x,float y)
{
	org.x = x;
	org.y = y;
}
void Spectrum::UpdateSpectrum()
{
	SumOfPS = SumOfAS = 0.0;
	derta_sum = 0;
	theta_sum = 0;
	alpha_sum = 0;
	beta_sum = 0;
	gamma_sum = 0;

	if(IsFull2048)
	{
		int indx_1024 = 0;
		double magnitudeMax_in8 = 0;
		for(int i = 0;i < Samples_Hz; i++)
		{
			magnitudeMax_in8 = 0;
			int count_nozero=0;
			for(int k = 0;k < 8;k++)
			{
				double magnitude = sqrt(out_array[indx_1024 + k][0]*out_array[indx_1024+ k][0] + out_array[indx_1024+ k][1]*out_array[indx_1024+ k][1]);
				//if(magnitude > magnitudeMax_in8)
					//magnitudeMax_in8 = magnitude;
				//if(magnitude > 0.001)
				//{
					magnitudeMax_in8 += magnitude;
					//count_nozero++;
				//}
			}
			//magnitudeMax_in8 /= count_nozero;//avg
			//double magnitude = sqrt(out_array[indx_1024][0]*out_array[indx_1024][0] + out_array[indx_1024][1]*out_array[indx_1024][1]);

			//double as_ = 2.0*magnitude/PROCESSING_FFT_BUF_SIZE;//(I -> 0.5*N -1)
			double as_ = 2.0*magnitudeMax_in8/(PROCESSING_FFT_BUF_SIZE);//(I -> 0.5*N -1)
			//double as_ = magnitude/(8.0*Samples_Hz);//(I -> 0.5*N -1)
			//Y_Array_AS.push_back(as_);
			Y_Array_AS_50[i] = as_;
			
			//Y_Array_AS.push_back(as_);

			double PowerSpectrum_ = as_ * as_;
			PowerSpectrum_ = 10*log10(PowerSpectrum_);
			
			//double PowerSpectrum_ = 20*log10(as_*AMPSCAL);
			double y_ = PowerSpectrum_;
			//Y_Array_PS.push_back(y_);
			Y_Array_PS_50[i] = PowerSpectrum_;
			if(i != 0)
			{
				SumOfAS += as_;
				if (y_ < 0.0)y_ = 0.0;
				SumOfPS += y_;

				if(i<4)derta_sum +=  as_;
				else if( i>= 4 && i<= 7)theta_sum += as_;
				else if( i>= 8 && i<= 14)alpha_sum += as_;
				else if( i>= 15 && i<= 32)beta_sum += as_;
				else gamma_sum += as_;
			}
			indx_1024 += 8;
		}//end for
	}
	else
	{
		for(int i = 0;i < Samples_Hz; i++)
		{
			double magnitude = sqrt(out_array256[i][0]*out_array256[i][0] + out_array256[i][1]*out_array256[i][1]);
			double as_ = 2.0*magnitude/256;//(I -> 0.5*N -1)
			
			Y_Array_AS_50[i] = as_;

			double PowerSpectrum_ = 10*log10(as_*as_);
			//double PowerSpectrum_ = 20*log10(as_*AMPSCAL);
			double y_ = PowerSpectrum_;
			Y_Array_PS_50[i] = PowerSpectrum_;
			if(i != 0)
			{
				SumOfAS += as_;
				if (y_ < 0.0)y_ = 0.0;
				SumOfPS += y_;

				if(i<4)derta_sum +=  as_;
				else if( i>= 4 && i<= 7)theta_sum += as_;
				else if( i>= 8 && i<= 14)alpha_sum += as_;
				else if( i>= 15 && i<= 32)beta_sum += as_;
				else gamma_sum += as_;
			}
		}//end for
	}
	Y_Array_AS_50[0]= Y_Array_PS_50[0] = 0.0;
	
}
void Spectrum::UpdateSpectrum256TLC()
{
	for(int i = 0;i < Samples_Hz; i++)
	{
		double magnitude = sqrt(out_array256[i][0]*out_array256[i][0] + out_array256[i][1]*out_array256[i][1]);
		double as_ = 2.0*magnitude/256.0;//(I -> 0.5*N -1)

		Y_Array_AS_50[i] = as_;
		Y_Array_Phase_50[i] = atan2(out_array256[i][1],out_array256[i][0])*180.0/Pi;
	}
}
void Spectrum::UpdateSpectrum256()
{

	High_sum = Reward_sum = Low_sum = 0;
	for(int i = 0;i < Samples_Hz; i++)
	{
		double magnitude = sqrt(out_array256[i][0]*out_array256[i][0] + out_array256[i][1]*out_array256[i][1]);
		double as_ = 2.0*magnitude/256;//(I -> 0.5*N -1)

		//Y_Array_AS_50[i] = as_;

		double PowerSpectrum_ = 10*log10(as_*as_);
		//double PowerSpectrum_ = 20*log10(as_*AMPSCAL);
		double y_ = PowerSpectrum_;
		//Y_Array_PS_50[i] = PowerSpectrum_;
		if(i != 0)
		{

			if( i>= 4 && i<= 7)theta_sum += as_;
			else if( i>= 8 && i<= 14)alpha_sum += as_;
			else if( i>= 15 && i<= 32)beta_sum += as_;
			else if( i>= 33 && i<= 49)gamma_sum += as_;
		}
	}//end for
	//Y_Array_AS_50[0]= Y_Array_PS_50[0] = 0.0;
}
void Spectrum::FFTProcessing(double Y_Array_R[],int currentDataFrameInx_FFT)
{
	if(currentDataFrameInx_FFT < 256)
	{
		for(int j = 0;j < PROCESSING_FFT_BUF_SIZE;j++)
		{
			if(j < currentDataFrameInx_FFT)
				in_array_double[j] = Y_Array_R[j];
			else
				in_array_double[j] = 0.0;
		}
	}
	else
	{
		for(int j = 0;j < PROCESSING_FFT_BUF_SIZE;j++)
		{
			in_array_double[j] = (double)(Y_Array_R[j + currentDataFrameInx_FFT - PROCESSING_FFT_BUF_SIZE]);//update per 0.125sec
			
		}
	}
	fftw_execute(fft_p);
}
void Spectrum::FFTProcessing256TLC(double Y_Array[],int c_idx)
{
	IsFull2048 = false;
	for(int j = 0;j < 256;j++)
	{
		in_array_double_256[j] = (double)(Y_Array[j + c_idx - 256]);//update per 0.125sec
	}
	fftw_execute(fft_p256);
}
void Spectrum::FFTProcessing4(double Y_Array[],int c_idx)//46080 for TLC
{
	if(c_idx < PROCESSING_FFT_BUF_SIZE)
	{
		if(c_idx < 256)
		{
			for(int j = 0;j < 256;j++)
			{
				if(j < c_idx)
					in_array_double_256[j] = Y_Array[j];
				else
					in_array_double_256[j] = 0.0;
			}
		}
		else
		{
			for(int j = 0;j < 256;j++)
			{
				in_array_double_256[j] = (double)(Y_Array[j + c_idx - 256]);//update per 0.125sec
			}
		}
		fftw_execute(fft_p256);
	}
	else
	{
		IsFull2048 = true;
		for(int j = 0;j < PROCESSING_FFT_BUF_SIZE;j++)
		{
			in_array_double[j] = (double)(Y_Array[j + c_idx - PROCESSING_FFT_BUF_SIZE]);
		}
		fftw_execute(fft_p);
	}
}
void Spectrum::FFTProcessing3(vector<double>Y_Array)//2048
{
	int Inx_FFT = Y_Array.size();
	if(Inx_FFT < PROCESSING_FFT_BUF_SIZE)
	{
		if(Inx_FFT < 256)
		{
			for(int j = 0;j < 256;j++)
			{
				if(j < (int)Y_Array.size())
					in_array_double_256[j] = Y_Array[j];
				else
					in_array_double_256[j] = 0.0;
			}
		}
		else
		{
			for(int j = 0;j < 256;j++)
			{
				in_array_double_256[j] = (double)(Y_Array[j + Inx_FFT - 256]);//update per 0.125sec
			}
		}
		//window process//
		/*for(int j = 0;j < 256;j++)
			in_array_double_256[j] *= HammingWin256[j];*/

		fftw_execute(fft_p256);
	}
	else
	{
		IsFull2048 = true;
		for(int j = 0;j < PROCESSING_FFT_BUF_SIZE;j++)
		{
			//in_array_double[j] = (double)(Y_Array[j + Inx_FFT - PROCESSING_FFT_BUF_SIZE]) * HammingWin2048[j];
			in_array_double[j] = (double)(Y_Array[j + Inx_FFT - PROCESSING_FFT_BUF_SIZE]) ;
		}

		fftw_execute(fft_p);
	}
}
void Spectrum::FFTProcessing2(vector<double>Y_Array)//compute for recoded data/input data  in one time
{
	for(unsigned int j = 0;j < Y_Array.size();j++)
	{
		in_array_double[j] = (double)(Y_Array[j])/32727;//update per 0.125sec

	}
	fftw_execute(fft_p);
}
void Spectrum::FFTProcessingDAQ(vector<double>Y_Array_R)
{
	for(int j = 0;j < PROCESSING_FFT_BUF_SIZE;j++)
	{
		in_array_double[j] = Y_Array_R[j];
	}
	fftw_execute(fft_p);
}
void Spectrum::FFTProcessing256(vector<double>Y_Array_R)
{
	for(int j = 0;j < 256;j++)
	{
		in_array_double_256[j] = Y_Array_R[j];
	}
	fftw_execute(fft_p256);
}
void Spectrum::DisplayPowerSpectrum()
{
	stringstream ss_2 (stringstream::in | stringstream::out);
	string ss_;
	double max_aaa = 0;
	int max_ps_indx =0;
	for(int j =0;j<Samples_Hz;j++)
		if(Y_Array_PS_50[j] > max_aaa)
		{
			max_aaa = Y_Array_PS_50[j];
			max_ps_indx = j;
		}
	glPushMatrix();
	glTranslatef(org.x,org.y,0);

	/////////////////////AMP current max///////////
	glPushMatrix();
	glColor3d(0.0, 1.0, 1.0);
	//glRasterPos2f(X_Array[max_ps_indx], 0.05*Y_Array_PS[max_ps_indx]);
	glRasterPos2f(X_Array_50[max_ps_indx], Y_Array_PS_50[max_ps_indx]*AMPSCAL);
	ss_ = "dB";
	int ps_ = (int)Y_Array_PS_50[max_ps_indx];
	if(ps_ >= 0)
	{
		//ss_2 << (int)(327.67*Y_Array_AS[max_as_indx]);//new 303 uv 
		ss_2 << ps_;//new 303 uv
		ss_ = "         ->"+ss_2.str() + ss_;
	}
	else
	{
		ss_2 << abs(ps_);//new 303 uv
		ss_ = "-"+ss_2.str() + ss_;
	}
	
	//drawString(ss_.c_str());
	ss_2.str("");
	glPopMatrix();
	/////////////////////AMP current max///////////
	glPopMatrix();

	glPushMatrix();
	glTranslatef(org.x,org.y,0);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_LINE_SMOOTH);
	glLineWidth(1.0);
	glBegin( GL_LINE_STRIP );
	glColor3d(0.0, 1.0, 1.0);

	//for(int j =0;j<Samples_Hz;j++)	
		//glVertex3f(X_Array_50[j], Y_Array_PS_50[j], 0.0); 
		
	for(int j =0;j<Samples_Hz;j++)
	{
		if(Y_Array_AS_50[j] < 55)
		{
			if(Y_Array_PS_50[j]*AMPSCAL < -10)
				glVertex3f(X_Array_50[j], -10, 0.0);
			else
				glVertex3f(X_Array_50[j], Y_Array_PS_50[j]*AMPSCAL, 0.0);
		}
		else
			glVertex3f(X_Array_50[j], 110, 0.0);

		
	}
	

	glEnd();
	glLineWidth(1.0);

	glBegin( GL_LINES );
	glColor3d(0.0, 0.0, 0.0);
	glVertex3f(0, 0.0, 0.0); 
	glVertex3f(900, 0.0, 0.0);
	glVertex3f(0, 0.0, 0.0); 
	glVertex3f(0, 120.0, 0.0);
	glEnd();

	glRasterPos2f(0.0, -15.0);
	drawString("0hz");

	glPushMatrix();//x
	glTranslatef(900,0,0);
	glRasterPos2f(-15.0, -15.0);
	drawString("50hz");
	//glTranslatef(0.0f, 0.0, 0.0);
	glBegin(GL_TRIANGLES);
	glVertex3f(0.0,5.0,0.0);
	glVertex3f(0.0,-5.0,0.0);
	glVertex3f(14.1f,0.0,0.0);
	glEnd();
	glPopMatrix();

	glPushMatrix();//y
	glTranslatef(0,120,0);
	glBegin(GL_TRIANGLES);
	glVertex3f(-5.0,0.0,0.0);
	glVertex3f(5.0,0.0,0.0);
	glVertex3f(0.0,14.1f,0.0);
	glEnd();
	glPopMatrix();

	glPopMatrix();
	glDisable(GL_LINE_SMOOTH);
	glDisable(GL_BLEND);
}
void Spectrum::DisplayPowerSpectrumTLC()
{
	stringstream ss_2 (stringstream::in | stringstream::out);
	string ss_;
	double max_aaa = 0;
	int max_ps_indx =0;
	for(int j =0;j<Samples_Hz;j++)
		if(Y_Array_PS_50[j] > max_aaa)
		{
			max_aaa = Y_Array_PS_50[j];
			max_ps_indx = j;
		}
		glPushMatrix();
		glTranslatef(org.x,org.y,0);

		/////////////////////AMP current max///////////
		glPushMatrix();
		glColor3d(0.0, 1.0, 1.0);
		//glRasterPos2f(X_Array[max_ps_indx], 0.05*Y_Array_PS[max_ps_indx]);
		if(!IsChanel2)
			glRasterPos2f(X_Array_50[max_ps_indx], Y_Array_PS_50[max_ps_indx]*AMPSCAL);
		else
			glRasterPos2f(X_Array_50[max_ps_indx], -1*Y_Array_PS_50[max_ps_indx]*AMPSCAL);

		ss_ = "dB";
		int ps_ = (int)Y_Array_PS_50[max_ps_indx];
		if(ps_ >= 0)
		{
			//ss_2 << (int)(327.67*Y_Array_AS[max_as_indx]);//new 303 uv 
			ss_2 << ps_;//new 303 uv
			ss_ = "         ->"+ss_2.str() + ss_;
		}
		else
		{
			ss_2 << abs(ps_);//new 303 uv
			ss_ = "-"+ss_2.str() + ss_;
		}

		//drawString(ss_.c_str());
		ss_2.str("");
		glPopMatrix();
		/////////////////////AMP current max///////////
		glPopMatrix();

		glPushMatrix();
		glTranslatef(org.x,org.y,0);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_LINE_SMOOTH);
		glLineWidth(1.0);
		glBegin( GL_LINE_STRIP );
		glColor3d(0.0, 1.0, 1.0);

		//for(int j =0;j<Samples_Hz;j++)	
		//glVertex3f(X_Array_50[j], Y_Array_PS_50[j], 0.0); 

		if(!IsChanel2)
		{
			for(int j =0;j<Samples_Hz;j++)
			{
				if(Y_Array_AS_50[j] < 55)
				{
					if(Y_Array_PS_50[j]*AMPSCAL < -10)
						glVertex3f(X_Array_50[j], -10, 0.0);
					else
						glVertex3f(X_Array_50[j], Y_Array_PS_50[j]*AMPSCAL, 0.0);
				}
				else
					glVertex3f(X_Array_50[j], 110, 0.0);
			}
		}
		else
		{
			for(int j = 0;j<Samples_Hz;j++)
			{
				if(Y_Array_AS_50[j] < 55)
				{
					if(-1*Y_Array_PS_50[j]*AMPSCAL > 10)
						glVertex3f(X_Array_50[j], 10, 0.0);
					else
						glVertex3f(X_Array_50[j], -1*Y_Array_PS_50[j]*AMPSCAL, 0.0);
				}
				else
					glVertex3f(X_Array_50[j], -110, 0.0);
			}	
		}
	

		glEnd();
		glLineWidth(1.0);

		glBegin( GL_LINES );
		glColor3d(0.0, 0.0, 0.0);
		glVertex3f(0, 0.0, 0.0); 
		glVertex3f(900, 0.0, 0.0);
		glEnd();
		if(!IsChanel2)
		{
			glBegin( GL_LINES );
			glVertex3f(0, 0.0, 0.0); 
			glVertex3f(0, 120.0, 0.0);
			glEnd();
		}
		else
		{
			glBegin( GL_LINES );
			glVertex3f(0, 0.0, 0.0); 
			glVertex3f(0, -120.0, 0.0);
			glEnd();
		}
		glEnd();

		if(!IsChanel2)
		{
			glRasterPos2f(0.0, -15.0);
			drawString(" 0hz");
		}
		
		glPushMatrix();//x
		glTranslatef(900,0,0);
		if (!IsChanel2)
		{
			glRasterPos2f(-15.0, -15.0);
			drawString("50hz");
		}
			
		
		//glTranslatef(0.0f, 0.0, 0.0);
		glBegin(GL_TRIANGLES);
		glVertex3f(0.0,5.0,0.0);
		glVertex3f(0.0,-5.0,0.0);
		glVertex3f(14.1f,0.0,0.0);
		glEnd();
		glPopMatrix();

		if(!IsChanel2)
		{
			glPushMatrix();//y
			glTranslatef(0,120,0);

			glRasterPos2f(5.0, 15.0);
			drawString("EEG Singal 1");

			glBegin(GL_TRIANGLES);
			glVertex3f(-5.0,0.0,0.0);
			glVertex3f(5.0,0.0,0.0);
			glVertex3f(0.0,14.1f,0.0);
			glEnd();
			glPopMatrix();
		}
		else
		{
			glPushMatrix();//y
			glTranslatef(0,-120,0);

			glRasterPos2f(5.0, -15.0);
			drawString(" EEG Singal 2");


			glBegin(GL_TRIANGLES);
			glVertex3f(-5.0,0.0,0.0);
			glVertex3f(5.0,0.0,0.0);
			glVertex3f(0.0,-14.1f,0.0);
			glEnd();
			glPopMatrix();
		}

		glPopMatrix();
		glDisable(GL_LINE_SMOOTH);
		glDisable(GL_BLEND);
}
void Spectrum::DisplayAmplitudeSpectrum()
{
	stringstream ss_2 (stringstream::in | stringstream::out);
	string ss_,ss_1;
	double max_aaa = 0;
	int max_as_indx =0;
	for(int j =0;j<Samples_Hz;j++)
	{
		if(Y_Array_AS_50[j] > max_aaa)
		{
			max_aaa = Y_Array_AS_50[j];
			max_as_indx = j;
		}
	}
		
	/*glColor3d(0.1, 0.1, 0.2);
	glBegin(GL_QUADS);
	glVertex3d(-200.0, height,-0.1);
	glVertex3d(-200, 0,-0.1);
	glVertex3d(0, 0,-0.1);
	glVertex3d(0, height,-0.1);
	glEnd();*/

	glPushMatrix();
	glTranslatef(org.x,org.y,-0.5);

	glLineWidth(1.0);
	glColor3d(0.0, 0.0, 0.0);
	glBegin( GL_LINES );
	glVertex3f(X_Array_50[0] + 16, -1.0, 1.0); 
	glVertex3f(X_Array_50[49] + 16, -1.0, 1.0);
	glEnd();

	glPushMatrix();
	glTranslatef(0.0, 0.0, 1.0);
	glRasterPos2f(16.0, -15.0);
	drawString("0hz");
	glTranslatef(X_Array_50[49] + 10,0.0,0.0);
	glRasterPos2f(-25.0, -15.0);
	drawString("50hz");
	glPopMatrix();

	//glPushMatrix();
	//glColor3d(1.0, 0.0, 0.0);
	//glRasterPos2f(10.0, 120.0);
	////drawString(SensorPosName.c_str());
	//glPopMatrix();

	/////////////////////AMP current max///////////
	//glPushMatrix();
	//if(max_as_indx < 4)glColor3d(1.0, 0.0, 0.0);
	//else if( max_as_indx >= 4 && max_as_indx <= 7)glColor3d(0.0, 1.0, 0.0);
	//else if( max_as_indx >= 8 && max_as_indx <= 14)glColor3d(0.0, 0.0, 1.0);
	//else if( max_as_indx >= 15 && max_as_indx <= 32)glColor3d(1.0, 0.0, 1.0);
	//else glColor3d(0.1, 0.4, 0.6);
	//glRasterPos2f(X_Array_50[max_as_indx], Y_Array_AS_50[max_as_indx]*AMPSCAL);
	//ss_ = "uV";
	//
	//int as_ = (int)(Y_Array_AS_50[max_as_indx]);
	//ss_2 << as_;
	//ss_ = ss_2.str() + ss_;
	////drawString(ss_.c_str());
	//ss_2.str("");
	//glPopMatrix();
	/////////////////////AMP current max///////////

	//glLineWidth(12.0);
	glBegin(GL_QUADS);
	for(int j =0;j<Samples_Hz;j++)	
	{
		if(j < 10)glColor3d(0.8*1.0, (double)(j)*0.8/10.0, 0.0);
		else if( j>= 10 && j< 20)glColor3d(1.0*0.8 - (double)(j - 10)*0.8/10.0, 1.0*0.8, 0.0);
		else if( j>= 20 && j< 30)glColor3d(0.0, 1.0*0.8 ,(double)(j+1)*0.8/30.0);
		else if( j>= 30 && j< 40)glColor3d(0.0, 1.0*0.8 - (double)(j - 30)*0.8/10.0,1.0*0.8 );
		else if( j>= 40 && j< 50)glColor3d((double)(j-39)*0.8/10.0, 0.0, 1.0*0.8);
		
		glVertex3f(X_Array_50[j], 0.0, 1.0); 
		glVertex3f(X_Array_50[j] + 15, 0.0, 1.0); 
		
		if(j < 10)glColor3d(1.0, (double)(j)/10.0, 0.0);
		else if( j>= 10 && j< 20)glColor3d(1.0 - (double)(j - 10)/10.0, 1.0, 0.0);
		else if( j>= 20 && j< 30)glColor3d(0.0, 1.0 ,(double)(j+1)/30.0);
		else if( j>= 30 && j< 40)glColor3d(0.0, 1.0 - (double)(j - 30)/10.0,1.0 );
		else if( j>= 40 && j< 50)glColor3d((double)(j-39)/10.0, 0.0, 1.0);
		if(Y_Array_AS_50[j] < 55)
		{
			glVertex3f(X_Array_50[j] + 15, Y_Array_AS_50[j]*AMPSCAL, 1.0);
			glVertex3f(X_Array_50[j] , Y_Array_AS_50[j]*AMPSCAL, 1.0);
		}
		else
		{
			glVertex3f(X_Array_50[j] + 15, 110, 1.0);
			glVertex3f(X_Array_50[j], 110, 1.0);
		}
			
	}
	glEnd();
	//glLineWidth(1.0);

	//glPushMatrix();//y
	////glTranslatef(105.0, 130.0, 1.0);
	//glTranslatef(width*0.61, 40.0, 1.0);
	//if(SumOfAS == 0.0)SumOfAS = 0.0001;
	//double dp = derta_sum/SumOfAS;
	//double tp = theta_sum/SumOfAS;
	//double ap = alpha_sum/SumOfAS;
	//double bp = beta_sum/SumOfAS;
	//double gp = gamma_sum/SumOfAS;

	//
	///*int dd = (int)(1000*dp);
	//int tt = dd + (int)(1000*tp);
	//int aa = tt + (int)(1000*ap);
	//int bb = aa + (int)(1000*bp);
	//int gg = bb + (int)(1000*gp);*/
	//double dd = (2*Pi*dp);
	//double tt = dd + (2*Pi*tp);
	//double aa = tt + (2*Pi*ap);
	//double bb = aa + (2*Pi*bp);
	//double gg = bb + (2*Pi*gp);

	//glColor3d(1.0, 0.1, 0.1);
	//glRasterPos2f(-150.0, 40.0);
	//ss_ = "¥ä:";
	//ss_2 << std::fixed<<std::setprecision(1)<<abs(dp*100);
	//ss_1 = "%";
	//ss_ = ss_ + ss_2.str()+ss_1;
	//drawString(ss_.c_str());
	//ss_2.str("");

	//glColor3d(0.1, 1.0, 0.1);
	//glRasterPos2f(-150.0, 20.0);
	//ss_ = "¥è:";
	//ss_2 << std::fixed<<std::setprecision(1)<<abs(tp*100);
	//ss_ = ss_ + ss_2.str()+ss_1;
	//drawString(ss_.c_str());
	//ss_2.str("");

	//glColor3d(0.1, 0.1, 1.0);
	//glRasterPos2f(-150.0, 0.0);
	//ss_ = "¥á:";
	//ss_2 << std::fixed<<std::setprecision(1)<<abs(ap*100);
	//ss_ = ss_ + ss_2.str()+ss_1;
	//drawString(ss_.c_str());
	//ss_2.str("");

	//glColor3d(1.0, 0.1, 1.0);
	//glRasterPos2f(-150.0, -20.0);
	//ss_ = "¥â:";
	//ss_2 << std::fixed<<std::setprecision(1)<<abs(bp*100);
	//ss_ = ss_ + ss_2.str()+ss_1;
	//drawString(ss_.c_str());
	//ss_2.str("");

	//glColor3d(0.2, 0.2, 0.5);
	//glRasterPos2f(-150.0, -40.0);
	//ss_ = "¥ã:";
	//ss_2 <<std::fixed<<std::setprecision(1)<<abs(gp*100);
	//ss_ = ss_ + ss_2.str()+ss_1;
	//drawString(ss_.c_str());
	//ss_2.str("");

	///*Low_success = 0.7;
	//Reward_success = 0.8;
	//High_success = 0.6;*/

	//glColor3d(0.0, 0.0, 0.0);
	//glRasterPos2f(160.0, 44.0);
	//ss_ = " L :";
	//ss_2 <<std::fixed<<std::setprecision(1)<<abs(Low_success*100);
	//ss_ = ss_ + ss_2.str()+ss_1;
	//drawString(ss_.c_str());
	//ss_2.str("");

	//glColor3d(0.0, 0.0, 0.0);
	//glRasterPos2f(160.0, 4.0);
	//ss_ = " R :";
	//ss_2 <<std::fixed<<std::setprecision(1)<<abs(Reward_success*100);
	//ss_ = ss_ + ss_2.str()+ss_1;
	//drawString(ss_.c_str());
	//ss_2.str("");

	//glColor3d(0.0, 0.0, 0.0);
	//glRasterPos2f(160.0, -36.0);
	//ss_ = " H :";
	//ss_2 <<std::fixed<<std::setprecision(1)<<abs(High_success*100);
	//ss_ = ss_ + ss_2.str()+ss_1;
	//drawString(ss_.c_str());
	//ss_2.str("");

	//glPushMatrix();
	//glDisable (GL_DEPTH_TEST);
	//glEnable(GL_BLEND);
	//glEnable(GL_POLYGON_SMOOTH);
	//glEnable(GL_LINE_SMOOTH);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glColor3d(1.0, 0.1, 0.1);
	//
	//glTranslatef(30.0, 10.0, 0.0);
	//glBegin(GL_TRIANGLE_FAN);
	//glVertex3f(0.0,0.0,0.0);
	//for(double i = 0; i < dd; i += 0.01)
	//	//glVertex3f(40.f*cos(i*2*Pi/1000.0), 40.f*sin(i*2*Pi/1000.0),0.0);
	//{
	//	//glVertex3f(45.f*cosf(i), 45.f*sinf(i),0.0);
	//	glVertex3f(65.f*cosf(i), 65.f*sinf(i),0.0);
	//}
	//glEnd();	
	//
	//glColor3d(0.1, 1.0, 0.1);
	//glBegin(GL_TRIANGLE_FAN);
	//glVertex3f(0.0,0.0,0.0);
	//for(double i = dd; i < tt;i += 0.01)
	//	//glVertex3f(40.f*cos(i*2*Pi/1000.0), 40.f*sin(i*2*Pi/1000.0),0.0);
	//	//glVertex3f(50.f*cosf(i), 50.f*sinf(i),0.0);
	//glVertex3f(65.f*cosf(i), 65.f*sinf(i),0.0);
	//glEnd();

	//glColor3d(0.1, 0.1, 1.0);
	//glBegin(GL_TRIANGLE_FAN);
	//glVertex3f(0.0,0.0,0.0);
	//for(double i = tt; i < aa; i += 0.01)
	//	//glVertex3f(40*cos(i*2*Pi/1000.0), 40*sin(i*2*Pi/1000.0),0.0);
	//	//glVertex3f(55.f*cosf(i), 55.f*sinf(i),0.0);
	//glVertex3f(65.f*cosf(i), 65.f*sinf(i),0.0);
	//glEnd();

	//glColor3d(1.0, 0.1, 1.0);
	//glBegin(GL_TRIANGLE_FAN);
	//glVertex3f(0.0,0.0,0.0);
	//for(double i = aa; i < bb; i += 0.01)
	//	//glVertex3f(40*cos(i*2*Pi/1000.0), 40*sin(i*2*Pi/1000.0),0.0);
	//	//glVertex3f(60.f*cosf(i), 60.f*sinf(i),0.0);
	//	glVertex3f(65.f*cosf(i), 65.f*sinf(i),0.0);
	//glEnd();

	//if(bb != 0)
	//{
	//	glColor3d(0.1, 0.4, 0.6);
	//	glBegin(GL_TRIANGLE_FAN);
	//	glVertex3f(0.0,0.0,0.0);
	//	for(double i = bb; i <= 2*Pi; i += 0.01)
	//		//glVertex3f(40*cos(i*2*Pi/1000.0), 40*sin(i*2*Pi/1000.0),0.0);
	//		glVertex3f(65.f*cosf(i), 65.f*sinf(i),0.0);
	//	glEnd();
	//}
	//
	////glColor3d(0.0, 0.0, 0.0);
	////glBegin(GL_LINE_STRIP);
	////for(double i = 0; i <= 2*Pi; i += 0.001)
	////	//glVertex3f(40*cos(i*2*Pi/1000.0), 40*sin(i*2*Pi/1000.0),0.0);
	////	glVertex3f(40.5f*cosf(i), 40.5f*sinf(i),0.0);
	////glEnd();
	//glEnable(GL_DEPTH_TEST); 
	//glDisable(GL_POLYGON_SMOOTH);
	//glDisable(GL_LINE_SMOOTH);
	//glDisable(GL_BLEND);
	//glPopMatrix();
	glPushMatrix();
	glTranslatef(X_Array_50[49],50,1.0);
	
		glPushMatrix();
		glTranslatef(200,40,0);
		glColor3d(236/255.0,63/255.0,49.0/255);
		glRasterPos2f(0.0, 0.0);
		ss_ = " Low Inhibit: ";
		ss_2 <<std::fixed<<std::setprecision(1)<<abs(Low_success*100);
		ss_ = ss_ + ss_2.str()+ss_1 +"%";
		drawString(ss_.c_str());
		ss_2.str("");

		//glColor3d(0.0, 0.0, 0.0);
		//glRasterPos2f(160.0, 4.0);
		//ss_ = " R :";
		//ss_2 <<std::fixed<<std::setprecision(1)<<abs(Reward_success*100);
		//ss_ = ss_ + ss_2.str()+ss_1;
		//drawString(ss_.c_str());
		//ss_2.str("");

		//glColor3d(0.0, 0.0, 0.0);
		//glRasterPos2f(160.0, -36.0);
		//ss_ = " H :";
		//ss_2 <<std::fixed<<std::setprecision(1)<<abs(High_success*100);
		//ss_ = ss_ + ss_2.str()+ss_1;
		//drawString(ss_.c_str());
		//ss_2.str("");
		glTranslatef(120,-5.0,0);

		glColor3d(0.0, 0.0, 0.0);
		glLineWidth(1);
		glBegin(GL_LINES);
		glVertex3f(0,0,0);
		glVertex3f(400,0,0);
		glVertex3f(400,0,0);
		glVertex3f(400,20,0);
		glVertex3f(400,20,0);
		glVertex3f(0,20,0);
		glVertex3f(0,20,0);
		glVertex3f(0,0,0);
		glEnd();
	
		if(Low_success != 0)
		{
			glLineWidth(10);
			glColor3d(236/255.0,63/255.0,49.0/255);
			glBegin(GL_LINES);
			glVertex3f(0,5,0);
			glVertex3f(Low_success*400,5,0);
			glVertex3f(0,15,0);
			glVertex3f(Low_success*400,15,0);
			glEnd();
		}
		glPopMatrix();


	
		glPushMatrix();
		glTranslatef(200,0,0);
		glColor3d(30/255.0,155/255.0,151/255);
		glRasterPos2f(0.0, 0.0);
		ss_ = "    Reward: ";
		ss_2 <<std::fixed<<std::setprecision(1)<<abs(Reward_success*100);
		ss_ = ss_ + ss_2.str()+ss_1 +"%";
		drawString(ss_.c_str());
		ss_2.str("");

		glTranslatef(120,-5.0,0);
		glColor3d(0.0, 0.0, 0.0);
		glLineWidth(1);
		glBegin(GL_LINES);
		glVertex3f(0,0,0);
		glVertex3f(400,0,0);
		glVertex3f(400,0,0);
		glVertex3f(400,20,0);
		glVertex3f(400,20,0);
		glVertex3f(0,20,0);
		glVertex3f(0,20,0);
		glVertex3f(0,0,0);

		glColor3d(0.0, 0.75, 0.0);
		glVertex3f(-130,-10,0);
		glVertex3f(420,-10,0);
		glVertex3f(420,-10,0);
		glVertex3f(420,30,0);
		glVertex3f(420,30,0);
		glVertex3f(-130,30,0);
		glVertex3f(-130,30,0);
		glVertex3f(-130,-10,0);

		glEnd();
		if(Reward_success != 0)
		{
			glLineWidth(10);
			glColor3d(30/255.0,155/255.0,151/255);
			glBegin(GL_LINES);
			glVertex3f(0,5,0);
			glVertex3f(Reward_success*400,5,0);
			glVertex3f(0,15,0);
			glVertex3f(Reward_success*400,15,0);
			glEnd();
		}
		glPopMatrix();
	

	
		glPushMatrix();
		glTranslatef(200,-40,0);
		glColor3d(76/255.0,89/255.0,158/255);
		glRasterPos2f(0.0, 0.0);
		ss_ = " High Inhibit: ";
		ss_2 <<std::fixed<<std::setprecision(1)<<abs(High_success*100);
		ss_ = ss_ + ss_2.str()+ss_1 +"%";
		drawString(ss_.c_str());
		ss_2.str("");

		glTranslatef(120,-5.0,0);
		glColor3d(0.0, 0.0, 0.0);
		glLineWidth(1);
		glBegin(GL_LINES);
		glVertex3f(0,0,0);
		glVertex3f(400,0,0);
		glVertex3f(400,0,0);
		glVertex3f(400,20,0);
		glVertex3f(400,20,0);
		glVertex3f(0,20,0);
		glVertex3f(0,20,0);
		glVertex3f(0,0,0);
		glEnd();

		if(High_success != 0)
		{
			glLineWidth(10);
				glColor3d(76/255.0,89/255.0,158/255);
			glBegin(GL_LINES);
			glVertex3f(0,5,0);
			glVertex3f(High_success*400,5,0);
			glVertex3f(0,15,0);
			glVertex3f(High_success*400,15,0);
			glEnd();
		}
		glPopMatrix();
	

	glPopMatrix();
	glPopMatrix();

	//glEnable(GL_DEPTH_TEST); 
	//glDisable(GL_ALPHA_TEST);
	//glDisable(GLUT_MULTISAMPLE);
	//glDisable(GL_POLYGON_SMOOTH);
	//glDisable(GL_POLYGON_SMOOTH_HINT);
	//glDisable(GL_BLEND);
	//glFlush();
}
void Spectrum::DisplayAmplitudeSpectrumTLC()
{
	stringstream ss_2 (stringstream::in | stringstream::out);
	string ss_,ss_1;
	double max_aaa = 0;
	int max_as_indx =0;
	for(int j =0;j<Samples_Hz;j++)
	{
		if(Y_Array_AS_50[j] > max_aaa)
		{
			max_aaa = Y_Array_AS_50[j];
			max_as_indx = j;
		}
	}

	glPushMatrix();
	glTranslatef(org.x,org.y,-0.5);

	glPushMatrix();
	glColor3d(1.0, 0.0, 0.0);
	glRasterPos2f(10.0, 120.0);
	//drawString(SensorPosName.c_str());
	glPopMatrix();

	if(!IsChanel2)
	{
		glBegin(GL_QUADS);
		for(int j =0;j<Samples_Hz;j++)	
		{
			if(j < 10)glColor3d(0.8*1.0, (double)(j)*0.8/10.0, 0.0);
			else if( j>= 10 && j< 20)glColor3d(1.0*0.8 - (double)(j - 10)*0.8/10.0, 1.0*0.8, 0.0);
			else if( j>= 20 && j< 30)glColor3d(0.0, 1.0*0.8 ,(double)(j+1)*0.8/30.0);
			else if( j>= 30 && j< 40)glColor3d(0.0, 1.0*0.8 - (double)(j - 30)*0.8/10.0,1.0*0.8 );
			else if( j>= 40 && j< 50)glColor3d((double)(j-39)*0.8/10.0, 0.0, 1.0*0.8);

			glVertex3f(X_Array_50[j], 0.0, 1.0); 
			glVertex3f(X_Array_50[j] + 15, 0.0, 1.0); 

			if(j < 10)glColor3d(1.0, (double)(j)/10.0, 0.0);
			else if( j>= 10 && j< 20)glColor3d(1.0 - (double)(j - 10)/10.0, 1.0, 0.0);
			else if( j>= 20 && j< 30)glColor3d(0.0, 1.0 ,(double)(j+1)/30.0);
			else if( j>= 30 && j< 40)glColor3d(0.0, 1.0 - (double)(j - 30)/10.0,1.0 );
			else if( j>= 40 && j< 50)glColor3d((double)(j-39)/10.0, 0.0, 1.0);
			if(Y_Array_AS_50[j] < 55)
			{
				glVertex3f(X_Array_50[j] + 15, Y_Array_AS_50[j]*AMPSCAL, 1.0);
				glVertex3f(X_Array_50[j] , Y_Array_AS_50[j]*AMPSCAL, 1.0);
			}
			else
			{
				glVertex3f(X_Array_50[j] + 15, 110, 1.0);
				glVertex3f(X_Array_50[j], 110, 1.0);
			}
		}
		glEnd();
	}
	else
	{
		glBegin(GL_QUADS);
		for(int j =0;j<Samples_Hz;j++)	
		{
			if(j < 10)glColor3d(0.8*1.0, (double)(j)*0.8/10.0, 0.0);
			else if( j>= 10 && j< 20)glColor3d(1.0*0.8 - (double)(j - 10)*0.8/10.0, 1.0*0.8, 0.0);
			else if( j>= 20 && j< 30)glColor3d(0.0, 1.0*0.8 ,(double)(j+1)*0.8/30.0);
			else if( j>= 30 && j< 40)glColor3d(0.0, 1.0*0.8 - (double)(j - 30)*0.8/10.0,1.0*0.8 );
			else if( j>= 40 && j< 50)glColor3d((double)(j-39)*0.8/10.0, 0.0, 1.0*0.8);

			glVertex3f(X_Array_50[j], 0.0, 1.0); 
			glVertex3f(X_Array_50[j] + 15, 0.0, 1.0); 

			if(j < 10)glColor3d(1.0, (double)(j)/10.0, 0.0);
			else if( j>= 10 && j< 20)glColor3d(1.0 - (double)(j - 10)/10.0, 1.0, 0.0);
			else if( j>= 20 && j< 30)glColor3d(0.0, 1.0 ,(double)(j+1)/30.0);
			else if( j>= 30 && j< 40)glColor3d(0.0, 1.0 - (double)(j - 30)/10.0,1.0 );
			else if( j>= 40 && j< 50)glColor3d((double)(j-39)/10.0, 0.0, 1.0);
			if(Y_Array_AS_50[j] < 55)
			{
				glVertex3f(X_Array_50[j] + 15, -1*Y_Array_AS_50[j]*AMPSCAL, 1.0);
				glVertex3f(X_Array_50[j] , -1*Y_Array_AS_50[j]*AMPSCAL, 1.0);
			}
			else
			{
				glVertex3f(X_Array_50[j] + 15, -1*110, 1.0);
				glVertex3f(X_Array_50[j], -1*110, 1.0);
			}
		}
		glEnd();
	}
	glPopMatrix();

}
//void Spectrum::DisplayAmplitudeSpectrumTLC()
//{
//	stringstream ss_2 (stringstream::in | stringstream::out);
//	string ss_,ss_1;
//	double max_aaa = 0;
//	int max_as_indx =0;
//	for(int j =0;j<Samples_Hz;j++)
//	{
//		if(Y_Array_AS_50[j] > max_aaa)
//		{
//			max_aaa = Y_Array_AS_50[j];
//			max_as_indx = j;
//		}
//	}
//		
//	/*glColor3d(0.1, 0.1, 0.2);
//	glBegin(GL_QUADS);
//	glVertex3d(-200.0, height,-0.1);
//	glVertex3d(-200, 0,-0.1);
//	glVertex3d(0, 0,-0.1);
//	glVertex3d(0, height,-0.1);
//	glEnd();*/
//
//	glPushMatrix();
//	glTranslatef(org.x,org.y,-0.5);
//
//	glPushMatrix();
//	glColor3d(1.0, 0.0, 0.0);
//	glRasterPos2f(10.0, 120.0);
//	//drawString(SensorPosName.c_str());
//	glPopMatrix();
//
//	if(!IsChanel2)
//	{
//	/////////////////////AMP current max///////////
//		glPushMatrix();
//		if(max_as_indx < 4)glColor3d(1.0, 0.0, 0.0);
//		else if( max_as_indx >= 4 && max_as_indx <= 7)glColor3d(0.0, 1.0, 0.0);
//		else if( max_as_indx >= 8 && max_as_indx <= 14)glColor3d(0.0, 0.0, 1.0);
//		else if( max_as_indx >= 15 && max_as_indx <= 32)glColor3d(1.0, 0.0, 1.0);
//		else glColor3d(0.1, 0.4, 0.6);
//		glRasterPos2f(X_Array_50[max_as_indx], Y_Array_AS_50[max_as_indx]*AMPSCAL);
//		ss_ = "uV";
//		
//		int as_ = (int)(Y_Array_AS_50[max_as_indx]);
//		ss_2 << as_;
//		ss_ = ss_2.str() + ss_;
//		//drawString(ss_.c_str());
//		ss_2.str("");
//		glPopMatrix();
//		/////////////////////AMP current max///////////
//
//		glLineWidth(10.0);
//		glBegin( GL_LINES );
//		for(int j =0;j<Samples_Hz;j++)	
//		{
//			if(j < 4)glColor3d(1.0, 0.0, 0.0);
//			else if( j>= 4 && j<= 7)glColor3d(0.0, 1.0, 0.0);
//			else if( j>= 8 && j<= 14)glColor3d(0.0, 0.0, 1.0);
//			else if( j>= 15 && j<= 32)glColor3d(1.0, 0.0, 1.0);
//			else glColor3d(0.1, 0.4, 0.6);
//
//			glVertex3f(X_Array_50[j], 0.0, 1.0); 
//			if(Y_Array_AS_50[j] < 55)
//				glVertex3f(X_Array_50[j], Y_Array_AS_50[j]*AMPSCAL, 1.0);
//			else
//				glVertex3f(X_Array_50[j], 110, 1.0);
//		}
//		glEnd();
//	}
//	else
//	{
//		/////////////////////AMP current max///////////
//		glPushMatrix();
//		if(max_as_indx < 4)glColor3d(1.0, 0.0, 0.0);
//		else if( max_as_indx >= 4 && max_as_indx <= 7)glColor3d(0.0, 1.0, 0.0);
//		else if( max_as_indx >= 8 && max_as_indx <= 14)glColor3d(0.0, 0.0, 1.0);
//		else if( max_as_indx >= 15 && max_as_indx <= 32)glColor3d(1.0, 0.0, 1.0);
//		else glColor3d(0.1, 0.4, 0.6);
//		glRasterPos2f(X_Array_50[max_as_indx],-1*Y_Array_AS_50[max_as_indx]*AMPSCAL);
//		ss_ = "uV";
//
//		int as_ = (int)(Y_Array_AS_50[max_as_indx]);
//		ss_2 << as_;
//		ss_ = ss_2.str() + ss_;
//		//drawString(ss_.c_str());
//		ss_2.str("");
//		glPopMatrix();
//		/////////////////////AMP current max///////////
//
//		glLineWidth(10.0);
//		glBegin( GL_LINES );
//		for(int j =0;j<Samples_Hz;j++)	
//		{
//			if(j < 4)glColor3d(1.0, 0.0, 0.0);
//			else if( j>= 4 && j<= 7)glColor3d(0.0, 1.0, 0.0);
//			else if( j>= 8 && j<= 14)glColor3d(0.0, 0.0, 1.0);
//			else if( j>= 15 && j<= 32)glColor3d(1.0, 0.0, 1.0);
//			else glColor3d(0.1, 0.4, 0.6);
//
//			glVertex3f(X_Array_50[j], 0.0, 1.0); 
//			if(Y_Array_AS_50[j] < 55)
//				glVertex3f(X_Array_50[j], -1*Y_Array_AS_50[j]*AMPSCAL, 1.0);
//			else
//				glVertex3f(X_Array_50[j], -1*110, 1.0);
//		}
//		glEnd();
//	}
//
//	
//	glLineWidth(1.0);
//
//	glPushMatrix();//y
//	//glTranslatef(105.0, 130.0, 1.0);
//	if(!IsChanel2)
//		glTranslatef(width*0.7,-15 , 1.0);
//	else
//		glTranslatef(width*0.78, -15+0.02*height, 1.0);
//
//	if(SumOfAS == 0.0)SumOfAS = 0.0001;
//	double dp = derta_sum/SumOfAS;
//	double tp = theta_sum/SumOfAS;
//	double ap = alpha_sum/SumOfAS;
//	double bp = beta_sum/SumOfAS;
//	double gp = gamma_sum/SumOfAS;
//
//	
//	/*int dd = (int)(1000*dp);
//	int tt = dd + (int)(1000*tp);
//	int aa = tt + (int)(1000*ap);
//	int bb = aa + (int)(1000*bp);
//	int gg = bb + (int)(1000*gp);*/
//	double dd = (2*Pi*dp);
//	double tt = dd + (2*Pi*tp);
//	double aa = tt + (2*Pi*ap);
//	double bb = aa + (2*Pi*bp);
//	double gg = bb + (2*Pi*gp);
//
//	glColor3d(1.0, 0.1, 0.1);
//	glRasterPos2f(-150.0, 40.0);
//	if(!IsChanel2)
//		ss_ = "¥ä:";
//	else
//		ss_ = " ¥ä:";
//	ss_2 << std::fixed<<std::setprecision(1)<<abs(dp*100);
//	ss_1 = "%";
//	ss_ = ss_ + ss_2.str()+ss_1;
//	drawString(ss_.c_str());
//	ss_2.str("");
//
//	glColor3d(0.1, 1.0, 0.1);
//	glRasterPos2f(-150.0, 20.0);
//	ss_ = "¥è:";
//	ss_2 << std::fixed<<std::setprecision(1)<<abs(tp*100);
//	ss_ = ss_ + ss_2.str()+ss_1;
//	drawString(ss_.c_str());
//	ss_2.str("");
//
//	glColor3d(0.1, 0.1, 1.0);
//	glRasterPos2f(-150.0, 0.0);
//	ss_ = "¥á:";
//	ss_2 << std::fixed<<std::setprecision(1)<<abs(ap*100);
//	ss_ = ss_ + ss_2.str()+ss_1;
//	drawString(ss_.c_str());
//	ss_2.str("");
//
//	glColor3d(1.0, 0.1, 1.0);
//	glRasterPos2f(-150.0, -20.0);
//	ss_ = "¥â:";
//	ss_2 << std::fixed<<std::setprecision(1)<<abs(bp*100);
//	ss_ = ss_ + ss_2.str()+ss_1;
//	drawString(ss_.c_str());
//	ss_2.str("");
//
//	glColor3d(0.2, 0.2, 0.5);
//	glRasterPos2f(-150.0, -40.0);
//	ss_ = "¥ã:";
//	ss_2 <<std::fixed<<std::setprecision(1)<<abs(gp*100);
//	ss_ = ss_ + ss_2.str()+ss_1;
//	drawString(ss_.c_str());
//	ss_2.str("");
//
//	glPushMatrix();
//	glDisable (GL_DEPTH_TEST);
//	glEnable(GL_BLEND);
//	glEnable(GL_POLYGON_SMOOTH);
//	glEnable(GL_LINE_SMOOTH);
//	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//	glColor3d(1.0, 0.1, 0.1);
//	
//	if(!IsChanel2)
//		glTranslatef(-width*0.155, 5.0, 0.0);
//	else
//		glTranslatef(30.0, 5.0, 0.0);
//
//	glBegin(GL_TRIANGLE_FAN);
//	glVertex3f(0.0,0.0,0.0);
//	for(double i = 0; i < dd; i += 0.01)
//		//glVertex3f(40.f*cos(i*2*Pi/1000.0), 40.f*sin(i*2*Pi/1000.0),0.0);
//	{
//		//glVertex3f(40.f*cosf(i), 40.f*sinf(i),0.0);
//		glVertex3f(80.f*cosf(i), 80.f*sinf(i),0.0);
//	}
//	glEnd();	
//	
//	glColor3d(0.1, 1.0, 0.1);
//	glBegin(GL_TRIANGLE_FAN);
//	glVertex3f(0.0,0.0,0.0);
//	for(double i = dd; i < tt;i += 0.01)
//		//glVertex3f(40.f*cos(i*2*Pi/1000.0), 40.f*sin(i*2*Pi/1000.0),0.0);
//		//glVertex3f(50.f*cosf(i), 50.f*sinf(i),0.0);
//		glVertex3f(80.f*cosf(i), 80.f*sinf(i),0.0);
//	glEnd();
//
//	glColor3d(0.1, 0.1, 1.0);
//	glBegin(GL_TRIANGLE_FAN);
//	glVertex3f(0.0,0.0,0.0);
//	for(double i = tt; i < aa; i += 0.01)
//		//glVertex3f(40*cos(i*2*Pi/1000.0), 40*sin(i*2*Pi/1000.0),0.0);
//		//glVertex3f(60.f*cosf(i), 60.f*sinf(i),0.0);
//		glVertex3f(80.f*cosf(i), 80.f*sinf(i),0.0);
//	glEnd();
//
//	glColor3d(1.0, 0.1, 1.0);
//	glBegin(GL_TRIANGLE_FAN);
//	glVertex3f(0.0,0.0,0.0);
//	for(double i = aa; i < bb; i += 0.01)
//		//glVertex3f(40*cos(i*2*Pi/1000.0), 40*sin(i*2*Pi/1000.0),0.0);
//		//glVertex3f(70.f*cosf(i), 70.f*sinf(i),0.0);
//		glVertex3f(80.f*cosf(i), 80.f*sinf(i),0.0);
//	glEnd();
//
//	glColor3d(0.1, 0.4, 0.6);
//	if(bb != 0)
//	{
//		glBegin(GL_TRIANGLE_FAN);
//		glVertex3f(0.0,0.0,0.0);
//		for(double i = bb; i <= 2*Pi; i += 0.01)
//			//glVertex3f(40*cos(i*2*Pi/1000.0), 40*sin(i*2*Pi/1000.0),0.0);
//			glVertex3f(80.f*cosf(i), 80.f*sinf(i),0.0);
//		glEnd();
//	}
//	
//	glTranslatef(0,-90,0);
//	glRasterPos2f(-30.0, -10.0);
//	if(!IsChanel2)
//		drawString("EEG Singal 1");
//
//	else
//		drawString("EEG Singal 2");
//
//
//	//glColor3d(0.0, 0.0, 0.0);
//	//glBegin(GL_LINE_STRIP);
//	//for(double i = 0; i <= 2*Pi; i += 0.001)
//	//	//glVertex3f(40*cos(i*2*Pi/1000.0), 40*sin(i*2*Pi/1000.0),0.0);
//	//	glVertex3f(40.5f*cosf(i), 40.5f*sinf(i),0.0);
//	//glEnd();
//	glEnable(GL_DEPTH_TEST); 
//	glDisable(GL_POLYGON_SMOOTH);
//	glDisable(GL_LINE_SMOOTH);
//	glDisable(GL_BLEND);
//	glPopMatrix();
//	
//	//glPushMatrix();
//	//glTranslatef(140,40,0);
//
//	//glColor3d(0.0, 0.0, 0.0);
//	//glLineWidth(1);
//	//glBegin(GL_LINES);
//	//glVertex3f(0,0,0);
//	//glVertex3f(150,0,0);
//	//glVertex3f(150,0,0);
//	//glVertex3f(150,20,0);
//	//glVertex3f(150,20,0);
//	//glVertex3f(0,20,0);
//	//glVertex3f(0,20,0);
//	//glVertex3f(0,0,0);
//	//glEnd();
//	//glLineWidth(10);
//	//glColor3d(0.0, 1.0, 0.1);
//	//glBegin(GL_LINES);
//	//glVertex3f(0,5,0);
//	//glVertex3f(Low_success*150,5,0);
//	//glVertex3f(0,15,0);
//	//glVertex3f(Low_success*150,15,0);
//	//glEnd();
//
//	//glPopMatrix();
//
//
//	glPopMatrix();
//	glPopMatrix();
//
//	//glEnable(GL_DEPTH_TEST); 
//	//glDisable(GL_ALPHA_TEST);
//	//glDisable(GLUT_MULTISAMPLE);
//	//glDisable(GL_POLYGON_SMOOTH);
//	//glDisable(GL_POLYGON_SMOOTH_HINT);
//	//glDisable(GL_BLEND);
//	//glFlush();
//}

void Spectrum::DisplayPowerSpectrum2()
{
	stringstream ss_2 (stringstream::in | stringstream::out);
	string ss_;
	double max_aaa = 0;
	int max_ps_indx =0;
	for(int j =0;j<Samples_Hz;j++)
		if(Y_Array_PS_50[j] > max_aaa)
		{
			max_aaa = Y_Array_PS_50[j];
			max_ps_indx = j;
		}
	glPushMatrix();
	glTranslatef(org.x,org.y,0);

	/////////////////////AMP current max///////////
	glPushMatrix();
	glColor3d(0.0, 1.0, 1.0);
	//glRasterPos2f(X_Array[max_ps_indx], 0.05*Y_Array_PS[max_ps_indx]);
	glRasterPos2f(X_Array_50[max_ps_indx], Y_Array_PS_50[max_ps_indx]*AMPSCAL);
	ss_ = "dB";
	int ps_ = (int)Y_Array_PS_50[max_ps_indx];
	if(ps_ >= 0)
	{
		//ss_2 << (int)(327.67*Y_Array_AS[max_as_indx]);//new 303 uv 
		ss_2 << ps_;//new 303 uv
		ss_ = "         ->"+ss_2.str() + ss_;
	}
	else
	{
		ss_2 << abs(ps_);//new 303 uv
		ss_ = "-"+ss_2.str() + ss_;
	}
	
	//drawString(ss_.c_str());
	ss_2.str("");
	glPopMatrix();
	/////////////////////AMP current max///////////
	glPopMatrix();

	glPushMatrix();
	glTranslatef(org.x,org.y,0);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_LINE_SMOOTH);
	glLineWidth(1.0);
	glBegin( GL_LINE_STRIP );
	glColor3d(0.0, 1.0, 1.0);

	//for(int j =0;j<Samples_Hz;j++)	
		//glVertex3f(X_Array_50[j], Y_Array_PS_50[j], 0.0); 
		
	for(int j =0;j<Samples_Hz;j++)
	{
		if(Y_Array_AS_50[j] < 55)
		{
			if(Y_Array_PS_50[j]*AMPSCAL < -10)
				glVertex3f(X_Array_50[j], -10, 0.0);
			else
				glVertex3f(X_Array_50[j], Y_Array_PS_50[j]*AMPSCAL, 0.0);
		}
		else
			glVertex3f(X_Array_50[j], 110, 0.0);
	}
	

	glEnd();
	glLineWidth(1.0);

	glBegin( GL_LINES );
	glColor3d(0.0, 0.0, 0.0);
	glVertex3f(0, 0.0, 0.0); 
	glVertex3f(450, 0.0, 0.0);
	glVertex3f(0, 0.0, 0.0); 
	glVertex3f(0, 120.0, 0.0);
	glEnd();

	glRasterPos2f(0.0, -15.0);
	drawString("0hz");

	glPushMatrix();//x
	glTranslatef(450,0,0);
	glRasterPos2f(-15.0, -15.0);
	drawString("50hz");
	//glTranslatef(0.0f, 0.0, 0.0);
	glBegin(GL_TRIANGLES);
	glVertex3f(0.0,5.0,0.0);
	glVertex3f(0.0,-5.0,0.0);
	glVertex3f(14.1f,0.0,0.0);
	glEnd();
	glPopMatrix();

	glPushMatrix();//y
	glTranslatef(0,120,0);
	glBegin(GL_TRIANGLES);
	glVertex3f(-5.0,0.0,0.0);
	glVertex3f(5.0,0.0,0.0);
	glVertex3f(0.0,14.1f,0.0);
	glEnd();
	glPopMatrix();

	glPopMatrix();
	glDisable(GL_LINE_SMOOTH);
	glDisable(GL_BLEND);
}

void Spectrum::DisplayAmplitudeSpectrum2()
{
	stringstream ss_2 (stringstream::in | stringstream::out);
	string ss_,ss_1;
	double max_aaa = 0;
	int max_as_indx =0;
	for(int j =0;j<Samples_Hz;j++)
	{
		if(Y_Array_AS_50[j] > max_aaa)
		{
			max_aaa = Y_Array_AS_50[j];
			max_as_indx = j;
		}
	}
		
	/*glColor3d(0.1, 0.1, 0.2);
	glBegin(GL_QUADS);
	glVertex3d(-200.0, height,-0.1);
	glVertex3d(-200, 0,-0.1);
	glVertex3d(0, 0,-0.1);
	glVertex3d(0, height,-0.1);
	glEnd();*/

	glPushMatrix();
	glTranslatef(org.x,org.y,-0.5);

	glPushMatrix();
	glColor3d(1.0, 0.0, 0.0);
	glRasterPos2f(10.0, 120.0);
	//drawString(SensorPosName.c_str());
	glPopMatrix();

	/////////////////////AMP current max///////////
	glPushMatrix();
	if(max_as_indx < 4)glColor3d(1.0, 0.0, 0.0);
	else if( max_as_indx >= 4 && max_as_indx <= 7)glColor3d(0.0, 1.0, 0.0);
	else if( max_as_indx >= 8 && max_as_indx <= 14)glColor3d(0.0, 0.0, 1.0);
	else if( max_as_indx >= 15 && max_as_indx <= 32)glColor3d(1.0, 0.0, 1.0);
	else glColor3d(0.1, 0.4, 0.6);
	glRasterPos2f(X_Array_50[max_as_indx], Y_Array_AS_50[max_as_indx]*AMPSCAL);
	ss_ = "uV";
	
	int as_ = (int)(Y_Array_AS_50[max_as_indx]);
	ss_2 << as_;
	ss_ = ss_2.str() + ss_;
	//drawString(ss_.c_str());
	ss_2.str("");
	glPopMatrix();
	/////////////////////AMP current max///////////

	glLineWidth(5.0);
	glBegin( GL_LINES );
	for(int j =0;j<Samples_Hz;j++)	
	{
		if(j < 4)glColor3d(1.0, 0.0, 0.0);
		else if( j>= 4 && j<= 7)glColor3d(0.0, 1.0, 0.0);
		else if( j>= 8 && j<= 14)glColor3d(0.0, 0.0, 1.0);
		else if( j>= 15 && j<= 32)glColor3d(1.0, 0.0, 1.0);
		else glColor3d(0.1, 0.4, 0.6);

		glVertex3f(X_Array_50[j], 0.0, 1.0); 
		if(Y_Array_AS_50[j] < 55)
			glVertex3f(X_Array_50[j], Y_Array_AS_50[j]*AMPSCAL, 1.0);
		else
			glVertex3f(X_Array_50[j], 110, 1.0);
	}
	glEnd();
	glLineWidth(1.0);

	glPushMatrix();//y
	//glTranslatef(105.0, 130.0, 1.0);
	glTranslatef(width*0.61, 40.0, 1.0);
	if(SumOfAS == 0.0)SumOfAS = 0.0001;
	double dp = derta_sum/SumOfAS;
	double tp = theta_sum/SumOfAS;
	double ap = alpha_sum/SumOfAS;
	double bp = beta_sum/SumOfAS;
	double gp = gamma_sum/SumOfAS;

	
	/*int dd = (int)(1000*dp);
	int tt = dd + (int)(1000*tp);
	int aa = tt + (int)(1000*ap);
	int bb = aa + (int)(1000*bp);
	int gg = bb + (int)(1000*gp);*/
	double dd = (2*Pi*dp);
	double tt = dd + (2*Pi*tp);
	double aa = tt + (2*Pi*ap);
	double bb = aa + (2*Pi*bp);
	double gg = bb + (2*Pi*gp);

	glPushMatrix();
	glTranslatef(0, 10.0, 0.0);

	glColor3d(1.0, 0.1, 0.1);
	glRasterPos2f(-150.0, 40.0);
	ss_ = "¥ä:";
	ss_2 << std::fixed<<std::setprecision(1)<<abs(dp*100);
	ss_1 = "%";
	ss_ = ss_ + ss_2.str()+ss_1;
	drawString(ss_.c_str());
	ss_2.str("");

	glColor3d(0.1, 1.0, 0.1);
	glRasterPos2f(-150.0, 20.0);
	ss_ = "¥è:";
	ss_2 << std::fixed<<std::setprecision(1)<<abs(tp*100);
	ss_ = ss_ + ss_2.str()+ss_1;
	drawString(ss_.c_str());
	ss_2.str("");

	glColor3d(0.1, 0.1, 1.0);
	glRasterPos2f(-150.0, 0.0);
	ss_ = "¥á:";
	ss_2 << std::fixed<<std::setprecision(1)<<abs(ap*100);
	ss_ = ss_ + ss_2.str()+ss_1;
	drawString(ss_.c_str());
	ss_2.str("");

	glColor3d(1.0, 0.1, 1.0);
	glRasterPos2f(-150.0, -20.0);
	ss_ = "¥â:";
	ss_2 << std::fixed<<std::setprecision(1)<<abs(bp*100);
	ss_ = ss_ + ss_2.str()+ss_1;
	drawString(ss_.c_str());
	ss_2.str("");

	glColor3d(0.2, 0.2, 0.5);
	glRasterPos2f(-150.0, -40.0);
	ss_ = "¥ã:";
	ss_2 <<std::fixed<<std::setprecision(1)<<abs(gp*100);
	ss_ = ss_ + ss_2.str()+ss_1;
	drawString(ss_.c_str());
	ss_2.str("");

	glPopMatrix();
	/*Low_success = 0.7;
	Reward_success = 0.8;
	High_success = 0.6;*/

	glPushMatrix();
	glTranslatef(-40,0,0);
	glColor3d(0.0, 0.0, 0.0);
	glRasterPos2f(160.0, 44.0);
	ss_ = " L :";
	ss_2 <<std::fixed<<std::setprecision(1)<<abs(Low_success*100);
	ss_ = ss_ + ss_2.str()+ss_1;
	drawString(ss_.c_str());
	ss_2.str("");

	glColor3d(0.0, 0.0, 0.0);
	glRasterPos2f(160.0, 4.0);
	ss_ = " R :";
	ss_2 <<std::fixed<<std::setprecision(1)<<abs(Reward_success*100);
	ss_ = ss_ + ss_2.str()+ss_1;
	drawString(ss_.c_str());
	ss_2.str("");

	glColor3d(0.0, 0.0, 0.0);
	glRasterPos2f(160.0, -36.0);
	ss_ = " H :";
	ss_2 <<std::fixed<<std::setprecision(1)<<abs(High_success*100);
	ss_ = ss_ + ss_2.str()+ss_1;
	drawString(ss_.c_str());
	ss_2.str("");

	glPushMatrix();
	glDisable (GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glEnable(GL_POLYGON_SMOOTH);
	glEnable(GL_LINE_SMOOTH);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor3d(1.0, 0.1, 0.1);
	
	glTranslatef(30.0, 10.0, 0.0);
	glBegin(GL_TRIANGLE_FAN);
	glVertex3f(0.0,0.0,0.0);
	for(double i = 0; i < dd; i += 0.01)
		//glVertex3f(40.f*cos(i*2*Pi/1000.0), 40.f*sin(i*2*Pi/1000.0),0.0);
	{
		//glVertex3f(45.f*cosf(i), 45.f*sinf(i),0.0);
		glVertex3f(65.f*cosf(i), 65.f*sinf(i),0.0);
	}
	glEnd();	
	
	glColor3d(0.1, 1.0, 0.1);
	glBegin(GL_TRIANGLE_FAN);
	glVertex3f(0.0,0.0,0.0);
	for(double i = dd; i < tt;i += 0.01)
		//glVertex3f(40.f*cos(i*2*Pi/1000.0), 40.f*sin(i*2*Pi/1000.0),0.0);
		//glVertex3f(50.f*cosf(i), 50.f*sinf(i),0.0);
		glVertex3f(65.f*cosf(i), 65.f*sinf(i),0.0);
	glEnd();

	glColor3d(0.1, 0.1, 1.0);
	glBegin(GL_TRIANGLE_FAN);
	glVertex3f(0.0,0.0,0.0);
	for(double i = tt; i < aa; i += 0.01)
		//glVertex3f(40*cos(i*2*Pi/1000.0), 40*sin(i*2*Pi/1000.0),0.0);
		//glVertex3f(55.f*cosf(i), 55.f*sinf(i),0.0);
	glVertex3f(65.f*cosf(i), 65.f*sinf(i),0.0);
	glEnd();

	glColor3d(1.0, 0.1, 1.0);
	glBegin(GL_TRIANGLE_FAN);
	glVertex3f(0.0,0.0,0.0);
	for(double i = aa; i < bb; i += 0.01)
		//glVertex3f(40*cos(i*2*Pi/1000.0), 40*sin(i*2*Pi/1000.0),0.0);
		//glVertex3f(60.f*cosf(i), 60.f*sinf(i),0.0);
		glVertex3f(65.f*cosf(i), 65.f*sinf(i),0.0);
	glEnd();

	if(bb != 0)
	{
		glColor3d(0.1, 0.4, 0.6);
		glBegin(GL_TRIANGLE_FAN);
		glVertex3f(0.0,0.0,0.0);
		for(double i = bb; i <= 2*Pi; i += 0.01)
			//glVertex3f(40*cos(i*2*Pi/1000.0), 40*sin(i*2*Pi/1000.0),0.0);
			glVertex3f(65.f*cosf(i), 65.f*sinf(i),0.0);
		glEnd();
	}
	
	//glColor3d(0.0, 0.0, 0.0);
	//glBegin(GL_LINE_STRIP);
	//for(double i = 0; i <= 2*Pi; i += 0.001)
	//	//glVertex3f(40*cos(i*2*Pi/1000.0), 40*sin(i*2*Pi/1000.0),0.0);
	//	glVertex3f(40.5f*cosf(i), 40.5f*sinf(i),0.0);
	//glEnd();
	glEnable(GL_DEPTH_TEST); 
	glDisable(GL_POLYGON_SMOOTH);
	glDisable(GL_LINE_SMOOTH);
	glDisable(GL_BLEND);
	glPopMatrix();
	if(Low_success != 0)
	{
		glPushMatrix();
		glTranslatef(140,40,0);

		glColor3d(0.0, 0.0, 0.0);
		glLineWidth(1);
		glBegin(GL_LINES);
		glVertex3f(0,0,0);
		glVertex3f(150,0,0);
		glVertex3f(150,0,0);
		glVertex3f(150,20,0);
		glVertex3f(150,20,0);
		glVertex3f(0,20,0);
		glVertex3f(0,20,0);
		glVertex3f(0,0,0);
		glEnd();
		glLineWidth(10);
		glColor3d(0.0, 1.0, 0.1);
		glBegin(GL_LINES);
		glVertex3f(0,5,0);
		glVertex3f(Low_success*150,5,0);
		glVertex3f(0,15,0);
		glVertex3f(Low_success*150,15,0);
		glEnd();

		glPopMatrix();
	}

	if(Reward_success != 0)
	{
		glPushMatrix();
		glTranslatef(140,0,0);

		glColor3d(0.0, 0.0, 0.0);
		glLineWidth(1);
		glBegin(GL_LINES);
		glVertex3f(0,0,0);
		glVertex3f(150,0,0);
		glVertex3f(150,0,0);
		glVertex3f(150,20,0);
		glVertex3f(150,20,0);
		glVertex3f(0,20,0);
		glVertex3f(0,20,0);
		glVertex3f(0,0,0);
		glEnd();
		glLineWidth(10);
		glColor3d(0.0, 1.0, 0.1);
		glBegin(GL_LINES);
		glVertex3f(0,5,0);
		glVertex3f(Reward_success*150,5,0);
		glVertex3f(0,15,0);
		glVertex3f(Reward_success*150,15,0);
		glEnd();

		glPopMatrix();
	}

	if(High_success != 0)
	{
		glPushMatrix();
		glTranslatef(140,-40,0);

		glColor3d(0.0, 0.0, 0.0);
		glLineWidth(1);
		glBegin(GL_LINES);
		glVertex3f(0,0,0);
		glVertex3f(150,0,0);
		glVertex3f(150,0,0);
		glVertex3f(150,20,0);
		glVertex3f(150,20,0);
		glVertex3f(0,20,0);
		glVertex3f(0,20,0);
		glVertex3f(0,0,0);
		glEnd();
		glLineWidth(10);
		glColor3d(0.0, 1.0, 0.1);
		glBegin(GL_LINES);
		glVertex3f(0,5,0);
		glVertex3f(High_success*150,5,0);
		glVertex3f(0,15,0);
		glVertex3f(High_success*150,15,0);
		glEnd();

		glPopMatrix();
	}
	glPopMatrix();

	glPopMatrix();
	glPopMatrix();

	//glEnable(GL_DEPTH_TEST); 
	//glDisable(GL_ALPHA_TEST);
	//glDisable(GLUT_MULTISAMPLE);
	//glDisable(GL_POLYGON_SMOOTH);
	//glDisable(GL_POLYGON_SMOOTH_HINT);
	//glDisable(GL_BLEND);
	//glFlush();
}

void Spectrum::drawString(const char* str)
{
	int len, i;
	wchar_t* wstring;
	HDC hDC = wglGetCurrentDC();
	GLuint list = glGenLists(1);

	len = 0;
	for(i=0; str[i]!='\0'; ++i)
	{
		if( IsDBCSLeadByte(str[i]) )
			++i;
		++len;
	}

	wstring = (wchar_t*)malloc((len+1) * sizeof(wchar_t));
	MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, str, -1, wstring, len);
	wstring[len] = L'\0';

	for(i=0; i<len; ++i)
	{
		wglUseFontBitmapsW(hDC, wstring[i], 1, list);
		glCallList(list);
	}

	free(wstring);
	glDeleteLists(list, 1);
}
//void Spectrum::FFTProcessingALLTLC(double Y_Array[],int c_idx)
//{
//	//IsFull2048 = false;
//	//for(int j = 0;j < 256;j++)
//	//{
//	//	in_array_double_256[j] = (double)(Y_Array[j + c_idx - 256]);//update per 0.125sec
//	//}
//	//fftw_execute(fft_p256);
//}
//void Spectrum::UpdateSpectrumALLTLC()
//{
//	//for(int i = 0;i < Samples_Hz; i++)
//	//{
//	//	double magnitude = sqrt(out_array256[i][0]*out_array256[i][0] + out_array256[i][1]*out_array256[i][1]);
//	//	double as_ = 2.0*magnitude/256.0;//(I -> 0.5*N -1)
//
//	//	Y_Array_AS_50[i] = as_;
//	//	Y_Array_Phase_50[i] = atan2(out_array256[i][1],out_array256[i][0])*180/Pi;
//	//}
//}