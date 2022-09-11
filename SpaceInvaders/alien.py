import pygame

class Alien(pygame.sprite.Sprite):
    def __init__(self,color,x,y):
        super().__init__()
        path = 'graphics/' + color + '.png'
        self.image = pygame.image.load(path).convert_alpha()
        self.image = pygame.transform.scale(self.image,(60,60))
        self.rect  = self.image.get_rect(topleft=(x,y))
        if color == 'red': self.value = 100
        elif color == 'violet': self.value= 20
        elif color == 'black': self.value= 10
        elif color == 'blue': self.value=50

    def update(self,direction):
        self.rect.x += direction

class Extra(pygame.sprite.Sprite):
    def __init__(self,side,screen_width):
        super().__init__()
        self.image=pygame.image.load('graphics/extra.png').convert_alpha()
        self.image = pygame.transform.scale(self.image,(222,347//3))

        if side =="right":
            x= screen_width+ 50
            self.speed = -3
        else:
            x= -50
            self.speed = 3
        self.rect = self.image.get_rect(topleft=(x,80))

    def update(self):
        self.rect.x += self.speed