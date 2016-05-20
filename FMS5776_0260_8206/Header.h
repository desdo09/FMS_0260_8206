#pragma warning(disable:4996)
#pragma once
#include <iostream>
#include<string>
#include <bitset>
#include <fstream>
#include <ctime>
#include "ProgramExeption.h"
using namespace std;

using  uint = unsigned int;

#define  diskExtension ".bin"

#define  extensionLenght 4

#define amountOfSectors 1600

using  DATtype = bitset<amountOfSectors>;

class DAT;

class Disk;

class SectorDir;

class RootDir;

class FileHeader;

class VolumeHeader;

class Sector;

class DirEntry;

class FCB;

namespace enumsFMS
{
	enum FCBtypeToOpening { input = 0, output, inputOutput, Extension };
	enum FCBseekfrom { beginning = 0, current, eof };
}





