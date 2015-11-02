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
	class turtle {
	
		// Variables of the L-System
		std::string turtle_axiom;
		float turtle_angle;
		std::vector<std::string> turtle_rules;
		int turtle_iterations;

	public:

		// Empty Constructor
		turtle(){}

		// Init method to set the parameters received
		void init(std::string axiom, float angle, std::vector<std::string> rules, int iterations){
			turtle_axiom = axiom;
			turtle_angle = angle;
			turtle_rules = rules;
			turtle_iterations = iterations;
		}

		// Some useful get methods
		std::string getAxiom() { 
			return turtle_axiom;
		}
		float getAngle() {
			return turtle_angle;
		}
		int getIterations() {
			return turtle_iterations;
		}
		std::vector<std::string> getRules(){
			return turtle_rules;
		}		
	};
}