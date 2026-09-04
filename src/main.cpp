#include <windows.h>
#include <filesystem>
#include <string>

namespace fs = std::filesystem;

void InitializeModFolders() {
    // Get the directory where our custom dinput8.dll is loaded from
    char dllPath[MAX_PATH];
    GetModuleFileNameA(GetModuleHandleA("dinput8.dll"), dllPath, MAX_PATH);
    fs::path gameDir = fs::path(dllPath).parent_path();

    // Define target directories
    fs::path scriptsDir = gameDir / "scripts";
    fs::path menyooDir = gameDir / "menyooStuff";

    // Create directories if they don't exist yet
    if (!fs::exists(scriptsDir)) {
        fs::create_directories(scriptsDir);
    }
    if (!fs::exists(menyooDir)) {
        fs::create_directories(menyooDir);
    }

    // Example: Scan scripts folder for custom files or native plugins
    for (const auto& entry : fs::directory_iterator(scriptsDir)) {
        if (entry.is_regular_file()) {
            std::string ext = entry.path().extension().string();
            // Handle loading logic based on extension (e.g., .asi or config files)
        }
    }
}
