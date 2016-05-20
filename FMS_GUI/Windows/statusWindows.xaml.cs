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
        Func<double> status = null;
        public StatusWindows(string title, Func<double> status)
        {
            InitializeComponent();
            titleLabel.Content = title;
            this.status = status;
            this.Loaded += StatusWindows_Loaded;
        }


        bool _shown;

        protected override void OnContentRendered(EventArgs e)
        {
            base.OnContentRendered(e);

            if (_shown)
                return;

            _shown = true;

            Action del = () => progress.Value = (double)status();
            new Thread(() =>
            {

                while (status != null)
                {
                    //Thread.Sleep(500);

                    Dispatcher.Invoke(del);
              //      MessageBox.Show(status().ToString());
                }

            }).Start();
        }

        private void StatusWindows_Loaded(object sender, RoutedEventArgs e)
        {


        }
    }
}
