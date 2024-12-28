# Keylogger Program 🔑

### **Project Description** 📜

This project implements a simple keylogger using C and the Windows API.  It monitors keyboard input and logs each pressed key to a text file named "keystrokes.txt". While this can be useful for debugging or specific application testing, it's crucial to understand the ethical implications of keyloggers and use them responsibly.  Unauthorized use is illegal and unethical.

### **Installation Instructions** 💾

1. **Save the Code:** Save the provided C code as a `.c` file (e.g., `keylogger.c`).
2. **Compile:** You'll need a C compiler (like MinGW or Visual Studio) installed on your Windows machine. Open a command prompt or terminal and navigate to the directory where you saved the file.  Then, compile the code using a command similar to this (adjust the compiler command if necessary):
   ```bash
   gcc keylogger.c -o keylogger.exe -lw32
   ```
   *This assumes you are using GCC.  For Visual Studio, you will need to create a project and add the source file.*
3. **Run:** Execute the compiled program (`keylogger.exe`) from the command prompt.


### **Features** ✨

* **Keystroke Logging:** Captures and logs all pressed keys to a text file.
* **Continuous Monitoring:** Runs continuously in the background until manually stopped (e.g., by closing the command prompt window).
* **Simple Implementation:**  Relatively straightforward code for understanding basic keylogging mechanisms.


### **Usage Instructions** ▶️

1. **Run the executable:** Execute `keylogger.exe` from your command prompt.
2. **Keystrokes are logged:**  The program will start recording keystrokes immediately.
3. **View logged keys:** Open the `keystrokes.txt` file to see the recorded keystrokes.  **Note:** This file will be created in the same directory as the executable.
4. **Stop logging:** Close the command prompt window to stop the keylogger.


### **Dependencies** 🔧

* **C Compiler:**  A C compiler (like MinGW, GCC, or Visual Studio) is required to compile the source code.
* **Windows API:** The code uses the Windows API (`windows.h`), so it will only work on Windows operating systems.


### **Commands** ⚙️

```bash
# Compilation (using GCC):
gcc keylogger.c -o keylogger.exe -lw32

# Execution:
keylogger.exe
```


### **Contributing** 🤝

Feel free to open a pull request or suggest improvements!  However, please remember that this is a basic example and lacks many features of a robust keylogger (e.g., stealth mode, handling special keys).  Enhancements should focus on making the code more robust and improving its security features (if used for legitimate testing purposes).  Adding more advanced features should carefully consider ethical implications.


### **Developer Signature** ✍️

Developer: **Sunny** | GitHub: **[JustM3Sunny](https://github.com/JustM3Sunny)**
