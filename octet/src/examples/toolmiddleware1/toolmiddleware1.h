////////////////////////////////////////////////////////////////////////////////
//																			  //
// (C) Andy Thomason 2012-2014												  //
//																			  //
// Javier de la Osa Escalada												  //
//																			  //
// Modular Framework for OpenGLES2 rendering on multiple platforms.			  //
//																			  //
// Tools & Middleware Assignment 1											  //
// Requirements:															  //
// - Hinge constraints														  //
// - Springe constraints													  //
// - Collision callbacks													  //
// - Read from CSV or text file												  //
//																			  //
////////////////////////////////////////////////////////////////////////////////

// Libraries to read the file
#include <iostream>
#include <fstream>
#include <sstream>

namespace octet {
  /// Scene containing a box with octet.
  class toolmiddleware1 : public app {
    // scene for drawing box
    ref<visual_scene> app_scene;
	// We need a variable bullet physics world variable
	btDiscreteDynamicsWorld* gameWorld;
	// Matrix to position the elements in the scene
	mat4t mat;
	// Counter of elements
	int sphereCounter = 0;
	int boxCounter = 0;

  public:
    /// this is called when we construct the class before everything is initialised.
    toolmiddleware1(int argc, char **argv) : app(argc, argv) {
    }

	/// this is called once OpenGL is initialized
	void app_init() {
		// Camera definition
		app_scene = new visual_scene();
		// we obtain the bullet physics world for the scene
		gameWorld = app_scene->get_world();
		// Definition of lights.
		app_scene->create_default_camera_and_lights_jav();
		// Position of camera related with elements
		app_scene->get_camera_instance(0)->get_node()->translate(vec3(0, 20, 18));
		// Try to rotate the camera to have better view of elements in the scene
		app_scene->get_camera_instance(0)->get_node()->rotate(-20, vec3(1, 0, 0));

		// Hard way to have the tablero of the game
		// TO DO: Find the BT collider to make a composition of all the tablero parts

		// Ground
		mat.loadIdentity();
		mat.translate(0, -1, 0);
		app_scene->add_shape(mat, new mesh_box(vec3(25, 1, 25)), materialSelection("green"), false);
		// Border 1 (closest to the camera)
		mat.loadIdentity();
		mat.translate(0, 1, 25);
		app_scene->add_shape(mat, new mesh_box(vec3(25, 2, 1)), materialSelection("green"), false);
		// Border 2 (farest to the camera)
		mat.loadIdentity();
		mat.translate(0, 1, -25);
		app_scene->add_shape(mat, new mesh_box(vec3(25, 2, 1)), materialSelection("green"), false);
		// Border 3 (left hand side)
		mat.loadIdentity();
		mat.translate(-24, 1, 0);
		mat.rotateY90();
		app_scene->add_shape(mat, new mesh_box(vec3(25, 2, 1)), materialSelection("green"), false);
		// Border 4 (right hand side)
		mat.loadIdentity();
		mat.translate(24, 1, 0);
		mat.rotateY90();
		app_scene->add_shape(mat, new mesh_box(vec3(25, 2, 1)), materialSelection("green"), false);

		// Call to the method to load the configuration file
		load_configuration_file();

		// If we added two or more spheres or two or more boxes we create the relevant constraints
		if (sphereCounter >= 2) {
			createHingeConstraint();
		}
		if (boxCounter >= 2) {
			createSpringConstraint();
		}
		
		// Rigid Body Sphere 1
		//scene_node* sphere1 = app_scene->get_mesh_instance(0)->get_node();
		//btRigidBody* rbSphere1 = sphere1->get_rigid_body();
		//// We create a velocity vector to add some movements and see better the hinge constraint
		//btVector3 velocitySphere1 = btVector3(0, 0, 0);
		//rbSphere1->setLinearVelocity(velocitySphere1);
		//// Rigid Body Sphere 2
		//scene_node* sphere2 = app_scene->get_mesh_instance(1)->get_node();
		//btRigidBody* rbSphere2 = sphere2->get_rigid_body();
		//// Speed for sphere 2
		//btVector3 velocitySphere2 = btVector3(100, 0, 100);
		//rbSphere2->setLinearVelocity(velocitySphere2);
		//// Axis (We큞l set the axis in the Y plane, but it can be changed easily)
		//btVector3 axisY = btVector3(1, 0, 0);
		// With all this information we can create the bullet physics hinge constraint and see how that constraint work in the world
		//btHingeConstraint* hinge = new btHingeConstraint(*rbSphere1, *rbSphere2, locationSphere1, locationSphere2, axisY, axisY);
		// Once we have created the constraint, we add it to the world
		//gameWorld->addConstraint(hinge, true);


		// Cylinder
		/*mat.loadIdentity();
		mat.translate(3, 6, 0);
		app_scene->add_shape(mat, new mesh_cylinder(zcylinder(vec3(0, 0, 0), 2, 4)), blue, true);*/


		
		
	}

