#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define PI 3.14

double real;
double image;

int DFT(int N)
{
	char* X = (char*)malloc(N * sizeof(char));       //배열 X의 메모리 동적할당    

	FILE* fp = NULL;
	fopen_s(&fp, "C:\\Users\\user\\source\\repos\\ConsoleApplication2\\ConsoleApplication2\\sin.snd", "rb");   //입력 스트림 형성
	
	if (fp == NULL) {
		fprintf(stderr, "파일을 열 수 없습니다.");
		return -1;                //파일을 열 수 없을 때(-1의 반환은 비정상적 종료 의미)
	}
	
	fread(X, 1, N, fp);         //sample1.snd 파일을 1B씩 N개 불러와 배열 X에 저장


	double w;
	double* Xr, * Xi, *Xk;
	int k;
	int i;

	for (i = 0; i < N; i++) {
		//printf("X[%d] : %d \n", i, X[i]);     //배열 확인용
	}


	Xr = (double*)malloc(N * sizeof(double));  
	Xi = (double*)malloc(N * sizeof(double));
	Xk = (double*)malloc(N * sizeof(double));       //메모리 동적할당


	FILE* fw;
	FILE* fw1;
	fopen_s(&fw, "C:\\Users\\user\\source\\repos\\ConsoleApplication2\\ConsoleApplication2\\test.csv", "wt");     //출력 스트림 형성

	
	if (fw == NULL) {
		fprintf(stderr, "파일을 쓸 수 없습니다.");
		return -1;             //파일을 쓸 수 없을 때(-1의 반환은 비정상적 종료 의미)
	}
	

	double max;
	max = Xk[0];     
	int max_i = 0;    //max_i : 최대주파수 =max_i*8000/N


	for (i = 0; i < N; i++)
	{
		Xr[i] = 0;
		Xi[i] = 0;

		for (k = 0; k < N; k++){
			w = 2 * PI * k * i / N;       //DFT 공식

			Xr[i] += cos(w) * X[k];       //실수부
			Xi[i] += sin(w) * X[k];       //허수부
		}

		real = Xr[i];
		image = Xi[i];

		Xk[i]= sqrt(pow(real, 2) + pow(image, 2));          //주파수 크기 구하기

		if (Xk[i] > max) {
			max = Xk[i];
			max_i = i;
		}                //주파수 최대값 구하기


		fprintf(fw, "%d ,%lf \n", i, Xk[i]);  

		//printf("%d:(%lf,%lf)\n", i, real, image);         //DFT변환 확인용
	}

	printf("최대주파수 : %d", max_i*8000/N);      //최대주파수 출력

	fclose(fp);       
	fclose(fw);		  //스트림 종료

	free(X);          
	free(Xr);
	free(Xi);		  //동적할당 해제
	return 0;
}










int main(N) {
	printf("Put the Window Size : ");        
	scanf_s("%d", &N);                      //사용자에게 윈도우 크기 (N값) 입력받기
	DFT(N);
	return 0;
}
