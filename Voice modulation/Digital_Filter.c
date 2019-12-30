#include <stdio.h>
#include <stdlib.h>

#define PI 3.14
#define f_peak 700      //원하는 소리에 맞춰 변경할 수 있음
#define f0 48000
#define ts  0.00002083     //sampling time
#define bandwidth 300     //원하는 소리에 맞춰 변경할 수 있음
#define f 50            //원하는 소리에 맞춰 변경할 수 있음    

float tau = 1 / (2 * PI * f);


//LowPass filter 
int LPF(short X[], short Y[], float x[], float y[], int length) {

	for (int i = 0; i < length; i++)
		(float)x[i] = X[i];       //형변환

	for (int i = 0; i < length; i++)
		y[i] = 0;              //초기화

	for (int i = 1; i < length; i++) 
		y[i] = (tau * y[i - 1] + ts * x[i]) / (tau + ts);     //lowpass

			
	for (int i = 0; i < length; i++) {
		(short)Y[i] = y[i];
		//printf("%f, %d\n",x[i], Y[i]);
	}

	FILE* fw = NULL;
	fopen_s(&fw, "C:\\Users\\user\\Desktop\\noisy\\In_the_water_result.snd", "wb");
	if (fw == NULL) {
		fprintf(stderr, "파일을 쓸 수 없습니다.");
		return -1;                //파일을 쓸 수 없을 때(-1의 반환은 비정상적 종료 의미)
	}
	fwrite(Y, sizeof(short), length, fw);

	fclose(fw);
}



//HighPass Filter 
int HPF(short X[], short Y[], float x[], float y[], int length) {

	for (int i = 0; i < length; i++)
		(float)x[i] = X[i];

	for (int i = 0; i < length; i++)
		y[i] = 0;              //초기화

	for (int i = 1; i < length; i++)
		y[i] = tau / (tau + ts) * y[i - 1] + tau / (tau + ts) * (x[i] - x[i - 1]);     //highpass


	for (int i = 0; i < length; i++) {
		(short)Y[i] = y[i];
		//printf("%f, %d\n",x[i], Y[i]);
	}

	FILE* fw = NULL;
	fopen_s(&fw, "C:\\Users\\user\\Desktop\\noisy\\alan_HPF_result.snd", "wb");
	if (fw == NULL) {
		fprintf(stderr, "파일을 쓸 수 없습니다.");
		return -1;                //파일을 쓸 수 없을 때(-1의 반환은 비정상적 종료 의미)
	}
	fwrite(Y, sizeof(short), length, fw);

	fclose(fw);
}









//Band Pass Filter
int BPF(short X[], short Y[], float x[], float y[], int length) {

	float w0 = 2 * PI * f_peak;
	float H = 1 / w0;
	float Q = f_peak / bandwidth;
	float H0 = H / Q;

	float aa0 = 4 / (ts * ts) + (2 * w0) / (Q * ts) + (w0 * w0);
	float aa1 = -8 / (ts * ts) + 2 * w0 * w0;
	float aa2 = 4 / (ts * ts) - (2 * w0) / (Q * ts) + (w0 * w0);
	float bb0 = (2 / ts) * (H0 * w0 * w0);
	float bb1 = 0;
	float bb2 = (-2 / ts) * (H0 * w0 * w0);

	float a0 = 1;
	float a1 = aa1 / aa0;
	float a2 = aa2 / aa0;
	float b0 = bb0 / aa0;
	float b1 = bb1 / aa0;
	float b2 = bb2 / aa0;

	float* yy = (float*)malloc(sizeof(float) * length);

	for (int i = 0; i < length; i++)
		(float)x[i] = X[i];       //형변환

	for (int i = 0; i < length; i++)
		y[i] = 0;              //초기화

	for (int i = 2; i < length; i++)
		y[i] = b0 * x[i] + b1 * x[i - 1] + b2 * x[i - 2] - a1 * y[i - 1] - a2 * y[i - 2];    //bandpass


	for (int i = 0; i < length; i++) {
		(short)Y[i] = y[i];
		//printf("%f, %d\n", x[i], Y[i]);
	}

	FILE* fw = NULL;
	fopen_s(&fw, "C:\\Users\\user\\Desktop\\noisy\\radios_result.snd", "wb");
	if (fw == NULL) {
		fprintf(stderr, "파일을 쓸 수 없습니다.");
		return -1;                //파일을 쓸 수 없을 때(-1의 반환은 비정상적 종료 의미)
	}
	fwrite(Y, sizeof(short), length, fw);

	fclose(fw);
}




