#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <optional>
#include <vector>



const uint8_t frame_name_size = 4;
const uint8_t header_size = 10;

uint64_t SizeFromBytes(std::ifstream& input);

class FrameGeneral{
public:
    FrameGeneral(const std::string &frame_name, const uint64_t frame_size, char flag1, char flag2):
            frame_name_(frame_name),
            frame_size_(frame_size),
            flag1_(flag1),
            flag2_(flag2){}
protected:
    std::string frame_name_;
    uint64_t frame_size_;
    char flag1_;
    char flag2_;
};

class ExtendedHeader{
public:
    friend void Extended_Read(std::ifstream& input, ExtendedHeader& ext);
protected:
    uint64_t ext_size_;
    std::optional<uint64_t> crc_;
    std::optional<uint64_t> restrictions_;
};

class ID3Header: public ExtendedHeader{
public:
    bool Is_Valid() const;
    ID3Header(std::ifstream& input);
    uint64_t Get_header_size() const;
    std::optional<uint64_t> seek_;
private:
    bool identifier_;
    bool version_;
    bool unsynchronisation_ = false;
    bool extended_ = false;
    bool experimental_ = false;
    bool footer_ = false;
    uint64_t size_;
};


///////////////////////////////////////////////////////////////////////////////////////////

///Frames

class Language_Encode{
public:
    Language_Encode(std::ifstream& input, uint64_t& count);
protected:
    char encode;
    std::string language;
};


class UFID: public FrameGeneral{
public:
    UFID(std::ifstream& input, const std::string& frame_name, const uint64_t frame_size, char flag1, char flag2);
    void Show();
protected:
    std::string UFID_owner_id;
    std::string UFID_id;
};

class USER: public FrameGeneral, public Language_Encode{
public:
    USER(std::ifstream& input, uint64_t& count, const std::string& frame_name, const uint64_t frame_size, char flag1, char flag2);
    void Show();
protected:
    std::optional<std::string> USER_text;
    std::optional<std::wstring> USER_wtext;
};

class TEXT: public FrameGeneral{
public:
    TEXT(std::ifstream& input, const std::string& frame_name, const uint64_t frame_size, char flag1, char flag2);
    void Show();
protected:
    char TEXT_encode;
    std::vector<std::string> TEXT_text;
    std::vector<std::wstring> TEXT_wtext;
};

class URL: public FrameGeneral{
public:
    URL(std::ifstream& input, const std::string& frame_name, const uint64_t frame_size, char flag1, char flag2);

    void Show() const;
protected:
    std::string url;
};

class WXXX: public FrameGeneral{
public:
    WXXX(std::ifstream& input, const std::string& frame_name, const uint64_t frame_size, char flag1, char flag2);
    void Show() const;
protected:
    char WXXX_encode;
    std::optional<std::string> WXXX_descr;
    std::optional<std::wstring> WXXX_wdescr;
    std::string WXXX_url;
};

class TXXX: public FrameGeneral{
public:
    TXXX(std::ifstream& input, const std::string& frame_name, const uint64_t frame_size, char flag1, char flag2);
    void Show();
protected:
    char TXXX_encode;
    std::optional<std::string> TXXX_descr;
    std::optional<std::wstring> TXXX_wdescr;
    std::optional<std::string> TXXX_value;
    std::optional<std::wstring> TXXX_wvalue;
};

class COMM: public FrameGeneral, public Language_Encode{
public:
    COMM(std::ifstream& input, uint64_t& count, const std::string& frame_name, const uint64_t frame_size, char flag1, char flag2);

    virtual void Show();
protected:
    std::optional<std::string> COMM_descr;
    std::optional<std::wstring> COMM_wdescr;
    std::optional<std::string> COMM_text;
    std::optional<std::wstring> COMM_wtext;
};

class SEEK: public FrameGeneral{
public:
    SEEK(std::ifstream& input, const std::string& frame_name, const uint64_t frame_size, char flag1, char flag2);
    void Show();
    uint64_t Get_SEEK(){
        return SEEK_length;
    }
protected:
    uint32_t SEEK_length;
};

class USLT: public COMM{
public:
    USLT(std::ifstream& input, uint64_t& count, const std::string& frame_name, const uint64_t frame_size, char flag1, char flag2);
    void Show() override;
};

class GRID_ENCR_Shell: public FrameGeneral{
public:
    GRID_ENCR_Shell(std::ifstream& input, const std::string& frame_name, const uint64_t frame_size, char flag1, char flag2);
protected:
    char symbol;
    std::string binary_data;
    std::string owner_id;
};

