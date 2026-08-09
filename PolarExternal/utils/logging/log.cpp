#include "log.h"

#include <Windows.h>
#include <cstdarg>
#include <cstdio>
#include <cstring>

#include <fstream>
#include <filesystem>
#include <ShlObj.h>

// we use kernel functions: why? because why the fuck not

namespace logging
{
    namespace
    {
        HANDLE console_handle = INVALID_HANDLE_VALUE;
        bool initialized = false;

        void set_color(WORD c)
        {
            if (console_handle != INVALID_HANDLE_VALUE)
            {
                SetConsoleTextAttribute(console_handle, c);
            }
        }

        void write_raw(const char* text, std::size_t length)
        {
            if (!text || !length)
                return;

            if (console_handle != INVALID_HANDLE_VALUE)
            {
                DWORD written = 0;
                WriteConsoleA(console_handle, text, static_cast<DWORD>(length), &written, nullptr);
            }
        }

        void emit(WORD level_attr, const char* level_str, const char* fmt, va_list args)
        {
            if (!initialized)
                return;

            char msg[512] = {};
            vsnprintf(msg, sizeof(msg), fmt, args);

            set_color(level_attr);
            write_raw(level_str, std::strlen(level_str));
            write_raw(" ", 1);

            set_color(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
            write_raw(msg, std::strlen(msg));
            write_raw("\r\n", 2);

            set_color(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
        }
    }

    bool init()
    {
        if (initialized)
            return true;

        if (!AllocConsole())
        {
            if (!AttachConsole(ATTACH_PARENT_PROCESS) && !AttachConsole(GetCurrentProcessId()))
            {
                console_handle = GetStdHandle(STD_OUTPUT_HANDLE);
                initialized = true;
                return true;
            }
        }

        SetConsoleTitleA("Polar v1");

        FILE* fp = nullptr;
        freopen_s(&fp, "CONOUT$", "w", stdout);
        freopen_s(&fp, "CONOUT$", "w", stderr);
        freopen_s(&fp, "CONIN$", "r", stdin);

        console_handle = GetStdHandle(STD_OUTPUT_HANDLE);

        initialized = true;
        return true;
    }

    void shutdown()
    {
        initialized = false;

        HWND wnd = GetConsoleWindow();
        FreeConsole();

        if (wnd)
            PostMessageW(wnd, WM_CLOSE, 0, 0);
    }

    void info(const char* fmt, ...)
    {
        va_list args;
        va_start(args, fmt);
        emit(FOREGROUND_BLUE | FOREGROUND_INTENSITY, "[/]", fmt, args);
        va_end(args);
    }

    void ok(const char* fmt, ...)
    {
        va_list args;
        va_start(args, fmt);
        emit(FOREGROUND_GREEN | FOREGROUND_INTENSITY, "[+]", fmt, args);
        va_end(args);
    }

    void fail(const char* fmt, ...)
    {
        va_list args;
        va_start(args, fmt);
        emit(FOREGROUND_RED | FOREGROUND_INTENSITY, "[-]", fmt, args);
        va_end(args);
    }
}
