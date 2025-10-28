#ifndef SCORING_H
#define SCORING_H

#include "hand.h"
#include "tiles.h"

#define HAS_SEQUENCE (128)
#define SEQUENCE_BELOW_MASK (24)
#define SEQUENCE_CENTER_MASK (10)
#define SEQUENCE_ABOVE_MASK (3)

#define IS_TRIPLET (128)
#define ORPHANS_TEMPLATE (40959)

char orphansWait(struct hand* hand);
uint8_t findTriplets(struct hand* hand, uint8_t* ends);
uint8_t findSequences(struct hand* hand, uint8_t* ends);
uint8_t findSequencesForTile(struct hand* hand, char tile);
uint8_t findWaits(struct hand* hand, char* waits);

#endif