// leechp_helloworld_linux.c : implementation of a "Hello World" example.
// This example reads a page from address specified by -min parameter to
// PCILeech and displays it. If no -min parameter have been specified
// physical address 0x1000 is shown.
//
// (c) Ulf Frisk, 2020
// Author: Ulf Frisk, pcileech@frizk.net
//
#include <leechcore.h>
#include <pcileech.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ----------------------------------------------------------------------------
// Utilities functions below for printing binary as hexascii dump.
// ----------------------------------------------------------------------------

#define TRUE            1
#define FALSE           0
#define Util_2HexChar(x) (((((x) & 0xf) <= 9) ? '0' : ('a' - 10)) + ((x) & 0xf))

#define UTIL_PRINTASCII \
    "................................ !\"#$%&'()*+,-./0123456789:;<=>?" \
    "@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~ " \
    "................................................................" \
    "................................................................" \

BOOL Util_FillHexAscii(_In_opt_ PBYTE pb, _In_ DWORD cb, _In_ DWORD cbInitialOffset, _Out_opt_ LPSTR sz, _Inout_ PDWORD pcsz)
{
    DWORD i, j, o = 0, iMod, cRows;
    // checks
    if((cbInitialOffset > cb) || (cbInitialOffset > 0x1000) || (cbInitialOffset & 0xf)) { return FALSE; }
    cRows = (cb + 0xf) >> 4;
    if(!sz) {
        *pcsz = 1 + cRows * 76;
        return TRUE;
    }
    if(!pb || (*pcsz <= cRows * 76)) { return FALSE; }
    // fill buffer with bytes
    for(i = cbInitialOffset; i < cb + ((cb % 16) ? (16 - cb % 16) : 0); i++)
    {
        // address
        if(0 == i % 16) {
            iMod = i % 0x10000;
            sz[o++] = Util_2HexChar(iMod >> 12);
            sz[o++] = Util_2HexChar(iMod >> 8);
            sz[o++] = Util_2HexChar(iMod >> 4);
            sz[o++] = Util_2HexChar(iMod);
            sz[o++] = ' ';
            sz[o++] = ' ';
            sz[o++] = ' ';
            sz[o++] = ' ';
        } else if(0 == i % 8) {
            sz[o++] = ' ';
        }
        // hex
        if(i < cb) {
            sz[o++] = Util_2HexChar(pb[i] >> 4);
            sz[o++] = Util_2HexChar(pb[i]);
            sz[o++] = ' ';
        } else {
            sz[o++] = ' ';
            sz[o++] = ' ';
            sz[o++] = ' ';
        }
        // ascii
        if(15 == i % 16) {
            sz[o++] = ' ';
            sz[o++] = ' ';
            for(j = i - 15; j <= i; j++) {
                if(j >= cb) {
                    sz[o++] = ' ';
                } else {
                    sz[o++] = UTIL_PRINTASCII[pb[j]];
                }
            }
            sz[o++] = '\n';
        }
    }
    sz[o] = 0;
    *pcsz = o;
    return TRUE;
}

VOID Util_PrintHexAscii(_In_ PBYTE pb, _In_ DWORD cb, _In_ DWORD cbInitialOffset)
{
    DWORD szMax;
    LPSTR sz;
    if(cb > 0x10000) {
        printf("Large output. Only displaying first 65kB.\n");
        cb = 0x10000 - cbInitialOffset;
    }
    Util_FillHexAscii(pb, cb, cbInitialOffset, NULL, &szMax);
    if(!(sz = malloc(szMax))) { return; }
    Util_FillHexAscii(pb, cb, cbInitialOffset, sz, &szMax);
    printf("%s", sz);
    free(sz);
}

// ----------------------------------------------------------------------------
// PCILeech entrypoint "DoAction" function below:
// ----------------------------------------------------------------------------

__attribute__((visibility("default")))
void DoAction(PPCILEECH_CONTEXT ctx)
{
    QWORD pa;
    char pb[0x1000];
    int cb;
    // 1: Ensure version of context matches header.
    if((ctx->magic != PCILEECH_CONTEXT_MAGIC) || (ctx->version != PCILEECH_CONTEXT_VERSION)) {
        printf("Failed. Version mismatch betwen header and context.\n");
        return;
    }
    // 2: Set the physical address to read.
    pa = ctx->cfg.qwAddrMin;
    if(pa == 0) {
        pa = 0x1000;
    }
    // 3: Read 0x1000 bytes from the address.
    memset(pb, 0, 0x1000);
    cb = LeechCore_Read(ctx->hLeechCore, pa, pb, 0x1000);
    if(cb != 0x1000) {
        printf("Failed reading 0x1000 bytes from address: 0x%016llx\n", pa);
        return;
    }
    // 4: Print the result on the screen:
    printf("=== RESULT OF ADDRESS 0x%016llx IS SHOWN BELOW ===\n", pa);
    Util_PrintHexAscii(pb, 0x1000, 0);
    printf("\n");
}