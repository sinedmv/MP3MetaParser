#include <iostream>

#include "classes.h"

std::string Read_WhileNot_Zero8(std::ifstream& input, uint64_t& count, uint64_t frame_size){
    std::string temp;
    for (; count < frame_size; count++) {
        char temp_char;
        input.read(reinterpret_cast<char *>(&temp_char), sizeof(temp_char));
        if (temp_char == 0) {
            count++;
            break;
        }
        temp += temp_char;
    }
    return temp;
}

std::string Read_WhileNot_End8(std::ifstream& input, uint64_t& count, uint64_t frame_size){
    std::string temp;
    for (; count < frame_size; count++) {
        char temp_char;
        input.read(reinterpret_cast<char *>(&temp_char), sizeof(temp_char));
        temp += temp_char;
    }
    return temp;
}

std::wstring Read_WhileNot_Zero16(std::ifstream& input, uint64_t& count, uint64_t frame_size){
    std::wstring temp;
    for (; count < frame_size; count += 2) {
        wchar_t temp_char;
        input.read(reinterpret_cast<char *>(&temp_char), sizeof(temp_char));
        if (temp_char == 0) {
            break;
        }
        temp += temp_char;
    }
    return temp;
}

std::wstring Read_WhileNot_End16(std::ifstream& input, uint64_t& count, uint64_t frame_size){
    std::wstring temp;
    for (; count < frame_size; count += 2) {
        wchar_t temp_char;
        input.read(reinterpret_cast<char *>(&temp_char), sizeof(temp_char));
        temp += temp_char;
    }
    return temp;
}

std::string Read_Language(std::ifstream& input, uint64_t& count){
    std::string temp;
    for (int j = count; count < j + 3; count++) {
        char temp_char;
        input.read(reinterpret_cast<char *>(&temp_char), sizeof(temp_char));
        temp += temp_char;
    }
    return temp;
}

uint64_t StringToInt(std::string& str){
    uint64_t temp;
    for (int i = 0; i < str.size(); i++) {
        temp = (temp << 8) + str[str.size() - 1 - i];
    }
    return temp;
}

std::string What_Kind_encode(char& encode){
    if (encode == 0){
        return "ISO 8859-1";
    }
    if (encode == 1){
        return "UTF-16";
    }
    if (encode == 2){
        return "UTF-16BE";
    }
    return "UTF-8";
}

Language_Encode::Language_Encode(std::ifstream& input, uint64_t& count){
    input.read(reinterpret_cast<char *>(&encode), sizeof(encode));
    count++;
    language = Read_Language(input, count);
}


UFID::UFID(std::ifstream& input, const std::string& frame_name, const uint64_t frame_size, char flag1, char flag2):
FrameGeneral(frame_name, frame_size, flag1, flag2){
    uint64_t i = 0;
    UFID_owner_id = Read_WhileNot_Zero8(input, i, frame_size);
    UFID_id = Read_WhileNot_End8(input, i, frame_size);
}

USER::USER(std::ifstream &input, uint64_t& count, const std::string &frame_name, const uint64_t frame_size, char flag1, char flag2):
FrameGeneral(frame_name, frame_size, flag1, flag2), Language_Encode(input, count){
    if (encode == 0 || encode == 3) {
        USER_text = Read_WhileNot_End8(input, count, frame_size);
    } else {
        USER_wtext = Read_WhileNot_End16(input, count, frame_size);
    }
}

SYLT::SYLT(std::ifstream &input, uint64_t& count, const std::string &frame_name, const uint64_t frame_size, char flag1, char flag2):
FrameGeneral(frame_name, frame_size, flag1, flag2), Language_Encode(input, count) {
    input.read(reinterpret_cast<char *>(&format), sizeof(format));
    input.read(reinterpret_cast<char *>(&content), sizeof(content));
    count += 2;
    if(encode == 0 || encode == 3){
        SYLT_descr = Read_WhileNot_Zero8(input, count, frame_size);
        while(count < frame_size){
            std::string text = Read_WhileNot_Zero8(input, count, frame_size);
            uint32_t time = 0;
            for (int j = count; count < j + 4; count++){
                char temp_char;
                input.read(reinterpret_cast<char *>(&temp_char), sizeof(temp_char));
                time = (time << 8) + temp_char;
            }
            data.push_back(std::make_pair(text, time));
        }
    } else {
        SYLT_wdescr = Read_WhileNot_Zero16(input, count, frame_size);
        while(count < frame_size){
            std::wstring text = Read_WhileNot_Zero16(input, count, frame_size);
            uint32_t time = 0;
            for (int j = count; count < j + 4; count++){
                char temp_char;
                input.read(reinterpret_cast<char *>(&temp_char), sizeof(temp_char));
                time = (time << 8) + temp_char;
            }
            wdata.push_back(std::make_pair(text, time));
        }
    }
}


