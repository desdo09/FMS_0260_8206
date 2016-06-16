#pragma once
#include "Disk.h"
#include "TestLevel_0.h"
#include "TestLevel_2.h"
#include "FCB.h"
#include "Students.h"
#include "ExternalFile.h"
using namespace std;






enum menu{	clear = 1,																				// Others
			testLevel0 , createDisk, mountDisk, unmountDisk, flush, printDiskDetails,     // Level 0
			printDat, formatDisk, allocate, allocateext, deallocate,								// Level 1
			createFile, getFile, extendFile, ShowFileFAT, deleteFile,								// Level 2
			newFCB, fileDetails,NStudent, IStudent, GStudent, CUStudent,UStudent,DStudent,			// Level 3
			SFiles,PAStudents,	
			defragment,importFile,exportFile,help
};

string Menu []{ " ",
	"clear",																						// Others
	"testLevel0", "create", "mount", "unmount", "flush", "printDisk",					// Level 0
	"DAT", "format", "allocate", "allocateext", "deallocate",										// Level 1
	"new", "get", "extend", "FAT", "del",															// Level 2
	"open", "details", "NStudent", "IStudent", "GStudent", "CUStudent", "UStudent", "DStudent",		// Level 3
	"attrib","Sattrib", "defragment","importFile","exportFile","help"
};