	// Method to create hinge constraint between sphere initally
	// This mehtod will be triggered only if the count of spheres is greater than 2 as it큦 needed two objects
	void createHingeConstraint() {

		// We create the HINGE Constraint
		// Definition taken from Bullet physics Wiki
		// http://bulletphysics.org/Bullet/BulletFull/classbtHingeConstraint.html#a5ae4261a17e0f0f8947025eae818f0c8
		// https://github.com/bulletphysics/bullet3/blob/master/examples/Constraints/TestHingeTorque.cpp
		// btHingeConstraint::btHingeConstraint(btRigidBody & rbA, btRigidBody & rbB, const btVector3 & pivotInA, const btVector3 & pivotInB, const btVector3 & axisInA, const btVector3 & axisInB, bool useReferenceFrameA = false )
		// We need:
		// 1. Rigid body Sphere 1 (btRigidBody, scene_node has one)
		// 2. Rigid body Sphere 2 (btRigidBody, scene_node has one)
		// 3. Location Sphere 1 (btVector3, with coordinates of rigid body)
		// 4. Location Sphere 2 (btVector3, with coordinates of rigid body)
		// 5. Axis (btVector3, axis X, Y and Z, depending where we want the hinge axis)

		// Rigid Body Sphere 1
		scene_node* sphere1 = app_scene->get_mesh_instance(5)->get_node();
		btRigidBody* rbSphere1 = sphere1->get_rigid_body();
		// Rigid Body Sphere 2
		scene_node* sphere2 = app_scene->get_mesh_instance(6)->get_node();
		btRigidBody* rbSphere2 = sphere2->get_rigid_body();
		// Axis (We큞l set the axis in the Y plane, but it can be changed easily)
		btVector3 axisY = btVector3(1, 0, 0);
		// With all this information we can create the bullet physics hinge constraint and see how that constraint work in the world
		btHingeConstraint* hinge = new btHingeConstraint(*rbSphere1, *rbSphere2, rbSphere1->getRigidBodyLocation(), rbSphere2->getRigidBodyLocation(), axisY, axisY);
		// Once we have created the constraint, we add it to the world
		gameWorld->addConstraint(hinge, true);
	}

	// Method to create spring constraint between boxes initally
	// This mehtod will be triggered only if the count of boxes is greater than 2 as it큦 needed two objects
	void createSpringConstraint() {

		// We create the SPRING Constraint
		// Definition taken from Bullet physics Wiki
		// http://bulletphysics.org/Bullet/BulletFull/classbtGeneric6DofConstraint.html
		// Example used: http://gamedev.stackexchange.com/questions/54349/what-are-frame-a-and-frame-b-in-btgeneric6dofconstraints-constructor-for
		// 1. Rigid body Box 1 (btRigidBody, scene_node has one)
		// 2. Rigid body Box 2 (btRigidBody, scene_node has one)
		// 3. Transform Box 1 (btTransform)
		// 4. Transform Box 2 (btTransform)
		// 5. Linear Reference Frame A (bool)

		// Rigid Body Box 1
		scene_node* box1 = app_scene->get_mesh_instance(7)->get_node();
		btRigidBody* rbBox1 = box1->get_rigid_body();

		// Rigid Body Box 2
		scene_node* box2 = app_scene->get_mesh_instance(8)->get_node();
		btRigidBody* rbBox2 = box2->get_rigid_body();

		// Transform Box 1
		btTransform frameInA;
		frameInA = btTransform::getIdentity();
		frameInA.setOrigin(btVector3(rbBox1->getOrigin()));

		// Transform Box 2
		btTransform frameInB;
		frameInB = btTransform::getIdentity();
		frameInB.setOrigin(btVector3(rbBox2->getOrigin()));

		// Spring Constraint Definition with all the information created before
		btGeneric6DofSpringConstraint* springConstraint = new btGeneric6DofSpringConstraint(*rbBox1, *rbBox2, frameInA, frameInB, true);

		// The Spring Constraint have a number of limits we need to define now
		// 1. Linear Upper Limit
		// 2. Linear Lower Limit
		// 3. Angular Upper Limit
		// 4. Angular Lower Limit
		// There are other limits like the stiffness, damping, equilibrum point. We are not going to set anything yet. We큞l add something if needed
		springConstraint->setLinearUpperLimit(btVector3(7.0, 0.0, 0.0));
		springConstraint->setLinearLowerLimit(btVector3(-7.0, 0.0, 0.0));
		// lock all rorations
		springConstraint->setAngularLowerLimit(btVector3(0.0f, 0.0f, -1.5f));
		springConstraint->setAngularUpperLimit(btVector3(0.0f, 0.0f, 1.5f));
		// We add the constraint to the world
		// We set the second parameter to false to be able to do collisions
		gameWorld->addConstraint(springConstraint, false);
		// More limits added
		springConstraint->enableSpring(0, true);
		springConstraint->setStiffness(0, 39.478f);
	
	}

