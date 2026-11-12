#include "hyperg_x86.hpp"
#include <intrin.h>

CR0 ReadCR0()
{
	return { __readcr0() };
}

CR4 ReadCR4()
{
	return { __readcr4() };
}

void WriteCR0(u64 Value)
{
	__writecr0(Value);
}

void WriteCR4(u64 Value)
{
	__writecr4(Value);
}

void WriteMSR(u64 MSR, u64 Value)
{
	__writemsr(MSR, Value);
}

u8 VMXON(u64 Vmcs)
{
	return __vmx_on(&Vmcs);
}

void VMXOFF()
{
	__vmx_off();
}

void VMLAUNCH()
{
	__vmx_vmlaunch();
}

u8 VMPTRLD(u64 Vmcs)
{
	return __vmx_vmptrld(&Vmcs);
}

u8 VMCLEAR(u64 Vmcs)
{
	return __vmx_vmclear(&Vmcs);
}
