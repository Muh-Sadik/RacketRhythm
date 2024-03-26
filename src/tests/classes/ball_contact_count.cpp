#include "ball_contact_count.h"
#include "tempo.h"
#include <iostream>

int ballContactCount = 0;

// Define variables to store the times of the last two ball contacts
std::chrono::steady_clock::time_point lastContactTime;
std::chrono::steady_clock::time_point prevContactTime;

// Function to handle ball contact event
void processBallContact() {
    auto currentTime = std::chrono::steady_clock::now();    // Get the current time
    auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - prevContactTime).count();   // elapsedTime converted to miliseconds
if (elapsedTime >= 500) {  // Ensure at least 500 milliseconds between detections
            ++ballContactCount;     // Increment the ball contact count
            std::cout << "Sound pulse detected!" << std::endl;
        }

    // Calculate tempo if there have been at least two ball contacts
    if (ballContactCount == 2) {
    // Calculate time difference from the last contact
        auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - prevContactTime);   // elapsedTime converted to miliseconds
        std::cout << "Time between contacts: " << elapsedTime.count() << " miliseconds" << std::endl;
        playingTempo(elapsedTime);
        ballContactCount =0 ;
    }
    // Update variables for next contact
    prevContactTime = lastContactTime;
    lastContactTime = currentTime;
}