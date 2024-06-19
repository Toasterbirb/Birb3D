#include "Crypto.hpp"
#include "Types.hpp"

#include <limits>

namespace birb
{
	namespace crypto
	{
		std::string cipher(std::string data);

		// Two keys with different lengths are used to avoid repeating patterns
		static const std::string key = "BIRBS_ARE_CUTE_UwU!";
		static const std::string key2 = "This thing probably doesn't need two keys but oh well";

		static constexpr i8 counter_wraparound = std::numeric_limits<char>().max();

		// Since the "encryption" is just XOR, the same cipher function
		// works both ways. However there are separate functions for
		// encryption and decryption to avoid confusion

		std::string cipher(std::string data)
		{
			for (size_t i = 0; i < data.size(); ++i)
			{
				data[i] ^= key.at(i % (key.size() - 1));
				data[i] ^= key2.at(i % (key2.size() - 1));
				data[i] ^= (i + data.size()) % counter_wraparound;
			}

			return data;
		}

		std::string encrypt(std::string plaintext)
		{
			return cipher(plaintext);
		}

		std::string decrypt(std::string ciphertext)
		{
			return cipher(ciphertext);
		}
	}
}
