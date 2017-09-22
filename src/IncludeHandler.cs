using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;

namespace Pyramid
{
    class IncludeResult : IIncludeResult
    {
        public IncludeResult( string path )
        {
            FullPath = path;
            Contents = File.ReadAllBytes(path);
        }
        public byte[] Contents { get; private set; }
        public string FullPath { get; private set; }
    }

    class IncludeHandler : IIncludeHandler
    {
        private List<string> m_SearchDirs = new List<string>();

        public IncludeHandler( IEnumerable<string> IncludePaths )
        {
            if (IncludePaths != null)
                m_SearchDirs.AddRange(IncludePaths);
        }

        IIncludeResult DoPath(string searchPath, string includePath )
        {
            string relPath = Path.GetFullPath(Path.Combine(searchPath, includePath));
            if( File.Exists(relPath))
            {
                return new IncludeResult(relPath);
            }
            return null;
        }

        public IIncludeResult OpenInclude(IncludeType eType, string path, string includerPath)
        {
            if (!String.IsNullOrEmpty(includerPath))
            {
                string fileDir = Path.GetDirectoryName(includerPath);
                IIncludeResult r = DoPath(fileDir, path);
                if (r != null)
                    return r;
            }

            foreach( string s in m_SearchDirs )
            {
                if( Directory.Exists(s))
                {
                    IIncludeResult r = DoPath(s, path);
                    if (r != null)
                        return r;
                }
            }

            return null;
        }
    }
}
