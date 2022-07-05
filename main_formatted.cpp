#include <iostream>
#include <fstream>
#include <sstream>
#include <windows.h>
#include <thread>
#include <array>
#include <chrono>
#include "Color.h"

WORD get_random_color()
{
    std::array<WORD, 16> colors = { BLACK, BLUE, CYAN, DARK_BLUE, DARK_GREEN, DARK_RED, GREEN, GREY, LIGHT_BLUE, LIGHT_GREY, MAGENTA, ORANGE, PINK, RED, WHITE, YELLOW };
    uint32_t index = std::rand() % colors.size();
    return colors[index];
}

std::string base64_encode(const std::string& in)
{

    std::string out;
    int val = 0, valb = -6;
    for (unsigned char c : in)
    {
        val = (val << 8) + c;
        valb += 8;
        while (valb >= 0)
        {
            out.push_back("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[(val >> valb) & 0x3F]);
            valb -= 6;
        }
    }
    if (valb > -6)
    {
        out.push_back("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[((val << 8) >> (valb + 8)) & 0x3F]);
    }
    while (out.size() % 4)
    {
        out.push_back('=');
    }
    return out;
}

void syrup(std::string content, std::string path, int cycles, bool* finished)
{
    std::string encoded_content = base64_encode(content);
    int cycles_finished = 0;
    std::string all_encoded_content = "import base64\nexec(base64.b64decode('" + encoded_content + "'))";
    for (int i = 0; i < cycles; i++)
    {
        std::string encoded_string = base64_encode(all_encoded_content);
        all_encoded_content = "import base64\nexec(base64.b64decode('" + encoded_string + "'))";
        cycles_finished++;
        std::string title = "[Syrup Obfuscation] Cycles Finished [" + std::to_string(cycles_finished) + "/" + std::to_string(cycles) + "]";
        SetConsoleTitleA(title.c_str());
    }

    std::ofstream file(path);
    file << all_encoded_content << std::endl;
    file.close();
    *finished = true;
}

int main()
{
    int cycles{};
    bool finished = false;
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    std::array<std::string, 4> spinners = { "[/]", "[-]", "[\\]", "[|]" };

    SetConsoleTextAttribute(handle, CYAN);
    std::cout << "Welcome to Syrup Obfuscation :3" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(3));

    SetConsoleTextAttribute(handle, GREEN);
    std::cout << "[+] Filename: ";
    std::string path;
    std::getline(std::cin, path);

    if (path.find(".py") == std::string::npos)
    {
        SetConsoleTextAttribute(handle, DARK_RED);
        std::cout << "[X] File given is not a python file." << std::endl;
        return 1;
    }

    std::ifstream file(path);
    if (!file.is_open())
    {
        SetConsoleTextAttribute(handle, DARK_RED);
        std::cout << "[X] File given could not be opened." << std::endl;
        return 1;
    }

    std::cout << "[!] File has been found!" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::string content = std::string(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
    file.close();
    if (content.empty())
    {
        SetConsoleTextAttribute(handle, DARK_RED);
        std::cout << "File empty. Cannot proceed." << std::endl;
        return 1;
    }

    std::cout << "[!] Read all content from the file!" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));

    std::cout << "[+] Cycles: ";
    std::cin >> cycles;


    std::cout << "[+] New File Path: ";
    std::string new_path;
    std::getline(std::cin, new_path);

    SetConsoleTextAttribute(handle, GREEN);

    std::cout << "[!] Starting now....";
    std::this_thread::sleep_for(std::chrono::seconds(1));
    
    std::thread obfuscator(syrup, content, path, cycles, &finished);
    while (!finished)
    {
        SetConsoleTextAttribute(handle, get_random_color());
        for (auto const& spinner : spinners)
        {
            std::cout << "\r" << spinner << "Obfuscating with " << cycles << " cycles.\r";
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
    SetConsoleTextAttribute(handle, CYAN);
    std::cout << "[!] Obfuscated your Python script :)" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(5));
    return 0;
}
