#pragma once
#include "Header.h"



class DAT
{
private:

	uint    sectorNr;
	DATtype dat;
	char    emptyArea[1024-8-sizeof(DATtype)];

public:
	DAT();
	~DAT();

	void resetDat();

	//get
	uint    GetsectorNr() { return sectorNr; }
	DATtype Getdat() { return dat; }

	friend class Disk;
};

