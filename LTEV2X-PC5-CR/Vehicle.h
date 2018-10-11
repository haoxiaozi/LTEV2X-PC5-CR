#pragma once
#include <iostream>
#include <list>
#include "Basic.h"
#include "BBlock.h"
#include "Radio.h"

class Vehicle
{
public:
	Vehicle();
	~Vehicle();
	/*类成员函数*/
	/*获得的基本信息函数*/
	const int& getId();
	const Pos& getPos();
	const Speed& getSpeed();
	const stateVehicle* getState();
	/*车辆生成模块*/
	void rsuInitial(int idTemp, Radio* radioTemp);
	void vehicleInitial(int idTemp, BBlock* blockTemp, Radio* radioTemp);
	/*位置更新模块*/
	bool isNotInCross();
	bool isNotInsideBlock(BBlock* blockTemp);
	void blockAccess();
	void updateRelativeSpeed();
	void updateLocation();
	void reLocation(BBlock *blockTemp);
	void wrapAround();
	/*邻居判断模块*/
	bool isNotNeighbor(Vehicle* vehicleTemp);
	void updateNeighbor();
	/*路径损耗计算*/
	void updateDis();
	void updatePathLoss();
	void updateShadowLoss();
	void updateBigLoss();
	void updateMultiLoss(int tTemp);
	void updateRSRP();
	/*数值计算模块*/
	void updateResEnv();
	void updateCqi(int tTemp);
	void updateCycleData(int tTemp);
	/*资源调度模块*/
	void scheduleRandom();
	/*信息消除相关函数*/
	void cycleClear();
	void dropClear();
private:
	//下面是类成员变量
	/*基本信息*/
	Pos pos;
	Speed speed;
	int id;
	Radio* radio = NULL;
	int blockID;
	BBlock* block;
	bool crossJudgeOrNot = false;
	/*路径损耗信息*/
	double disCross[CROSS_NUM];
	double dis[VEHICLE_NUM + RSU_NUM];
	double pathLoss[VEHICLE_NUM + RSU_NUM];
	double shadowLoss[VEHICLE_NUM + RSU_NUM];
	double bigLoss[VEHICLE_NUM + RSU_NUM];
	double rsrp[VEHICLE_NUM + RSU_NUM];
	double rsrpM[VEHICLE_NUM + RSU_NUM];
	double relativeSpeed[VEHICLE_NUM + RSU_NUM];
	double multiLoss[VEHICLE_NUM + RSU_NUM];
	bool lossTypeNLOSorNot[VEHICLE_NUM + RSU_NUM];
	/*信号与频谱信息*/
	Cqi cqi;
	std::list<int>neighborList;
	std::list<int>neighborTransCycle;
	int resourceWindow[CYCLE][RB_NUM];
	int resourceEnvironment[CYCLE][RB_NUM];
	int receiveID[CYCLE][RB_NUM];
	/*行为信息和统计信息*/
	stateVehicle Vehiclestate[CYCLE][RB_NUM];
	PrrData prrData;
	RecTransData rectransdata;
	std::vector<Pair>cumData;
	std::vector<int>recSucID;
};

Vehicle::Vehicle()
{
}

Vehicle::~Vehicle()
{
}