# Aizawa Attractor Visualization

This is a C program for visualizing the Aizawa attractor using OpenGL.

## Controls

- **Up Arrow**: Increase the number of points.
- **Down Arrow**: Decrease the number of points.
- **Left Arrow**: Decrease the size of points.
- **Right Arrow**: Increase the size of points.
- **r**: Change the color of the attractor (random).
- **R**: Reset to the initial color.
- **f**: Toggle fullscreen mode.
- **i**: Toggle information string.
- **q**: Exit fullscreen mode (if in fullscreen) or quit the program (if in windowed mode).
- **Escape (Esc)**: Quit the program.

## How-To

1. Ensure you have the necessary dependencies installed:
    ```sh
    sudo apt-get update
    sudo apt-get install build-essential libglu1-mesa-dev freeglut3-dev mesa-common-dev
    ```
2. Clone the repository:
    ```sh
    git clone https://github.com/0x1e9307/TheAizawaAttractor.git
    cd TheAizawaAttractor
    ```
3. Compile the program using the Makefile:
    ```sh
    make
    ```

4. Run the program:
    ```sh
    ./Aizawa
    ```
## Example

   ![Example](Aizawa.gif)
