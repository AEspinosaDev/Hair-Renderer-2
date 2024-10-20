
cmake_minimum_required(VERSION 3.16)

project(HairViewer VERSION 1.0.0)

set(CMAKE_GENERATOR_PLATFORM x64)
# Use C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON) #DOES NOT WORK WITH MSVC


find_package(Vulkan REQUIRED)
if(NOT Vulkan_FOUND)
    message(FATAL_ERROR "Vulkan SDK not found. Please install Vulkan SDK and ensure it's in your CMake search path.")
endif()
get_filename_component(VULKAN_SDK_ROOT ${Vulkan_LIBRARY} DIRECTORY)


set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(ext/Vulkan-Engine)
link_directories(${VULKAN_SDK_ROOT})


file(GLOB APP_SOURCES "src/*.cpp")
file(GLOB APP_HEADERS "src/*.h")
add_executable(HairViewer ${APP_SOURCES} ${APP_HEADERS})

target_link_libraries(HairViewer PRIVATE VulkanEngine)

target_compile_definitions(HairViewer PUBLIC RESOURCES_PATH="${CMAKE_CURRENT_SOURCE_DIR}/resources/")



