#include <iostream>
#include <fstream>
#include <sstream>
#include <lib/base64.h>
#include <windows.h>
#include <thread>

static
const std::string base64_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
"abcdefghijklmnopqrstuvwxyz"
"0123456789+/";

unsigned short RED_COLOR = 12;
unsigned short GREEN_COLOR = 10;
unsigned short CYAN_COLOR = 11;

static inline bool is_base64(unsigned char c)
{
	return (isalnum(c) || (c == '+') || (c == '/'));
}

std::string encode(unsigned char
	const *bytes_to_encode, unsigned int in_len)
{
	std::string ret;
	int i = 0;
	int j = 0;
	unsigned char char_array_3[3];
	unsigned char char_array_4[4];

	while (in_len--)
	{
		char_array_3[i++] = *(bytes_to_encode++);
		if (i == 3)
		{
			char_array_4[0] = (char_array_3[0] &0xfc) >> 2;
			char_array_4[1] = ((char_array_3[0] &0x03) << 4) + ((char_array_3[1] &0xf0) >> 4);
			char_array_4[2] = ((char_array_3[1] &0x0f) << 2) + ((char_array_3[2] &0xc0) >> 6);
			char_array_4[3] = char_array_3[2] &0x3f;

			for (i = 0;
				(i < 4); i++)
				ret += base64_chars[char_array_4[i]];
			i = 0;
		}
	}

	if (i)
	{
		for (j = i; j < 3; j++)
			char_array_3[j] = '\0';

		char_array_4[0] = (char_array_3[0] &0xfc) >> 2;
		char_array_4[1] = ((char_array_3[0] &0x03) << 4) + ((char_array_3[1] &0xf0) >> 4);
		char_array_4[2] = ((char_array_3[1] &0x0f) << 2) + ((char_array_3[2] &0xc0) >> 6);
		char_array_4[3] = char_array_3[2] &0x3f;

		for (j = 0;
			(j < i + 1); j++)
			ret += base64_chars[char_array_4[j]];

		while ((i++<3))
			ret += '=';

	}

	return ret;
}

int main()
{
	HANDLE handle;
	handle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(handle, GREEN_COLOR);
	std::cout << "Path: " << std::endl;
	std::string path;
	std::cin >> path;
	if (path.find(".py") == std::string::npos)
	{
		SetConsoleTextAttribute(handle, RED_COLOR);
		std::cout << "File given is not a python file." << std::endl;
		return 1;
	}

	std::ifstream file;
	if (!file.is_open())
	{
		SetConsoleTextAttribute(handle, RED_COLOR);
		std::cout << "File given could not be opened." << std::endl;
		return 1;
	}

	std::string content = std::string(std::istreambuf_iterator<char> (file), std::istreambuf_iterator<char> ());
	file.close();
	if (content.compare("") == 0)
	{
		SetConsoleTextAttribute(handle, RED_COLOR);
		std::cout << "File empty. Cannot proceed." << std::endl;
		return 1;
	}

	SetConsoleTextAttribute(handle, GREEN_COLOR);
	std::cout << "Obfuscating..." << std::endl;
	syrup(content, path);
	SetConsoleTextAttribute(handle, CYAN_COLOR);
	std::cout << "Obfuscated your Python script :)" << std::endl;
	std::this_thread::sleep_for(std::chrono::seconds(5));
	return 0;
}

void syrup(std::string content, std::string path)
{
	const unsigned char *content_bytes = reinterpret_cast<const unsigned char*> (content.c_str());
	std::string encoded_content = encode(content_bytes, sizeof(content_bytes));
	std::string all_encoded_content = "import base64\nexec(base64.b64decode('" + encoded_content + "'))";
	for (int i = 0; i < 199; i++)
	{
		const unsigned char *encoded_bytes = reinterpret_cast<const unsigned char*> (all_encoded_content.c_str());
		std::string encoded_string = encode(encoded_bytes, sizeof(encoded_bytes));
		all_encoded_content = "import base64\nexec(base64.decode('" + encoded_string + "'))";
	}

	std::ofstream file;
	file.open(path);
	file << all_encoded_content << std::endl;
	file.close();
}