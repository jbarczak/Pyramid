
Pyramid is distributed as is. There is no warranty of any kind. I will not guarantee timely support, but I will be happy to accept bug fixes, patches, or contributions.
You may send constructive feedback to:  jbarcz1@gmail.com

Pyramid is intended to eventually support arbitrary languages and glue together disassemblers from all of the GPU vendors.  

IHVs:  You want developers scrutinizing the ISA for your GPUs.  You know you do :) I will work with you to incorporate your disassemblers into this tool.  I am willing to jump through hoops to make this happen, just tell me what I need to do.

Configuration
--------------
Pyramid is built against .NET framework 4.5.  If you don't have it, you need to go get it.  Otherwise, just put the contents of 'bin' wherever you like, there is no installer.

Pyramid will create a configuration file under AppData\Local\Pyramid.  At runtime, it will also create and delete temporary files in this location.  Apart from that, there are no other changes made to the system.

For convenience, pyramid includes the following third party binaries:
 - D3D compiler
 - The AMD CodeXL Analyzer (version 1.5)
 - The PowerVR rogue compilers (from SDK version 3.4)
 - An AMD DXX driver (Catalyst 14.9)

These are all publicly available binaries which are provided with pyramid as a convenience, but I do not own them, maintain them, and am not responsible for them.  If you like you can use the options screen to point pyramid at different versions of the external tools.

Pyramid also includes a managed wrapper that contains interfaces to GLSLOptimizer and the GLSL reference compiler.

Building it Yourself
---------------------
Pyramid was developed using VC++ 2013 express edition (Desktop).  It runs correctly on my machine (Windows 8.1 with VC++ express installed).  The build products go to 'src/bin' instead of 'bin', so if you are debugging, you probably want to use the options screen to redirect the tool paths.
