#include <iostream>
#include <windows.h>
#include <TlHelp32.h>
#include <Psapi.h>
#include <limits>
#include <algorithm>
#include <iomanip>

#undef max
#undef min

#include "memory/patterns.hpp"
#include "memory/memory.cpp"

#pragma comment(lib, "Psapi.lib")

void EnableVirtualTerminalProcessing() {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut == INVALID_HANDLE_VALUE) return;

    DWORD dwMode = 0;
    if (!GetConsoleMode(hOut, &dwMode)) return;

    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);
}

void WaitForExit() {
    std::cout << "\033[38;2;0;255;255m\n  [+] Press Enter to exit...\033[0m";
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
}

uintptr_t GetModuleBaseAddress(DWORD processId, const wchar_t* moduleName) {
    uintptr_t moduleBaseAddress = 0;
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, processId);
    if (hSnapshot != INVALID_HANDLE_VALUE) {
        MODULEENTRY32W moduleEntry;
        moduleEntry.dwSize = sizeof(moduleEntry);
        if (Module32FirstW(hSnapshot, &moduleEntry)) {
            do {
                if (_wcsicmp(moduleEntry.szModule, moduleName) == 0) {
                    moduleBaseAddress = (uintptr_t)moduleEntry.modBaseAddr;
                    break;
                }
            } while (Module32NextW(hSnapshot, &moduleEntry));
        }
    }
    CloseHandle(hSnapshot);
    return moduleBaseAddress;
}

void DumpOffsets(HANDLE hProcess, uintptr_t baseAddress) {
    const std::string neon_cyan = "\033[38;2;0;255;255m";
    const std::string bright_white = "\033[38;2;255;255;255m";
    const std::string error_red = "\033[38;2;255;50;50m";
    const std::string reset = "\033[0m";

    system("cls");
    std::cout << "\n\n";
    std::cout << neon_cyan << "  [" << bright_white << "+" << neon_cyan << "] " << bright_white << "FiveM Offset Dumper " << neon_cyan << "by Ghost0Dev\n";
    std::cout << neon_cyan << "  " << std::string(46, '=') << "\n\n" << reset;

    for (const auto& pattern : patterns::offsets) {
        uintptr_t instruction_address = memory::pattern_scan(hProcess, baseAddress, pattern.signature);
        
        std::cout << "    " << bright_white << std::left << std::setw(30) << pattern.name;
        
        if (instruction_address) {
            uintptr_t resolved_address = memory::get_rip_relative_address(hProcess, instruction_address);
            uintptr_t offset_fix = resolved_address - baseAddress;
            std::cout << neon_cyan << "0x" << std::hex << std::uppercase << offset_fix << reset << "\n";
        }
        else {
            std::cout << error_red << "Not Found" << reset << "\n";
        }
        std::cout << std::dec << std::nouppercase;
    }
    std::cout << "\n";
}

int main() {
    EnableVirtualTerminalProcessing();
    SetConsoleTitleA("FiveM Offset Dumper");

    HWND console = GetConsoleWindow();
    if (console) {
        RECT r;
        GetWindowRect(console, &r);
        MoveWindow(console, r.left, r.top, 550, 400, TRUE);
    }
    
    const std::string neon_cyan = "\033[38;2;0;255;255m";
    const std::string bright_white = "\033[38;2;255;255;255m";
    const std::string reset = "\033[0m";

    std::cout << "\n\n";
    std::cout << neon_cyan << "  [" << bright_white << "+" << neon_cyan << "] " << bright_white << "FiveM Offset Dumper " << neon_cyan << "by Ghost0Dev\n";
    std::cout << neon_cyan << "  " << std::string(46, '=') << "\n\n" << reset;

    DWORD processId = 0;
    HANDLE hProcess = NULL;

    PROCESSENTRY32 processEntry;
    processEntry.dwSize = sizeof(PROCESSENTRY32);
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snapshot == INVALID_HANDLE_VALUE) {
        std::cout << "\033[38;2;255;50;50m  [!] Error: Failed to create process snapshot!\033[0m\n";
        WaitForExit();
        return 1;
    }

    if (Process32First(snapshot, &processEntry)) {
        do {
            std::wstring exeName = processEntry.szExeFile;
            if (exeName.find(L"FiveM") != std::wstring::npos && exeName.find(L"GTAProcess") != std::wstring::npos) {
                processId = processEntry.th32ProcessID;
                break;
            }
        } while (Process32Next(snapshot, &processEntry));
    }

    CloseHandle(snapshot);

    if (processId == 0) {
        std::cout << "\033[38;2;0;255;255m  [\033[38;2;255;255;255m*\033[38;2;0;255;255m] \033[38;2;255;255;255mWaiting for FiveM to start...\033[0m\n";
        WaitForExit();
        return 1;
    }

    hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processId);
    if (hProcess == NULL) {
        std::cout << "\033[38;2;255;50;50m  [!] Error: Could not open FiveM process!\033[0m\n";
        WaitForExit();
        return 1;
    }

    uintptr_t baseAddress = 0;
    HMODULE modules[1024];
    DWORD needed = 0;
    if (EnumProcessModules(hProcess, modules, sizeof(modules), &needed)) {
        baseAddress = (uintptr_t)modules[0];
    }
    
    if (baseAddress == 0) {
        std::cout << "\033[38;2;255;50;50m  [!] Error: Could not find base address of FiveM process!\033[0m\n";
        CloseHandle(hProcess);
        WaitForExit();
        return 1;
    }

    std::cout << "\033[38;2;0;255;255m  [\033[38;2;255;255;255m+\033[38;2;0;255;255m] \033[38;2;255;255;255mSuccessfully attached. Press Enter to dump...\033[0m";
    std::cin.get();

    DumpOffsets(hProcess, baseAddress);

    CloseHandle(hProcess);

    WaitForExit();

    return 0;
}