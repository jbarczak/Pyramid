

namespace Pyramid
{
    public enum Languages
    {
        HLSL,
        GLSL,
    };

    public interface ICompileOptions
    {
        Languages Language { get; }
    }
}
