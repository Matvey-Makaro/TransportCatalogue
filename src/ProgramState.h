#pragma once

#include <string_view>
#include <string>

class ProgramState
{
    public:
        ProgramState() = delete;

        static void RegisterCurrFunc(std::string_view funcName);
        static const std::string& GetCurrFunc();

    private:
        static std::string _currFunc;
};

#define REGISTER_CURR_FUNC() ProgramState::RegisterCurrFunc(__PRETTY_FUNCTION__)