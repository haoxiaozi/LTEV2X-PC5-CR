#include <iostream>
#include "Vehicle.h"
#include "BBlock.h"
#include "Radio.h"
#include "Random.h"

/*为OBU分配基本信息，位置信息和资源窗格信息
 ID、空口连接对象、所属街区指针和对象及ID;并将本OBU放入对应街区的vehiclelist中
*/
void Vehicle::vehicleInitial(int idTemp, BBlock* blockTemp, Radio* radioTemp)
{
	if (idTemp < VEHICLE_NUM)
	{
		id = idTemp;
		radio = radioTemp;
		block = blockTemp;
		blockID = block->getId();
		block->blockAccept(this);
		int k1;
		k1 = Random::random() % 8;
		/*参照图5-3的拓扑，按照随机概率将OBU的位置坐标初始化在街区的8个车道中某一个车道的某个位置上，并按照顺时针的行车方向确定OBU的速度信息。
		例如：当k1 = 0, OBU的位置初始化于街区右侧第一个车道的某个位置上，令其速度绝对值为最大车速，方向为下方向*/
		switch (k1) 
		{
		case 0:
		{
			speed.value = SPEED_MAX;
			speed.direction = VEHICLE_DIRECTION_DOWN;
			pos.x = blockTemp->getPos().x + (ROAD_WIDTH / 2) - LANE_WIDTH - LANE_WIDTH / 2.0;
			pos.y = blockTemp->getPos().y + Random::Uniform(-ROAD_LENGTH / 2, ROAD_LENGTH / 2);
			break;
		}
		case 1:
		{
			speed.value = SPEED_MAX;
			speed.direction = VEHICLE_DIRECTION_DOWN;
			pos.x = blockTemp->getPos().x + (ROAD_WIDTH / 2) - LANE_WIDTH / 2.0;
			pos.y = blockTemp->getPos().y + Random::Uniform(-ROAD_LENGTH / 2, ROAD_LENGTH / 2);
			break;
		}
		case 2:
		{
			speed.value = SPEED_MAX;
			speed.direction = VEHICLE_DIRECTION_RIGHT;
			pos.y = blockTemp->getPos().y + (ROAD_LENGTH / 2) - LANE_WIDTH - LANE_WIDTH / 2.0;
			pos.x = blockTemp->getPos().x + Random::Uniform(-ROAD_WIDTH / 2, ROAD_WIDTH / 2);
			break;
		}
		case 3:
		{
			speed.value = SPEED_MAX;
			speed.direction = VEHICLE_DIRECTION_RIGHT;
			pos.y = blockTemp->getPos().y + (ROAD_LENGTH / 2) - LANE_WIDTH / 2.0;
			pos.x = blockTemp->getPos().x + Random::Uniform(-ROAD_WIDTH / 2, ROAD_WIDTH / 2);
			break;
		}
		case 4:
		{
			speed.value = SPEED_MAX;
			speed.direction = VEHICLE_DIRECTION_UP;
			pos.x = blockTemp->getPos().x - (ROAD_WIDTH / 2) + LANE_WIDTH + LANE_WIDTH / 2.0;
			pos.y = blockTemp->getPos().y + Random::Uniform(-ROAD_LENGTH / 2, ROAD_LENGTH / 2);
			break;
		}
		case 5:
		{
			speed.value = SPEED_MAX;
			speed.direction = VEHICLE_DIRECTION_UP;
			pos.x = blockTemp->getPos().x - (ROAD_WIDTH / 2) + LANE_WIDTH / 2.0;
			pos.y = blockTemp->getPos().y + Random::Uniform(-ROAD_LENGTH / 2, ROAD_LENGTH / 2);
			break;
		}
		case 6:
		{
			speed.value = SPEED_MAX;
			speed.direction = VEHICLE_DIRECTION_LEFT;
			pos.y = blockTemp->getPos().y - (ROAD_LENGTH / 2) + LANE_WIDTH + LANE_WIDTH / 2.0;
			pos.x = blockTemp->getPos().x + Random::Uniform(-ROAD_WIDTH / 2, ROAD_WIDTH / 2);
			break;
		}
		case 7:
		{
			speed.value = SPEED_MAX;
			speed.direction = VEHICLE_DIRECTION_LEFT;
			pos.y = blockTemp->getPos().y - (ROAD_LENGTH / 2) + LANE_WIDTH / 2.0;
			pos.x = blockTemp->getPos().x + Random::Uniform(-ROAD_WIDTH / 2, ROAD_WIDTH / 2);
			break;
		}
		default:break;
		}
	}
	/*按照随机方式初始化本OBU占用资源窗格的情况。
	需连续占用4ms，每1ms占用10RB的频率资源。
	例如：可认为将50行100列的资源窗格矩阵分为125份，根据k2,k3取值，OBU占用其中的一份资源*/
	int k2, k3;
	k2 = Random::random() % (CYCLE / SLOT_PER_PACKET);
	k3 = Random::random() % (RB_NUM / RB_PER_PACKET);
	for (int i = 0; i < CYCLE; i++)
	{
		for (int j = 0; j < RB_NUM; j++)
		{
			if (((i >= k2 * SLOT_PER_PACKET) && (i < (k2 + 1)*SLOT_PER_PACKET)) && ((j >= k3 * RB_PER_PACKET) && (j < (k3 + 1)*RB_PER_PACKET)))
			{
				resourceWindow[i][j] = 1;
			}
			else
			{
				resourceWindow[i][j] = 0;
			}
		}
	}
}

