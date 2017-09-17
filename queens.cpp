#include <stdio.h>  
#include <math.h>   
#include "Header.h"
#include <cmath>

FILE *trap_f;
char fnameout2[256] = "trap.txt";

void main(void) {

	int h[size];
	for (int i = 0; i < size; i++) {
		h[i] = pow(2, i);
	}

	double Ih_trap[size] = { 0 };  //�������� ���������
	double Ih_TheSimpsons[size] = { 0 };  //�������� ���������
	double Ih_goose[size] = { 0 };  //�������� ���������
	trap_f = fopen(fnameout2, "w+");
////////////////////////////////////////////������� Ih � ���� Ih � ����� ��//////////////////////////////////////////////////////
	


	for (int j = 0; j < size; j++) {
		double step = (b-a)/h[j];
		double a_new = a;  //��������
		double b_new = a_new +step ;  //��������
		for (int i = 0; i < h[j]-1; i++) {
			Ih_trap[j] += trap(a_new, b_new);
			Ih_TheSimpsons[j] += TheSimpsons(a_new, b_new);
			Ih_goose[j] += goose(a_new, b_new);  
			a_new = a_new + step;   
			b_new = b_new + step;

		}
	}
	
	

	

	fprintf(trap_f, "    ��������            �������           ���� �� ���� ������    ���  --  �������� ���������� ��� ������ ����\n");
	for (int i = 1; i < size-3; i++) {   //� i=0 �����		
		fprintf(trap_f, "%13.14lf   %13.14lf       %13.14lf       %d\n", Ih_trap[i], Ih_TheSimpsons[i], Ih_goose[i], h[i]);
	}
////////////////////////////////////////////����� Rh=I-Ih � ��������� ����//////////////////////////////////////////////
	double Rh_trap[size];  //I-Ih
	double Rh_TheSimpsons[size];  //I-Ih
	double Rh_goose[size];  //I-Ih
	for (int i = 0; i < size; i++) {
		Rh_trap[i] = real_integral(a, b) - Ih_trap[i];
		Rh_TheSimpsons[i] = real_integral(a, b) - Ih_TheSimpsons[i];
		Rh_goose[i] = real_integral(a, b) - Ih_goose[i];
	}


	fprintf(trap_f, "\n    ��������            �������           ���� �� ���� ������    ���   --  ���������� I-Ih\n");
	for (int i = 1; i < size-3; i++) {  //� i=0 �����		
		fprintf(trap_f, "%13.14lf   %13.14lf       %13.14lf       %d\n", Rh_trap[i], Rh_TheSimpsons[i], Rh_goose[i], h[i]);
	}


/////////////////////////////////////////////���������� ������ ������� �����������////////////////////////////////////////////////////////////////////////////////////////

	fprintf(trap_f, "\n   �� �����          �� �������      �������� h\n");
	double fla[size];  //�� �������
	double runge[size];   //�������� ����������� �� �����
	for (int i = 1; i < size-3; i++) {
		fla[i] =
			abs(
				log((Ih_trap[i] - Ih_trap[i + 1]) / (Ih_trap[i + 1] - Ih_trap[i + 2])) / log(2.)
				)
			;
		runge[i] = runge_f(Ih_trap, i);
		fprintf(trap_f, "%13.14lf  %13.14lf   %d\n", runge[i], fla[i], h[i]);

	}
///////////////////////////////////////////������/////////////////////////////////////////////
	fprintf(trap_f, "�������� � ������ ������       �������� hi\n");
	for (int i = 1; i < size-3; i++) {
		fprintf(trap_f, "%13.14lf   			%d\n", Ih_trap[i] + runge[i], h[i]);
	}

	fprintf(trap_f, "\n������� �� �������-�������� %13.14lf",real_integral(a,b));

}

