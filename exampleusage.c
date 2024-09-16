#include <windows.h>
#include <stdio.h>

// Define the function pointer type for the DSOD function
typedef void (*DSODFunc)();

int main() {
    HINSTANCE hDLL;
    DSODFunc DSOD;

    // Load the DLL
    hDLL = LoadLibrary(TEXT("dllDSOD.dll"));
    if (hDLL == NULL) {
        printf("Failed to load DLL: %lu\n", GetLastError());
        return 1;
    }

    // Get the address of the DSOD function
    DSOD = (DSODFunc)GetProcAddress(hDLL, "DSOD");
    if (DSOD == NULL) {
        printf("Failed to find DSOD function: %lu\n", GetLastError());
        FreeLibrary(hDLL);
        return 1;
    }

    // Call the DSOD function
    printf("Calling DSOD function...\n");
    DSOD();

    // Unload the DLL
    FreeLibrary(hDLL);

    return 0;
}