using System;
using System.Collections.Generic;
using System.Windows.Forms;
using System.Reflection;

namespace Pyramid
{
    static class Program
    {
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main()
        {
            
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);

            Options opts = Options.Get();
            System.IO.Directory.CreateDirectory(opts.TempPath);


            IWrapper wrapper = new Wrapper();
            MainForm main = new MainForm( opts, wrapper );
            Application.Run(main);

            

            main.Options.Write();
        }
    }
}
