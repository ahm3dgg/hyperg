#pragma once

#include "hyperg_types.hpp"
#include <intrin.h>

constexpr u64 MSR_IA32_FEATURE_CONTROL           = 0x03A;
constexpr u64 MSR_IA32_VMX_BASIC                 = 0x480;
constexpr u64 MSR_IA32_VMX_CR0_FIXED0            = 0x486;
constexpr u64 MSR_IA32_VMX_CR0_FIXED1            = 0x487;
constexpr u64 MSR_IA32_VMX_CR4_FIXED0            = 0x488;
constexpr u64 MSR_IA32_VMX_CR4_FIXED1            = 0x489;

typedef union _CR0
{
    u64 Value;

    struct
    {
        u64 PE : 1;   // 0  Protected mode enable
        u64 MP : 1;   // 1  Monitor coprocessor (controls WAIT/FWAIT with TS)
        u64 EM : 1;   // 2  Emulate FPU: x87 instructions raise #NM
        u64 TS : 1;   // 3  Task switched: set on task switch, used for lazy FPU save
        u64 ET : 1;   // 4  Extension type: always 1 on modern CPUs
        u64 NE : 1;   // 5  Numeric error: native x87 error reporting (#MF)
        u64 Reserved1 : 10;  // 6-15
        u64 WP : 1;   // 16 Write protect: ring 0 can't write to read-only pages
        u64 Reserved2 : 1;   // 17
        u64 AM : 1;   // 18 Alignment mask: enables alignment checking (with EFLAGS.AC)
        u64 Reserved3 : 10;  // 19-28
        u64 NW : 1;   // 29 Not write-through: write-through/write-back cache policy
        u64 CD : 1;   // 30 Cache disable: turns off the CPU caches
        u64 PG : 1;   // 31 Paging enable
        u64 Reserved4 : 32;  // 32-63
    };
} CR0, * PCR0;

typedef union _CR4
{
    u64 Value;

    struct
    {
        u64 VME : 1;  // 0  Virtual-8086 mode extensions
        u64 PVI : 1;  // 1  Protected-mode virtual interrupts
        u64 TSD : 1;  // 2  Time stamp disable: RDTSC only allowed in ring 0
        u64 DE : 1;  // 3  Debugging extensions: DR4/DR5 references raise #UD
        u64 PSE : 1;  // 4  Page size extensions: 4 MB pages in 32-bit paging
        u64 PAE : 1;  // 5  Physical address extension: required for 64-bit mode
        u64 MCE : 1;  // 6  Machine-check exception enable
        u64 PGE : 1;  // 7  Page global enable: global pages survive CR3 reloads
        u64 PCE : 1;  // 8  Performance counter enable: RDPMC allowed in user mode
        u64 OSFXSR : 1;  // 9  OS supports FXSAVE/FXRSTOR and SSE
        u64 OSXMMEXCPT : 1;  // 10 OS handles unmasked SSE exceptions (#XM)
        u64 UMIP : 1;  // 11 User-mode instruction prevention (SGDT, SIDT, etc.)
        u64 LA57 : 1;  // 12 5-level paging (57-bit linear addresses)
        u64 VMXE : 1;  // 13 VMX enable: MUST be 1 before VMXON
        u64 SMXE : 1;  // 14 Safer mode extensions enable
        u64 Reserved1 : 1;  // 15
        u64 FSGSBASE : 1;  // 16 Allows RDFSBASE/WRFSBASE/RDGSBASE/WRGSBASE in user mode
        u64 PCIDE : 1;  // 17 Process-context identifiers enable
        u64 OSXSAVE : 1;  // 18 OS supports XSAVE and extended processor state
        u64 KL : 1;  // 19 Key Locker enable
        u64 SMEP : 1;  // 20 Supervisor-mode execution prevention
        u64 SMAP : 1;  // 21 Supervisor-mode access prevention
        u64 PKE : 1;  // 22 Protection keys for user-mode pages
        u64 CET : 1;  // 23 Control-flow enforcement technology
        u64 PKS : 1;  // 24 Protection keys for supervisor-mode pages
        u64 Reserved2 : 39; // 25-63
    };
} CR4, * PCR4;

typedef union _IA32_FEATURE_CONTROL
{
    u64 Value;
    struct
    {
        u64 Lock : 1;   // 0     Lock: once set, the MSR is read-only until reset. BIOS sets it
        u64 VmxInsideSmx : 1;   // 1     Enable VMX inside SMX operation (Intel TXT / safer mode)
        u64 VmxOutsideSmx : 1;   // 2     Enable VMX outside SMX operation. This is the one you need
        u64 Reserved1 : 5;   // 3-7
        u64 SenterLocal : 7;   // 8-14  SENTER local function enables (TXT)
        u64 SenterGlobal : 1;   // 15    SENTER global enable (TXT)
        u64 Reserved2 : 1;   // 16
        u64 SgxLaunchCtl : 1;   // 17    SGX launch control enable
        u64 SgxGlobal : 1;   // 18    SGX global enable
        u64 Reserved3 : 1;   // 19
        u64 LmceEnable : 1;   // 20    Local machine-check exception (LMCE) enable
        u64 Reserved4 : 43;  // 21-63
    };
} IA32_FEATURE_CONTROL, * PIA32_FEATURE_CONTROL;

CR0 ReadCR0();
CR4 ReadCR4();
void WriteCR0(u64 Value);
void WriteCR4(u64 Value);

u8 VMXON(u64 Vmcs);
void VMXOFF();
void VMLAUNCH();

template <typename T>
T ReadMSR(u64 MSR);
void WriteMSR(u64 MSR, u64 Value);

template<typename T>
inline T ReadMSR(u64 MSR)
{
    return { __readmsr(MSR) };
}
