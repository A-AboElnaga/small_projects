import sys
from turtle import window_height, window_width
from pygame.locals import *
import pygame
from pygame import mixer
from player import Player 
import obstacles
from alien import Alien,Extra
from random import choice,randint
from laser import Laser

class Game:
    def __init__(self):
        #Player Setup
        player_sprite = Player((screen_width/2,screen_height-30),screen_width,screen_height,5)
        self.player = pygame.sprite.GroupSingle(player_sprite)
        #health and score Setup
        self.lives =3 
        self.live_surf = pygame.image.load('graphics/player.png').convert_alpha()
        self.live_surf = pygame.transform.scale(self.live_surf,(30,30))
        self.live_x_start_pos = screen_width - (self.live_surf.get_size()[0]*2 +50)
        self.score = 0
        self.font = pygame.font.Font('freesansbold.ttf',25) # also could use "fonts/slkscr.ttf"     


        #Obstacles Setup
        self.shape = obstacles.shape
        self.block_size = 10
        self.blocks = pygame.sprite.Group()
        self.obstacle_amount= 4
        self.obstacle_x_position =[num*(screen_width/self.obstacle_amount) for num in range(self.obstacle_amount)]
        self.create_multiple_obstacles(*self.obstacle_x_position, x_start=100, y_start=480) 

        #Alien setup
        self.aliens = pygame.sprite.Group()
        self.alien_setup(rows=6,cols=8)
        self.aliens_direction= 1
        self.alien_laser = pygame.sprite.Group()

        #Extra unique alien
        self.extra = pygame.sprite.GroupSingle()
        self.extra_spawn_time = randint(600,3000)

    def create_obstacle(self,x_start,y_start,offset_x):
        for row_index, row in enumerate(self.shape):
            for col_index, col in enumerate(row):
                if col== 'x':
                    x,y= offset_x + x_start + col_index*self.block_size, y_start + row_index*self.block_size
                    block = obstacles.Block(self.block_size,(0,69,255),x,y)
                    self.blocks.add(block)

    def create_multiple_obstacles(self,*offset,x_start,y_start):
        for offset_x in offset:
            offset_x = int(offset_x)
            self.create_obstacle(x_start,y_start,offset_x)
    
    def alien_setup(self,rows,cols,x_distance = 120,y_distance = 60,x_offset=10,y_offset=10):
        for row_index, row in enumerate(range(rows)):
            for col_index, col in enumerate(range(cols)):
                x,y= col_index*x_distance+x_offset , row_index*y_distance+y_offset
                if row_index==0 : alien_sprite= Alien('violet',x,y)
                elif row_index<=2: alien_sprite= Alien('blue',x,y)
                elif row_index<=4: alien_sprite= Alien('red',x,y)
                else : alien_sprite= Alien('black',x,y)
                self.aliens.add(alien_sprite)
    
    def alien_pos_checker(self):
        all_aliens=self.aliens.sprites()
        for alien in all_aliens:
            if alien.rect.right >= screen_width:
                self.aliens_direction= -1
                self.aliens_movedown(2)
            elif alien.rect.left <=0:
                self.aliens_direction =1
                self.aliens_movedown(2)

    def aliens_movedown(self,distance):
        if self.aliens:
            for alien in self.aliens.sprites():
                alien.rect.y+=distance

    def alien_shoot(self):
        if self.aliens.sprites():
            random_alien = choice(self.aliens.sprites())
            laser_sprite = Laser(random_alien.rect.center,screen_height,-6,'red')
            self.alien_laser.add(laser_sprite)
    
    def extra_alien_timer(self):
        self.extra_spawn_time -=1
        if self.extra_spawn_time <=0:
            self.extra.add(Extra(choice(['right','left']),screen_width))
            self.extra_spawn_time = randint(600,3000)

    def collision_checks(self):

        #player_laser
        if self.player.sprite.lasers:
            for laser in self.player.sprite.lasers:
                #obstecle coll
                if pygame.sprite.spritecollide(laser,self.blocks,True):
                    laser.kill()
                #alien collisions
                aliens_hit = pygame.sprite.spritecollide(laser,self.aliens,True)
                if aliens_hit:
                    for alien in aliens_hit:
                        self.score+=alien.value
                        explosionSound.play()
                        laser.kill()
                
                #extra collisions
                if pygame.sprite.spritecollide(laser,self.extra,True):
                    laser.kill()
                    explosionSound.play()
                    self.score+=500

        #alien laser
        if self.alien_laser:
            for laser in self.alien_laser:
                #obstecle coll
                if pygame.sprite.spritecollide(laser,self.blocks,True):
                    laser.kill()
                
                #player kill
                if pygame.sprite.spritecollide(laser,self.player,False):
                    laser.kill()
                    self.lives -=1
                    if self.lives <=0:
                        self.font = pygame.font.Font('fonts/slkscr.ttf',40)  # also could use "freesansbold.ttf" without "fotns/""
                        vicotry_surf = self.font.render("You Lost! Wish you good luck next time!",False,"red")
                        vicotry_rect = vicotry_surf.get_rect(center=(screen_width/2,screen_height/2))
                        screen.blit(vicotry_surf,vicotry_rect)
                        clock.tick(10)
                        pygame.quit()
                        sys.exit()

        if self.aliens:
            for alien in self.aliens:
                pygame.sprite.spritecollide(alien,self.blocks,True)

                if pygame.sprite.spritecollide(alien,self.player,False):
                    self.font = pygame.font.Font('fonts/slkscr.ttf',40)  # also could use "freesansbold.ttf" without "fotns/""
                    vicotry_surf = self.font.render("You Lost! Wish you good luck next time!",False,"red")
                    vicotry_rect = vicotry_surf.get_rect(center=(screen_width/2,screen_height/2))
                    screen.blit(vicotry_surf,vicotry_rect)
                    clock.tick(10)
                    pygame.quit()
                    sys.exit()

    def display_lives(self):
        for life in range((self.lives) -1):
            x = self.live_x_start_pos + (life * (self.live_surf.get_size()[0]+ 20))
            screen.blit(self.live_surf,(x,10))

    def display_score(self):
        score_surf = self.font.render(f'score : {self.score}',False,'white')
        score_rect = score_surf.get_rect(topleft =(0,0))
        screen.blit(score_surf,score_rect)

    def victory_message(self):
        if not self.aliens.sprites():
            self.font = pygame.font.Font('fonts/slkscr.ttf',40)  # also could use "freesansbold.ttf" without "fotns/""
            vicotry_surf = self.font.render("You Won!",False,"Black")
            vicotry_rect = vicotry_surf.get_rect(center=(screen_width/2,screen_height/2))
            screen.blit(vicotry_surf,vicotry_rect)

    def run(self):
        self.alien_pos_checker()
        self.extra_alien_timer()
        self.collision_checks()

        self.player.update()
        self.aliens.update(self.aliens_direction)
        self.alien_laser.update()
        self.extra.update()


        self.player.sprite.lasers.draw(screen)
        self.player.draw(screen)
        self.blocks.draw(screen)
        self.aliens.draw(screen)
        self.alien_laser.draw(screen)
        self.extra.draw(screen)
        self.display_lives()
        self.display_score()
        self.victory_message()

