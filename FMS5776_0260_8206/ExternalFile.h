#pragma once
#include "Header.h"
#include "FCB.h"


class ExternalFileSec
{
private:
	unsigned short id;
	char filemem[1018]{ '\0' };
public:
	ExternalFileSec();
	~ExternalFileSec();

	int LastmemUsed();

	friend class ExternalFile;

};

class ExternalFile
{
private:
	char * path;							// The path or file name
	int fileSizeInDisk;						// The Size of the file into the disk
	int fileSize;							// Real file size
	int lastdata;							// Total of data into last char
	fstream file;							// fstream object to help import/export files
	ExternalFileSec * fileMemory;			// All file into  a memory
	FCB * fileInDisk;						// The file fcb

	double *status = NULL;							//To dll

public:

	ExternalFile();
	ExternalFile(const char * path);
	~ExternalFile();
	// Static function return file size (Sector) into the disk  
	static int calculateFileSize(const char * path)
	{
		ifstream file(path, ios::binary | ios::ate);
		if (!file.is_open())
			throw ProgramExeption("Wrong path:", "calculateFileSize");
	
		long fileSize = (long) file.tellg();

		file.close();

		if (fileSize % 1018 == 0)											// Add 1 to the fileHeader
			return fileSize / 1018 + 1;
		else
			return (fileSize / 1018) + 2;
	}
	// Static function return real file size (bytes)
	static unsigned long getFileSize(const char * path)
	{
		ifstream file(path, ios::binary | ios::ate);
		if (!file.is_open())
			throw ProgramExeption("Wrong path", "calculateFileSize");

		long fileSize = (long)file.tellg();

		file.close();

		return fileSize;
		
	}
	//Import file to fileMemory object
	void importFile();
	//Export file from fileMemory to disk through fcb
	void importFromFcb(FCB * fcb);
	//Import file from the disk through fcb fileMemory 
	void exportToFcb(FCB * fcb);
	//Export file fileMemory to pc
	void exportFile(char * exPatch);


	void setStatus(double * satus){
		this->status=satus;
	}
};



