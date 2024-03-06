cmake_minimum_required(VERSION 3.25.1)
project(CONTACT_DETECTOR)

# Find necessary libraries
set(PORTAUDIO_DIR "/usr/lib/")
set(FFTW3_DIR "/usr/lib/arm-linux-gnueabihf/")

#find_library(FFTW3_LIBRARIES fftw3 PATHS ${FFTW3_DIR})
LINK_DIRECTORIES(/usr/lib/arm-linux-gnueabihf/)

# Add executable target
add_executable(CONTACT_DETECTOR main.cpp CONTACT_DETECTOR.cpp)

# Link libraries
target_link_libraries(CONTACT_DETECTOR PRIVATE portaudio ${fftw3_DIR} /usr/lib/arm-linux-gnueabihf/libfftw3f.a)
target_link_libraries(CONTACT_DETECTOR PRIVATE portaudio fftw3)

# Include directories
target_include_directories(CONTACT_DETECTOR PRIVATE ${PORTAUDIO_INCLUDE_DIRS} ${FFTW3_INCLUDE_DIRS})
