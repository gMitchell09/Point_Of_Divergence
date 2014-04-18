#ifndef GLOBAL_H
#define GLOBAL_H

#define TILE_HEIGHT 32
#define TILE_WIDTH 32
#define MIN(x, y) ((x<y)?x:y)
#define MAX(x, y) ((x>y)?x:y)
#define SIGN(x) ((x>0)?1:-1)

enum ItemType { kBlock = 0, kBox, kCoin, kLever, kButton, kSwitch, kDoor, kMovingPlatform, kGate, kClock, kTime_Lever,
                kSlope30Right = 11, kSlope45Right, kSlope60Right,
                kSlope30Left = 14,  kSlope45Left,  kSlope60Left,
                kGoomba = 17, kMainCharacter = 18, kLadder = 19,
                kLeverFlipped = 20, kDoorOpen = 21,
                kBoxButton = 22,
                kTeleStation = 23,
                kTurretRight = 24,
                kTurretLeft = 25,
                kBossEnemy = 26,
                kTeleReceiver = 27,
                kNextLevel = 28,
                kPrevLevel = 29 };

#define SCALINGFACTOR 1.0/16.0
#define PX_TO_M(x) (SCALINGFACTOR * (x))
#define M_TO_PX(x) ((x) * 16.0)

#endif // GLOBAL_H
