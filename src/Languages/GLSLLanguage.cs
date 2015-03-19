
namespace Pyramid
{
   


    public class GLSLLanguage : ILanguage
    {
        private GLSLOptionsPanel m_Options = new GLSLOptionsPanel();
        public ICompileOptionsPanel OptionsPanel { get { return m_Options; } }
        public string Name {  get {return "GLSL";} }
    }


}