TEXT::TEXT(std::ifstream &input, const std::string &frame_name, const uint64_t frame_size, char flag1, char flag2):
FrameGeneral(frame_name, frame_size, flag1, flag2){
    uint64_t position = input.tellg();
    input.read(reinterpret_cast<char *>(&TEXT_encode), sizeof(TEXT_encode));
    if (TEXT_encode == 0 || TEXT_encode == 3) {
        std::string temp;
        while(input.tellg() < position + frame_size) {
            char temp_char;
            input.read(reinterpret_cast<char *>(&temp_char), sizeof(temp_char));
            if (temp_char == 0) {
                TEXT_text.push_back(temp);
                temp.clear();
                continue;
            }
            temp += temp_char;
        }
        if (temp.length() != 0){
            TEXT_text.push_back(temp);
        }
    } else {
        std::wstring temp;
        while(input.tellg() < position + frame_size) {
            wchar_t temp_char;
            input.read(reinterpret_cast<char *>(&temp_char), sizeof(temp_char));
            if (temp_char == 0) {
                TEXT_wtext.push_back(temp);
                temp.clear();
                continue;
            }
            temp += temp_char;
        }
        if (!temp.empty()){
            TEXT_wtext.push_back(temp);
        }
    }
}

TXXX::TXXX(std::ifstream& input, const std::string& frame_name, const uint64_t frame_size, char flag1, char flag2):
FrameGeneral(frame_name, frame_size, flag1, flag2){
    input.read(reinterpret_cast<char *>(&TXXX_encode), sizeof(TXXX_encode));
    uint64_t count = 1;
    if (TXXX_encode == 0 || TXXX_encode == 3) {
        TXXX_descr = Read_WhileNot_Zero8(input, count, frame_size);
        TXXX_value = Read_WhileNot_End8(input, count, frame_size);
    } else {
        TXXX_wdescr = Read_WhileNot_Zero16(input, count, frame_size);
        TXXX_wvalue = Read_WhileNot_End16(input, count, frame_size);
    }

}

COMM::COMM(std::ifstream &input, uint64_t& count, const std::string &frame_name, const uint64_t frame_size, char flag1, char flag2):
FrameGeneral(frame_name, frame_size, flag1, flag2), Language_Encode(input, count){
    std::cout << 1 << std::endl;
    if (encode == 0 || encode == 3){
        COMM_descr = Read_WhileNot_Zero8(input, count, frame_size);
        COMM_text = Read_WhileNot_End8(input, count, frame_size);
    } else {
        COMM_wdescr = Read_WhileNot_Zero16(input, count, frame_size);
        COMM_wtext = Read_WhileNot_End16(input, count, frame_size);
    }
}

ETCO::ETCO(std::ifstream& input, const std::string& frame_name, const uint64_t frame_size, char flag1, char flag2):
FrameGeneral(frame_name, frame_size, flag1, flag2){
    input.read(reinterpret_cast<char *>(&format), sizeof(format));
    uint64_t count = 1;
    for (; count < frame_size; count += 5){
        char type;
        input.read(reinterpret_cast<char *>(&type), sizeof(type));
        uint32_t stamp = 0;
        for (int i = 0; i < 4; i++) {
            char temp_char;
            input.read(reinterpret_cast<char *>(&temp_char), sizeof(temp_char));
            stamp = (stamp << 8) + temp_char;
        }
        data.push_back(std::make_pair(type, stamp));
    }
}

USLT::USLT(std::ifstream &input, uint64_t& count, const std::string &frame_name, const uint64_t frame_size, char flag1, char flag2):
COMM(input, count, frame_name, frame_size, flag1, flag2){}

URL::URL(std::ifstream& input, const std::string& frame_name, const uint64_t frame_size, char flag1, char flag2):
FrameGeneral(frame_name, frame_size, flag1, flag2){
    uint64_t count = 0;
    url = Read_WhileNot_End8(input, count, frame_size);
}

