#pragma once

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <d3d11.h>
#include <dxgi.h>
#include <cstdint>

namespace PowerEssential {
    // Core lifecycle functions
    void Initialize();
    void CleanupRenderer();
    
    // Memory and pattern scanning functions
    bool InitializeMemoryPatches();
    uintptr_t FindPattern(const char* pattern, const char* mask);
    
    // DirectX rendering and hook functions
    bool InitializeRenderer();
}
