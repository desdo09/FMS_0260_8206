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
using FMS_GUI.Classes;
using System.Collections.ObjectModel;

namespace FMS_GUI.Windows
{
    /// <summary>
    /// Interaction logic for FileOpen.xaml
    /// </summary>
    public partial class FileOpen : Window
    {
        FCB currentFile;
        DirEntry fileDesc;
        ObservableCollection<Student> allrec;
        public FileOpen(FCB file)
        {
            InitializeComponent();

            allrec = new ObservableCollection<Student>();
            currentFile = file;
            this.Loaded += FileOpen_Loaded;





        }

        private void FileOpen_Loaded(object sender, RoutedEventArgs e)
        {
            try
            {
                //check if fcb is null
                if (currentFile == null)
                    throw new FMS_adapter.ProgramException("Fcb cannot be null", "Open file");


                //get file's DirEntry
                fileDesc = currentFile.getfileDesc();
                //check if the DirEntry is null
                if (fileDesc == null)
                    throw new Exception("Fatal error, fileDesc is null");

                //check if the file are .stu
                if (fileDesc.filename.Substring(fileDesc.filename.LastIndexOf('.')) != ".stud")
                    throw new Exception("The file is not a student file");


                recList.ItemsSource = allrec;

                getAllRecord();

                this.Title = "Open file - " + fileDesc.filename;

            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Source + ": " + ex.Message, "Open file", MessageBoxButton.OK, MessageBoxImage.Error);
                this.Close();
            }
        }

        private void updateData()
        {
            fileDesc = currentFile.getfileDesc();
            if (fileDesc == null)
                throw new Exception("Fatal error, fileDesc is null");

            getAllRecord();
        }

        private void getAllRecord()
        {
            allrec.Clear();
            currentFile.seekRec(FCBseekfrom.beginning, 0);
            Student stud;
            for (int i = 0; i < fileDesc.eofRecNr; i++)
            {
                stud = new Student();
                currentFile.readRec((object)stud);
                if (stud.Id != 0)
                    allrec.Add(stud);
                
            }

        }

        private void newStudentButton_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                StudentManager newStud = new StudentManager();
                newStud.ShowDialog();
                if (newStud.result)
                {
                    currentFile.writeRec((object)newStud.student);
                    updateData();
                }
                else
                    MessageBox.Show("canceled");
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Source + ": " + ex.Message, "New student", MessageBoxButton.OK, MessageBoxImage.Error);
            }
        }
    }
}
