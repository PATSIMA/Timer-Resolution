#include <iostream>
#include <windows.h>
#include <tlhelp32.h>
#include <mmsystem.h>
#include <string>
#include <memory>
#pragma comment(lib, "winmm.lib")

typedef NTSTATUS(NTAPI* NtSetTimerResolutionFunc)(IN ULONG, IN BOOLEAN, OUT PULONG);
typedef NTSTATUS(NTAPI* NtQueryTimerResolutionFunc)(OUT PULONG, OUT PULONG, OUT PULONG);

class TimerResolutionManager {
public:
    TimerResolutionManager(ULONG resolution) : MaxResolution(resolution), NtSetTimerResolution(nullptr), NtQueryTimerResolution(nullptr) {}

    bool LoadNtSetTimerResolution() {
        hNtDll.reset(LoadLibraryW(L"ntdll.dll"));
        if (!hNtDll) {
            std::cerr << "Error: could not load ntdll.dll\n";
            return false;
        }

        NtSetTimerResolution = (NtSetTimerResolutionFunc)GetProcAddress(hNtDll.get(), "NtSetTimerResolution");
        NtQueryTimerResolution = (NtQueryTimerResolutionFunc)GetProcAddress(hNtDll.get(), "NtQueryTimerResolution");

        if (!NtSetTimerResolution || !NtQueryTimerResolution) {
            std::cerr << "Error: could not get the address of NtSetTimerResolution or NtQueryTimerResolution\n";
            return false;
        }
        return true;
    }

    void ActivateTimerResolution() {
        if (!NtSetTimerResolution) {
            std::cerr << "Error: NtSetTimerResolution is not available.\n";
            return;
        }

        ULONG CurrentResolution = 0, MinResolution = 0, MaxResolution = 0;
        NtQueryTimerResolution(&MinResolution, &MaxResolution, &CurrentResolution);

        std::wcout << L"Current timer resolution: " << CurrentResolution / 10000.0 << L" ms\n";

        NTSTATUS status = NtSetTimerResolution(MaxResolution, TRUE, &CurrentResolution);
        if (status == 0) {
            std::wcout << L"Timer Resolution activated to: " << CurrentResolution / 10000.0 << L" ms\n";
        }
        else {
            std::wcerr << L"Error activating Timer Resolution. Status code: " << status << L"\n";
        }
    }

    void DeactivateTimerResolution() {
        if (!NtSetTimerResolution) {
            std::cerr << "Error: NtSetTimerResolution is not available.\n";
            return;
        }

        ULONG CurrentResolution = 0;
        NTSTATUS status = NtSetTimerResolution(MaxResolution, FALSE, &CurrentResolution);
        if (status == 0) {
            std::wcout << L"Timer Resolution deactivated, current resolution: " << CurrentResolution / 10000.0 << L" ms\n";
        }
        else {
            std::wcerr << L"Error deactivating Timer Resolution. Status code: " << status << L"\n";
        }
    }

    bool IsProcessRunning(const std::wstring& processName) {
        HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
        if (snapshot == INVALID_HANDLE_VALUE) {
            return false;
        }

        PROCESSENTRY32 processEntry;
        processEntry.dwSize = sizeof(PROCESSENTRY32);

        if (Process32First(snapshot, &processEntry)) {
            do {
                if (processName == processEntry.szExeFile) {
                    CloseHandle(snapshot);
                    return true;
                }
            } while (Process32Next(snapshot, &processEntry));
        }

        CloseHandle(snapshot);
        return false;
    }

    void MonitorProcess(const std::wstring& processName) {
        while (true) {
            if (IsProcessRunning(processName)) {
                ActivateTimerResolution();
                std::wcout << processName << L" is running. Applying Timer Resolution.\n";
                while (IsProcessRunning(processName)) {
                    Sleep(2000);  // Check every 2 seconds
                }
                std::wcout << L"The process " << processName << L" has terminated.\n";
                DeactivateTimerResolution();
                break;  // Exit the loop when the process terminates
            }
            else {
                Sleep(2000);  // Check every 2 seconds
            }
        }
    }

private:
    ULONG MaxResolution;
    NtSetTimerResolutionFunc NtSetTimerResolution;
    NtQueryTimerResolutionFunc NtQueryTimerResolution;
    std::unique_ptr<std::remove_pointer<HMODULE>::type, decltype(&FreeLibrary)> hNtDll{ nullptr, FreeLibrary };
};

int main() {
    std::wcout << L"Created by Patsima - Version 1.0.0\n";

    TimerResolutionManager timerManager(5000);  // Resolution of 0.5 ms

    if (!timerManager.LoadNtSetTimerResolution()) {
        std::cerr << "Could not load NtSetTimerResolution.\n";
        return -1;
    }

    std::wstring targetProcessName;
    std::wcout << L"Please enter the process name (including the .exe extension): ";
    std::getline(std::wcin, targetProcessName);

    if (targetProcessName.empty()) {
        std::cerr << "No process name entered. Exiting...\n";
        return -1;
    }

    std::wcout << L"Monitoring process: " << targetProcessName << L"\n";
    timerManager.MonitorProcess(targetProcessName);

    // Wait for user input before exiting
    std::wcout << L"Press any key to exit...";
    std::wcin.get();

    return 0;
}
