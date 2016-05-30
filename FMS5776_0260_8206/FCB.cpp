#include "FCB.h"

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

	maxRecPerSec = (sizeof(Sector)-4) / fileDesc.actualRecSize;

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


/*Private functions*/

void FCB::findLastRecord()
{
	
	this->seek(FCBseekfrom::beginning, 0);
	for (uint i = 0; i < fileDesc.eofRecNr; i++)
	{
		
		if (!currRecNr)
			i--;
		if(i != fileDesc.eofRecNr - 1)
			this->seek(FCBseekfrom::current, 1);

	}
	
}

bool FCB::findEmptySpace()
{
	
	this->seek(FCBseekfrom::beginning, 0);
	for (uint i = 0; i < fileDesc.eofRecNr; i++)
	{
		if (!currRecNr)
			return true;

		this->seek(FCBseekfrom::current, 1);

	}
	return false;
}

int FCB::getSectorNrInDisk(int secNumber)
{
	secNumber += 1;																						// the file header

	int i;
	//Get the file first sector
	for (i = (this->d->firstIndex(this->FAT, false) / 2); i < amountOfSectors; i++)
	{

		
		if (FAT[i])																						// if is last than 2 then stop 
			secNumber -= 2;																				// 2 sectors	

		if (secNumber == -1)
			return  i * 2 + 1;
	
		if (secNumber == -2)
			return  i * 2;
	

	}

																				
	return -1;												

																				
}

/*Private functions*/

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
	delete d;
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
	if (this->currSecNrInBuff != *this->currSecNr)
		this->seek(FCBseekfrom::current, 0);
	else
		this->currRecNr = getKey();

	if (currRecNr)
	{
		for (uint i = 0; i < fileDesc.actualRecSize; i++)
		{
			*(Data + i) = *(Buffer.getData() + (currRecNrInBuff % maxRecPerSec)*fileDesc.actualRecSize + i);
		}
	}
	else
	{
		this->seek(FCBseekfrom::current, 1);
		this->read(Data, update);
		return;
	}

	lock = update;

	if (!lock) // in case is unlocked then seek to the next record
	{        
		if(currRecNrInBuff + 1 != fileDesc.eofRecNr)
			this->seek(enumsFMS::FCBseekfrom::current, 1); // seek to the next sector 
		else
			this->seek(enumsFMS::FCBseekfrom::beginning, 0); // seek to the next sector 

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

	if(maxRecPerSec*((FAT.count()*2)-1) <= fileDesc.eofRecNr)
		throw ProgramExeption("The file is full", "FCB::write");
	
	if (recordInFile > 0)
	{
		this->seek(FCBseekfrom::beginning, recordInFile);
		if(this->currRecNr)
			throw ProgramExeption("Record in use", "FCB::write");
	
	}
	else
		this->seek(FCBseekfrom::eof, 0);


	char * alldata = this->Buffer.getData();


	memcpy(alldata + (currRecNrInBuff % maxRecPerSec)*fileDesc.actualRecSize, data, fileDesc.actualRecSize);

	
	d->writeSector(Buffer.getSectorNumber(),&this->Buffer);

	fileDesc.eofRecNr++;

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
	{
		findLastRecord();
		//check if is not the last sector
		if (currSecNrInBuff >= d->lastIndex(FAT, false) && currRecNrInBuff%maxRecPerSec == maxRecPerSec - 1)
		{
			if (!findEmptySpace())
				throw ProgramExeption("File is full", "FCB::seek");
		}
		else
			if(fileDesc.eofRecNr)
				this->seek(FCBseekfrom::current, 1);
		return;
	}

	int JumpToSector = getSectorNrInDisk(toRecord / maxRecPerSec);

	if (JumpToSector == -1)
		throw ProgramExeption("The file cannot seek to this record", "FCB::seek");

	
	if (JumpToSector != currSecNrInBuff || currSecNrInBuff != *currSecNr)
	{
		d->seekToSector(JumpToSector);

		this->d->readSector(&this->Buffer);
	}

	this->currSecNrInBuff = *this->currSecNr;

	this->currRecNrInBuff = toRecord;

	if(fileDesc.eofRecNr>0)
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
		throw ProgramExeption("Operation are not allowed", "FCB::update");
	
	if (d == NULL)
		throw ProgramExeption("There is no disk loaded", "FCB::deleteRec");
		
	if (type == enumsFMS::FCBtypeToOpening::input || type == enumsFMS::FCBtypeToOpening::output)
		throw ProgramExeption("The user have no permission to delete", "FCB::deleteRec");
	
	//check if the sector pointer was changed 
	if (this->currSecNrInBuff != *this->currSecNr)
		this->seek(FCBseekfrom::current, 0);

	if (!currRecNr)
		return;

	long zero = 0;

	memcpy((this->Buffer.getData() + (currRecNrInBuff % maxRecPerSec)*fileDesc.actualRecSize + fileDesc.keyOffset), &zero, fileDesc.keySize);
	
	lock = false;

	d->writeSector(Buffer.getSectorNumber(),&this->Buffer);
	
	fileDesc.eofRecNr--;

	this->flushfile();
}

