// Libraries to read the file
#include <iostream>
#include <fstream>
#include "turtle.h"
#include "sprite.h"
#include "point.h"
#include "color.h"
#include <sstream>
#include "tinyxml2.h"
using namespace tinyxml2;

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
		float line_length = 0.10f;
		// Variable to control the line thickness
		float line_thickness = 1.0f;
		// Variable to set the file to upload. We start from 1
		int num_file = 1;
		// Boolean variable to see if we have scaled before
		boolean scaled = false;
		int times_scaled = 0;
		Colour colouring = Colour();
		vec3 color1;
		vec3 color2;

	public:

		// this is called when we construct the class
		L_System_Generator(int argc, char **argv) : app(argc, argv) {
		}

		// We generate the substring looking at the set of rules
		dynarray<char> substitute_string(dynarray<char>& result, char letter){

			dynarray<string> different_rules = decision_times(letter);
			if (different_rules.size() > 0){
					int random_rule = rand() % different_rules.size();
					// We push in the result array the letter of the rule
					for (int j = 3; j < different_rules[random_rule].size(); j++){
						result.push_back(different_rules[random_rule][j]);
					}
				}
			return result;
		}

		// Function to tell you how many times the letter repeat how decision branching
		dynarray<string> decision_times(char letter){
			dynarray<string> result;
			for (int i = 0; i < rules.size(); i++){
				int rule_position = rules[i].find("->");
				if (rule_position == 1){
					/*printf("LEFT: %c\n", rules[i][0]);
					printf("LET: %c\n", letter);*/
					if (rules[i][0] == letter){
						result.push_back(rules[i]);
					}
				}
			}
			return result;
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
			for (int i = 0; i < axiom.size(); i++){
				if (axiom[i] == 'X' || axiom[i] == 'F'){
					substitute_string(result, axiom[i]);
				}
				else {
					result.push_back(axiom[i]);
				}
			}
			result.push_back(0x00);
			printf("R: %s\n", string(result.data()));
			axiom = string(result.data());
		}

		// Interpretation and drawing of the 
		void intepret_tree_string(){
			tree_points.reset();
			current_point = Point();
			// We iterate through the String to draw the tree
			for (int i = 0; i < axiom.size(); i++){
				if (axiom[i] == 'F' || axiom[i] == 'G'){
					// F means draw forward (draw trunk)
					draw_trunk();
				}
				else if (axiom[i] == 'X'){
					// Don´t correspond with anything (let´s print a leaf)
					draw_leaf();
				}
				else if (axiom[i] == '['){
					// Store the point in a vector of points
					// Vectors stored elements at the end
					stored_points.push_back(current_point);
				}
				else if (axiom[i] == ']'){
					// Restore the position and angle previously saved
					// We want the last element of the vector as we want to retrieve the points in order
					current_point = stored_points[stored_points.size() - 1];
					stored_points.pop_back();					
				}
				else if (axiom[i] == '-'){
					// '-' means "turn left X degrees", we change the sign and set the angle (Wikipedia L-System) 
					current_point.set_angle(current_point.get_angle() + L_System.getAngle());
				}
				else if (axiom[i] == '+'){
					// '+' means "turn right X degrees", we set the angle as positive one (Wikipedia L-System) 
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
			current_point.set_point_type("trunk");
			tree_points.push_back(current_point);
			// We calculate the end position of the line
			update_current_point_position();
			// We store this point??
			tree_points.push_back(current_point);
		}

		// We draw a leaf at the end of a branch
		void draw_leaf(){
			// We store the first point
			current_point.set_point_type("leaf");
			tree_points.push_back(current_point);
			// We calculate the end position of the line
			update_current_point_position();
			// We store this point??
			tree_points.push_back(current_point);
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
			glClearColor(0, 0, 0, 1);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glLineWidth(line_thickness);
			glBegin(GL_LINES);
			
			for (int i = 0; i < tree_points.size(); i++){
				if (tree_points[i].get_point_type() == "trunk"){
					glColor3f(color1.x(), color1.y(), color1.z());
					glVertex3f(tree_points[i].get_point_position_x(), tree_points[i].get_point_position_y(), 0);
				}
				else if (tree_points[i].get_point_type() == "leaf"){
					glColor3f(color2.x(), color2.y(), color2.z());
					glVertex3f(tree_points[i].get_point_position_x(), tree_points[i].get_point_position_y(), 0);
				}
			}
			glEnd();
		}

		// We draw a trunk line
		void scale_tree(){
			// We have to check if the highest point in the tree is out of the screen to scale the tree
			float bottom_point_y;
			float top_point_y;

			// The bottom point will be the first point in the tree
			bottom_point_y = tree_points[0].get_point_position_y();
			top_point_y = tree_points[0].get_point_position_y();

			for (int i = 1; i < tree_points.size(); i++){
				if (tree_points[i].get_point_position_y() < bottom_point_y){
					bottom_point_y = tree_points[i].get_point_position_y();
				}
				if (tree_points[i].get_point_position_y() > top_point_y){
					top_point_y = tree_points[i].get_point_position_y();
				}
			}
			printf("TOP: %f\n", top_point_y);
			printf("BOTTOM: %f\n", bottom_point_y);
			// After discover the both points we set the init point of start the tree and the scale (factor * line_height)
			// We check if the top point is out of the screen > 1
			if (top_point_y > 1){
				line_length = line_length * 0.5f;
				// if we have scaled the tree we need to re-interpret the tree to fit in the page
				tree_generation();
				times_scaled++;
				// We find out  if we have to scale the tree to fit in the page
				scale_tree();
			}
			else if (top_point_y < 0 && times_scaled > 0){
				printf("TS: %i\n", times_scaled);
				line_length = line_length * 2.0f * times_scaled;
				// if we have scaled the tree we need to re-interpret the tree to fit in the page
				tree_generation();
				//times_scaled--;
				times_scaled = 0;
				// We find out  if we have to scale the tree to fit in the page
				scale_tree();
			}
			printf("LL: %f\n", line_length);
		}

		// Function to generate trees
		void tree_generation(){
			// First thing is to generate the new String
			for (int i = 0; i < num_iteration; i++){
				generate_tree_string();
			}
			// We interpret now the string generated
			intepret_tree_string();
		}

		// Use the keyboard to generate the tree
		void management_system() {
			// We want in key up to add another iteration to the tree and in key down to decrease this iteration
			if (is_key_down(key_up)) {
				// We need to generate the new String from the axiom and the rules and interpret this
				// We increase the number of iterations
				if (num_iteration < L_System.getIterations()){
					num_iteration++;
					// Call to tree generation function
					tree_generation();
					// We find out  if we have to scale the tree to fit in the page
					scale_tree();
				}
			}
			if (is_key_down(key_down)) {
				// We reduce the number of iterations and generate the tree
				if (num_iteration > 0){
					num_iteration--;
					// Call to tree generation function
					tree_generation();
					// We find out  if we have to scale the tree to fit in the page
					scale_tree();
				}
			}
			// Key left to load previous file
			if (is_key_down(key_left)) {
				// Decrease the number of file to load if we are not at the first one
				if (num_file > 1){
					num_file--;
					load_configuration_file();
				}
			}
			// Key right we load another file
			if (is_key_down(key_right)) {
				// Increase the number of the file if we are not exceeding the number of files
				if (num_file < 8){
					num_file++;
					load_configuration_file();
				}
			}
			// Key F1 to increase the angle in 5 degrees
			if (is_key_down(key_f1)) {
				L_System.set_angle(L_System.getAngle() + 5);
				// Call to tree generation function
				tree_generation();
			}
			// Key F2 to decrease the angle in 5 degrees
			if (is_key_down(key_f2)) {
				L_System.set_angle(L_System.getAngle() - 5);
				// Call to tree generation function
				tree_generation();
			}
			// Key F7 to increase the number of iterations
			if (is_key_down(key_f7)) {
				L_System.set_iterations(L_System.getIterations() + 1);
			}
			// Key F8 to decrease the number of iterations
			if (is_key_down(key_f8)) {
				// We check if the number of iteration is greater than 1 so the least number of iteration allowed is 1
				if (L_System.getIterations() > 1){
					L_System.set_iterations(L_System.getIterations() - 1);
				}
			}
			// A key to change randomly the colours
			if (is_key_down('A')) {
				// We generate a new pair of colours randomly
				color1 = colouring.get_random_color();
				color2 = colouring.get_random_color();
				// Call to tree generation function
				tree_generation();
			}
			// C key increase the line thickness
			if (is_key_down('C')) {
				// We increase the line thickness
				line_thickness += 1.0f;
				// Call to tree generation function
				tree_generation();
			}
			// V key increase the line thickness
			if (is_key_down('V')) {
				// We increase the line thickness
				line_thickness -= 1.0f;
				// Call to tree generation function
				tree_generation();
			}
		}

		// Load Configuration file Method depending
		void load_configuration_file() {
			//http://www.dinomage.com/2012/01/tutorial-using-tinyxml-part-1/
			// We reset the rules to avoid load previous ones
			L_System.reset_rules_array();
			// We reset as the num of iterations to start from the beginning
			num_iteration = 0;
			// We reset as well the line length, thickness and the times scaled variable because the previous tree
			// could have modified that
			line_length = 0.10f;
			line_thickness = 1.0f;
			times_scaled = 0;
			// First of all, we generate the file name
			std::string filename = "File";
			char file_number [2];
			itoa(num_file, file_number, 10);
			filename += file_number;
			filename += ".xml";
			printf("Filename: %s\n", filename.c_str());
			TiXmlDocument file;
			file.LoadFile(filename.c_str());
			// Once loaded the file we set the initial parameters
			L_System.set_axiom(file.FirstChildElement("LSystem")->FirstChildElement("Axiom")->GetText());
			L_System.set_angle(atof(file.FirstChildElement("LSystem")->FirstChildElement("Angle")->GetText()));
			L_System.set_iterations(atoi(file.FirstChildElement("LSystem")->FirstChildElement("Iterations")->GetText()));
			// We load the rules now
			for (TiXmlElement *elem = file.FirstChildElement("LSystem")->FirstChildElement("Rules")->FirstChildElement();
				elem != NULL; elem = elem->NextSiblingElement()) {
				printf("RL: %s\n", elem->ToElement()->GetText());
				L_System.set_rule(elem->ToElement()->GetText());
			}

		}


		// this is called once OpenGL is initialized
		void app_init() {
			app_scene = new visual_scene();
			app_scene->create_default_camera_and_lights();
			app_scene->get_camera_instance(0)->set_far_plane(100000.0f);

			// We load the first tree to start displaying something
			color1 = colouring.get_color(0);
			color2 = colouring.get_color(1);
			load_configuration_file();
		}

		// this is called to draw the world
		void draw_world(int x, int y, int w, int h) {

			// Method to iterate when pressing the keys
			management_system();
			// We draw the points all the time
			draw_tree();
		}
	};

}