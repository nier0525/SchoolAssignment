#pragma once
#include"Global.h"

class CProtocol
{
private:
    DECLARE_SINGLETONE(CProtocol);
    CProtocol() = default;
    ~CProtocol() = default;
    enum class BODY : unsigned __int64
    {
        MAIN = 0xff00000000000000,
        SUB = 0x00ff000000000000,
        PROTOCOL = 0x0000ff0000000000,
        DETAIL = 0x000000ffffffffff
    };

public:
    void ProtocolMaker(unsigned __int64& Full_Code, unsigned __int64 input);
    bool ProtocolUnpacker(unsigned __int64 _full_code, unsigned __int64 main, unsigned __int64 sub, unsigned __int64 protocol);
};


