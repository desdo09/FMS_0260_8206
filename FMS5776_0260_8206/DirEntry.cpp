#include "DirEntry.h"



DirEntry::DirEntry()
{
	entryStatus = 0;
	memcpy(filename, "RootDir", 7);
}


DirEntry::~DirEntry()
{
}