WXXX::WXXX(std::ifstream &input, const std::string &frame_name, const uint64_t frame_size, char flag1, char flag2):
FrameGeneral(frame_name, frame_size, flag1, flag2){
    input.read(reinterpret_cast<char *>(&WXXX_encode), sizeof(WXXX_encode));
    uint64_t i = 1;
    if (WXXX_encode == 0 || WXXX_encode == 3) {
        WXXX_descr = Read_WhileNot_Zero8(input, i, frame_size);
    } else {
        WXXX_wdescr = Read_WhileNot_Zero16(input, i, frame_size);
    }
    WXXX_url = Read_WhileNot_End8(input, i, frame_size);
}


SEEK::SEEK(std::ifstream &input, const std::string &frame_name, const uint64_t frame_size, char flag1, char flag2):
FrameGeneral(frame_name, frame_size, flag1, flag2){
    char temp_char[4];
    for (int i = 0; i < 4; i++) {
        input.read(reinterpret_cast<char *>(&temp_char[i]), sizeof(temp_char[i]));
    }
    SEEK_length = (temp_char[0] << 24) | (temp_char[1] << 16) | (temp_char[2] << 8) | temp_char[3];
}



GRID_ENCR_Shell::GRID_ENCR_Shell(std::ifstream &input, const std::string &frame_name, const uint64_t frame_size, char flag1, char flag2):
FrameGeneral(frame_name, frame_size, flag1, flag2){
    uint64_t count = 0;
    owner_id = Read_WhileNot_Zero8(input, count, frame_size);
    input.read(reinterpret_cast<char *>(&symbol), sizeof(symbol));
    count++;
    binary_data = Read_WhileNot_End8(input, count, frame_size);
}

GRID::GRID(std::ifstream &input, const std::string &frame_name, const uint64_t frame_size, char flag1, char flag2):
GRID_ENCR_Shell(input, frame_name, frame_size, flag1, flag2){}

ENCR::ENCR(std::ifstream &input, const std::string &frame_name, const uint64_t frame_size, char flag1, char flag2):
GRID_ENCR_Shell(input, frame_name, frame_size, flag1, flag2){}

PRIV::PRIV(std::ifstream &input, const std::string &frame_name, const uint64_t frame_size, char flag1, char flag2):
FrameGeneral(frame_name, frame_size, flag1, flag2){
    uint64_t count = 0;
    owner_id = Read_WhileNot_Zero8(input, count, frame_size);
    binary_data = Read_WhileNot_End8(input, count, frame_size);
}

POSS::POSS(std::ifstream &input, const std::string &frame_name, const uint64_t frame_size, char flag1, char flag2):
FrameGeneral(frame_name, frame_size, flag1, flag2){
    input.read(reinterpret_cast<char *>(&POSS_format), sizeof(POSS_format));
    uint64_t count = 1;
    std::string temp = Read_WhileNot_End8(input, count, frame_size);
    POSS_position = StringToInt(temp);
}

RVA2::RVA2(std::ifstream& input, const std::string& frame_name, const uint64_t frame_size, char flag1, char flag2):
FrameGeneral(frame_name, frame_size, flag1, flag2){
    uint64_t count = 0;
    RVA2_id = Read_WhileNot_Zero8(input, count, frame_size);
    input.read(reinterpret_cast<char *>(&channel), sizeof(channel));
    char temp_char;
    input.read(reinterpret_cast<char *>(&temp_char), sizeof(temp_char));
    volume_adj = temp_char;
    input.read(reinterpret_cast<char *>(&temp_char), sizeof(temp_char));
    volume_adj = (volume_adj << 8) + temp_char;

    input.read(reinterpret_cast<char *>(&bits), sizeof(bits));
    count += 4;
    if (bits != 0){
        for (; count < frame_size; count++) {
            input.read(reinterpret_cast<char *>(&temp_char), sizeof(temp_char));
            peak_volume = (peak_volume << 8) + temp_char;
        }
    }
}

LINK::LINK(std::ifstream& input, const std::string& frame_name, const uint64_t frame_size, char flag1, char flag2):
FrameGeneral(frame_name, frame_size, flag1, flag2){
    uint64_t count = 0;
    for (; count < 4; count++) {
        char temp_char;
        input.read(reinterpret_cast<char *>(&temp_char), sizeof(temp_char));
        LINK_frame_id += temp_char;
    }
    LINK_url = Read_WhileNot_Zero8(input, count, frame_size);
    LINK_id_data = Read_WhileNot_End8(input, count, frame_size);
}

POPM_Shell::POPM_Shell(std::ifstream& input, uint64_t& count, const uint64_t frame_size){
    POPM_email = Read_WhileNot_Zero8(input, count, frame_size);
    input.read(reinterpret_cast<char *>(&POPM_rating), sizeof(POPM_rating));
    count++;
}

