#pragma once
#include <iostream>
#include <list>
#include "Basic.h"
#include "Vehicle.h"

class BBlock
{
public:
	BBlock();
	~BBlock();
	void blockInitial(int idTemp, Pos posTemp, Radio* radioTemp);
	const int& getId();
	const Pos& getPos();
	void blockAccept(Vehicle* vehicleTemp);
	void blockKick(Vehicle* vehicleTemp);
	void blockCycleClear();
	void blockDropClear();

private:
	int id;
	Pos pos;
	Radio* radio;
	std::list<Vehicle*>vehicleList;

};

BBlock::BBlock()
{
}

BBlock::~BBlock()
{
}