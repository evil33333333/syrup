#include <iostream>
#include <fstream>
#include <sstream>
#include <windows.h>
#include <thread>

static const std::string base64_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ" "abcdefghijklmnopqrstuvwxyz" "0123456789+/";

unsigned short RED_COLOR = 12;
unsigned short GREEN_COLOR = 10;
unsigned short CYAN_COLOR = 11;
unsigned short YELLOW_COLOR = 14;
bool finished = false;

static inline bool is_base64(unsigned char c)
{
	return (isalnum(c) || (c == '+') || (c == '/'));
}

/*
std::string encode(unsigned char
	const* bytes_to_encode, unsigned int in_len)
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
			char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
			char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
			char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
			char_array_4[3] = char_array_3[2] & 0x3f;

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

		char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
		char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
		char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
		char_array_4[3] = char_array_3[2] & 0x3f;

		for (j = 0;
			(j < i + 1); j++)
			ret += base64_chars[char_array_4[j]];

		while ((i++ < 3))
			ret += '=';

	}

	return ret;
}
*/
int GetRandomColor() {
	int random_number = std::rand() % 5;
	if (random_number == 0) return RED_COLOR;
	if (random_number == 1) return GREEN_COLOR;
	if (random_number == 2) return CYAN_COLOR;
	if (random_number == 3) return YELLOW_COLOR;
	if (random_number == 4) GetRandomColor();
}

static std::string encode(const std::string& in) {

	std::string out;
	int val = 0, valb = -6;
	for (unsigned char c : in) {
		val = (val << 8) + c;
		valb += 8;
		while (valb >= 0) {
			out.push_back("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[(val >> valb) & 0x3F]);
			valb -= 6;
		}
	}
	if (valb > -6) out.push_back("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[((val << 8) >> (valb + 8)) & 0x3F]);
	while (out.size() % 4) out.push_back('=');
	return out;
}

void syrup(std::string content, std::string path, int cycles)
{
	std::string encoded_content = encode(content); int cycles_finished = 0;
	std::string all_encoded_content = "import base64\nexec(base64.b64decode('" + encoded_content + "'))";
	for (int i = 0; i < cycles; i++)
	{
		std::string encoded_string = encode(all_encoded_content);
		all_encoded_content = "import base64\nexec(base64.b64decode('" + encoded_string + "'))";
		cycles_finished++;  std::string title = " title [Syrup Obfuscation] Cycles Finished [" + std::to_string(cycles_finished) + "/" + std::to_string(cycles) + "]";
		system(title.c_str());
	}
	std::ofstream file;
	file.open(path);
	file << all_encoded_content << std::endl;
	file.close();
	finished = true;
}

int main()
{
	HANDLE handle;
	handle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(handle, CYAN_COLOR);
	std::cout << "Welcome to Syrup Obfuscation :3" << std::endl;
	std::this_thread::sleep_for(std::chrono::seconds(3));
	SetConsoleTextAttribute(handle, GREEN_COLOR);
	std::cout << "Path: "; std::string path; std::cin >> path;
	if (path.find(".py") == std::string::npos)
	{
		SetConsoleTextAttribute(handle, RED_COLOR);
		std::cout << "File given is not a python file." << std::endl;
		return 1;
	}

	std::ifstream file(path);
	if (!file.is_open())
	{
		SetConsoleTextAttribute(handle, RED_COLOR);
		std::cout << "File given could not be opened." << std::endl;
		return 1;
	}

	std::string content = std::string(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
	file.close();
	if (content.compare("") == 0)
	{
		SetConsoleTextAttribute(handle, RED_COLOR);
		std::cout << "File empty. Cannot proceed." << std::endl;
		return 1;
	}
	std::cout << "Cycles: "; int cycles; std::cin >> cycles;
	SetConsoleTextAttribute(handle, GREEN_COLOR);
	std::thread obfuscator(syrup, content, path, cycles);
	while (!finished) 
	{
		SetConsoleTextAttribute(handle, GetRandomColor());
		std::cout << "\r[/] Obfuscating with " << cycles << " cycles.\r"; std::cout << "\r[-] Obfuscating with " << cycles << " cycles..\r"; std::cout << "\r[\\] Obfuscating with "<< cycles << " cycles...\r";
	}
	SetConsoleTextAttribute(handle, CYAN_COLOR);
	std::cout << "Obfuscated your Python script :)" << std::endl;
	std::this_thread::sleep_for(std::chrono::seconds(5));
	return 0;
}
