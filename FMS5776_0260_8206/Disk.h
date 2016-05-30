#pragma once
#include "Header.h"
#include "VolumeHeader.h"
#include "Dir.h"
#include "DAT.h"
#include "Sector.h"
#include "FileHeader.h"
#include "FCB.h"

class Disk
{
private:

	VolumeHeader  vhd;
	DAT           dat;
	RootDir       rootdir;
	bool          mounted = false;
	fstream       dskfl;
	uint          currDiskSectorNr;
	Sector        buffer;
	string		  lastErrorMessage;
	string		  lastErrorSource;
	double		  status = 0;			//For dll
	/*************************************************
	*
	*				Private function from
	*						Level 1
	*
	**************************************************/
	/*
		Theses tree functions return the index of the 
		first sector free to allocate according to the 
		algorithm
	*/
	uint firstFit(uint, uint);
	uint bestFit(uint, uint);
	uint worstFit(uint, uint);

	/*************************************************
	*
	*				Private function from
	*						Level 2
	*
	**************************************************/

	/*
		Theses functions returns the first/last index of the FAT/DAT
	*/
	int firstIndex(DATtype DAT, bool isDAT = true, uint indexStart = 0);
	int lastIndex(DATtype DAT, bool isDAT = true);

public:
	enum ConstructorCod
	{
		Create = 'c',
		Mount = 'm',
		
	};
	enum AlgorithmType { first_Fit=1, best_Fit, worst_Fit};

	

	/*************************************************
	* CONSTRUCTOR
	*    Default constructor
	*
	**************************************************/
	Disk();
	/*************************************************
	* CONSTRUCTOR
	*    constructor with 3 parameters
	* PARAMETERS
	*    diskName – The name of the disk that will be created
	*	 owner    - The disk owner name
	*	 cod      - The action that the constructor will do
	*
	* MEANING
	*	In case the constructor receive m:
	*		The constructor will mount the disk
	*	In case the constructor receive c:
	*		The constructor will create a new disk
	*
	**************************************************/
	Disk(string, string, ConstructorCod);
	/*************************************************
	* Destructor
	*    Default destructor
	*
	**************************************************/
	~Disk();

	/*************************************************
	*
	*				  Others functions
	*
	**************************************************/
	bool getMounted() { return mounted; }
	
	DATtype  getDatDAt() { return dat.Getdat(); }

	RootDir getRootDir() { return this->rootdir; }
	
	string& GetLastErrorMessage() { return this->lastErrorMessage; } 

	string& GetLastErrorSource() { return this->lastErrorSource; }
	
	void SetLastErrorMessage(string lastErrorMessage) { this->lastErrorMessage = lastErrorMessage; }
	
	void SetLastErrorSource(string lastErrorSource) { this->lastErrorSource = lastErrorSource; }

	/*************************************************
	* 
	*				  Level 0
	*
	**************************************************/

	/*************************************************
	* FUNCTION
	*    createdisk
	*
	* RETURN VALUE
	*	This function does not return parameters
	*
	* PARAMETERS
	*    diskName – The name of the disk that will be created
	*	 owner    - The disk owner name
	*
	* MEANING
	*     This functions will create a new disk in a few steps:
	*			1. Create the file and insert all sectors
	*			2. Create the volume header and insert it
	*			3. Create the DAT and insert it
	*
	*	 Important: The function make a new disk just in case:
	*		   1. There is not another disk mounted
	*		   2. The file doesn't exist
	*
	* SEE ALSO
	*	  mountdisk function
	*
	**************************************************/
	void createdisk(string , string );
	/*************************************************
	* FUNCTION
	*    mountdisk
	*
	* PARAMETERS
	*   This function does not receive parameters
	*
	* RETURN VALUE
	*	This function does not return parameters
	*
	* PARAMETERS
	*    FileName – The file name of the disk
	*
	* MEANING
	*     This functions mount a disk and load the
	*	  volume header and the DAT
	*
	*	 Important: The function mount the disk just in case:
	*		   1. There is not another disk mounted
	*		   2. The disk exist
	*
	* SEE ALSO
	*	  unmountdisk function
	*
	**************************************************/
	void mountdisk(string );
	/*************************************************
	* FUNCTION
	*    unmountdisk
	*
	* PARAMETERS
	*   This function does not receive parameters
	*
	* RETURN VALUE
	*	This function does not return parameters
	*
	* MEANING
	*     This functions unmount this disk and unload
	*	  the volume header and the DAT
	*
	*	 Important: The function unmount the disk just in case:
	*		   1. The disk is mounted
	*		   2. Disk is opened
	*
	* SEE ALSO
	*	  unmountdisk function
	*
	**************************************************/
	void unmountdisk();

