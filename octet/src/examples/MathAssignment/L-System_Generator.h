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
		// String variable
		std::string tree_string = "";
		// Variable iteration to see in which iteration we are
		int iteration = 1;
		std::vector<std::string> rules;
		std::string axiom;
		

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

		// Generation of new String applying rules
		void generate_tree_string(){
			// We applied the rules to the different characters of the axiom String
			// Axiom variable (first iteration we load it)
			if (iteration == 1) {
				axiom = L_System.getAxiom();
				rules = L_System.getRules();
			}
			printf("SIZE: %i\n", rules.size());
			for (int i = 0; i < axiom.length(); i++){
				//printf("%c\n", axiom[i]);
				if (axiom[i] == 'X'){
					tree_string += rules[0];
				}
				else if (axiom[i] == 'F'){
					tree_string += rules[1];
				}
				else{
					tree_string += axiom[i];
				}
			}
			axiom = tree_string;
			printf("TREE: %s\n", tree_string.c_str());
			tree_string = "";
			iteration++;
			
		}

		// Interpretation and drawing of the 
		void intepret_tree_string(){

		}

		// Use the keyboard to generate the tree
		void management_system() {
			// We want in key up to add another iteration to the tree and in key down to decrease this iteration
			if (is_key_down(key_up)) {
				// We need to generate the new String from the axiom and the rules and interpret this 
				// First thing is to generate the new String
				generate_tree_string();
				// We interpret now the string generated
				intepret_tree_string();
			}


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
			turtle_rules.push_back(rule1);
			turtle_rules.push_back(rule2);
			int iterations = 5;

			L_System = Turtle(axiom, angle, turtle_rules, iterations);

		}

		// called every frame to move things
		void simulate() {
			
			// Method to iterate when pressing the keys
			management_system();

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