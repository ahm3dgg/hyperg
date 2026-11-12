#include <ntddk.h>
#include "hyperg_log.hpp"
#include "hyperg_vtx.hpp"
#include <intrin.h>

VMM_CONTEXT VmmContext;

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

	Log("hyperg Loaded.\n");

	DriverObject->DriverUnload = DriverUnload;

	InitVMMContext(&VmmContext);

	for (u64 i = 0; i < VmmContext.ProcessorCount; i++)
	{
		PROCESSOR_NUMBER processor_number;
		GROUP_AFFINITY affinity = {}, old_affinity = {};

		KeGetProcessorNumberFromIndex(i, &processor_number);

		affinity.Group = processor_number.Group;
		affinity.Mask = (KAFFINITY)1 << processor_number.Number;
		KeSetSystemGroupAffinityThread(&affinity, &old_affinity);

		InitVCPU(&VmmContext.VirtualProcessors[i]);

		KeRevertToUserGroupAffinityThread(&old_affinity);
	}

	Status = STATUS_SUCCESS;

	return Status;
}