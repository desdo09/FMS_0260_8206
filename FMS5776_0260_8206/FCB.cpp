#include "FCB.h"



FCB::FCB(Disk * d, DirEntry fileDesc, DATtype FAT, Sector * Buffer, uint currRecNr, uint currSecNr, uint currRecNrInBuff, enumsFMS::FCBtypeToOpening type)
{
	this->d = d;
	this->fileDesc = fileDesc;
	this->FAT = FAT;
	if (Buffer != NULL)
		this->Buffer = *Buffer;
	this->currRecNr = currRecNr;
	this->currSecNr = currSecNr;
	this->currRecNrInBuff = currRecNrInBuff;
	this->type = type;
	lock = false;
}
FCB::FCB(Disk * d)
{
	this->d = d;
	lock = false;
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
		
}


void FCB::read(char * Data, bool update)
{

	if (lock)
		throw ProgramExeption("Operation are not allowed", "FCB::read");

	if (d == NULL)
		throw ProgramExeption("There is no disk loaded", "FCB::read");

	if(type == enumsFMS::FCBtypeToOpening::input)
		throw ProgramExeption("The user have no permission to read", "FCB::read");
	
	Data = Buffer.getData();

	lock = update;

	//מיד אחרי קריאת הרשומה, הרשומה שאחריה הופכת להיות הנוכחית שיהיה אפשר לקרוא 
}

void FCB::write(char * data)
{
	if (lock)
		throw ProgramExeption("Operation are not allowed", "FCB::write");

	if (d == NULL)
		throw ProgramExeption("There is no disk loaded", "FCB::write");

	if (type == enumsFMS::FCBtypeToOpening::output)
		throw ProgramExeption("The user have no permission to write", "FCB::write");
	
	d->writeSector(new Sector(data));

	this->flushfile();
}

void FCB::seek(enumsFMS::FCBseekfrom from, int toIndex)
{
	if (lock)
		throw ProgramExeption("Operation are not allowed", "FCB::seek");

	if (d == NULL)
		throw ProgramExeption("There is no disk loaded", "FCB::seek");

	if(from == enumsFMS::FCBseekfrom::beginning && toIndex < 0 || from == enumsFMS::FCBseekfrom::eof && toIndex > 0)
		throw ProgramExeption("Operation are not allowed", "FCB::seek");

	if(fileDesc.recFormat == "V" && (from == enumsFMS::FCBseekfrom::current || toIndex != 0) )
		throw ProgramExeption("Operation are not allowed", "FCB::seek");

	switch (from)
	{
	case enumsFMS::beginning:
		d->seekToSector(FAT, toIndex);
		break;
	case enumsFMS::current:
		d->seekToSector(FAT,this->currSecNr + toIndex);
		break;
	case enumsFMS::eof:
		d->seekToSector(d->lastIndex(FAT));
		break;
	default:
		break;
	}

}

void FCB::updateCancel()
{
	if (d == NULL)
		throw ProgramExeption("There is no disk loaded", "FCB::updateCancel");

	lock = false;

}

void FCB::deleteRec()
{
}

void FCB::update(char *)
{
}



