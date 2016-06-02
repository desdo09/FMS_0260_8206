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
		DirEntry * fileDesc;						// The file data
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

		void findLastRecord();				    // Seek to the last record
		bool findEmptySpace();				    // Find an empty spot 
		int getSectorNrInDisk(int);				// Check in which sector is the record on disk


public:
	
		
		/*************************************************
		*
		*		  constructor and destructor
		*
		**************************************************/

		FCB(Disk * d = NULL);
		FCB (Disk * d, DirEntry * fileDesc, DATtype FAT, enumsFMS::FCBtypeToOpening type);
		~FCB(){ 
		
			this->closefile();
		}
		
		/*************************************************
		*
		*				 Gets functions
		*
		**************************************************/

		
		char ** getAllFile(); //Disabled

		unsigned long fileTotalRec() { return fileDesc->eofRecNr; }

		uint getRecSize() { return fileDesc->actualRecSize; }

		DirEntry * getfileDesc() { return fileDesc;}

		DATtype * getFAT() { return &this->FAT; };
	
		/*************************************************
		*
		*				 Public functions
		*
		**************************************************/


		/*************************************************
		* FUNCTION
		*    closefile
		*
		* RETURN VALUE
		*	This function doesn't return parameters
		*
		* PARAMETERS
		*	This function doesn't receive parameters
		*
		* MEANING
		*  The function will flush the data
		*
		**************************************************/
		void closefile();


		/*************************************************
		* FUNCTION
		*    flushfile
		*
		* RETURN VALUE
		*	This function doesn't return parameters
		*
		* PARAMETERS
		*	This function doesn't receive parameters
		*
		* MEANING
		*  The function will update the rootdir, the file
		*  header and flush the data.
		*
		**************************************************/
		void flushfile();

		/*************************************************
		* FUNCTION
		*    read
		*
		* RETURN VALUE
		*	This function doesn't return parameters
		*
		* PARAMETERS
		*   char * Data 		- The object where the data will be saved
		*	bool   update		- If the read operation is to update the data
		*
		* MEANING
		*     This functions will:
		*			+ Read the current register from the current sector in buffer 
		*			+ Transfer the data to the object
		*			+ Check if is to update:
		*				+ if not, then will go to the next record
		*				+ if yes, then lock the fcb
		*
		**************************************************/
		void read(char *, bool update = false);

		/*************************************************
		* FUNCTION
		*    write
		*
		* RETURN VALUE
		*	This function doesn't return parameters
		*
		* PARAMETERS
		*   char * Data 		 - The object where is the data
		*	[int   recordInFile] - write into the record number
		*
		* MEANING
		*  This functions will seek to the end of file or to an empty
		*  spot and then write the data

		*
		**************************************************/

		void write(char * data,int recordInFile = -1 );

		/*************************************************
		* FUNCTION
		*    seek
		*
		* RETURN VALUE
		*	This function doesn't return parameters
		*
		* PARAMETERS
		*   FCBseekfrom * from 	 - Start seek from
		*	int	 toRecord		 - how many records to jump
		*
		* MEANING
		*  This functions will seek through the file FAT
		*
		**************************************************/

		void seek(enumsFMS::FCBseekfrom, int = 0);

		/*************************************************
		* FUNCTION
		*    updateCancel
		*
		* RETURN VALUE
		*	This function doesn't return parameters
		*
		* PARAMETERS
		*	This function doesn't receive parameters
		*
		* MEANING
		*  This functions unlock the fcb in case the user
		*  don't want anymore to update a record
		*
		**************************************************/

		void updateCancel();

		/*************************************************
		* FUNCTION
		*    deleteRec
		*
		* RETURN VALUE
		*	This function doesn't return parameters
		*
		* PARAMETERS
		*	This function doesn't receive parameters
		*
		* MEANING
		*  This functions will delete the current record 
		*  to update and then unlock the fcb.
		*
		**************************************************/
		void deleteRec();

		/*************************************************
		* FUNCTION
		*    update
		*
		* RETURN VALUE
		*	This function doesn't return parameters
		*
		* PARAMETERS
		*   char * Data 		 - The object where is the data 
		*						   to update
		*
		* MEANING
		*  This functions will update the record, in case the user 
		*  did a read to update, then unlock the fcb 
		*
		**************************************************/
		void update(char * = NULL);

		/*************************************************
		* FUNCTION
		*    seekToRecId
		*
		* RETURN VALUE
		*	This function doesn't return parameters
		*
		* PARAMETERS
		*	int   id - The ID
		*
		* MEANING
		*  This functions will seek until find the record 
		*  with id receive
		*
		**************************************************/
		void seekToRecId(unsigned long id);

		/*************************************************
		* FUNCTION
		*    extendfile
		*
		* RETURN VALUE
		*	This function doesn't return parameters
		*
		* PARAMETERS
		*	int amountOfSectores - How many sectors to 
		*						   extend
		*
		* MEANING
		*  This functions will extend this file
		*
		**************************************************/
		void extendfile(uint amountOfSectores);

		/*************************************************
		* FUNCTION
		*    islastRecord
		*
		* RETURN VALUE
		*	True in case is the last record
		*
		* PARAMETERS
		*	This function doesn't receive parameters
		*
		* MEANING
		*  This functions will check if the current record is the last
		*
		**************************************************/		
		bool islastRecord();

		/*************************************************
		* FUNCTION
		*    getKey
		*
		* RETURN VALUE
		*	Return the record id
		*
		* PARAMETERS
		*	This function doesn't receive parameters
		*
		* MEANING
		*  This functions will get from the disk the record id
		*  and return it
		*
		**************************************************/
		unsigned long getKey();

		/*************************************************
		*
		*				  Functions to Dll
		*
		**************************************************/

		void SetLastErrorMessage(string lastErrorMessage) { this->lastErrorMessage = lastErrorMessage; }

		void SetLastErrorSource(string lastErrorSource) { this->lastErrorSource = lastErrorSource; }

		string & GetLastErrorMessage() { return this->lastErrorMessage; }

		string & GetLastErrorSource() { return this->lastErrorSource; }

		/*************************************************
		*
		*				 Friends
		*
		**************************************************/

		friend class Disk;

		friend class ExternalFile;
};



