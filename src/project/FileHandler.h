#pragma once
#include <string>
#include <filesystem>
#include <fstream>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

template <typename T>
struct ProjectResult
{
    bool ok = false;
    T value{};
    std::string error;

    static ProjectResult<T> Ok(T value)
    {
        ProjectResult<T> result;
        result.ok = true;
        result.value = std::move(value);
        return result;
    }

    static ProjectResult<T> Error(std::string message)
    {
        ProjectResult<T> result;
        result.ok = false;
        result.error = std::move(message);
        return result;
    }
};

template <>
struct ProjectResult<void>
{
    bool ok = false;
    std::string error;

    static ProjectResult<void> Ok()
    {
        ProjectResult<void> result;
        result.ok = true;
        return result;
    }

    static ProjectResult<void> Error(std::string message)
    {
        ProjectResult<void> result;
        result.ok = false;
        result.error = std::move(message);
        return result;
    }
};

class FileHandler {
public:
    template <typename T>
    static ProjectResult<void> Save(
        const std::filesystem::path& path,
        const T& value
    ) 
    {
        try
        {
            json j = value;

            std::filesystem::create_directories(path.parent_path());

            std::ofstream out(path, std::ios::binary | std::ios::trunc);

            if (!out)
                return ProjectResult<void>::Error("Could not open file for writing.");

            out << j.dump(4);

            if (!out)
                return ProjectResult<void>::Error("Could not write JSON file.");

            return ProjectResult<void>::Ok();
        }
        catch (const std::exception& e)
        {
            return ProjectResult<void>::Error(e.what());
        }
    }
    template <typename T>
    static ProjectResult<T> Load(
        const std::filesystem::path& path) 
    {
        try
        {
            std::ifstream in(path, std::ios::binary);

            if (!in)
                return ProjectResult<T>::Error("Could not open file for reading.");

            json j;
            in >> j;

            T value = j.get<T>();

            return ProjectResult<T>::Ok(std::move(value));
        }
        catch (const std::exception& e)
        {
            return ProjectResult<T>::Error(e.what());
        }
    }

    static bool CreateFolder(const std::filesystem::path& path) {
        //todo use try catch
        return std::filesystem::create_directories(path);
    }

	static bool CreateFile(const std::filesystem::path& path) {
		try {
            //todo doesn't create file???
			std::filesystem::create_directories(path.parent_path());
			std::ofstream out(path);
			return out.good();
		}
		catch (const std::exception& e) {
			// Handle exceptions if necessary
			return false;
		}
	}
};