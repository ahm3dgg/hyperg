#include <ntddk.h>
#include "hyperg_log.hpp"
#include "hyperg_vtx.hpp"
#include "hyperg_x86.hpp"

VMM_CONTEXT VmmContext;

u64 GetProcessorCount()
{
	KAFFINITY active_processors;
	u64 processor_count = 0;
	KeQueryActiveProcessorCount(&active_processors);

	for (u64 i = 0; i < 64; i++)
	{
		if ((active_processors & (1ULL << i)) != 0)
		{
			processor_count++;
		}
	}

	return processor_count;
}

template <typename TFunc, typename... TArgs>
VOID
CallOnAllProcessors(TFunc&& Func, TArgs&&... Args)
{
	for (u64 i = 0; i < GetProcessorCount(); i++)
	{
		PROCESSOR_NUMBER processor_number;
		GROUP_AFFINITY affinity = {}, old_affinity = {};

		KeGetProcessorNumberFromIndex((ULONG)i, &processor_number);

		affinity.Group = processor_number.Group;
		affinity.Mask = (KAFFINITY)1 << processor_number.Number;
		KeSetSystemGroupAffinityThread(&affinity, &old_affinity);

		Func(i, Args...);

		KeRevertToUserGroupAffinityThread(&old_affinity);
	}
}

VOID
DriverUnload(
	PDRIVER_OBJECT DriverObject)
{
	UNREFERENCED_PARAMETER(DriverObject);
	Log("Driver Unloaded");

	CallOnAllProcessors([](u64 processor_number) { VMXOFF(); });

	ExFreePool(VmmContext.VirtualProcessors);
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

	CallOnAllProcessors(
		[](u64 processor_number, VMM_CONTEXT& VmmCtx) {
			InitVCPU(&VmmCtx.VirtualProcessors[processor_number]);
		},

		VmmContext
	);

	Status = STATUS_SUCCESS;

	return Status;
}