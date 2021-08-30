#include <jni.h>
#include <string>
#include <xhook.h>
#include <unistd.h>
#include <sys/prctl.h>
#include <android/log.h>
#include "backtrace.h"
#include "thread_args.h"

#define  LOG_TAG    "TAware"
#define  ALOG(...)  __android_log_print(ANDROID_LOG_WARN,LOG_TAG, __VA_ARGS__)

std::atomic<bool> enable_aware_thread;  // 使能感知线程

extern "C" JNIEXPORT jstring JNICALL
Java_com_bt_lib_taware_TAware_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}

int (*origin_pthread_create)(pthread_t* thread, const pthread_attr_t* attr,
                             void* (*start_routine) (void *), void* arg);

void *create_call_back(void *args) {
    ALOG("thread running and tid is %d.", gettid());

    auto* ta = reinterpret_cast<thread_args*>(args);
    ta->start_routine(ta->start_routine_arg);

    char tname[32];
    prctl(PR_GET_NAME, tname);
    ALOG("thread ending and thread name is %s.", tname);
    return nullptr;
}

int hook_pthread_create(pthread_t* thread, const pthread_attr_t* attr,
                        void* (*start_routine) (void *), void* arg) {
    auto *bt = new backtrace();
    bt->backtraceToLogcat();

    auto *ta = new thread_args();
    ta->start_routine = start_routine;
    ta->start_routine_arg = arg;
    return origin_pthread_create(thread, attr, create_call_back, ta);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_bt_lib_taware_TAware_enableAwareThread(
        JNIEnv *env,
        jclass clazz,
        jint async) {

    if (enable_aware_thread) {
        return;
    }

    enable_aware_thread = true;

    xhook_register(".*/.*\\.so", "pthread_create", (void *)hook_pthread_create, (void **)(&origin_pthread_create));
    xhook_refresh(async);
}