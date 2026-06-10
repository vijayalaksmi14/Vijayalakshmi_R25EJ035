# 2D Graphics Editor

A terminal-based 2D graphics editor using ncurses library.

## Features

- **Draw Rectangle**: Draw rectangles on the canvas
- **Draw Line**: Draw lines using Bresenham's line algorithm
- **Draw Triangle**: Draw triangles by connecting three points
- **Draw Circle**: Draw circles using Midpoint Circle algorithm
- **Delete Rectangle**: Erase rectangles from the canvas
- **Display Picture**: View the current canvas
- **Clear Canvas**: Clear all drawings
- **Exit**: Quit the application

## Requirements

- C++ compiler (g++, clang, etc.)
- ncurses library development files

### Installing Dependencies

**Ubuntu/Debian:**
```bash
sudo apt-get install libncurses-dev
```

**macOS (with Homebrew):**
```bash
brew install ncurses
```

**Fedora/RHEL:**
```bash
sudo dnf install ncurses-devel
```

## Compilation

### Using Makefile

```bash
make
```

### Manual Compilation

```bash
g++ -Wall -Wextra -std=c++11 -o graphics_editor graphics_editor.cpp -lncurses
```

## Running the Application

### Using Makefile

```bash
make run
```

### Direct Execution

```bash
./graphics_editor
```

## Usage

1. Start the application
2. Select an option from the menu:
   - **1**: Draw Rectangle - Enter top-left coordinates (x, y) and width, height
   - **2**: Draw Line - Enter two endpoints (x1, y1, x2, y2)
   - **3**: Draw Triangle - Enter three vertices (x1, y1), (x2, y2), (x3, y3)
   - **4**: Draw Circle - Enter center (xc, yc) and radius (r)
   - **5**: Delete Rectangle - Enter rectangle coordinates to erase
   - **6**: Display Picture - View the current canvas
   - **7**: Clear Canvas - Erase all drawings
   - **8**: Exit - Quit the application

## Canvas Specifications

- **Rows**: 25
- **Columns**: 50
- **Background Character**: `_`
- **Drawing Character**: `*`

## Algorithms Used

### Bresenham's Line Algorithm
Used for efficient line drawing with minimal artifacts.

### Midpoint Circle Algorithm
Used for efficient circle drawing with symmetric points.

## Notes

- The canvas uses a character-based display in the terminal
- All coordinates must be within bounds (0-24 for rows, 0-49 for columns)
- Press any key after viewing the canvas to return to the menu

## Cleaning Up

To remove compiled files:

```bash
make clean
```

## License

This project is part of an ACP mini project.
