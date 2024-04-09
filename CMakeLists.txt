cmake_minimum_required(VERSION 3.25.1)
project(RacketRhythm_V1.0)

# Find necessary libraries
set(PORTAUDIO_DIR "/usr/lib/")
set(FFTW3_DIR "/usr/lib/arm-linux-gnueabihf/")

#find_library(FFTW3_LIBRARIES fftw3 PATHS ${FFTW3_DIR})
LINK_DIRECTORIES(/usr/lib/arm-linux-gnueabihf/)

# Add executable target
add_executable(${PROJECT_NAME}
                RacketRhythm_V1.0/src/main.cpp
                RacketRhythm_V1.0/src/contact_detector.cpp
                RacketRhythm_V1.0/src/ALSACapture.cpp
                RacketRhythm_V1.0/src/ball_contact_count.cpp
                RacketRhythm_V1.0/src/tempo.cpp
    )

# Link libraries
target_link_libraries(${PROJECT_NAME} PRIVATE portaudio ${fftw3_DIR} /usr/lib/arm-linux-gnueabihf/libfftw3f.a asound)

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE ${PORTAUDIO_INCLUDE_DIRS} ${FFTW3_INCLUDE_DIRS} RacketRhythm_V1.0/include)