/*
为RSU分配基本信息，位置信息和资源窗格信息
*/
void Vehicle::rsuInitial(int idTemp, Radio* radioTemp)
{
	/* 初始化基本信息，包括: ID;空口连接对象；速度默认为0；
	所属街区指针和对象及ID;并将本 OBU放入对应街区的vehiclelist 中。其中，RSU 默认输入0号街区*/
	id = idTemp;
	radio = radioTemp;
	speed.value = 0.0;
	block = &(radio->blocks[0]);
	blockID = block->getId();
	block->blockAccept(this);
	/* 将RSU的位置按照id固定在对应十字路口的中心位置*/
	int column = (id - VEHICLE_NUM) % 4;
	int row = (id - VEHICLE_NUM) / 4;
	pos.x = column * ROAD_WIDTH;
	pos.y = row * ROAD_LENGTH;

	/*和OBU分配资源窗格的方式相同*/
	int k1, k2;
	k1 = Random::random() % (CYCLE / SLOT_PER_PACKET);
	k2 = Random::random() % (RB_NUM / RB_PER_PACKET);
	for (int i = 0; i < CYCLE; i++)
	{
		for (int j = 0; j < RB_NUM; j++)
		{
			if (((i >= k1 * SLOT_PER_PACKET) && (i < (k1 + 1)*SLOT_PER_PACKET)) && ((j >= k2 * RB_PER_PACKET) && (j < (k2 + 1)*RB_PER_PACKET)))
			{
				resourceWindow[i][j] = 1;
			}
			else
			{
				resourceWindow[i][j] = 0;
			}
		}
	}
}

