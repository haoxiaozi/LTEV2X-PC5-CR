#pragma once

#include <iostream>
#include <vector>
#include <math.h>
/* 自然常量和街区参数
*/
#define PI 3.14159265358979323846
const double LANE_WIDTH = 3.5; //单位是m,道路宽度
const double SIDEWALK_WIDTH = 3.0;//单位m,人行道宽度
const double STREET_WIDTH = 20.0;//单位m,街道宽度
const double ROAD_LENGTH = 433.0;//单位m,街区长度
const double ROAD_WIDTH = 250.0;//单位m,街区宽度
const double NUM_LANE_UNI = 2;//单向二车道
const int NUM_LANE_DOU = NUM_LANE_UNI * 2;//双向四车道
const int RSU_NUM = 16;//RSU个数
const int CROSS_NUM = 16;//十字路口数量
const int BLOCK_NUM = 9;//街区数量
const double ROAD_ALL_LENGTH = (ROAD_LENGTH * BLOCK_NUM + ROAD_WIDTH * BLOCK_NUM) * NUM_LANE_DOU ;//拓扑车道总长度

/* 射频和空口参数*/
const double PENETRATION_LOSS = 0;//dB,插入损耗
const double NOISE_FIGURE_V = 9;//dB，噪声系数
const double MAX_TX_PWOER = 23;//dBm,最大发射功率
const double ANTENNA_GAIN_V = 3;//dB,OBU天线增益
const double ANTENNA_GAIN_R = 3;//dB,RSU天线增益
const double ANTENNA_HEIGHT_V = 1.5;//OBU天线高度
const double ANTENNA_HEIGHT_R = 5;//RSU天线高度
const double FREQUENCY_CW = 5905;//MHz,载波频率
const double LAMDA = 3e8 / FREQUENCY_CW / 1000000;//波长,m
const double CENTER_FREQ = 2 * PI * FREQUENCY_CW / 1e3;//载波角频率,单位为弧度(rad)
const int RB_NUM = 50;//RB数量，10MHz
const double BAND_WIDTH = 10;// MHz, 带宽
const double SUM_BAND_WIDTH = 0.18;//RB频率宽度，MHz
const double N0 = SUM_BAND_WIDTH * 1E6 * pow(10.0, (-174.0 / 10.0)) * pow(10.0, (NOISE_FIGURE_V / 10.0)); //单RB底噪能量，单位为mW

/*车辆运动参数*/
const double VEHICLE_DIRECTION_RIGHT = 0.0 / 360.0 * 2 * PI;//rad,右向离轴角
const double VEHICLE_DIRECTION_UP = 90.0 / 360.0 * 2 * PI;// 上向离轴角
const double VEHICLE_DIRECTION_LEFT = 180.0 / 360.0 * 2 * PI;//左向离轴角
const double VEHICLE_DIRECTION_DOWN = 270.0 / 360.0 * 2 * PI;//下向离轴角
const double P_TURN_RIGHT = 0.25;//十字路口右转概率
const double P_TURN_LEFT = 0.25;//十字路口左转概率
const double P_TURN_STRAIGHT = 0.5;//十字路口直行概率
const double SPEED_MAX = 60.0 * 1000 / 60 / 60;//最大车速，m/s
const double VEHICLE_SPACING = 2.5 * SPEED_MAX;//撒点车辆平均车距
const double MIN_VEHICLE_SPACE = 1 * SPEED_MAX;//撒点最小车辆间距
/*路径损耗相关参数*/
const double SHADOW_LOSS_DEV_NLOS = 4;//NLOS情况下的阴影衰落标准方差
const double SHADOW_LOSS_DEV_LOS = 3;//LOS情况下的阴影衰落标准方差
const double LOS_Threshold_Horizon = ROAD_WIDTH;//LOS环境判断条件 水平方向
const double LOS_Threshold_Vertical = ROAD_LENGTH;//LOS环境判断条件 垂直方向
const int NP = 9;//多径损耗中的簇数
const int MSP = 20;//多径损耗中的每个簇的子径数
const double PARTH_DELAY[NP] = { 0, 30, 150, 310, 370, 710, 1090, 1730, 2510 };//每个簇的到达时延
const double PATH_POWER[NP] = { 0.2412, 0.1708, 0.1747, 0.1053, 0.2101, 0.0297, 0.0481, 0.0152, 0.0049 };//每个组的归一化能量比例
const double COS_ALPHA[MSP] = { 1.000, 0.9511, 0.8090, 0.5878,
								0.3090, 0.0000, -0.3090, -0.5878,
								-0.8090, -0.9511, -1.0000, -0.9511,
								-0.8090, -0.5878, -0.3090, -0.0000,
								0.3090, 0.5878, 0.8090, 0.9511 };//每个簇的每个子径达到角度的余弦值
