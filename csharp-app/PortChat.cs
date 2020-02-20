using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO.Ports;
using System.Threading;
using System.Data;
using System.Diagnostics;

namespace MusicBallProLogs
{
    class PortChat
    {

        static Stopwatch stopwatch;  
        static bool _continue;
        static SerialPort _serialPort;
        private static DataTable table;
        private Thread readThread;
        static public int[,] buffer = new int[3, 4];
        static long old_timestamp = 0; 
        static long new_timestamp = 0; 
        public  PortChat(DataTable table, string port)
        {
       
            StringComparer stringComparer = StringComparer.OrdinalIgnoreCase;
            readThread = new Thread(Read);
            PortChat.table = table; 
           
            _serialPort = new SerialPort();
            _serialPort.PortName = port;  
            _serialPort.BaudRate = 115200;  
            _serialPort.Parity =  (Parity)Enum.Parse(typeof(Parity), "None", true);
            _serialPort.DataBits = 8;  
            _serialPort.StopBits =  (StopBits)Enum.Parse(typeof(StopBits), "One", true);
            _serialPort.Handshake = (Handshake)Enum.Parse(typeof(Handshake), "None", true);
            _serialPort.ReadTimeout = 500;
            _serialPort.WriteTimeout = 500;

            _serialPort.Open();
            _continue = true;
            readThread.Start();
            stopwatch = new Stopwatch();
            stopwatch.Reset();
            stopwatch.Start();

        }

        public void Close() {

            _continue = false; 
           readThread.Join();
            _serialPort.Close();

        }

        public static void Read()
        {
            while (_continue)
            {
                try
                {
                    string message = _serialPort.ReadLine();
                    Console.Write(message);
                    string[] values = message.Split(' ');
                     //Console.WriteLine(" ------  {0}", values.Length);
		//     new_timestamp = stopwatch.ElapsedTicks; 
                //    Console.WriteLine( (new_timestamp - old_timestamp) +" -- " + message );  
		//    old_timestamp = new_timestamp; 
		    if (13 == values.Length) {
			    for (int i = 0; i < 12; i++) {
                                 table.Rows[i / 3][i % 3] = values[i];
                                 PortChat.buffer[i % 3, i / 3] = int.Parse(values[i]);

			    }

		    }
                    

                }
                catch (TimeoutException) { }
            }
        }

    }
}
