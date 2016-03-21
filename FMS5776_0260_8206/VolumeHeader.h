#include"Header.h"

// Class size
// 4+12+12+10+2+4*7+10+1+1+944=1024

class VolumeHeader
{
private:
	
	uint sectorNr; //index of sector
	char diskName[12] = ""; //name of the disk
	char diskOwner[12] = ""; //name of the disk owner
	char prodDate[10] = ""; //production date
	uint clusQty; // numbers of total clusters
	uint dataClusQty; //number of cluster with data
	uint addrDAT; // sector adress where the dat is
	uint addrRootDir;// sector adress of the first one
	uint addrDATcpy; //copy of the addroot
	uint addrRootDirCpy;//copy of the root directory adress
	uint addrDataStart;
	char formatDate[10] = "";
	bool isFormated;
	char emptyArea[944] ="";
	//addrDATcpy

public:

#pragma region Get/set
	//Get
	char * GetdiskName() { return diskName; }
	char * GetdiskOwner() { return diskOwner; }
	char * GetprodDate() { return prodDate; }
	char * GetformatDate() { return formatDate; }
	char * GetemptyArea() { return emptyArea; }
	uint GetsectorNr() { return sectorNr; }
	uint GetclusQty() { return clusQty; }
	uint GetdataClusQty() { return dataClusQty; }
	uint GetaddrDAT() { return addrDAT; }
	uint GetaddrRootDir() { return addrRootDir; }
	uint GetaddrDataStart() { return addrDataStart; }
	bool GetisFormated() { return isFormated; }

	//set
	void SetdiskName(string);
	void SetdiskOwner(string);
	void SetprodDate(string);
	void SetformaDate(string);
	void SetisFormated(bool);

	void Start();

#pragma endregion

	

	VolumeHeader();
	~VolumeHeader();

	//friends
	 
	friend class TestLevel_0;
	//friend class Disk;
	

};

