
#ifndef _DXIL_COMPILER_IMPL_H_
#define _DXIL_COMPILER_IMPL_H_

#pragma unmanaged
#include <Windows.h>
#include "dxc/dxcapi.h"
struct IDxcLibrary;
struct IDxcCompiler;
struct IDxcBlob;
class GenIntrinsicTable;
#pragma managed

typedef unsigned char byte;

ref class DXILCompiler_Impl;

private ref class DXILRootSignatureBlob_Impl : Pyramid::IDXBlob
{
public:
    DXILRootSignatureBlob_Impl( IDxcBlob* pBlob,DXILCompiler_Impl^ pCompiler );
    ~DXILRootSignatureBlob_Impl();

    virtual array<byte>^ ReadBytes();


private:
    IDxcBlob* m_pBlob;
    DXILCompiler_Impl^ m_pmCompiler;
};

private ref class DXILShaderBlob_Impl : Pyramid::IDXILShaderBlob
{
public:
    DXILShaderBlob_Impl( IDxcBlob* pBlob,DXILCompiler_Impl^ pCompiler );
    ~DXILShaderBlob_Impl();

    virtual Pyramid::IDXBlob^ ExtractRootSignature();
    virtual System::String^ Disassemble();
    virtual array<byte>^ ReadBytes();

private:
    IDxcBlob* m_pBlob;
    DXILCompiler_Impl^ m_pmCompiler;
};


private ref class DXILCompiler_Impl : Pyramid::IDXILCompiler
{
public:

    DXILCompiler_Impl( System::String^ DLLPath,Pyramid::IIncludeHandler^ pmIncludes );
    ~DXILCompiler_Impl();

    virtual bool CompileRootSignature( System::String^ Shader,
        Pyramid::IHLSLOptions^ opts,
        System::String^ Path,
        Pyramid::IDXBlob^% blob,
        System::String^% Errors );

    virtual bool Compile( System::String^ Shader,
        Pyramid::IHLSLOptions^ opts,
        System::String^ Path,
        Pyramid::IDXILShaderBlob^% blob,
        System::String^% Errors );

internal:

    Pyramid::IIncludeHandler^ m_pmInclude;
    IDxcLibrary* m_pDXCLibrary = nullptr;
    IDxcCompiler* m_pDXCCompiler = nullptr;
    GenIntrinsicTable* m_pGenIntrinsics = nullptr;
    DxcCreateInstanceProc m_pfnCreateInstance = nullptr;
};



#endif