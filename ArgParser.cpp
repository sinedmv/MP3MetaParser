#include "ArgParser.h"

using namespace ArgumentParser;

ArgParser::Args& ArgParser::AddStringArgument(const char shortInput, const char* inputName, const char* inputDescr) {
    Args* temp = new Args;
    temp->shortName = shortInput;
    temp->longName = inputName;
    temp->description = inputDescr;
    temp->addStr = true;
    values.push_back(temp);
    return *temp;
}

ArgParser::Args& ArgParser::AddStringArgument(const char* inputName, const char* inputDescr) {
    Args* temp = new Args;
    temp->longName = inputName;
    temp->description = inputDescr;
    temp->addStr = true;
    values.push_back(temp);
    return *temp;
}

ArgParser::Args& ArgParser::AddIntArgument(const char shortInput, const char* inputName, const char* inputDescr) {
    Args* temp = new Args;
    temp->shortName = shortInput;
    temp->longName = inputName;
    temp->description = inputDescr;
    temp->addInt = true;
    values.push_back(temp);
    return *temp;
}

ArgParser::Args& ArgParser::AddIntArgument(const char* inputName, const char* inputDescr) {
    Args* temp = new Args;
    temp->longName = inputName;
    temp->description = inputDescr;
    temp->addInt = true;
    values.push_back(temp);
    return *temp;
}

ArgParser::Args& ArgParser::AddFlag(const char shortInput, const char* inputName, const char* inputDescr) {
    Args* temp = new Args;
    temp->shortName = shortInput;
    temp->longName = inputName;
    temp->description = inputDescr;
    temp->addFlag = true;
    temp->isFlag = true;
    values.push_back(temp);
    return *temp;
}

ArgParser::Args& ArgParser::AddFlag(const char* inputName, const char* inputDescr) {
    Args* temp = new Args;
    temp->longName = inputName;
    temp->description = inputDescr;
    temp->addFlag = true;
    values.push_back(temp);
    return *temp;
}

ArgParser::Args& ArgParser::AddHelp(const char shortInput, const char* inputName, const char* inputDescr) {
    Args* temp = new Args;
    temp->shortName = shortInput;
    temp->longName = inputName;
    temp->description = inputDescr;
    temp->isHelp = true;
    helpIndex = values.size();
    values.push_back(temp);
    return *temp;
}

/////////////////////////////////////////////

std::string ArgParser::GetStringValue(const char* inputName) {
    for (int i = 0; i < values.size(); i++) {
        if (values[i]->longName == inputName) {
            return *values[i]->value_str;
        }
    }
    std::cerr << "Wrong name" << std::endl;
    std::exit(1);
}

std::string ArgParser::GetStringValue(const char inputName) {
    for (int i = 0; i < values.size(); i++) {
        if (values[i]->shortName == inputName) {
            return *values[i]->value_str;
        }
    }
    std::cerr << "Wrong name" << std::endl;
    std::exit(1);
}

std::string ArgParser::GetStringValue(const char* inputName, uint16_t pos) {
    for (int i = 0; i < values.size(); i++) {
        if (values[i]->longName == inputName) {
            return values[i]->multi_str[pos];
        }
    }
    std::cerr << "Wrong name" << std::endl;
    std::exit(1);
}

int ArgParser::GetIntValue(const char* inputName) {
    for (int i = 0; i < values.size(); i++) {
        if (values[i]->longName == inputName) {
            return *values[i]->value_int;
        }
    }
    std::cerr << "Wrong name" << std::endl;
    std::exit(1);
}

int ArgParser::GetIntValue(const char inputName) {
    for (int i = 0; i < values.size(); i++) {
        if (values[i]->shortName == inputName) {
            return *values[i]->value_int;
        }
    }
    std::cerr << "Wrong name" << std::endl;
    std::exit(1);
}
int ArgParser::GetIntValue(const char* inputName, uint16_t pos) {
    for (int i = 0; i < values.size(); i++) {
        if (values[i]->longName == inputName) {
            return values[i]->multi_int[pos];
        }
    }
    std::cerr << "Wrong name" << std::endl;
    std::exit(1);
}

