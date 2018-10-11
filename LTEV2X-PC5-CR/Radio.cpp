#include "Radio.h"
#include "BBlock.h"
#include "Vehicle.h"

void Radio::radioInitial(BBlock blocksTemp[BLOCK_NUM], Vehicle vehicleTemp[VEHICLE_NUM + RSU_NUM])
{
	blocks = blocksTemp;
	vehicles = vehicleTemp;
}