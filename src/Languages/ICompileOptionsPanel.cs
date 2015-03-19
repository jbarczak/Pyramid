using System;
using System.Collections.Generic;
using System.Windows.Forms;

namespace Pyramid
{


    public interface ICompileOptionsPanel
    {
        Control Panel { get; }
        ICompileOptions ReadOptions();
    };



   
}
