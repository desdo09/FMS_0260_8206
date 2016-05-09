#pragma once
#include "Header.h"
#include "DirEntry.h"
#include "Sector.h"
#include "Disk.h"




class FCB
{
public:
	

private:
		Disk * d;								// The disk
		DirEntry fileDesc;						// The file data
		DATtype FAT;							// The file FAT
		Sector Buffer;							// The buffer
		unsigned long currRecNr;				// The record ID
		uint currRecNrInBuff;					// Where is the record into the buffer
		uint currSecNrInBuff;					// Where is the record into the buffer
		uint * currSecNr;						// The sector number
		enumsFMS::FCBtypeToOpening type;		// File open type
		bool lock;								// 
	
public:
	
		

		FCB(Disk * d = NULL);
		FCB (Disk * d, DirEntry fileDesc, DATtype FAT, Sector * Buffer, unsigned long currRecNr, uint currRecNrInBuff, enumsFMS::FCBtypeToOpening type);
		~FCB(){ 
			if(d!=NULL)
				delete [] d;
		}
		
		void closefile();

		void flushfile();
		void read(char *, bool update = false);
		void write(char * data,int recordInFile = -1 );
		void seek(enumsFMS::FCBseekfrom, int = 0);
		void updateCancel();
		void deleteRec();
		void update(char *);
		unsigned long getKey();
	
	
		friend class Disk;
		friend void main();
};

