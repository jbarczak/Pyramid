using System;
using System.Collections.Generic;
using System.Windows.Forms;

namespace Pyramid
{

    public interface IResultSet
    {
        string Name { get; }
        Control AnalysisPanel { get; }
        Control ResultsPanel { get; }
    };

   
    public interface IBackend
    {
        string Name { get; }
        IResultSet Compile( IShader shader );
    }
}
