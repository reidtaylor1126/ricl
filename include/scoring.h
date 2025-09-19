#ifndef SCORING_H
#define SCORING_H

#include "hand.h"

#define SEQUENCE_MASK_BELOW (24)
#define SEQUENCE_MASK_CENTER (10)
#define SEQUENCE_MASK_ABOVE (3)

uint8_t findTriplets(struct hand* hand, uint8_t* ends);
uint8_t findSequences(struct hand* hand, uint8_t* ends);
uint8_t findSequencesFor(struct hand* hand, char tile, char* heads);
uint8_t findWaits(struct hand* hand, char* waits);

#endif