	// Method to select the material from the colour input in the file
	material* materialSelection(string colour) {

		// Selection of material depending on the input
		if (colour == "red") {
			return new material(vec4(1, 0, 0, 1));
		}
		else if (colour == "green") {
			return new material(vec4(0, 1, 0, 1));
		}
		else if (colour == "blue") {
			return new material(vec4(0, 0, 1, 1));
		}
	}


	// Method to read the XML file and create in the scene the diferent elements
	void load_configuration_file() {

		// Counter of elements in the file
		// We큞l use that counter to input the elements in the rigid body of the objects added to the scene
		int counter = 5;

		// First of all, we generate the file name
		std::string filename = "ConfigurationFile.xml";
		TiXmlDocument file;
		file.LoadFile(filename.c_str());

		for (TiXmlElement *elem = file.FirstChildElement("TableroGame")->FirstChildElement("Elements")->FirstChildElement();
		elem != NULL; elem = elem->NextSiblingElement()) {

			// First element to retrieve is the type of object it is
			string objectType = elem->FirstChildElement("Type")->GetText();
			printf("TYPE OBJECT:  %s\n", objectType);
			// We obtain the position of the object read from the file
			float posX = atof(elem->FirstChildElement("Position")->FirstChildElement("X")->GetText());
			float posY = atof(elem->FirstChildElement("Position")->FirstChildElement("Y")->GetText());
			float posZ = atof(elem->FirstChildElement("Position")->FirstChildElement("Z")->GetText());
			// Create the vector position
			btVector3 objectPosition = btVector3(posX, posY, posZ);
			// We obtain the weight now
			float objectWeight = atof(elem->FirstChildElement("Weight")->GetText());
			// We retrieve the colour now
			string objectColour = elem->FirstChildElement("Colour")->GetText();
			// We obtain the material from the colour choice
			material *objectMaterial = materialSelection(objectColour);
			// Octet position vector definition
			vec3 positionVector = vec3(posX, posY, posZ);
			// We add the object to the scene now
			mat.loadIdentity();
			mat.translate(positionVector);

			// We check the type of object it is to retrieve different parameters on it
			if (objectType == "Sphere") {
		
				// Add sphere to the scene
				app_scene->add_shape(mat, new mesh_sphere(vec3(2, 2, 2), 2), objectMaterial, true, objectWeight);

				// We add the velocity and location to the rigid body in order to perform the constraints
				// Rigid Body Sphere 
				scene_node* sphere = app_scene->get_mesh_instance(counter)->get_node();
				btRigidBody* rbSphere = sphere->get_rigid_body();
				// We create a velocity vector to add some movements and see better the hinge constraint
				// We obtain now the velocity of the object from the XML file
				float velX = atof(elem->FirstChildElement("Speed")->FirstChildElement("X")->GetText());
				float velY = atof(elem->FirstChildElement("Speed")->FirstChildElement("Y")->GetText());
				float velZ = atof(elem->FirstChildElement("Speed")->FirstChildElement("Z")->GetText());
				// Create the vector velocity
				btVector3 sphereVelocity = btVector3(velX, velY, velZ);
				// We set the linear velocity with the information retrieve from the XML file
				rbSphere->setLinearVelocity(sphereVelocity);
				// We set the location as well
				rbSphere->setRigidBodyLocation(objectPosition);

				// Increase the sphere counter
				sphereCounter++;
			} else if (objectType == "Box") {

				// Add box to the scene
				app_scene->add_shape(mat, new mesh_box(vec3(2, 2, 2)), objectMaterial, true);

				// Rigid Body Box 
				scene_node* box = app_scene->get_mesh_instance(counter)->get_node();
				btRigidBody* rbBox = box->get_rigid_body();
				// We obtain now the velocity of the object from the XML file
				float origX = atof(elem->FirstChildElement("Origin")->FirstChildElement("X")->GetText());
				float origY = atof(elem->FirstChildElement("Origin")->FirstChildElement("Y")->GetText());
				float origZ = atof(elem->FirstChildElement("Origin")->FirstChildElement("Z")->GetText());
				// Create the vector velocity
				btVector3 boxOrigin = btVector3(origX, origY, origZ);
				// We set the linear velocity with the information retrieve from the XML file
				rbBox->setOrigin(boxOrigin);

				// Increase the box counter
				boxCounter++;
			}
			
			// Increase the counter to not overwrite the next element in the current one
			counter++;
		}

	}


