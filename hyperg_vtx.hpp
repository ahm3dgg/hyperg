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

typedef union _VMX_PRIMARY_CONTROLS
{
	u64 Value;

	struct
	{
		u64 Reserved0 : 2;
		u64 InterruptWindowExiting : 1;
		u64 UseTscOffsetting : 1;
		u64 Reserved1 : 3;
		u64 HltExiting : 1;
		u64 Reserved2 : 1;
		u64 InvlpgExiting : 1;
		u64 MwaitExiting : 1;
		u64 RdpmcExiting : 1;
		u64 RdtscExiting : 1;
		u64 Reserved3 : 2;
		u64 Cr3LoadExiting : 1;
		u64 Cr3StoreExiting : 1;
		u64 Reserved4 : 2;
		u64 Cr8LoadExiting : 1;
		u64 Cr8StoreExiting : 1;
		u64 UseTprShadow : 1;
		u64 NmiWindowExiting : 1;
		u64 MovDrExiting : 1;
		u64 UnconditionalIoExiting : 1;
		u64 UseIoBitmaps : 1;
		u64 Reserved5 : 1;
		u64 MonitorTrapFlag : 1;
		u64 UseMsrBitmaps : 1;
		u64 MonitorExiting : 1;
		u64 PauseExiting : 1;
		u64 ActivateSecondaryControls : 1;
	};
}VMX_PRIMARY_CONTROLS, * PVMX_PRIMARY_CONTROLS;

typedef union _VMX_SECONDARY_CONTROLS
{
	u64 Value;

	struct
	{
		u64 VirtualizeApicAccesses : 1;
		u64 EnableEpt : 1;
		u64 DescriptorTableExiting : 1;
		u64 EnableRdtscp : 1;
		u64 VirtualizeX2apic : 1;
		u64 EnableVpid : 1;
		u64 WbinvdExiting : 1;
		u64 UnrestrictedGuest : 1;
		u64 ApicRegisterVirtualization : 1;
		u64 VirtualInterruptDelivery : 1;
		u64 PauseLoopExiting : 1;
		u64 RdrandExiting : 1;
		u64 EnableInvpcid : 1;
		u64 EnableVmfunc : 1;
		u64 VmcsShadowing : 1;
		u64 EnableEnclsExiting : 1;
		u64 RdseedExiting : 1;
		u64 EnablePml : 1;
		u64 UseVirtualizationException : 1;
		u64 ConcealVmxFromPt : 1;
		u64 EnableXsaveXrstor : 1;
		u64 Reserved0 : 1;
		u64 ModeBasedExecuteControlEpt : 1;
		u64 Reserved1 : 2;
		u64 UseTscScaling : 1;
	};

}VMX_SECONDARY_CONTROLS, * PVMX_SECONDARY_CONTROLS;

typedef union _VMX_EXIT_CONTROLS
{
	u64 Value;

	struct
	{
		u64 Reserved0 : 2;
		u64 SaveDbgControls : 1;
		u64 Reserved1 : 6;
		u64 HostAddressSpaceSize : 1;
		u64 Reserved2 : 2;
		u64 LoadIa32PerfGlobalControl : 1;
		u64 Reserved3 : 2;
		u64 AckInterruptOnExit : 1;
		u64 Reserved4 : 2;
		u64 SaveIa32Pat : 1;
		u64 LoadIa32Pat : 1;
		u64 SaveIa32Efer : 1;
		u64 LoadIa32Efer : 1;
		u64 SaveVmxPreemptionTimerValue : 1;
		u64 ClearIa32Bndcfgs : 1;
		u64 ConcealVmxFromPt : 1;
	};

}VMX_EXIT_CONTROLS, * PVMX_EXIT_CONTROLS;

typedef union _VMX_ENTRY_CONTROLS
{
	u64 Value;

	struct
	{
		u64 Reserved0 : 2;
		u64 LoadDbgControls : 1;
		u64 Reserved1 : 6;
		u64 Ia32eModeGuest : 1;
		u64 EntryToSmm : 1;
		u64 DeactivateDualMonitorTreatment : 1;
		u64 Reserved2 : 1;
		u64 LoadIa32PerfGlobalControl : 1;
		u64 LoadIa32Pat : 1;
		u64 LoadIa32Efer : 1;
		u64 LoadIa32Bndcfgs : 1;
		u64 ConcealVmxFromPt : 1;
	};

}VMX_ENTRY_CONTROLS, * PVMX_ENTRY_CONTROLS;

typedef struct _VCPU
{
	PVMCS		  vVmcs;
	u64			  pVmcs;

	PVMXON_REGION vVmxonRegion;
	u64			  pVmxonRegion;
}VCPU, * PVCPU;

typedef struct _VMM_CONTEXT
{
	PVCPU VirtualProcessors;
	u64 ProcessorCount;
}VMM_CONTEXT, *PVMM_CONTEXT;

void EnterVmx();
void InitVMXON(PVCPU VCPU);
void InitVMCS(PVCPU VCPU);
void AdjustControlRegisters();
void InitVMMContext(PVMM_CONTEXT VmmContext);

// Note: Must Be Called while executing on the Processor
void InitVCPU(PVCPU VCPU);