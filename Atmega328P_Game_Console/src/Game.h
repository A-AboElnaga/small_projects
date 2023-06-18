#ifndef GAME_H
#define GAME_H

#include "Engine.h"

typedef struct {
	void (*Engine_UpdateLoop)(Engine* engine);
} Game;

#endif /* GAME_H */
