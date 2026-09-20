//
// Classic99 - pico9918-core bridge, debugger half
//
// (C) 2026 Troy Schrapel (visrealm)
//
// Separate TU because this is the integration's only use of pico9918-core's
// debugger surface, which the library builds only when asked for it
// (PICO9918_DEBUG_API, wired on in classic99.vcxproj).
//

#define WIN32_LEAN_AND_MEAN
#define _WIN32_WINNT 0x0501

#include <stdio.h>
#include <windows.h>

#include "tiemul.h"
#include "tivdp_pico9918.h"

#include "pico9918.h"
#include "gpu/gpu.h"
#include "pico9918_debug.h"

extern pico9918_t *tms9918;

// not the bus - that aliases VR30 onto R6 on a locked device
void p9918DbgWriteReg(unsigned char r, unsigned char v) {
	if (!p9918Active()) return;

	pico9918_debug_reg_write(tms9918, r, v);
	p9918SyncShadow();
}

unsigned char p9918DbgStatus(unsigned char reg) {
	if (!p9918Active()) return 0;
	return pico9918_status_value(tms9918, (pico9918_status_register_t)(reg & 0x0f));
}

unsigned int p9918DbgGpuPC() {
	if (!p9918Active()) return 0xffff;
	return pico9918_gpu_pc(tms9918);
}

bool p9918DbgGpuArmed() {
	return p9918Active() && pico9918_debug_gpu_armed(tms9918);
}

unsigned int p9918DbgGpuReg(unsigned char reg) {
	if (!p9918Active()) return 0;
	return pico9918_gpu_reg_value(tms9918, reg);
}

unsigned int p9918DbgGpuStatus() {
	if (!p9918Active()) return 0;
	return pico9918_gpu_status(tms9918);
}
