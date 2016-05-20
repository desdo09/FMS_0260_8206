using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace FMS_adapter
{
    public class FileToImageIconConverter
    {
        private string filePath;

        private System.Windows.Media.ImageSource icon;

        public string FilePath { get { return filePath; } }

        public System.Windows.Media.ImageSource Icon
        {
            get
            {
                if (icon == null && System.IO.File.Exists(FilePath))
                {
                    using (System.Drawing.Icon sysicon = System.Drawing.Icon.ExtractAssociatedIcon(FilePath))
                    {
                        icon = System.Windows.Interop.Imaging.CreateBitmapSourceFromHIcon(
                                  sysicon.Handle,
                                  System.Windows.Int32Rect.Empty,
                                  System.Windows.Media.Imaging.BitmapSizeOptions.FromEmptyOptions());
                    }
                }

                return icon;
            }
        }

        public FileToImageIconConverter(string filePath)
        {
            this.filePath = filePath;
        }
    }

}
