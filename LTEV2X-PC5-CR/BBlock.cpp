#include "BBlock.h"

/*为街区分配基本信息，包括：ID,位置和空口连接对象。其中街区位置为街区中心位置*/
void BBlock::blockInitial(int idTemp, Pos posTemp, Radio* radioTemp)
{
	id = idTemp;
	pos.x = posTemp.x;
	pos.y = posTemp.y;
	radio = radioTemp;
}