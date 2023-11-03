Some of these files use Image Magick. To install it do:
```
sudo apt-get install -y imagemagick libmagick++-dev 
```
 The cmake stuff for those things is as follows:

```
############# IMAGE MAGICK STUFF #################################
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wfatal-errors")
add_definitions( -DMAGICKCORE_QUANTUM_DEPTH=16 )
add_definitions( -DMAGICKCORE_HDRI_ENABLE=0 )
find_package(ImageMagick COMPONENTS Magick++)
include_directories(${ImageMagick_INCLUDE_DIRS})
target_link_libraries(HighSpeedAlliedVision PRIVATE ${ImageMagick_LIBRARIES})
##################################################################
```

Some also use the Vimba X library. To install it do:

```
git clone git@github.com:DaleGia/AlliedVisionVimbaX.git
cd AlliedVisionVimbaX
sh install.sh
```
 The cmake stuff for this is something like this:

```
list(APPEND CMAKE_PREFIX_PATH "/usr/local/lib/VimbaX/api")
list(APPEND CMAKE_PREFIX_PATH "/usr/local/lib/VimbaX/api/include/VmbCPP")
list(APPEND CMAKE_PREFIX_PATH "/usr/local/lib/VimbaX/api/include/VmbImageTransform")
list(APPEND CMAKE_PREFIX_PATH "/usr/local/lib/VimbaX/lib")
list(APPEND CMAKE_PREFIX_PATH "/usr/local/lib/VimbaX/lib/cmake")

if(EXISTS "/usr/local/lib/VimbaX/cmake/vmb_cmake_prefix_paths.cmake")
    # read hardcoded package location information, if the example is still located in the original install location
    include("/usr/local/lib/VimbaX/cmake/vmb_cmake_prefix_paths.cmake")
endif()

find_package(Vmb REQUIRED COMPONENTS CPP)
find_package(Vmb REQUIRED COMPONENTS ImageTransform)
```