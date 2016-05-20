using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Drawing;
using System.Windows.Media;
using System.Windows.Media.Imaging;

namespace FMS_adapter
{

    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]
    public class DirEntry
    {
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 12)]
        string _filename;
        public string filename { get { return _filename; } }

        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 12)]
        string _fileOwner;
        public string fileOwner { get { return _fileOwner; } }

        int _fileAddr;
        public int fileAddr { get { return _fileAddr; } }

        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 10)]
        string _crDate;
        public string crDate { get { return _crDate.Substring(3, 2) + "/" + _crDate.Substring(0, 2) + "/" + _crDate.Substring(6); } }

        int _fileSize;
        public string fileSize { get { return _fileSize + " Kb"; } }

        int _eofRecNr;
        public int eofRecNr { get { return _eofRecNr; } }

        int _maxRecSize;
        //   public int maxRecSize { get { return _maxRecSize; } }

        int _actualRecSize;
        //    public int actualRecSize { get { return _actualRecSize; } }

        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 2)]
        string _recFormat;
        //   public string recFormat { get { return _recFormat; } }

        int _keyOffset;
        //   public int keyOffset { get { return _keyOffset; } }

        int _keySize;
        //   public int keySize { get { return _keySize; } }

        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 2)]
        string _keyType;
        //   public string keyType { get { return _keyType; } }

        int _entryStatus;
        public int entryStatus { get { return _entryStatus; } }

        public BitmapSource fileImage
        {

            get
            {

                try
                {
                    if (filename.Substring(filename.LastIndexOf('.')) == ".stud")
                    {
                        BitmapSource temp = new BitmapImage(new Uri("..\\Images\\stud.png", UriKind.Relative));
                       
                        return temp;
                    }

                    string fileAndParam = RegisteredFileType.allFileTypeAndIcon[filename.Substring(filename.LastIndexOf('.'))].ToString();

                    if (String.IsNullOrEmpty(fileAndParam))
                        return null;

                    Icon icon = null;

                    bool isLarge = false;



                    icon = RegisteredFileType.ExtractIconFromFile(fileAndParam, isLarge); //RegisteredFileType.ExtractIconFromFile(fileAndParam);

                    //The icon cannot be zero.
                    if (icon != null)
                    {
                        //Draw the icon to the picture box.
                        return icon.ToBitmap().ToBitmapSource();
                    }
                    return null;
                }
                catch (Exception ex)
                {
                    throw ex;
                }



            }

        }
    }
}
