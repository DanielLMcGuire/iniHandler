/**
 * @file iniHandler.cpp
 * @brief Implementation of Daniels INI Handler (MIT License)
 * @author Daniel McGuire
 */
#include "iniHandler.h"

bool IniHandler::writeSection(const iniSection& section)
{
    if (!readAll())
        return false;

    bool found = false;
    for (auto& s : file.sections)
    {
        if (s.name == section.name)
        {
            s.entries = section.entries;
            found = true;
            break;
        }
    }

    if (!found)
        file.sections.push_back(section);

    std::ofstream out(file.path);
    if (!out.is_open())
        return false;

    for (const auto& s : file.sections)
    {
        out << "[" << s.name << "]\n";
        for (const auto& entry : s.entries)
            out << entry.name << "=" << entry.value << "\n";
        out << "\n";
    }
    return true;
}

bool IniHandler::readSection(const iniSection& section)
{
    if (!readAll())
        return false;

    for (const auto& s : file.sections)
    {
        if (s.name == section.name)
            return !s.entries.empty();
    }

    return false;
}

std::string IniHandler::readEntry_str(const std::string& section, const iniEntry& key)
{
    return readEntry(section, key);
}

std::string IniHandler::readEntry(const std::string& section, const iniEntry& entry)
{
    if (!readAll())
        return "";

    for (const auto& s : file.sections)
    {
        if (s.name == section)
        {
            for (const auto& src_entry : s.entries)
            {
                if (src_entry.name == entry.name)
                    return src_entry.value;
            }
            break;
        }
    }
    return "";
}

bool IniHandler::readAll()
{
    std::ifstream in(file.path);
    if (!in.is_open())
        return false;

    file.sections.clear();
    std::string line;
    iniSection* currentSection = nullptr;

    while (std::getline(in, line))
    {
        if (line.empty())
            continue;

        if (line.front() == '[' && line.back() == ']')
        {
            file.sections.push_back({ line.substr(1, line.size() - 2), {} });
            currentSection = &file.sections.back();
            continue;
        }

        auto pos = line.find('=');
        if (pos == std::string::npos || !currentSection)
            continue;

        std::string key = line.substr(0, pos);
        std::string value = line.substr(pos + 1);

        currentSection->entries.push_back({ key, value });
    }
    return true;
}

bool IniHandler::writeEntry_str(const std::string& section, const std::string& key, const std::string& value)
{
    if (!readAll())
        return false;

	IniHandler::iniEntry entry{ key, value };
	return writeEntry(section, entry);
}

bool IniHandler::writeEntry(const std::string& section, const iniEntry& entry)
{
    if (!readAll())
        return false;
    iniSection* targetSection = nullptr;
    for (auto& s : file.sections)
    {
        if (s.name == section)
        {
            targetSection = &s;
            break;
        }
    }
    if (!targetSection)
    {
        file.sections.push_back({ section, {} });
        targetSection = &file.sections.back();
    }
    bool found = false;
    for (auto& e : targetSection->entries)
    {
        if (e.name == entry.name)
        {
            e.value = entry.value;
            found = true;
            break;
        }
    }
    if (!found)
        targetSection->entries.push_back(entry);
    std::ofstream out(file.path);
    if (!out.is_open())
        return false;
    for (const auto& s : file.sections)
    {
        out << "[" << s.name << "]\n";
        for (const auto& e : s.entries)
            out << e.name << "=" << e.value << "\n";
        out << "\n";
    }
    return true;
}