	void recreatedisk(string );
	/*************************************************
	* FUNCTION
	*    getdskfl
	* PARAMETERS
	*   The function does not get a parameter
	* RETURN VALUE
	*	Returns the fstream object that opened the file
	*
	* MEANING
	*     The function will verify if there a file opened
	*	  if yes the function return the fstream object
	*	  if not the function return a nullptr
	*
	***************************************************/
	fstream * getdskfl();

	/*************************************************
	* FUNCTION
	*   seekToSector
	* PARAMETERS
	*   The sector index
	* RETURN VALUE
	*	This function does not return parameters
	*
	* MEANING
	*	The function will seek the fstream object (dskfl)
	*	to a sector
	*
	***************************************************/
	void seekToSector(uint);
	/*************************************************
	* FUNCTION
	*   writeSector
	* PARAMETERS
	*   The sector index - Type: unsigned int
	*	A sector		 - Type: Sector
	* RETURN VALUE
	*	This function does not return parameters
	*
	* MEANING
	*	The function will seek to the sector index and
	*	then write it. 
	*
	***************************************************/
	void writeSector(uint, Sector*);
	/*************************************************
	* FUNCTION
	*   writeSector
	* PARAMETERS
	*
	*	A sector		 - Type: Sector
	* RETURN VALUE
	*	This function does not return parameters
	*
	* MEANING
	*	The function write a sector into the file.
	*
	***************************************************/
	void writeSector(Sector *);
	/*************************************************
	* FUNCTION
	*   readSector
	* PARAMETERS
	*   The sector index - Type: unsigned int
	*	A sector		 - Type: Sector
	* RETURN VALUE
	*	This function does not return parameters
	*
	* MEANING
	*	The function will seek to the sector index and
	*	then read the sector.
	*
	***************************************************/
	void readSector(uint, Sector*);
	/*************************************************
	* FUNCTION
	*   writeSector
	* PARAMETERS
	*	A sector		 - Type: Sector
	* RETURN VALUE
	*	This function does not return parameters
	*
	* MEANING
	*	The function read a sector from the file.
	*
	***************************************************/
	void readSector(Sector *);

	/*************************************************
	* FUNCTION
	*   writeSector
	* PARAMETERS
	*   The filename - Type: unsigned int
	*
	* RETURN VALUE
	*	This function does not return parameters
	*
	* MEANING
	*	The function will check if the filename contain 
	*	the program file extension. in case don't the
	*	function will added it.
	*
	***************************************************/
	void VerifyAndAddExt(string &);

	/*************************************************
	*
	*				  Level 1
	*
	**************************************************/

	/*************************************************
	* FUNCTION
	*	format
	* PARAMETERS
	*	The file name
	* RETURN VALUE
	*	This function does not return any parameters
	*
	* MEANING
	*   The function will write the Rootdir and set
	*	into volume header as  formated
	*
	***************************************************/
	void format(string &);

	/*************************************************
	* FUNCTION
	*	howmuchempty
	* PARAMETERS
	*	The index to start, the default is 0
	* RETURN VALUE
	*	how many sectors is free
	*
	* MEANING
	*	the function will check the DAT and return how many 
	*	sectors (clusters) is free to use;
	*
	*
	***************************************************/
	uint howmuchempty(uint = 0);

	/*************************************************
	* FUNCTION
	*
	*	alloc
	*
	* PARAMETERS
	*
	*	The FAT            - Type: DATtype
	*	The amount         - Type: unsigned int
	*	The algorithm type - Type: Disk::AlgorithmType
	*
	* RETURN VALUE
	*
	*	The function does not return a value
	*
	* MEANING
	*	
	*	The function will allocate the numbers of 
	*	sectors into the DAT and FAT, received from the 
	*	user, with the algorithm request by the user. 
	***************************************************/
	void alloc(DATtype &, uint, AlgorithmType);