bool ArgParser::GetFlag(const char* inputName) {
    for (int i = 0; i < values.size(); i++) {
        if (values[i]->longName == inputName) {
            return *values[i]->flag;
        }
    }
    std::cerr << "Wrong name" << std::endl;
    std::exit(1);
}

bool ArgParser::GetFlag(const char inputName) {
    for (int i = 0; i < values.size(); i++) {
        if (values[i]->shortName == inputName) {
            return *values[i]->flag;
        }
    }
    std::cerr << "Wrong name" << std::endl;
    std::exit(1);
}

/////////////////////////////////////////////

void ArgParser::HelpDescription() {
    std::cout << parsName << '\n';
    if (helpIndex != std::nullopt) {
        std::cout << values[*helpIndex]->description << '\n';
    }
    std::cout << '\n';
    for (int i = 0; i < values.size(); i++) {
        if (values[i]->shortName != ' ') {
            std::cout << '-' << values[i]->shortName << " ";
        }
        std::cout << "--" << values[i]->longName << " ";
        std::cout << values[i]->description << '\n';
    }
}

bool ArgParser::Help() {
    return true;
}

bool ArgParser::Parse(const std::vector<std::string>& parse_values) {
    //Checking for positional
    std::optional<int> positional;
    for (int i = 0; i < values.size(); i++) {
        if (values[i]->isPos) {
            positional = i;
            break;
        }
    }

    for (int i = 1; i < parse_values.size(); i++) {
        if (parse_values[i][0] == '-') { //Checking for command;
            if (parse_values[i].size() < 2) {
                std::cerr << "Wrong command" << std::endl;
                HelpDescription();
                return false;
            }
            int j = 0;
            std::string temp;
            while (parse_values[i][j] != '=' && j < parse_values[i].size()) {
                if (parse_values[i][j] != '-') {
                    temp += parse_values[i][j];
                }
                j++;
            }
            j++;
            std::string inputValue;
            for (; j < parse_values[i].size(); j++) {
                inputValue += parse_values[i][j];
            }
            std::string longName;
            std::string shortNames;
            if (parse_values[i][1] == '-') { //Checking for longName or shortName;
                longName = temp;
            } else {
                shortNames = temp;
            }
            if (!inputValue.empty()) {

                for (int t = 0; t < values.size(); t++) {

                    if (values[t]->longName == longName || CheckForLetter(shortNames, values[t]->shortName)) {
                        ParseStrOrInt(t, inputValue);
                    }
                }
            } else {

                for (int t = 0; t < values.size(); t++) {

                    if (values[t]->longName == longName || CheckForLetter(shortNames, values[t]->shortName)) {
                        if (!values[t]->isFlag && !values[t]->isHelp) {
                            std::cerr << "Command without value";
                            HelpDescription();
                            return false;
                        }

                        if (values[t]->isFlag) {
                            values[t]->addFlag = false;
                            values[t]->flag = true;

                            if (values[t]->store_flag != std::nullopt) {
                                **values[t]->store_flag = *values[t]->flag;
                            }
                        } else {
                            HelpDescription();
                            return true;
                        }
                    }
                }
            }
        } else if (positional != std::nullopt) {
            ParseStrOrInt(*positional, parse_values[i]);
        }
    }

    for (int i = 0; i < values.size(); i++) {
        if (values[i]->addStr || values[i]->addInt || values[i]->addFlag) {
            return false;
        }
    }
    return true;
}

