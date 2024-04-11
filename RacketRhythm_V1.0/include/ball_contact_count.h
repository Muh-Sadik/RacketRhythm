#ifndef BALL_CONTACT_COUNT_H
#define BALL_CONTACT_COUNT_H

#include <chrono> // For time-related operations, we are using it for measuring durations between ball contacts.

class BallContactCount {
public:
    void processBallContact(); // Function to process ball contact
    //void morethanthree(std::chrono::steady_clock::time_point& prevContactTime); // Function to find if more than 3seconds has passed
private:

};

#endif /* BALL_CONTACT_COUNT_H */

/*
    extern bool x;

    
    */