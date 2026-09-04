#include <windows.h>
#include <filesystem>
#include <string>
#include "pugixml.hpp"

namespace fs = std::filesystem;

bool nativeUIOpen = false;
bool menyooOpen = false;
ULONGLONG lastF8Tick = 0;

void InitializeModFolders(const fs::path& gameDir) {
    fs::path scriptsDir = gameDir / "scripts";
    fs::path menyooDir = gameDir / "menyooStuff";

    if (!fs::exists(scriptsDir)) {
        fs::create_directories(scriptsDir);
    }
    if (!fs::exists(menyooDir)) {
        fs::create_directories(menyooDir);
    }

    // Parse VehicleList.xml if present
    fs::path vehicleListPath = menyooDir / "VehicleList.xml";
    if (fs::exists(vehicleListPath)) {
        pugi::xml_document doc;
        if (doc.load_file(vehicleListPath.wstring().c_str())) {
            // Placeholder for loading vehicle nodes into memory
        }
    }
}

void CheckInputKeybinds() {
    // F4 for Native UI toggle
    if (GetAsyncKeyState(VK_F4) & 1) {
        nativeUIOpen = !nativeUIOpen;
    }

    // F8 Double-Tap for Menyoo toggle
    if (GetAsyncKeyState(VK_F8) & 1) {
        ULONGLONG currentTick = GetTickCount64();
        if (currentTick - lastF8Tick < 400) { 
            menyooOpen = !menyooOpen;
            lastF8Tick = 0; 
        } else {
            lastF8Tick = currentTick;
        }
    }
}

DWORD WINAPI MainThread(LPVOID lpParam) {
    char dllPath[MAX_PATH];
    GetModuleFileNameA(GetModuleHandleA("dinput8.dll"), dllPath, MAX_PATH);
    fs::path gameDir = fs::path(dllPath).parent_path();

    InitializeModFolders(gameDir);

    while (true) {
        CheckInputKeybinds();
        
        // Render triggers based on states
        if (nativeUIOpen) {
            // Native UI draw loop logic
        }
        if (menyooOpen) {
            // Menyoo UI draw loop logic
        }

        Sleep(5);
    }
    return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    switch (ul_reason_for_call) {
    case DLL_PROCESS_ATTACH:
        DisableThreadLibraryCalls(hModule);
        CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)MainThread, nullptr, 0, nullptr);
        break;
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}
