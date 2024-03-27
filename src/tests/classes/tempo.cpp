#include "ball_contact_count.h"
#include "tempo.h"
#include <iostream>
#include <chrono>

// Define a function to calculate and display the playing tempo
void tempo::playingTempo(const std::chrono::steady_clock::duration &elapsedTime) {
    // Calculate tempo based on the time difference between ball contacts
    double tempo = 60.0 / std::chrono::duration<double>(elapsedTime).count(); // Beats per minute calculation
    std::cout << "Tempo(Game Rhythm): " << tempo << " BPM" << std::endl;
}