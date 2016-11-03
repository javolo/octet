#TOOLS & MIDDLEWARE: ASSIGNMENT ONE

In this first assignment of Tools & Middleware we were asked to investigate and play with the bullet physics library.
Specifically we needed to implement the following concepts:

 1. Hinge constraint
 2. Spring constraint 
 3. Detect collisions
 4. Read from external file


 [Assignment 1]()

# 1. Hinge Constraint

A hinge constraint limit the rotation movement of an object as it was fixed by a hinge joint like a door. To implement that using bullet
physics I´ve followed its documentation <a href="http://bulletphysics.org/Bullet/BulletFull/classbtHingeConstraint.html#a5ae4261a17e0f0f8947025eae818f0c8">here</a>.
Following the official specification I created step by step all the elements involved in the hinge constraint and at the end I´ve joined
all together. 

Once I have the constraint fully done and I tried to run the program, nothing was happening. I looked on internet about that issue and I 
found out that I was missing the addition of the constraint to the bullet physics dynamic world. The scene node class has a variable defined 
but it wasn´t accesible so I implemented the getter method. Once I did that, added the constraint to the bullet physics world and run the
programme again the cosntraint was working.

Another problem I faced it´s I couldn´t see properly in the example how the hinge constraint worked. After a few try and error tests I 
decided to put some weight into one of the objects involved in the constraint. With that fix, using spheres I could see how one of the 
balls rotated around the other one taking like reference the axis in which the hinge constraint was defined. 

# 2. Spring Constraint

After the implementation of the hinge constraint where I spend a lot of time investigating new concepts and the insights of the bullet 
physics library, create a spring constraint was relatively easier job to do. I found a really good example in this 
<a href="http://gamedev.stackexchange.com/questions/54349/what-are-frame-a-and-frame-b-in-btgeneric6dofconstraints-constructor-for">page</a>.

I follow the same steps, create all the elements needed in order to create the constraint looking in the example in case of doubt. I used cubes
for this example and one of the problems raised was how to position them to clearly see the effect of the spring constraint. After figuring out
that now the boxes did the spring effect but when collide they overlapped each other. That was consequence of not have implemented the collisions
properly, something to add later on.

# 3. Collision Detection

For the collision detection I´ve used the example <a href="http://hamelot.io/programming/using-bullet-only-for-collision-detection/"> here</a> 
The example is strainght forward and it fixed the overlapping problem had with the spring constraint example.

One of the issues I faced with the collision detection is that having the objects in the ground they generated a collision as well. I didn´t want
that collision to be detected so I had to create a new boolean variable in the scene node class to deal with this. When I add the objects to the 
scene I populate that field depending if I want to detect the collision or not. The only element deactivated are the ground and its boundaries.

# 4. READ FROM CSV

One of the requirements was to read from a CSV, text file or similar to input the parameters of the objects in order to create
them for the assignment. I decided to use an XML file to put in the game pitch the elements that will be affected by the constraints. 
In the first Intro to Programming homework I implemented a method to read from this type of file so I´ve re-used that code.

I read the element list (boxes, spheres and cylinders) that contains the shape key values like the position, the speed and the weigth 
needed in the different kind of constraints implemented in the assignment. I've used the "tinyxml2" library to help me and get all
the elements in the file, looping through all the single shapes in the list.

With all the objects retrieved from the XML file I created a list that I used to identify the type of shape. The hinge constraint 
will be only applied to spheres or spheres and cylinders. The spring constraint could be applied to any kind of object. 

# 5. Sound system

One of the most complicated part of including some sound into the assignment was the installation of all the parts needed. Following 
the steps in this <a href="https://cuboidzone.wordpress.com/2013/07/26/tutorial-implementing-fmod/"> page</a> I´ve created the class 
with all its elements.

Next steps is to find a sound for the bouncing of elements and include it with all the assignment already implemented.

# 6. Improvements

No limitation of spheres for hinge constraints and boxes for spring constraints. At the moment all elements contain the same parameters
in order to be used in any kind of constraints

