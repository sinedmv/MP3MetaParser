#pragma once

#include <iostream>
#include <vector>
#include <optional>

namespace ArgumentParser {

    class ArgParser {

    private:

        struct Args {
            bool addStr = false;
            bool addInt = false;
            bool addFlag = false;
            bool isFlag = false;
            bool isPos = false;
            bool isHelp = false;

            char shortName = ' ';
            std::string longName;
            std::string description;

            std::optional<std::string> value_str;
            std::optional<std::string*> store_str;

            std::vector<std::string> multi_str;
            std::optional<std::vector<std::string>*> store_multi_str;

            std::optional<int> value_int;
            std::optional<int*> store_int;

            std::vector<int> multi_int;
            std::optional<std::vector<int>*> store_multi;
            std::optional<uint16_t> minArgsCount;

            std::optional<bool> flag;
            std::optional<bool*> store_flag;

            Args& Default(const char* input);
            Args& Default(const bool input);
            Args& StoreValue(std::string& inputStore);
            Args& StoreValue(int& inputStore);
            Args& StoreValues(std::vector<std::string>& inputStore);
            Args& StoreValues(std::vector<int>& inputStore);
            Args& StoreValue(bool& inputStore);
            Args& MultiValue(uint16_t count = 0);
            Args& Positional();
        };

        std::string parsName;
        std::vector<Args*> values;
        std::optional<uint16_t> helpIndex;

        bool CheckForLetter(const std::string& input, const char ch);
        void ParseStrOrInt(const uint16_t& pos, const std::string& value);

    public:


        Args& AddStringArgument(const char shortInput = 'p', const char* inputName = "param", const char* inputDescr = "Smth");
        Args& AddStringArgument(const char* inputName = "param", const char* inputDescr = "Smth");
        Args& AddIntArgument(const char shortInput = 'p', const char* inputName = "param", const char* inputDescr = "Smth");
        Args& AddIntArgument(const char* inputName = "param", const char* inputDescr = "Smth");
        Args& AddFlag(const char shortInput = 'p', const char* inputName = "param", const char* inputDescr = "Smth");
        Args& AddFlag(const char* inputName = "param", const char* inputDescr = "Smth");
        Args& AddHelp(const char shortInput = 'p', const char* inputName = "param", const char* inputDescr = "Smth");


        std::string GetStringValue(const char* inputName);
        std::string GetStringValue(const char inputName);
        std::string GetStringValue(const char* inputName, uint16_t pos);
        int GetIntValue(const char* inputName);
        int GetIntValue(const char inputName);
        int GetIntValue(const char* inputName, uint16_t pos);
        bool GetFlag(const char* inputName);
        bool GetFlag(const char inputName);

        ArgParser(const char* inputName) {
            parsName = inputName;
        }

        void HelpDescription();
        bool Help();

        bool Parse(const std::vector<std::string>& parse_values);
        bool Parse(int argc, char** argv);
    };


} // namespace ArgumentParser