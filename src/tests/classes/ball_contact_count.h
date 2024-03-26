#ifndef BALL_CONTACT_COUNT_H
#define BALL_CONTACT_COUNT_H

#include <chrono>

extern int ballContactCount;
extern std::chrono::steady_clock::time_point lastContactTime;
extern std::chrono::steady_clock::time_point prevContactTime;

void processBallContact();

#endif /* BALL_CONTACT_COUNT_H */