//BandStop Filter
int BSF(short X[], short Y[], float x[], float y[], int length) {

	float w0 = 2 * PI * f_peak;
	float Q = f_peak / bandwidth;
	float H0 = 1;

	float aa0 = 4 / (ts * ts) + (2 * w0) / (Q * ts) + (w0 * w0);
	float aa1 = -8 / (ts * ts) + (2 * w0 * w0);
	float aa2 = 4 / (ts * ts) - (2 * w0) / (Q * ts) + (w0 * w0);
	float bb0 = (H0 * 4) / (ts * ts) + (H0 * w0 * w0);
	float bb1 = (-8 * H0 ) / (ts * ts) + (2 * w0 * w0);
	float bb2 = (H0 * 4) / (ts * ts) + (H0 * w0 * w0);

	float a0 = 1;
	float a1 = aa1 / aa0;
	float a2 = aa2 / aa0;
	float b0 = bb0 / aa0;
	float b1 = bb1 / aa0;
	float b2 = bb2 / aa0;

	for (int i = 0; i < length; i++)
		(float)x[i] = X[i];       //형변환

	for (int i = 0; i < length; i++)
		y[i] = 0;              //초기화

	for (int i = 2; i < length; i++)
		y[i] = b0 * x[i] + b1 * x[i - 1] + b2 * x[i - 2] - a1 * y[i - 1] - a2 * y[i - 2];    //bandstop


	for (int i = 0; i < length; i++) {
		(short)Y[i] = y[i];
		//printf("%f, %d\n", x[i], Y[i]);
	}

	FILE* fw = NULL;
	fopen_s(&fw, "C:\\Users\\user\\Desktop\\noisy\\BSF_result.snd", "wb");
	if (fw == NULL) {
		fprintf(stderr, "파일을 쓸 수 없습니다.");
		return -1;                //파일을 쓸 수 없을 때(-1의 반환은 비정상적 종료 의미)
	}
	fwrite(Y, sizeof(short), length, fw);

	fclose(fw);
}












int main() {


	FILE* fp = NULL;
	fopen_s(&fp, "C:\\Users\\user\\Desktop\\noisy\\voice.snd", "rb");
	if (fp == NULL) {
		fprintf(stderr, "파일을 읽을 수 없습니다.");
		return -1;                //파일을 쓸 수 없을 때(-1의 반환은 비정상적 종료 의미)
	}
	fseek(fp, 0, 2);
	int length;
	length = ftell(fp) / sizeof(short);
	fseek(fp, 0, 0);
	short* X = (short*)malloc(sizeof(short) * length);
	fread(X, sizeof(short), length, fp);         //sample1.snd 파일을 2B씩 N개 불러와 배열 X에 저장
	for (int i = 0; i < length; i++) {
		//printf("%d\n", X[i]);
	}


	float* y = (float*)malloc(sizeof(float) * length);
	short* Y = (short*)malloc(sizeof(short) * length);
	float* x = (float*)malloc(sizeof(float) * length);




	LPF(X, Y, x, y, length);

	HPF(X, Y, x, y, length);

	BPF(X, Y, x, y, length);
	BSF(X, Y, x, y, length);

	fclose(fp);
}




