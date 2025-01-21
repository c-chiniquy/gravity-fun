cmake_minimum_required(VERSION 3.22)

set(name "gravity-fun")
project(${name} LANGUAGES CXX)

# requires C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# set desired agility SDK version here
set(AGILITY_SDK_VERSION "1.715.1-preview")

# project
set(PROJECT_SOURCES
    ${CMAKE_SOURCE_DIR}/src/Main.h
    ${CMAKE_SOURCE_DIR}/src/Main.cpp
    ${CMAKE_SOURCE_DIR}/src/ShaderCommon.h
    ${CMAKE_SOURCE_DIR}/src/ColorGradient.h
    ${CMAKE_SOURCE_DIR}/src/ColorGradient.cpp
    ${CMAKE_SOURCE_DIR}/src/GenerateParticles.h
    ${CMAKE_SOURCE_DIR}/src/GenerateParticles.cpp
    ${CMAKE_SOURCE_DIR}/src/ParticleRenderer.h
    ${CMAKE_SOURCE_DIR}/src/ParticleRenderer.cpp
    ${CMAKE_SOURCE_DIR}/src/Scenarios.h
    ${CMAKE_SOURCE_DIR}/src/Scenarios.cpp
    ${CMAKE_SOURCE_DIR}/src/Universe.h
    ${CMAKE_SOURCE_DIR}/src/Universe.cpp
)

# iglo
set(IGLO_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/extern/iglo-0.1.0")
set(IGLO_SOURCES
    ${IGLO_INCLUDE_DIR}/iglo.h
    ${IGLO_INCLUDE_DIR}/iglo.cpp
    ${IGLO_INCLUDE_DIR}/igloBatchRenderer.h
    ${IGLO_INCLUDE_DIR}/igloBatchRenderer.cpp
    ${IGLO_INCLUDE_DIR}/igloCamera.h
    ${IGLO_INCLUDE_DIR}/igloCamera.cpp
    ${IGLO_INCLUDE_DIR}/igloFont.h
    ${IGLO_INCLUDE_DIR}/igloFont.cpp
    ${IGLO_INCLUDE_DIR}/igloMainLoop.h
    ${IGLO_INCLUDE_DIR}/igloMainLoop.cpp
    ${IGLO_INCLUDE_DIR}/igloUtility.h
    ${IGLO_INCLUDE_DIR}/igloUtility.cpp
    ${IGLO_INCLUDE_DIR}/igloConfig.h
)

# imgui
set(IMGUI_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/extern/imgui-1.90.4")
set(IMGUI_SOURCES
    ${IMGUI_INCLUDE_DIR}/imconfig.h
    ${IMGUI_INCLUDE_DIR}/imgui.cpp
    ${IMGUI_INCLUDE_DIR}/imgui.h
    ${IMGUI_INCLUDE_DIR}/imgui_demo.cpp
    ${IMGUI_INCLUDE_DIR}/imgui_draw.cpp
    ${IMGUI_INCLUDE_DIR}/imgui_internal.h
    ${IMGUI_INCLUDE_DIR}/imgui_tables.cpp
    ${IMGUI_INCLUDE_DIR}/imgui_widgets.cpp
    ${IMGUI_INCLUDE_DIR}/backends/imgui_impl_win32.h
    ${IMGUI_INCLUDE_DIR}/backends/imgui_impl_win32.cpp
    ${IMGUI_INCLUDE_DIR}/backends/imgui_impl_dx12.h
    ${IMGUI_INCLUDE_DIR}/backends/imgui_impl_dx12.cpp
)

# NuGet is required
find_program(NUGET_EXECUTABLE nuget.exe REQUIRED)
if (NOT NUGET_EXECUTABLE)
    message(FATAL_ERROR "nuget.exe is required but not found. Please ensure it is in the PATH.")
endif()

# download AgilitySDK
set(AGILITY_SDK_PACKAGE_DIR "${CMAKE_BINARY_DIR}/packages")
if (NOT EXISTS "${AGILITY_SDK_PACKAGE_DIR}")
    execute_process(
        COMMAND ${NUGET_EXECUTABLE} install Microsoft.Direct3D.D3D12 -Version ${AGILITY_SDK_VERSION} -Prerelease -OutputDirectory ${AGILITY_SDK_PACKAGE_DIR}
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    )
endif()

# see if it succeeded
set(AGILITY_SDK_PATH "${AGILITY_SDK_PACKAGE_DIR}/Microsoft.Direct3D.D3D12.${AGILITY_SDK_VERSION}")
if (NOT EXISTS "${AGILITY_SDK_PATH}")
    message(FATAL_ERROR "Failed to download Agility SDK. Ensure nuget.exe is correctly configured.")
endif()

# Agility SDK files that will be copied to D3D12 folder
set(AGILITY_SDK_DLLS
    "${AGILITY_SDK_PATH}/build/native/bin/x64/D3D12Core.dll"
    "${AGILITY_SDK_PATH}/build/native/bin/x64/D3D12Core.pdb"
    "${AGILITY_SDK_PATH}/build/native/bin/x64/D3D12SDKLayers.dll"
    "${AGILITY_SDK_PATH}/build/native/bin/x64/D3D12SDKLayers.pdb"
    "${AGILITY_SDK_PATH}/build/native/bin/x64/DirectSR.dll"
    "${AGILITY_SDK_PATH}/build/native/bin/x64/DirectSR.pdb"
)

if(MSVC)
    # set warning level
    add_compile_options(/W3 /WX)
endif()

# source files
add_executable(${name} ${PROJECT_SOURCES} ${IGLO_SOURCES} ${IMGUI_SOURCES})

if(MSVC)
    # set linker subsystem
    target_link_options(${name} PRIVATE "/SUBSYSTEM:WINDOWS")
    # no filter for project source files
    source_group(" " FILES ${PROJECT_SOURCES})
    # include Agility SDK props
    set_target_properties(${name} PROPERTIES VS_USER_PROPS ${AGILITY_SDK_PATH}/build/native/Microsoft.Direct3D.D3D12.props)
endif()

# includes
target_include_directories(${name} PRIVATE 
    ${IGLO_INCLUDE_DIR}
    ${IMGUI_INCLUDE_DIR}
    ${AGILITY_SDK_PATH}/build/native/include
)

# filters
source_group("iglo" FILES ${IGLO_SOURCES})
source_group("imgui" FILES ${IMGUI_SOURCES})

# create D3D12 folder
add_custom_command(
    TARGET ${name} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:${name}>/D3D12
)

# copy D3D12 files
foreach(d3d12file ${AGILITY_SDK_DLLS})
    add_custom_command(
        TARGET ${name} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${d3d12file} $<TARGET_FILE_DIR:${name}>/D3D12
    )
endforeach()

# copy shaders
add_custom_command(
    TARGET ${name} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/shaders/compiled $<TARGET_FILE_DIR:${name}>/shaders
)

# set executable output directory
# and set working dir to 'name/Debug' or 'name/Release' whichever is active.
set_target_properties(${name} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${name}
    VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/${name}/${CMAKE_CFG_INTDIR}
)
