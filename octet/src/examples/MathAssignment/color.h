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
	class Colour {

		// Different kind of colours
		dynarray<vec3> colours;
		vec3 color0 = vec3(0.9f, 0.3f, 0.0f);
		vec3 color1 = vec3(0, 1, 0);
		vec3 color2 = vec3(0.2f, 0.7f, 0.2f);
		vec3 color3 = vec3(1, 0, 0);
		vec3 color4 = vec3(1.0f, 0.3f, 0.6f);
		vec3 color5 = vec3(0, 0, 1);
		vec3 color6 = vec3(0.0f, 0.6f, 0.6f);
		vec3 color7 = vec3(0.5f, 0.5f, 0.0f);
		vec3 color8 = vec3(0.3f, 0.3f, 0.3f);
		vec3 color9 = vec3(1.0f, 1.0f, 0.0f);
		

	public:

		// Empty Constructor with default values
		Colour(){
			colours.push_back(color0);
			colours.push_back(color1);
			colours.push_back(color2);
			colours.push_back(color3);
			colours.push_back(color4);
			colours.push_back(color5);
			colours.push_back(color6);
			colours.push_back(color7);
			colours.push_back(color8);
			colours.push_back(color9);
		}

		// Some useful get methods
		vec3 get_color(int num_color) {
			
			return colours[num_color];
		}
		vec3 get_random_color() {
			int randomizer = rand() % colours.size();
			return colours[randomizer];
		}
	};
}