#INTRO TO PROGRAMMING: ASSIGNMENT ONE

To this first assignment the task was to hack the example of Invaders 2D Game that Andy provide us. 
I decided to start a new game from scratch re-using all possible code from the example given. This are the steps 
followed create the assignment game:

 1. Background
 2. Player 
 3. Jump Effect
 4. Never ending screen
 5. Coins 
 6. Read from CSV
 7. Sounds 
 8. Problems found 
 9. Possible improvements
 10. Greetings 

 [linkname](https://youtu.be/8_KrbfG6R2U)

# 1. BACKGROUND

The first step in the game was to remove all the elements in the invaders games such as ship, invaders and borders.
Once that was done the next stage was to set the background image, the position and the size. Before adjusting the 
background I wanted to modify hte size of the default window to be wider and less high that the invaders one. I had 
to go to the default configuration in "windows_specific.h" class and modify the following line:

 window_handle = CreateWindowW(L"MyClass", L"octet",
        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, **"900, 600",**
        NULL, NULL, wndclass.hInstance, (LPVOID)this
      );

![Alt text](https://github.com/javolo/octet/blob/master/octet/src/examples/AssignmentOne/images/ScreenDifference.gif "Background Image")

# 2. PLAYER

The second step was to create a player and go around the screen. The first thing was to made it collide with the 
borders not taking in consideration the background image. Using the invaders example that wasn´t a problem.

Once the window has been changed and the player sprite was set, the next step was to set the bottom border as I wanted 
the player to go above the brick lane of the image. So for that I had to modify the definition of this border and
establish the heigh of it to -1.8f as the 0 value is the axis of image´s middle.

When the movement of the player was right I included the movement of the camera when the player is moving left or right.
I remove the key down functionality as I set the bottom border at a specific height and the init position of the player 
to this height.

![Alt text](https://github.com/javolo/octet/blob/master/octet/src/examples/AssignmentOne/images/PlayerBackground.gif "Background Player Image")

# 3. JUMP EFFECT

One of the key features I wanted my player to have is being able to jump to pick up objects, avoid enemies or go up to
boxes.

To have the desired effect instead of apply a constant speed each time the user presses the key up I set this amount to be
0.3 units. That get the effect of going up quicker. To complete the full jump effect I had to create a function 
(**"update_player_position"**) that it was run each frame to push the player down at a constant speed (**look at "key_up" logic section**)

This function does basically checks if the player is in the air (the position of the player is not the bottom border one) and 
apply a negative speed in the y axis until reach the floor. To get the position of the player I made one function to return 
its position (**"get_position"** in the sprite class).

I did´t want the player to be jumping forever so I decided to limit the jump effect. To do that I set the **"max_height"**
variable. This sets up how far the player can get in the jump knowing the starting point and the height of the screen. Once
it reaches this maximum I change the player speed to be negative and start going down. 

I have declared a boolean variable (**"player_descending"**) to make the player wait until is back at the floor to start another 
jump when is reched. What I want is either the user is with the key up pressed or try to jump when the player is descending not 
allow to jump again until the character has touched the floor.

# 4. NEVER ENDING SCREEN

Getting the player jumping and moving around the screen the next phase I wanted to achieve is move the player more than the
first screen, being capable of going right without limitation. After try several formulas I spoke with Andy and gave me the 
advice to have two background images and when the camera is fully aligned with the second one move the first to the right
of the second and repeat the process again and again to have infinite screen effect.

# 5. COINS

As part of make more interesting the game I included some coins so when the player takes them the score is incremented. I
defined some static coins and include the logic in the jump effect to when the player collision with the coin update the
score accordingly.

To include new features to the game I made the coins move each frame its position. I created a function (**"update_coin_position"**)
so each coin rotate a specific angle. I also made an auxiliar function in the sprite class (**"rotate_sprite_y"**) that
rotates in the y axis the image of the coin. The coins only rotate when they are active, once they have been taken I don´t
do it.

![Alt text](https://github.com/javolo/octet/blob/master/octet/src/examples/AssignmentOne/images/screenCoins.gif "Coins Example")

# 6. READ FROM CSV

One of the requirements was to read from a CSV file to input parameter, create a map or draw elements in the game. I decided
to use the file to create the level´s map of my game. For that I use the code Andy provided and also use the cplusplus forum
for further information.

I read coma by comma all the elements and I interpret them. At this early stage of the game, 0 means nothings, 1 means to draw
a coin. I also included different level of coins what brought a special treatment for the end of line characters solved finding 
an unusual lenght of String. Once found I updated the spacing variables to get the correct position.

One problem found was that the file wasn´t read properly. Searching different alternatives the issue was with the path forcing
me to put absolute path to the file in order to be read correctly.

# 7. SOUNDS

The last thing I´ve included in the game is the jump and coin sounds. One issue found is that mantaining the key_up pressed 
the sound repeated continuosly so I included a boolean variable (**"sound_on"**). Only at the beginning of the jump it´s 
played the sound. Then it´s invalidated until the floor is touched for the player.

# 8. PROBLEMS FOUND

## 8.1 Bottom space after player´s jump
The jump effect generated a problem and it was that sometimes the player didn´t get back to the original position 
touching the floor, it was some space between them but not always. To correct this gap and after get an advice 
from Andy I applied a height correction factor when the player was in this situation. 

## 8.2 Never ending screen issues
The first problem found with the never ending effect is when I move from the first to the second background image the
bottom border dissapear and when the player jumped it moved down and didn´t make the desired effect. What I did was to 
move the bottom border with the camera´s movement so this error was fixed.

The second problem was the left border. When I moved the first image and the camera aligned with the second background,
the player going left back to the first image created wrong effects and the game to crash to not have image to draw. As 
part of the the movement of background images I change the left border position as well (**look at "key_right" logic section**)

## 8.3 Position Score movement	
One issue found moving the camera with the player was that the score dissapeared and I wanted to move with the camera 
to be shown always. After several tries I get to increase the x position of the score with the camera x position in the
draw world method. That made the score follow the camera movement.

# 9. POSSIBLE IMPROVEMENTS

As this is the first kind of game I made there is huge space for improvement adding some more features:
 * Make the player not to be always in the left part and start the movement of the camera along the middle
 of the screen
 * Include enemies, boxes and features to the map (such as pipes, stairs...)
 * Create a level random generator
 * Amend the jump max_height to be calculated with start and end position.
 * Every 100 coins one live up

# 10. GREETINGS

Just a small note to say big thank you to Raul Araujo and Andy Thomason for all the help provided during the whole 
assignment explaining me some key concepts and giving me useful advices.