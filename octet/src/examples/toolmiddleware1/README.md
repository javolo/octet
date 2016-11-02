#TOOLS & MIDDLEWARE: ASSIGNMENT ONE

In this first assignment of Tools & Middleware we were asked to investigate and play with the bullet physics library.
Specifically we needed to implement the following concepts:
	1. Hinge constraint
	2. Spring constraint 
	3. Detect collisions
	4. Read from external file

 [Assignment 1]()

# 1. Hinge Constraint

![Alt text]()

# 2. Spring Constraint

![Alt text]()

# 3. Collision Detection

For the collision detection I´ve used the example <a href="http://hamelot.io/programming/using-bullet-only-for-collision-detection/"> here</a> 
One of the issues I faced with the collision detection is that having the objects in the ground they generated a collision as well. I had to
created a new boolean field in the scene node class to deal with this. When I add the objects to the scene I populate that field depending if 
I want to detect the collision or not. The only ones deactivated is the ground and its boundaries.

# 4. READ FROM CSV

One of the requirements was to read from a CSV, text file or similar to input the parameters of the objects in order to create
them for the assignment. I decided to use an XML file to put in the game pitch the elements that will be affected by the constraints. 
In the first Intro to Programming homework I implemented a method to read from this type of file so I´ve re-used that code.

I read the element list (boxes, spheres and cylinders) that contains the shape key values like the position, the speed and the weigth 
needed in the different kind of constraints implemented in the assignment. I've used the "tinyxml2" library to help me and get all
the elements in the file, looping through all the single shapes in the list.

With all the objects retrieved from the XML file I created a list that I used to identify the type of shape. The hinge constraint 
will be only applied to spheres or spheres and cylinders. The spring constraint could be applied to any kind of object. 

# 5. 

![Alt text]()
<a href="http://bullet.googlecode.com/svn/trunk/Demos/ConstraintDemo/ConstraintDemo.cpp"> this page</a> 

# 6. 

# 7. 

# 8. 

# 9. 

# 10. 