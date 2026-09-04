#include "hook.hpp"
#include <thread>

namespace PowerEssential {
    void Initialize() {
        Sleep(3000); // Allow game modules to settle on startup
        
        if (!InitializeMemoryPatches()) {
            // Handle patch failure if necessary
        }
        
        if (!InitializeRenderer()) {
            // Handle renderer hook failure if necessary
        }
    }
}

// DirectInput8 Proxy Forwarding (Minimal export structure)
extern "C" {
    HMODULE hOriginalDll = nullptr;

    BOOL WINAPI DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
        switch (ul_reason_for_call) {
        case DLL_PROCESS_ATTACH:
            DisableThreadLibraryCalls(hModule);
            std::thread(PowerEssential::Initialize).detach();
            break;
        case DLL_PROCESS_DETACH:
            PowerEssential::CleanupRenderer();
            break;
        }
        return TRUE;
    }
}
