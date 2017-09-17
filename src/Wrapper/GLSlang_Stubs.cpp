
// GLSLang in library mode needs this crap defined or it won't link.... Lame...

#pragma unmanaged

#include "GLSlang/glslang/Include/Common.h"
#include "GLSlang/glslang/Include/ShHandle.h"
#include "GLSlang/glslang/MachineIndependent/Versions.h"

#if 0
//
// Here is where real machine specific high-level data would be defined.
//
class TGenericCompiler : public TCompiler {
public:
    TGenericCompiler(EShLanguage l, int dOptions) : TCompiler(l, infoSink), debugOptions(dOptions) { }
    virtual bool compile(TIntermNode* root, int version = 0, EProfile profile = ENoProfile);
    TInfoSink infoSink;
    int debugOptions;
};

//
// This function must be provided to create the actual
// compile object used by higher level code.  It returns
// a subclass of TCompiler.
//
TCompiler* ConstructCompiler(EShLanguage language, int debugOptions)
{
    return new TGenericCompiler(language, debugOptions);
}

//
// Delete the compiler made by ConstructCompiler
//
void DeleteCompiler(TCompiler* compiler)
{
    delete compiler;
}

//
//  Generate code from the given parse tree
//
bool TGenericCompiler::compile(TIntermNode *root, int version, EProfile profile)
{
    haveValidObjectCode = true;

    return haveValidObjectCode;
}


//
// Actual link object, derived from the shader handle base classes.
//
class TGenericLinker : public TLinker {
public:
    TGenericLinker(EShExecutable e, int dOptions) : TLinker(e, infoSink), debugOptions(dOptions) { }
    bool link(TCompilerList&, TUniformMap*) { return true; }
	void getAttributeBindings(ShBindingTable const **t) const { }
    TInfoSink infoSink;
    int debugOptions;
};

//
// The internal view of a uniform/float object exchanged with the driver.
//
class TUniformLinkedMap : public TUniformMap {
public:
    TUniformLinkedMap() { }
    virtual int getLocation(const char* name) { return 0; }
};

TShHandleBase* ConstructLinker(EShExecutable executable, int debugOptions)
{
    return new TGenericLinker(executable, debugOptions);
}

void DeleteLinker(TShHandleBase* linker)
{
    delete linker;
}

TUniformMap* ConstructUniformMap()
{
    return new TUniformLinkedMap();
}

void DeleteUniformMap(TUniformMap* map)
{
    delete map;
}

TShHandleBase* ConstructBindings()
{
    return 0;
}

void DeleteBindingList(TShHandleBase* bindingList)
{
    delete bindingList;
}
#endif