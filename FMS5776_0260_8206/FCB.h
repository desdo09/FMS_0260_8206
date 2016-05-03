#pragma once
#include "Header.h"
#include "DirEntry.h"
#include "Sector.h"
#include "Disk.h"




class FCB
{
public:
	

private:
		Disk * d;
		DirEntry fileDesc;
		DATtype FAT;
		Sector Buffer;
		uint currRecNr;
		uint currSecNr;
		uint currRecNrInBuff;
		enumsFMS::FCBtypeToOpening type;
		bool lock;
	
public:
	
		

		FCB(Disk * d = NULL);
		FCB (Disk * d, DirEntry fileDesc, DATtype FAT, Sector * Buffer, uint currRecNr, uint currSecNr, uint currRecNrInBuff, enumsFMS::FCBtypeToOpening type);
		~FCB(){ 
			if(d!=NULL)
				delete [] d;
		}
		
		void closefile();

		void flushfile();
		void read(char *, bool update = false);
		void write(char *);
		void seek(enumsFMS::FCBseekfrom, int = 0);
		void updateCancel();
		void deleteRec();
		void update(char *);

	
	
		friend class Disk;
		friend void main();
};

