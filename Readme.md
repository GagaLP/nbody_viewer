# NBody Viewer

This is a simple viewer for the NBody simulation. 
It is written in C++ and uses Raylib (OpenGL as backend) for rendering.

## Build

### Linux
Then you can build the project with the following commands:

    mkdir build
    cd build
    cmake ..
    make

### MacOS
Then you can build the project with the following commands:

    mkdir build
    cd build
    cmake ..
    make

### Windows 
Not officially supported, and tested

## Run
To run the program, you can use the following command:

    ./nbody_viewer <path_to_data_file>

where `<path_to_data_file>` is the path to the data file generated by an NBody simulation.
The default path is `./output.txt`.

## Data file format

The data file is a text file with the following format:

    <position_1_x> <position_1_y> <position_1_z> 
    <position_2_x> <position_2_y> <position_2_z> 
    ...
    <position_n_x> <position_n_y> <position_n_z> 
    

    <position_1_x> <position_1_y> <position_1_z> 
    <position_2_x> <position_2_y> <position_2_z> 
    ...
    <position_n_x> <position_n_y> <position_n_z> 


    ...
    
where `<position_i_xyz>` are the coordinates of the i-th particle at the current time step. 
The different time steps are separated by two empty lines.

## Controls

The following controls are available:

- `W`/`S`/`A`/`D`/`C`/`E`: Move the camera
- `R`/`F`: Movement speed
- `ESC`: Exit the program
- `P`: Debug mode
- `Q`: Toggle pause mode

## Screenshots

![Screenshot 1](./screenshots/screenshot_1.png)
![Screenshot 1](./screenshots/screenshot_2.png)
