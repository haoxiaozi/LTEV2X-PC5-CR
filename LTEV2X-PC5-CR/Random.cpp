
#include <iostream>
#include<cmath>
#include"Random.h"
#include "Basic.h"
static unsigned int seed = 0;

int Random::random()
{
	srand(seed++);
	return rand();
}

double Random::Uniform(double min, double max)
{
	return Uniform_dis1(min, max);
}

double Random::Uniform_dis1(double min, double max)
{
	double ans = random();
	ans = (max - min)*ans / RAND_MAX;
	ans += min;
	return ans;
}

double Random::Normal(double mu, double sigma)
{
	return Normal_dis1(mu, sigma);
}

double Random::Normal_dis1(double mu, double sigma)
{
	double dResult;
	double dScope;
	double dNormal;
	do
	{
		dResult = Uniform(-3, 3);
		dScope = Uniform(0, 1.0 / sqrt(2 * PI));
		dNormal = 1.0 / sqrt(2 * PI)*exp(-1 * (dResult)*(dResult) / 2.0);
	} while (dScope > dNormal);
	dResult *= sigma;
	dResult += mu;
	return dResult;
}

double Random::LogNormal(double mu, double sigma)
{
	return LogNormal_dis1(mu, sigma);
}

double Random::LogNormal_dis1(double mu, double sigma)
{
	double dResult;
	double dScope;
	double dLogNormal;
	do
	{
		dResult = Uniform(-3, 3);
		dScope = Uniform(0, 1.0 / (dResult*sqrt(2 * PI)));
		if (dResult > 0) 
		{
			dLogNormal = 1.0 / (dResult*sqrt(2 * PI))*exp(-1 * (log(dResult)*log(dResult)) / 2.0);
		}else
		{
			dLogNormal = 0.0;
		}
	} while (dScope>dLogNormal);
	dResult *= sigma;
	dResult += mu;
	return dResult;
}

double Random::Exponential(double mu)
{
	return Exponential_dis1(mu);
}

double Random::Exponential_dis1(double mu)
{
	double z;
	do
	{
		srand(seed++);
		z = ((double)random() / RAND_MAX);
	} while ((z == 0) || (z == 1));
	return (-mu * log(z));
}

double Random::max(double *pa)
{
	double tmpMax;
	tmpMax = pa[0];
	int length = sizeof(pa) / sizeof(pa[0]); //数组长度
	int i = 1;
	do
	{
		tmpMax = tmpMax > pa[i] ? tmpMax : pa[i];
		i++;
	} while (i < length);
	return tmpMax;
}

int Random::maxId(double *pa)
{
	double tmpMax;
	tmpMax = pa[0];
	int tmpMaxIndex;
	tmpMaxIndex = 0;
	int length = sizeof(pa) / sizeof(pa[0]); //数组长度
	int i = 1;
	do
	{
		if (tmpMax < pa[i])
		{
			tmpMax = pa[i];
			tmpMaxIndex = i;
		}
		i++;
	} while (i < length);
	return tmpMaxIndex;
}

int Random::minId(double *pa, int length)
{
	double tmpMin;
	tmpMin = pa[0];
	int tmpMinIndex;
	tmpMinIndex = 0;
	int length = sizeof(pa) / sizeof(pa[0]); //数组长度
	int i = 1;
	do
	{
		if (tmpMin > pa[i])
		{
			tmpMin = pa[i];
			tmpMinIndex = i;
		}
		i++;
	} while (i < length);
	return tmpMinIndex;
}

int Random::sum(int* pa, int length)
{
	int tmpSum = 0;
	for (int i = 0; i < length; i++)
		tmpSum = tmpSum + pa[i];
	return tmpSum;
}