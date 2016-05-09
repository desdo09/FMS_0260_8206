#include "FCB.h"
#include "Students.h"
using namespace enumsFMS;


FCB::FCB(Disk * d, DirEntry fileDesc, DATtype FAT, Sector * Buffer, unsigned long currRecNr, uint currRecNrInBuff, enumsFMS::FCBtypeToOpening type)
{
	if (d == NULL)
		return;
	this->FAT.reset();
	this->d = d;
	this->fileDesc = fileDesc;
	this->FAT = FAT;
	this->currSecNr = &d->currDiskSectorNr;
	if (Buffer != NULL)
		this->Buffer = *Buffer;
	else
	{
		seek(FCBseekfrom::beginning);
		this->Buffer.setSectorNumbe(*currSecNr);
	}
	this->currRecNr = currRecNr;
	this->currRecNrInBuff = currRecNrInBuff;
	this->type = type;
	lock = false;
}

FCB::FCB(Disk * d)
{
	this->d = d;
	lock = false;
	FAT.reset();
	seek(FCBseekfrom::beginning);
}

void FCB::closefile()
{
	if(lock)
		throw ProgramExeption("Operation are not allowed", "FCB::closefile");
	this->flushfile();
	d->unmountdisk();
	d = NULL;
}

void FCB::flushfile()
{
	if (lock)
		throw ProgramExeption("Operation are not allowed", "FCB::flushfile");
	if (d == NULL)
		throw ProgramExeption("There is no disk loaded", "FCB::flushfile");

	d->flush();

	d->updateFile(fileDesc);

	this->seek(FCBseekfrom::beginning, currRecNrInBuff);
		
}

void FCB::read(char * Data, bool update)
{

	if (lock)
		throw ProgramExeption("Operation are not allowed", "FCB::read");

	if (d == NULL)
		throw ProgramExeption("There is no disk loaded", "FCB::read");

	if(type == enumsFMS::FCBtypeToOpening::input)
		throw ProgramExeption("The user have no permission to read", "FCB::read");

	//check if the sector pointer was changed 
	if(this->currSecNrInBuff != *this->currSecNr)
		this->seek(FCBseekfrom::current, 0);

	if (currRecNr)
	{
		for (int i = 0; i < fileDesc.actualRecSize; i++)
		{
			*(Data + i) = *(Buffer.getData() + currRecNrInBuff*fileDesc.actualRecSize + i);			
		}
	}
	else
		throw ProgramExeption("No record found!", "FCB::read");

	lock = update;

	if (!lock) // in case is unlocked then seek to the next record
	{
		currRecNrInBuff++; 

		if (currRecNrInBuff / fileDesc.fileSize > 0) // in case that was the last record
			this->seek(enumsFMS::FCBseekfrom::current, 1); // seek to the next sector 

		this->currRecNr = getKey();
	
	}
}

void FCB::write(char * data, int recordInFile)
{
	if (lock)
		throw ProgramExeption("Operation are not allowed", "FCB::write");

	if (d == NULL)
		throw ProgramExeption("There is no disk loaded", "FCB::write");

	if (type == FCBtypeToOpening::output)
		throw ProgramExeption("The user have no permission to write", "FCB::write");
	
	if (recordInFile > 0)
	{
		this->seek(FCBseekfrom::beginning, recordInFile);
		if(this->currRecNr)
			throw ProgramExeption("Record in use", "FCB::write");
	
	}
	else
		this->seek(FCBseekfrom::eof, 0);


	char * alldata = this->Buffer.getData();

	memcpy(alldata + currRecNrInBuff*fileDesc.actualRecSize, data, fileDesc.actualRecSize);

	
	d->writeSector(&this->Buffer);

	currRecNrInBuff++;

	if (currRecNrInBuff > fileDesc.eofRecNr)
		fileDesc.eofRecNr = currRecNrInBuff;

	this->flushfile();

}

void FCB::seek(enumsFMS::FCBseekfrom from, int toRecord)
{
	
	if (lock)
		throw ProgramExeption("Operation are not allowed", "FCB::seek");

	if (d == NULL)
		throw ProgramExeption("There is no disk loaded", "FCB::seek");

	if(from == enumsFMS::FCBseekfrom::beginning && toRecord < 0 || from == enumsFMS::FCBseekfrom::eof && toRecord > 0)
		throw ProgramExeption("Operation are not allowed", "FCB::seek");

	if(fileDesc.recFormat == "V" && (from == enumsFMS::FCBseekfrom::current || toRecord != 0) )
		throw ProgramExeption("Operation are not allowed", "FCB::seek");

	if (from == enumsFMS::current)
			toRecord += this->currRecNrInBuff;
	
	if (from == enumsFMS::eof)
		toRecord = fileDesc.eofRecNr;

	int JumpToSector =													 // Amount sector to jump equal 
						( toRecord)										 // Amount records to jump	
							/											 // Divided by
						((sizeof(Sector) - 4) / fileDesc.actualRecSize); // Record per sector
	



	d->seekToSector(FAT, JumpToSector);

	this->d->readSector(&this->Buffer);

	this->currSecNrInBuff = *this->currSecNr;

	this->currRecNrInBuff = toRecord;
	
	this->currRecNr = getKey();



}

void FCB::updateCancel()
{
	if (d == NULL)
		throw ProgramExeption("There is no disk loaded", "FCB::updateCancel");

	lock = false;

}

void FCB::deleteRec()
{
	if (!lock)
		throw ProgramExeption("Operation are not allowed", "FCB::deleteRec");

	if (d == NULL)
		throw ProgramExeption("There is no disk loaded", "FCB::deleteRec");
		
	if (type == enumsFMS::FCBtypeToOpening::input || type == enumsFMS::FCBtypeToOpening::output)
		throw ProgramExeption("The user have no permission to delete", "FCB::deleteRec");
	
	//check if the sector pointer was changed 
	if (this->currSecNrInBuff != *this->currSecNr)
		this->seek(FCBseekfrom::current, 0);

	long zero = 0;

	memcpy((this->Buffer.getData() + currRecNrInBuff + fileDesc.keyOffset), &zero, fileDesc.keySize);
	
	lock = false;
	
	d->writeSector(&this->Buffer);
}

void FCB::update(char * update)
{
	if (!lock)
		throw ProgramExeption("Operation are not allowed", "FCB::update");

	if (d == NULL)
		throw ProgramExeption("There is no disk loaded", "FCB::update");

	if (type == FCBtypeToOpening::output || type == FCBtypeToOpening::input)
		throw ProgramExeption("The user have no permission to write", "FCB::update");

	if (currRecNrInBuff < fileDesc.eofRecNr || !getKey())
		throw ProgramExeption("No data found", "FCB::update");

	char * alldata = this->Buffer.getData();

	memcpy(alldata + currRecNrInBuff*fileDesc.actualRecSize, update, fileDesc.actualRecSize);

	d->writeSector(&this->Buffer);

	this->flushfile();
}

unsigned long FCB::getKey()
{

	unsigned long key = 0;

	memcpy(&key, (this->Buffer.getData() + currRecNrInBuff*fileDesc.actualRecSize + fileDesc.keyOffset), fileDesc.keySize);

	return key;
}



