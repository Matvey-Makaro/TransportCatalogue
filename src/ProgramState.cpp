#include "ProgramState.h"

std::string ProgramState::_currFunc = "";

void ProgramState::RegisterCurrFunc(std::string_view funcName)
{
    _currFunc = std::string(funcName);
}

const std::string& ProgramState::GetCurrFunc()
{
    return _currFunc;
}
