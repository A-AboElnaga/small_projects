import pygame
from pygame import mixer
from laser import Laser 

class Player(pygame.sprite.Sprite):
    def __init__(self,pos,constraint_x,constraint_y,speed):
        super().__init__()
        self.image = pygame.image.load('graphics/player.png').convert_alpha()
        self.rect  = self.image.get_rect(midbottom=pos)
        self.speed = speed
        self.max_x_constraint = constraint_x
        self.max_y_constraint = constraint_y
        self.ready = True
        self.laser_time= 0
        self.laser_cooldown= 600

        self.lasers = pygame.sprite.Group()

    def get_input(self):
        keys = pygame.key.get_pressed()
        if  keys[pygame.K_RIGHT] or keys[pygame.K_d]:
            self.rect.x+=self.speed
        elif keys[pygame.K_LEFT] or keys[pygame.K_a]:
            self.rect.x-=self.speed
        if keys[pygame.K_UP] or keys[pygame.K_w]:
            self.rect.y-=self.speed
        elif keys[pygame.K_DOWN] or keys[pygame.K_s]:
            self.rect.y+=self.speed

        if keys[pygame.K_SPACE] and self.ready:
            self.shoot_laser()
            self.ready = False
            self.laser_time = pygame.time.get_ticks()
    
    def recharge(self):
        if not self.ready:
            current_time = pygame.time.get_ticks()
            if current_time - self.laser_time >=self.laser_cooldown:
                self.ready =True

    def constraint(self):
        if self.rect.left<=0:
            self.rect.left =0
        if self.rect.right>=self.max_x_constraint:
            self.rect.right=self.max_x_constraint
        if self.rect.top<=0:
            self.rect.top =0
        if self.rect.bottom>=self.max_y_constraint:
            self.rect.bottom=self.max_y_constraint
    
    def shoot_laser(self):
        laserSound = mixer.Sound("audio/laser.wav")
        self.lasers.add(Laser(self.rect.center,screen_height=self.max_y_constraint-10,speed=4,color='blue'))
        laserSound.play()
    def update(self):
        self.get_input()
        self.constraint()
        self.recharge()
        self.lasers.update()

