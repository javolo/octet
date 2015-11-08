////////////////////////////////////////////////////////////////////////////////
//
// (C) Javier de la Osa Escalada 2015
//
// Modular Framework for OpenGLES2 rendering on multiple platforms.
//
// Math Assignment: L-System
//
// Turtle class
//

namespace octet {
	class Point {

		// Variables of the L-System
		vec3 point_position;
		float point_angle;

	public:

		// Empty Constructor
		Point(){
			point_position = vec3(0, 0, 0);
			point_angle = 0.0f;
		}

		// Init method to set the parameters received
		Point(vec3 position, float angle){
			point_position = position;
			point_angle = angle;
		}

		// Some useful get methods
		vec3 get_point_position() {
			return point_position;
		}
		float get_angle() {
			return point_angle;
		}
	};
}