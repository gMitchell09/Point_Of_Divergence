#ifndef GLOBAL_H
#define GLOBAL_H

#define TILE_HEIGHT 32
#define TILE_WIDTH 32
#define MIN(x, y) ((x<y)?x:y)
#define MAX(x, y) ((x>y)?x:y)
#define SIGN(x) ((x>0)?1:-1)

enum ItemType { kBlock = 0, kBox, kCoin, kLever, kButton, kSwitch, kDoor, kMovingPlatform, kGate, kClock, kTime_Lever,
                kSlope30Right = 11, kSlope45Right, kSlope60Right,
                kSlope30Left = 14,  kSlope45Left,  kSlope60Left};

#endif // GLOBAL_H