/*基于图5-33 和式 5-4 更新OBU位置坐标*/
void Vehicle::updateLocation()
{
	if (id < VEHICLE_NUM)
	{
		pos.x = pos.x + speed.value / (1000.0 / CHANNEL_UPDATE_INTERVAL)*cos(speed.direction);
		pos.y = pos.y + speed.value / (1000.0 / CHANNEL_UPDATE_INTERVAL)*sin(speed.direction);
		bool resultBlock = isNotInsideBlock(block);
		if (!resultBlock)
		{
			double distemp = 0.0;
			int k1;
			k1 = Random::random() % 4;
			switch (k1)
			{
			case(0):break;
			case(1):break;
			case(2):
			{
				if (speed.direction == VEHICLE_DIRECTION_UP)
				{
					distemp = abs(pos.y - (block->getPos().y + ROAD_LENGTH / 2.0));
					if (abs(pos.x - block->getPos().x)==ROAD_WIDTH/2-LANE_WIDTH/2)
					{
						pos.x = block->getPos().x - ROAD_WIDTH / 2.0 - distemp;
						pos.y = block->getPos().y + ROAD_LENGTH / 2.0 + LANE_WIDTH / 2.0;
						speed.direction = VEHICLE_DIRECTION_LEFT;
					}
					else
					{
						pos.x = block->getPos().x - ROAD_WIDTH / 2.0 - distemp;
						pos.y = block->getPos().y + ROAD_LENGTH / 2.0 + LANE_WIDTH * 1.5;
						speed.direction = VEHICLE_DIRECTION_LEFT;
					}
				}
				else if (speed.direction == VEHICLE_DIRECTION_DOWN)
				{
					distemp = abs(pos.y - (block->getPos().y - ROAD_LENGTH / 2.0));
					if (abs(pos.x - block->getPos().x) == ROAD_WIDTH / 2 - LANE_WIDTH / 2)
					{
						pos.x = block->getPos().x + ROAD_WIDTH / 2.0 + distemp;
						pos.y = block->getPos().y - ROAD_LENGTH / 2.0 - LANE_WIDTH / 2.0;
						speed.direction = VEHICLE_DIRECTION_RIGHT;
					}
					else
					{
						pos.x = block->getPos().x + ROAD_WIDTH / 2.0 + distemp;
						pos.y = block->getPos().y - ROAD_LENGTH / 2.0 - LANE_WIDTH * 1.5;
						speed.direction = VEHICLE_DIRECTION_RIGHT;
					}
				}
				else if (speed.direction == VEHICLE_DIRECTION_LEFT)
				{
					distemp = abs(pos.x - (block->getPos().x - ROAD_WIDTH / 2.0));
					if (abs(pos.y - block->getPos().y) == ROAD_LENGTH / 2 - LANE_WIDTH / 2)
					{
						pos.x = block->getPos().x - ROAD_WIDTH / 2.0 - LANE_WIDTH / 2.0;
						pos.y = block->getPos().y - ROAD_LENGTH / 2.0 - distemp;
						speed.direction = VEHICLE_DIRECTION_DOWN;
					}
					else
					{
						pos.x = block->getPos().x - ROAD_WIDTH / 2.0 - 1.5*LANE_WIDTH / 2.0;
						pos.y = block->getPos().y - ROAD_LENGTH / 2.0 - distemp;
						speed.direction = VEHICLE_DIRECTION_DOWN;
					}
				}
				else
				{
					distemp = abs(pos.x - (block->getPos().x + ROAD_WIDTH / 2.0));
					if (abs(pos.y - block->getPos().y) == ROAD_LENGTH / 2 - LANE_WIDTH / 2)
					{
						pos.x = block->getPos().x + ROAD_WIDTH / 2.0 + LANE_WIDTH / 2.0;
						pos.y = block->getPos().y + ROAD_LENGTH / 2.0 + distemp;
						speed.direction = VEHICLE_DIRECTION_UP;
					}
					else
					{
						pos.x = block->getPos().x + ROAD_WIDTH / 2.0 + 1.5*LANE_WIDTH / 2.0;
						pos.y = block->getPos().y + ROAD_LENGTH / 2.0 + distemp;
						speed.direction = VEHICLE_DIRECTION_UP;
					}
				}
				break;
			}
			case(3):
			{
				if (speed.direction == VEHICLE_DIRECTION_UP)
				{
					distemp = abs(pos.y - (block->getPos().y + ROAD_LENGTH / 2.0));
					if (abs(pos.x - block->getPos().x) == ROAD_WIDTH / 2.0 - LANE_WIDTH / 2.0)
					{
						pos.x = block->getPos().x - ROAD_WIDTH / 2.0 - distemp;
						pos.y = block->getPos().y + ROAD_LENGTH / 2.0 - LANE_WIDTH / 2.0;
						speed.direction = VEHICLE_DIRECTION_RIGHT;
					}
					else
					{
						pos.x = block->getPos().x - ROAD_WIDTH / 2.0 - distemp;
						pos.y = block->getPos().y + ROAD_LENGTH / 2.0 - LANE_WIDTH * 1.5;
						speed.direction = VEHICLE_DIRECTION_RIGHT;
					}
				}
				else if (speed.direction == VEHICLE_DIRECTION_DOWN)
				{
					distemp = abs(pos.y - (block->getPos().y - ROAD_LENGTH / 2.0));
					if (abs(pos.x - block->getPos().x) == ROAD_WIDTH / 2 - LANE_WIDTH / 2)
					{
						pos.x = block->getPos().x + ROAD_WIDTH / 2.0 + distemp;
						pos.y = block->getPos().y - ROAD_LENGTH / 2.0 + LANE_WIDTH / 2.0;
						speed.direction = VEHICLE_DIRECTION_LEFT;
					}
					else
					{
						pos.x = block->getPos().x + ROAD_WIDTH / 2.0 + distemp;
						pos.y = block->getPos().y - ROAD_LENGTH / 2.0 + 1.5*LANE_WIDTH / 2.0;
						speed.direction = VEHICLE_DIRECTION_LEFT;
					}
				}
				else if (speed.direction == VEHICLE_DIRECTION_LEFT)
				{
					distemp = abs(pos.x - (block->getPos().x - ROAD_WIDTH / 2.0));
					if (abs(pos.y - block->getPos().y) == ROAD_LENGTH / 2 - LANE_WIDTH / 2)
					{
						pos.x = block->getPos().x - ROAD_WIDTH / 2.0 + LANE_WIDTH / 2.0;
						pos.y = block->getPos().y - ROAD_LENGTH / 2.0 - distemp;
						speed.direction = VEHICLE_DIRECTION_UP;
					}
					else
					{
						pos.x = block->getPos().x - ROAD_WIDTH / 2.0 + 1.5*LANE_WIDTH / 2.0;
						pos.y = block->getPos().y - ROAD_LENGTH / 2.0 - distemp;
						speed.direction = VEHICLE_DIRECTION_UP;
					}
				}
				else
				{
					distemp = abs(pos.x - (block->getPos().x + ROAD_WIDTH / 2.0));
					if (abs(pos.y - block->getPos().y) == ROAD_LENGTH / 2 - LANE_WIDTH / 2)
					{
						pos.x = block->getPos().x + ROAD_WIDTH / 2.0 - LANE_WIDTH / 2.0;
						pos.y = block->getPos().y + ROAD_LENGTH / 2.0 + distemp;
						speed.direction = VEHICLE_DIRECTION_DOWN;
					}
					else
					{
						pos.x = block->getPos().x + ROAD_WIDTH / 2.0 - 1.5*LANE_WIDTH / 2.0;
						pos.y = block->getPos().y + ROAD_LENGTH / 2.0 + distemp;
						speed.direction = VEHICLE_DIRECTION_DOWN;
					}
				}
				break;
			}
			default:
				break;
			}
		}
		resultBlock = isNotInsideBlock(block);
		if (!resultBlock)
		{
			if (pos.x <= 0 || pos.x >= 3 * ROAD_WIDTH || pos.y <= 0 || pos.y >= 3 * ROAD_LENGTH)
			{
				this->wrapAround();
			}
			else
			{
				block->blockKick(this);
				int whichBlock = -1;
				for (int i = 0; i < BLOCK_NUM; i++)
				{
					if (this->isNotInsideBlock(&(radio->blocks[i])))
					{
						whichBlock = i;
						break;
					}
				}
				block = &(radio->blocks[whichBlock]);
				blockID = block->getId();
				block->blockAccept(this);
			}
		}
	}
}

