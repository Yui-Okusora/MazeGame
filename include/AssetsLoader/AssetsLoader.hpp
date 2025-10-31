#pragma once

#include <Core.hpp>

class AssetsLoader
{
public:
    static void test()
    {
        std::filesystem::path s;
        const std::filesystem::path root = "resources"; // your root directory

        try {
            for (const auto& entry : std::filesystem::recursive_directory_iterator(root)) {
                if (entry.is_regular_file()) {
                    std::cout << entry.path() << '\n';
                }
            }
        }
        catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << '\n';
        }
    }
private:

};