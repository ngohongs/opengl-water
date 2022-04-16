# OpenGL Water

## Description

OpenGL Water is a sandbox displaying heightfield simulation of a water surface proposed by Matthias Fischer-Mueller [[1]](#1) using graphics API OpenGL. Additionally it has implemeted Shah's [[2]](#2) caustics formation, custom reflactions and refractions with same intersection algorithm as caustics calculations.

Apart from the water surface other objects are naively shaded with Phong's empiric model. Color of under water objects are shifted and attenuated according to the distance from the water surface.

# Usage

Point crosshair at water surface and drag mouse while holding left mouse button to displace it. To adjust parameters of the simulation switch to cursor mode by pressing E, after that you are able to use the GUI of the right panel. Press setting headers (e.g. Water simulation settings) to reveal all parameters of the section. To switch back to preview mode press E again.

# Controls
* to exit program press Esc
* to move press WSAD
* to look around move your mouse
* to displace water drag mouse while holding left mouse button
* to toggle cursor press E
* to toggle GUI press R
* to reset water to the rest state press Q"

# Dependencies
* [GLEW](http://glew.sourceforge.net/)
* [GLFW](https://www.glfw.org/)
* [GLM](https://github.com/g-truc/glm)
* [ASSIMP](https://github.com/assimp/assimp) - [License to ASSIMP](dep/assimp/LICENSE) 
* [stb](https://github.com/nothings/stb)
* [ImGui](https://github.com/ocornut/imgui)

# Executable

Executable file is precompiled for Windows 10/11 platform. For other OS you need to compile the program and link it with appropriate dependencies for your OS. Windows version of dependencies are located in [dep](dep) folder.

Executable file is located in [bin](bin) folder. Be noted that every file located in the folder is essential for running the program [opengl-water.exe](bin/opengl-water.exe).

# Compilation

For Windows open the [solution file](opengl-water.sln) choose `Release` and build by pressing `Ctrl + Shift + B`. All dependencies have been predownloaded and should link with immiediantly after compilation. 

For other OS `int WINAPI WinMain` in [src/main.cpp](src/main.cpp) should be change to `int main`. Dependencies compiled for your OS stated above need to be also installed. After that the project should be able to be compiled.  

# Resources 

[Skybox texture link](https://learnopengl.com/Advanced-OpenGL/Cubemaps)

[Rubber duck link](https://www.cgtrader.com/free-3d-models/sports/toy/rubber-duck-b31f3585-0347-4532-bd92-7ddea6107d0d)

[Water bed link (CC BY 4.0)](https://sketchfab.com/3d-models/sandground-terrain-2019-437fc27dd7ff4a36ae0cf56ffbd1dc5c)

# Bibliography

<a name="1">[1]</a> MÜLLER-FISCHER, Matthias. Fast Water Simulation for Games Using
Height Fields [online]. London, England, 2008 [cit. 2022-04-15]. Available
at: https://www.gdcvault.com/play/203/Fast-Water-Simulationfor-Games. https://archive.org/details/GDC2008Fischer.

<a name="2">[2]</a> SHAH, Musawir; KONTTINEN, Jaakko; PATTANAIK, Sumanta. Caustics Mapping: An Image-Space Technique for Real-Time Caustics. IEEE
transactions on visualization and computer graphics. 2007, roč. 13, s. 272–280. Available from doi: 10.1109/TVCG.2007.32.
