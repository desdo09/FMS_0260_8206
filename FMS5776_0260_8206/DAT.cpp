#include "DAT.h"



DAT::DAT()
{
	this->sectorNr = 1;															// DAT sector index 		
	this->dat.set();															// Set all bits 1
	dat[0] = 0;
	dat[1] = 0;
}


DAT::~DAT()
{
}

void DAT::resetDat()
{

	this->sectorNr = 1;															// DAT sector index 		
	this->dat.set();															// Set all bits 1
	dat[0] = 0;																	// Sector for vhd and DAT
	dat[1] = 0;																	// Sector for rootDir


}



