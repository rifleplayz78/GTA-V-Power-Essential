#include <windows.h>
#include <filesystem>
#include <fstream>
#include <string>
#include "pugixml.hpp"

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

DWORD WINAPI MainThread(LPVOID lpParam) {
    char dllPath[MAX_PATH];
    GetModuleFileNameA(GetModuleHandleA("dinput8.dll"), dllPath, MAX_PATH);
    fs::path gameDir = fs::path(dllPath).parent_path();

    WriteLog(gameDir, "MainThread started successfully.");

    while (true) {
        // Mod loop logic
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
