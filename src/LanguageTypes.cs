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

    public interface IShader
    {
        Languages Language { get; }
        string Code { get; }
    };
}
