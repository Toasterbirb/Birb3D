#pragma once

/////////////////////////////////////////////////////////
//*****************************************************//
//* These functions ARE NOT CRYPTOGRAPHICALLY SECURE! *//
//*****************************************************//
/////////////////////////////////////////////////////////

// The purpose of these functions is to obfuscate save game data
// and stuff. They are not meant to be secure in any measurable way.
//
// Remember to change the keys if you don't want the decryption tool
// shipped with birb3d to work directly on the save files. This should
// make it slightly more challenging to manipulate the save data. However
// if someone is dedicated enough to reverse engineer the binary, figuring
// out what the keys are is trivial. Heck, running `strings` on the binary
// is enough to extract the keys if you know what you are looking for.

#include <string>

namespace birb
{
	namespace crypto
	{
		/**
		 * @brief Encrypt a piece of text
		 */
		std::string encrypt(std::string plaintext);

		/**
		 * @brief Decrypt a piece of encrypted text
		 */
		std::string decrypt(std::string ciphertext);
	}
}
