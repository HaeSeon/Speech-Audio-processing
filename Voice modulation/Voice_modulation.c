#include <stdio.h>
#include <stdlib.h>
#include<windows.h>



#define NO_1 0
#define NO_2 1
#define NO_3 2
#define NO_4 3
#define NO_5 4
#define NO_6 5
#define NO_7 6




#define PI 3.14
#define f_peak 700      //원하는 소리에 맞춰 변경할 수 있음
#define f0 48000
#define ts  0.00002083     //sampling time
#define bandwidth 300     //원하는 소리에 맞춰 변경할 수 있음
#define f 50            //원하는 소리에 맞춰 변경할 수 있음     


int keyControl();
void init();
void titleDraw();
void menuDraw();
void gotoxy(int, int);

float tau = 1 / (2 * PI * f);



int keyControl() {
	char temp = getch();

	if (temp == '1') {
		return NO_1;
	}

	else if (temp == '2') {
		return NO_2;
	}
	else if (temp == '3') {
		return NO_3;
	}
	else if (temp == '4') {
		return NO_4;
	}
	else if (temp == '5') {
		return NO_5;
	}
	else if (temp == '6') {
		return NO_6;
	}
	else if (temp == '7') {
		return NO_7;
	}
}


void init() {
	system("mode con cols=100 lines=50 | title 음성변조");

}

void titleDraw() {
	printf("\n\n\n\n");
	printf("     ######           #       #     #     #    #       ########## \n");
	printf("    #      #         # #      #     #     #    #           #      \n");
	printf("   #        #       #   #     #     #######  ###          ###     \n");
	printf("    #      #       #     #  ###     #     #  ###         #   #    \n");
	printf("     #####        #       #   #     #######    #        #     #   \n");
	printf("  #############         ####  #        #       #           #      \n");
	printf("   #########           #    #          #                   #      \n");
	printf("   #       #          #      #         #                   #      \n");
	printf("   #########           #####           ############    ###########\n");



}





void menuDraw() {
	gotoxy(10 - 2, 15);
	printf("1.In the water");
	gotoxy(30 - 2, 15);
	printf("2.radios");
	gotoxy(10 - 2, 17);
	printf("3. Ailen");
	gotoxy(30 - 2, 17);
	printf("4.In the cave");
	gotoxy(10 - 2, 19);
	printf("5. 확성기");
	gotoxy(30 - 2, 19);
	printf("6. Only left");
	gotoxy(10 - 2, 21);
	printf("7. Only right\n");


}


void gotoxy(int x, int y) {
	HANDLE consolehandle = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos;
	pos.X = x;
	pos.Y = y;
	SetConsoleCursorPosition(consolehandle, pos);
}


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








