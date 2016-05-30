using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace FMS_adapter
{
    public class Disk
    {
        string diskName;
        string owner;
        //the disk pointer
        IntPtr myDiskPointer;



        public Disk()
        {
            try
            {

                this.myDiskPointer = cppToCsharpAdapter.makeDiskObject();
            }
            catch (DllNotFoundException)
            {
                throw new Exception("Dll error");
            }
        }

        ~Disk()
        {

            if (myDiskPointer != null)
            {
                cppToCsharpAdapter.deleteDiskObject(ref myDiskPointer);
            }
        }

        public string diskPointer
        {
            get
            {
                return Marshal.PtrToStringAuto(myDiskPointer);
            }
        }

        public VolumeHeader GetVolumeHeader()
        {
            try
            {

                VolumeHeader v = new VolumeHeader();
                int structSize = Marshal.SizeOf(v.GetType()); //Marshal.SizeOf(typeof(Student)); 
                IntPtr buffer = Marshal.AllocHGlobal(structSize);
                Marshal.StructureToPtr(v, buffer, true);

                // ... send buffer to dll
                cppToCsharpAdapter.getVolumeHeader(this.myDiskPointer, buffer);
                Marshal.PtrToStructure(buffer, v);

                // free allocate
                Marshal.FreeHGlobal(buffer);

                return v;
            }
            catch (SEHException)
            {
                IntPtr cString = cppToCsharpAdapter.getLastDiskErrorMessage(this.myDiskPointer);
                string message = Marshal.PtrToStringAnsi(cString);
                cString = cppToCsharpAdapter.getLastDiskErrorSource(this.myDiskPointer);
                string source = Marshal.PtrToStringAnsi(cString);
                throw new ProgramException(message, source);

            }

        }
        public List<DirEntry> getDirEntryInRootDir()
        {
            try
            {
                List<DirEntry> list = new List<DirEntry>();
                DirEntry dirTemp;
                int structSize = Marshal.SizeOf(typeof(DirEntry)); //Marshal.SizeOf(typeof(Student)); 
                IntPtr buffer = Marshal.AllocHGlobal(structSize);


                // ... send buffer to dll
                for (int i = 0; i < 28; i++)
                {
                    if (cppToCsharpAdapter.dirExist(this.myDiskPointer, i) == 1)
                    {
                        dirTemp = new DirEntry();
                        Marshal.StructureToPtr(dirTemp, buffer, true);
                        cppToCsharpAdapter.getDirEntry(this.myDiskPointer, buffer, i);
                        Marshal.PtrToStructure(buffer, dirTemp);
                        list.Add(dirTemp);
                    }
                }


                // free allocate
                Marshal.FreeHGlobal(buffer);

                return list;

            }
            catch (SEHException)
            {
                IntPtr cString = cppToCsharpAdapter.getLastDiskErrorMessage(this.myDiskPointer);
                string message = Marshal.PtrToStringAnsi(cString);
                cString = cppToCsharpAdapter.getLastDiskErrorSource(this.myDiskPointer);
                string source = Marshal.PtrToStringAnsi(cString);
                throw new ProgramException(message, source);

            }

        }

        public bool isMounted()
        {
            return cppToCsharpAdapter.isMounted(this.myDiskPointer) != 0;
        }

        public void Createdisk(string diskName, string diskOwner)
        {
            try
            {
                cppToCsharpAdapter.createdisk(this.myDiskPointer, diskName, diskOwner);
            }
            catch (SEHException)
            {
                IntPtr cString = cppToCsharpAdapter.getLastDiskErrorMessage(this.myDiskPointer);
                string message = Marshal.PtrToStringAnsi(cString);
                cString = cppToCsharpAdapter.getLastDiskErrorSource(this.myDiskPointer);
                string source = Marshal.PtrToStringAnsi(cString);
                throw new ProgramException(message, source);
            }

        }

        public void Mountdisk(string diskName)
        {
            try
            {
                cppToCsharpAdapter.mountdisk(this.myDiskPointer, diskName);
                this.diskName = diskName;
                this.owner = this.GetVolumeHeader().DiskOwner;
            }
            catch (SEHException)
            {
                IntPtr cString = cppToCsharpAdapter.getLastDiskErrorMessage(this.myDiskPointer);
                string message = Marshal.PtrToStringAnsi(cString);
                cString = cppToCsharpAdapter.getLastDiskErrorSource(this.myDiskPointer);
                string source = Marshal.PtrToStringAnsi(cString);
                throw new ProgramException(message, source);
            }

        }

        public void Unmountdisk()
        {
            try
            {
                cppToCsharpAdapter.unmountdisk(this.myDiskPointer);
                this.diskName = null;
                this.owner = null;
            }
            catch (SEHException)
            {
                IntPtr cString = cppToCsharpAdapter.getLastDiskErrorMessage(this.myDiskPointer);
                string message = Marshal.PtrToStringAnsi(cString);
                cString = cppToCsharpAdapter.getLastDiskErrorSource(this.myDiskPointer);
                string source = Marshal.PtrToStringAnsi(cString);
                throw new ProgramException(message, source);
            }

        }


        public void Recreatedisk(string diskOwner)
        {
            try
            {
                cppToCsharpAdapter.recreatedisk(this.myDiskPointer, diskOwner);
            }
            catch (SEHException)
            {
                IntPtr cString = cppToCsharpAdapter.getLastDiskErrorMessage(this.myDiskPointer);
                string message = Marshal.PtrToStringAnsi(cString);
                cString = cppToCsharpAdapter.getLastDiskErrorSource(this.myDiskPointer);
                string source = Marshal.PtrToStringAnsi(cString);
                throw new ProgramException(message, source);
            }

        }



        // Level 1

        public void Format(string diskOwner = null)
        {
            try
            {
                if (diskOwner == null)
                    diskOwner = this.owner;
                cppToCsharpAdapter.format(this.myDiskPointer, diskOwner);
            }
            catch (SEHException)
            {
                IntPtr cString = cppToCsharpAdapter.getLastDiskErrorMessage(this.myDiskPointer);
                string message = Marshal.PtrToStringAnsi(cString);
                cString = cppToCsharpAdapter.getLastDiskErrorSource(this.myDiskPointer);
                string source = Marshal.PtrToStringAnsi(cString);
                throw new ProgramException(message, source);
            }

        }


        public int Howmuchempty()
        {
            try
            {
                return cppToCsharpAdapter.howmuchempty(this.myDiskPointer);
            }
            catch (SEHException)
            {
                IntPtr cString = cppToCsharpAdapter.getLastDiskErrorMessage(this.myDiskPointer);
                string message = Marshal.PtrToStringAnsi(cString);
                cString = cppToCsharpAdapter.getLastDiskErrorSource(this.myDiskPointer);
                string source = Marshal.PtrToStringAnsi(cString);
                throw new ProgramException(message, source);
            }

        }


        //Level 2
        /// <summary>
        /// The function create a new student file 
        /// </summary>
        /// <param name="fileSize">The file size (sectors) </param>
        /// <param name="fileName">The file name</param>
        /// <param name="recSize">The size of each record</param>
        /// <param name="fileOwner">The file owner</param>
        public void createStudentfile(int fileSize, string fileName, int recSize = 0, string fileOwner = null)
        {
            try
            {
                if (fileSize < 1)
                    throw new Exception("File size most be bigger than or equal than 1 byte");
                if (fileOwner == null)
                    fileOwner = this.owner;

                cppToCsharpAdapter.createStudentfile(this.myDiskPointer, fileName, fileOwner, fileSize, recSize);
            }
            catch (SEHException)
            {
                IntPtr cString = cppToCsharpAdapter.getLastDiskErrorMessage(this.myDiskPointer);
                string message = Marshal.PtrToStringAnsi(cString);
                cString = cppToCsharpAdapter.getLastDiskErrorSource(this.myDiskPointer);
                string source = Marshal.PtrToStringAnsi(cString);
                throw new ProgramException(message, source);
            }

        }

        public void importfile(string directory, string fileOwner = null)
        {
            try
            {
                if (fileOwner == null)
                    fileOwner = this.owner;


                string path = directory.Substring(0, directory.LastIndexOf("\\"));
                string name = directory.Substring(directory.LastIndexOf("\\") + 1);


                cppToCsharpAdapter.importFile(this.myDiskPointer, path, name, this.owner);
            }
            catch (SEHException)
            {
                IntPtr cString = cppToCsharpAdapter.getLastDiskErrorMessage(this.myDiskPointer);
                string message = Marshal.PtrToStringAnsi(cString);
                cString = cppToCsharpAdapter.getLastDiskErrorSource(this.myDiskPointer);
                string source = Marshal.PtrToStringAnsi(cString);
                throw new ProgramException(message, source);
            }
        }

        public void Delfile(string fileName, string fileOwner = null)
        {
            try
            {
                if (fileOwner == null)
                    fileOwner = this.owner;

                cppToCsharpAdapter.delfile(this.myDiskPointer, fileName, fileOwner);
            }
            catch (SEHException)
            {
                IntPtr cString = cppToCsharpAdapter.getLastDiskErrorMessage(this.myDiskPointer);
                string message = Marshal.PtrToStringAnsi(cString);
                cString = cppToCsharpAdapter.getLastDiskErrorSource(this.myDiskPointer);
                string source = Marshal.PtrToStringAnsi(cString);
                throw new ProgramException(message, source);
            }

        }


        public void Extendfile(string fileName, string fileOwner, uint size)
        {
            try
            {
                cppToCsharpAdapter.extendfile(this.myDiskPointer, fileName, fileOwner, size);
            }
            catch (SEHException)
            {
                IntPtr cString = cppToCsharpAdapter.getLastDiskErrorMessage(this.myDiskPointer);
                string message = Marshal.PtrToStringAnsi(cString);
                cString = cppToCsharpAdapter.getLastDiskErrorSource(this.myDiskPointer);
                string source = Marshal.PtrToStringAnsi(cString);
                throw new ProgramException(message, source);
            }

        }

        public void flush()
        {
            try
            {
                cppToCsharpAdapter.flush(this.myDiskPointer);
            }
            catch (SEHException)
            {
                IntPtr cString = cppToCsharpAdapter.getLastDiskErrorMessage(this.myDiskPointer);
                string message = Marshal.PtrToStringAnsi(cString);
                cString = cppToCsharpAdapter.getLastDiskErrorSource(this.myDiskPointer);
                string source = Marshal.PtrToStringAnsi(cString);
                throw new ProgramException(message, source);
            }
        }

        // Level 3

        public FCB Openfile(string fileName, FCBtypeToOpening openMode = FCBtypeToOpening.inputOutput, string fileOwner = null)
        {
            try
            {
                if (fileOwner == null)
                    fileOwner = this.owner;

                IntPtr p = cppToCsharpAdapter.openfile(this.myDiskPointer, fileName, fileOwner, (int)openMode);
                return new FCB(p);
            }
            catch (SEHException)
            {
                IntPtr cString = cppToCsharpAdapter.getLastDiskErrorMessage(this.myDiskPointer);
                string message = Marshal.PtrToStringAnsi(cString);
                cString = cppToCsharpAdapter.getLastDiskErrorSource(this.myDiskPointer);
                string source = Marshal.PtrToStringAnsi(cString);
                throw new ProgramException(message, source);
            }

        }

        public void exportFile(string exportDir, string fileName, string fileOwner = null, FCB fcb = null)
        {
            try
            {
                if (fileOwner == null)
                    fileOwner = this.owner;

                cppToCsharpAdapter.exportFile(this.myDiskPointer, exportDir, fileName, fileOwner, fcb);
            }
            catch (SEHException)
            {
                IntPtr cString = cppToCsharpAdapter.getLastDiskErrorMessage(this.myDiskPointer);
                string message = Marshal.PtrToStringAnsi(cString);
                cString = cppToCsharpAdapter.getLastDiskErrorSource(this.myDiskPointer);
                string source = Marshal.PtrToStringAnsi(cString);
                throw new ProgramException(message, source);
            }
        }

        //extra
        public string getDAT()
        {
            try
            {
                IntPtr DAT = cppToCsharpAdapter.getDat(this.myDiskPointer);
                return Marshal.PtrToStringAnsi(DAT).ToString();

            }
            catch (SEHException)
            {
                IntPtr cString = cppToCsharpAdapter.getLastDiskErrorMessage(this.myDiskPointer);
                string message = Marshal.PtrToStringAnsi(cString);
                cString = cppToCsharpAdapter.getLastDiskErrorSource(this.myDiskPointer);
                string source = Marshal.PtrToStringAnsi(cString);
                throw new ProgramException(message, source);
            }



        }
        public double getStatus()
        {

            try
            {
                return cppToCsharpAdapter.getStatus(this.myDiskPointer);

            }
            catch (SEHException)
            {
                IntPtr cString = cppToCsharpAdapter.getLastDiskErrorMessage(this.myDiskPointer);
                string message = Marshal.PtrToStringAnsi(cString);
                cString = cppToCsharpAdapter.getLastDiskErrorSource(this.myDiskPointer);
                string source = Marshal.PtrToStringAnsi(cString);
                throw new ProgramException(message, source);
            }


        }

        public void defragment()
        {

            try
            {

                cppToCsharpAdapter.defragmentDisk(this.myDiskPointer);
            }
            catch (SEHException)
            {
                IntPtr cString = cppToCsharpAdapter.getLastDiskErrorMessage(this.myDiskPointer);
                string message = Marshal.PtrToStringAnsi(cString);
                cString = cppToCsharpAdapter.getLastDiskErrorSource(this.myDiskPointer);
                string source = Marshal.PtrToStringAnsi(cString);
                throw new ProgramException(message, source);
            }

        }
    }
}
