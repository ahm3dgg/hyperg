#include <ntddk.h>
#include "hyperg_log.hpp"
#include "hyperg_vtx.hpp"
#include <intrin.h>

VOID
DriverUnload(
	PDRIVER_OBJECT DriverObject)
{
	UNREFERENCED_PARAMETER(DriverObject);
	Log("Driver Unloaded");
}

NTSTATUS 
DriverEntry(
	IN PDRIVER_OBJECT DriverObject,
	IN PUNICODE_STRING RegistryPath)
{
	UNREFERENCED_PARAMETER(DriverObject);
	UNREFERENCED_PARAMETER(RegistryPath);

	NTSTATUS Status;
	VCPU vcpu;

	Log("hyperg Loaded.\n");

	InitVMXON(&vcpu);
	InitVMCS(&vcpu);
	AdjustControlRegisters();
	EnterVmx();

	if (__vmx_on(&vcpu.pVmxonRegion) != 0)
	{
		Log("Failed to Enter VMX Mode.");
	}
	else
	{
		Log("Entered VMX Mode.");
	}

	Status = STATUS_SUCCESS;

	return Status;
}