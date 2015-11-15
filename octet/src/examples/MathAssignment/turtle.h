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
	class Turtle {
	
		// Variables of the L-System
		string turtle_axiom;
		float turtle_angle;
		dynarray<string> turtle_rules;
		int turtle_iterations;

	public:
		
		// Empty Constructor
		Turtle(){}

		// Some useful get methods
		string getAxiom() { 
			return turtle_axiom;
		}
		float getAngle() {
			return turtle_angle;
		}
		int getIterations() {
			return turtle_iterations;
		}
		dynarray<string>& getRules(){
			return turtle_rules;
		}
		void set_rule(string rule) {
			turtle_rules.push_back(rule);
		}
		void set_axiom(string axiom) {
			turtle_axiom = axiom;
		}
		void set_iterations(int iterations) {
			turtle_iterations = iterations;
		}
		void set_angle(float angle) {
			turtle_angle = angle;
		}
		void reset_rules_array(){
			turtle_rules.reset();
		}
	};
}