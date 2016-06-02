using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;

namespace FMS_adapter
{
    public enum FCBtypeToOpening { input = 0, output, inputOutput, Extension };
    public enum FCBseekfrom { beginning = 0, current, eof };
    public class FCB
    {


        private IntPtr myFCBpointer;

        public FCB(IntPtr myFCBpointer)
        {
            this.myFCBpointer = myFCBpointer;
        }

        ~FCB()
        {
            if (myFCBpointer != null)
            {
                //  cppToCsharpAdapter.closefile(myFCBpointer);
                cppToCsharpAdapter.deleteFcbObject(ref myFCBpointer);
            }
        }

        public void Closefile()
        {
            try
            {
                cppToCsharpAdapter.closefile(this.myFCBpointer);
            }
            catch (SEHException)
            {
                IntPtr cString = cppToCsharpAdapter.getLastFcbErrorMessage(this.myFCBpointer);
                string message = Marshal.PtrToStringAnsi(cString);
                cString = cppToCsharpAdapter.getLastFcbErrorSource(this.myFCBpointer);
                string source = Marshal.PtrToStringAnsi(cString);
                throw new ProgramException(message, source);
            }
            catch
            {
                throw;
            }
        }

        public object readRec(object dest, uint readForUpdate = 0)
        {
            try
            {

                IntPtr buffer;
                buffer = Marshal.AllocHGlobal(Marshal.SizeOf(dest.GetType()));

                cppToCsharpAdapter.readRec(this.myFCBpointer, buffer, readForUpdate);
                Marshal.PtrToStructure(buffer, dest);

                Marshal.FreeHGlobal(buffer);

                return dest;
            }
            catch (SEHException)
            {
                IntPtr cString = cppToCsharpAdapter.getLastFcbErrorMessage(this.myFCBpointer);
                string message = Marshal.PtrToStringAnsi(cString);
                cString = cppToCsharpAdapter.getLastFcbErrorSource(this.myFCBpointer);
                string source = Marshal.PtrToStringAnsi(cString);
                throw new ProgramException(message, source);
            }
            catch
            {
                throw;
            }
        }

        public IEnumerable<T> getAllRecord<T>() where T : class, new()
        {
            List<T> allRec = new List<T>();
            this.seekRec(FCBseekfrom.beginning, 0);
            T obj;
            int eof = getfileDesc().eofRecNr;



            for (int i = 0; i < eof; i++)
            {
                try
                {
                    obj = new T();
                    this.readRec((object)obj);
                    allRec.Add((obj as T));
                }
                catch (Exception)
                {
                    IntPtr cString = cppToCsharpAdapter.getLastFcbErrorMessage(this.myFCBpointer);
                    string message = Marshal.PtrToStringAnsi(cString);
                    cString = cppToCsharpAdapter.getLastFcbErrorSource(this.myFCBpointer);
                    string source = Marshal.PtrToStringAnsi(cString);
                    new ProgramException(message, source);

                }
            }


            return allRec;
        }

        public void writeRec(object source)
        {
            try
            {
                IntPtr buffer = Marshal.AllocHGlobal(Marshal.SizeOf(source.GetType()));
                Marshal.StructureToPtr(source, buffer, true);

                cppToCsharpAdapter.writeRec(this.myFCBpointer, buffer);

                Marshal.FreeHGlobal(buffer);
            }
            catch (SEHException)
            {
                IntPtr cString = cppToCsharpAdapter.getLastFcbErrorMessage(this.myFCBpointer);
                string message = Marshal.PtrToStringAnsi(cString);
                cString = cppToCsharpAdapter.getLastFcbErrorSource(this.myFCBpointer);
                string esource = Marshal.PtrToStringAnsi(cString);
                throw new ProgramException(message, esource);
            }
            catch (Exception)
            { throw; }

        }

        public void seekRec(FCBseekfrom from, int pos)
        {
            try
            {
                cppToCsharpAdapter.seekRec(this.myFCBpointer, (uint)from, pos);
            }
            catch (SEHException)
            {
                IntPtr cString = cppToCsharpAdapter.getLastFcbErrorMessage(this.myFCBpointer);
                string message = Marshal.PtrToStringAnsi(cString);
                cString = cppToCsharpAdapter.getLastFcbErrorSource(this.myFCBpointer);
                string source = Marshal.PtrToStringAnsi(cString);
                throw new ProgramException(message, source);
            }
            catch
            {
                throw;
            }
        }

