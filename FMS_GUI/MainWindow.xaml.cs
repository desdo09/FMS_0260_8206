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
using System.Windows.Navigation;
using System.Windows.Shapes;
using FMS_adapter;
using FMS_GUI.Windows;
using WinForms = System.Windows.Forms;

namespace FMS_GUI
{

    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {

        /*Objects*/
        private Disk currentDisk;
        private VolumeHeader vhd;
        private List<DirEntry> allFiles;
        /*End objects*/
        public MainWindow()
        {
            InitializeComponent();
            currentDisk = null;
            fileList.ItemsSource = allFiles;

        }

        #region Operations
        public void mountDisk(string path)
        {
            currentDisk.Mountdisk(path);

            vhd = currentDisk.GetVolumeHeader();

            if (!vhd.IsFormated)
            {
                if (MessageBox.Show("\nYou need to format the disk before you can used,\n\nformat now?", "New Disk", MessageBoxButton.YesNo, MessageBoxImage.Warning) == MessageBoxResult.Yes)
                {
                    formatDisk();
                    MessageBox.Show("Format complete", "Format Disk", MessageBoxButton.OK, MessageBoxImage.Information, MessageBoxResult.OK);
                }

            }

            if (vhd.IsFormated)
            {
                allFiles = currentDisk.getDirEntryInRootDir();

                fileList.ItemsSource = allFiles;
            }


            MessageBox.Show("Name:" + vhd.DiskName + "\nOwner:" + vhd.DiskOwner);




        }

        public void formatDisk()
        {
            if (currentDisk == null || !currentDisk.isMounted())
                throw new Exception("No disk mounted");

            this.currentDisk.Format();

            allFiles = currentDisk.getDirEntryInRootDir();

            fileList.ItemsSource = allFiles;
        }
        private void newDiskButton_Click(object sender, RoutedEventArgs e)
        {

            NewDisk newDisk = new NewDisk();
            newDisk.ShowDialog();

            if (newDisk.cancel)
                return;

            try
            {
                currentDisk = new Disk();
                currentDisk.Createdisk(newDisk.directory + newDisk.diskName, newDisk.diskOwner);


                if (MessageBox.Show("Disk created in: " + newDisk.directory +
                                    "\nName:" + newDisk.diskName +
                                    "\nOwner:" + newDisk.diskOwner +
                                    "\nDo you want to mount this disk?", "New Disk", MessageBoxButton.YesNo, MessageBoxImage.Warning) == MessageBoxResult.Yes)
                {
                    mountDisk(newDisk.directory + newDisk.diskName);
                }


            }
            catch (Exception ex)
            {

                MessageBox.Show(ex.Source + ": " + ex.Message, "Fms");
            }

        }

        #endregion

        #region Buttons
        private void mountButton_Click(object sender, RoutedEventArgs e)
        {
            WinForms.OpenFileDialog browser = new WinForms.OpenFileDialog();
            browser.Filter = "Bin Files (*.bin) |*.bin| All Files(*.*) | *.*";
            browser.FilterIndex = 1;
            browser.Multiselect = false;

            if (browser.ShowDialog() == WinForms.DialogResult.OK)
            {
                if (currentDisk == null)
                    currentDisk = new Disk();

                try
                {
                    mountDisk(browser.FileName);

                }
                catch (Exception ex)
                {
                    MessageBox.Show(ex.Source + ": " + ex.Message, "Fms");
                }

            }




        }
        private void formatButton_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                if (
                MessageBox.Show("WARNING: Formatting will erase ALL data on this disk.\nTo format the disk, click OK. To quit click CANCEL", "Format Disk",
                                MessageBoxButton.OKCancel,
                                MessageBoxImage.Exclamation,
                                MessageBoxResult.Cancel)
                                ==
                MessageBoxResult.OK
                    )
                {
                    formatDisk();
                    MessageBox.Show("Format complete", "Format Disk", MessageBoxButton.OK, MessageBoxImage.Information);
                }



            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message, "Format Disk", MessageBoxButton.OK, MessageBoxImage.Exclamation);
            }
        }

        private void unmountButton_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                if (currentDisk != null)
                    currentDisk.Unmountdisk();
                fileList.ItemsSource = null;

                MessageBox.Show("unmounted!", "unmount disk", MessageBoxButton.OK, MessageBoxImage.Information);
            }
            catch (Exception ex)
            {

                MessageBox.Show(ex.Source + ": " + ex.Message, "Fms");
            }
        }
        #endregion
    }
}
