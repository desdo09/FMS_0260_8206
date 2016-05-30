#include "ExternalFile.h"
#include <chrono>
#include <thread>
#pragma warning(disable: 4244)

ExternalFileSec::ExternalFileSec()
{
}

ExternalFileSec::~ExternalFileSec()
{
}

int ExternalFileSec::LastmemUsed()
{
	int last = 0;
	bool data = true;
	for (int i = 0; i < 1018; i++)
	{
		if (this->filemem[i] != '\0')
			data = true;

		if (this->filemem[i] == '\0' && data == true)
		{
			last = i;
			data = false;
		}
	}

	return last;
}

/*************************************************
*
*				  ExternalFile
*
**************************************************/


ExternalFile::ExternalFile()
{
	this->path = NULL;
	fileSizeInDisk = 0;
	status = 0;
}
ExternalFile::ExternalFile(const char * path)
{
	this->path = (char *)path;
	fileSizeInDisk = 0;
	status = 0;
}


ExternalFile::~ExternalFile()
{
	if (file.is_open())
		file.close();
	if (this->status != NULL)
		*status = 0;
}

void ExternalFile::importFile()
{
	if (path == NULL)
		throw ProgramExeption("File name error", "ExternalFile::importFile");

	fileSizeInDisk = ExternalFile::calculateFileSize(path) - 1;

	this->fileSize = ExternalFile::getFileSize(path);

	file.open(this->path, ios::in | ios::binary);

	if (!file.is_open())
		throw ProgramExeption("file problem", "ExternalFile::importFile");

	fileMemory = new ExternalFileSec[fileSizeInDisk];
	if (status != NULL)
		*status = 0;
	for (int i = 0; i < fileSizeInDisk; i++)
	{

		if (i != fileSizeInDisk - 1)
			file.read(fileMemory[i].filemem, 1018);
		else
		{
			this->lastdata = fileSize - file.tellg();
			file.read(fileMemory[i].filemem, lastdata);
		}

		fileMemory[i].id = i + 1;
		if (status != NULL)
			*status = (i / (fileSizeInDisk - 1)) * 100;
	}

	file.close();

}

void ExternalFile::importFromFcb(FCB * fcb)
{
	if (fcb == NULL)
		throw ProgramExeption("Fcb cannot be null", "ExternalFile::importFromFcb");


	fileSize = fcb->fileDesc.maxRecSize;
	fileSizeInDisk = fcb->fileDesc.fileSize;
	this->path = fcb->fileDesc.filename;
	this->lastdata = fcb->fileDesc.maxRecSize;

	fileMemory = new ExternalFileSec[fileSizeInDisk];

	fcb->seek(enumsFMS::FCBseekfrom::beginning);
	if (status != NULL)
		*status = 0;
	for (int i = 0; i < fileSizeInDisk; i++)
	{
				
		fcb->read((char *)&fileMemory[i]);
				
		if (status != NULL)
			*status = (i / (fileSizeInDisk - 1)) * 100;
	}


}

void ExternalFile::exportToFcb(FCB * fcb)
{
	if (fcb == NULL)
		throw ProgramExeption("Fcb cannot be null", "ExternalFile::exportToFcb");
	if (fileSizeInDisk == 0)
		throw ProgramExeption("Import file before", "ExternalFile::exportToFcb");
	if (status != NULL)
		*status = 0;
	for (int i = 0; i < fileSizeInDisk; i++)
	{
		fcb->write((char *)&fileMemory[i]);
		if (status != NULL)
			*status = (i / (fileSizeInDisk - 1)) * 100;
	}

	fcb->fileDesc.maxRecSize = fileSize;
	fcb->fileDesc.fileSize = fileSizeInDisk;
}

void ExternalFile::exportFile(char * exPatch)
{
	if (exPatch == NULL)
		throw ProgramExeption("File name error", "ExternalFile::exportFile");

	string dir;
	if (exPatch != NULL)
	{
		dir = exPatch;
		dir += "\\";
		dir += path;
	}
	else
		dir = path;

	file.open(dir.c_str(), ios::out | ios::binary);

	if (!file.is_open())
		throw ProgramExeption("file problem", "ExternalFile::importFile");
	if (status != NULL)
		*status = 0;
	for (int i = 0; i < fileSizeInDisk; i++)
	{

		if (i != fileSizeInDisk - 1)
		{
			file.write(fileMemory[i].filemem, 1018);
		}
		else
		{
			file.write(fileMemory[i].filemem, lastdata);
		}

		if (status != NULL)
			*status = (i / (fileSizeInDisk - 1)) * 100;

		if (i == 200)
			std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}


}

