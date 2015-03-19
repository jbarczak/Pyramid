
namespace Pyramid
{

    public interface ILanguage
    {
        ICompileOptionsPanel OptionsPanel { get; }
        string Name { get; }
    };
}
