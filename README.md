
# Chess Game Project (Windows Only)

## Overview

This project is a chess game with a **C++ backend** and a **JavaFX-based frontend**. The backend handles the game logic and networking, while the frontend provides a graphical user interface for players to interact with the game.

**This code is designed specifically for Windows OS.**

## Prerequisites

1. **C++ Compiler:** Ensure `g++` is installed (e.g., MinGW for Windows).
2. **Java Development Kit (JDK):** Download and install the JDK (Java 11 or higher recommended).
3. **JavaFX SDK:** Download the JavaFX SDK from the link below and unzip it:  
   [JavaFX SDK 23.0.1 (Windows)](https://download2.gluonhq.com/openjfx/23.0.1/openjfx-23.0.1_windows-x64_bin-jmods.zip)

   **Note:** Ensure to use the correct path to the `lib` folder in the JavaFX SDK when running the frontend.

## Setup and Execution (Windows Only)

### Step 1: Run the Backend

1. Open a terminal (Command Prompt or PowerShell) and navigate to the `src` directory where the `backend.cpp` file is located.
2. Compile the backend with the following command:
   ```bash
   g++ -o backend backend.cpp -lws2_32
   ```
3. Run the compiled backend:
   ```bash
   ./backend
   ```
   The terminal will display:
   ```
   Waiting for two clients to connect...
   ```

### Step 2: Run the Frontend (Player Interface)

1. Open a **new terminal**.
2. Navigate to the directory containing the frontend Java files (`ChessBoard.java`, `PieceManager.java`, `ChessTile.java`, etc.).
3. Compile the Java files using `javac`. Replace `"path_to_javafx_lib"` with the path to the `lib` folder inside the JavaFX SDK:
   ```bash
   javac --module-path "path_to_javafx_lib" --add-modules javafx.controls,javafx.fxml ChessBoard.java PieceManager.java ChessTile.java
   ```
4. Run the program:
   ```bash
   java --module-path "path_to_javafx_lib" --add-modules javafx.controls,javafx.graphics -cp bin ChessBoard
   ```
5. Enter the **IP address** of the backend when prompted.
6. Repeat **Step 2** in another terminal for the second player.

## Project Files

1. **Backend:**
   - `backend.cpp`
2. **Frontend:**
   - `ChessBoard.java`
   - `PieceManager.java`
   - `ChessTile.java`
   - `ChessLauncher.java`

## Important Links

- [Download JavaFX SDK 23.0.1 (Windows)](https://download2.gluonhq.com/openjfx/23.0.1/openjfx-23.0.1_windows-x64_bin-jmods.zip)

## Troubleshooting

1. **Compilation Errors:**
   - Ensure `g++` is added to your system's PATH.
   - Verify the correct path to the JavaFX `lib` folder is used in all `javac` and `java` commands.

2. **Connection Issues:**
   - Ensure both frontend terminals use the correct IP address of the backend.

3. **JavaFX Not Found:**
   - Check that the JavaFX SDK is correctly downloaded and extracted.
   - Use the absolute path to the JavaFX `lib` folder in your commands.

Enjoy playing chess! ♟️
