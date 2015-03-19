

namespace Pyramid
{
    public class HLSLLanguage : ILanguage
    {
        private HLSLOptionsPanel m_Options = new HLSLOptionsPanel();
        public ICompileOptionsPanel OptionsPanel { get { return m_Options; } }
        public string Name { get { return "HLSL"; } }
    }

   

}