bool ArgParser::Parse(int argc, char** parse_values){
    //Checking for positional
    std::optional<int> positional;
    for (int i = 0; i < values.size(); i++) {
        if (values[i]->isPos) {
            positional = i;
            break;
        }
    }

    for (int i = 1; i < argc; i++) {
        if (parse_values[i][0] == '-') { //Checking for command;
            if (argc < 2) {
                std::cerr << "Wrong command" << std::endl;
                HelpDescription();
                return false;
            }
            int j = 0;
            std::string temp;
            while (parse_values[i][j] != '=' && j < argc) {
                if (parse_values[i][j] != '-') {
                    temp += parse_values[i][j];
                }
                j++;
            }
            j++;
            std::string inputValue;
            for (; j < argc; j++) {
                inputValue += parse_values[i][j];
            }
            std::string longName;
            std::string shortNames;
            if (parse_values[i][1] == '-') { //Checking for longName or shortName;
                longName = temp;
            } else {
                shortNames = temp;
            }
            if (!inputValue.empty()) {

                for (int t = 0; t < values.size(); t++) {

                    if (values[t]->longName == longName || CheckForLetter(shortNames, values[t]->shortName)) {
                        ParseStrOrInt(t, inputValue);
                    }
                }
            } else {

                for (int t = 0; t < values.size(); t++) {

                    if (values[t]->longName == longName || CheckForLetter(shortNames, values[t]->shortName)) {
                        if (!values[t]->isFlag && !values[t]->isHelp) {
                            std::cerr << "Command without value";
                            HelpDescription();
                            return false;
                        }

                        if (values[t]->isFlag) {
                            values[t]->addFlag = false;
                            values[t]->flag = true;

                            if (values[t]->store_flag != std::nullopt) {
                                **values[t]->store_flag = *values[t]->flag;
                            }
                        } else {
                            HelpDescription();
                            return true;
                        }
                    }
                }
            }
        } else if (positional != std::nullopt) {
            ParseStrOrInt(*positional, parse_values[i]);
        }
    }

    for (int i = 0; i < values.size(); i++) {
        if (values[i]->addStr || values[i]->addInt || values[i]->addFlag) {
            return false;
        }
    }
    return true;
}

/////////////////////////////////////////////

ArgParser::Args& ArgParser::Args::Default(const char* input) {
    value_str = input;
    addStr = false;
    return *this;
}

ArgParser::Args& ArgParser::Args::Default(const bool input) {
    flag = input;
    addFlag = false;
    return *this;
}

ArgParser::Args& ArgParser::Args::StoreValue(std::string& inputStore) {
    store_str = &inputStore;
    return *this;
}

ArgParser::Args& ArgParser::Args::StoreValue(int& inputStore) {
    store_int = &inputStore;
    return *this;
}

ArgParser::Args& ArgParser::Args::StoreValues(std::vector<std::string>& inputStore) {
    store_multi_str = &inputStore;
    return *this;
}

ArgParser::Args& ArgParser::Args::StoreValues(std::vector<int>& inputStore) {
    store_multi = &inputStore;
    return *this;
}

ArgParser::Args& ArgParser::Args::StoreValue(bool& inputStore) {
    store_flag = &inputStore;
    return *this;
}

ArgParser::Args& ArgParser::Args::MultiValue(uint16_t count) {
    minArgsCount = count;
    return *this;
}

ArgParser::Args& ArgParser::Args::Positional() {
    isPos = true;
    return *this;
}

/////////////////////////////////////////////

bool ArgParser::CheckForLetter(const std::string& input, const char ch) {
    for (int i = 0; i < input.length(); i++) {
        if (input[i] == ch) {
            return true;
        }
    }
    return false;
}

void ArgParser::ParseStrOrInt(const uint16_t& pos, const std::string& value) {
    if (values[pos]->minArgsCount == std::nullopt) {
        if (values[pos]->store_str == std::nullopt) {
            values[pos]->value_str = value;
            values[pos]->addStr = false;

        } else {

            **values[pos]->store_str = value;
            values[pos]->value_str = value;
            values[pos]->addStr = false;

        }
    } else {
        values[pos]->multi_str.push_back(value);
        if (values[pos]->multi_str.size() >= values[pos]->minArgsCount) {
            values[pos]->addStr = false;
        }
        if (values[pos]->store_multi_str != std::nullopt){
            **values[pos]->store_multi_str = values[pos]->multi_str;
        }
    }
}

//End