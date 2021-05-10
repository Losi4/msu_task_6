#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

extern double f1(double);
extern double f2(double);
extern double f3(double);
extern double f1_(double);
extern double f2_(double);
extern double f3_(double);
int N;

double eps = 1e-10;

void Step(double (*f)(double), double (*g)(double), double (*f_)(double), double (*g_)(double), double *a, double *b)
{
	int f1 = 0, f2 = 0;
	if (f(*a) - g(*a) < f(*b) - g(*b))
    {
		f1 = 1;
	}
	if ((f(((*a) + (*b))/2) - g(((*a) + (*b))/2)) < (f(*a) - g(*a) + f(*b) - g(*b)) / 2)
	{
		f2 = 1;
	}
	if ((f1 == 1 && f2 == 1) || (f1 == 0 && f2 == 0))
    {
		*a = *a + (f(*a) - g(*a)) * (*b - *a) / (f(*a) - g(*a) - (f(*b) - g(*b)));
		*b = *b - (f(*b) - g(*b)) / (f_(*b) - g_(*b));
	}
	else
	{
		*b = *a + (f(*a) - g(*a)) * (*b - *a) / (f(*a) - g(*a) - (f(*b) - g(*b)));
		*a = *a - (f(*a) - g(*a)) / (f_(*a) - g_(*a));
	}

}

double root(double (*f)(double), double (*g)(double), double (*f_)(double), double (*g_)(double), double a, double b, double eps)
{
	for (int n = 1;; ++n)
    {
		N++;
		Step(f, g, f_, g_, &a, &b);
		double s1 = f(a) - g(a);
		double s2 = f(a + eps) - g(a + eps);
		double s3 = f(a - eps) - g(a - eps);
		if (fabs(a - b) <= eps || s1 * s2 <= 0 || s1 * s3 <= 0)
		{
			break;
		}
	}
	return a;
}

double simpson(double (*f)(double), double a, double b, int n)
{
	const double width = (b - a) / n;
	double simpson_integral = 0;
	for (int Step = 0; Step < n; ++Step)
    {
		const double x1 = a + Step * width;
		const double x2 = a + (Step + 1) * width;
		simpson_integral += (x2 - x1) / 6.0 * (f(x1) + 4.0 * f(0.5 * (x1 + x2)) + f(x2));
	}
	return simpson_integral;
}

double integral(double (*f)(double), double a, double b, double eps)
{
	for (int n = 1;; ++n)
    {
		double s1 = simpson(f, a, b, n);
		double s2 = simpson(f, a, b, 2 * n);
		double dif = fabs(s2 - s1) / 3.	;
		if (dif < eps)
		{
			return s1;
		}
	}
	return 0;
}

int main(int argc, char **argv)
{
    for (int i = 1; i < argc; ++i)
    {
        if (strcmp(argv[i], "-help") == 0)
        {
            printf("HELP\n");
            printf("-points  -  print all intersections\n-iterations  -  print number of interations\n-test-integral  -  User input: number of function(1,2,3), segment [a,b](input a, input b), epsilon; after input print value of this integral\n-test-root  -  User input: number of first function(1,2,3), number of second function(1,2,3), segment [a,b](input a, input b), epsilon; after input print value of root\n\n");
            return 0;
        }
    }
        double x1 = root(f1, f2, f1_, f2_, 1, 2, 0.00001);
        double x2 = root(f2, f3, f2_, f3_, 0, 1, 0.00001);
        double x3 = root(f3, f1, f3_, f1_, -2, -1, 0.000001);

        double S = integral(f1, x3, x1, 0.000001) - integral(f2, x2, x1, 0.000001) - integral(f3, x3, x2, 0.000001);
        N = 0;
        printf("square = %f\n\n", S);
        int ff = 1;
        double a = 1, b = 2, eps = 0.01;
        double (*func[3])(double);
        double (*dfunc[3])(double);
        func[0] = f1;
        func[1] = f2;
        func[2] = f3;
        dfunc[0] = f1_;
        dfunc[1] = f2_;
        dfunc[2] = f3_;
        for (int i = 1; i < argc; ++i)
        {
            if (strcmp(argv[i], "-help") == 0)
            {
                printf("HELP\n");
                printf("-points  -  print all intersections\n-iterations  -  print number of interations\n-test-integral  -  User input: number of function(1,2,3), segment [a,b](input a, input b), epsilon; after input print value of this integral\n-test-root  -  User input: number of first function(1,2,3), number of second function(1,2,3), segment [a,b](input a, input b), epsilon; after input print value of root\n\n");
                return 0;
            }
            else if(strcmp("-points", argv[i]) == 0)
            {
                printf("POINTS\n");
                printf("f1 and f2    %f\n", root(f1, f2, f1_, f2_, 1, 2, 0.00001));
                printf("f2 and f3    %f\n", root(f2, f3, f2_, f3_, 0, 1, 0.00001));
                printf("f3 and f1    %f\n\n", root(f3, f1, f3_, f1_, -2, -1, 0.000001));
            }
            else if (strcmp("-iterations", argv[i]) == 0)
            {
                printf("ITERATIONS\n");
                N = 0;
                root(f1, f2, f1_, f2_, 1, 2, 0.00001);
                printf("f1 and f2 iterations = %d\n", N);

                N = 0;
                root(f2, f3, f2_, f3_, 0, 1, 0.00001);
                printf("f2 and f3 iterations = %d\n", N);

                N = 0;
                root(f3, f1, f3_, f1_, -2, -1, 0.000001);
                printf("f3 and f1 iterations = %d\n\n", N);
            }
            else if(strcmp("-test-integral", argv[i]) == 0)
            {
                printf("number of function = ");
                scanf("%d", &ff);
                printf("\n");
                printf("a = ");scanf("%lf", &a);printf("\n");
                printf("b = ");scanf("%lf", &b);printf("\n");
                printf("eps = ");scanf("%lf", &eps);printf("\n");
                if (ff == 1)
                {
                    printf("%f\n", integral(f1, a, b, eps));
                }
                else if (ff == 2)
                {
                    printf("%f\n", integral(f2, a, b, eps));
                }
                else
                {
                    printf("%f\n", integral(f3, a, b, eps));
                }
                printf("\n");
            }
            else if (strcmp("-test-root", argv[i]) == 0)
            {
                int f1, f2;
                printf("number of first function = ");scanf("%d", &f1);printf("\n");
                printf("number of second function = ");scanf("%d", &f2);printf("\n");
                printf("a = ");scanf("%lf", &a);printf("\n");
                printf("b = ");scanf("%lf", &b);printf("\n");
                printf("eps = ");scanf("%lf", &eps);printf("\n");
                double (*ff1)(double) = func[f1 - 1];
                double (*ff2)(double) = func[f2 - 1];
                double (*dff1)(double) = dfunc[f1-1];
                double (*dff2)(double) = dfunc[f2-1];
                printf("%f\n", root(ff1, ff2, dff1, dff2, a, b, eps));
                printf("\n");
            }
            else
            {
                printf("Invalid key\n\n");
            }
        }
	return 0;
}
