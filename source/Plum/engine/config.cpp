#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "config.h"

namespace plum
{
    Config::Config(const std::string& name)
    {
        std::ifstream f(name);
        if(f.good())
        {
            std::string line;
            while(std::getline(f, line))
            {
                size_t index = line.find_first_of(" \t");
                std::string key, value;

                if(index == std::string::npos)
                {
                    key = line;
                }
                else
                {
                    key = line.substr(0, index);
                    value = line.substr(index + 1);
                }
                data[key] = value;
            }
        }
    }

    template<> std::string Config::get<std::string>(const std::string& key, std::string fallback)
    {
        auto it = data.find(key);
        return it != data.end() ? it->second : fallback;
    }

    template<> int Config::get<int>(const std::string& key, int fallback)
    {
        int value;
        std::istringstream stream(get<std::string>(key, ""));
        stream >> value;
        return !stream.fail() && stream.peek() == std::istringstream::traits_type::eof() ? value : fallback;
    }

    template<> bool Config::get<bool>(const std::string& key, bool fallback)
    {
        auto it = data.find(key);
        if(it != data.end())
        {
            if(it->second.size())
            {
                return get<int>(key, fallback) != 0;
            }
            else
            {
                return true;
            }
        }
        return fallback;
    }
}