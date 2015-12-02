
#define _CRT_SECURE_NO_DEPRECATE

#include <Windows.h>
#include <stdio.h>
#include <stdarg.h>
#include <malloc.h>
#include "elf.h"
#include <time.h>
#include "d3d11tokenizedprogramformat.hpp"

struct esi_struct;
struct edi_struct;

#define CALL __cdecl
typedef DWORD (CALL *COMPILE_SHADER)( esi_struct*, edi_struct* );
typedef void  (CALL *FREE_SHADER)   (void*);

static FILE*            g_pLog = 0;
static COMPILE_SHADER   g_pCompileShader = 0;
static FREE_SHADER      g_pFreeShader = 0;
static HMODULE          g_hAMDDriver = 0;
static CRITICAL_SECTION g_Lock;

class ScopedLock
{
public:
    ScopedLock() { EnterCriticalSection(&g_Lock); }
    ~ScopedLock() { LeaveCriticalSection(&g_Lock); }
};


void Log( const char* Format, ... )
{
    va_list vl;
    va_start(vl,Format);

    int n = _vscprintf( Format, vl );
    char* p = (char*) malloc( n+1 );
    vsprintf(p,Format,vl);
    printf(p);
    OutputDebugStringA(p);
    if( g_pLog )
        fprintf( g_pLog, p );

    free(p);

    va_end(vl);
}


enum
{
    INVALID_ARG = 0x80070057,
    UNSPECIFIED_ERROR = 0x80004005,
};


struct esi_struct
{ 
    DWORD        d0; // esi    Two dwords here that get passed into a call (pointer and size)?  asic info?
    DWORD        d1; // esi+4
    DWORD*       pShaderBlob;               // esi+8  If this is zero, return error ... pointer to D3D bytecode?
    DWORD        nShaderBlobLengthInBytes; // esi+10h.  If this is non-zero, go to (53581b4e) (jumps over the esi+14h test)
    void *  oddball;                        // esi+14h.  If this is non-zero, error, but only if esi+10 is also 
    DWORD mystery0;

    void Print( )
    {
        Log( "esi_struct at: 0x%08x { 0x%08x 0x%08x 0x%08x 0x%08x 0x%08x 0x%08x }\n", 
            this, d0,d1,pShaderBlob,nShaderBlobLengthInBytes,
            oddball, mystery0 );
    }

};  


struct edi_struct
{
    DWORD size; // 12
    Elf32_Ehdr* pHeader;
    DWORD nDataSize;

    void Print( )
    {
        Log("edi_struct at (0x%08x): { 0x%08x 0x%08x  0x%08x }\n", this, size, pHeader,nDataSize );
    }

    void PrintSections( )
    {
        char* pHeaderBytes = (char*)pHeader;
        Elf32_Shdr* pSections = (Elf32_Shdr*) (pHeaderBytes + pHeader->e_shoff);

        Elf32_Shdr* pStringTable = pSections + pHeader->e_shstrndx;
        char* pElfStringTable = pHeaderBytes + pStringTable->sh_offset;
        for( DWORD i=0; i<pHeader->e_shnum; i++ )
        {
            DWORD nString = pSections[i].sh_name;
            char* p = pElfStringTable + pSections[i].sh_name;
            char* pSectionData = pHeaderBytes + pSections[i].sh_offset;
            
            Log("ELF Section: %u named: %s   (%u bytes)\n", i, p, pSections[i].sh_size );
        }

    }

    void PrintGoryDetails( )
    {
        char* pHeaderBytes = (char*)pHeader;
        Elf32_Shdr* pSections = (Elf32_Shdr*) (pHeaderBytes + pHeader->e_shoff);

        Elf32_Shdr* pStringTable = pSections + pHeader->e_shstrndx;
        char* pElfStringTable = pHeaderBytes + pStringTable->sh_offset;
        for( DWORD i=0; i<pHeader->e_shnum; i++ )
        {
            DWORD nString = pSections[i].sh_name;
            char* p = pElfStringTable + pSections[i].sh_name;
            char* pSectionData = pHeaderBytes + pSections[i].sh_offset;
            
            Log("ELF Section: %u named: %s   (%u bytes)\n", i, p, pSections[i].sh_size );
            Log("********************\n");
            if( pSections[i].sh_size % 4 == 0 )
            {
                for( DWORD k=0; k<pSections[i].sh_size; k += 4 )
                    Log( "%08x\n", *((DWORD*)(pSectionData+k)) );
            }

            Log("ASCII: \"");
            for( DWORD k=0; k<pSections[i].sh_size; k++ )
                Log( "%c", pSectionData[k] );
                
            Log("\"\n");
        }

    }

}; 




