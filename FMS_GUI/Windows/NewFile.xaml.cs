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
    /// Interaction logic for NewFile.xaml
    /// </summary>
    public partial class NewFile : Window
    {
        public enum Type { none, student, file }
        public Type type;
        public bool result = false;
        public string filename = null;
        public int filesize = 0;

        public NewFile()
        {
            InitializeComponent();
        }



        private void cancel_Click(object sender, RoutedEventArgs e)
        {
            this.Close();
        }

        private void submit_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                ComboBoxItem item = (ComboBoxItem)typeBox.SelectedItem;
                if (item.Tag == null || item.Tag.ToString() == "None")
                    throw new Exception("Please select type");

                switch (item.Tag.ToString())
                {
                    case "Student":
                        if (string.IsNullOrEmpty(studentFileName.Text))
                            throw new Exception("File name cannot be empty");

                        filesize = int.Parse(studentFileSize.Text);

                        if (filesize < 1)
                            throw new Exception("The student file size must be bigger than 0");
                        filename = studentFileName.Text;
                        type = Type.student;

                        break;
                    case "File":
                        if (string.IsNullOrEmpty(browserInput.Text))
                            throw new Exception("Please select a file");
                        filename = browserInput.Text;
                        filename.Replace(@"\", @"\\");
                        type = Type.file;
                        break;
                    default:
                        break;
                }


                result = true;
                this.Close();

            }
            catch (FormatException)
            {
                MessageBox.Show("The student file size must be bigger than 0 and contains no characters", "New File");
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message, "New file");
            }
        }

        private void browserButton_Click(object sender, RoutedEventArgs e)
        {
            WinForms.OpenFileDialog browser = new WinForms.OpenFileDialog();
            browser.Filter = "All Files(*.*) | *.*";
            browser.FilterIndex = 1;
            browser.Multiselect = false;

            if (browser.ShowDialog() == WinForms.DialogResult.OK)
                browserInput.Text = browser.FileName;

        }
    }
}
