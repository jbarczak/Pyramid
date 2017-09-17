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

        /// <summary>
        ///  Full path to source file, if we have one. 
        ///  May be empty if text was typed into editor and not saved
        /// </summary>
        string SourceFilePath { get; }
    };
}