        public void seekToRecId(ulong id)
        {
            try
            {
                cppToCsharpAdapter.seekToRecId(this.myFCBpointer, id);
            }
            catch (SEHException)
            {
                IntPtr cString = cppToCsharpAdapter.getLastFcbErrorMessage(this.myFCBpointer);
                string message = Marshal.PtrToStringAnsi(cString);
                cString = cppToCsharpAdapter.getLastFcbErrorSource(this.myFCBpointer);
                string source = Marshal.PtrToStringAnsi(cString);
                throw new ProgramException(message, source);
            }
            catch
            {
                throw;
            }
        }
        public void updateRecCancel()
        {
            try
            {
                cppToCsharpAdapter.updateRecCancel(this.myFCBpointer);
            }
            catch (SEHException)
            {
                IntPtr cString = cppToCsharpAdapter.getLastFcbErrorMessage(this.myFCBpointer);
                string message = Marshal.PtrToStringAnsi(cString);
                cString = cppToCsharpAdapter.getLastFcbErrorSource(this.myFCBpointer);
                string source = Marshal.PtrToStringAnsi(cString);
                throw new ProgramException(message, source);
            }
            catch
            {
                throw;
            }
        }

        public void deleteRec()
        {
            try
            {
                cppToCsharpAdapter.deleteRec(this.myFCBpointer);
            }
            catch (SEHException)
            {
                IntPtr cString = cppToCsharpAdapter.getLastFcbErrorMessage(this.myFCBpointer);
                string message = Marshal.PtrToStringAnsi(cString);
                cString = cppToCsharpAdapter.getLastFcbErrorSource(this.myFCBpointer);
                string source = Marshal.PtrToStringAnsi(cString);
                throw new ProgramException(message, source);
            }
            catch
            {
                throw;
            }
        }

        public void updateRec(object source)
        {
            try
            {
                IntPtr buffer = Marshal.AllocHGlobal(Marshal.SizeOf(source.GetType()));
                Marshal.StructureToPtr(source, buffer, true);
                cppToCsharpAdapter.updateRec(this.myFCBpointer, buffer);
                Marshal.FreeHGlobal(buffer);
            }
            catch (SEHException)
            {
                IntPtr cString = cppToCsharpAdapter.getLastFcbErrorMessage(this.myFCBpointer);
                string message = Marshal.PtrToStringAnsi(cString);
                cString = cppToCsharpAdapter.getLastFcbErrorSource(this.myFCBpointer);
                string esource = Marshal.PtrToStringAnsi(cString);
                throw new ProgramException(message, esource);
            }
            catch
            {
                throw;
            }
        }

        public DirEntry getfileDesc()
        {
            try
            {

                DirEntry dirTemp;
                int structSize = Marshal.SizeOf(typeof(DirEntry)); //Marshal.SizeOf(typeof(Student)); 
                IntPtr buffer = Marshal.AllocHGlobal(structSize);

                dirTemp = new DirEntry();
                Marshal.StructureToPtr(dirTemp, buffer, true);
                cppToCsharpAdapter.getfileDesc(this.myFCBpointer, buffer);
                Marshal.PtrToStructure(buffer, dirTemp);
                Marshal.FreeHGlobal(buffer);
                return dirTemp;


            }
            catch (SEHException)
            {
                IntPtr cString = cppToCsharpAdapter.getLastFcbErrorMessage(this.myFCBpointer);
                string message = Marshal.PtrToStringAnsi(cString);
                cString = cppToCsharpAdapter.getLastFcbErrorSource(this.myFCBpointer);
                string source = Marshal.PtrToStringAnsi(cString);
                throw new ProgramException(message, source);

            }
        }

        public string getFAT()
        {
            try
            {
                IntPtr DAT = cppToCsharpAdapter.getFAT(this.myFCBpointer);
                return Marshal.PtrToStringAnsi(DAT).ToString();

            }
            catch (SEHException)
            {
                IntPtr cString = cppToCsharpAdapter.getLastFcbErrorMessage(this.myFCBpointer);
                string message = Marshal.PtrToStringAnsi(cString);
                cString = cppToCsharpAdapter.getLastFcbErrorSource(this.myFCBpointer);
                string source = Marshal.PtrToStringAnsi(cString);
                throw new ProgramException(message, source);
            }



        }


        public void extendfile(uint AmountOfSectors)
        {
            try
            {
                cppToCsharpAdapter.FCBextendfile(this.myFCBpointer, AmountOfSectors);
            }
            catch (SEHException)
            {
                IntPtr cString = cppToCsharpAdapter.getLastFcbErrorMessage(this.myFCBpointer);
                string message = Marshal.PtrToStringAnsi(cString);
                cString = cppToCsharpAdapter.getLastFcbErrorSource(this.myFCBpointer);
                string source = Marshal.PtrToStringAnsi(cString);
                throw new ProgramException(message, source);
            }
            catch
            {
                throw;
            }
        }

        public void exportFile<T>(string path) where T : class, new()
        {
            StreamWriter file;
            if (File.Exists(path))
                file = File.AppendText(path);
            else
                file = File.CreateText(path);

            foreach (var item in getAllRecord<T>())
            {
                file.WriteLine(item.ToString());
            }



            file.Close();
        }
    }
}
