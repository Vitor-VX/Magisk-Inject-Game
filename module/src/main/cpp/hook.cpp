#include "hook.h"
#include "log.h"
#include <cstring>
#include <cstdio>
#include <unistd.h>
#include <sys/system_properties.h>
#include <dlfcn.h>
#include <jni.h>
#include <thread>
#include <sys/mman.h>
#include <linux/unistd.h>
#include <array>

#include "KittyMemory/KittyMemory.h"
#include "KittyMemory/MemoryPatch.h"
#include "KittyMemory/KittyUtils.h"
#include "Includes/Dobby/dobbyForHooks.h"
#include "MenuInjector.h"
#include <string.h>

//#include "obfuscate.h"
//#include "Include/Unity.h"

uint64_t location;

using namespace std;

uint64_t getRealOffset(uint64_t offset) {
    if (location == 0) {
        KittyMemory::ProcMap map;
        do {
            map = KittyMemory::getLibraryMap("libil2cpp.so");
            sleep(1);
        } while (!map.isValid());

        location = reinterpret_cast<uint64_t>(map.startAddr);

    }

    return location + offset;
}

void SetMaxHP(void *a1) {
    void (*SetMaxHP)(void *a1) = (void (*)(void *)) getRealOffset(0x154B48A);
    SetMaxHP(a1);

    return;
}

void SetSpeed(void *a1) {
    void (*SetSpeed)(void *a1, float value) = (void (*)(void *, float)) getRealOffset(0x1533500);
    SetSpeed(a1, 130.0f);

    return;
}

void SetHealth(void *a1) {
    void (*SetHealth)(void *a1, float hp) = (void (*)(void *, float)) getRealOffset(0x15478A9);
    SetHealth(a1, 0.0f);

    return;
}

void ActionType(void *a1) {
    void (*ActionType)(void *a1, void *value) = (void (*)(void*,void*)) getRealOffset(0x1548FB2);
    ActionType(a1, (void *)0);

    return;
}

void *(*orig_DamageHealth)(void *a1, void *attacker, float damage, void *hitType, bool right);

void *hook_DamageHealth(void *a1, void *attacker, float damage, void *hitType, bool right) {

    if (funcoes.increasedDamage) {
        damage = 9999.9f;

        return orig_DamageHealth(a1, attacker, damage, hitType, right);
    }

    return orig_DamageHealth(a1, attacker, damage, hitType, right);
}

void *(*orig_UpdateGame)(void *a1);

void *hook_UpdateGame(void *a1) {

    bool (*IsEnemy)(bool *a1) = (bool (*)(bool *)) getRealOffset(0x153E7FF);

    if (funcoes.maxHp && IsEnemy((bool *)a1) == 0) {
        SetMaxHP(a1);
    }

    if (funcoes.setSpeed && IsEnemy((bool *)a1) == 0) {
        SetSpeed(a1);
    }

    if (funcoes.increasedDamage && IsEnemy((bool *)a1) == 0) {
        DobbyHook((void *) getRealOffset(0x154829C), (void *) hook_DamageHealth,(void **) &orig_DamageHealth);
    }

    if (funcoes.hpZero && IsEnemy((bool *)a1) == 1) {
        SetHealth(a1);
    }

    /*
    if (funcoes.testando && IsEnemy((bool*) a1) == 0) {
        ActionType(a1);
    }

    if(funcoes.attackSpeed) {
        uint64_t TableLevel = reinterpret_cast<uint64_t>(a1) + 0x50;

        float* AttackSpeed = reinterpret_cast<float*>(TableLevel + 0x34);
        if (AttackSpeed != nullptr) {
            LOGI("%f", *AttackSpeed);
            *AttackSpeed = 9999.0f;
        }
    }
    */

    return orig_UpdateGame(a1);
}

// update game instance

void ClearPerson(void *a1) {
    void (*ClearPerson)(void *a) = (void (*)(void *)) getRealOffset(0x15ABF74);
    ClearPerson(a1);

    return;
}

