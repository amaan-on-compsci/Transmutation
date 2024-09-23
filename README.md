# Transmutation - A GUI for Image Processing and Transformation

Transmutation is a GUI-based application that enables users to load, manipulate, and visualize TIFF images. It offers a variety of image transformation techniques including grayscale conversion, auto-level adjustment, ordered dithering, and gamma correction. The app is built using wxWidgets for the GUI, and supports easy-to-use navigation through image manipulation steps.


Key Features:
Grayscale Conversion: Converts color images to grayscale with optional gamma correction for fine-tuned brightness control.

Ordered Dithering: Applies a dithering algorithm to the grayscale image for an artistic or more pronounced grayscale effect.

Auto-Level Adjustment: Automatically adjusts image brightness and contrast by stretching the image’s intensity values across the full color range.

Gamma Correction: Provides dynamic control over the brightness of both the original and grayscale images.


Technologies Used:
C++: Core programming language for the application logic.

wxWidgets: Cross-platform GUI framework used for rendering images, managing the user interface, and handling events.

CMake: Build system to manage dependencies and create executables.

TIFF Image Support: wxWidgets allows for loading and manipulation of TIFF files.


# How to Build and Run (Atleast how I remember I did it last I tried):

Clone the repository, initialize submodules using "git submodule update --init --recursive" and then create a build directory and use CMake to compile the project:

mkdir build
cd build
cmake ..
cmake --build . --config Debug

Then run using ./exe_name

I will update this in the future for more functionality, God willing.
