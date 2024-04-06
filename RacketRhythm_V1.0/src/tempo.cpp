#include "ball_contact_count.h"
#include "tempo.h"
#include <iostream>   // For input/output stream commands
#include <chrono>     // For time-related operations, we are using it for measuring durations between ball contacts.
#include <sys/ipc.h>  // To initiate inter-process communication (IPC) and shared memory management.
#include <sys/shm.h>

// Define a function to calculate and display the playing tempo
void tempo::playingTempo(const std::chrono::steady_clock::duration &elapsedTime) {
    // Calculate tempo based on the time difference between ball contacts
    double tempoValue = 60.0 / std::chrono::duration<double>(elapsedTime).count(); // Beats per minute calculation
    std::cout << "Tempo(Game Rhythm): " << tempoValue << " BPM" << std::endl;

    // Write tempoValue to shared memory so you can use it later
    key_t key = ftok("/tmp", 'a');
    if (key == -1) {
        std::cerr << "ftok failed" << std::endl;
        exit(1);
    }

    int shared_memory_id = shmget(key, sizeof(double), IPC_CREAT | 0666);
    if (shared_memory_id == -1) {
        std::cerr << "shmget failed" << std::endl;
        exit(1);
    }

    double* shared_memory = (double*)shmat(shared_memory_id, nullptr, 0);
    if (shared_memory == (void*)-1) {
        std::cerr << "shmat failed" << std::endl;
        exit(1);
    }

    *shared_memory = tempoValue;

    shmdt(shared_memory);
} /* tempo.cpp */