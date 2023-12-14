#include "classes.h"

uint64_t SizeFromBytes(std::ifstream& input) {
    char temp_char[4];
    for (int i = 0; i < 4; i++) {
        input.read(reinterpret_cast<char *>(&temp_char[i]), sizeof(temp_char[i]));
    }
    return (temp_char[0] << 21) | (temp_char[1] << 14) | (temp_char[2] << 7) | temp_char[3];
}

void Extended_Read(std::ifstream& input, ExtendedHeader& ext) {
    ext.ext_size_ = SizeFromBytes(input);
    char temp_char;
    input.read(reinterpret_cast<char *>(&temp_char), sizeof(temp_char));
    input.read(reinterpret_cast<char *>(&temp_char), sizeof(temp_char));
    if ((temp_char >> 5) & 1) {
        for (int i = 0; i < 5; i++) {
            input.read(reinterpret_cast<char *>(&temp_char), sizeof(temp_char));
            ext.crc_ = (*ext.crc_ << 8) + temp_char;
        }
    }
    if ((temp_char >> 4) & 1) {
        input.read(reinterpret_cast<char *>(&temp_char), sizeof(temp_char));
        ext.restrictions_ = temp_char;
    }
}

bool ID3Header::Is_Valid() const{
    return identifier_ && version_;
}
uint64_t ID3Header::Get_header_size() const{
    return size_;
}

ID3Header::ID3Header(std::ifstream& input) {
    unsigned char header_inf[header_size];
    for (int i = 0; i < header_size - 4; i++) {
        input.read(reinterpret_cast<char *>(&header_inf[i]), sizeof(header_inf[i]));
    }
    if (header_inf[0] != 'I' || header_inf[1] != 'D' || header_inf[2] != '3') {
        std::cerr << "Not ID3" << std::endl;
        return;
    }
    identifier_ = true;
    if (header_inf[3] != 4) {
        std::cerr << "Not v2.4" << std::endl;
        return;
    }
    version_ = true;
    unsynchronisation_ = (header_inf[5] >> 7) & 1;
    extended_ = (header_inf[5] >> 6) & 1;
    experimental_ = (header_inf[5] >> 5) & 1;
    footer_ = (header_inf[5] >> 4) & 1;
    if (footer_){
        size_ -= header_size;
    }
    size_ = SizeFromBytes(input) + header_size;

    if (extended_){
        Extended_Read(input, *this);
    }
}


