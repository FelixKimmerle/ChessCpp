#pragma once

#pragma once

#include <string>

namespace ConsoleColor
{
    namespace Backgound
    {
        static const std::string Default = "\e[49m";
        static const std::string Black = "\e[48;5;0m";
        static const std::string Maroon = "\e[48;5;1m";
        static const std::string Green = "\e[48;5;2m";
        static const std::string Olive = "\e[48;5;3m";
        static const std::string Navi = "\e[48;5;4m";
        static const std::string Purple = "\e[48;5;5m";
        static const std::string Teal = "\e[48;5;6m";
        static const std::string Silver = "\e[48;5;7m";
        static const std::string Grey = "\e[48;5;8m";
        static const std::string Red = "\e[48;5;9m";
        static const std::string Lime = "\e[48;5;10m";
        static const std::string Yellow = "\e[48;5;11m";
        static const std::string Blue = "\e[48;5;12m";
        static const std::string Fuchsia = "\e[48;5;13m";
        static const std::string Aqua = "\e[48;5;14m";
        static const std::string White = "\e[48;5;15m";
    } // namespace Backgound

    namespace Text
    {
        static const std::string Default = "\e[39m";
        static const std::string Black = "\e[38;5;0m";
        static const std::string Maroon = "\e[38;5;1m";
        static const std::string Green = "\e[38;5;2m";
        static const std::string Olive = "\e[38;5;3m";
        static const std::string Navi = "\e[38;5;4m";
        static const std::string Purple = "\e[38;5;5m";
        static const std::string Teal = "\e[38;5;6m";
        static const std::string Silver = "\e[38;5;7m";
        static const std::string Grey = "\e[38;5;8m";
        static const std::string Red = "\e[38;5;9m";
        static const std::string Lime = "\e[38;5;10m";
        static const std::string Yellow = "\e[38;5;11m";
        static const std::string Blue = "\e[38;5;12m";
        static const std::string Fuchsia = "\e[38;5;13m";
        static const std::string Aqua = "\e[38;5;14m";
        static const std::string White = "\e[38;5;15m";

    } // namespace Text

    namespace Formatting
    {
        static const std::string Bold = "\e[1m";
        static const std::string Dim = "\e[2m";
        static const std::string Italic = "\e[3m";
        static const std::string Underlined = "\e[4m";
        static const std::string Blink = "\e[5m";
        static const std::string Reverse = "\e[7m";
        static const std::string Hidden = "\e[8m";

        static const std::string ResetBold = "\e[21m";
        static const std::string ResetDim = "\e[22m";
        static const std::string ResetItalic = "\e[23m";
        static const std::string ResetUnderlined = "\e[24m";
        static const std::string ResetBlink = "\e[25m";
        static const std::string ResetReverse = "\e[27m";
        static const std::string ResetHidden = "\e[28m";
    } // namespace Formatting
    static const std::string Reset = "\e[0m";
} // namespace Color