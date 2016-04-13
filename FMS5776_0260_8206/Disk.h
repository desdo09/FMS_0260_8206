#pragma once
#include"Header.h"
#include "VolumeHeader.h"
#include "Dir.h"
#include "DAT.h"
#include "Sector.h"
#include "FileHeader.h"

class Disk
{
private:

	VolumeHeader  vhd;
	DAT           dat;
	RootDir       rootdir;
	bool          mounted;
	fstream       dskfl;
	uint          currDiskSectorNr;
	char          buffer[sizeof(Sector)];
	
	uint firstFit(uint, uint);
	uint bestFit(uint, uint);
	uint worstFit(uint, uint);
	int firstIndex(DATtype DAT, bool isDAT = true, uint indexStart = 0);
	int lastIndex(DATtype DAT, bool isDAT = true);

public:
	enum ConstructorCod
	{
		Create = 'c',
		Mount = 'm',
		
	};
	enum AlgorithmType { first_Fit=1, best_Fit, worst_Fit};

	bool getMounted() { return mounted; }
	DATtype  getDatDAt() { return dat.Getdat(); }

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
	*	Returns the fstream object that opened the file
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

	void seekToSector(uint);
	
	void writeSector(uint, Sector*);

	void writeSector(Sector *);

	void readSector(uint, Sector*);

	void readSector(Sector *);

	void VerifyAndAddExt(string &);

	/*************************************************
	*
	*				  Level 1
	*
	**************************************************/

	/*************************************************
	* FUNCTION
	*   
	* PARAMETERS
	*  
	* RETURN VALUE
	*	
	*
	* MEANING
	*    
	*
	***************************************************/
	void format(string &);

	/*************************************************
	* FUNCTION
	*
	* PARAMETERS
	*
	* RETURN VALUE
	*
	*
	* MEANING
	*
	*
	***************************************************/
	uint howmuchempty(uint);

	/*************************************************
	* FUNCTION
	*
	*	Allocate sectors into the DAT
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
	*	The function will allocate the numbers of sectors into the DAT 
	*	with the algorithm request by the user. 
	***************************************************/
	void alloc(DATtype &, uint, AlgorithmType);




	/*************************************************
	* FUNCTION
	*
	* PARAMETERS
	*
	* RETURN VALUE
	*
	*
	* MEANING
	*
	*
	***************************************************/
	void allocextend(DATtype &, uint, AlgorithmType);

	/*************************************************
	* FUNCTION
	*
	* PARAMETERS
	*
	* RETURN VALUE
	*
	*
	* MEANING
	*
	*
	***************************************************/
	void dealloc(DATtype &);

	void flush();


	/*************************************************
	*
	*				  Level 2
	*
	**************************************************/

	void createfile(string & , string & , bool , uint , uint , string, uint , uint );

	void extendfile(string &, string &, uint);
	
	void delfile(string &, string &);

	DirEntry * getDir( const char * FileName);



	/*************************************************
	*
	*				 Friends
	*
	**************************************************/
	friend class TestLevel_0;
	friend class TestLevel_2;


	

};

