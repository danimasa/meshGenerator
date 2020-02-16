## Building in Windows

To get started:
```
> git clone https://github.com/Microsoft/vcpkg.git
> cd vcpkg

PS> .\bootstrap-vcpkg.bat
```

Then install de required packages:
```
PS> .\vcpkg install --triplet x64-windows clapack armadillo cgal
```

Create the build folder and run cmake with the following options:
```
> mkdir build
> cd build
> cmake .. "-DCMAKE_TOOLCHAIN_FILE=[vcpkg_directory]\scripts\buildsystems\vcpkg.cmake" "-DVCPKG_TARGET_TRIPLET=x64-windows"
```