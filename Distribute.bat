echo off
mkdir Pyramid
mkdir Pyramid\bin
mkdir Pyramid\src
mkdir Pyramid\src\Backends
mkdir Pyramid\src\Languages
mkdir Pyramid\src\MainUI
mkdir Pyramid\src\Properties
mkdir Pyramid\src\Wrapper
mkdir Pyramid\src\Wrapper\glsl-optimizer
mkdir Pyramid\src\Wrapper\GLSlang
cp -r bin/* Pyramid/bin
cp -r LICENSE.txt Pyramid/LICENSE.txt
cp -r README.txt Pyramid/README.txt
cp src/*.cs   Pyramid/src
cp src/*.resx Pyramid/src
cp src/Backends/*.cs   Pyramid/src/Backends
cp src/Backends/*.resx   Pyramid/src/Backends
cp src/Languages/*.cs   Pyramid/src/Languages
cp src/Languages/*.resx   Pyramid/src/Languages
cp src/MainUI/*.cs Pyramid/src/MainUI
cp src/MainUI/*.resx Pyramid/src/MainUI
cp src/App.config Pyramid/src/App.config
cp src/*.csproj Pyramid/src
cp src/Pyramid.sln Pyramid/src/Pyramid.sln
cp -r src/Properties/* Pyramid/src/Properties
cp src/Wrapper/*.cpp Pyramid/src/Wrapper
cp src/Wrapper/*.h  Pyramid/src/Wrapper
cp src/Wrapper/Wrapper.vcxproj Pyramid/src/Wrapper
cp src/Wrapper/Wrapper.vcxproj.filters Pyramid/src/Wrapper
cp -r src/Wrapper/glsl-optimizer Pyramid/src/Wrapper
cp src/Wrapper/GLSlang/glslang_vs2013.vcxproj Pyramid/src/Wrapper/GLSlang/glslang_vs2013.vcxproj
cp -r src/Wrapper/GLSlang/glslang          Pyramid/src/Wrapper/GLSlang
cp -r src/Wrapper/GLSlang/OGLCompilersDLL  Pyramid/src/Wrapper/GLSlang
cp -r src/Wrapper/GLSlang/tools            Pyramid/src/Wrapper/GLSlang

