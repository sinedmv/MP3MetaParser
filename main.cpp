#include <sstream>
#include <iterator>

#include "ArgParser.h"
#include "classes.h"

using namespace ArgumentParser;

int main(int argc, char** argv) {

    ArgParser parser("Parse");

    std::string file;
    parser.AddStringArgument('f', "file").StoreValue(file);

    std::vector<std::string> show;
    parser.AddStringArgument("show").MultiValue().StoreValues(show);

    parser.Parse(argc, argv);
    std::ifstream input(file, std::ifstream::binary);

    uint64_t begin_header = 0;
    while(true) {
        ID3Header head(input);

        if(!head.Is_Valid()){
            std::cerr << "Header is not valid" << std::endl;
            return 1;
        }

        while (input.tellg() < head.Get_header_size() + begin_header && input.tellg() != input.eof()) {
            Frame_Read(input, head, show);
        }
        if (head.seek_ == std::nullopt || input.tellg() != input.eof()){
            break;
        } else {
            input.seekg(head.Get_header_size(), std::ios_base::beg);
            input.seekg(*head.seek_, std::ios_base::cur);
            begin_header = input.tellg();
        }
    }
}
