//
// Created by yinhua on 2021/8/30.
//

#include <iostream>
#include <iomanip>
#include <unwind.h>
#include <dlfcn.h>
#include <sstream>
#include "backtrace.h"
#include <android/log.h>

#define  LOG_TAG    "TAware"
#define  ALOG(...)  __android_log_print(ANDROID_LOG_WARN,LOG_TAG, __VA_ARGS__)

namespace {

    struct BacktraceState {
        void **current;
        void **end;
    };

    static _Unwind_Reason_Code unwindCallback(struct _Unwind_Context *context, void *arg) {
        BacktraceState *state = static_cast<BacktraceState *>(arg);
        uintptr_t pc = _Unwind_GetIP(context);
        if (pc) {
            if (state->current == state->end) {
                return _URC_END_OF_STACK;
            } else {
                *state->current++ = reinterpret_cast<void *>(pc);
            }
        }
        return _URC_NO_REASON;
    }

}

size_t captureBacktrace(void **buffer, size_t max) {
    BacktraceState state = {buffer, buffer + max};
    _Unwind_Backtrace(unwindCallback, &state);

    return state.current - buffer;
}

void dumpBacktrace(std::ostream &os, void **buffer, size_t count) {
    if (count <= 2) {
        return;
    }

    for (size_t idx = 2; idx < count; ++idx) {
        const void *addr = buffer[idx];
        const char *symbol = "";

        Dl_info info;
        if (dladdr(addr, &info) && info.dli_sname) {
            symbol = info.dli_sname;
        }

        if (strlen(symbol) <= 0) {
            continue;
        }
        os << "  #" << std::setw(2) << (idx - 2) << ": " << addr << "  " << symbol << "\n";
    }
}

void backtrace::backtraceToLogcat() {
    const size_t max = 30;
    void *buffer[max];
    std::ostringstream oss;

    dumpBacktrace(oss, buffer, captureBacktrace(buffer, max));

    ALOG("invoke back trace:\n %s", oss.str().c_str());
}
