/**
 * @file iniHandler.h
 * @brief Daniel's INI Handler (MIT License)
 * @author Daniel McGuire
 */
#pragma once
#include <fstream>
#include <string>
#include <vector>
#include <utility>
#include <filesystem>
#include <unordered_map>

 /// @class IniHandler
 /// @brief Utility class for reading and writing INI style configuration files.
class IniHandler
{
public:
    /**
     * @brief Creates a handler for an INI file.
     *        If the file does not exist, it will be created.
     * @param filePath Absolute or relative path to the INI file.
     *
     * @code
     * IniHandler handler("config.ini");
     * @endcode
     */
    explicit IniHandler(const std::filesystem::path& filePath)
    {
        file.path = filePath;

        if (!std::filesystem::exists(file.path))
        {
            std::ofstream createFile(file.path);
        }
    }

    struct iniEntry {
        std::string name;
        std::string value;
    };

    struct iniSection {
        std::string name;
        std::vector<iniEntry> entries;
    };

    struct iniFile {
        std::filesystem::path path;
        std::vector<iniSection> sections;
    };

    /**
     * @brief Writes a full section and its entries.
     *
     * If the section already exists, it will be replaced.
     *
     * @param section The iniSection structure containing the section name and entries.
     * @return true when successful, false otherwise.
     *
     * @code
     * IniHandler::iniSection graphics;
     * graphics.name = "Graphics";
     * graphics.entries = {
     *     { "Resolution", "1920x1080" },
     *     { "Fullscreen", "true" }
     * };
     * handler.writeSection(graphics);
     * @endcode
     */
    bool writeSection(const iniSection& section);

    /**
     * @brief Checks if a section exists and contains entries.
     *
     * @param section The iniSection structure with the section name to check.
     * @return true if the section exists and has entries, false otherwise.
     *
     * @code
     * IniHandler::iniSection graphics;
     * graphics.name = "Graphics";
     * if(handler.readSection(graphics))
     * {
     *     std::cout << "Graphics section exists" << std::endl;
     * }
     * @endcode
     */
    bool readSection(const iniSection& section);

    /**
     * @brief Reads a single value from a specific section.
     *
     * @param section Section name.
     * @param key Key within the section.
     * @return Found value, or an empty string if the key or section does not exist.
     *
     * @code
     * std::string fullscreen = handler.readValue("Graphics", "Fullscreen");
     * @endcode
     */
    std::string readValue(const std::string& section, const std::string& key);

    /**
     * @brief Writes or updates a single value in a specific section.
     *
     * If the section or key does not exist, they are created.
     *
     * @param section Section name.
     * @param key Key to write.
     * @param value New value for the key.
     * @return true on successful write, false on file failure.
     *
     * @code
     * handler.writeValue("Graphics", "VSync", "false");
     * @endcode
     */
    bool writeValue(const std::string& section,
        const std::string& key,
        const std::string& value);

    /**
     * @brief Checks whether the INI file exists and contains data.
     *
     * @return true if the file does not exist or is empty, false otherwise.
     *
     * @code
     * if(handler.empty())
     *     std::cout << "Config is empty" << std::endl;
     * @endcode
     */
    bool empty() const
    {
        if (!std::filesystem::exists(file.path))
            return true;

        std::ifstream in(file.path, std::ios::ate);
        return in.tellg() == 0;
    }
private:
    iniFile file;

    /// Internal helper that loads the entire INI file into memory.
    bool readAll();
};
