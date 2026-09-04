#include <windows.h>
#include <filesystem>
#include <fstream>
#include <string>
#include "pugixml.hpp"

// Forward declarations or inclusions for NativeUI / Menyoo hooks if applicable
// #include "NativeUI.h" 

#pragma comment(linker, "/export:DirectInput8Create=C:\\Windows\\System32\\dinput8.DirectInput8Create,@1")
#pragma comment(linker, "/export:GetdfDIJoystick=C:\\Windows\\System32\\dinput8.GetdfDIJoystick,@2")

namespace fs = std::filesystem;

void WriteLog(const fs::path& gameDir, const std::string& message) {
    fs::path logPath = gameDir / "GTAV essen log.txt";
    std::ofstream logFile(logPath, std::ios::app);
    if (logFile.is_open()) {
        logFile << message << "\n";
    }
}

// Configuration loading via pugixml
void LoadConfig(const fs::path& gameDir) {
    fs::path xmlPath = gameDir / "VehicleList.xml";
    pugi::xml_document doc;
    if (doc.load_file(xmlPath.wstring().c_str())) {
        WriteLog(gameDir, "Successfully loaded VehicleList.xml");
        // Parse your nodes here
    } else {
        WriteLog(gameDir, "Failed to load VehicleList.xml - using defaults");
    }
}

DWORD WINAPI MainThread(LPVOID lpParam) {
    char dllPath[MAX_PATH];
    GetModuleFileNameA(GetModuleHandleA("dinput8.dll"), dllPath, MAX_PATH);
    fs::path gameDir = fs::path(dllPath).parent_path();

    WriteLog(gameDir, "MainThread started successfully.");
    LoadConfig(gameDir);

    bool f4PressedLastFrame = false;
    bool f8PressedLastFrame = false;
    bool nativeUiVisible = false;
    bool menyooVisible = false;

    while (true) {
        // F4 Keybind: NativeUI Toggle
        bool f4Current = (GetAsyncKeyState(VK_F4) & 0x8000) != 0;
        if (f4Current && !f4PressedLastFrame) {
            nativeUiVisible = !nativeUiVisible;
            WriteLog(gameDir, nativeUiVisible ? "NativeUI Opened" : "NativeUI Closed");
            // TODO: Call NativeUI render/toggle function here
        }
        f4PressedLastFrame = f4Current;

        // F8 Keybind: Menyoo / Trainer Toggle
        bool f8Current = (GetAsyncKeyState(VK_F8) & 0x8000) != 0;
        if (f8Current && !f8PressedLastFrame) {
            menyooVisible = !menyooVisible;
            WriteLog(gameDir, menyooVisible ? "Menyoo Opened" : "Menyoo Closed");
            // TODO: Call Menyoo render/toggle function here
        }
        f8PressedLastFrame = f8Current;

        // Continuous tick logic for menus/placement can go here
        if (nativeUiVisible) {
            // NativeUI process tick
        }
        if (menyooVisible) {
            // Menyoo process tick
        }

        Sleep(5);
    }
    return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    switch (ul_reason_for_call) {
    case DLL_PROCESS_ATTACH: {
        DisableThreadLibraryCalls(hModule);
        
        char dllPath[MAX_PATH];
        GetModuleFileNameA(hModule, dllPath, MAX_PATH);
        fs::path gameDir = fs::path(dllPath).parent_path();
        
        WriteLog(gameDir, "DLL_PROCESS_ATTACH triggered. GTAV essen log.txt created.");
        CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)MainThread, nullptr, 0, nullptr);
        break;
    }
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}
