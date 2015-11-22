////////////////////////////////////////////////////////////////////////////////
//
// (C) Javier de la Osa Escalada 2015
//
// Modular Framework for OpenGLES2 rendering on multiple platforms.
//
// Math Assignment: L-System
//
// Point class
//

namespace octet {
	class Point {

		// Variables of the L-System
		vec3 point_position;
		float point_angle;
		string point_type;

	public:

		// Empty Constructor with default values
		Point(){
			point_position = vec3(0, -0.9, 0);
			point_angle = 0.0f;
		}

		// Init method to set the parameters received
		Point(vec3 position, float angle, string type){
			point_position = position;
			point_angle = angle;
			point_type = type;
		}

		// Some useful get methods
		vec3 get_point_position() {
			return point_position;
		}
		float get_point_position_y(){
			return point_position.y();
		}
		float get_point_position_x(){
			return point_position.x();
		}
		float get_angle() {
			return point_angle;
		}
		string get_point_type(){
			return point_type;
		}
		void set_point_type(string type){
			point_type = type;
		}
		void set_angle(float angle) {
			point_angle = angle;
		}
		void set_position_y(float new_position){
			point_position.y() = new_position;
		}
		void set_position_x(float new_position){
			point_position.x() = new_position;
		}
	};
}