/*离开拓扑后使用Wrap-around 技术进入拓扑功能由Vehicle::wrapAround()函数实现*/
void Vehicle::wrapAround()
{
	if (pos.x < 0 && pos.y < 0)
	{
		pos.x += 3 * ROAD_WIDTH;
		pos.y += 3 * ROAD_LENGTH;
	}
	else if (pos.x < 0 && (pos.y > 3 * ROAD_LENGTH))
	{
		pos.x += 3 * ROAD_WIDTH;
		pos.y -= 3 * ROAD_LENGTH;
	}
	else if ((pos.x > 3 * ROAD_WIDTH) && (pos.y > 3 * ROAD_LENGTH))
	{
		pos.x -= 3 * ROAD_WIDTH;
		pos.y -= 3 * ROAD_LENGTH;
	}
	else if ((pos.x > 3 * ROAD_WIDTH) && pos.y < 0)
	{
		pos.x -= 3 * ROAD_WIDTH;
		pos.y += 3 * ROAD_LENGTH;
	}
	else if ((pos.x>3*ROAD_WIDTH)&&(pos.y>0)&&(pos.y<3*ROAD_LENGTH))
	{
		pos.x -= 3 * ROAD_WIDTH;
	}
	else if ((pos.x <0 )&& (pos.y>0)&&(pos.y<3*ROAD_LENGTH))
	{
		pos.x += 3 * ROAD_WIDTH;
	}
	else if ((pos.y > 3 * ROAD_LENGTH) && (pos.x > 0) && (pos.x < 3 * ROAD_WIDTH))
	{
		pos.y -= 3 * ROAD_LENGTH;
	}
	else if ((pos.y < 0) &&(pos.x>0)&& (pos.x < 3 * ROAD_WIDTH))
	{
		pos.y += 3 * ROAD_LENGTH;
	}
	else
	{
		pos.x = ROAD_WIDTH - LANE_WIDTH / 2.0;
		pos.y = ROAD_LENGTH - LANE_WIDTH / 2.0;
	}
	block->blockKick(this);
	int blockIdTemp = -1;
	for (int i = 0; i < BLOCK_NUM; i++)
	{
		if (this->isNotInsideBlock(&(radio->blocks[i])))
		{
			blockIdTemp = i;
			break;
		}
	}
	block = &(radio->blocks[blockIdTemp]);
	blockID = block->getId();
	block->blockAccept(this);
}

/*所属街区判断功能 */
bool Vehicle::isNotInsideBlock(BBlock* blockTemp)
{

}