#include "DAT.h"



DAT::DAT()
{
	this->sectorNr = 1;															// DAT sector index 		
	this->dat.set();															// Set all bits 1
	for (int i = 0; i < 4; i++)													// Set The first 4 bits 0 
		this->dat.set(i, 0);
}


DAT::~DAT()
{
}

void DAT::resetDat()
{

	this->sectorNr = 1;															// DAT sector index 		
	this->dat.set();															// Set all bits 1
	for (int i = 0; i < 4; i++)													// Set The first 4 bits 0 
		this->dat.set(i, 0);


}



