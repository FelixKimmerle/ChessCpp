#pragma once

#include <string>

namespace Color
{
    namespace Compatibility
    {
        namespace Text
        {
            static const std::string Default = "\e[39m";
            static const std::string Black = "\e[30m";
            static const std::string Red = "\e[31m";
            static const std::string Green = "\e[32m";
            static const std::string Yellow = "\e[33m";
            static const std::string Blue = "\e[34m";
            static const std::string Magenta = "\e[35m";
            static const std::string Cyan = "\e[36m";

            static const std::string LightGray = "\e[37m";
            static const std::string DarkGray = "\e[90m";
            static const std::string LightRed = "\e[91m";
            static const std::string LightGreen = "\e[92m";
            static const std::string LightYellow = "\e[93m";
            static const std::string LightBlue = "\e[94m";
            static const std::string LightMagenta = "\e[95m";
            static const std::string LightCyan = "\e[96m";
            static const std::string White = "\e[97m";
        } // namespace Text
        namespace Background
        {
            static const std::string Default = "\e[49m";
            static const std::string Black = "\e[40m";
            static const std::string Red = "\e[41m";
            static const std::string Green = "\e[42m";
            static const std::string Yellow = "\e[43m";
            static const std::string Blue = "\e[44m";
            static const std::string Magenta = "\e[45m";
            static const std::string Cyan = "\e[46m";

            static const std::string LightGray = "\e[47m";
            static const std::string DarkGray = "\e[100m";
            static const std::string LightRed = "\e[101m";
            static const std::string LightGreen = "\e[102m";
            static const std::string LightYellow = "\e[103m";
            static const std::string LightBlue = "\e[104m";
            static const std::string LightMagenta = "\e[105m";
            static const std::string LightCyan = "\e[106m";
            static const std::string White = "\e[107m";
        } // namespace Background

    } // namespace Compatibility

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
        static const std::string GetColor(unsigned short index)
        {
            return "\e[48;5;" + std::to_string(index) + "m";
        }
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
        static const std::string GetColor(unsigned short index)
        {
            return "\e[38;5;" + std::to_string(index) + "m";
        }

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