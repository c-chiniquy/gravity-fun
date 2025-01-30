# Gravity Fun

## About the project

Gravity Fun is an interactive 2D n-body simulator.
This is a personal programming project I created just for fun.
The rendering abstraction layer [iglo](https://github.com/c-chiniquy/iglo) is used, with D3D12 serving as the rendering backend.

## Features

- Modern CS_6_6 compute shaders.
- Multiple premade scenes to choose from.
- All simulation parameters are modifiable.
- Supports two particle types
  - Mass particles: Has mass and can affect other particles.
  - Visual particles: Cannot affect other particles, but has less computational time complexity.
- Supports recording simulations as .PNG image sequences.
- Multiple rendering methods to choose from.
- You can interact with the particles using your cursor.

## Gallery

![](images/20k-galaxy.png)
![](images/50-million-particle-galaxy-collision.png)
![](images/black-hole-forms.gif)
![](images/circle-galaxies-collide.gif)
![](images/galaxies-collide.gif)
![](images/merging-clusters.png)
![](images/nbody-heatmap.png)

## Build

### Minimum requirements

- Windows 10 (version 1909 or later)
- A shader model 6.6 capable graphics card and graphics drivers
- CMake 3.22 or higher

### Windows

Follow these steps to build this project on Windows.

1. Download [nuget.exe](https://learn.microsoft.com/en-us/nuget/install-nuget-client-tools?tabs=windows#nugetexe-cli).

2. Add `nuget.exe` to your system's environment PATH.

3. Run `build.cmd` to generate Visual Studio 2019 project files using CMake. The generated project files will appear in a folder named `build`.
   - **Note**: An internet connection is required during this step for NuGet to download Agility SDK (required for D3D12). The Agility SDK package will also be placed in the `build` folder.
   - **Other IDEs**: The `build.cmd` script builds for Visual Studio 2019. If you want to build for another IDE, run CMake commands manually to generate project files for your chosen IDE. Note that the compatibility with IDEs other than Visual Studio is not guaranteed, as this is the only IDE tested so far.

4. After the script completes, it will automatically open the Visual Studio solution file. From there, you can build and run the project.

## Third Party Libraries

- [iglo](https://github.com/c-chiniquy/iglo)
- [ImGui](https://github.com/ocornut/imgui)
