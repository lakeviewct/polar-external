#pragma once

namespace logging
{
    bool init();
    void shutdown();
    void info(const char* fmt, ...);
    void ok(const char* fmt, ...);
    void fail(const char* fmt, ...);
}