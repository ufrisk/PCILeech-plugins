PCILeech Command Module Plugins
===============================
This repository contains various plugins that extends upon the [PCILeech - Direct Memory Access Attack Toolkit](https://github.com/ufrisk/pcileech).

The repository currently contains two test command modules, helloworld_win and helloworld_linux. The built command module should be placed alongside the PCILeech binary upon completion.



## leechp_helloworld_win

#### Author:
- Ulf Frisk

#### Supported Platforms:
- Windows

#### Overview:
The helloworld_win example demonstrates a PCILeech command module that utilizes MemProcFS to retrieve memory from the ntoskrnl.exe PE header of a targeted Windows system.

#### Installation instructions:
Place leechp_helloworld_win.dll alongside pcileech.exe.



## leechp_helloworld_linux

#### Author:
- Ulf Frisk

#### Supported Platforms:
- Linux

#### Overview:
The helloworld_linux example demonstrates a PCILeech command module that reads and displays memory from physical address 0x1000 of the target system.

#### Installation instructions:
Place leechp_helloworld_linux.so alongside pcileech.