if __name__ == "__main__":

    # Intialize the pygame
    pygame.init()

    # create the screen
    screen_width,screen_height= 1200,720
    screen = pygame.display.set_mode((screen_width,screen_height))

    #clock
    clock = pygame.time.Clock()
    bg_music= pygame.mixer.Sound("audio/background.wav")
    bg_music.set_volume(0.3)
    bg_music.play(-1)    
    pygame.display.set_caption("Space Invader")
    icon = pygame.image.load('graphics/icon.png')
    pygame.display.set_icon(icon)
    explosionSound = mixer.Sound("audio/explosion.wav")
    explosionSound.set_volume(0.4)

    game = Game()

    AlienLaser = pygame.USEREVENT +1

    pygame.time.set_timer(AlienLaser,800)
    # Game Loop
    running = True
    background = pygame.image.load('graphics/background.png')
    background = pygame.transform.scale(background,(screen_width,screen_height))   
    start = False
    title_font = pygame.font.SysFont("comicsans",70)
    while running:
     while not start: 
      for event in pygame.event.get():
        if event.type == pygame.MOUSEBUTTONDOWN:
            start= True
        else:
            screen.fill((0, 0, 0))
            # Background Image
            screen.blit(background, (0, 0))
            title_label = title_font.render("Press the mouse tp begin...",1,(255,255,255))
            title_rect = title_label.get_rect(center=(screen_width/2,screen_height/2))
            screen.blit(title_label,title_rect)
            pygame.display.update()
     else:
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                running = False
                pygame.quit()
                sys.exit()
            if event.type == AlienLaser:
                game.alien_shoot()
        pygame.display.update()
        screen.fill((0, 0, 0))
        # Background Image
        screen.blit(background, (0, 0))

        game.run()
        pygame.display.flip()
        clock.tick(60)
'''
def main():
    while True:
        if event.type == AlienLaser:
           game.alien_shoot()
        pygame.display.update()


        game.run()
        pygame.display.flip()
        clock.tick(60)

if __name__ == "__main__":

    # Intialize the pygame
    pygame.init()

    # create the screen
    screen_width,screen_height= 1200,720
    screen = pygame.display.set_mode((screen_width,screen_height))

    #clock
    clock = pygame.time.Clock()
    bg_music= pygame.mixer.Sound("audio/background.wav")
    bg_music.set_volume(0.3)
    bg_music.play(-1)    
    pygame.display.set_caption("Space Invader")
    icon = pygame.image.load('graphics/icon.png')
    pygame.display.set_icon(icon)
    explosionSound = mixer.Sound("audio/explosion.wav")
    explosionSound.set_volume(0.4)

    game = Game()

    AlienLaser = pygame.USEREVENT +1

    pygame.time.set_timer(AlienLaser,800)
    # Game Loop
    running = True
    background = pygame.image.load('graphics/background.png')
    background = pygame.transform.scale(background,(screen_width,screen_height))   
    title_font = pygame.font.SysFont("comicsans",70)
    while running:
        screen.fill((0, 0, 0))
        # Background Image
        screen.blit(background, (0, 0))
        title_label = title_font.render("Press the mouse tp begin...",1,(255,255,255))
        title_rect = title_label.get_rect(center=(screen_width/2,screen_height/2))
        screen.blit(title_label,title_rect)
        pygame.display.update()

        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                running = False
                pygame.quit()
                sys.exit()
            if event.type == pygame.MOUSEBUTTONDOWN:
                main()    

'''