/*车辆数和链路层接口参数*/
const int VEHICLE_NUM = 585;//实际计算方式为int(floor(ROAD_ALL_LENGTH/VEHICLE_SPACING)).由于需要将车辆平均分配在9个街区，所以此数应为9的倍数
const int vehicleNumPerBlock = VEHICLE_NUM / BLOCK_NUM;//撒点时每个街区的车辆数
const double maxRange = 50;//邻居判断范围
const double MCL = 130;//链路层仿真结果，城区随机调度方法情况下的最大耦合损耗
const double SINR_REQUIRED = MCL - 10 * log10(pow(10, (MAX_TX_PWOER / 10.0)) / RB_NUM) - ANTENNA_GAIN_V - ANTENNA_GAIN_V + 10 * log10(N0);//MCL和相关参数推导得出的正确接收数据所需的SINR
const double MESSAGE_SIZE = 300;//默认发送300bytes的数据分组
const int SLOT_PER_PACKET = 4;//每个数据分组所需要的TTI数
const int RB_PER_PACKET = 10;//每个数据分组在每个TTI中所消耗的RB数

/*仿真周期*/
const int CYCLE = 100; //ms,

/*基本结构和功能类*/
// 位置结构体，X,Y坐标
struct Pos
{
	double x;
	double y;
};
// 速度结构体，速度绝对值和离轴角
struct Speed
{
	double value;
	double direction;
};
// 记录成功接收发送方发送数据的RB数，包括发送方的ID和成功接收数据的RB数
struct Pair
{
	int id;
	int times;
};
// 复数，实部与虚部
struct Complex
{
	double real;
	double virt;
};
// 信道质量指示，包括本传输周期内所有RB的虚拟接收信号能量总和、主信号能量和SINR
struct Cqi
{
	double all[CYCLE][RB_NUM];
	double main[CYCLE][RB_NUM];
	double SINR[CYCLE][RB_NUM];
};

/*
传输统计信息， 包括： 
1. 本周期传输数据分组数
2. 本周期接收数据分组数
3. 累计传输数据分组数
4. 累计接收数据分组数
5. 本周期分组接收率
6. 本撒点周期分组接收率
*/
struct PrrData
{
	unsigned int transCycle;
	unsigned int receCycle;
	unsigned int transAccu;
	unsigned int receAccu;
	double prrPerCycle;
	double prrPerDrop;
};

/*
行为和接收情况统计，包括:
1. 本周期每个RB接收信号来源节点ID;
2. 本周期每个RB是否成功接收数据
3. 本周期每个RB是否接收数据；
4. 本周期每个RB是否传输数据
5. 本周期每个RB是否静默
*/
struct RecTransData
{
	int receiveid[CYCLE][RB_NUM];
	bool receiveSucOrNot[CYCLE][RB_NUM];
	bool receiveOrNot[CYCLE][RB_NUM];
	bool transOrNot[CYCLE][RB_NUM];
	bool listenOrNot[CYCLE][RB_NUM];
};

/*
	行为：
	1. 接收
	2. 传输
	3. 静默
*/
typedef enum
{
	receive,
	transmit,
	listen
}stateVehicle;

/*
V2V链路/V2I链路的信道环境，
1. LOS
2. NLOS
3.相隔太远所以不考虑
*/
typedef enum
{
	LOS,
	NLOS,
	notConcern
}lossType;

/*
功能类，用作函数对象，用于在vector容器中，利用find_if函数寻找指定发送方的成功传输数据的RB数目
*/
class vector_finder
{
public:
	vector_finder(const int a) :m_i_a(a) {};
	bool operator()(const std::vector<Pair>::value_type &value)
	{
		return value.id == m_i_a;
	}
private:
	int m_i_a;
};
