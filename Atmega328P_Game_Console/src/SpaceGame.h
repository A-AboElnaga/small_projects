#ifndef SPACE_GAME_H
#define SPACE_GAME_H

#include "Vector2.h"
#include "Game.h"
#include "Engine.h"
#include <stdbool.h>

typedef struct {
	Game base;

	// Difficulty settings:
	const float enemySpeed;
	const float spawnDelayStart;
	const float spawnDelayEnd;
	const float difficultyDuration;
	const float delayBetweenShots;
	const float playerSpeed;

	Vector2 playerPos;
	Vector2 bullets[16];
	Vector2 enemies[32];

	int numEnemies;
	int numBullets;
	int numEnemiesDestroyed;
	float timeToNextEnemySpawn;
	float scoreDisplayAmount;
	float lastShootTime;
	float elapsedTime;
	bool gameOver;
} SpaceGame;

void SpaceGame_Init(SpaceGame* game);
void SpaceGame_UpdateLoop(SpaceGame* game, Engine* engine, KY023* ky023);
#endif /* SPACE_GAME_H */
