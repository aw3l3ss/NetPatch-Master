# NetPatch-Master
Network-Wide Patch Management System

## Overview 
NetPatch-Master is an application written in C++, thanks to which you can check your server for vulnerabilities in apt packages.
 
## Key Features 
1. The application is written in C++ using the Dear ImGui graphics library and GLFW - an OpenGL library.
2. You can quickly check installed apt packages for vulnerabilities on several servers and get a complete list with a detailed description and severity level.
3. Connection to the server occurs via SSH; you just need to enter the server address, username (root), password and open port for connection.
4. The sqlite3 database is used for work.

## How to compile, build and launch project
```
git clone https://github.com/aw3l3ss/NetPatch-Master.git
cd NetPatch-Master
mkdir build
cd build
sudo apt install libglu-dev
sudo apt install libglfw-dev
sudo apt install libssh-dev
cmake ..
make
./NetPatch-Master.o
```

I don't recommend to launch the project not in the build directory, because it could work incorrect!

## Libraries used
```
libssh
glad
glfw
imgui
sqlite3
```