POPM::POPM(std::ifstream& input, uint64_t& count, const std::string& frame_name, const uint64_t frame_size, char flag1, char flag2):
POPM_Shell(input, count, frame_size), PCNT(input, count, frame_name, frame_size, flag1, flag2){}

PCNT::PCNT(std::ifstream& input, uint64_t& count, const std::string& frame_name, const uint64_t frame_size, char flag1, char flag2):
FrameGeneral(frame_name, frame_size, flag1, flag2){
    std::string temp = Read_WhileNot_End8(input, count, frame_size);
    PCNT_count = StringToInt(temp);
}

OWNE::OWNE(std::ifstream& input, uint64_t& count, const std::string& frame_name, const uint64_t frame_size, char flag1, char flag2):
FrameGeneral(frame_name, frame_size, flag1, flag2){
    input.read(reinterpret_cast<char *>(&OWNE_encode), sizeof(OWNE_encode));
    count++;
    OWNE_price = Read_WhileNot_Zero8(input, count, frame_size);
    for (int i = count; count < i + 8; count++){
        char temp_char;
        input.read(reinterpret_cast<char *>(&temp_char), sizeof(temp_char));
        OWNE_date += temp_char;
    }
    if(frame_name == "OWNE") {
        if (OWNE_encode == 0 || OWNE_encode == 3) {
            OWNE_seller = Read_WhileNot_End8(input, count, frame_size);
        } else {
            OWNE_wseller = Read_WhileNot_End16(input, count, frame_size);
        }
    }
}

COMR::COMR(std::ifstream& input, uint64_t& count, const std::string& frame_name, const uint64_t frame_size, char flag1, char flag2):
OWNE(input, count, frame_name, frame_size, flag1, flag2){
    COMR_url = Read_WhileNot_Zero8(input, count, frame_size);
    char temp_char;
    input.read(reinterpret_cast<char *>(&temp_char), sizeof(temp_char));
    if (temp_char == 0) {
        COMR_received_as = "Other";
    } else if (temp_char == 1) {
        COMR_received_as = "Standard CD album with other songs";
    } else if (temp_char == 2) {
        COMR_received_as = "Compressed audio on CD";
    } else if (temp_char == 3) {
        COMR_received_as = "File over the Internet";
    } else if (temp_char == 4) {
        COMR_received_as = "Stream over the Internet";
    } else if (temp_char == 5) {
        COMR_received_as = "As note sheets";
    } else if (temp_char == 6) {
        COMR_received_as = "As note sheets in a book with other sheets";
    } else if (temp_char == 7) {
        COMR_received_as = "Music on other media";
    } else if (temp_char == 8) {
        COMR_received_as = "Non-musical merchandise";
    }
    if (OWNE_encode == 0 || OWNE_encode == 3) {
        COMR_name_seller = Read_WhileNot_Zero8(input, count, frame_size);
        COMR_descr = Read_WhileNot_Zero8(input, count, frame_size);
    } else {
        COMR_wname_seller = Read_WhileNot_Zero16(input, count, frame_size);
        COMR_wdescr = Read_WhileNot_Zero16(input, count, frame_size);
    }
    MIME_type = Read_WhileNot_Zero8(input, count, frame_size);
    logo = Read_WhileNot_End8(input, count, frame_size);
}

RBUF::RBUF(std::ifstream& input, const std::string& frame_name, const uint64_t frame_size, char flag1, char flag2):
FrameGeneral(frame_name, frame_size, flag1, flag2){
    char buffer[3];
    for (int i = 0; i < 3; i++){
        input.read(reinterpret_cast<char *>(&buffer[i]), sizeof(buffer[i]));
    }
    buffer_size = (buffer[0] << 16) | (buffer[1] << 8) | buffer[2];
    char temp_char;
    input.read(reinterpret_cast<char *>(&temp_char), sizeof(temp_char));
    flag = temp_char & 1;
    for (int i = 0; i < 4; i++){
        input.read(reinterpret_cast<char *>(&temp_char), sizeof(temp_char));
        offset = (offset << 8) + temp_char;
    }
}

