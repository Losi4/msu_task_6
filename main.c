#include <stdio.h>
#include <math.h>

double f1(double x) {
	return x * x * x;
}

double f1_(double x) {
	return 3 * x * x;
}

double f2(double x) {
	return x * x + 3 * x;
}

double f2_(double x) {
	return 2 * x + 3;
}

double f3(double x) {
	return sin(x);
}

double f3_(double x) {
	return cos(x);
}

void STEP(double (*f)(double), double (*g)(double), double (*f_)(double), double (*g_)(double), double *a, double *b) {
	int f1 = 0, f2 = 0;
	if (f(*a) - g(*a) < f(*b) - g(*b)) {
		f1 = 1;
	}
	if ((f(((*a) + (*b))/2) - g(((*a) + (*b))/2)) < (f(*a) - g(*a) + f(*b) - g(*b)) / 2) {
		f2 = 1;
	}

	if ((f1 == 1 && f2 == 1) || (f1 == 0 && f2 == 0)) {
		*a = *a + (f(*a) - g(*a)) * (*b - *a) / (f(*a) - g(*a) - (f(*b) - g(*b)));
		*b = *b - (f(*b) - g(*b)) / (f_(*b) - g_(*b));
	}
	else {
		*b = *a + (f(*a) - g(*a)) * (*b - *a) / (f(*a) - g(*a) - (f(*b) - g(*b)));
		*a = *a - (f(*a) - g(*a)) / (f_(*a) - g_(*a));
	}

}

double root(double (*f)(double), double (*g)(double), double (*f_)(double), double (*g_)(double), double a, double b, double eps) {
	for (int n = 1;; ++n) {
		STEP(f, g, f_, g_, &a, &b);
		if (fabs(a - b) <= eps) {
			break;
		}
	}
	return a;
}

double simpson(double (*f)(double), double a, double b, int n) {
	const double width = (b - a) / n;
	double simpson_integral = 0;
	for (int step = 0; step < n; ++step) {
		const double x1 = a + step * width;
		const double x2 = a + (step + 1) * width;
		simpson_integral += (x2 - x1) / 6.0 * (f(x1) + 4.0 * f(0.5 * (x1 + x2)) + f(x2));
	}
	return simpson_integral;
}

double integral(double (*f)(double), double a, double b, double eps) {
	for (int n = 1;; ++n) {
		double s1 = simpson(f, a, b, n);
		double s2 = simpson(f, a, b, 2 * n);
		double dif = fabs(s2 - s1) / 3.;
		if (dif < eps) {
			return s1;
		}
	}
	return 0;
}

int main(void) {

	printf("Integral of x^3 from 0 to 3\n");
	printf("program answer: %0.10f\nright answer: 20.25\n\n", integral(f1, 0, 3, 0.00000001));
	printf("Integral of x^2 + 3x from -1 to 2.5\n");
	printf("program answer: %0.10f\nright answer: 13.4167\n\n\n", integral(f2, -1, 2.5, 0.00000001));

	printf("Abscissa of intersection of functions x^3 and x^2 + 3x\n");
	printf("program answer: %0.10f\nright answer: -1.302775637732\n\n", root(f2, f1, f2_, f1_, -2, -1, 0.00000001));
	printf("Abscissa of intersection of functions x^2 + 3x and x^3\n");
	printf("program answer: %0.10f\nright answer: -1.302775637732\n\n", root(f1, f2, f1_, f2_, -2, -1, 0.00000001));
	printf("Abscissa of intersection of functions sin(x) and x^2 + 3x\n");
	printf("program answer: %0.10f\nright answer: -2.92738\n\n", root(f3, f2, f3_, f2_, -4, -2, 0.00000001));
	printf("Abscissa of intersection of functions x^2 + 3x and sin(x)\n");
	printf("program answer: %0.10f\nright answer: -2.92738\n", root(f2, f3, f2_, f3_, -4, -2, 0.00000001));

	return 0;
}
