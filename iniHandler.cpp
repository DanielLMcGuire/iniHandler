/**
 * @file iniHandler.cpp
 * @brief Implementation of Daniels INI Handler (MIT License)
 * @author Daniel McGuire
 */
#include "iniHandler.h"

bool IniHandler::writeSection(const std::string& section,
                      const std::vector<std::pair<std::string, std::string>>& entries)
{
    std::unordered_map<std::string, std::vector<std::pair<std::string, std::string>>> allData;
    readAll(allData);

    allData[section] = entries;

    std::ofstream out(iniPath);
    if(!out.is_open())
        return false;

    for(const auto& s : allData)
    {
        out << "[" << s.first << "]\n";
        for(const auto& kv : s.second)
            out << kv.first << "=" << kv.second << "\n";
        out << "\n";
    }
    return true;
}

bool IniHandler::readSection(const std::string& section,
                             std::vector<std::pair<std::string, std::string>>& outEntries)
{
    std::unordered_map<std::string, std::vector<std::pair<std::string, std::string>>> allData;
    if(!readAll(allData))
        return false;

    outEntries.clear();
    if(allData.count(section))
    {
        outEntries = allData[section];
        return true;
    }
    return false;
}

std::string IniHandler::readValue(const std::string& section, const std::string& key)
{
    std::unordered_map<std::string,
        std::vector<std::pair<std::string, std::string>>> allData;

    if(!readAll(allData))
        return "";

    if(!allData.count(section))
        return "";

    for(const auto& kv : allData[section])
    {
        if(kv.first == key)
            return kv.second;
    }
    return "";
}

bool IniHandler::readAll(std::unordered_map<std::string, std::vector<std::pair<std::string, std::string>>>& outData)
{
    std::ifstream in(iniPath);
    if(!in.is_open())
        return false;

    outData.clear();
    std::string line;
    std::string currentSection;

    while(std::getline(in, line))
    {
        if(line.empty())
            continue;

        if(line.front() == '[' && line.back() == ']')
        {
            currentSection = line.substr(1, line.size() - 2);
            continue;
        }

        auto pos = line.find('=');
        if(pos == std::string::npos)
            continue;

        std::string key = line.substr(0, pos);
        std::string value = line.substr(pos + 1);

        outData[currentSection].emplace_back(key, value);
    }
    return true;
}

bool IniHandler::writeValue(const std::string& section, const std::string& key, const std::string& value)
{
    std::vector<std::pair<std::string, std::string>> entries;
    readSection(section, entries);

    bool found = false;
    for (auto& kv : entries)
    {
        if (kv.first == key)
        {
            kv.second = value;
            found = true;
            break;
        }
    }

    if (!found)
        entries.emplace_back(key, value);

    return writeSection(section, entries);
}