int Radios(short X[], short Y[], float x[], float y[], int length) {

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
		yy[i] = 0;              //초기화

	for (int i = 0; i < length; i++)
		y[i] = 0;              //초기화

	for (int i = 2; i < length; i=i+5)                //소리 끊어지게
		yy[i] = b0 * x[i] + b1 * x[i - 1] + b2 * x[i - 2] - a1 * yy[i - 1] - a2 * yy[i - 2];    //bandpass

	for (int i = 1; i < length; i++)
		yy[i] = 200+350 * yy[i];        //소리 증폭, noise

	for (int i = 1; i < length; i = i + 1)
		y[i] = yy[i];

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





//In the water
int Water(short X[], short Y[], float x[], float y[], int length) {

	for (int i = 0; i < length; i++)
		(float)x[i] = X[i];       //형변환

	for (int i = 0; i < length; i++)
		y[i] = 0;              //초기화

	for (int i = 1; i < length; i++)
		y[i] = (tau * y[i - 1] + ts * x[i]) / (tau + ts);     //lowpass

	for (int i = 1; i < length; i++)
		y[i] = 5 * y[i];


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




int DELAY(short X[], short Y[], float x[], float y[], int length) {




	for (int i = 0; i < length; i++)
		(float)x[i] = X[i];       //형변환

	for (int i = 0; i < length; i++)
		y[i] = 0;              //초기화

	for (int i = 50000; i < length; i++)
		y[i - 50000] = 1.5 * x[i - 50000] + 1.1 * x[i - 30000] + 0.8 * x[i - 10000] + 1.5 * x[i];     //배열을 지연시켜, 크기에 변화를 준 후 합침


	for (int i = 0; i < length; i++) {
		(short)Y[i] = y[i];
		//printf("%d, %d, %d\n",i, x[i], y[i]);
	}

	FILE* fw = NULL;
	fopen_s(&fw, "C:\\Users\\user\\Desktop\\noisy\\In_the_cave_result.snd", "wb");
	if (fw == NULL) {
		fprintf(stderr, "파일을 쓸 수 없습니다.");
		return -1;                //파일을 쓸 수 없을 때(-1의 반환은 비정상적 종료 의미)
	}
	fwrite(Y, sizeof(short), length, fw);

	fclose(fw);
}








int ALIEN(short X[], short Y[], float x[], float y[], int length) {


	for (int i = 0; i < length; i++)
		(float)x[i] = X[i];       //형변환

	for (int i = 0; i < length; i++)
		y[i] = 0;              //초기화

	for (int i = 0; i < length; i = i + 17)
		y[i] = 2 * x[i];              //17단위로 소리를 끊어줌


	for (int i = 0; i < length; i++) {
		(short)Y[i] = y[i];
		//printf("%d, %d, %d\n",i, x[i], y[i]);
	}
	FILE* fw = NULL;
	fopen_s(&fw, "C:\\Users\\user\\Desktop\\noisy\\Alien_result.snd", "wb");
	if (fw == NULL) {
		fprintf(stderr, "파일을 쓸 수 없습니다.");
		return -1;                //파일을 쓸 수 없을 때(-1의 반환은 비정상적 종료 의미)
	}
	fwrite(Y, sizeof(short), length, fw);

	fclose(fw);
}



int LEFT(short X[], short Y[], float x[], float y[], int length) {


	for (int i = 0; i < length; i++)
		(float)x[i] = X[i];       //형변환

	for (int i = 0; i < length; i++)
		y[i] = 0;              //초기화

	for (int i = 0; i < length; i = i + 2)
		y[i] = x[i];                  //짝수배열만 들어가고 홀수배열은 0으로 만들어 왼쪽 소리만 출력


	for (int i = 0; i < length; i++) {
		(short)Y[i] = y[i];
		//printf("%d, %d, %d\n",i, x[i], y[i]);
	}
	FILE* fw = NULL;
	fopen_s(&fw, "C:\\Users\\user\\Desktop\\noisy\\left_result.snd", "wb");
	if (fw == NULL) {
		fprintf(stderr, "파일을 쓸 수 없습니다.");
		return -1;                //파일을 쓸 수 없을 때(-1의 반환은 비정상적 종료 의미)
	}
	fwrite(Y, sizeof(short), length, fw);

	fclose(fw);
}

int RIGHT(short X[], short Y[], float x[], float y[], int length) {


	for (int i = 0; i < length; i++)
		(float)x[i] = X[i];       //형변환

	for (int i = 0; i < length; i++)
		y[i] = 0;              //초기화

	for (int i = 1; i < length; i = i + 2)
		y[i] = x[i];               //홀수 배열만 들어가고 짝수 배열은 0으로 만들어 오른쪽 소리만 출력


	for (int i = 0; i < length; i++) {
		(short)Y[i] = y[i];
		//printf("%d, %d, %d\n",i, x[i], y[i]);
	}
	FILE* fw = NULL;
	fopen_s(&fw, "C:\\Users\\user\\Desktop\\noisy\\right_result.snd", "wb");
	if (fw == NULL) {
		fprintf(stderr, "파일을 쓸 수 없습니다.");
		return -1;                //파일을 쓸 수 없을 때(-1의 반환은 비정상적 종료 의미)
	}
	fwrite(Y, sizeof(short), length, fw);

	fclose(fw);
}



int Big(short X[], short Y[], float x[], float y[], int length){
	float* yy = (float*)malloc(sizeof(float) * length);

	for (int i = 0; i < length; i++)
		(float)x[i] = X[i];       //형변환

	for (int i = 0; i < length; i++)
		yy[i] = 0;              //초기화

	for (int i = 0; i < length; i = i++) {
		if (x[i] <= 100)
			yy[i] = (x[i] + 10) * 20+5000;      //목소리가 없는 부분에 대하여 잡음
		else
			yy[i] = x[i] * 38+1000;             //소리 증폭
	}
	for (int i = 0; i < length; i++)
		y[i] = 0;              //초기화

	for (int i = 1; i < length; i++)
		y[i] = (tau * y[i - 1] + ts * yy[i]) / (tau + ts);     //lowpass filter


	for (int i = 0; i < length; i++) {
		(short)Y[i] = y[i];
		//printf("%d, %d, %d\n",i, x[i], y[i]);
	}
	FILE* fw = NULL;
	fopen_s(&fw, "C:\\Users\\user\\Desktop\\noisy\\BIG_result.snd", "wb");
	if (fw == NULL) {
		fprintf(stderr, "파일을 쓸 수 없습니다.");
		return -1;                //파일을 쓸 수 없을 때(-1의 반환은 비정상적 종료 의미)
	}
	fwrite(Y, sizeof(short), length, fw);

	fclose(fw);
}


int main() {


	init();
	titleDraw();
	menuDraw();

	int keyCode = keyControl();




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

	if (keyCode == 0) {
		printf("\n In the water 변조완료\n");
		Water(X, Y, x, y, length);
	}
	if (keyCode == 1) {
		printf("radios 변조완료\n");
		Radios(X, Y, x, y, length);
	}
	if (keyCode == 2) {
		printf("Alien 변조완료\n");
		ALIEN(X, Y, x, y, length);
	}
	if (keyCode == 3) {
		printf("In the cave 변조완료\n");
		DELAY(X, Y, x, y, length);
	}
	if (keyCode == 4) {
		printf("확성기 변조완료\n");
		Big(X, Y, x, y, length);
		
	}
	if (keyCode == 5) {
		printf("Only left 변조완료\n");
		LEFT(X, Y, x, y, length);

	}
	if (keyCode == 6) {
		printf("Only right 변조완료\n");
		RIGHT(X, Y, x, y, length);
	}



	fclose(fp);
}
