using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.Serialization;
using System.Text;
using System.Threading.Tasks;

namespace FMS_adapter
{
    class ProgramException : Exception
    {
        public ProgramException()
        {
        }

        public ProgramException(string message) : base(message)
        {
        }

        public ProgramException(string message, Exception innerException) : base(message, innerException)
        {
        }

        protected ProgramException(SerializationInfo info, StreamingContext context) : base(info, context)
        {
        }

        public ProgramException(string message, string source) : base(message)
        {
            base.Source = source;
        }
    }
}
