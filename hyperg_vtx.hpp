#pragma
#include "hyperg_types.hpp"

typedef struct _VM_HEADER
{
	union
	{
		u32 Value;

		struct
		{
			u32 RevisionId : 31;
			u32 ShadowVmcsIndicator : 1;
		} Bits;
	};
} VM_HEADER, * PVM_HEADER;

typedef struct _VMCS
{
	VM_HEADER Header;
	u32 AbortIndicator;
	u8 Data[0x1000 - sizeof(Header) - sizeof(AbortIndicator)];
}VMCS, * PVMCS;

typedef struct _VMXON_REGION
{
	VM_HEADER Header;
	u32 Reserved0;
	u8 Data[0x1000 - sizeof(Header)];
}VMXON_REGION, * PVMXON_REGION;

typedef union _IA32_VMX_BASIC
{
	u64 Value;
	struct
	{
		u64 RevisionId : 31;  // 0-30   VMCS revision ID. Write it into the first 4 bytes of the VMXON region and VMCS
		u64 MustBeZero : 1;   // 31     Always 0
		u64 RegionSize : 13;  // 32-44  Size in bytes of the VMXON region and VMCS (never more than 4096)
		u64 Reserved1 : 3;   // 45-47
		u64 PhysAddrWidth32 : 1;   // 48     0 = addresses limited to the CPU's physical address width, 1 = limited to 32 bits (always 0 on x64)
		u64 DualMonitorSmm : 1;   // 49     1 = dual-monitor treatment of SMIs and SMM is supported
		u64 MemoryType : 4;   // 50-53  Memory type the CPU uses to access the VMCS (0 = UC, 6 = WB)
		u64 InsOutsInfo : 1;   // 54     1 = INS/OUTS give extra info in the VM-exit information field
		u64 TrueControls : 1;   // 55     1 = the IA32_VMX_TRUE_*_CTLS MSRs exist (default-1 controls can be cleared)
		u64 HwExceptionNoErr : 1;   // 56     1 = VM entry can inject a hardware exception with or without an error code
		u64 Reserved2 : 7;   // 57-63
	};
} IA32_VMX_BASIC, * PIA32_VMX_BASIC;

typedef struct _VCPU
{
	PVMCS		  vVmcs;
	u64			  pVmcs;

	PVMXON_REGION vVmxonRegion;
	u64			  pVmxonRegion;
}VCPU, * PVCPU;

void EnterVmx();
void InitVMXON(PVCPU VCPU);
void InitVMCS(PVCPU VCPU);
void AdjustControlRegisters();