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

namespace FMS_GUI.Windows
{
    /// <summary>
    /// Interaction logic for Prompt.xaml
    /// </summary>
    public partial class Prompt : Window
    {
        bool number;
        string result = null;

        /// <summary>
        /// A little prompt 
        /// </summary>
        /// <param name="Title">The title that will appear at the message</param>
        /// <param name="number">If input need be only numbers</param>
        public Prompt(string Title, bool number = false)
        {
            InitializeComponent();
            this.number = number;
            titleLabel.Content = Title;
            this.KeyDown += Prompt_KeyDown;
        }

        private void Prompt_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.Key == Key.Enter)
                submit_Click(null, null);
        }

        public static string show(string Title, bool number = false)
        {
            Prompt a = new Prompt(Title, number);
            a.ShowDialog();
            return a.result;
        }


        private void cancel_Click(object sender, RoutedEventArgs e)
        {

            result = null;
            this.Close();
        }
        private void submit_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                if (string.IsNullOrEmpty(inputText.Text))
                    throw new Exception("Input cannot be empty");

                if (this.number)
                {
                    result = int.Parse(inputText.Text).ToString();
                }
                else
                {
                    result = inputText.Text;
                }
                this.Close();

            }
            catch (FormatException)
            {
                MessageBox.Show("The input can be only numbers", "Prompt", MessageBoxButton.OK, MessageBoxImage.Hand);
                result = null;
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message, "Prompt", MessageBoxButton.OK, MessageBoxImage.Hand);
                result = null;
            }
        }
    }
}