	// Method to handle the collision in the bullet physics world
	// Code taken from here: http://hamelot.io/programming/using-bullet-only-for-collision-detection/

	void handleCollisions() {

		//Perform collision detection
		gameWorld->performDiscreteCollisionDetection();
		int numManifolds = gameWorld->getDispatcher()->getNumManifolds();
		//For each contact manifold
		for (int i = 0; i < numManifolds; i++) {
			btPersistentManifold* contactManifold = gameWorld->getDispatcher()->getManifoldByIndexInternal(i);
			const btCollisionObject* obA = static_cast<const btCollisionObject*>(contactManifold->getBody0());
			// We obtain the scene node of the bt collision object to get the position later on OBJECT A
			scene_node * currentObjA = ((scene_node*)obA->getUserPointer());
			if (currentObjA->getEnableCollisions()) continue;
			const btCollisionObject* obB = static_cast<const btCollisionObject*>(contactManifold->getBody1());
			// We obtain the scene node of the bt collision object to get the position later on OBJECT B
			scene_node * currentObjB = ((scene_node*)obB->getUserPointer());
			if (currentObjB->getEnableCollisions()) continue;
			contactManifold->refreshContactPoints(obA->getWorldTransform(), obB->getWorldTransform());
			int numContacts = contactManifold->getNumContacts();
			//For each contact point in that manifold
			for (int j = 0; j < numContacts; j++) {
				//Get the contact information
				btManifoldPoint& pt = contactManifold->getContactPoint(j);
				btVector3 ptA = pt.getPositionWorldOnA();
				btVector3 ptB = pt.getPositionWorldOnB();
				double ptdist = pt.getDistance();

				std::string dist = "Collision between objects at position (" + std::to_string(currentObjA->get_position().x()) + "," + std::to_string(currentObjA->get_position().y()) + "," + std::to_string(currentObjA->get_position().z()) + ") and (" + std::to_string(currentObjB->get_position().x()) + ", " + std::to_string(currentObjB->get_position().y()) + ", " + std::to_string(currentObjB->get_position().z()) + ") " + "\n";
				printf(dist.c_str());

			}
		}
	}

	// Use the keyboard to move the elements around the screen
	// The first element we are going to move is the sphere to set up the hinge constraint
	// TO DO: Set up the keys to move the camera around the world
	// Try to move one of the balls with the keys to see if we can see better the hinge constraint

	void manageKeyInputs() {

		if (is_key_down(key_left)) {
		}

		if (is_key_down(key_right)) {
		}

		if (is_key_down(key_up)) {
		}

		if (is_key_down(key_down)) {
		}

	}

	// TO DO: Handle collisions with walls to not stop when hit them


	/// this is called to draw the world
	void draw_world(int x, int y, int w, int h) {

		// Call the method to manage inputs
		manageKeyInputs();

		int vx = 0, vy = 0;
		get_viewport_size(vx, vy);
		app_scene->begin_render(vx, vy);

		// update matrices. assume 30 fps.
		app_scene->update(1.0f / 30);

		// draw the scene
		app_scene->render((float)vx / vy);

		// We call the method to handle collisions
		handleCollisions();
	}
  };
}
