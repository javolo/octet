#INTRO TO PROGRAMMING: ASSIGNMENT ONE

To this first assignment the task was to hack the example of Invaders 2D Game that Andy provide us. 
I decided to start a new game from scratch re-using all possible code from the example give. For that
reason this are the step followed in the development of this first task:
 1. Background
 2. Player 
 3. Jump Effect (max_height jump, stop with border bottom)
 4. Never ending screen (Andy´s advice, two background images)
 5. Coins (including coin sprites, collision with player, score update)
 6. Read from CSV (using Andy´s code, searching on the net, problem with path)
 7. Different level of coins (interpreation of eol)
 8. Sounds (jump effect, taking coins)
 9. Problems found (removal of borders, position the score not following the camera, move border wit
	never ending screen, jump effect & border bottom space)
 10. Possible improvements (move the camera when the player is around the middle of the screen,
	include enemies, include boxes, include features to the map such as pipes or stairs, level random generator)
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

# 3. PLAYER


# 9. PROBLEMS FOUND

## 9.1 Border removal
When the background image was set still around had the white border Andy set to delimiter the movement of the invaders
in the original game. Playing with the different parameters of the sprite init method, finally I discovered that the 
last two (widht and height) and setting them accordingly to 0, removed the white background but keeping the walls of 
the game.
