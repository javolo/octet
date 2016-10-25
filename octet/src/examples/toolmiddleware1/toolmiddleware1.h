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
		app_scene->create_default_camera_and_lights_jav();
		// Position of camera related with elements
		app_scene->get_camera_instance(0)->get_node()->translate(vec3(0, 20, 18));
		// Try to rotate the camera to have better view of elements in the scene
		app_scene->get_camera_instance(0)->get_node()->rotate(-20, vec3(1, 0, 0));

		// Material definition
		material *red = new material(vec4(1, 0, 0, 1));
		material *green = new material(vec4(0, 1, 0, 1));
		material *blue = new material(vec4(0, 0, 1, 1));

		// Object is scene definition
		// Sphere
		mat4t mat;
		mat.translate(-3, 6, 0);
		app_scene->add_shape(mat, new mesh_sphere(vec3(2, 2, 2), 2), red, true);

		// Box
		mat.loadIdentity();
		mat.translate(0, 10, 0);
		app_scene->add_shape(mat, new mesh_box(vec3(2, 2, 2)), red, true);

		// Cylinder
		mat.loadIdentity();
		mat.translate(3, 6, 0);
		app_scene->add_shape(mat, new mesh_cylinder(zcylinder(vec3(0, 0, 0), 2, 4)), blue, true);

		// Ground
		mat.loadIdentity();
		mat.translate(0, -1, 0);
		app_scene->add_shape(mat, new mesh_box(vec3(20, 1, 20)), green, false);
		//// Border 1 (closest to the camera)
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

		// Potential change when load from xml or csv file
	}

	/// this is called to draw the world
	void draw_world(int x, int y, int w, int h) {
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
