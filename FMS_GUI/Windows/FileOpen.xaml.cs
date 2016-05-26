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
                    throw new ArgumentException("Fcb cannot be null");


                //get file's DirEntry
                fileDesc = currentFile.getfileDesc();
                //check if the DirEntry is null
                if (fileDesc == null)
                    throw new ArgumentException("Fatal error, fileDesc is null");

                //check if the file are .stu
                if (fileDesc.filename.Substring(fileDesc.filename.LastIndexOf('.')) != ".stud")
                    throw new ArgumentException("The file is not a student file");


                recList.ItemsSource = allrec;

                getAllRecord();

                this.Title = "Open file - " + fileDesc.filename;

            }
            catch (ArgumentException ex)
            {
                MessageBox.Show(ex.Message, "Open file", MessageBoxButton.OK, MessageBoxImage.Error);
                this.Close();
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Source + ": " + ex.Message, "Open file", MessageBoxButton.OK, MessageBoxImage.Error);
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
            foreach (var item in currentFile.getAllRecord<Student>())
            {

                allrec.Add(item);

            }
            //  if(allrec.Count>0)
            // currentFile.writeRec((object)allrec.FirstOrDefault());

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

            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Source + ": " + ex.Message, "New student", MessageBoxButton.OK, MessageBoxImage.Error);
            }
        }

        private void deleteStudentButton_Click(object sender, RoutedEventArgs e)
        {
            Student currentStud = recList.SelectedItem as Student;
            if (currentStud == null)
            {
                MessageBox.Show("Select a item", "Delete student", MessageBoxButton.OK, MessageBoxImage.Exclamation);
                return;
            }

            if (
                       MessageBox.Show("Are you sure you want to delete the object ID " + currentStud.Id + " ?",
                                       "Delete record",
                                       MessageBoxButton.YesNo,
                                       MessageBoxImage.Exclamation)
                                       ==
                       MessageBoxResult.Yes
                       )
            {
                currentFile.seekToRecId((ulong)currentStud.Id);
                currentFile.deleteRec();

                updateData();
            }

        }

        private void updateStudentButton_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                Student currentStud = recList.SelectedItem as Student;
                if (currentStud == null)
                {
                    MessageBox.Show("Select a item", "Update student", MessageBoxButton.OK, MessageBoxImage.Exclamation);
                    return;
                }

                currentFile.seekToRecId((ulong)currentStud.Id);
                currentFile.readRec(currentStud, 1);

                StudentManager updateStud = new StudentManager(currentStud);
                updateStud.ShowDialog();
                if (updateStud.result)
                {
                    currentFile.updateRec((object)updateStud.student);
                    updateData();
                }



            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Source + ": " + ex.Message, "Update student", MessageBoxButton.OK, MessageBoxImage.Error);
            }
            finally
            {
                currentFile.updateRecCancel();
            }
        }
    }
}
