# SCALES Demo Development

This demo combines the F Prime Hub Pattern developed by the SCALES team with COTS evaluation boards for the Flight Computer and Edge Computer. The demo aims to accomplish the following:

- Test the capability of the split computing architecture
- Test the capability of using a network ethernet switch in this architecture
- Test the command and data handling aspects of the Flight Computer
- Test using the Hub Pattern to have the Flight Computer command the Edge Computer

## Setup

The i.MX8X Flight Computer evaluation board, Jetson Edge Computer evaluation board, and COTS ethernet camera will be connected through a COTS managed ethernet switch. The i.MX will command the Jetson through the Hub Pattern to take a picture using the Ethernet Camera. The picture will be saved on the Jetson. When completed, the i.MX will command the Jetson through the Hub Pattern to run a computer vision algorithm on the images taken with the camera. The results will be sent to the i.MX.

## Process

Using a pre-existing F Prime deployment, we created a new component called RunLucidCamera. The component achieves the following:

- GDS command to set up the camera and make sure it is connected
- GDS command to take a picture and save an image from the ethernet camera

We recycled and modified existing example code from the ethernet camera's SDK. To do this, we needed to integrate the Arena SDK for the ethernet camera into the F Prime deployment. 

### Arena SDK in F Prime

The Arena SDK was installed from [Lucid](https://thinklucid.com/downloads-hub/)

1. Create a `Libs` folder in the root directory of the project.
2. In `scales-software/project.cmake` add the `Libs` folder as a subdirectory.
3. Copy the Arena SDK folders into `Libs` at `scales-software/Libs/ArenaSDK`
4. Make a CMakeLists.txt files at `scales-software/Libs/CMakeLists/txt` and add the following: `add_fprime_subdirectory("${CMAKE_CURRENT_LIST_DIR}/ArenaSDK")`
5. Make a new CMakeLists.txt at `scales-software/Libs/ArenaSDK/CMakeLists/txt` and add the Arena SDK library paths. Currently, the only way this worked was to add the paths to the exact files we wanted. 

```
set(MODULE_NAME Libs_ArenaSDK)

add_library(${MODULE_NAME} INTERFACE)
target_include_directories(${MODULE_NAME} INTERFACE
	${CMAKE_CURRENT_LIST_DIR}/include/Arena
	${CMAKE_CURRENT_LIST_DIR}/GenICam/library/CPP/include
	${CMAKE_CURRENT_LIST_DIR}/include/Save
	${CMAKE_CURRENT_LIST_DIR}/include/GenTL
	)

target_link_libraries(${MODULE_NAME} INTERFACE
    # Your existing libraries
    Components_RunLucidCamera
    
    # Arena SDK libraries - provide full paths to .so or .a files
    ${CMAKE_CURRENT_LIST_DIR}/lib/libarena.so          # Core Arena library
    ${CMAKE_CURRENT_LIST_DIR}/lib/libsavec.so          # Save functionality
    ${CMAKE_CURRENT_LIST_DIR}/lib/libgentl.so          # GenTL functionality
    ${CMAKE_CURRENT_LIST_DIR}/lib/liblucidlog.so       # Lucid logging
	${CMAKE_CURRENT_LIST_DIR}/lib/libsave.so
	${CMAKE_CURRENT_LIST_DIR}/lib/libarenac.so

	${CMAKE_CURRENT_LIST_DIR}/ffmpeg/libavcodec.so
	${CMAKE_CURRENT_LIST_DIR}/ffmpeg/libavformat.so
	${CMAKE_CURRENT_LIST_DIR}/ffmpeg/libavutil.so
	${CMAKE_CURRENT_LIST_DIR}/ffmpeg/libswresample.so


    # GenICam libraries
    ${CMAKE_CURRENT_LIST_DIR}/GenICam/library/lib/Linux64_ARM/libGenApi_gcc54_v3_3_LUCID.so
    ${CMAKE_CURRENT_LIST_DIR}/GenICam/library/lib/Linux64_ARM/libGCBase_gcc54_v3_3_LUCID.so
    ${CMAKE_CURRENT_LIST_DIR}/GenICam/library/lib/Linux64_ARM/libMathParser_gcc54_v3_3_LUCID.so
    ${CMAKE_CURRENT_LIST_DIR}/GenICam/library/lib/Linux64_ARM/liblog4cpp_gcc54_v3_3_LUCID.so
    ${CMAKE_CURRENT_LIST_DIR}/GenICam/library/lib/Linux64_ARM/libLog_gcc54_v3_3_LUCID.so
    ${CMAKE_CURRENT_LIST_DIR}/GenICam/library/lib/Linux64_ARM/libNodeMapData_gcc54_v3_3_LUCID.so
    ${CMAKE_CURRENT_LIST_DIR}/GenICam/library/lib/Linux64_ARM/libXmlParser_gcc54_v3_3_LUCID.so
)
```

6. In `scales-software/Components/RunLucidCamera/RunLucidCamera.cpp`, update the include path to `#include "ArenaApi.h"` and `#include SaveApi.h`, and integrate the `CppSave_Png.cpp` example code from the Arena SDK into the `RunLucidCamera.cpp`. The constructor and deconstructor for the component was modified to include parts of the Arena SDK example code. We also modified the way the example code was saving the images so the old images don't get overwritten. The current code is [here](https://github.com/BroncoSpace-Lab/scales-software/blob/kdizzy-ArenaWorking/Components/RunLucidCamera/RunLucidCamera.cpp). 



## Progress