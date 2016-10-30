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


namespace octet {
  /// Scene containing a box with octet.
  class toolmiddleware1 : public app {
    // scene for drawing box
    ref<visual_scene> app_scene;

  public:
    /// this is called when we construct the class before everything is initialised.
    toolmiddleware1(int argc, char **argv) : app(argc, argv) {
    }

	/// this is called once OpenGL is initialized
	void app_init() {
		// Camera definition
		app_scene = new visual_scene();
		// Definition of lights.
		app_scene->create_default_camera_and_lights_jav();
		// Position of camera related with elements
		app_scene->get_camera_instance(0)->get_node()->translate(vec3(0, 20, 18));
		// Try to rotate the camera to have better view of elements in the scene
		app_scene->get_camera_instance(0)->get_node()->rotate(-20, vec3(1, 0, 0));

		// Material definition
		material *red = new material(vec4(1, 0, 0, 1));
		material *green = new material(vec4(0, 1, 0, 1));
		material *blue = new material(vec4(0, 0, 1, 1));

		// Definiton of the objects in the scene
		// TO DO: Read the objects from a CSV or XML file.

		// Sphere 1
		mat4t mat;
		mat.translate(-3, 6, 0);
		app_scene->add_shape(mat, new mesh_sphere(vec3(2, 2, 2), 2), red, true);

		// Sphere 2
		mat.loadIdentity();
		mat.translate(0, 10, 0);
		app_scene->add_shape(mat, new mesh_sphere(vec3(2, 2, 2), 2), blue, true);

		// We create the HINGE Constraint
		// btHingeConstraint::btHingeConstraint(btRigidBody & rbA, btRigidBody & rbB, const btVector3 & pivotInA, const btVector3 & pivotInB, const btVector3 & axisInA, const btVector3 & axisInB, bool useReferenceFrameA = false )
		// We need:
		// 1. Rigid body Sphere 1 (btRigidBody, scene_node has one)
		// 2. Rigid body Sphere 2 (btRigidBody, scene_node has one)
		// 3. Location Sphere 1 (btVector3, with coordinates of rigid body)
		// 4. Location Sphere 2 (btVector3, with coordinates of rigid body)
		// 5. Axis (btVector3, axis X, Y and Z, depending where we want the hinge axis)

		// With all this information we can create the bullet physics hinge constraint and see how that constraint work in the world
		btHingeConstraint* hinge;
		// Rigid Body Sphere 1
		btRigidBody * rbSphere1;
		btRigidBody * rbSphere2;




		// Box
		/*mat.loadIdentity();
		mat.translate(0, 10, 0);
		app_scene->add_shape(mat, new mesh_box(vec3(2, 2, 2)), red, true);*/

		// Cylinder
		/*mat.loadIdentity();
		mat.translate(3, 6, 0);
		app_scene->add_shape(mat, new mesh_cylinder(zcylinder(vec3(0, 0, 0), 2, 4)), blue, true);*/


		// Hard way to have the tablero of the game
		// TO DO: Find the BT collider to make a composition of all the tablero parts

		// Ground
		mat.loadIdentity();
		mat.translate(0, -1, 0);
		app_scene->add_shape(mat, new mesh_box(vec3(20, 1, 20)), green, false);
		// Border 1 (closest to the camera)
		mat.loadIdentity();
		mat.translate(0, 1, 20);
		app_scene->add_shape(mat, new mesh_box(vec3(20, 1, 1)), green, false);
		// Border 2 (farest to the camera)
		mat.loadIdentity();
		mat.translate(0, 1, -20);
		app_scene->add_shape(mat, new mesh_box(vec3(20, 1, 1)), green, false);
		// Border 3 (left hand side)
		mat.loadIdentity();
		mat.translate(-19, 1, 0);
		mat.rotateY90();
		app_scene->add_shape(mat, new mesh_box(vec3(20, 1, 1)), green, false);
		// Border 4 (right hand side)
		mat.loadIdentity();
		mat.translate(19, 1, 0);
		mat.rotateY90();
		app_scene->add_shape(mat, new mesh_box(vec3(20, 1, 1)), green, false);
		
	}

	// Use the keyboard to move the elements around the screen
	// The first element we are going to move is the sphere to set up the hinge constraint
	// TO DO: Set up the keys to move the camera around the world

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
	}
  };
}
