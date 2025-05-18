# Sierpinski's Tetrahedron Fractals
## Made with <3 and GLFW

Two Sierpinski's Tetrahedron variants rendered in OpenGL.
I made this one into it's own repo, go check it out!!!!

## About

These fractals are something I stumbled on while experimenting with the Koch Tetrahedron. One is a 3D version of a Sierpinski's triangle and the other is the inverse of the 3D Sierpinski's Triangle.

## How to swap between fractals:

There is no GUI (yet...) so you have to go to line 280 of main.cpp and comment out the four lines under "//Sierpinski" and uncomment the lines under "//Inverse Sierpinski" to swap to the Inverse Sierpinski. Also, I urge you to mess around with maxDepth, the colors, and frankly whatever you want to see if you can make something that looks cool. I like the Inverse Sierpinski on low maxDepth values (3-5) and the normal Sierpinski on however high a maxDepth your computer can handle.

## Requirements

- OpenGL (development libraries)
- GLFW
- C++ compiler (e.g., g++)
- CMake or Make (depending on your build system)

## How to Run

1. Build the project (see your build instructions or Makefile).
2. Run the executable:
   ```sh
   ./koch_tetrahedron
   ```