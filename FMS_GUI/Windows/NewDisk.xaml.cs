using Microsoft.Win32;
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
using WinForms = System.Windows.Forms;


namespace FMS_GUI.Windows
{
    /// <summary>
    /// Interaction logic for NewDisk.xaml
    /// </summary>
    public partial class NewDisk : Window
    {
        public string directory;
        public string diskName;
        public string diskOwner;
        public bool cancel;


        public NewDisk()
        {
            InitializeComponent();
            cancel = true;
        }


        private void submit_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                directory = browserInput.Text.Replace(@"\", @"\\");

                if (string.IsNullOrEmpty(directory))
                    throw new Exception("Please select a directory");

                directory += @"\\";

                if (string.IsNullOrEmpty(diskNameInput.Text))
                    throw new Exception("Please insert the disk name");

                if (string.IsNullOrEmpty(diskOwnerInput.Text))
                    throw new Exception("Please insert the disk owner");

                cancel = false;

                diskName = diskNameInput.Text;
                diskOwner = diskOwnerInput.Text;

                this.Close();

            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message, "New disk", MessageBoxButton.OK, MessageBoxImage.Exclamation);
            }

        }
        private void cancel_Click(object sender, RoutedEventArgs e)
        {
            this.Close();
        }

        private void Browser_Click(object sender, RoutedEventArgs e)
        {
            WinForms.FolderBrowserDialog browser = new WinForms.FolderBrowserDialog();
            if (browser.ShowDialog() == System.Windows.Forms.DialogResult.OK)
            {
                browserInput.Text = browser.SelectedPath;

            }

        }
    }
}
