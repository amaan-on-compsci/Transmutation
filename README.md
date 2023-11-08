# ImageProcessor
Displays and manipulates TIFF files with grayscale conversion, auto-level and ordered dithering for grayscale functionality.

Developed in VSCode 2022, external dependencies (wxWidgets) added as git submodules and compiled using CMake. 
May need to run "git submodule update --init --recursive" and then create a "build" directory.
Then, inside the build directory run "cmake .." from the terminal.
Finally, run "cmake --build . --config Debug" to generate an executable. 

*This is how I personally built/ran the project (inexperienced)