extern "C" {

static HRESULT Test(  esi_struct* esi, edi_struct* edi )
{
    const size_t PAD_SIZE   = 2048;
    unsigned char* p = (unsigned char*) malloc( PAD_SIZE );
    for( size_t i=0; i<PAD_SIZE; i++ )
        p[i] = rand();

    size_t offs = rand() % (PAD_SIZE-(sizeof(edi_struct)+sizeof(esi_struct)));
    edi_struct* new_edi = (edi_struct*) (p+offs);
    esi_struct* new_esi = (esi_struct*) (new_edi+1);
    *new_edi = *edi;
    *new_esi = *esi;

    HRESULT h = g_pCompileShader(new_esi,new_edi);
    if( !SUCCEEDED(h) )
        Log(" FAIL\n");

    *edi = *new_edi;
    free(p);
    return h;
}

__declspec(dllexport) DWORD CALL AmdDxGsaCompileShader( esi_struct* esi, edi_struct* edi )
{
    //edi_struct first_one = *edi;
    //g_pCompileShader(esi,&first_one);

    //for( int i=0; i<10000; i++ )
    //{
    //    edi_struct current = *edi;
    //    Test(esi,&current);
    //    if( current.nDataSize != first_one.nDataSize || memcmp(current.pHeader,first_one.pHeader,current.nDataSize) != 0 )
    //        Log("Results differ\n");
    //    g_pFreeShader( current.pHeader);
    //}

    //*edi = first_one;
    //return S_OK;

    HRESULT h;
    Log("Asic numbers: %u,%u\n", esi->d0, esi->d1);
    h = g_pCompileShader(esi,edi);

    return h;
}

__declspec(dllexport) void CALL AmdDxGsaFreeCompiledShader( void* p )
{
    ScopedLock l;

    if( g_pFreeShader )
        g_pFreeShader(p);
}

}


BOOL WINAPI DllMain(
  _In_  HINSTANCE hinstDLL,
  _In_  DWORD fdwReason,
  _In_  LPVOID lpvReserved
)
{
    switch( fdwReason )
    {
    case DLL_PROCESS_ATTACH:
        {
            InitializeCriticalSection(&g_Lock);
            
            g_pLog = fopen("ShimLog.txt", "a" );
            if( !g_pLog )
                Log("Failed to open log\n");

            HMODULE hAMDDriver = LoadLibrary( "real_driver.dll" );
            if( !hAMDDriver )
                return UNSPECIFIED_ERROR;

            FARPROC pCompile = GetProcAddress( hAMDDriver, "AmdDxGsaCompileShader" );
            FARPROC pFree    = GetProcAddress( hAMDDriver, "AmdDxGsaFreeCompiledShader" );
            if( !pCompile || !pFree )
            {
                FreeLibrary(hAMDDriver);
                return FALSE;
            }

            g_pCompileShader = (COMPILE_SHADER) pCompile;
            g_pFreeShader = (FREE_SHADER) pFree;
            Log("**************************************************************************\n");
            Log("Pyramid driver shim has loaded\n");
            Log("Command line: %s\n", GetCommandLineA() );
        }
        break;

    case DLL_PROCESS_DETACH:
       
        DeleteCriticalSection( &g_Lock );
        if( g_hAMDDriver )
        {
            FreeLibrary(g_hAMDDriver);
            g_pFreeShader=0;
            g_pCompileShader=0;
            g_hAMDDriver=0;
            Log("Pyramid driver shim has unloaded\n");
        }
        if( g_pLog )
        {
            fclose(g_pLog);
            g_pLog=0;
        }

        break;
    }

    return TRUE;
}


