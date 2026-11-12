#include "hyperg_vtx.hpp"
#include "hyperg_x86.hpp"
#include "hyperg_log.hpp"
#include <ntddk.h>

void EnterVmx()
{
	auto cr4 = ReadCR4();
	cr4.VMXE = 1;
	WriteCR4(cr4.Value);

	auto feature_control = 
		ReadMSR<IA32_FEATURE_CONTROL>(MSR_IA32_FEATURE_CONTROL);

	if (!feature_control.Lock)
	{
		feature_control.Lock = 1;
		feature_control.VmxOutsideSmx = 1;
		WriteMSR(MSR_IA32_FEATURE_CONTROL, feature_control.Value);
	}
}

void InitVMXON(PVCPU VCPU)
{
	PHYSICAL_ADDRESS max_physc = { ~0ULL };

	auto vmx_basic = ReadMSR<IA32_VMX_BASIC>(MSR_IA32_VMX_BASIC);

	Log("VT-X RevisionId: %lx\n", vmx_basic.RevisionId);
	Log("VMXON Region Size = 0x%lx\n", vmx_basic.RegionSize);
	VCPU->vVmxonRegion = (PVMXON_REGION)MmAllocateContiguousMemory(vmx_basic.RegionSize, max_physc);
	Log("VMXON Region = %p\n", VCPU->vVmxonRegion);
	VCPU->pVmxonRegion = MmGetPhysicalAddress((PVOID)VCPU->vVmxonRegion).QuadPart;

	RtlSecureZeroMemory((PVOID)VCPU->vVmxonRegion, vmx_basic.RegionSize);

	VCPU->vVmxonRegion->Header.Bits.RevisionId = vmx_basic.RevisionId;

	Log("VMXON Initialized for Processor #%d, %p\n", KeGetCurrentProcessorNumber(), VCPU->vVmxonRegion);
}

void InitVMCS(PVCPU VCPU)
{
	PHYSICAL_ADDRESS max_physc = { ~0ULL };

	auto vmx_basic = ReadMSR<IA32_VMX_BASIC>(MSR_IA32_VMX_BASIC);

	VCPU->vVmcs = (PVMCS)MmAllocateContiguousMemory(vmx_basic.RegionSize, max_physc);
	VCPU->pVmcs = MmGetPhysicalAddress((PVOID)VCPU->vVmcs).QuadPart;

	RtlSecureZeroMemory((PVOID)VCPU->vVmcs, vmx_basic.RegionSize);

	VCPU->vVmcs->Header.Bits.RevisionId = vmx_basic.RevisionId;

	Log("VMCS Initialized for Processor #%d, %p\n", KeGetCurrentProcessorNumber(), VCPU->vVmcs);
}

void AdjustControlRegisters()
{
	auto cr0_en_mask = ReadMSR<u64>(MSR_IA32_VMX_CR0_FIXED0);
	auto cr0_dis_mask = ReadMSR<u64>(MSR_IA32_VMX_CR0_FIXED1);

	auto cr4_en_mask = ReadMSR<u64>(MSR_IA32_VMX_CR4_FIXED0);
	auto cr4_dis_mask = ReadMSR<u64>(MSR_IA32_VMX_CR4_FIXED1);

	LARGE_INTEGER cr0 = { ReadCR0().Value };
	LARGE_INTEGER cr4 = { ReadCR4().Value };

	cr0.LowPart |= cr0_en_mask;
	cr0.LowPart &= cr4_dis_mask;

	cr4.LowPart |= cr4_en_mask;
	cr4.LowPart &= cr4_dis_mask;
}

void InitVMMContext(PVMM_CONTEXT VmmContext)
{
	auto processor_affinity = KeQueryActiveProcessors();
	u64 processor_count = 0;

	for (u64 i = 0; i < 32; i++)
	{
		if (processor_affinity & (1ULL << i))
		{
			processor_count++;
		}
	}

	VmmContext->ProcessorCount = processor_count;

	VmmContext->VirtualProcessors = 
		(PVCPU)ExAllocatePool(NonPagedPool, sizeof(VCPU) * VmmContext->ProcessorCount);
}

void InitVCPU(PVCPU VCPU)
{
	InitVMXON(VCPU);
	InitVMCS(VCPU);
	AdjustControlRegisters();
	EnterVmx();

	auto processor_number = KeGetCurrentProcessorNumber();

	if (VMXON(VCPU->pVmxonRegion) != 0)
	{
		Log("Failed to Enter VMX Mode on Processor #%d.\n", processor_number);
		return;
	}
	
	Log("Entered VMX Mode on Processor #%d.\n", processor_number);
	
	if (VMCLEAR(VCPU->pVmcs) != 0 || VMPTRLD(VCPU->pVmcs) != 0)
	{
		Log("Failed to activate VMCS on Processor %#d\n", processor_number);
		return;
	}

	Log("Activated VMCS on Processor #%d.\n", processor_number);
}