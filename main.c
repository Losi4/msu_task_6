#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
double strtod (const char *str, char **endstr);

extern double f1(double);
extern double f2(double);
extern double f3(double);
extern double f1_(double);
extern double f2_(double);
extern double f3_(double);

int N = 0;
const double EPS = 1e-5;

int check_double(char *s)
{
    int flag = 1, i = 0;
    while(s[i])
    {
        if (s[i] != '-' && s[i] != '.' && !isdigit(s[i]))
        {
            flag = 0;
            printf("!!%c", s[i]);
            break;
        }
        i++;
    }
    return flag;
}

double tf0 (double x)
{
    return x - x;
}

double tf0_ (double x)
{
    return x - x;
}

double tf1 (double x)
{
    return x * x - 2 * x - 10;
}

double tf1_ (double x)
{
    return 2 * x - 2;
}

double tf2 (double x)
{
    return 2 * pow(x, 30) - 5 * pow(x, 24) + 11 * pow(x, 3) - 3 * x - 15;
}

double tf2_ (double x)
{
    return 60 * pow(x, 29) - 120 * pow(x, 23) + 33 * pow(x, 2) - 3;
}

double tf3 (double x)
{
    return pow(x, -4) + pow(x, -6) + pow(x, -13) - 1;
}

double tf3_ (double x)
{
    return -4 * pow(x, -5) - 6 * pow(x, -7) - 13 * pow(x, -14);
}

double tf4 (double x)
{
    return sin(x) - 2 * cos(x);
}

double tf4_ (double x)
{
    return cos(x) + 2 * sin(x);
}

double tf5 (double x)
{
    return pow(2, x) - pow(3, x) + 1;
}

double tf5_ (double x)
{
    return log(2) * pow(2, x) - log(3) * pow(3, x);
}

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
	for (int step = 0; step < n; ++step)
    {
		const double x1 = a + step * width;
		const double x2 = a + (step + 1) * width;
		simpson_integral += (x2 - x1) / 6.0 * (f(x1) + 4.0 * f(0.5 * (x1 + x2)) + f(x2));
	}
	return simpson_integral;
}

