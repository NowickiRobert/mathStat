// mpwi_lab6.cpp : Defines the entry point for the console application.
//

#include<iostream>
#include<tuple>
#include <cstdlib>
#include <cmath>


using namespace std;

float fRand(double fMin = 0.0, double fMax = 1.0)
{
	float f = (float)rand() / RAND_MAX;
	return fMin + f * (fMax - fMin);
}

//mu-srednia
//sigma - odchylenie standardowe
double box_muller(double m, double s)
{
	float x1, x2, w, y1;
	static float y2;
	static int use_last = 0;

	if (use_last)
	{
		y1 = y2;
		use_last = 0;
	}
	else
	{
		do {
			x1 = 2.0 * fRand() - 1.0;
			x2 = 2.0 * fRand() - 1.0;
			w = x1 * x1 + x2 * x2;
		} while (w >= 1.0);

		w = sqrt((-2.0 * log(w)) / w);
		y1 = x1 * w;
		y2 = x2 * w;
		use_last = 1;
	}

	return(m + y1 * s);
}

double drand(double dmin, double dmax)
{
	double d = (double)rand() / RAND_MAX;
	return dmin + d * (dmin - dmax);
}

void print_tab(double *tab, int n)
{
	for(int i=0; i<n;i++)
	{
		cout << tab[i] << " ";
	}
}


int compare(const void * a, const void * b)
{
	return (*(double*)a - *(double*)b);
}

void sort(double *tab, unsigned int size)
{
	qsort(tab, size, sizeof(double), compare);
}

double mediane(double *tab, unsigned int size)
{
	sort(tab, size);
	return size % 2 == 0 ? 0.5*(tab[(size-1)/2]+tab[size/2]) : tab[size / 2];
}

double findMin(const double *tab, unsigned int size)
{
	double min = tab[0];
	for(int i=1; i<size; i++)
	{
		if (tab[i] < min) min = tab[i];
	}
	return min;
}

double findMax(const double *tab, unsigned int size)
{
	double max = tab[0];
	for (int i = 1; i<size; i++)
	{
		if (tab[i] > max) max = tab[i];
	}
	return max;
}

double findMode(const double *tab, unsigned int size)
{
	//liczba wystapien
	int maxc = 0, c;
	double maxw = tab[0] - 1, w;
	

	for (int i = 0; i < size - maxc; i++)
	{
		w = tab[i];
		if (w != maxw)
		{
			c = 1;
			for (int j = i + 1; j < size; j++) if (w == tab[j]) c++;

			if (c > maxc)
			{
				maxc = c;
				maxw = w;
			}
		}
	}
	return  maxw;
}

double findAverageArithmetical(const double *tab, unsigned int size)
{
	double avg = 0;
	for(int i=0; i<size;i++)
	{
		avg += tab[i] / size;
	}
	return avg;
}

double findSum(const double *tab, unsigned int size)
{
	int tmp = 0;
	for(int i=0; i<size;i++)
	{
		tmp += tab[i];
	}
	return tmp;
}

double findAverageArithmeticalWeighted(const double *tab, const double *weights, unsigned int size)
{
	double avg = 0;
	for (int i = 0; i < size; i++)
	{
		avg += tab[i] * weights[i];
	}
	return avg/findSum(weights,size);
}

double findAverageGeometric(const double *tab, unsigned int size)
{
	double avg = 1, tmp;
	for(int i=0; i<size;i+=10)
	{
		tmp = 1;
		for(int j=0;i+j<size && j<10;j++)
		{
			tmp *= tab[i + j];
		}
		tmp = pow(tmp, 1.0 / size);
		avg *= tmp;
	}
	return avg;
}

double findAverageGeometricWeighted(const double *tab, const double *weights, unsigned int size)
{
	double avg = 1, tmp;
	for (int i = 0; i<size; i += 10)
	{
		tmp = 1;
		for (int j = 0; i + j<size && j<10; j++)
		{
			tmp *= pow(tab[i + j], weights[i]);
		}
		tmp = pow(tmp, 1.0 / findSum(weights,size));
		avg *= tmp;
	}
	return avg;
}

double findMoment(const double *tab, unsigned int moment_ordinal, unsigned int size)
{
	double moment = 0;
	for(int i=0;i<size;i++)
	{
		moment+=pow(tab[i], moment_ordinal)/size;
	}
	return moment;
}

double findMomentCentral(const double *tab, unsigned int moment_ordinal, unsigned int size)
{
	double moment = 0;
	double avg = findAverageArithmetical(tab, size);
	for (int i = 0; i<size; i++)
	{
		moment += pow(tab[i]-avg, moment_ordinal) / size;
	}
	return moment;
}

double findVariance(const double *tab, unsigned int size)
{
	return findMomentCentral(tab, 2, size);
}

double findStandardDeviation(const double *tab, unsigned int size)
{
	return sqrt(findVariance(tab, size));
}


double findDeviation(const double *tab, unsigned int size)
{
	double dev = 0;
	double avg = findAverageArithmetical(tab, size);
	for (int i = 0; i<size; i++)
	{
		dev += abs(tab[i] - avg) / size;
	}
	return dev;
}

//mu-mean
//std-standard deviation
void generate_date(int n, double *result, double mu, double std)
{
	for (int i = 0; i<n; i++)
	{
		result[i] = box_muller(mu, std);
	}	
}

int main()
{
	const int size1 = 1000;
	const int size = 100;
	double tab[size];
	double tab1[size1];
	double mu = 100, std = 50;
	//napisac generator o rozkladzie normalnym 100 - 1 000 000 liczb
	generate_date(size1, tab1, mu, std);
	generate_date(size, tab, mu, std);
	//print_tab(tab, size);
	sort(tab1, size);
	
	//dla poziomu 0.95
	double ufn_min, ufn_max;

	cout << "Dla "<< size1 << endl;
	ufn_min = findAverageArithmetical(tab1, size1) - 1.96*findStandardDeviation(tab1, size1) / sqrt(size1);
	ufn_max = findAverageArithmetical(tab1, size1) + 1.96*findStandardDeviation(tab1, size1) / sqrt(size1);

	cout << "Dla poziomu 0.95 min: " << ufn_min << " max: " << ufn_max << " dlugosc: " << ufn_max-ufn_min<<endl;

	ufn_min = findAverageArithmetical(tab1, size) - 1.28*findStandardDeviation(tab1, size1) / sqrt(size1);
	ufn_max = findAverageArithmetical(tab1, size) + 1.28*findStandardDeviation(tab1, size1) / sqrt(size1);

	cout << "Dla poziomu 0.8 min: " << ufn_min << " max: " << ufn_max << " dlugosc: " << ufn_max - ufn_min << endl;

	cout << "Dla "<< size << endl;
	ufn_min = findAverageArithmetical(tab, size) - 1.96*findStandardDeviation(tab, size) / sqrt(size);
	ufn_max = findAverageArithmetical(tab, size) + 1.96*findStandardDeviation(tab, size) / sqrt(size);

	cout << "Dla poziomu 0.95 min: " << ufn_min << " max: " << ufn_max << " dlugosc: " << ufn_max - ufn_min << endl;

	ufn_min = findAverageArithmetical(tab, size) - 1.28*findStandardDeviation(tab, size) / sqrt(size);
	ufn_max = findAverageArithmetical(tab, size) + 1.28*findStandardDeviation(tab, size) / sqrt(size);

	cout << "Dla poziomu 0.8 min: " << ufn_min << " max: " << ufn_max << " dlugosc: " << ufn_max - ufn_min << endl;



	

	//cout << findAverageGeometric(tab, 12) << endl;
    return 0;
}