	/*************************************************
	* FUNCTION
	*
	*	allocextend
	*
	* PARAMETERS
	*
	*	The FAT            - Type: DATtype
	*	The amount         - Type: unsigned int
	*	The algorithm type - Type: Disk::AlgorithmType
	*
	* RETURN VALUE
	*
	*	The function does not return a value
	*
	* MEANING
	*
	*	As alloc the function will allocate the numbers of
	*	sectors into the DAT and into the FAT that already 
	*	contain data.
	*
	***************************************************/
	void allocextend(DATtype &, uint, AlgorithmType);

	/*************************************************
	* FUNCTION
	*	dealloc
	* PARAMETERS
	*	
	*	The FAT
	*
	* RETURN VALUE
	*
	*	The function does not return a value
	*
	* MEANING
	*
	*	The function will take the FAT and deallocate
	*	the sector used by the him, into the DAT.
	*
	***************************************************/
	void dealloc(DATtype &);

	/*************************************************
	* FUNCTION
	*	flush
	* PARAMETERS
	*	
	*	The function does not receive a value
	*
	* RETURN VALUE
	*
	*	The function does not return a value
	*
	* MEANING
	*
	*	The function will release the data to the disk file 
	*	(DAT, Volumen Header ,Rootdir)
	*
	***************************************************/
	void flush();


	/*************************************************
	*
	*				  Level 2
	*
	**************************************************/

	/*************************************************
	* FUNCTION
	*
	*	createfile
	*
	* PARAMETERS
	*
	*	The name				- Type: DATtype
	*	The owner				- Type: unsigned int
	*	If is dynamic			- Type: bool
	*	The regSize				- Type: unsigned int
	*	The Amount of sectors	- Type: unsigned int
	*	The key type			- Type: string
	*	The key offset			- Type: unsigned int
	*	The key size			- Type: unsigned int
	*
	* RETURN VALUE
	*
	*	The function does not return a value
	*
	* MEANING
	*
	*	The function will create a file header and then 
	*	allocate the file into the disk.
	*
	***************************************************/
	void Disk::createfile(string & fileName, string & ownerFile, bool dynamic, uint regSize, uint sectorSize, string  keyType, uint offset, uint keySize);

	/*************************************************
	* FUNCTION
	*
	*	extendfile
	*
	* PARAMETERS
	*
	*	The name				- Type: DATtype
	*	The owner				- Type: unsigned int
	*	The Amount of sectors	- Type: unsigned int
	*
	* RETURN VALUE
	*
	*	The function does not return a value
	*
	* MEANING
	*
	*	The function will extend a file header through
	*	allocext.
	*
	***************************************************/
	void extendfile(string &, string &, uint, FCB * file = NULL);

	/*************************************************
	* FUNCTION
	*
	*	delfile
	*
	* PARAMETERS
	*
	*	The name				- Type: DATtype
	*	The owner				- Type: unsigned int
	*
	* RETURN VALUE
	*
	*	The function does not return a value
	*
	* MEANING
	*
	*	The function will delete a file.
	*
	***************************************************/
	void delfile(string &, string &);

	/*************************************************
	* FUNCTION
	*
	*	delfile
	*
	* PARAMETERS
	*
	*	The name				- Type: DATtype
	*
	* RETURN VALUE
	*
	*	The DirEntry of the file received
	*	In case the file doesn't exist the function 
	*	return NULL
	*
	* MEANING
	*
	*	The function returns the DirEntry of a file
	*
	***************************************************/
	DirEntry * getDir( const char * FileName);

	/*************************************************
	* FUNCTION
	*
	*	getFileHeader
	*
	* PARAMETERS
	*
	*	A DirEntry
	*
	* RETURN VALUE
	*
	*	Return the file header
	*
	* MEANING
	*
	*	The function will seek to the first sector of the file
	*	and get the file header, then the function will return
	*	it.
	*
	***************************************************/


	FileHeader getFileHeader(DirEntry *);

	/*************************************************
	*
	*				  Level 3
	*
	**************************************************/

	FCB * openfile(string , string , enumsFMS::FCBtypeToOpening type);

	uint updateFile(DirEntry);

	void defragmentation();

	/*************************************************
	*
	*				  Functions to Dll
	*
	**************************************************/

	VolumeHeader getVolumeHeader() { return vhd; }

	DirEntry * getDirEntry(int index,SectorDir * sector = NULL);

	bool dirExist(int index,SectorDir * = NULL);

	bool IsFormated();

	double * getStatus() { return &this->status; }

	/*************************************************
	*
	*				 Friends
	*
	**************************************************/
	friend class TestLevel_0;
	friend class TestLevel_2;
	friend class FCB;


	

};