EQU2::EQU2(std::ifstream& input, const std::string& frame_name, const uint64_t frame_size, char flag1, char flag2):
FrameGeneral(frame_name, frame_size, flag1, flag2){
    input.read(reinterpret_cast<char *>(&interpolation_method), sizeof(interpolation_method));
    uint64_t count = 1;
    EQU2_id = Read_WhileNot_Zero8(input, count, frame_size);
    for (; count < frame_size; count += 4){
        char temp_char1, temp_char2;
        input.read(reinterpret_cast<char *>(&temp_char1), sizeof(temp_char1));
        input.read(reinterpret_cast<char *>(&temp_char2), sizeof(temp_char2));
        uint16_t temp1 = (temp_char1 << 8) + temp_char2;
        input.read(reinterpret_cast<char *>(&temp_char1), sizeof(temp_char1));
        input.read(reinterpret_cast<char *>(&temp_char2), sizeof(temp_char2));
        uint16_t temp2 = (temp_char1 << 8) + temp_char2;
        freq_vol.push_back(std::make_pair(temp1, temp2));
    }
}

void Frame_Read(std::ifstream& input, ID3Header& head, std::vector<std::string>& show){
    std::string frame_name;
    for (int i = 0; i < frame_name_size; i++) {
        char temp_char;
        input.read(reinterpret_cast<char *>(&temp_char), sizeof(temp_char));
        frame_name += temp_char;
    }
    uint64_t frame_size = SizeFromBytes(input);
    char flag1, flag2;
    input.read(reinterpret_cast<char *>(&flag1), sizeof(flag1));
    input.read(reinterpret_cast<char *>(&flag2), sizeof(flag2));

    if (frame_name == "UFID"){
        UFID frame(input, frame_name, frame_size, flag1, flag2);
        if (show.empty() || find(show.begin(), show.end(), frame_name) != show.end()) {
            frame.Show();
        }
    } else if (frame_name == "TXXX") {
        TXXX frame(input, frame_name, frame_size, flag1, flag2);
        if (show.empty() || find(show.begin(), show.end(), frame_name) != show.end()) {
            frame.Show();
        }
    } else if (frame_name[0] == 'T'){
        TEXT frame(input, frame_name, frame_size, flag1, flag2);
        if (show.empty() || find(show.begin(), show.end(), frame_name) != show.end()) {
            frame.Show();
        }
    } else if (frame_name == "WXXX") {
        WXXX frame(input, frame_name, frame_size, flag1, flag2);
        if (show.empty() || find(show.begin(), show.end(), frame_name) != show.end()) {
            frame.Show();
        }
    } else if (frame_name[0] == 'W'){
        URL frame(input, frame_name, frame_size, flag1, flag2);
        if (show.empty() || find(show.begin(), show.end(), frame_name) != show.end()) {
            frame.Show();
        }
    } else if (frame_name == "COMM") {
        uint64_t count = 0;
        COMM frame(input, count, frame_name, frame_size, flag1, flag2);
        if (show.empty() || find(show.begin(), show.end(), frame_name) != show.end()) {
            frame.Show();
        }
    } else if (frame_name == "USLT") {
        uint64_t count = 0;
        USLT frame(input, count, frame_name, frame_size, flag1, flag2);
        if (show.empty() || find(show.begin(), show.end(), frame_name) != show.end()) {
            frame.Show();
        }
    } else if (frame_name == "GRID") {
        GRID frame(input, frame_name, frame_size, flag1, flag2);
        if (show.empty() || find(show.begin(), show.end(), frame_name) != show.end()) {
            frame.Show();
        }
    } else if (frame_name == "ENCR") {
        ENCR frame(input, frame_name, frame_size, flag1, flag2);
        if (show.empty() || find(show.begin(), show.end(), frame_name) != show.end()) {
            frame.Show();
        }
    } else if (frame_name == "PRIV") {
        PRIV frame(input, frame_name, frame_size, flag1, flag2);
        if (show.empty() || find(show.begin(), show.end(), frame_name) != show.end()) {
            frame.Show();
        }
    } else if (frame_name == "OWNE") {
        uint64_t count = 0;
        OWNE frame(input, count, frame_name, frame_size, flag1, flag2);
        if (show.empty() || find(show.begin(), show.end(), frame_name) != show.end()) {
            frame.Show();
        }
    } else if (frame_name == "POSS") {
        POSS frame(input, frame_name, frame_size, flag1, flag2);
        if (show.empty() || find(show.begin(), show.end(), frame_name) != show.end()) {
            frame.Show();
        }
    } else if (frame_name == "ETCO") {
        ETCO frame(input, frame_name, frame_size, flag1, flag2);
        if (show.empty() || find(show.begin(), show.end(), frame_name) != show.end()) {
            frame.Show();
        }
    } else if (frame_name == "EQU2") {
        EQU2 frame(input, frame_name, frame_size, flag1, flag2);
        if (show.empty() || find(show.begin(), show.end(), frame_name) != show.end()) {
            frame.Show();
        }
    } else if (frame_name == "RVA2") {
        RVA2 frame(input, frame_name, frame_size, flag1, flag2);
        if (show.empty() || find(show.begin(), show.end(), frame_name) != show.end()) {
            frame.Show();
        }
    } else if (frame_name == "SYLT") {
        uint64_t count = 0;
        SYLT frame(input, count, frame_name, frame_size, flag1, flag2);
        if (show.empty() || find(show.begin(), show.end(), frame_name) != show.end()) {
            frame.Show();
        }
    } else if (frame_name == "LINK") {
        LINK frame(input, frame_name, frame_size, flag1, flag2);
        if (show.empty() || find(show.begin(), show.end(), frame_name) != show.end()) {
            frame.Show();
        }
    } else if (frame_name == "POPM") {
        uint64_t count = 0;
        POPM frame(input, count, frame_name, frame_size, flag1, flag2);
        if (show.empty() || find(show.begin(), show.end(), frame_name) != show.end()) {
            frame.Show();
        }
    } else if (frame_name == "PCNT") {
        uint64_t count = 0;
        PCNT frame(input, count, frame_name, frame_size, flag1, flag2);
        if (show.empty() || find(show.begin(), show.end(), frame_name) != show.end()) {
            frame.Show();
        }
    } else if (frame_name == "COMR") {
        uint64_t count = 0;
        COMR frame(input, count, frame_name, frame_size, flag1, flag2);
        if (show.empty() || find(show.begin(), show.end(), frame_name) != show.end()) {
            frame.Show();
        }
    } else if (frame_name == "RBUF") {
        RBUF frame(input, frame_name, frame_size, flag1, flag2);
        if (show.empty() || find(show.begin(), show.end(), frame_name) != show.end()) {
            frame.Show();
        }
    } else if (frame_name == "SEEK") {
        SEEK frame(input, frame_name, frame_size, flag1, flag2);
        if (show.empty() || find(show.begin(), show.end(), frame_name) != show.end()) {
            frame.Show();
        }
        head.seek_ = frame.Get_SEEK();
    } else {
        input.seekg(frame_size, std::ios_base::cur);
    }
}

