import pygame

class Laser(pygame.sprite.Sprite):
    def __init__(self,pos,screen_height,speed= 4,color='red'):
        super().__init__()
        self.image = pygame.Surface((4,20))
        self.image.fill(color)
        self.rect  =self.image.get_rect(center=pos)
        self.speed = speed
        self.height_y_constraint = screen_height

    def destroy(self):
        if self.rect.y <= 10 or self.rect.y >= self.height_y_constraint+10:
            self.kill()

    def update(self):
        self.rect.y -= self.speed
        self.destroy()