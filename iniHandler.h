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
        : iniPath(filePath)
    {
        if (!std::filesystem::exists(iniPath))
        {
            std::ofstream createFile(iniPath);
        }
    }

    /**
     * @brief Writes a full section and its key/value pairs.
     *
     * If the section already exists, it will be replaced.
     *
     * @param section Name of the INI section (without brackets).
     * @param entries Vector of key-value pairs to write.
     * @return true when successful, false otherwise.
     *
     * @code
     * std::vector<std::pair<std::string, std::string>> entries =
     * {
     *     { "Resolution", "1920x1080" },
     *     { "Fullscreen", "true" }
     * };
     * handler.writeSection("Graphics", entries);
     * @endcode
     */
    bool writeSection(const std::string& section,
        const std::vector<std::pair<std::string, std::string>>& entries);

    /**
     * @brief Reads an entire section into a vector of key-value pairs.
     *
     * @param section Name of the section to read.
     * @param outEntries Storage for the result.
     * @return true if the section exists and was read, false if missing or empty.
     *
     * @code
     * std::vector<std::pair<std::string, std::string>> graphicsData;
     * if(handler.readSection("Graphics", graphicsData))
     * {
     *     for(auto& [key, value] : graphicsData)
     *     {
     *         std::cout << key << " = " << value << std::endl;
     *     }
     * }
     * @endcode
     */
    bool readSection(const std::string& section,
        std::vector<std::pair<std::string, std::string>>& outEntries);

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
     * @return true if the file does not exist or is empty.
     *
     * @code
     * if(handler.empty())
     *     std::cout << "Config is empty" << std::endl;
     * @endcode
     */
    bool empty() const
    {
        if (!std::filesystem::exists(iniPath))
            return true;

        std::ifstream in(iniPath, std::ios::ate);
        return in.tellg() == 0;
    }

private:
    std::filesystem::path iniPath;

    /**
     * @brief Internal helper that loads the entire INI file into a
     *        structure keyed by section name.
     *
     * @param outData Map storing each section and its key-value pairs.
     * @return true on success, false if the file could not be read or parsed.
     *
     * @code
     * std::unordered_map<std::string,
     *     std::vector<std::pair<std::string, std::string>>> data;
     * handler.readAll(data);
     * @endcode
     */
    bool readAll(std::unordered_map<std::string,
        std::vector<std::pair<std::string, std::string>>>& outData);
};
