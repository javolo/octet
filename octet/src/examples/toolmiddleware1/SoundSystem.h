#undef output 
#include <windows.h>
#include "fmod.hpp"
#include "fmod_errors.h"
#define output &scratch_r[bod->getNumDofs()]
#include <stddef.h>

// Code class taken from a tutorial of this page:
// https://cuboidzone.wordpress.com/2013/07/26/tutorial-implementing-fmod/
// http://stackoverflow.com/questions/19105190/fmod-channel-free-undefined-different-version-of-fmod
// http://stackoverflow.com/questions/35008426/link-3rd-party-library-in-visual-studio

namespace octet {

	typedef FMOD::Sound* SoundClass;

	class SoundSystemClass {
		// Pointer to the FMOD instance
		FMOD::System *m_pSystem;

	public:

		// Constructor of the class
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

		// Method that created the sound in the system
		void createSound(SoundClass *pSound, const char* pFile) {
			m_pSystem->createSound(pFile, FMOD_CREATESAMPLE, 0, pSound);
		}

		// Method that play the sound when it´s called
		FMOD::Channel* playSound(SoundClass pSound, bool bLoop = false) {
			if (!bLoop)
				pSound->setMode(FMOD_LOOP_OFF);
			else {
				pSound->setMode(FMOD_LOOP_NORMAL);
				pSound->setLoopCount(-1);
			}

			FMOD::Channel *channel = NULL;
			FMOD_RESULT result = m_pSystem->playSound(pSound, NULL, false, &channel);
			return channel;
		}

		// Method that released the sound
		void releaseSound(SoundClass pSound) {
			pSound->release();
		}
	};
}