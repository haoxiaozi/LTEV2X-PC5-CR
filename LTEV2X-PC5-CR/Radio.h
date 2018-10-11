#pragma once
#include "Basic.h"
#include "Vehicle.h"
#include "BBlock.h"

class Radio
{
public:
	Radio();
	~Radio();
	BBlock* blocks;
	Vehicle* vehicles;
	void radioInitial(BBlock blocksTemp[BLOCK_NUM], Vehicle vehicleTemp[VEHICLE_NUM + RSU_NUM]);
private:

};

Radio::Radio()
{
}

Radio::~Radio()
{
}