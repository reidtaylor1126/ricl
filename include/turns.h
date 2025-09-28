#ifndef TURNS_H
#define TURNS_H

#include "table.h"
#include "cursor.h"
#include <string.h>
#include <stdio.h>

#define ACTION_DRAW (1)
#define ACTION_PON (2)
#define ACTION_CHI (3)
#define ACTION_KAN (4)
#define ACTION_RON (5)

#define ACTION_DISCARD (1)
#define ACTION_RIICHI (2)
#define ACTION_TSUMO (3)

#define NOT_TURN (0)
#define AWAITING_DRAW (1)
#define HAS_14TH_TILE (2)
#define CHOOSING_CHI (3)
#define DISCARDING (5)

#define CAN_KAN (4)

int tickTurn(struct table* table);
void handleSkippingPon(struct table* table, uint8_t playerData);

#endif