void testLevel1(Disk * d, DATtype & FAT, bool ext = false);
void DATprint(DATtype DAT,char type = 'D');
int Student::used = 0;
void main(int argc, char * argv[]) {
	
#pragma region objects



	 
	srand(time(0));
	//system("color a");

	string choice;

	char ans;

	char ** allData = NULL;

	TestLevel_0 a;


	Disk * disk = &a.d;

	DATtype FAT;

	FAT.set();

	FCB * fcb = NULL;

	RootDir rt;

	Student stud;

	ExternalFile exFile;

	short anstemp;

	int option = 0;
#pragma endregion

	cout << "File Manager [Version 1.0]" << endl;
	cout << "(c) 2016 David Aben Athar and Jossef Chaim Yakopow\n\n";

	try
	{
		if (argc > 1)
		{
			
			disk->mountdisk(argv[1]);
			a.diskName = disk->getVolumeHeader().GetdiskName();
			a.ownerName = disk->getVolumeHeader().GetdiskOwner();

		}
		
		
	}
	catch (const std::exception& ex)
	{
		cout << ex.what() << endl;
	}
	

	do {




		cout << "\nDisk:\\>";

		cin >> choice;

		try
		{
			if (!choice.compare("exit"))
				throw string();

			
			
			option = atoi(choice.c_str());

			if (!atoi(choice.c_str()))
			{
				for (int i = 0; i < 32; i++)
				{
					if (!choice.compare(Menu[i]))
					{
						option = i;
						break;
					}
				}
			}


			switch (option)
			{
			
			case createDisk:
				a.test_create();
				cout << "Disk name: " << a.diskName << endl;
				cout << "Disk owner: " << a.ownerName << endl;
				break;
			case mountDisk:
				cin >> choice;
				disk->mountdisk(choice);
				if (disk->getMounted())
				{
					cout << "Disk mounted!" << endl;
					a.diskName = disk->getVolumeHeader().GetdiskName();
					a.ownerName = disk->getVolumeHeader().GetdiskOwner();
					cout << "Disk name: " << a.diskName << endl;
					cout << "Disk owner: " << a.ownerName << endl;
				}
				break;
			case unmountDisk:
				disk->unmountdisk();
				cout << "unmounted!" << endl;
				break;
			case menu::flush:
				disk->flush();
				break;
			case formatDisk:
				cout << "are you sure <Y/N>? " << endl;
				cin >> ans;
				if (ans == 'Y' || ans == 'y')
				{
					disk->format(a.ownerName);
					cout << "Disk formated!" << endl;
				}
				break;
			case printDiskDetails:
				a.printDiskInfo();
				break;
			case testLevel0:
				a.printStructSize();
				break;
			case allocate:
				FAT.reset();
				testLevel1(disk,FAT);
				break;
			case allocateext:
				testLevel1(disk, FAT,true);
				break;
			case deallocate:
				disk->dealloc(FAT);
				cout << "deallocated! " << endl;
				break;
			case printDat:
				if (disk->getMounted())
					DATprint(disk->getDatDAt());
				else
					cout << "There is not a mounted disk" << endl;
				break;
			case createFile: 
				cin >> choice;
				TestLevel_2::createFile(disk, choice);
				break;
			case extendFile:
				TestLevel_2::extFile(disk);
				break;
			case getFile:
				TestLevel_2::printHeader(disk);
				break;
			case deleteFile:
				TestLevel_2::DeleteFile(disk,a.ownerName);
				break;
			case ShowFileFAT:
				FAT = TestLevel_2::getFAT(disk);
				DATprint(FAT,'F');
				break;
			case newFCB: 
				cin >> choice;
				fcb = disk->openfile(choice, a.ownerName, enumsFMS::FCBtypeToOpening::inputOutput);

				if (fcb != NULL)
					cout << "File " << choice << " is open" << endl;
				else
					cout << "File doesn't exist" << endl;
				break;
			case fileDetails:
				if (fcb != NULL)
					TestLevel_2::printHeader(fcb->getfileDesc());
				else
					cout << "File are not opened" << endl;
				break;
			case NStudent:
				cin >> choice;
				if (!choice.compare("V"))
				{
					for (int i = 0; i < 31; i++)
					{
						stud = Student::newStudentTest();
						stud.id = i + 1;
						stud.printStudent();

						if (fcb != NULL) {
							fcb->write((char *)&stud);
						}
						else
							cout << "File are not opened" << endl;
						cout << "Inserted!" << endl;
					}
					break;
				}
				if (!choice.compare("F"))
				{
					stud = Student::newStudentTest();
					stud.printStudent();
				}
			case IStudent:
				if (fcb != NULL) {
					fcb->write((char *)&stud);
				}
				else
					cout << "File are not opened" << endl;
				cout << "Inserted!" << endl;
				break;
			case GStudent:

				if (fcb != NULL)
				{
					cout << "Insert record number:" << endl;
					cin >> anstemp;
					fcb->seek(enumsFMS::FCBseekfrom::beginning, anstemp);
					fcb->read((char *)&stud, true);
					stud.printStudent();
				}
				else
					cout << "File are not opened" << endl;
				break;
			
			case CUStudent:
				fcb->updateCancel();
				cout << "Update canceled" << endl;
				break;
			case UStudent:
				stud.updateStudent();
				fcb->update((char *)&stud);
				break;
			case DStudent:
				cin >> choice;
				fcb->seekToRecId(atoi(choice.c_str()));
				fcb->deleteRec();
				choice = "";
				break;
			case SFiles:
				if (!disk->getMounted())
					throw exception("There no disk mounted");
				rt = disk->getRootDir();
				for (int i = 0; i < 28; i++)
				{
					if(rt[i]->active())
						cout <<"\t\t"<< rt[i]->getname() << endl;
					
				}
				
				break;

			case PAStudents:
				if (fcb == NULL)
					throw "no file open";
				cout << "Total rec in file --> " << fcb->fileTotalRec() << endl;
				allData = fcb->getAllFile();
				for (uint i = 0; i < fcb->fileTotalRec(); i++)
				{
					if (allData[i] != NULL)
					{
						memcpy(&stud, allData[i], sizeof(Student));
						stud.printStudent();
					}
				}
				break;
			case clear: std::system("cls"); break;

			case 
				defragment: disk->defragmentation();
				break;
			case exportFile: 
				
				exFile.importFromFcb(fcb);
				exFile.exportFile("C:\\Users\\User\\Desktop\\News");
				break;
			case help: 

				cout << "Select option:" << endl;
				cout << "\n\t\t<100/exit> Exit " << endl;
				cout << "\t\t<1/> Clear screen " << endl;
				cout << "\n\tLevel 0:" << endl;
				cout << "\t\t<2/testLevel0> Test level 0 " << endl;
				cout << "\t\t<3/create> Create disk " << endl;
				cout << "\t\t<4/mount> Mount Disk " << endl;
				cout << "\t\t<5/unmount> Unmount Disk " << endl;
				cout << "\t\t<6/flush> Release data into the Disk " << endl;
				cout << "\t\t<7/printDisk> Print disk details " << endl;
				cout << "\n\tLevel 1:" << endl;
				cout << "\t\t<8/DAT> Print DAT " << endl;
				cout << "\t\t<9/format> Format Disk " << endl;
				cout << "\t\t<10/allocate> Allocate sectors " << endl;
				cout << "\t\t<11/allocateext> Allocate last FAT " << endl;
				cout << "\t\t<12/deallocate> Deallocate last FAT " << endl;
				cout << "\n\tLevel 2:" << endl;
				cout << "\t\t<12/new> Create new file " << endl;
				cout << "\t\t<14/get> Get file header" << endl;
				cout << "\t\t<15/extend> Extend File " << endl;
				cout << "\t\t<16/FAT> Get FAT from a file " << endl;
				cout << "\t\t<17/deleteFile> Delete File " << endl;
				cout << "\n\tLevel 3:" << endl;
				cout << "\t\t<18/open> Get FCB " << endl;
				cout << "\t\t<19/details> Print file details " << endl;
				cout << "\t\t<20/NStudent> New Student  " << endl;
				cout << "\t\t<21/IStudent> Insert new Student  " << endl;
				cout << "\t\t<22/GStudent> Get Student by id " << endl;
				cout << "\t\t<23/CUStudent> Cancel student update " << endl;
				cout << "\t\t<24/UStudent> Update student " << endl;
				cout << "\t\t<25/DStudent> Delete Student by id " << endl;
				cout << "\t\t<26/attrib> Print all files " << endl;
				cout << "\t\t<27/Sattrib> Print all students " << endl;
				cout << "\t\t<28/defragment> Disk defragmentation " << endl;

				
				
				break;
				
			default:
				cout << "Invalid command, insert help to show the help menu" << endl;
				break;

			}


		


		}
		catch (ProgramExeption ex)
		{
			cout << ex.Getsource() << ":" << endl;
			cout << ex.what() << endl;
		}
		catch (exception ex)
		{
			cout << ex.what() << endl;
		}
		catch (char * msg)
		{
			string err(msg);
			cout << err << endl;
		}
		catch (...)
		{
			
		}

		


	} while (option != 100 && choice.compare("exit"));

	std::system("pause");

	
}

void testLevel1(Disk * d, DATtype & FAT ,bool ext)
{
	short algo;
	int amount;
	cout << "Insert amount of sector to allocate:" << endl;
	cin >> amount;
	cout << "Select algorithm type" << endl;
	do {
		cout << "<1>First fit <2> Best Fit <3> Worst Fit" << endl;
		cin >> algo;
	} while (algo <= 0 && algo > 4);
	if (ext)
		d->allocextend(FAT, amount, (Disk::AlgorithmType)algo);
	else
		d->alloc(FAT, amount, (Disk::AlgorithmType)algo);

	cout << "FAT:" << endl;
	DATprint(FAT,'F');

	
}

void DATprint(DATtype DAT, char type)
{
	
	for (int i = 0; i < amountOfSectors; i++)
	{
		if (type == 'D' && DAT[i] || type != 'D' && !DAT[i])
			cout << (char)219;
		else
			cout << (char)176;

		cout << '|';

		if (!((i + 1) % 50))
			cout << endl;
	}
	cout << endl;

}