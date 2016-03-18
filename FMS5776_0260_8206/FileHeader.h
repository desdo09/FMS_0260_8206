#include "Header.h"
#include "DirEntry.h"

class FileHeader
{
	uint sectorNr;								// Sector number 
	DirEntry fileDesc;							// File descriptor
	DATtype FAT;								// FAT
	char emptyArea[1024-8-72-sizeof(DATtype)];	
public:
	FileHeader();
	~FileHeader();
};

