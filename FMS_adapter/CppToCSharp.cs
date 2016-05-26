using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace FMS_adapter
{

    class cppToCsharpAdapter
    {
        const string dllPath = "FMS_DLL.dll";

        [DllImport(dllPath, CallingConvention = CallingConvention.Cdecl)]
        public static extern int sum(int a, int b);


        //  init disk
        [DllImport(dllPath)]
        public static extern IntPtr makeDiskObject();

        [DllImport(dllPath, CallingConvention = CallingConvention.Cdecl)]
        public static extern void deleteDiskObject(ref IntPtr THIS);

        [DllImport(dllPath, CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr getLastDiskErrorMessage(IntPtr THIS);

        [DllImport(dllPath, CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr getLastFcbErrorMessage(IntPtr THIS);

        /******/
        [DllImport(dllPath, CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr getLastDiskErrorSource(IntPtr THIS);

        [DllImport(dllPath, CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr getLastFcbErrorSource(IntPtr THIS);


        /****/

        [DllImport(dllPath, CallingConvention = CallingConvention.Cdecl)]
        public static extern void deleteFcbObject(ref IntPtr THIS);

        // Level 0

        [DllImport(dllPath, CallingConvention = CallingConvention.Cdecl)]
        public static extern void createdisk(IntPtr THIS, string diskName, string diskOwner);

        [DllImport(dllPath, CallingConvention = CallingConvention.Cdecl)]
        public static extern void mountdisk(IntPtr THIS, string diskName);

        [DllImport(dllPath, CallingConvention = CallingConvention.Cdecl)]
        public static extern void unmountdisk(IntPtr THIS);

        [DllImport(dllPath, CallingConvention = CallingConvention.Cdecl)]
        public static extern void recreatedisk(IntPtr THIS, string diskOwner);



        // Level 1
        [DllImport(dllPath, CallingConvention = CallingConvention.Cdecl)]
        public static extern void format(IntPtr THIS, string diskOwner);

        [DllImport(dllPath, CallingConvention = CallingConvention.Cdecl)]
        public static extern int howmuchempty(IntPtr THIS);


        //Level 2
        [DllImport(dllPath, CallingConvention = CallingConvention.Cdecl)]
        public static extern void createStudentfile(IntPtr THIS, string fileName, string fileOwner, int fileSize, int recSize = 0);

        [DllImport(dllPath, CallingConvention = CallingConvention.Cdecl)]
        public static extern void importFile(IntPtr THIS, string path, string fileName, string fileOwner);

        [DllImport(dllPath, CallingConvention = CallingConvention.Cdecl)]
        public static extern void delfile(IntPtr THIS, string fileName, string fileOwner);

        [DllImport(dllPath, CallingConvention = CallingConvention.Cdecl)]
        public static extern void extendfile(IntPtr THIS, string fileName, string fileOwner, uint size);


        // Level 3
        [DllImport(dllPath, CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr openfile(IntPtr THIS, string fileName, string fileOwner, int openMode);


        // FCB
        [DllImport(dllPath, CallingConvention = CallingConvention.Cdecl)]
        public static extern void closefile(IntPtr THIS);

        [DllImport(dllPath, CallingConvention = CallingConvention.Cdecl)]
        public static extern void readRec(IntPtr THIS, IntPtr dest, uint readForUpdate = 0);

        [DllImport(dllPath, CallingConvention = CallingConvention.Cdecl)]
        public static extern void writeRec(IntPtr THIS, IntPtr source);


        [DllImport(dllPath, CallingConvention = CallingConvention.Cdecl)]
        public static extern void seekRec(IntPtr THIS, uint from, int pos);

        [DllImport(dllPath, CallingConvention = CallingConvention.Cdecl)]
        public static extern void seekToRecId(IntPtr THIS, ulong from);

        [DllImport(dllPath, CallingConvention = CallingConvention.Cdecl)]
        public static extern void updateRecCancel(IntPtr THIS);

        [DllImport(dllPath, CallingConvention = CallingConvention.Cdecl)]
        public static extern void deleteRec(IntPtr THIS);

        [DllImport(dllPath, CallingConvention = CallingConvention.Cdecl)]
        public static extern void updateRec(IntPtr THIS, IntPtr source);

        [DllImport(dllPath, CallingConvention = CallingConvention.Cdecl)]
        public static extern void getfileDesc(IntPtr THIS, IntPtr Dir);

        // extra
        [DllImport(dllPath, CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr getDat(IntPtr THIS);

        [DllImport(dllPath, CallingConvention = CallingConvention.Cdecl)]
        public static extern void getVolumeHeader(IntPtr THIS, IntPtr buffer);
        //THIS->getfileDesc()
        [DllImport(dllPath, CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.AnsiBStr)]
        public static extern string getDirEntry(IntPtr THIS);

        [DllImport(dllPath, CallingConvention = CallingConvention.Cdecl)]
        public static extern void getDirEntry(IntPtr THIS, IntPtr Dir, int index);

        [DllImport(dllPath, CallingConvention = CallingConvention.Cdecl)]
        public static extern int dirExist(IntPtr THIS, int index);

        [DllImport(dllPath, CallingConvention = CallingConvention.Cdecl)]
        public static extern int isMounted(IntPtr THIS);
        [DllImport(dllPath, CallingConvention = CallingConvention.Cdecl)]
        public static extern void exportFile(IntPtr THIS, string exportDir, string fileName, string fileOwner, FCB fcb = null);
        [DllImport(dllPath, CallingConvention = CallingConvention.Cdecl)]
        public static extern double getStatus(IntPtr THIS);



    }
}
