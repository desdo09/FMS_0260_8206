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
                cppToCsharpAdapter.deleteDiskObject(ref myDiskPointer);
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
            catch
            {
                throw;
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
            catch
            {
                throw;
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
            catch
            {
                throw;
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
            catch
            {
                throw;
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
            catch
            {
                throw;
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
            catch
            {
                throw;
            }
        }


        //Level 2

        public void Createfile(string fileName, string fileOwner, string FinalOrVar,
                                uint recSize, uint fileSize,
                                string keyType, uint keyOffset, uint keySize = 4)
        {
            try
            {
                cppToCsharpAdapter.createfile(this.myDiskPointer, fileName, fileOwner, FinalOrVar,
                              recSize, fileSize,
                                keyType, keyOffset, keySize);
            }
            catch (SEHException)
            {
                IntPtr cString = cppToCsharpAdapter.getLastDiskErrorMessage(this.myDiskPointer);
                string message = Marshal.PtrToStringAnsi(cString);
                cString = cppToCsharpAdapter.getLastDiskErrorSource(this.myDiskPointer);
                string source = Marshal.PtrToStringAnsi(cString);
                throw new ProgramException(message, source);
            }
            catch
            {
                throw;
            }
        }


        public void Delfile(string fileName, string fileOwner)
        {
            try
            {
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
            catch
            {
                throw;
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
            catch
            {
                throw;
            }
        }


        // Level 3

        public FCB Openfile(string fileName, string fileOwner, string openMode)
        {
            try
            {
                IntPtr p = cppToCsharpAdapter.openfile(this.myDiskPointer, fileName, fileOwner, openMode);
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
            catch
            {
                throw;
            }
        }
    }
}
