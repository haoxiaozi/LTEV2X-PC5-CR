#pragma once
#include <iostream>

class Random
{
public:
	Random();
	~Random();
	static int random();
	static double Uniform(double min, double max);
	static double Normal(double mu, double sigma);
	static double LogNormal(double mu, double sigma);
	static double Exponential(double mu);
	static double max(double* pa);
	static int maxId(double* pa);
	static int minId(double* pa, int length);
	static int sum(int* pa, int length);

private:
	static double Uniform_dis1(double min, double max);
	static double Normal_dis1(double mu, double sigma);
	static double LogNormal_dis1(double mu, double sigma);
	static double Exponential_dis1(double mu);
};

Random::Random()
{
}

Random::~Random()
{
}
