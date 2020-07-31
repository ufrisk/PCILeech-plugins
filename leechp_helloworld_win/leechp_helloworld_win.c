// leechp_helloworld_win.c : implementation of a "Hello World" example.
// A PCILeech Command Module Plugin which uses MemProcFS to display the PE
// header page of ntoskrnl.exe of the analyzed target system.
//
// (c) Ulf Frisk, 2020
// Author: Ulf Frisk, pcileech@frizk.net
//
#include <pcileech.h>
#include <vmmdll.h>
#include <stdio.h>

_Success_(return) __declspec(dllexport)
VOID DoAction(_Inout_ PPCILEECH_CONTEXT ctx)
{
    BYTE pbPageNtoskrnlHead[0x1000];
    VMMDLL_MAP_MODULEENTRY ModuleNtoskrnl;
    DWORD cszText = 0;
    LPSTR szText = NULL;
    // 1: Ensure version of context matches header.
    if((ctx->magic != PCILEECH_CONTEXT_MAGIC) || (ctx->version != PCILEECH_CONTEXT_VERSION)) {
        printf("Failed. Version mismatch betwen header and context.\n");
        goto fail;
    }
    // 2: Initialize MemProcFS (vmm.dll) against the existing PCILeech device.
    // Auto-updates are not required (-norefresh).
    // PDB Debugging Symbols from the Microsoft Symbol Server is not required
    // for this example and are disabled (-symbolserverdisable).
    if(!VMMDLL_Initialize(3, (LPSTR[]){ "", "-device", "existing", "-norefresh", "-symbolserverdisable" })) {
        printf("Failed to initialize vmm.dll.\n");
        goto fail;
    }
    // 3: Retrieve module information about the 'ntoskrnl.exe' module in the
    // process with PID = 4 (System).
    if(!VMMDLL_ProcessMap_GetModuleFromName(4, L"ntoskrnl.exe", &ModuleNtoskrnl)) {
        printf("Failed to locate base ntoskrnl.exe.\n");
        goto fail;
    }
    // 4: Read the PE header (base) of the 'ntoskrnl.exe' module.
    if(!VMMDLL_MemRead(4, ModuleNtoskrnl.vaBase, pbPageNtoskrnlHead, 0x1000)) {
        printf("Failed to read ntoskrnl.exe PE header.\n");
        goto fail;
    }
    // 5: Calculate the length of the buffer needed for the hexascii text.
    if(!VMMDLL_UtilFillHexAscii(pbPageNtoskrnlHead, 0x1000, 0, NULL, &cszText)) {
        printf("Failed to query size of display text.\n");
        goto fail;
    }
    // 6: Allocate a buffer to hold the hexascii text.
    if(!(szText = LocalAlloc(LMEM_ZEROINIT, cszText))) {
        printf("Failed to allocate memory for display text.\n");
        goto fail;
    }
    // 7: Set the hexascii text representation of the ntoskrnl.exe PE header.
    if(!VMMDLL_UtilFillHexAscii(pbPageNtoskrnlHead, 0x1000, 0, szText, &cszText)) {
        printf("Failed to get display text.\n");
        goto fail;
    }
    // 8: Display the successful result.
    printf("=== NTOSKRNL.EXE PE HEADER (BASE) PAGE DISPLAYED BELOW ===\n");
    printf("%s", szText);
    printf("\n");
    // fall-through to cleanup
fail:
    LocalFree(szText);
    if(!VMMDLL_Close()) {
        printf("Failed to close vmm.dll.\n");
    }
}