class GRID: public GRID_ENCR_Shell{
public:
    GRID(std::ifstream& input, const std::string& frame_name, const uint64_t frame_size, char flag1, char flag2);
    void Show();
};

class ENCR: public GRID_ENCR_Shell{
public:
    ENCR(std::ifstream& input, const std::string& frame_name, const uint64_t frame_size, char flag1, char flag2);
    void Show();
};

class PRIV: public FrameGeneral{
public:
    PRIV(std::ifstream& input, const std::string& frame_name, const uint64_t frame_size, char flag1, char flag2);
    void Show();
protected:
    std::string owner_id;
    std::string binary_data;
};

class POSS: public FrameGeneral{
public:
    POSS(std::ifstream& input, const std::string& frame_name, const uint64_t frame_size, char flag1, char flag2);
    void Show();
protected:
    char POSS_format;
    uint64_t POSS_position;
};

class LINK: public FrameGeneral{
public:
    LINK(std::ifstream& input, const std::string& frame_name, const uint64_t frame_size, char flag1, char flag2);
    void Show();
protected:
    std::string LINK_frame_id;
    std::string LINK_url;
    std::string LINK_id_data;
};

class SYLT: public FrameGeneral, public Language_Encode{
public:
    SYLT(std::ifstream& input, uint64_t& count, const std::string& frame_name, const uint64_t frame_size, char flag1, char flag2);
    void Show();
protected:
    char format;
    char content;
    std::optional<std::string> SYLT_descr;
    std::optional<std::wstring> SYLT_wdescr;
    std::vector<std::pair<std::string, uint32_t>> data;
    std::vector<std::pair<std::wstring, uint32_t>> wdata;
};

class POPM_Shell{
public:
    POPM_Shell(std::ifstream& input, uint64_t& count, const uint64_t frame_size);
protected:
    std::string POPM_email;
    char POPM_rating;
};

class PCNT: public FrameGeneral{
public:
    PCNT(std::ifstream& input, uint64_t& count, const std::string& frame_name, const uint64_t frame_size, char flag1, char flag2);

    virtual void Show();
protected:
    uint64_t PCNT_count;
};

class POPM: public POPM_Shell, public PCNT{
public:
    POPM(std::ifstream& input, uint64_t& count, const std::string& frame_name, const uint64_t frame_size, char flag1, char flag2);
    void Show() override;
};

class OWNE: public FrameGeneral{
public:
    OWNE(std::ifstream& input, uint64_t& count, const std::string& frame_name, const uint64_t frame_size, char flag1, char flag2);
    virtual void Show();
protected:
    char OWNE_encode;
    std::string OWNE_price;
    std::string OWNE_date;
    std::optional<std::string> OWNE_seller;
    std::optional<std::wstring> OWNE_wseller;
};

class COMR: public OWNE{
public:
    COMR(std::ifstream& input, uint64_t& count, const std::string& frame_name, const uint64_t frame_size, char flag1, char flag2);
    void Show() override;
protected:
    std::string COMR_url;
    std::string COMR_received_as;
    std::optional<std::string> COMR_name_seller;
    std::optional<std::wstring> COMR_wname_seller;
    std::optional<std::string> COMR_descr;
    std::optional<std::wstring> COMR_wdescr;
    std::string MIME_type;
    std::string logo;
};

class RBUF: public FrameGeneral{
public:
    RBUF(std::ifstream& input, const std::string& frame_name, const uint64_t frame_size, char flag1, char flag2);
    void Show();
protected:
    uint32_t buffer_size;
    bool flag;
    uint64_t offset;
};

class RVA2: public FrameGeneral{
public:
    RVA2(std::ifstream& input, const std::string& frame_name, const uint64_t frame_size, char flag1, char flag2);
    void Show();
protected:
    std::string RVA2_id;
    char channel;
    uint16_t volume_adj;
    uint8_t bits;
    uint64_t peak_volume;
};

class EQU2: public FrameGeneral{
public:
    EQU2(std::ifstream& input, const std::string& frame_name, const uint64_t frame_size, char flag1, char flag2);
    void Show();
protected:
    char interpolation_method;
    std::string EQU2_id;
    std::vector<std::pair<uint16_t, uint16_t>> freq_vol;
};

class ETCO: public FrameGeneral{
public:
    ETCO(std::ifstream& input, const std::string& frame_name, const uint64_t frame_size, char flag1, char flag2);
    void Show();
protected:
    char format;
    std::vector<std::pair<char, uint32_t>> data;
};


void Frame_Read(std::ifstream& input, ID3Header& head, std::vector<std::string>& show);