void COMR::Show(){
    std::cout << "Frame type: " << frame_name_ << std::endl;
    std::cout << "Price: " << OWNE_price << std::endl;
    std::cout << "Date: ";
    for (int i = 0; i < OWNE_date.size(); i++){
        std::cout << OWNE_date[i];
        if (i == 3 || i == 5 || i == 7) {
            std::cout << '/';
        }
    }
    std::cout << std::endl << "Contact URL: " << COMR_url << std::endl;
    std::cout << "Received as: " << COMR_received_as << std::endl;
    if (OWNE_encode == 0 || OWNE_encode == 3) {
        std::cout << "Name of seller: " << *COMR_name_seller << std::endl;
        std::cout << "Description: " << *COMR_descr << std::endl;
    } else {
        std::wcout << "Name of seller: " << *COMR_wname_seller << std::endl;
        std::wcout << "Description: " << *COMR_wdescr << std::endl;
    }
    std::cout << "MIME type: " << MIME_type << std::endl;
    std::cout << "Seller logo: " << logo << std::endl;
}

void OWNE::Show(){
    std::cout << "Frame type: " << frame_name_ << std::endl;
    std::cout << "Frame encode: " << What_Kind_encode(OWNE_encode) << std::endl;
    std::cout << "Price: " << OWNE_price << std::endl;
    std::cout << "Date: ";
    for (int i = 0; i < OWNE_date.size(); i++){
        std::cout << OWNE_date[i];
        if (i == 3 || i == 5 || i == 7) {
            std::cout << '/';
        }
    }
    std::cout << std::endl << "Seller: ";
    if (OWNE_seller != std::nullopt) {
        std::cout << *OWNE_seller << std::endl;
    } else {
        std::wcout << *OWNE_wseller << std::endl;
    }
}


void PRIV::Show() {
    std::cout << "Frame type: " << frame_name_ << std::endl;
    std::cout << "Owner ID: " << owner_id << std::endl;
    std::cout << "Binary Data: ";
    for (int i = 0; i < binary_data.size(); i++){
        for (int j = 0; j < 8; j++) {
            bool temp = (binary_data[i] >> i) & 1 ? 1 : 0;
            std::cout << temp;
        }
    }
    std::cout << std::endl;
}

