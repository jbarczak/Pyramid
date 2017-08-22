using System;

namespace Pyramid
{
    namespace SPIRV
    {
        public interface IProgram
        {
            string Disassemble();
            uint[] GetTokens();
            byte[] GetBytes();
        }


    }
}