
#ifndef _UTILITIES_H_
#define _UTILITIES_H_

#include <vcclr.h>

#pragma unmanaged
#include <string.h>
#include <malloc.h>
#pragma managed

typedef unsigned int UINT;

using namespace System::Runtime::InteropServices;

private ref class MarshalledString
{
public:
    MarshalledString( System::String^ str )
        : m_Ptr( Marshal::StringToHGlobalAnsi(str) )
    {
    }

    ~MarshalledString()
    {
        Marshal::FreeHGlobal(m_Ptr);
    }

    operator char* () { return GetString(); }

    char* GetString() { return (char*)m_Ptr.ToPointer(); }
    size_t Length()  { return strlen( GetString() ); }
private:
    System::IntPtr m_Ptr;
};

private ref class MarshalledBlob
{
public: 
    MarshalledBlob( array<unsigned char>^ blob ) : m_nLength(0), m_pBlob(0)
    {
        if( blob != nullptr && blob->Length > 0 )
        {
            void* pBlob = malloc(blob->Length);
            if( !pBlob )
                throw gcnew System::OutOfMemoryException();
            Marshal::Copy( blob, 0, System::IntPtr(pBlob), blob->Length);
            m_nLength = blob->Length;
            m_pBlob   = (unsigned char*) pBlob;            
        }
    }

    ~MarshalledBlob()
    {
        free(m_pBlob);
    }

    operator unsigned char*() { return GetBlob(); }
    unsigned char* GetBlob() { return m_pBlob; }
    size_t GetLength() { return m_nLength; }
private:
    size_t m_nLength;
    unsigned char* m_pBlob;
};

inline System::String^ MakeString( const char* p ) { return Marshal::PtrToStringAnsi(System::IntPtr((char*)p));}

#endif