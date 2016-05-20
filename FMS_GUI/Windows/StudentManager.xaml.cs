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
using FMS_GUI.Classes;

namespace FMS_GUI.Windows
{
    /// <summary>
    /// Interaction logic for StudentManager.xaml
    /// </summary>
    public partial class StudentManager : Window
    {
        public Student student;
        public bool result = false;
        public StudentManager(Student stud)
        {
            InitializeComponent();


            newStud.Visibility = Visibility.Collapsed;
            updateStud.Visibility = Visibility.Visible;

            idText.Text = stud.Id.ToString();
            nameText.Text = stud.Name;
            yearText.Text = stud.Year.ToString();
            averageText.Text = stud.Averange.ToString();


        }
        public StudentManager()
        {
            InitializeComponent();

        }
        private void Button_Click(object sender, RoutedEventArgs e)
        {


            try
            {
                if (string.IsNullOrEmpty(idText.Text) || int.Parse(idText.Text) < 1)
                    throw new Exception("Id cannot be empty or lass than 1");

                if (string.IsNullOrEmpty(nameText.Text))
                    throw new Exception("name cannot be empty");

                if (string.IsNullOrEmpty(yearText.Text) || int.Parse(yearText.Text) < 1)
                    throw new Exception("Year cannot be empty or lass than 1");

                if (string.IsNullOrEmpty(averageText.Text) || float.Parse(averageText.Text) < 1)
                    throw new Exception("Id cannot be empty or lass than 1");

                result = true;
                student = new Student
                {
                    Id = (int.Parse(idText.Text)),
                    Name = nameText.Text,
                    Averange = float.Parse(averageText.Text),
                    Year = int.Parse(yearText.Text)
                };

                this.Close();

            }
            catch (FormatException)
            {

                MessageBox.Show("The fields id, year and average can only contain number", "Student", MessageBoxButton.OK, MessageBoxImage.Warning);
            }
            catch (Exception ex)
            {

                MessageBox.Show(ex.Message, "Student", MessageBoxButton.OK, MessageBoxImage.Warning);
            }
        }

        private void Button_Click_1(object sender, RoutedEventArgs e)
        {
            result = false;
            this.Close();
        }
    }
}
