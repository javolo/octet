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
		string tree_string;
		// Variable iteration to see in which iteration we are
		int num_iteration = 0;
		dynarray<string> rules;
		string axiom;
		float current_angle = 0.0f;
		// Structure to store position and angle depending on rules
		dynarray<Point> stored_points;
		// Structure to store all the points to be drawn
		dynarray<Point> tree_points;
		// Variable of the current position and current angle
		// We´ve created a class for that and created an init method to set the parameters
		// at the beginning to 0
		Point current_point = Point();
		// Variable to set the length of the lines
		float line_length = 0.1f;

	public:

		// this is called when we construct the class
		L_System_Generator(int argc, char **argv) : app(argc, argv) {
		}

		// Generation of new String applying rules
		void generate_tree_string(){
			// We applied the rules to the different characters of the axiom String
			// Axiom variable (first iteration we load it)
			dynarray<char> result;
			if (num_iteration == 1) {
				axiom = L_System.getAxiom();
				rules = L_System.getRules();
			}
			printf("SIZE: %i\n", rules.size());
			for (int i = 0; i < axiom.size(); i++){
				if (axiom[i] == 'X'){
					for (int j = 0; j < rules[0].size(); j++){
						result.push_back(rules[0][j]);
					}
				}
				else if (axiom[i] == 'F'){
					for (int j = 0; j < rules[0].size(); j++){
						result.push_back(rules[0][j]);
					}
				}
				else {
					result.push_back(axiom[i]);
				}
			}
			result.push_back(0x00);
			axiom = string(result.data());
			//axiom = tree_string;
			//printf("TREE: %s\n", axiom.c_str());	
		}

		// Interpretation and drawing of the 
		void intepret_tree_string(){
			tree_points.reset();
			current_point = Point();
			// We iterate through the String to draw the tree
			for (int i = 0; i < axiom.size(); i++){
				if (axiom[i] == 'F'){
					// F means draw forward (draw trunk)
					draw_trunk();
				}
				else if (axiom[i] == 'X'){
					// Don´t correspond with anything (let´s print a leaf)
					draw_trunk();
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
					
				}
				else if (axiom[i] == '-'){
					// '-' means "turn left 25 degrees", we change the sign and set the angle (Wikipedia L-System) 
					current_point.set_angle(current_point.get_angle() + L_System.getAngle());
				}
				else if (axiom[i] == '+'){
					// '+' means "turn right 25 degrees", we set the angle as positive one (Wikipedia L-System) 
					current_point.set_angle(current_point.get_angle() - L_System.getAngle());
				}
			}
			axiom = L_System.getAxiom();
		}

		// We draw a trunk line
		void draw_trunk(){
			// In this method we only store the point of the tree as in the draw world method each frame
			// it´s gonna paint the tree
			// We store the first point
			tree_points.push_back(current_point);
			// We calculate the end position of the line
			update_current_point_position();
			// We store this point??
			tree_points.push_back(current_point);
		}

		// We draw a leaf at the end of a branch
		void draw_leaf(){
			//glClearColor(1, 0.35f, 0.15f, 0.4f);
			glColor3f(0.9f, 0.3f, 0);
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
			current_point.set_position_y(current_point.get_point_position_y() + (line_length*cos((current_point.get_angle())* (M_PI / 180))));
			current_point.set_position_x(current_point.get_point_position_x() + (line_length*sin((current_point.get_angle())* (M_PI / 180))));
		}

		// We draw a trunk line
		void draw_tree(){

			// We print the tree
			glBegin(GL_LINES);
			glColor3f(0.9f, 0.3f, 0);
			for (int i = 0; i < tree_points.size(); i++){
				glVertex3f(tree_points[i].get_point_position_x(), tree_points[i].get_point_position_y(), 0);
			}
			glEnd();
		}

		// Use the keyboard to generate the tree
		void management_system() {
			// We want in key up to add another iteration to the tree and in key down to decrease this iteration
			if (is_key_down(key_up)) {
				// We need to generate the new String from the axiom and the rules and interpret this
				// We increase the number of iterations
				if (num_iteration <= L_System.getIterations()){
					num_iteration++;
					
					// First thing is to generate the new String
					for (int i = 0; i < num_iteration; i++){
						generate_tree_string();
					}
					// We interpret now the string generated
					intepret_tree_string();
				}
			}
			
			if (is_key_down(key_down)) {
				// We reduce the number of iterations and generate the tree
				if (num_iteration > 0){
					num_iteration--;
					// First thing is to generate the new String
					for (int i = 0; i < num_iteration; i++){
						generate_tree_string();
					}
					// We interpret now the string generated
					intepret_tree_string();
				}
			}
		}

		// this is called once OpenGL is initialized
		void app_init() {
			app_scene = new visual_scene();
			app_scene->create_default_camera_and_lights();
			app_scene->get_camera_instance(0)->set_far_plane(100000.0f);

			// We generate the parameter of L-System
			string axiom = "F";
			float angle = 25.7f;
	/*		string rule1 = "F-[[X]+X]+F[+FX]-X";
			string rule2 = "FF";
			int iterations = 5;*/

			/*string rule1 = "F[+X][-X]FX";
			string rule2 = "FF";
			int iterations = 7;*/

			string rule1 = "F[+F]F[-F]F";
			//string rule2 = "FF";
			int iterations = 5;

			L_System.set_rule(rule1);
			//L_System.set_rule(rule2);
			L_System.set_angle(angle);
			L_System.set_axiom(axiom);
			L_System.set_iterations(iterations);
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