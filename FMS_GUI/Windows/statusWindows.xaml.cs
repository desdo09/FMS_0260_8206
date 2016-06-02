using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
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
    /// Interaction logic for statusWindows.xaml
    /// </summary>
    public partial class StatusWindows : Window
    {
        static Func<double> status = null;
        public StatusWindows(string title, Func<double> status)
        {
            InitializeComponent();
            titleLabel.Content = title;
            StatusWindows.status = status;
            this.Closing += StatusWindows_Closing;



        }

        private void StatusWindows_Closing(object sender, System.ComponentModel.CancelEventArgs e)
        {
            status = null;
        }


        public void start()
        {
            Action del = null;
            del = () =>
            {
                if (status != null)
                {
                    MessageBox.Show("Del" + status().ToString());
                    progress.Value = (double)StatusWindows.status();
                }
                else MessageBox.Show("Null");
            };

            Thread add = new Thread(() =>
           {

               while (status != null)
               {
                   //Thread.Sleep(500);
                   if (status != null)
                       Dispatcher.BeginInvoke(del);
                //   MessageBox.Show(status().ToString());

               }

           });

            add.Start();
        }



    }
}