void set_EnableInvincibility(void *a1) {
    void (*set_EnableInvincibility)(void *a, bool value) = (void (*)(void *, bool)) getRealOffset(0x15ABF74);
    set_EnableInvincibility(a1, true);

    return;
}

void *(*orig_updateInstance)(void *a1);

void *hook_updateInstace(void *a1) {

    //LOGI("instance active!");

    if (funcoes.clearPerson) {
        ClearPerson(a1);
    }

    if (funcoes.vida) {
        set_EnableInvincibility(a1);
    }

    return orig_updateInstance(a1);
}

void menu_init() {
    LOGI("menu init!");

    do {
        sleep(1);
    } while (!isLibraryLoaded("libEGL.so"));

    auto swapBuffers = ((uint64_t) DobbySymbolResolver("libEGL.so", "eglSwapBuffers"));
    mprotect((void *) swapBuffers, 8, PROT_READ | PROT_WRITE | PROT_EXEC);
    DobbyHook((void *) swapBuffers, (void *) swapbuffers_hook, (void **) &o_swapbuffers);


    void *sym_input = DobbySymbolResolver(("/system/lib/libinput.so"), ("_ZN7android13InputConsumer21initializeMotionEventEPNS_11MotionEventEPKNS_12InputMessageE"));
    if (NULL != sym_input) {
        LOGI("system input success!");
        DobbyHook(sym_input,(void*)hook_Input,(void**)&orig_Input);
    }
}

void hack_start() {
    LOGI("thread iniciada!");

    DobbyHook((void *) getRealOffset(0x153F144), (void *) hook_UpdateGame,(void **) &orig_UpdateGame);

    std::thread menu_thread(menu_init);
    menu_thread.detach();
}

static std::string GetNativeBridgeLibrary() {
    auto value = std::array<char, PROP_VALUE_MAX>();
    __system_property_get("ro.dalvik.vm.native.bridge", value.data());
    return {value.data()};
}

struct NativeBridgeCallbacks {
    uint32_t version;
    void *initialize;

    void *(*loadLibrary)(const char *libpath, int flag);

    void *(*getTrampoline)(void *handle, const char *name, const char *shorty, uint32_t len);

    void *isSupported;
    void *getAppEnv;
    void *isCompatibleWith;
    void *getSignalHandler;
    void *unloadLibrary;
    void *getError;
    void *isPathSupported;
    void *initAnonymousNamespace;
    void *createNamespace;
    void *linkNamespaces;

    void *(*loadLibraryExt)(const char *libpath, int flag, void *ns);
};

bool NativeBridgeLoad(const char *game_data_dir, int api_level, void *data, size_t length) {
    sleep(5);

    auto nb = dlopen("libhoudini.so", RTLD_NOW);
    if (!nb) {
        auto native_bridge = GetNativeBridgeLibrary();
        nb = dlopen(native_bridge.data(), RTLD_NOW);
    }
    if (nb) {
        auto callbacks = (NativeBridgeCallbacks *) dlsym(nb, "NativeBridgeItf");
        if (callbacks) {

            int fd = syscall(__NR_memfd_create, "anon", MFD_CLOEXEC);
            ftruncate(fd, (off_t) length);
            void *mem = mmap(nullptr, length, PROT_WRITE, MAP_SHARED, fd, 0);
            memcpy(mem, data, length);
            munmap(mem, length);
            munmap(data, length);
            char path[PATH_MAX];
            snprintf(path, PATH_MAX, "/proc/self/fd/%d", fd);
        }
    }
    return false;
}

void hack_prepare(const char *game_data_dir, void *data, size_t length) {
    //LOGI("hack thread: %d", gettid());
    int api_level = android_get_device_api_level();
    //LOGI("api level: %d", api_level);

#if defined(__i386__) || defined(__x86_64__)
    if (!NativeBridgeLoad(game_data_dir, api_level, data, length)) {
#endif
        hack_start();
#if defined(__i386__) || defined(__x86_64__)
    }
#endif
}

#if defined(__arm__) || defined(__aarch64__)
JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved) {
    std::thread hack_thread(hack_start);
    hack_thread.detach();
    return JNI_VERSION_1_6;
}

#endif