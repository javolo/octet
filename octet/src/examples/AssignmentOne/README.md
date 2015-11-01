#INTRO TO PROGRAMMING: ASSIGNMENT ONE

To this first assignment the task was to hack the example of Invaders 2D Game that Andy provide us. 
I decided to start a new game from scratch re-using all possible code from the example give. For that
reason this are the step followed in the development of this first task:
 1. Background
 2. Player 
 3. Jump Effect
 4. Never ending screen (Andy´s advice, two background images)
 5. Coins (including coin sprites, collision with player, score update)
 6. Read from CSV (using Andy´s code, searching on the net, problem with path)
 7. Different level of coins (interpreation of eol)
 8. Sounds (jump effect, taking coins)
 9. Problems found (position the score not following the camera, move border wit
	never ending screen)
 10. Possible improvements (move the camera when the player is around the middle of the screen,
	include enemies, include boxes, include features to the map such as pipes or stairs, level random generator
	, modify jump max height with difference between start and finish "y")
 11. Greetings (thanks to Raul (basics of games, matrix understanding, read from CSV and Andy (never ending screen, 
	jump space when touching the bottom border))

# 1. BACKGROUND

The first step in the game was to remove all the elements in the invaders games such as ship, invaders and borders.
Once that was done the next stage was to set the background image, the position and the size. Before adjusting the 
background I wanted to modify size provided for default to be wider and less high that the invaders screen. To get
this effect I had to go to the default configuration in "windows_specific.h" class and modify the following line:

 window_handle = CreateWindowW(L"MyClass", L"octet",
        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, **"900, 600",**
        NULL, NULL, wndclass.hInstance, (LPVOID)this
      );

// Put the image of new background and old image to see the difference and the background layout of the page
![Alt text](https://github.com/javolo/octet/blob/master/octet/src/examples/AssignmentOne/images/Background.gif "Background Image")

# 2. PLAYER

The second step is to put a player in the screen and move it around. The first thing was to move the player around
the screen and made it collide with the borders not taking in consideration the background image. Using the invaders
example that wasn´t a problem.

Once the window has been changed and the player sprite was set, the next step is set the bottom border as I wanted 
the player to go above the brick lane of the image. So for that I had to modify the definition of this border and
establish the heigh of it to -1.8f as the 0 value is the axis of image´s middle.

When the movement of the player was right I included the movement of the camera when the player is moving left or right.
Setting the bottom border at a specific height and setting the init position of the player to this bottom border height
was useless to have a functionality when the user play the key down. It was removed.

// Put the image of background + player
![Alt text](https://github.com/javolo/octet/blob/master/octet/src/examples/AssignmentOne/images/Background.gif "Background Image")

# 3. JUMP EFFECT

One of the key features I wanted my player to have is being able to jump to pick up objects, avoid enemies or go up to
boxes. For that in definition of the key up treatment was modified to allow jump effect. 

To have the desired effect instead of apply a small amount each time the user presses the key up I set this amount to be
0.3 units. That get the effect of going up. To complete the full jump effect I had to create a function (**"update_player_position"**)
that it was run each frame to push the player down at a constant speed.

What this function does basically is check if the player is in the air (the position of the player is not equal at the height
of the bottom border) and apply to it negative speed in the y axis until reach the floor. To get the position of the player I
made one function to return its position (**"get_position"** in the sprite class).

I did´t want the player to be jumping forever so I decided to limit the jump effect. To do that I set the **"max_height"**
variable. This sets up how far the player can get in the jump knowing the starting point and the height of the screen. Once
it reaches this maximum I change the player speed to be negative and start going down. 

I have declared a boolean variable (**"player_descending"**) when the player reach the top height and make him wait until is  
back at the floor to start another jump. What I want is either the user is with the key up pressed or try to jump when the 
player is descending not allow to jump again until the character has touched the floor.

# 4. NEVER ENDING SCREEN

Getting the player jumping and moving around the screen the next phase I wanted to achieve is move the player more than the
first screen, being capable of going right without limitation. After try several formulas I spoke with Andy and gave me the 
advice to have two background images and when the camera is fully aligned with the second one move the first one to the right
of the second and repeat the process again and again to have infinite screen effect.

# 9. PROBLEMS FOUND

## 9.1 Border removal
When the background image was set still around had the white border Andy set to delimiter the movement of the invaders
in the original game. Playing with the different parameters of the sprite init method, finally I discovered that the 
last two (widht and height) and setting them accordingly to 0, removed the white background but keeping the walls of 
the game.

## 9.2 Bottom space after player´s jump
One of the problems found with the jump effect is that sometimes the player didn´t get back to the original position 
touching the floor, it was some space between the player and the floor. Not always happen this issue.To correct this 
space and after get an advice from Andy I applied a height correction factor when the player was in this situation. 

## 9.3 Never ending screen issues




