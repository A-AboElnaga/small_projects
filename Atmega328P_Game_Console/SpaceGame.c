#include <stdint.h>
#include <stdio.h >
#include <avr/io.h>
#include <stdbool.h>
#include <math.h>
#include <stdlib.h>
#include "delay.h"
#include "max7219.h"
#include "spi.h"
#include "ky023.h"
#include "Engine.h"
#include "SpaceGame.h"
#include "Game.h"
#include "Vector2.h"

void SpaceGame_Init(SpaceGame* game) {
	game->playerPos.x = 1;
	game->playerPos.y = 4;
	game->timeToNextEnemySpawn = 1;
	game->numEnemies = 0;
	game->numBullets = 0;
	game->elapsedTime = 0;
	game->gameOver = false;
	game->lastShootTime = 0;
	game->numEnemiesDestroyed = 0;
	game->scoreDisplayAmount = -5;

}

void SpaceGame_UpdateLoop(SpaceGame* game, Engine* engine, KY023* ky023){

	
	if (game->gameOver) {
		game->scoreDisplayAmount += engine->deltaTime * 10;
		for (int i = 0; i < game->numEnemiesDestroyed && i <game->scoreDisplayAmount; i++) {
			int x = i % 8;
			int y = i / 8;
			Engine_SetPixel(engine, x, y);
		}
		return;
	}
	game->elapsedTime += engine->deltaTime;

	// Move player on y axis
	engine->inputY=ky023_readYAxis(ky023);
	if(engine->inputY==2)engine->inputY=-1;

	game->playerPos.y +=engine->inputY;
	if(game->playerPos.y<=0)game->playerPos.y=7;
	else if(game->playerPos.y>7)game->playerPos.y=0;
	
	// Move player on x axis
	/*
	engine->inputX=ky023_readXAxis(ky023);
	if(engine->inputX==2)engine->inputX=-1;

	game->playerPos.x+=engine->inputX;
	if(game->playerPos.x<=0)game->playerPos.x=7;
	else if(game->playerPos.x>7)game->playerPos.x=0;
	*/
	
	// Draw player
	uint8_t playerCoordX = game->playerPos.x;
	uint8_t playerCoordY = game->playerPos.y;
	Engine_SetPixel(engine, playerCoordX, playerCoordY);
	Engine_SetPixel(engine, playerCoordX - 1, playerCoordY);
	Engine_SetPixel(engine, playerCoordX - 1, playerCoordY - 1);
	Engine_SetPixel(engine, playerCoordX - 1, playerCoordY + 1);

	// Shoot
	if (!engine->buttonState) {
		game->bullets[game->numBullets].x = game->playerPos.x + 1;
		game->bullets[game->numBullets].y = game->playerPos.y;
		game->numBullets++;
	}

	// Update and draw bullets
	for (int i = game->numBullets - 1; i >= 0; i--) {
		Engine_SetPixel(engine, (int)game->bullets[i].x, (int)game->bullets[i].y);
		game->bullets[i].x += 1;

		bool destroyBullet = game->bullets[i].x >= 8;

		// Check for collision with enemies
		for (int j = game->numEnemies - 1; j >= 0; j--) {
			if (game->bullets[i].x >= game->enemies[j].x && (int)game->bullets[i].y == (int)game->enemies[j].y) {
				destroyBullet = true;
				// Destroy enemy (put end enemy into current slot to keep active enemies in front)
				game->numEnemies--;
				game->enemies[j].x = game->enemies[game->numEnemies].x;
				game->enemies[j].y = game->enemies[game->numEnemies].y;
				game->numEnemiesDestroyed++;
				break;
			}
		}

		// bullet off screen, recycle
		if (destroyBullet) {
			game->numBullets--;
			// put end bullet into current slot to keep 'alive' bullets in front
			game->bullets[i].x = game->bullets[game->numBullets].x;
			game->bullets[i].y = game->bullets[game->numBullets].y;
		}
	}

	// Handle enemy spawning
	game->timeToNextEnemySpawn +=1 ;
	if (game->timeToNextEnemySpawn ==3 ) {
		game->timeToNextEnemySpawn=0;
		game->enemies[game->numEnemies].x = 7;
		game->enemies[game->numEnemies].y = (rand() % (0 - 7 + 1)) + 0;;
		game->numEnemies++;
	}

	// Update and draw enemies
	for (int i = game->numEnemies - 1; i >= 0; i--) {
		Engine_SetPixel(engine, (int)game->enemies[i].x, (int)game->enemies[i].y);

		// Player - enemy collision check
		int enemyCoordX = (int)game->enemies[i].x;
		int enemyCoordY = (int)game->enemies[i].y;
		bool collision = enemyCoordX == playerCoordX && enemyCoordY == playerCoordY;
		collision |= enemyCoordX == playerCoordX - 1 && enemyCoordY == playerCoordY - 1;
		collision |= enemyCoordX == playerCoordX - 1 && enemyCoordY == playerCoordY + 1;
		if (collision) {
			game->gameOver = true;
			break;
		}

		// Move
		game->enemies[i].x -= 1;

		// enemy off screen, recycle
		if (game->enemies[i].x < 0) {
			game->numEnemies--;
			// put end enemy into current slot to keep active enemies in front
			game->enemies[i].x = game->enemies[game->numEnemies].x;
			game->enemies[i].y = game->enemies[game->numEnemies].y;
		}
	}
	
}
