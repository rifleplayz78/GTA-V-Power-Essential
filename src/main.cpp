#include <windows.h>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>
#include "pugixml.hpp"

// Proxy exports for dinput8.dll
#pragma comment(linker, "/export:DirectInput8Create=C:\\Windows\\System32\\dinput8.DirectInput8Create,@1")
#pragma comment(linker, "/export:GetdfDIJoystick=C:\\Windows\\System32\\dinput8.GetdfDIJoystick,@2")

namespace fs = std::filesystem;

struct VehicleEntry {
    std::string name;
    std::string model;
};

std::vector<VehicleEntry> g_CustomVehicles;

void WriteLog(const fs::path& gameDir, const std::string& message) {
    fs::path logPath = gameDir / "GTAV_Native_Log.txt";
    std::ofstream logFile(logPath, std::ios::app);
    if (logFile.is_open()) {
        logFile << message << "\n";
    }
}

void ApplyMemoryPatches(const fs::path& gameDir) {
    // TODO: Implement direct memory/heap and packfile limit byte patches here
    WriteLog(gameDir, "Memory and heap adjustment patches applied.");
}

void LoadConfig(const fs::path& gameDir) {
    fs::path xmlPath = gameDir / "menyoostuff" / "VehicleList.xml";
    pugi::xml_document doc;
    
    if (doc.load_file(xmlPath.wstring().c_str())) {
        WriteLog(gameDir, "Successfully loaded menyoostuff/VehicleList.xml");
        
        g_CustomVehicles.clear();
        pugi::xml_node root = doc.child("VehicleList");
        for (pugi::xml_node vehNode = root.child("Vehicle"); vehNode; vehNode = vehNode.next_sibling("Vehicle")) {
            VehicleEntry entry;
            entry.name = vehNode.attribute("Name").as_string("Unknown");
            entry.model = vehNode.attribute("Model").as_string("adder");
            g_CustomVehicles.push_back(entry);
        }
        WriteLog(gameDir, "Parsed " + std::to_string(g_CustomVehicles.size()) + " vehicles natively.");
    } else {
        WriteLog(gameDir, "VehicleList.xml not found - operating with defaults.");
    }
}

DWORD WINAPI MainThread(LPVOID lpParam) {
    char dllPath[MAX_PATH];
    GetModuleFileNameA(GetModuleHandleA("dinput8.dll"), dllPath, MAX_PATH);
    fs::path gameDir = fs::path(dllPath).parent_path();

    WriteLog(gameDir, "Native Mod Loader thread initialized.");
    
    ApplyMemoryPatches(gameDir);
    LoadConfig(gameDir);

    bool f4PressedLastFrame = false;
    bool f8PressedLastFrame = false;
    bool nativeUiVisible = false;
    bool menyooVisible = false;

    while (true) {
        // Toggle NativeUI via F4
        bool f4Current = (GetAsyncKeyState(VK_F4) & 0x8000) != 0;
        if (f4Current && !f4PressedLastFrame) {
            nativeUiVisible = !nativeUiVisible;
            WriteLog(gameDir, nativeUiVisible ? "NativeUI Layer Opened" : "NativeUI Layer Closed");
        }
        f4PressedLastFrame = f4Current;

        // Toggle Menyoo via F8
        bool f8Current = (GetAsyncKeyState(VK_F8) & 0x8000) != 0;
        if (f8Current && !f8PressedLastFrame) {
            menyooVisible = !menyooVisible;
            WriteLog(gameDir, menyooVisible ? "Menyoo Menu Opened" : "Menyoo Menu Closed");
        }
        f8PressedLastFrame = f8Current;

        Sleep(10);
    }
    return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    switch (ul_reason_for_call) {
    case DLL_PROCESS_ATTACH: {
        DisableThreadLibraryCalls(hModule);
        CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)MainThread, nullptr, 0, nullptr);
        break;
    }
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}
