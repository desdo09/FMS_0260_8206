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
	
public:
	enum ConstructorCod
	{
		Create = 'c',
		Mount = 'm',
		
	};
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

	friend class TestLevel_0;

	void VerifyAndAddExt(string &);

};

