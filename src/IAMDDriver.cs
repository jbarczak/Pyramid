using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Pyramid
{
    public interface IAMDAsic
    {
        string Name { get; }
    };


    public interface IAMDShader 
    {
        IAMDAsic Asic { get; }
        byte[] ReadISABytes();
        string Disassemble();
        string ListEncodings();
        string PrintStats( );
        Scrutinizer.IScrutinizer CreateScrutinizer( );
    };

    public interface IAMDDriver
    {
        IEnumerable<IAMDAsic> Asics { get; }

        /// <summary>
        ///  Compile a DX shader blob and return the 
        /// </summary>
        /// <param name="blob"></param>
        /// <returns></returns>
        IAMDShader CompileDXBlob(IAMDAsic asic, byte[] blob, IDXShaderReflection reflect );
    }
}
