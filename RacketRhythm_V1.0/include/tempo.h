#ifndef TEMPO_H
#define TEMPO_H

#include <chrono> // For time-related operations, we are using it for measuring durations between ball contacts.

class tempo {
public:
void playingTempo(const std::chrono::steady_clock::duration &elapsedTime);

};

#endif /* TEMPO_H */
