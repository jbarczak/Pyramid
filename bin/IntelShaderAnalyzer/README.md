
## Intel Shader Analyzer
Intel Shader Analyzer is a tool for offline static analysis of shaders.  It allows a user to compile dxbc or HLSL code and inspect the generated code for either DX11 and DX12.   

Intel Shader Analyzer uses a dedicated driver API to compile and disassemble shaders, and may be run on any Windows 10 machine.  It depends on the following graphics driver components:
   * For 32-bit:
        * igc32.dll
        * iga32.dll
        * IntelGPUCompiler32.dll
    * For 64-bit:
        * igc64.dll
        * iga64.dll
        * IntelGPUCompiler64.dll


In the initial release, these driver components are bundled with the release executable.  Future driver releases will also allow Intel GPU users to run with in-situ drivers.

Besides being a functional tool, Intel Shader Analyzer is also intended to serve as a working refrence for the use of the driver compilation API, so that others may incorporate into their own tool chains as appropriate.

## Usage

Intel Shader Analyzer can consume HLSL input or DX bytecode, and pass them for compilation to either the DX11 or DX12 compilers.  Currently, HLSL is only supported for shader model 5 shaders.  

A simple HLSL command line is shown below:

    IntelShaderAnalyzer.exe -s hlsl -f MyVS -p vs_5_0 shader.hlsl

By default, DX11 will be used.  DX12 may be specified by using the `--api` command line switch.  For example:

    IntelShaderAnalyzer.exe -s hlsl --api dx12 -f MyVS -p vs_5_0 shader.hlsl

The `dxbc` source language may be used to feed dx bytecode directly to the tool.  For example:

    IntelShaderAnalyzer.exe -s dxbc --api dx11 shader.bin

 The `dxbc` option also supports passing DXIL shaders to the DX12 backend.  For example:
 
    IntelShaderAnalyzer.exe -s dxbc --api dx12 --rootsig_file rootsig.bin shader.bin

### Root Signatures

DX12 compilation requires a root signature, so that the compiler may determine how samplers, descriptors, and constants are to be provided to the shader.  Different root signatures will result in slightly different generation.   Intel Shader Analyzer can obtain a root signature in several differnet ways.  If more than one of them is attempted, they are applied in the order described here.

#### HLSL Attributes

The first option is to use HLSL attributes to embed a root signature in the resulting shader blob, as shown in the example below.  If a root signature is found embedded in the compiled shader, it will always be used first.  
    
```
#define MyRS1 "DescriptorTable(SRV(t0))," \
              "StaticSampler(s0, addressU = TEXTURE_ADDRESS_CLAMP, " \
                                "filter = FILTER_MIN_MAG_MIP_LINEAR )"
                                 
Texture2D<float4> tx : register(t0);
sampler SS : register(s0);

[RootSignature(MyRS1)]
float4 main( float4 uv : uv ) : SV_Target
{
   return tx.Sample( SS, uv );
}


```

#### Two-Pass Compilation

For HLSL shaders, a second option is to use the `--rootsig_macro` command line switch to compile a root signature out of the same source.   Here is the same shader, without the `rootsignature` attribute:

```
#define MyRS1 "DescriptorTable(SRV(t0))," \
              "StaticSampler(s0, addressU = TEXTURE_ADDRESS_CLAMP, " \
                                "filter = FILTER_MIN_MAG_MIP_LINEAR )"
                                 
Texture2D<float4> tx : register(t0);
sampler SS : register(s0);

float4 main( float4 uv : uv ) : SV_Target
{
   return tx.Sample( SS, uv );
}
```

If the following command line is used, Intel Shader Analyzer will attempt to compile the input file a second time to obtain the root signature:

    IntelShaderAnalyzer.exe -s HLSL --rootsig_macro MyRS1 --api dx12 --profile ps_5_0 filename.hlsl


#### Precompiled Root Signatures

The final option is to supply a pre-compiled, serialized root signature in a separate file, using the following command line:

    IntelShaderAnalyzer.exe -s HLSL --rootsig_file rootsig.bin --api dx12 --profile ps_5_0 filename.hlsl

This option may also be used with dxbc input:

    IntelShaderAnalyzer.exe -s dxbc --rootsig_file rootsig.bin --api dx12 filename.dxbc

## Command Line


### General-Purpose Options

    -l
    --list-asics

Print a list of supported device families.


    -c <device_name>
    --asic <device_name>

Add the specified device to the list of compile targets.   The device name must be one of the ones returned by --list-asics.  By default, all supported asics are compiled.

    --api dx11
    --api dx12

Set the target API.  Default is 'dx11'.  Code generation may differ between the dx11 and dx12 drivers.  Supported APIs are 'dx11' and 'dx12'  Compilation for DX12 requires a root signature.  

    --isa <path_prefix>

Set the directory name for output ISA files.   For each target device the compiler will emit a file named <path_prefix><device_name>.asm

For example:  
    --isa ./output   --asic Skylake   
        
will produce a file named:  ./outputSkylake.asm

The default is "./isa_".

    -s [hlsl | dxbc]

Set the source language.  Valid values are `hlsl` or `dxbc`.   The `dxbc`source language may be used for both legacy DX11 bytecode and DXIL.  Default is `dxbc`

    --rootsig_file <path>

Load a serialized DX root signature from the specified path.


### HLSL Options

    --rootsig_profile <profile>

Set the compilation profile for root signature compilation.  Default is:  `rootsig_1_0`.

    --rootsig_macro <name>

Sets the macro name for root signature compilation.  If this option is specified, and no root signature is provided.  The tool will attempt to re-compile the input HLSL source to extract the root signature.
     
    --DXLocation

Set the location of the D3D compiler DLL.  The default is `d3dcompiler_47.dll`

    --DXFlags <number>

Set the flags for the HLSL compiler.  Argument is a bitwise combination of D3DCOMPILE_ flags.  See the [MSDN documentation](https://docs.microsoft.com/en-us/windows/desktop/direct3dhlsl/d3dcompile-constants)

    -D <SYMBOL>=<VALUE>
    -D <SYMBOL>

Add a preprocessor define

    --profile
    -p

Set the shader profile for HLSL compilation.  Required.

    --function
    -f

Set the entrypoint for HLSL compilation.  Optional.  Default is `main`.


## Running Tests

The tests use a very simple-minded python script.  

To run the tests, cd to the 'tests' directory, copy the executable into it (and the driver DLLs, as required).  Then run `python run_tests.py`
The script will automatically check exit codes, but it is necessary to manually inspect the output to ensure that the tool is behaving as expected.

The tests need to be run from a bash shell, or a windows shell with GNU 'rm' utilities in the path.
