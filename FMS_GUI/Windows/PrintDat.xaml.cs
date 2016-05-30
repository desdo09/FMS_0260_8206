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
        string DAT;
        FCB fcb = null;


        public PrintDat(string DAT, VolumeHeader diskData)
        {
            InitializeComponent();
            if (DAT.Length < 1600)
            {
                MessageBox.Show("DAT type error", "PrintDat", MessageBoxButton.OK, MessageBoxImage.Error);
                this.Close();
            }
            this.DAT = DAT;

            diskGrid.Visibility = Visibility.Visible;
            diskNameLabel.Content = diskData.DiskName;
            diskOwnerLabel.Content = diskData.DiskOwner;
            diskcreatedLabel.Content = diskData.ProdDate;
            diskFormatedLabel.Content = (diskData.IsFormated) ? (diskData.FormatDate) : ("Not formated");

            drawMap();
        }

        public PrintDat(FCB fcb, string DAT)
        {
            InitializeComponent();

            if (DAT.Length < 1600)
            {
                MessageBox.Show("DAT type error", "PrintDat", MessageBoxButton.OK, MessageBoxImage.Error);
                this.Close();
            }

            if (fcb == null)
                return;

            DirEntry dir = fcb.getfileDesc();


            fileGrid.Visibility = Visibility.Visible;
            fileNameLabel.Content = dir.filename;
            fileOwnerLabel.Content = dir.fileOwner;
            filecreatedLabel.Content = dir.crDate;
            int Size = int.Parse(dir.fileSize.Substring(0, dir.fileSize.Length - 3));
            fileSizeLabel.Content = (Size % 2 == 0) ? dir.fileSize : (Size + 1) + "Kb";
            this.DAT = DAT;
            this.fcb = fcb;

            drawMap();

        }

        public void drawMap()
        {
            string FAT = null;
            if (fcb != null)
                FAT = this.fcb.getFAT();

            Rectangle rec;
            //i = Height
            for (int i = 0; i < 20; i++)
            {
                //j = width 
                for (int j = 0; j < 80; j++)
                {
                    rec = new Rectangle();
                    if (DAT[(i * 80) + j] == '0')
                        rec.Fill = Brushes.Gray;

                    if (!string.IsNullOrEmpty(FAT) && FAT[(i * 80) + j] == '1')
                        rec.Fill = Brushes.Blue;

                    rec.Height = 20;
                    Canvas.SetLeft(rec, j * 10);

                    rec.Width = 10;
                    rec.Stroke = Brushes.Black;
                    Canvas.SetTop(rec, i * 20);
                    DatMap.Children.Add(rec);


                }

            }

        }

        private void expandButton_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                int total = int.Parse(Prompt.show("How many sectors to expand ?", true));
                this.fcb.extendfile((uint)total);
                drawMap();
                DirEntry dir;
                dir = fcb.getfileDesc();
                int Size = int.Parse(dir.fileSize.Substring(0, dir.fileSize.Length - 3));
                fileSizeLabel.Content = (Size % 2 == 0) ? dir.fileSize : (Size + 1) + "Kb";
            }
            catch (ArgumentNullException)
            {
                MessageBox.Show("Canceled");

            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message, "Expand file", MessageBoxButton.OK, MessageBoxImage.Error);
            }

        }
    }
}
