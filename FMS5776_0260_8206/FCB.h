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
		bool lock;								// Check if update is required
		int maxRecPerSec;						// Amount of register(s) in a Sector 
		string lastErrorMessage;
		string lastErrorSource;

public:
	
		

		FCB(Disk * d = NULL);
		FCB (Disk * d, DirEntry fileDesc, DATtype FAT, Sector * Buffer, unsigned long currRecNr, uint currRecNrInBuff, enumsFMS::FCBtypeToOpening type);
		~FCB(){ 
			closefile();
			
		}
		


		/*Gets functions*/

		bool islastRecord();

		unsigned long getKey();

		char ** getAllFile();

		unsigned long fileTotalRec() { return fileDesc.eofRecNr; }

		uint getRecSize() { return fileDesc.actualRecSize; }

		string& GetLastErrorMessage() { return this->lastErrorMessage; }

		string& GetLastErrorSource() { return this->lastErrorSource; }

		DirEntry * getfileDesc() { return &fileDesc;}

		
		/****/
		
		void SetLastErrorMessage(string lastErrorMessage) { this->lastErrorMessage = lastErrorMessage; }

		void SetLastErrorSource(string lastErrorSource) { this->lastErrorSource = lastErrorSource; }

		void closefile();

		void flushfile();

		void read(char *, bool update = false);

		void write(char * data,int recordInFile = -1 );

		void seek(enumsFMS::FCBseekfrom, int = 0);

		void updateCancel();

		void deleteRec();

		void update(char *);

		friend class Disk;

		friend class ExternalFile;
};

