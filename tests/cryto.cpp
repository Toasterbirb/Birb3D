#include "Crypto.hpp"
#include "Types.hpp"

#include <doctest/doctest.h>
#include <string>

TEST_CASE("Encryption and decryption")
{
	std::string plaintext;

	SUBCASE("Chars")
	{
		plaintext = "The quick brown fox jumps over the lazy dog 0123456789 åäö _:;,.-!\"#¤%&/()=?``@£$‰‚{[]}\\";
	}

	SUBCASE("json")
	{
		plaintext = R"~({
    "category": {
        "key": {
            "x": 42,
            "y": 24
        }
    }
})~";
	}

	SUBCASE("Lorem ipsum")
	{
		plaintext = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Pellentesque ipsum risus, sollicitudin finibus sapien ut, auctor blandit erat. Pellentesque tincidunt odio in neque consequat, ac interdum odio porttitor. Aenean nec auctor quam. Nulla porttitor in ante quis convallis. Donec rhoncus posuere dolor in congue. Quisque dignissim pretium libero, ac viverra nibh tempor vel. Quisque euismod lectus eros, sed efficitur metus egestas blandit. Curabitur venenatis sapien justo, sed porta massa tincidunt sit amet. Curabitur lobortis urna justo, vel imperdiet purus faucibus vel. Quisque condimentum nibh in mattis lobortis. Ut elementum, eros ac feugiat ullamcorper, nunc diam rutrum elit, vel egestas ante velit nec orci. Quisque eu hendrerit ipsum, ut blandit enim. Cras ut euismod leo, eget placerat libero. Morbi convallis in est id ullamcorper. Nulla imperdiet imperdiet urna, sed iaculis urna bibendum ac. Aliquam erat volutpat. Vivamus vel dignissim enim, ac tempor justo. Sed at egestas sem, quis sollicitudin lorem. Lorem ipsum dolor sit amet, consectetur adipiscing elit. Ut nec egestas ligula, vel scelerisque odio. Aliquam varius molestie odio sit amet facilisis. Ut egestas ligula nec sem luctus fermentum. Aenean pharetra facilisis nisl ut tristique. Curabitur aliquet blandit pretium. Aliquam feugiat vehicula mauris, ac mattis lacus. Aliquam sollicitudin tempor odio a sollicitudin. Ut consectetur quam massa, ac pretium enim suscipit eget. Mauris tempor vulputate mi id tincidunt. Sed et lorem lorem. Vivamus magna purus, dignissim id eros vitae, accumsan ultrices augue. Ut semper ex nec libero molestie porta. Nulla vel magna nec mauris placerat elementum. Nam fringilla augue at arcu malesuada, dapibus sagittis magna malesuada. Nulla sit amet ultrices quam. Sed auctor dolor at est pharetra, at iaculis sem semper. Nulla molestie ex id quam ullamcorper faucibus. Nullam vel suscipit lectus. Vestibulum ante ipsum primis in faucibus orci luctus et ultrices posuere cubilia curae; Duis suscipit ultrices massa eu dapibus. Sed mattis tristique tellus, at fringilla ex semper id. Phasellus tincidunt tempor nulla ut venenatis. Duis tempor vitae massa id ornare. Mauris risus neque, semper ac porttitor ac, accumsan et augue. Aliquam lacinia feugiat orci. Nunc mollis congue felis sed facilisis. Aliquam suscipit est ut varius bibendum. Fusce arcu augue, porttitor in ex ut, eleifend blandit urna. Sed varius lacus id ex porta auctor. Vivamus suscipit magna eu lectus faucibus, ut pretium elit facilisis. Cras iaculis at leo aliquet faucibus. Aliquam ullamcorper, mi posuere lobortis varius, ipsum lorem sodales dui, nec dignissim felis neque in tellus. Lorem ipsum dolor sit amet, consectetur adipiscing elit. In hac habitasse platea dictumst. Maecenas ultrices dolor sed vehicula pretium. Maecenas massa nulla, laoreet vel luctus eget, volutpat id diam. Vestibulum ante ipsum primis in faucibus orci luctus et ultrices posuere cubilia curae; Praesent nibh magna, tincidunt quis justo in, porttitor finibus turpis. Ut eget augue sapien. Morbi iaculis elementum nunc, id fermentum urna. Nunc ac pharetra libero, et commodo arcu. Curabitur viverra risus eu ultricies vehicula. Proin quis facilisis velit. Sed luctus molestie orci, et rhoncus urna lobortis interdum. Phasellus velit nisi, imperdiet at nibh pellentesque, molestie viverra turpis. Nullam et ligula a sem gravida tincidunt sed et leo. Nunc venenatis lacus est, sit amet consectetur dolor dictum quis. Praesent dapibus orci ante, eget rhoncus massa facilisis ut.";
	}

	SUBCASE("Random")
	{
		plaintext = "\033[1m9Crbsc1$+&2;7&wMb2Xbsarecw?\x23 &wMb2Xbsare\x02cutew`srvasKrecuteuWbip;qea`qUcuteuWb4Xbsa/O\"I\033[0m";
	}

	SUBCASE("Hex")
	{
		plaintext = { 0x76, 0x30, 0x12, 0x64, 0x7e, 0x62, 0x79, 0x79, 0x22, 0x69, 0x26, 0x75, 0x20, 0x28, 0x25, 0x23, 0x1b, 0x21, 0x0d, 0x51, 0x08, 0x00, 0x04, 0x1e, 0x4e, 0x13, 0x14, 0x08, 0x40, 0x16, 0x03, 0x4d, 0x12, 0x1e, 0x1e, 0x3a, 0x09, 0x1e, 0x0c, 0x45, 0x53 };
	}

	std::string ciphertext = birb::crypto::encrypt(plaintext);
	CHECK(plaintext != ciphertext);

	std::string decrypted_ciphertext = birb::crypto::decrypt(ciphertext);
	CHECK(plaintext == decrypted_ciphertext);

	// For extra measure, compare each character separately
	bool matching_chars = true;
	for (size_t i = 0; i < plaintext.size(); ++i)
	{
		if (static_cast<i32>(plaintext[i]) != static_cast<i32>(decrypted_ciphertext[i]))
		{
			matching_chars = false;
			break;
		}
	}
	CHECK(matching_chars);
}
