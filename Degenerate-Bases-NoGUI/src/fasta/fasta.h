#pragma once
#include "pch.h"
#include "../settings/settings.h"

namespace dgn
{
    class Fasta
    {
    public:
        Fasta() = delete;

    public:
        static void Open(const std::string& filepath, std::ios_base::openmode mode = std::ios_base::app)
        {
            if (!s_File.is_open())
                s_File.open(filepath, mode);
        }

        static void Close()
        {
            if (!s_File.is_open())
                return;

            s_File.close();
            index = 0;
        }

    public:
        static void CreateDefault()
        {
            using namespace std::filesystem;

            auto directory = Settings::Get("results", "directory");
            if (!is_directory(directory) || exists(directory))
                create_directory(directory);
        }

    public:
        static void Write(const std::string& data)
        {
            if (!s_File.is_open())
                return;

            const std::string header = Settings::Get("fasta", "header_character") + Settings::Get("fasta", "header_template");

            s_File << header << std::to_string(index) << '\n';
            s_File << '\n' << data << '\n';

            ++index;
        }

        static void Write(const std::vector<std::string>& data)
        {
            if (!s_File.is_open())
                return;

            const std::string header = Settings::Get("fasta", "header_character") + Settings::Get("fasta", "header_template");

            for (const auto& result : data)
            {
                s_File << header << std::to_string(index) << '\n';
                s_File << result << '\n';

                ++index;
            }
        }

    private:
        static inline std::fstream s_File;

    public:
        static inline size_t index = 0;
    };
}
