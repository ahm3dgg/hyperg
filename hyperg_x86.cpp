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
