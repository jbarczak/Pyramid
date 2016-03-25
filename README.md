
Pyramid is a free, open GUI tool for offline shader validation and analysis.

Pyramid is intended to eventually support arbitrary languages and glue together tools and disassemblers from all over the graphics world.
 
IHVs:  You want developers scrutinizing the ISA for your GPUs.  You know you do :) I will work with you to incorporate your disassemblers into this tool.  I am willing to jump through hoops to make this happen, just tell me what I need to do.


Obtaining Pyramid
------------------

The best way to obtain a copy is to ask github to download a zip of the repository.  The 'bin' directory will be updated periodically with an up to date binary.  

Pyramid is built against .NET framework 4.5.  If you don't have it, you need to go get it.  Otherwise, just put the contents of 'bin' wherever you like, there is no installer.

Pyramid will create a configuration file under AppData\Local\Pyramid.  At runtime, it will also create and delete temporary files in this location.  Apart from that, there are no other changes made to the system.  In particular, there is no registry nonsense.

For convenience, pyramid includes the following third party binaries:
 - D3D compiler
 - The AMD CodeXL Analyzer (version 1.5)
 - The PowerVR rogue compilers (from SDK version 3.4)
 - An AMD DXX driver (Catalyst 14.9)
 - The ARM Mali Offline shader compiler (version 4.6.0)

These are all publicly available binaries which are provided with pyramid as a convenience, but I do not own them, maintain them, and am not responsible for them.  If you like you can use the options screen to point pyramid at different versions of the external tools.

Pyramid also includes a managed wrapper that contains interfaces to GLSLOptimizer, the GLSL reference compiler, the D3D compiler, and an AMD GCN disassembler.


Pyramid is distributed as is. There is no warranty of any kind. I will not guarantee timely support, but I will be happy to accept bug fixes, patches, or contributions.

You may send constructive feedback to:  jbarcz1@gmail.com



Building it Yourself
---------------------

Due to my use of submodules, downloading a zip will not get you the full source.

The best way to obtain the full source is to install git, open a shell, and type:  
    git clone --recurse https://github.com/jbarczak/Pyramid

This will ensure that all of the submodules are downloaded.  github does not include submodules when you download a zip.

I use and recommend MSysGit:  https://msysgit.github.io/

Pyramid was developed using VC++ 2013 express edition (Desktop).  It runs correctly on my machine (Windows 8.1 with VC++ express installed).  

The build products will be copied from 'src/bin' into 'bin' on each compile.  

By default, pyramid uses relative paths to look for all of the external binaries.  For debugging, it's best to point the working directory at 'bin' so that they will be found.  Alternatively, you can use the options menu to replace the paths with absolutes.
