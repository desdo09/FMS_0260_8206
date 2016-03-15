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
	enum diskCod
	{
		create = 'c',
		mount = 'm',
		
	};

	Disk();
	Disk(string &, string &, diskCod);
	~Disk();

	//step 0 Functions

	void createdisk(string &, string &);
	void mountdisk(string &);
	void unmountdisk();
	void recreatedisk(string &);

	fstream * getdskfl();

	void seekToSector(uint);
	
	void writeSector(uint, Sector*);
	void writeSector(Sector *);

	void readSector(int, Sector*);
	void readSector(Sector *);

	void importBufferToObject(void *, int size);

	friend class TestLevel_0;

};

