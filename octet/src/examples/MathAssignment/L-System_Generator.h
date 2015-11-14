// Libraries to read the file
#include <iostream>
#include <fstream>
#include "turtle.h"
#include "sprite.h"
#include "point.h"

namespace octet {
	class L_System_Generator : public octet::app {

		// Matrix to transform points in our camera space to the world.
		// This lets us move our camera
		mat4t cameraToWorld;
		// PI Variable
		const float M_PI = 3.14159265358979323846264338328;
		ref<visual_scene> app_scene;
		// Variable turtle set the values in init
		Turtle L_System;
		// String variable
		std::string tree_string = "";
		// Variable iteration to see in which iteration we are
		int iteration = 1;
		std::vector<std::string> rules;
		std::string axiom;
		float current_angle = 0.0f;
		// Structure to store position and angle
		std::vector<Point> stored_points;
		// Variable of the current position and current angle
		// We´ve created a class for that and created an init method to set the parameters
		// at the beginning to 0
		Point current_point = Point();
		// Variable to set the length of the lines
		float line_length = 1.0f;
		// Definition of colours on top
		

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
			// We iterate through the String to draw the tree
			for (int i = 0; i < axiom.size(); i++){
				if (axiom[i] == 'F'){
					// F means draw forward (draw trunk)
					draw_trunk();
				}
				else if (axiom[i] == 'X'){
					// Don´t correspond with anything (let´s print a leaf)
					//draw_leaf();
					//break;
				}
				else if (axiom[i] == '['){
					// Store the point in a vector of points
					// Vectors stored elements at the end
					stored_points.push_back(current_point);
				}
				else if (axiom[i] == ']'){
					// Restore the position and angle previously saved
					// We want the last element of the vector as we want to retrieve the point in order
					current_point = stored_points[stored_points.size() - 1];
					stored_points.pop_back();
					// Removing elements keep the space in memory, we want to free this space
					stored_points.shrink_to_fit();
				}
				else if (axiom[i] == '-'){
					// '-' means "turn left 25 degrees", we change the sign and set the angle (Wikipedia L-System) 
					current_angle -= L_System.getAngle();
				}
				else if (axiom[i] == '+'){
					// '+' means "turn right 25 degrees", we set the angle as positive one (Wikipedia L-System) 
					current_angle += L_System.getAngle();
				}


			}
		}

		// We draw a trunk line
		void draw_trunk(){
			glClearColor(0.9f, 0.3f, 0, 1);
			glBegin(GL_LINES);
			glVertex3f(current_point.get_point_position_x(), current_point.get_point_position_y(), 0);
			update_current_point_position();
			glVertex3f(current_point.get_point_position_x(), current_point.get_point_position_y(), 0);
			glEnd();
		}

		// Method to upadte current point position
		void update_current_point_position(){
			// We use Pitagorean and the trigronometric relationships to obtain what x and y grow when 
			// draw a line depending on the current position
			printf("C Y B: %f\n", current_point.get_point_position_y());
			printf("C X B: %f\n", current_point.get_point_position_x());
			current_point.set_position_y(current_point.get_point_position_y() + (line_length*cos((current_angle)* (M_PI / 180))));
			current_point.set_position_x(current_point.get_point_position_x() + (line_length*sin((current_angle)* (M_PI / 180))));
			printf("C Y A: %f\n", current_point.get_point_position_y());
			printf("C X A: %f\n", current_point.get_point_position_x());
		}

		// We draw a leaf at the end of a branch
		void draw_leaf(){

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
			std::string rule1 = "F-[[X]+X]+F[+FX]-X";
			std::string rule2 = "FF";
			std::vector<std::string> turtle_rules;
			turtle_rules.push_back(rule1);
			turtle_rules.push_back(rule2);
			int iterations = 5;

			// We set all the parameters for the L-System
			L_System = Turtle(axiom, angle, turtle_rules, iterations);

		}

		// called every frame to move things
		void simulate() {
			
			// Method to iterate when pressing the keys
			management_system();
			// We draw the points all the time
			draw_tree();

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