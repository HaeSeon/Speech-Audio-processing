#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define PI 3.14


void DFT(int windowsize)         //windowsize : 한프레임당 몇포인트인지
{

	FILE* fp = NULL;
	fopen_s(&fp, "C:\\Users\\user\\Desktop\\noisy\\noisy.raw", "rb");   //입력 스트림 형성




	if (fp == NULL) {
		fprintf(stderr, "파일을 열 수 없습니다.");
		//return -1;                //파일을 열 수 없을 때(-1의 반환은 비정상적 종료 의미)
	}

	fseek(fp, 0, 2);
	int length;
	length = ftell(fp) / sizeof(short);
	fseek(fp, 0, 0);
	short* X = (short*)malloc(sizeof(short) * length);
	fread((short*)(X), sizeof(short), length, fp);
	fclose(fp);

	double w;
	double* Xr, * Xi, * IDFT_sig, * Xkr, * Xki, * IDFT_sig2;
	short* resultvalue;
	int k;
	int i;
	int a;
	for (i = 0; i < length; i++) {
		//printf("X[%d] : %d \n", i, X[i]);     //배열 확인용
	}

	
	Xr = (double*)malloc(windowsize * sizeof(double));
	Xi = (double*)malloc(windowsize * sizeof(double));
	Xkr = (double*)malloc(windowsize * sizeof(double));       //메모리 동적할당
	Xki = (double*)malloc(windowsize * sizeof(double));
	IDFT_sig = (double*)malloc(windowsize * sizeof(double));
	IDFT_sig2 = (double*)malloc(length * sizeof(double));
	resultvalue = (short*)malloc(length * sizeof(short));


	double* DXi, * DXr, * DXk, * DX;
	DXr = (double*)malloc(5 * windowsize * sizeof(double));
	DXi = (double*)malloc(5 * windowsize * sizeof(double));
	DXk = (double*)malloc(5 * windowsize * sizeof(double));
	DX = (double*)malloc(5 * windowsize * sizeof(double));
	double noise = 0;









	//5프레임 노이즈 크기 평균
	for (a = 0; a < windowsize; a++)
		DX[a] = 0;

	for (a = 0; a < 5 * windowsize; a += windowsize) {

		for (i = 0; i < windowsize; i++) {
			DXk[a + i] = DXr[a + i] = DXi[a + i] = 0;
			for (k = 0; k < windowsize; k++) {
				w = 2 * PI * k * i / windowsize;       //DFT 공식
				DXr[a + i] += cos(w) * X[a + k];       //실수부
				DXi[a + i] += sin(w) * X[a + k];       //허수부
			}
			DXk[a + i] = sqrt(pow(DXr[a + i], 2) + pow(DXi[a + i], 2));
			noise += DXk[a + i];
			//printf("X[%d] : %lf \n", i, DX[i]);
		}

	}






	for (a = 0; a < length; a++)
		IDFT_sig2[i] = 0;

	for (a = 0; a < length; a += windowsize) {
		

		//DFT

		for (i = 0; i < windowsize; i++)
			Xr[i] = Xi[i] = 0;

		for (i = 0; i < windowsize; i++)
		{
			if (a + i < length) {


				for (k = 0; k < windowsize; k++) {
					w = 2 * PI * k * i / windowsize;       //DFT 공식
					Xr[i] += cos(w) * X[a + k];       //실수부
					Xi[i] += sin(w) * X[a + k];       //허수부
				}

				Xkr[i] = (cos(atan2(Xi[i], Xr[i]))) * (sqrt(pow(Xr[i], 2) + pow(Xi[i], 2)) - noise / (5 * windowsize));
				Xki[i] = (sin(atan2(Xi[i], Xr[i]))) * (sqrt(pow(Xr[i], 2) + pow(Xi[i], 2)) - noise / (5 * windowsize));
				//printf("X[%d] : %lf \n", i, Xkr[i]);
			}
		}
		// IDFT //
		for (i = 0; i < windowsize; i++)
			IDFT_sig[i] = 0;
		for (i = 0; i < windowsize; i++)
		{

			if (a + i < length) {
				for (k = 0; k < windowsize; k++)
				{
					IDFT_sig[i] += Xkr[k] * cos(2 * PI * i * k / windowsize) / windowsize;
					IDFT_sig[i] += Xki[k] * sin(2 * PI * i * k / windowsize) / windowsize;
				}

			}
		}

		for (i = 0; i < windowsize; i++) {
			if (a + i < length) {
				IDFT_sig2[a + i] = IDFT_sig[i];
				//printf("X[%d] : %d \n", i+a, IDFT_sig[i+a]);     //배열 확인용
			}
		}

	}
	for (i = 0; i < length; i++) {
		resultvalue[i] = (short)IDFT_sig2[i];
		//printf("X[%d] : %d \n", i, resultvalue[i]);     //배열 확인용
	}


	FILE* fw;
	fopen_s(&fw, "C:\\Users\\user\\Desktop\\noisy\\noisy_remove_result.raw", "wb");     //출력 스트림 형성

	if (fw == NULL) {
		fprintf(stderr, "파일을 쓸 수 없습니다.");
		//return -1;             //파일을 쓸 수 없을 때(-1의 반환은 비정상적 종료 의미)
	}

	fwrite(resultvalue, sizeof(short), length, fw);
	fclose(fw);        //스트림 종료



	free(X);
	free(Xr);
	free(Xi);
	free(Xkr);
	free(Xki);
	free(DXr);
	free(DXi);
	free(DXk);
	free(DX);
	free(IDFT_sig);
	free(IDFT_sig2);
	free(resultvalue);     //동적할당 해제
}


void main(N) {
	printf("Put the Window Size : ");
	scanf_s("%d", &N);                      //사용자에게 윈도우 크기 (N값) 입력받기
	DFT(N);
} 
