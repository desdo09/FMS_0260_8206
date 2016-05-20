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
using FMS_GUI.Classes;
using WinForms = System.Windows.Forms;
using System.Runtime.InteropServices;

namespace FMS_GUI
{

    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {

        /*Objects*/
        public Disk currentDisk;
        private VolumeHeader vhd;
        private List<DirEntry> allFiles;
        private double diskUsed
        {
            get { return DiskUsedBar.Value; }
            set { DiskUsedBar.Value = value; }
        }
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
            updateFileList();
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
                updateFileList();


            MessageBox.Show("Name:" + vhd.DiskName + "\nOwner:" + vhd.DiskOwner);


        }

        public void formatDisk()
        {
            if (currentDisk == null || !currentDisk.isMounted())
                throw new Exception("No disk mounted");

            this.currentDisk.Format();

            allFiles = currentDisk.getDirEntryInRootDir();

            fileList.ItemsSource = allFiles;

            updateFileList();
        }
        private void updateFileList()
        {
            allFiles = currentDisk.getDirEntryInRootDir();

            fileList.ItemsSource = allFiles;

            diskUsed = 1600 - this.currentDisk.Howmuchempty();

            totalItemsLabel.Content = allFiles.Count + " Files";

            double totalUsed = (diskUsed * 2 / 3200) * 100;

            diskUsedLabel.Content = "Total " + 3.200 + " MB, Used " + (diskUsed * 2) + " Kb (" + totalUsed + "%)";
        }

        #endregion

        #region Buttons
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
                if (currentDisk == null || !currentDisk.isMounted())
                {
                    MessageBox.Show("There is no mounted disk", "Disk info", MessageBoxButton.OK, MessageBoxImage.Error);
                    return;
                }

                currentDisk.Unmountdisk();
                fileList.ItemsSource = null;

                MessageBox.Show("unmounted!", "unmount disk", MessageBoxButton.OK, MessageBoxImage.Information);

                currentDisk = null;

                diskUsed = 0;

                totalItemsLabel.Content = 0 + " Files";

                diskUsedLabel.Content = "";

            }
            catch (Exception ex)
            {

                MessageBox.Show(ex.Source + ": " + ex.Message, "Fms");
            }
        }
        private void newButton_Click(object sender, RoutedEventArgs e)
        {
            if (currentDisk == null || !currentDisk.isMounted())
            {
                MessageBox.Show("Mount disk is necessary", "New file");
                return;
            }
            try
            {
                NewFile file = new NewFile();
                file.ShowDialog();

                if (file.result == true)
                {
                    switch (file.type)
                    {
                        case NewFile.Type.student:
                            this.currentDisk.createStudentfile(file.filesize, file.filename, Marshal.SizeOf(typeof(Student)));
                            break;
                        case NewFile.Type.file:
                            this.currentDisk.importfile(file.filename);
                            break;
                        default:
                            break;
                    }

                    updateFileList();
                }

            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Source + ": " + ex.Message, "New file");
            }
        }

        private void deleteButton_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                DirEntry current = fileList.SelectedItem as DirEntry;
                if (current != null)
                {
                    if (
                        MessageBox.Show("Are you sure you want to delete " + current.filename + "?",
                                        "Delete file",
                                        MessageBoxButton.YesNo,
                                        MessageBoxImage.Exclamation)
                                        ==
                        MessageBoxResult.Yes
                        )
                    {
                        currentDisk.Delfile(((DirEntry)fileList.SelectedItem).filename);

                        updateFileList();
                    }
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Source + ": " + ex.Message, "Delete file");
            }
        }

        private void saveButton_Click(object sender, RoutedEventArgs e)
        {
            if (currentDisk == null || !currentDisk.isMounted())
            {
                MessageBox.Show("Mount disk is necessary", "Save file");
                return;
            }
            string dir;
            DirEntry current = fileList.SelectedItem as DirEntry;
            WinForms.FolderBrowserDialog browser = new WinForms.FolderBrowserDialog();
            StatusWindows status = new StatusWindows("Exporting file:", currentDisk.getStatus);
            try
            {
                if (browser.ShowDialog() == System.Windows.Forms.DialogResult.OK && current != null)
                {
                    dir = browser.SelectedPath;
                    dir.Replace(@"\", @"\\");
                    status.Show();
                    currentDisk.exportFile(dir, current.filename);
                    status.Close();

                }
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Source + ": " + ex.Message, "Save file", MessageBoxButton.OK, MessageBoxImage.Error);
                if (status.IsLoaded)
                    status.Close();
            }
        }

        private void diskInfoButton_Click(object sender, RoutedEventArgs e)
        {
            if (currentDisk == null || !currentDisk.isMounted())
            {
                MessageBox.Show("There is no mounted disk", "Disk info", MessageBoxButton.OK, MessageBoxImage.Error);
                return;
            }

            try
            {
                PrintDat info = new PrintDat(currentDisk.getDAT(), vhd);
                info.Show();
            }
            catch (Exception ex)
            {

                MessageBox.Show(ex.Source + ": " + ex.Message, "Disk info", MessageBoxButton.OK, MessageBoxImage.Error);
            }


        }

        private void openButton_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                DirEntry current = fileList.SelectedItem as DirEntry;
                if (current == null)
                    throw new Exception("Select a file");
                FCB file = currentDisk.Openfile(current.filename, FCBtypeToOpening.inputOutput);

                FileOpen update = new FileOpen(file);
                update.Show();

            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Source + ": " + ex.Message, "Open File", MessageBoxButton.OK, MessageBoxImage.Error);
            }
        }
        #endregion
    }

}
