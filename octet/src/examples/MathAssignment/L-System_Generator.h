// Libraries to read the file
#include <iostream>
#include <fstream>
#include "turtle.h"
#include "sprite.h"

namespace octet {
	class L_System_Generator : public octet::app {

		// Matrix to transform points in our camera space to the world.
		// This lets us move our camera
		mat4t cameraToWorld;

		ref<visual_scene> app_scene;
		// Variable turtle set the values in init
		Turtle L_System;

		enum {
			// Constants definition

			num_sprites

		};

		// big array of sprites
		sprite sprites[num_sprites];


	public:

		// this is called when we construct the class
		L_System_Generator(int argc, char **argv) : app(argc, argv) {
		}

		// Use the keyboard to generate the tree
		void generateTree() {


		}

		// this is called once OpenGL is initialized
		void app_init() {
			app_scene = new visual_scene();
			app_scene->create_default_camera_and_lights();
			app_scene->get_camera_instance(0)->set_far_plane(100000.0f);

			// We generate the parameter of L-System
			std::string axiom = "X";
			float angle = 25.0f;
			std::string rule1 = "F−[[X]+X]+F[+FX]−X";
			std::string rule2 = "FF";
			std::vector<std::string> turtle_rules;
			turtle_rules[0] = rule1;
			turtle_rules[1] = rule2;
			int turtle_iterations;

			L_System = Turtle();

		}

		// called every frame to move things
		void simulate() {
			
			// Method to iterate when pressing the key up
			generateTree();

		}

		// this is called to draw the world
		void draw_world(int x, int y, int w, int h) {

			int vx = 0, vy = 0;
			get_viewport_size(vx, vy);
			app_scene->begin_render(vx, vy);

			// update matrices. assume 30 fps.
			app_scene->update(1.0f / 30);

			// draw the scene
			app_scene->render((float)vx / vy);

			// We call simulate to start doing things
			simulate();
		}
	};

}