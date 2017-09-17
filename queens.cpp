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

	double Ih_trap[size] = { 0 };  //значения интеграла
	double Ih_TheSimpsons[size] = { 0 };  //значения интеграла
	double Ih_goose[size] = { 0 };  //значения интеграла
	trap_f = fopen(fnameout2, "w+");
////////////////////////////////////////////подсчет Ih и сумм Ih и вывод их//////////////////////////////////////////////////////
	


	for (int j = 0; j < size; j++) {
		double step = (b-a)/h[j];
		double a_new = a;  //смещение
		double b_new = a_new +step ;  //смещение
		for (int i = 0; i < h[j]-1; i++) {
			Ih_trap[j] += trap(a_new, b_new);
			Ih_TheSimpsons[j] += TheSimpsons(a_new, b_new);
			Ih_goose[j] += goose(a_new, b_new);  
			a_new = a_new + step;   
			b_new = b_new + step;

		}
	}
	
	

	

	fprintf(trap_f, "    Трапеция            Симпсон           Гаус по трем точкам    шаг  --  значения интегралов при данном шаге\n");
	for (int i = 1; i < size-3; i++) {   //в i=0 мусор		
		fprintf(trap_f, "%13.14lf   %13.14lf       %13.14lf       %d\n", Ih_trap[i], Ih_TheSimpsons[i], Ih_goose[i], h[i]);
	}
////////////////////////////////////////////вывод Rh=I-Ih в текстовый файл//////////////////////////////////////////////
	double Rh_trap[size];  //I-Ih
	double Rh_TheSimpsons[size];  //I-Ih
	double Rh_goose[size];  //I-Ih
	for (int i = 0; i < size; i++) {
		Rh_trap[i] = real_integral(a, b) - Ih_trap[i];
		Rh_TheSimpsons[i] = real_integral(a, b) - Ih_TheSimpsons[i];
		Rh_goose[i] = real_integral(a, b) - Ih_goose[i];
	}


	fprintf(trap_f, "\n    Трапеция            Симпсон           Гаус по трем точкам    шаг   --  сходимсоть I-Ih\n");
	for (int i = 1; i < size-3; i++) {  //в i=0 мусор		
		fprintf(trap_f, "%13.14lf   %13.14lf       %13.14lf       %d\n", Rh_trap[i], Rh_TheSimpsons[i], Rh_goose[i], h[i]);
	}


/////////////////////////////////////////////Сравнениек оценки порядка погрешности////////////////////////////////////////////////////////////////////////////////////////

	fprintf(trap_f, "\n   по Рунге          По Формуле      значение h\n");
	double fla[size];  //по формуле
	double runge[size];   //значение погрешности по рунге
	for (int i = 1; i < size-3; i++) {
		fla[i] =
			abs(
				log((Ih_trap[i] - Ih_trap[i + 1]) / (Ih_trap[i + 1] - Ih_trap[i + 2])) / log(2.)
				)
			;
		runge[i] = runge_f(Ih_trap, i);
		fprintf(trap_f, "%13.14lf  %13.14lf   %d\n", runge[i], fla[i], h[i]);

	}
///////////////////////////////////////////Правка/////////////////////////////////////////////
	fprintf(trap_f, "трапеция с учетом правки       значение hi\n");
	for (int i = 1; i < size-3; i++) {
		fprintf(trap_f, "%13.14lf   			%d\n", Ih_trap[i] + runge[i], h[i]);
	}

	fprintf(trap_f, "\nИтеграл по ньютону-лейбницу %13.14lf",real_integral(a,b));

}

