using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace FMS_GUI.Classes
{

    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]
    public class Student
    {

        private int _id;
        public int Id
        {
            get
            {
                return _id;
            }
            set
            {
                _id = value;
            }
        }

        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 20)]
        private string _Name;
        public string Name
        {
            get
            {
                return _Name;
            }
            set
            {
                _Name = value;
            }
        }

        int _year;
        public int Year
        {
            get
            {
                return _year;
            }

            set
            {
                _year = value;
            }
        }

        float _average;
        public float Averange
        {
            get
            {
                return _average;
            }

            set
            {
                _average = value;
            }
        }


        public Student(int Id, string Name, int Year, float Averange)
        {
            this.Id = Id;
            this.Name = Name;
            this.Year = Year;
            this.Averange = Averange;
        }
        public Student()
        {

        }
    }

}