double integral(double (*f)(double), double a, double b, double eps)
{
    double s1 = simpson(f, a, b, 1);
	for (int n = 2;; n *= 2)
    {
		double s2 = simpson(f, a, b, n);
		double dif = fabs(s2 - s1) / 15.0;
		if (dif < eps)
		{
			return s2;
		}
		s1 = s2;
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
            printf("-v  -  Information about task\n");
            printf("-points  -  Print all intersections\n");
            printf("-iterations  -  Print number of iterations\n");
            printf("-test-integral_{number of function (1,2,3)}_{segment [a,b] input a}_{input b}_{epsilon} (-test-integral 1 0.5 2 0.001)\n");
            printf("-test-root_{number of first function (1,2,3)}_{number of second function (1,2,3)}_{segment [a,b] input a}_{input b}_{epsilon} (-test-root 1 2 -3 4 0.001)\n");
            printf("-test-functions  -  Print integral and intersection with 0 of all testing functions on testing segment\n\n");
            return 0;
        }
        else if (strcmp(argv[i], "-test-functions") == 0)
        {
            printf("Computing integral of x^2 - 2x - 10 on segment [1; 3] with 0.00001 precision\n");
            printf("Program result: %f\n", integral(tf1, 1, 3, EPS));
            printf("Real result: -19.33333\n");
            if (fabs(integral(tf1, 1, 3, EPS) + 19.333333) < 0.00001) printf("OK\n");
            else printf("FAIL\n");
            printf("Computing solution of x^2 - 2x - 10 = 0 on segment [3; 5] with 0.00001 precision\n");
            printf("Program result: %f\n", root(tf1, tf0, tf1_, tf0_, 3, 5, EPS));
            printf("Real result: 4.316624\n");
            if (fabs(root(tf1, tf0, tf1_, tf0_, 3, 5, EPS) - 4.316624) < 0.00001) printf("OK\n");
            else printf("FAIL\n");
            printf("\n");

            printf("Computing integral of 2 * x^30 - 5 * x^24 + 11 * x^3 - 3 * x - 15 on segment [0; 1] with 0.00001 precision\n");
            printf("Program result: %f\n", integral(tf2, 0, 1, EPS));
            printf("Real result: -13.885483\n");
            if (fabs(integral(tf2, 0, 1, EPS) + 13.885483) < 0.00001) printf("OK\n");
            else printf("FAIL\n");
            printf("Computing solution of 2 * x^30 - 5 * x^24 + 11 * x^3 - 3 * x - 15 = 0 on segment [1; 2] with 0.00001 precision\n");
            printf("Program result: %f\n", root(tf2, tf0, tf2_, tf0_, 1, 2, EPS));
            printf("Real result: 1.166021\n");
            if (fabs(root(tf2, tf0, tf2_, tf0_, 1, 2, EPS) - 1.166021) < 0.00001) printf("OK\n");
            else printf("FAIL\n");
            printf("\n");

            printf("Computing integral of x^(-4) + x^(-6) + x^(-13) - 1 on segment [3; 4] with 0.00001 precision\n");
            printf("Program result: %f\n", integral(tf3, 3, 4, EPS));
            printf("Real result: -0.99223\n");
            if (fabs(integral(tf3, 3, 4, EPS) + 0.99223) < 0.00001) printf("OK\n");
            else printf("FAIL\n");
            printf("Computing solution of x^(-4) + x^(-6) + x^(-13) - 1 = 0 on segment [1; 2] with 0.00001 precision\n");
            printf("Program result: %f\n", root(tf3, tf0, tf3_, tf0_, 1, 2, EPS));
            printf("Real result: 1.180511\n");
            if (fabs(root(tf3, tf0, tf3_, tf0_, 1, 2, EPS) - 1.180511) < 0.00001) printf("OK\n");
            else printf("FAIL\n");
            printf("\n");

            printf("Computing integral of sin(x) - 2 * cos(x) on segment [-1; 1] with 0.00001 precision\n");
            printf("Program result: %f\n", integral(tf4, -1, 1, EPS));
            printf("Real result: -3.365884\n");
            if (fabs(integral(tf4, -1, 1, EPS) + 3.365884) < 0.00001) printf("OK\n");
            else printf("FAIL\n");
            printf("Computing solution of sin(x) - 2 * cos(x) = 0 on segment [1; 2] with 0.00001 precision\n");
            printf("Program result: %f\n", root(tf4, tf0, tf4_, tf0_, 1, 2, EPS));
            printf("Real result: 1.107148\n");
            if (fabs(root(tf4, tf0, tf4_, tf0_, 1, 2, EPS) - 1.107148) < 0.00001) printf("OK\n");
            else printf("FAIL\n");
            printf("\n");

            printf("Computing integral of 2^x - 3^x + 1 on segment [2; 5] with 0.00001 precision\n");
            printf("Program result: %f\n", integral(tf5, 2, 5, EPS));
            printf("Real result: -169.600517\n");
            if (fabs(integral(tf5, 2, 5, EPS) + 169.600517) < 0.00001) printf("OK\n");
            else printf("FAIL\n");
            printf("Computing solution of 2^x - 3^x + 1 = 0 on segment [0; 2] with 0.00001 precision\n");
            printf("Program result: %f\n", root(tf5, tf0, tf5_, tf0_, 0, 2, EPS));
            printf("Real result: 1\n");
            if (fabs(root(tf5, tf0, tf5_, tf0_, 0, 2, EPS) - 1) < 0.00001) printf("OK\n");
            else printf("FAIL\n");
            printf("\n");

            return 0;
        }
    }
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
        if (strcmp("-test-integral", argv[i]) == 0)
        {
            if (argc - i - 1 < 4)
            {
                printf("error: too few arguments for -test-integral\n");
                break;
            }
            if (strcmp("1", argv[i + 1]) && strcmp("2", argv[i + 1]) && strcmp("3", argv[i + 1]))
            {
                printf("Invalid first argument for -test-integral\n");
                break;
            }
            ff = argv[i + 1][0] - '0';
            if (!check_double(argv[i + 2]))
            {
                printf("Invalid second argument for -test-integral\n");
                break;
            }
            a = strtod(argv[i + 2], 0);
            if (!check_double(argv[i + 3]))
            {
                printf("Invalid third argument for -test-integral\n");
                break;
            }
            b = strtod(argv[i + 3], 0);
            if (a > b)
            {
                printf("Error: The left border of the segment must be less or equal than the right (a <= b)\n");
                break;
            }
            if (!check_double(argv[i + 4]))
            {
                printf("Invalid fourth argument for -test-integral\n");
                break;
            }
            eps = strtod(argv[i + 4], 0);
            if (eps < 0)
            {
                printf("Error: epsilon should be positive (eps > 0)\n");
                break;
            }
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
            return 0;
        }
    }
    for (int i = 1; i < argc; ++i)
    {
        if (strcmp("-test-root", argv[i]) == 0)
        {
            if (argc - i - 1 < 5)
            {
                printf("error: too few arguments for -test-root\n");
                break;
            }
            int f1, f2;
            if (strcmp("1", argv[i + 1]) && strcmp("2", argv[i + 1]) && strcmp("3", argv[i + 1]))
            {
                printf("Invalid first argument for -test-root\n");
                break;
            }
            f1 = argv[i + 1][0] - '0';
            if (strcmp("1", argv[i + 2]) && strcmp("2", argv[i + 2]) && strcmp("3", argv[i + 2]))
            {
                printf("Invalid second argument for -test-root\n");
                break;
            }
            f2 = argv[i + 2][0] - '0';
            if (!check_double(argv[i + 3]))
            {
                printf("Invalid third argument for -test-root\n");
                break;
            }
            a = strtod(argv[i + 3], 0);
            if (!check_double(argv[i + 4]))
            {
                printf("Invalid fourth argument for -test-root\n");
                break;
            }
            b = strtod(argv[i + 4], 0);
            if (a > b)
            {
                printf("Error: The left border of the segment must be less or equal than the right (a <= b)\n");
                break;
            }
            if (!check_double(argv[i + 5]))
            {
                printf("Invalid fifth argument for -test-root\n");
                break;
            }
            eps = strtod(argv[i + 5], 0);
            if (eps < 0)
            {
                printf("Error: epsilon should be positive (eps > 0)\n");
                break;
            }
            double (*ff1)(double) = func[f1 - 1];
            double (*ff2)(double) = func[f2 - 1];
            double (*dff1)(double) = dfunc[f1-1];
            double (*dff2)(double) = dfunc[f2-1];
            printf("%f\n", root(ff1, ff2, dff1, dff2, a, b, eps));
            printf("\n");
            return 0;
        }
    }
    int right_flag = 0;
    double x1 = root(f1, f2, f1_, f2_, 1, 2, EPS);
    double x2 = root(f2, f3, f2_, f3_, 0, 1, EPS);
    double x3 = root(f3, f1, f3_, f1_, -2, -1, EPS);

    double S = integral(f1, x3, x1, EPS) - integral(f2, x2, x1, EPS) - integral(f3, x3, x2, EPS);
    N = 0;
    printf("square = %f\n\n", S);
    for (int i = 1; i < argc; ++i)
    {
        if (strcmp("-v", argv[i]) == 0)
        {
            printf("Task: Finding the area of a figure limited by three functions\n");
            printf("Solution method: combined method and Simpson`s formula\n");
            printf("Integrals finding on: f1 [%f;%f], f2 [%f;%f], f3 [%f; %f]\n", x3, x1, x2, x1, x3, x2);
            printf("Epsilon: 0.001\n");
            right_flag = 1;
            break;
        }
    }
    for (int i = 1; i < argc; ++i)
    {
        if(strcmp("-points", argv[i]) == 0)
        {
            printf("POINTS\n");
            printf("f1 and f2    %f\n", root(f1, f2, f1_, f2_, 1, 2, EPS));
            printf("f2 and f3    %f\n", root(f2, f3, f2_, f3_, 0, 1, EPS));
            printf("f3 and f1    %f\n\n", root(f3, f1, f3_, f1_, -2, -1, EPS));
            right_flag = 1;
            break;
        }
    }
    for (int i = 1; i < argc; ++i)
    {
        if (strcmp("-iterations", argv[i]) == 0)
        {
            printf("ITERATIONS\n");
            N = 0;
            root(f1, f2, f1_, f2_, 1, 2, EPS);
            printf("f1 and f2 iterations = %d\n", N);

            N = 0;
            root(f2, f3, f2_, f3_, 0, 1, EPS);
            printf("f2 and f3 iterations = %d\n", N);

            N = 0;
            root(f3, f1, f3_, f1_, -2, -1, EPS);
            printf("f3 and f1 iterations = %d\n\n", N);
            right_flag = 1;
            break;
        }
    }
    if (!right_flag && argc > 1) printf("Invalid key\n\n");
	return 0;
}
