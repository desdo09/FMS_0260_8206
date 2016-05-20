#include "Header.h"
#include "ExternalFile.h"
#include "Disk.h"
#include "FCB.h"
#include "DirEntry.h"
#include "Dir.h"



void main()
{
	try 
	{
		Disk d;
		FCB * fcb;
		ExternalFile * file;
	
		cout << sizeof(FileHeader) << endl;
		cout << sizeof(SectorDir)<<endl;
		cout << sizeof(DirEntry) << endl;

		d.mountdisk("C:\\Users\\User\\Desktop\\Disk.bin");

		//fcb = d.openfile("MG_0006.JPG", "David", enumsFMS::FCBtypeToOpening::inputOutput);

		//ExternalFile a("text.txt");

		//a.importFromFcb(fcb);
		//a.exportFile("C:\\Users\\User\\Desktop\\News");
		
		string str = "";
		for (int i = 0; i < 1600; i++)
		{
			str += (d.getDatDAt()[i]) ? ("1") : ("0");
		}
		cout << str<<endl;
	
	
	}
	catch (ProgramExeption ex)
	{
		cout << ex.Getsource() << ": " << ex.what()<<endl;
	}
	catch (exception ex)
	{
		cout << ex.what() << endl;
	}
	
	system("Pause");

}