# Create the executable binary
add_executable(bl_app main.cpp)

# Link to our core library.
# This pulls in the code AND the include paths
target_link_libraries(bl_app PRIVATE bl_core)
