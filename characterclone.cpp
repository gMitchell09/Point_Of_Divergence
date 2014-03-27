#include "characterclone.h"
#include "tilemap.h"

CharacterClone::CharacterClone(int width, int height, std::vector<State> stateStack, QGraphicsItem *parent) :
    AnimatedCollideableSprite(width, height, parent)
{
    this->setZValue(1);
    this->setSolid(true);

    TileMap * playerTiles = new TileMap(16, 33, 1, 1, "./resources/sprites/MarioMovement.png");

    std::vector<QPixmap> standRight;
    standRight.push_back(playerTiles->copyCellAt(4, 0));

    std::vector<QPixmap> standLeft;
    standLeft.push_back(playerTiles->copyCellAt(3, 0));

    std::vector<QPixmap> walkRight;
    walkRight.push_back(playerTiles->copyCellAt(5, 0));
    walkRight.push_back(playerTiles->copyCellAt(6, 0));

    std::vector<QPixmap> walkLeft;
    walkLeft.push_back(playerTiles->copyCellAt(1, 0));
    walkLeft.push_back(playerTiles->copyCellAt(2, 0));

    std::vector<QPixmap> runRight;
    runRight.push_back(playerTiles->copyCellAt(5, 0));
    runRight.push_back(playerTiles->copyCellAt(6, 0));

    std::vector<QPixmap> runLeft;
    runLeft.push_back(playerTiles->copyCellAt(1, 0));
    runLeft.push_back(playerTiles->copyCellAt(2, 0));

    std::vector<QPixmap> jumpRight;
    jumpRight.push_back(playerTiles->copyCellAt(3, 1));

    std::vector<QPixmap> jumpLeft;
    jumpLeft.push_back(playerTiles->copyCellAt(2, 1));

    std::vector<QPixmap> squatRight;
    squatRight.push_back(playerTiles->copyCellAt(1, 1));

    std::vector<QPixmap> squatLeft;
    squatLeft.push_back(playerTiles->copyCellAt(0, 1));

    std::vector<QPixmap> brakeRight;
    brakeRight.push_back(playerTiles->copyCellAt(7, 0));

    std::vector<QPixmap> brakeLeft;
    brakeLeft.push_back(playerTiles->copyCellAt(0, 0));

    this->addAnimation(standRight, Loop);
    this->addAnimation(standLeft, Loop);

    this->addAnimation(walkRight, Loop);
    this->addAnimation(walkLeft, Loop);

    this->addAnimation(runRight, Loop);
    this->addAnimation(runLeft, Loop);

    this->addAnimation(jumpRight, Loop);
    this->addAnimation(jumpLeft, Loop);

    this->addAnimation(squatRight, Loop);
    this->addAnimation(squatLeft, Loop);

    this->addAnimation(brakeRight, Loop);
    this->addAnimation(brakeLeft, Loop);

    this->triggerAnimation(Stand_Right);

    m_stateStack = stateStack;
}
