#ifndef SCORING_H
#define SCORING_H

#include "hand.h"

uint8_t findTriplets(struct hand* hand, uint8_t* ends);
uint8_t findSequences(struct hand* hand, uint8_t* ends);
uint8_t findWaits(struct hand* hand, char* waits);

#endif