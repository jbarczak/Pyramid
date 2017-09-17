using System;

namespace Pyramid
{
    public interface IIncludeResult
    {
        byte[] Contents { get; }
        string FullPath { get; }
    }

    public enum IncludeType
    {
        System,
        User,
    }

    public interface IIncludeHandler
    {
        IIncludeResult OpenInclude(IncludeType eType, string targetPath, string includerPath );
    }
}
