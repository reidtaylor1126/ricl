#ifndef SCORING_H
#define SCORING_H

#include "hand.h"

void extractTriplets(struct hand* hand);
uint8_t isTenpai(struct hand* hand);

#endif