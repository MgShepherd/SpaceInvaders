# SpaceInvaders

A Space Invaders clone written in C using the raylib library.

## Building the Project

This project is built using `make` and dynamically links with raylib in order to work.

Since we dynamically link with raylib, you will need to have raylib installed locally in order for this to run properly.

The c compiler used currently is `clang` and the library files will currently be looked for in the `/opt/homebrew` directory, but both of these properties will be made configurable in future.

Assuming you have raylib located in the required directory, you should then simply be able to run:
```
make
```

If this works, this should create a `build` directory in the root of the project with all the required files, and the executable can then be run using:
```
./build/SpaceInvaders
```
