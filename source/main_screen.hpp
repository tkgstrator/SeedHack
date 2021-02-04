#pragma once
#include "Aether/Aether.hpp"
#include "consts.hpp"

class MainScreen : public Aether::Screen
{
private:
    MainScreen();
    MainScreen(const MainScreen &) = delete;
    ~MainScreen();

    static constexpr auto SUB_TITLE_COLOR = Aether::Colour{0xFF, 0x71, 0xE7, 0xFF};

    // dementions
    static constexpr auto TITLE_X = 50;
    static constexpr auto TITLE_Y = 50;
    static constexpr auto TITLE_SIZE = 72;

    static constexpr auto SUB_TITLE_RAISE = 3;
    static constexpr auto SUB_TITLE_SIZE = 31;

    static constexpr auto LIST_X = 58;
    static constexpr auto LIST_Y = 160;
    static constexpr auto LIST_W = 720;
    static constexpr auto LIST_H = consts::SCREEN_H - LIST_Y - consts::CONTROL_BAR_H;

public:
    static inline auto &getInstance()
    {
        static MainScreen s_instance;
        return s_instance;
    }
};
