#pragma once
#include "header.h"
#include "DirEntry.h"


class SectorDir
{
private:
	int sectorNr;
	DirEntry dirEntry[14];
	char unUsed[12];

public:
	SectorDir();
	~SectorDir();

	short findNextIndex();
	bool  fileExist(const char *);
	short findDirByName(const char fileName[12]);

	friend class Disk;
	friend class RootDir;


};

class RootDir
{
private:
	SectorDir msbSector;
	SectorDir lsbSector;
	
public:
	RootDir();
	~RootDir();



	friend class Disk;

};

