#pragma warning(disable: 4297)
#include "Disk.h"
#include <string>       // std::string
#include <iostream>     // std::cout
#include <sstream>  
#include "Students.h"
#include "ExternalFile.h"



extern "C"
{

	//errors
	__declspec(dllexport) const  char*  getLastDiskErrorMessage(Disk* THIS)
	{
		const char* str = THIS->GetLastErrorMessage().c_str();
		return str;
	}
	__declspec(dllexport) const  char*  getLastFcbErrorMessage(FCB* THIS)
	{
		const char* str = THIS->GetLastErrorMessage().c_str();
		return str;
	}
	__declspec(dllexport) const  char*  getLastDiskErrorSource(Disk* THIS)
	{
		const char* str = THIS->GetLastErrorSource().c_str();
		return str;
	}
	__declspec(dllexport) const  char*  getLastFcbErrorSource(FCB* THIS)
	{
		const char* str = THIS->GetLastErrorSource().c_str();
		return str;
	}

	// init disk
	__declspec(dllexport) Disk*  __stdcall makeDiskObject()
	{
		return new Disk();
	}
	__declspec(dllexport) void  deleteDiskObject(Disk*& THIS)
	{
		if (THIS != NULL)
			delete  THIS;
		THIS = NULL;
	}

	__declspec(dllexport) void  deleteFcbObject(FCB*& THIS)
	{
		if (THIS != NULL)
			delete  THIS;
		THIS = NULL;
	}

	// Level 0
	__declspec(dllexport) void  createdisk(Disk* THIS, char* diskName, char* diskOwner)
	{
		try
		{
			THIS->createdisk(diskName, diskOwner);
		}
		catch (ProgramExeption ex)
		{
			THIS->SetLastErrorMessage(ex.what());
			THIS->SetLastErrorSource(ex.Getsource());
			throw ex;
		}
		catch (exception ex)
		{
			THIS->SetLastErrorMessage(ex.what());
			THIS->SetLastErrorSource("Dll");
			throw ex;
		}
	}
	__declspec(dllexport) void  mountdisk(Disk* THIS, char* diskName)
	{
		try
		{
			THIS->mountdisk(diskName);
		}
		catch (ProgramExeption ex)
		{
			THIS->SetLastErrorMessage(ex.what());
			THIS->SetLastErrorSource(ex.Getsource());
			throw ex;
		}
		catch (exception ex)
		{
			THIS->SetLastErrorMessage(ex.what());
			THIS->SetLastErrorSource("Dll");
			throw ex;
		}
	}
	__declspec(dllexport) void  unmountdisk(Disk* THIS)
	{
		try
		{
			THIS->unmountdisk();
		}
		catch (ProgramExeption ex)
		{
			THIS->SetLastErrorMessage(ex.what());
			THIS->SetLastErrorSource(ex.Getsource());
			throw ex;
		}
		catch (exception ex)
		{
			THIS->SetLastErrorMessage(ex.what());
			THIS->SetLastErrorSource("Dll");
			throw ex;
		}
	}
	__declspec(dllexport) void  recreatedisk(Disk* THIS, char* diskOwner)
	{
		try
		{
			THIS->recreatedisk(diskOwner);
		}
		catch (ProgramExeption ex)
		{
			THIS->SetLastErrorMessage(ex.what());
			THIS->SetLastErrorSource(ex.Getsource());
			throw ex;
		}
		catch (exception ex)
		{
			THIS->SetLastErrorMessage(ex.what());
			THIS->SetLastErrorSource("Dll");
			throw ex;
		}
	}

	// Level 1
	__declspec(dllexport) void  format(Disk* THIS, char* diskOwner)
	{
		try
		{
			THIS->format((string)diskOwner);
		}
		catch (ProgramExeption ex)
		{
			THIS->SetLastErrorMessage(ex.what());
			THIS->SetLastErrorSource(ex.Getsource());
			throw ex;
		}
		catch (exception ex)
		{
			THIS->SetLastErrorMessage(ex.what());
			THIS->SetLastErrorSource("Dll");
			throw ex;
		}
	}
	__declspec(dllexport) int   howmuchempty(Disk* THIS)
	{
		try
		{
			return THIS->howmuchempty();
		}
		catch (ProgramExeption ex)
		{
			THIS->SetLastErrorMessage(ex.what());
			THIS->SetLastErrorSource(ex.Getsource());
			throw ex;
		}
		catch (exception ex)
		{
			THIS->SetLastErrorMessage(ex.what());
			THIS->SetLastErrorSource("Dll");
			throw ex;
		}
	}

	//Level 2

	//Create student
	__declspec(dllexport) void  createStudentfile(Disk* THIS, char* fileName, char* fileOwner, int fileSize, int recSize = 0)
	{
		try
		{

			if (recSize == 0)
				recSize = sizeof(Student);

			string name = ((string)fileName).substr(0, 6);
			name += ".stud";

			THIS->createfile(name, (string)fileOwner, false,recSize, fileSize,"I", 0, 4);
		}
		catch (ProgramExeption ex)
		{
			THIS->SetLastErrorMessage(ex.what());
			THIS->SetLastErrorSource(ex.Getsource());
			throw ex;
		}
		catch (exception ex)
		{
			THIS->SetLastErrorMessage(ex.what());
			THIS->SetLastErrorSource("Dll");
			throw ex;
		}
	}
	//Import file from the pc
	__declspec(dllexport) void  importFile(Disk* THIS, char* path, char* fileName, char* fileOwner)
	{
		bool created = false;
		try
		{
			string allPath = path;
			allPath += "\\";
			allPath += fileName;
			if(strlen(fileName) > 11)
				strncpy_s(fileName, 12, &fileName[strlen(fileName) - 11], _TRUNCATE);
			
			ExternalFile file(allPath.c_str());
			file.setStatus(THIS->getStatus());
			THIS->createfile((string)fileName,(string) fileOwner, false, sizeof(ExternalFileSec), ExternalFile::calculateFileSize(allPath.c_str()), "I", 0, 2);
			created = true;
			file.importFile();
			file.exportToFcb(THIS->openfile((string)fileName, (string)fileOwner, enumsFMS::FCBtypeToOpening::inputOutput));
		}
		catch (ProgramExeption ex)
		{
			if (created)
				THIS->delfile(((string)fileName), (string)fileOwner);
			THIS->SetLastErrorMessage(ex.what());
			THIS->SetLastErrorSource(ex.Getsource());
			throw ex;
		}
		catch (exception ex)
		{
			THIS->SetLastErrorMessage(ex.what());
			THIS->SetLastErrorSource("Dll");
			throw ex;
		}
	}

	
	__declspec(dllexport) void  delfile(Disk* THIS, char* fileName, char* fileOwner)
	{
		try
		{
			THIS->delfile((string)fileName, (string)fileOwner);
		}
		catch (ProgramExeption ex)
		{
			THIS->SetLastErrorMessage(ex.what());
			THIS->SetLastErrorSource(ex.Getsource());
			throw ex;
		}
		catch (exception ex)
		{
			THIS->SetLastErrorMessage(ex.what());
			THIS->SetLastErrorSource("Dll");
			throw ex;
		}
	}
	__declspec(dllexport) void  extendfile(Disk* THIS, char* fileName, char* fileOwner, unsigned int size)
	{
		try
		{
			THIS->extendfile((string)fileName, (string)fileOwner, size);
		}
		catch (ProgramExeption ex)
		{
			THIS->SetLastErrorMessage(ex.what());
			THIS->SetLastErrorSource(ex.Getsource());
			throw ex;
		}
		catch (exception ex)
		{
			THIS->SetLastErrorMessage(ex.what());
			THIS->SetLastErrorSource("Dll");
			throw ex;
		}
	}


	// Level 3
	__declspec(dllexport)  FCB *  openfile(Disk* THIS, char* fileName, char* fileOwner, int openMode)
	{
		try
		{
			return THIS->openfile(fileName, fileOwner, (enumsFMS::FCBtypeToOpening) openMode);
		}
		catch (ProgramExeption ex)
		{
			THIS->SetLastErrorMessage(ex.what());
			THIS->SetLastErrorSource(ex.Getsource());
			throw ex;
		}
		catch (exception ex)
		{
			THIS->SetLastErrorMessage(ex.what());
			THIS->SetLastErrorSource("Dll");
			throw ex;
		}
	}


	// FCB
	__declspec(dllexport) void  closefile(FCB* THIS)
	{
		try
		{
			THIS->closefile();
		}
		catch (ProgramExeption ex)
		{
			THIS->SetLastErrorMessage(ex.what());
			THIS->SetLastErrorSource(ex.Getsource());
			throw ex;
		}
		catch (exception ex)
		{
			THIS->SetLastErrorMessage(ex.what());
			THIS->SetLastErrorSource("Dll");
			throw ex;
		}
	}
	__declspec(dllexport) void  readRec(FCB* THIS, char * dest, unsigned int readForUpdate)
	{
		try
		{
			THIS->read(dest, (readForUpdate != 0) ? true : false);
		}
		catch (ProgramExeption ex)
		{
			THIS->SetLastErrorMessage(ex.what());
			THIS->SetLastErrorSource(ex.Getsource());
			throw ex;
		}
		catch (exception ex)
		{
			THIS->SetLastErrorMessage(ex.what());
			THIS->SetLastErrorSource("Dll");
			throw ex;
		}
	}
	__declspec(dllexport) void  writeRec(FCB* THIS, char * source)
	{
		try
		{
			THIS->write(source);
		}
		catch (ProgramExeption ex)
		{
			THIS->SetLastErrorMessage(ex.what());
			THIS->SetLastErrorSource(ex.Getsource());
			throw ex;
		}
		catch (exception ex)
		{
			THIS->SetLastErrorMessage(ex.what());
			THIS->SetLastErrorSource("Dll");
			throw ex;
		}
	}
	__declspec(dllexport) void  seekRec(FCB* THIS, unsigned int from, int pos)
	{
		try
		{
			THIS->seek((enumsFMS::FCBseekfrom)from, pos);
		}
		catch (ProgramExeption ex)
		{
			THIS->SetLastErrorMessage(ex.what());
			THIS->SetLastErrorSource(ex.Getsource());
			throw ex;
		}
		catch (exception ex)
		{
			THIS->SetLastErrorMessage(ex.what());
			THIS->SetLastErrorSource("Dll");
			throw ex;
		}
	}
	__declspec(dllexport) void  seekToRecId(FCB* THIS, unsigned long id)
	{
		try
		{
			THIS->seekToRecId(id);
		}
		catch (ProgramExeption ex)
		{
			THIS->SetLastErrorMessage(ex.what());
			THIS->SetLastErrorSource(ex.Getsource());
			throw ex;
		}
		catch (exception ex)
		{
			THIS->SetLastErrorMessage(ex.what());
			THIS->SetLastErrorSource("Dll");
			throw ex;
		}
	}

	__declspec(dllexport) void  updateRecCancel(FCB* THIS)
	{
		try
		{
			THIS->updateCancel();
		}
		catch (ProgramExeption ex)
		{
			THIS->SetLastErrorMessage(ex.what());
			THIS->SetLastErrorSource(ex.Getsource());
			throw ex;
		}
		catch (exception ex)
		{
			THIS->SetLastErrorMessage(ex.what());
			THIS->SetLastErrorSource("Dll");
			throw ex;
		}
	}
	__declspec(dllexport) void  deleteRec(FCB* THIS)
	{
		try
		{
			THIS->deleteRec();
		}
		catch (ProgramExeption ex)
		{
			THIS->SetLastErrorMessage(ex.what());
			THIS->SetLastErrorSource(ex.Getsource());
			throw ex;
		}
		catch (exception ex)
		{
			THIS->SetLastErrorMessage(ex.what());
			THIS->SetLastErrorSource("Dll");
			throw ex;
		}
	}
	__declspec(dllexport) void  updateRec(FCB* THIS, char * source)
	{
		try
		{
			THIS->update(source);
		}
		catch (ProgramExeption ex)
		{
			THIS->SetLastErrorMessage(ex.what());
			THIS->SetLastErrorSource(ex.Getsource());
			throw ex;
		}
		catch (exception ex)
		{
			THIS->SetLastErrorMessage(ex.what());
			THIS->SetLastErrorSource("Dll");
			throw ex;
		}
	}
	__declspec(dllexport) void  getfileDesc(FCB* THIS, DirEntry * buffer)
	{
		try
		{
			if (THIS->getfileDesc() == NULL)
			{
				THIS->SetLastErrorMessage("File doesn't exist");
				THIS->SetLastErrorSource("Dll::getfileDesc");
				throw;
			}

			memcpy_s(buffer, sizeof(DirEntry), THIS->getfileDesc(), sizeof(DirEntry));

		}
		catch (ProgramExeption ex)
		{
			THIS->SetLastErrorMessage(ex.what());
			THIS->SetLastErrorSource(ex.Getsource());
			throw ex;
		}
		catch (exception ex)
		{
			THIS->SetLastErrorMessage(ex.what());
			THIS->SetLastErrorSource("Dll");
			throw ex;
		}
	}


	// extra

	__declspec(dllexport)   void  getVolumeHeader(Disk* THIS, VolumeHeader * buffer)
	{
		memcpy_s(buffer, sizeof(VolumeHeader), &THIS->getVolumeHeader(), sizeof(VolumeHeader));
	}


	__declspec(dllexport)   void  getDirEntry(Disk* THIS, DirEntry * buffer, int index)
	{
		try
		{
			if (THIS->getDirEntry(index) == NULL)
			{
				THIS->SetLastErrorMessage("File doesn't exist");
				THIS->SetLastErrorSource("Dll::getDirEntry");
				throw;
			}

			memcpy_s(buffer, sizeof(DirEntry), THIS->getDirEntry(index), sizeof(DirEntry));

		}
		catch (ProgramExeption ex)
		{
			THIS->SetLastErrorMessage(ex.what());
			THIS->SetLastErrorSource(ex.Getsource());
			throw ex;
		}
		catch (exception ex)
		{
			THIS->SetLastErrorMessage(ex.what());
			THIS->SetLastErrorSource("Dll");
			throw ex;
		}
	}

	__declspec(dllexport) int  dirExist(Disk* THIS, int index)
	{
		try
		{
			return THIS->dirExist(index);
		}
		catch (ProgramExeption ex)
		{
			THIS->SetLastErrorMessage(ex.what());
			THIS->SetLastErrorSource(ex.Getsource());
			throw ex;
		}
		catch (exception ex)
		{
			THIS->SetLastErrorMessage(ex.what());
			THIS->SetLastErrorSource("Dll");
			throw ex;
		}
	}

	__declspec(dllexport) int  isMounted(Disk* THIS)
	{
		try
		{
			return THIS->getMounted();
		}
		catch (ProgramExeption ex)
		{
			THIS->SetLastErrorMessage(ex.what());
			THIS->SetLastErrorSource(ex.Getsource());
			throw ex;
		}
		catch (exception ex)
		{
			THIS->SetLastErrorMessage(ex.what());
			THIS->SetLastErrorSource("Dll");
			throw ex;
		}
	}

	__declspec(dllexport) void  exportFile(Disk* THIS, char * exportDir, char* fileName, char * fileOwner , FCB * fcb = NULL)
	{
		bool created = false;
		try
		{
			ExternalFile file(fileName);
			file.setStatus(THIS->getStatus());
			if (fcb == NULL)
				fcb = THIS->openfile((string)fileName, (string)fileOwner, enumsFMS::FCBtypeToOpening::inputOutput);
			file.importFromFcb(fcb);
			
			file.exportFile(exportDir);
		}
		catch (ProgramExeption ex)
		{
			if (created)
				THIS->delfile(((string)fileName), (string)fileOwner);
			THIS->SetLastErrorMessage(ex.what());
			THIS->SetLastErrorSource(ex.Getsource());
			throw ex;
		}
		catch (exception ex)
		{
			THIS->SetLastErrorMessage(ex.what());
			THIS->SetLastErrorSource("Dll");
			throw ex;
		}
	}
	
	__declspec(dllexport) double  getStatus(Disk * d)
	{
		
			return *d->getStatus();
		
	}

	
	__declspec(dllexport) const char*  getDat(Disk* THIS)
	{
		char * str = new char[1601];
		for (int i = 0; i < 1600; i++)
		{
			str[i] = (THIS->getDatDAt()[i]) ?(char) 49 :(char)48;
		}
		str[1600] = '\0';
		return str;
	}
	
}
