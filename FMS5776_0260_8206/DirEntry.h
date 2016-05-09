#pragma once
#include "Header.h"

class DirEntry
{
	char filename[12];          // File name
	char fileOwner[12];			// File Owner 
	uint fileAddr;				// File Address (First sector) 
	char crDate[10];			// Created date 
	uint fileSize;				// File size (Numbers of sectors)
	uint eofRecNr;				// Register number when the file is allocated
	uint maxRecSize;			// Register size max
	uint actualRecSize;			// Register size actual
	char recFormat[2];			// 
	uint keyOffset;				//
	uint keySize;				// 
	char keyType[2];			// Key Type - (I)nt,(F)loat,(D)ouble
	unsigned short entryStatus;	// 0 - empty 1 - Active 2- inactive


public:
	DirEntry();
	~DirEntry();

	/*************************************************
	*
	*				 Friends
	*
	**************************************************/
	friend class Disk;
	friend class FileHeader;
	friend class SectorDir;
	friend class TestLevel_2;
	friend class FCB;
};

