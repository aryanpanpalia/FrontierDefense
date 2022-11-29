#pragma once

#include <vector>
#include "player.h"
#include "bullet.h"
#include "enemy.h"

class Game {
    public:
        Player player;
        std::vector<Bullet> bullets;
        std::vector<Enemy> enemies;
        float lastEnemySpawnTime;
        bool gameOver;
        int numEnemiesKilled;

        Game();
        void render();
        void update();
        void playerOutOfBounds();
        void handleCollisions();
};