void FCB::update(char * update)
{
	if (!lock)
		throw ProgramExeption("Operation are not allowed", "FCB::update");

	if (d == NULL)
		throw ProgramExeption("There is no disk loaded", "FCB::update");

	if (type == FCBtypeToOpening::output || type == FCBtypeToOpening::input)
		throw ProgramExeption("The user have no permission to write", "FCB::update");

	if (!currRecNr)
		throw ProgramExeption("No data found", "FCB::update");

	char * alldata = this->Buffer.getData();

	memcpy(alldata + (currRecNrInBuff % maxRecPerSec)*fileDesc.actualRecSize, update, fileDesc.actualRecSize);

	d->writeSector(Buffer.getSectorNumber(),&this->Buffer);

	lock = false;

	this->flushfile();
}

bool FCB::islastRecord()
{
	return (this->currRecNr == fileDesc.eofRecNr);
	
}

unsigned long FCB::getKey()
{

	unsigned long key = 0;

	memcpy(&key, (this->Buffer.getData() + (currRecNrInBuff % maxRecPerSec)*fileDesc.actualRecSize + fileDesc.keyOffset), fileDesc.keySize);

	return key;
}

char ** FCB::getAllFile()
{
	if (fileDesc.eofRecNr == 0)
		return NULL;
	char ** file;
	if(fileDesc.eofRecNr>1)
			file = new char *[fileDesc.eofRecNr];
	else
			file = new char *;


	this->seek(FCBseekfrom::beginning, 0);
	for (uint i = 0; i < fileDesc.eofRecNr; i++)
	{
		if (currRecNr)		//check if the key is not 0
		{
			file[i] = new char[fileDesc.actualRecSize];
			this->read(file[i]);
		}
		else
		{
			file[i] = NULL;
			this->seek(FCBseekfrom::current, 1);
		}
	}
	return file;
}

void FCB::seekToRecId(unsigned long id)
{
	if (fileDesc.eofRecNr == 0)
		throw ProgramExeption("File empty","FCB::seekToRecId");

	

	this->seek(FCBseekfrom::beginning, 0);
	for (uint i = 0; i < fileDesc.eofRecNr; i++)
	{
		if (!currRecNr)
			i--;

		if (currRecNr != id)
			this->seek(FCBseekfrom::current, 1);
		else
			return;
		
	}
	throw ProgramExeption("id doesn't exist", "FCB::seekToRecId");
}

void FCB::extendfile(uint amountOfSectores)
{
	this->d->extendfile((string) this->fileDesc.filename,(string) this->fileDesc.fileOwner, amountOfSectores,this);
	this->flushfile();
}


