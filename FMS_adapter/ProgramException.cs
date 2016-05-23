using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.Serialization;
using System.Text;
using System.Threading.Tasks;
using System.IO;

namespace FMS_adapter
{
    public class ProgramException : Exception
    {
        StreamWriter file = (File.Exists(@"log.txt")) ? File.AppendText(@"log.txt") : File.CreateText(@"log.txt");
        public ProgramException()
        {
            file.WriteLine("Error on: " + DateTime.Now);
            file.Close();
        }

        public ProgramException(string message) : base(message)
        {
            file.WriteLine("Error: " + message + " on: " + DateTime.Now);
            file.Close();

        }

        public ProgramException(string message, Exception innerException) : base(message, innerException)
        {
            file.WriteLine("Error: " + message + " on: " + DateTime.Now);
            file.Close();

        }

        protected ProgramException(SerializationInfo info, StreamingContext context) : base(info, context)
        {
            file.WriteLine("Error on: " + DateTime.Now);
            file.Close();

        }

        public ProgramException(string message, string source) : base(message)
        {
            base.Source = source;

            file.WriteLine("Error from " + source + ": " + message + " on: " + DateTime.Now);
            file.Close();

        }
    }
}

