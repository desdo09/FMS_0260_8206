using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;
using FMS_adapter;

namespace FMS_GUI.Windows
{
    /// <summary>
    /// Interaction logic for PrintDat.xaml
    /// </summary>
    public partial class PrintDat : Window
    {
        string map;
        DirEntry dir;
        public PrintDat(string DAT, VolumeHeader diskData)
        {
            InitializeComponent();
            if (DAT.Length < 1600)
            {
                MessageBox.Show("map type error", "PrintDat", MessageBoxButton.OK, MessageBoxImage.Error);
                this.Close();
            }
            this.map = DAT;
            this.dir = null;
            diskGrid.Visibility = Visibility.Visible;
            diskNameLabel.Content = diskData.DiskName;
            diskOwnerLabel.Content = diskData.DiskOwner;
            diskcreatedLabel.Content = diskData.ProdDate;
            diskFormatedLabel.Content = (diskData.IsFormated) ? (diskData.FormatDate) : ("Not formated");

            drawMap();
        }

        public PrintDat(string FAT, DirEntry dir)
        {
            InitializeComponent();
            if (map.Length < 1600)
            {
                MessageBox.Show("map type error", "PrintDat", MessageBoxButton.OK, MessageBoxImage.Error);
                this.Close();
            }
            if (dir == null)
            {
                MessageBox.Show("Dir cannot be null", "PrintDat", MessageBoxButton.OK, MessageBoxImage.Error);
                this.Close();
            }

            fileGrid.Visibility = Visibility.Visible;
            fileNameLabel.Content = dir.filename;
            fileOwnerLabel.Content = dir.fileOwner;
            filecreatedLabel.Content = dir.crDate;
            fileSizeLabel.Content = dir.fileSize;

            this.map = FAT;
            this.dir = dir;
            drawMap();

        }

        public void drawMap()
        {
            Rectangle rec;
            //i = Height
            for (int i = 0; i < 20; i++)
            {
                //j = width 
                for (int j = 0; j < 80; j++)
                {
                    rec = new Rectangle();
                    if ((map[(i * 80) + j] == '0' && dir == null) || (map[(i * 80) + j] == '1' && dir != null))
                        rec.Fill = Brushes.Gray;


                    rec.Height = 20;
                    Canvas.SetLeft(rec, j * 10);

                    rec.Width = 10;
                    rec.Stroke = Brushes.Black;
                    Canvas.SetTop(rec, i * 20);
                    DatMap.Children.Add(rec);

                    
                }
               
            }








        }
    }
}
