﻿#pragma execution_character_set(push, "utf-8")
#include <filesystem>
#include <iostream>
#include <string>
#include <locale>
#include <SDL3/SDL_main.h>
#ifdef _WIN32
    #include <windows.h>
#else
    #include <fcntl.h>
    #include <unistd.h>
#endif

#include "debug/Logger.hpp"
#include "debug/Fatal.hpp"
#include "CherryGrove.hpp"

#define SDL_MAIN_HANDLED
int SDL_main(int argc, char** argv) {
    typedef int32_t i32;
    using std::filesystem::current_path, std::filesystem::canonical, std::filesystem::remove, std::filesystem::path, std::locale, std::string, std::cout, std::endl;

    char** _argv = argv;

    //Set working directory to parent directory of the executable file
    #ifdef _WIN32
        path exePath(_argv[0]);
        path exeDir = canonical(exePath).parent_path();
        current_path(exeDir);
    #endif

    cout << "Setting up logger..." << endl;

    //todo: Use marco & options to determine
    #ifndef CG_DEBUG_CONSOLE
        Logger::setToFile(true);
    #endif

    //todo: Switch locale
    locale::global(locale("zh_CN.UTF-8"));

    lout << "Hello from Logger!" << endl;
    lout << "Working directory: " << current_path() << endl;
    lout << "Trying to get unique instance lock..." << endl;

    string lockFilePath = current_path().string();
    lockFilePath += "\\running";
    #ifdef _WIN32
        HANDLE lockFile = CreateFileA(
            lockFilePath.c_str(),
            GENERIC_READ | GENERIC_WRITE,
            0,
            nullptr,
            OPEN_ALWAYS,
            FILE_ATTRIBUTE_NORMAL,
            nullptr);
        if (lockFile != INVALID_HANDLE_VALUE) {
            bool locked = LockFile(lockFile, 0, 0, 1, 0);
            if (!locked) goto failure;
        }
        else goto failure;
    #else
        i32 lockFile = open(lockFilePath.c_str(), O_CREAT | O_RDWR, 0666);
        if (lockFile >= 0) {
            bool locked = lockf(lockFile, F_TLOCK, 0) == 0;
            if (!locked) goto failure;
        }
        else goto failure;
    #endif // _WIN32

    lout << "Launching CherryGrove..." << endl;

    //CherryGrove, launch!
    CherryGrove::launch();

    #ifdef _WIN32
        UnlockFile(lockFile, 0, 0, 1, 0);
        CloseHandle(lockFile);
    #else
        lockf(lockFile, F_ULOCK, 0);
        close(lockFile);
    #endif // _WIN32

    remove(lockFilePath);

    return 0;

    failure:
    //todo: Focus on the existing instance windowHandle
    lerr << "[Instance Lock] Please don't launch CherryGrove multiple times from one executable. If you need to use multiple instances, copy the whole directory to a new place, or install again at a different location." << endl;
    return Fatal::MISC_MULTIPLE_INSTANCES;
}