void RVA2::Show() {
    std::cout << "Frame type: " << frame_name_ << std::endl;
    std::cout << "Identification: " << RVA2_id << std::endl;
    std::cout << "Type of channel: " << static_cast<int>(channel) << std::endl;
    std::cout << "Volume Adjustment: " << volume_adj << std::endl;
    std::cout << "Bits representing peak: " << bits << std::endl;
    std::cout << "Peak volume: " << peak_volume << std::endl;
}

void ENCR::Show() {
    std::cout << "Frame type: " << frame_name_ << std::endl;
    std::cout << "Owner ID: " << owner_id << std::endl;
    std::cout << "Method symbol: " << symbol << std::endl;
    std::cout << "Encryption Data: ";
    for (int i = 0; i < binary_data.size(); i++) {
        for (int j = 0; j < 8; j++) {
            bool temp = (binary_data[i] >> i) & 1 ? 1 : 0;
            std::cout << temp;
        }
    }
    std::cout << std::endl;
}

void PCNT::Show(){
    std::cout << "Frame type: " << frame_name_ << std::endl;
    std::cout << "Counter: " << PCNT_count << std::endl;
}

void UFID::Show(){
    std::cout << "Frame type: "<< frame_name_ << std::endl;
    std::cout << "Owner ID: " << UFID_owner_id << std::endl;
    std::cout << "ID: " << UFID_id << std::endl;
}

void USER::Show(){
    std::cout << "Frame type: "<< frame_name_ << std::endl;
    std::cout << "Frame encode: " << What_Kind_encode(encode) << std::endl;
    std::cout << "Language: " << language << std::endl;
    std::cout << "Text: ";
    if (USER_wtext != std::nullopt){
        std::wcout << *USER_wtext << std::endl;
    } else {
        std::cout << *USER_text << std::endl;
    }
}

void TEXT::Show(){
    std::cout << "Frame type: "<< frame_name_ << std::endl;
    std::cout << "Frame encode: " << What_Kind_encode(TEXT_encode) << std::endl;
    std::cout << "Text: ";
    if (!TEXT_wtext.empty()){
        for (int i = 0; i < TEXT_wtext.size(); i++){
            std::wcout << TEXT_wtext.at(i) << " ";
        }
    } else {
        for (int i = 0; i < TEXT_text.size(); i++){
            std::cout << TEXT_text.at(i) << " ";
        }
    }
    std::cout << std::endl;
}

void WXXX::Show() const {
    std::cout << "Frame type: " << frame_name_ << std::endl;
    if(WXXX_descr != std::nullopt) {
        std::cout << "Description: " << *WXXX_descr << std::endl;
    } else {
        std::cout << "Description: ";
        std::wcout << *WXXX_wdescr << std::endl;
    }
    std::cout << "URL: " << WXXX_url << std::endl;
}

void TXXX::Show(){
    std::cout << "Frame type: " << frame_name_ << std::endl;
    std::cout << "Frame encode: " << What_Kind_encode(TXXX_encode) << std::endl;
    if(TXXX_encode == 0 || TXXX_encode == 3) {
        std::cout << "Description: " << *TXXX_descr << std::endl;
        std::cout << "Value: " << *TXXX_value << std::endl;
    } else {
        std::cout << "Description: ";
        std::wcout << *TXXX_wdescr << std::endl;
        std::cout << "Value: ";
        std::wcout << *TXXX_wvalue << std::endl;
    }
}

void ETCO::Show(){
    std::cout << "Frame type: " << frame_name_ << std::endl;
    if (format == 0) {
        std::cout << "Absolute time, 32 bit sized, using MPEG [MPEG] frames as unit" << std::endl;
    } else {
        std::cout << "Absolute time, 32 bit sized, using milliseconds as unit" << std::endl;
    }
    std::cout << "Events: " << std::endl;
    for (int i = 0; i < data.size(); i++) {
        std::cout << "Type: " << static_cast<int>(data[i].first) << "  Time stamp:" << data[i].second << std::endl;
    }
}

void COMM::Show(){
    std::cout << "Frame type: " << frame_name_ << std::endl;
    std::cout << "Frame encode: " << What_Kind_encode(encode) << std::endl;
    std::cout << "Language: " << language << std::endl;
    if(encode == 0 || encode == 3) {
        if(COMM_descr != std::nullopt) {
            std::cout << "Description: " << *COMM_descr << std::endl;
        }
        std::cout << "Text: " << *COMM_text << std::endl;
    } else {
        std::cout << "Description: ";
        std::wcout << *COMM_wdescr << std::endl;
        std::cout << "Text: ";
        std::wcout << *COMM_wtext << std::endl;
    }
}

