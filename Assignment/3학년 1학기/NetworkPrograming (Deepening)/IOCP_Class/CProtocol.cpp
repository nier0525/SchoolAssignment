#include "CProtocol.h"
IMPLEMENT_SINGLETON(CProtocol)

void CProtocol::ProtocolMaker(unsigned __int64& Full_Code, unsigned __int64 input)
{
    Full_Code = Full_Code | input;
}

bool CProtocol::ProtocolUnpacker(unsigned __int64 _full_code, unsigned __int64 main, unsigned __int64 sub, unsigned __int64 protocol)
{
    unsigned __int64 temp = 0;
    int variable_count = 0;//입력받은 매개변수 개수
    if (main != NULL)
    {
        temp = _full_code & (unsigned __int64)BODY::MAIN;
        variable_count++;
    }
    if (sub != NULL)
    {
        temp = _full_code & (unsigned __int64)BODY::SUB;
        variable_count++;
    }
    if (protocol != NULL)
    {
        temp = _full_code & (unsigned __int64)BODY::PROTOCOL;
        variable_count++;
    }
    if (variable_count == 2)
    {
        printf("사용 매개 변수의 수를 초과 하셨습니다.\n");
        return false;
    }
    if (temp & main || temp & sub || temp & protocol)
    {
        printf("일치\n");
        return true;
    }
    else
    {
        printf("불일치\n");
        return false;
    }

}
