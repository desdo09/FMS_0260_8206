#pragma once
#include "DirEntry.h"


class SectorDir
{
private:
	int sectorNr;
	DirEntry dirEntry[14];
	char unUse[12];

public:
	SectorDir();
	~SectorDir();

	short findNextIndex();
	bool  fileExist(const char *);
	short findDirByName(const char *);

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