void POPM::Show(){
    std::cout << "Frame type: " << frame_name_ << std::endl;
    std::cout << "Email to user: " << POPM_email << std::endl;
    std::cout << "Rating: " << What_Kind_encode(POPM_rating) << std::endl;
    std::cout << "Counter: " << PCNT_count << std::endl;
}

void LINK::Show(){
    std::cout << "Frame type: " << frame_name_ << std::endl;
    std::cout << "Frame ID: " << LINK_frame_id << std::endl;
    std::cout << "URL: " << LINK_url << std::endl;
    std::cout << "ID and additional data: " << LINK_id_data << std::endl;
}

void POSS::Show(){
    std::cout << "Frame type: " << frame_name_ << std::endl;
    std::cout << "Position: ";
    if (POSS_format == 1) {
        std::cout << POSS_position << " MPEG frames" << std::endl;
    } else {
        std::cout << POSS_position << " milliseconds" << std::endl;
    }
}

void URL::Show() const {
    std::cout << "Frame type: " << frame_name_ << std::endl;
    std::cout << "URL: " << url << std::endl;
}

void SYLT::Show() {
    std::cout << "Frame type: " << frame_name_ << std::endl;
    std::cout << "Frame encode: " << What_Kind_encode(encode) << std::endl;
    std::cout << "Language: " << language << std::endl;
    std::cout << "Time stamp format: " << format << std::endl;
    std::cout << "Content type: " << content << std::endl;
    if(encode == 0 || encode == 3) {
        std::cout << "Content descriptor: " << *SYLT_descr << std::endl;
        std::cout << "Text: " << std::endl;
        for (int i = 0; i < data.size(); i++) {
            std::cout << data[i].first << " " << data[i].second << std::endl;
        }
    } else {
        std::wcout << "Content descriptor: " << *SYLT_wdescr << std::endl;
        std::cout << "Text: " << std::endl;
        for (int i = 0; i < data.size(); i++) {
            std::wcout << wdata[i].first << " " << data[i].second << std::endl;
        }
    }
}

void USLT::Show(){
    std::cout << "Frame type: " << frame_name_ << std::endl;
    std::cout << "Frame encode: " << What_Kind_encode(encode) << std::endl;
    std::cout << "Language: " << language << std::endl;
    if(encode == 0 || encode == 3) {
        if(COMM_descr != std::nullopt) {
            std::cout << "Description: " << *COMM_descr << std::endl;
        }
        std::cout << "Lyrics: " << *COMM_text << std::endl;
    } else {
        std::cout << "Description: ";
        std::wcout << *COMM_wdescr << std::endl;
        std::cout << "Lyrics: ";
        std::wcout << *COMM_wtext << std::endl;
    }
}

void GRID::Show(){
    std::cout << "Frame type: " << frame_name_ << std::endl;
    std::cout << "Owner ID: " << owner_id << std::endl;
    std::cout << "Group symbol: " << symbol << std::endl;
    std::cout << "Binary Data: ";
    for (int i = 0; i < binary_data.size(); i++){
        for (int j = 0; j < 8; j++) {
            bool temp = (binary_data[i] >> i) & 1 ? 1 : 0;
            std::cout << temp;
        }
    }
    std::cout << std::endl;
}

void RBUF::Show(){
    std::cout << "Frame type: " << frame_name_ << std::endl;
    std::cout << "Embedded into flag: " << flag << std::endl;
    std::cout << "Offset: " << offset << std::endl;
}

void EQU2::Show(){
    std::cout << "Frame type: " << frame_name_ << std::endl;
    std::cout << "Interpolation method: ";
    if (interpolation_method == 1){
        std::cout << "Linear" << std::endl;
    } else {
        std::cout << "Band" << std::endl;
    }
    std::cout << "Identification: " << EQU2_id << std::endl;
    std::cout << "Frequency / Volume Adjustment: " << std::endl;
    for (int i = 0; i < freq_vol.size(); i++) {
        std::cout << freq_vol[i].first << " " << freq_vol[i].second << std::endl;
    }
}

void SEEK::Show(){
    std::cout << "Frame type: " << frame_name_ << std::endl;
    std::cout << "SEEK offset: " << SEEK_length << std::endl;
}
