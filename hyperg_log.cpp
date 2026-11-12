#include "hyperg_log.hpp"
#include <stdarg.h>
#include <wdm.h>

void Log(const char* Message, ...)
{
	va_list args;
	va_start(args, Message);

	vDbgPrintExWithPrefix(
		"[+] ", 
		DPFLTR_IHVDRIVER_ID, 
		DPFLTR_ERROR_LEVEL, 
		Message, 
		args
	);

	va_end(args);
}
