#ifndef TEMPO_H
#define TEMPO_H

#include <chrono>

class tempo {
public:
void playingTempo(const std::chrono::steady_clock::duration &elapsedTime);

};

#endif /* TEMPO_H */
