using System;
using System.Runtime.InteropServices;

namespace CsharpAppWithCppDll
{
    class Program
    {
        [DllImport("CppDllForCsharp.dll", EntryPoint = "SumNumbers")]
        public static extern int SumNumbers(int x, int y);

        [DllImport("CppDllForCsharp.dll")]
        public static extern IntPtr GetCurrentDate();

        [DllImport("user32.dll")]
        public static extern int MessageBox(int hWnd, string pText, string pCaption, int utype);


        [STAThread]
        private static void Main(string[] args)
        {
            //MessageBox(0, "Hello, programmer", "See dll in action", 0);

            Console.WriteLine("Sum of 2 and 3 is {0}", SumNumbers(2, 3));
            Console.WriteLine("Current date: " + PointerToStringUtf8(GetCurrentDate()));
        }

        private static string PointerToStringUtf8(IntPtr pointer)
        {
            int pointerValueLength = 0;
            while (Marshal.ReadByte(pointer, pointerValueLength) != 0)
                pointerValueLength++;

            byte[] pointerValueBytes = new byte[pointerValueLength];

            Marshal.Copy(pointer, pointerValueBytes, 0, pointerValueLength);

            return System.Text.Encoding.UTF8.GetString(pointerValueBytes);
        }
    }
}
