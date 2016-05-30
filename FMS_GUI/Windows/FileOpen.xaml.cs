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

        #region Names
        static string[] names = new string[] {
            "Noah","Liam","Mason","Jacob","James","John","Robert","Michael","Mary",
            "William","Richard","Charles","Joseph","Thomas","Patricia","Christopher",
            "Linda","Barbara","Daniel","Paul","Mark","Elizabeth","Donalt","Jennifer",
            "George","Maria"
        };

        static string[] surnames = new string[]
        {
            "Smith","Johnson","Brown","Davis","Miller","Wilson","Moore",
            "Taylor","Anderson","Thomas","Jackson"

        };
        #endregion

        public static Random ran = new Random();

        public FileOpen(FCB file)
        {
            InitializeComponent();

            allrec = new ObservableCollection<Student>();
            currentFile = file;
            this.Loaded += FileOpen_Loaded;
            this.KeyDown += FileOpen_KeyDown;
            Dispatcher.BeginInvoke(new Action(() =>
           MessageBox.Show("Due the program is in Beta status, press F1 to create aleatory names", "File open", MessageBoxButton.OK, MessageBoxImage.Information)
            ));

        }

        private void FileOpen_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.Key != Key.F1)
                return;

            int lastIndex = 1;
            if (allrec != null && allrec.Count > 0)
                lastIndex = allrec.Max(x => x.Id) + 1;

            try
            {
                Student stud = new Student(lastIndex, names[ran.Next(names.Length)] + " " + surnames[ran.Next(surnames.Length)], ran.Next(1, 6), ran.Next(100));

                currentFile.writeRec((object)stud);
                updateData();
            }
            catch (Exception ex)
            {

                MessageBox.Show(ex.Source + ": " + ex.Message, "Add student", MessageBoxButton.OK, MessageBoxImage.Error);
            }
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
            counterLabel.Content = "Total items: " + allrec.Count;


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
            Student temp = new Student();
            if (currentStud == null)
            {
                MessageBox.Show("Select a item", "Delete student", MessageBoxButton.OK, MessageBoxImage.Exclamation);
                return;
            }

            currentFile.seekToRecId((ulong)currentStud.Id);
            currentFile.readRec((object)temp, 1);

            if (temp.Id != currentStud.Id)
            {
                MessageBox.Show("Operation failed,", "Delete file", MessageBoxButton.OK, MessageBoxImage.Error);
                currentFile.updateRecCancel();
                return;
            }

            if (
                       MessageBox.Show("Are you sure you want to delete the object ID " + currentStud.Id + " ?",
                                       "Delete record",
                                       MessageBoxButton.YesNo,
                                       MessageBoxImage.Question)
                                       ==
                       MessageBoxResult.Yes
                       )
            {



                currentFile.deleteRec();

                updateData();
            }
            else
                currentFile.updateRecCancel();

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
