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
using System.IO;
using System.Globalization;
using System.Diagnostics;
using System.Reflection;
using System.Threading;

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
        private string diskDir = null;
        private List<DirEntry> allFiles;

        public static List<FCB> allFcb;
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
            allFcb = new List<FCB>();
            fileList.ItemsSource = allFiles;
            this.Loaded += MainWindow_Loaded;


        }

        private void MainWindow_Loaded(object sender, RoutedEventArgs e)
        {

            try
            {
                if (!string.IsNullOrEmpty(App.args))
                {
                    //  MessageBox.Show(App.args, "main");
                    this.diskDir = App.args.Replace(@"\", @"\\");
                    this.mountDisk(App.args.Replace(@"\", @"\\"));
                }
            }
            catch (FMS_adapter.ProgramException ex)
            {
                MessageBox.Show(ex.Message, ex.Source, MessageBoxButton.OK, MessageBoxImage.Exclamation);
            }
            catch (Exception ex)
            {
                new FMS_adapter.ProgramException(ex.Message);
                MessageBox.Show(ex.Message, ex.Source, MessageBoxButton.OK, MessageBoxImage.Exclamation);
            }
        }

        private void Window_Closing(object sender, System.ComponentModel.CancelEventArgs e)
        {

            MessageBoxResult res = MessageBox.Show("Are you sure you want to close FMS?", "Exit from FMS", MessageBoxButton.YesNo, MessageBoxImage.Question);

            if (res == MessageBoxResult.Yes)
                Application.Current.Shutdown();
            else
                if (e != null)
                e.Cancel = true;

        }






        #region Operations
        public void mountDisk(string path)
        {

            if (currentDisk == null)
                currentDisk = new Disk();


            if (string.IsNullOrEmpty(path))
                throw new ArgumentException("Path cannot be null");

            currentDisk.Mountdisk(path);

            updateFileList();

            vhd = currentDisk.GetVolumeHeader();

            if (vhd == null)
                throw new ArgumentException("vhd cannot be null");

            if (!vhd.IsFormated)
            {
                MessageBoxResult? rst = MessageBox.Show("\nYou need to format the disk before you can used,\n\nformat now?", "New Disk", MessageBoxButton.YesNo, MessageBoxImage.Warning);
                if (rst != null && rst == MessageBoxResult.Yes)
                {
                    formatDisk();
                    MessageBox.Show("Format complete", "Format Disk", MessageBoxButton.OK, MessageBoxImage.Information, MessageBoxResult.OK);
                }

            }


            if (vhd.IsFormated)
                updateFileList();
            else
                diskDataGrid.Items.Add(vhd);



            this.Title = this.Title + " - " + diskDir;


        }

        public void formatDisk()
        {
            if (currentDisk == null || !currentDisk.isMounted())
                throw new Exception("No disk mounted");

            this.currentDisk.Format();

            allFcb.Clear();

            allFiles = currentDisk.getDirEntryInRootDir();

            fileList.ItemsSource = allFiles;

            updateFileList();
        }
        private void updateFileList()
        {
            allFiles = currentDisk.getDirEntryInRootDir();

            fileList.ItemsSource = allFiles;

            if (diskDataGrid.Items.Count > 0)
                diskDataGrid.Items.Clear();
            if (vhd != null)
                diskDataGrid.Items.Add(vhd);

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
                    diskDir = browser.FileName;
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
                if (currentDisk == null)
                    throw new Exception("Select a mounted disk");
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
        private void defragmentButton_Click(object sender, RoutedEventArgs e)
        {
            if (currentDisk == null || !currentDisk.isMounted())
            {
                MessageBox.Show("Select a mounted disk", "Disk info", MessageBoxButton.OK, MessageBoxImage.Error);
                return;
            }

            try
            {
                currentDisk.defragment();
                new Thread(() => MessageBox.Show("the disc has successfully defragmented", "defragmentButton", MessageBoxButton.OK, MessageBoxImage.Information)).Start();
                allFcb.Clear();

                vhd = currentDisk.GetVolumeHeader();
                currentDisk.flush();
                updateFileList();
            }
            catch (Exception ex)
            {

                MessageBox.Show(ex.Message, "disk defragment", MessageBoxButton.OK, MessageBoxImage.Error);
            }
        }
        private void unmountButton_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                if (currentDisk == null || !currentDisk.isMounted())
                {
                    MessageBox.Show("Select a mounted disk", "Disk info", MessageBoxButton.OK, MessageBoxImage.Error);
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
                MessageBox.Show("Select a mounted disk", "New file");
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
                if (currentDisk == null || !currentDisk.isMounted())
                {
                    MessageBox.Show("Select a mounted disk", "Delete file");
                    return;
                }
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
                        allFcb.Remove(
                                allFcb.Where(x => x.getfileDesc().filename == ((DirEntry)fileList.SelectedItem).filename).FirstOrDefault()
                            );
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
                MessageBox.Show("Select a mounted disk", "Save file");
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
                MessageBox.Show("Select a mounted disk", "Disk info");
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
                if (currentDisk == null || !currentDisk.isMounted())
                {
                    MessageBox.Show("Select a mounted disk", "Open file");
                    return;
                }
                DirEntry current = fileList.SelectedItem as DirEntry;
                if (current == null)
                    throw new Exception("Select a file");

                FCB file = allFcb.Where(x => x.getfileDesc().filename == current.filename).FirstOrDefault();
                if (file == null)
                {
                    file = currentDisk.Openfile(current.filename, FCBtypeToOpening.inputOutput);
                    allFcb.Add(file);
                }

                FileOpen update = new FileOpen(file);
                update.Show();

            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Source + ": " + ex.Message, "Open File", MessageBoxButton.OK, MessageBoxImage.Error);
            }
        }
        private void fileInfoButton_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                if (currentDisk == null || !currentDisk.isMounted())
                    throw new Exception("Select a mounted disk");

                DirEntry current = fileList.SelectedItem as DirEntry;
                if (current == null)
                    throw new Exception("Select a file");

                FCB file = allFcb.Where(x => x.getfileDesc().filename == current.filename).FirstOrDefault();

                if (file == null)
                {
                    file = currentDisk.Openfile(current.filename, FCBtypeToOpening.inputOutput);
                    allFcb.Add(file);
                }
                string dat = currentDisk.getDAT();
                PrintDat print = new PrintDat(file, dat);
                print.Show();
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message, "File info", MessageBoxButton.OK, MessageBoxImage.Error);
            }

        }
        private void MenuItem_Click(object sender, RoutedEventArgs e)
        {
            Window_Closing(null, null);
        }

        #endregion


        private void diskDataGrid_MouseLeave(object sender, MouseEventArgs e)
        {
            DataGrid dg = sender as DataGrid;
            if (dg == null)
                return;
            if (dg.RowDetailsVisibilityMode == DataGridRowDetailsVisibilityMode.VisibleWhenSelected)
                dg.RowDetailsVisibilityMode = DataGridRowDetailsVisibilityMode.Collapsed;
            else
                dg.RowDetailsVisibilityMode = DataGridRowDetailsVisibilityMode.VisibleWhenSelected;

        }


        private void terminalButton_Click(object sender, RoutedEventArgs e)
        {
            try
            {

                // Prepare the process to run
                ProcessStartInfo start = new ProcessStartInfo();
                // Enter in the command line arguments, everything you would enter after the executable name itself
                start.Arguments = diskDir;
                // Enter the executable to run, including the complete path
                start.FileName = System.IO.Path.GetDirectoryName(Assembly.GetEntryAssembly().Location) + @"\FMS_Console.exe";

                using (Process proc = Process.Start(start))
                {
                    proc.WaitForExit();


                }
                if (currentDisk != null)
                    this.updateFileList();

            }
            catch (Exception ex)
            {

                MessageBox.Show(ex.Message);
            }



        }

        private void about_Click(object sender, RoutedEventArgs e)
        {
            About.showDialog();
        }

    }


}
