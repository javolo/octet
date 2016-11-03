#include "fmod.hpp"
#include "fmod_errors.h" // Only if you want error checking


// Code class taken from a tutorial of this page:
// https://cuboidzone.wordpress.com/2013/07/26/tutorial-implementing-fmod/

namespace octet {

	typedef FMOD::Sound* SoundClass;

	class SoundSystemClass {
		// Pointer to the FMOD instance
		FMOD::System *m_pSystem;

	public:

		SoundSystemClass() {
			if (FMOD::System_Create(&m_pSystem) != FMOD_OK) {
				// Report Error
				return;
			}

			int driverCount = 0;
			m_pSystem->getNumDrivers(&driverCount);

			if (driverCount == 0) {
				// Report Error
				return;
			}

			// Initialize our Instance with 36 Channels
			m_pSystem->init(36, FMOD_INIT_NORMAL, NULL);
		}
	}
}