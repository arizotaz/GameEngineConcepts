#ifndef WEB_H
#define WEB_H 1

#include <string>

void OpenWebURL(const char* url);
void OpenWebURL(std::string str) { OpenWebURL(str.c_str()); }

#ifdef _WIN32
#include <shellapi.h>
#include <windows.h>
void OpenWebURL(const char* url)
{
    ShellExecute(NULL, "open", url, NULL, NULL, SW_SHOWNORMAL);
}
#endif

#ifdef __APPLE__
#include <cstdlib>
void OpenWebURL(const char* url)
{
    std::system((std::string("open ") + std::string(url)).c_str());
}
#endif

#ifdef __linux__
#include <cstdlib>
void OpenWebURL(const char* url)
{
    std::system((std::string("xdg-open  ") + std::string(url)).c_str());
}
#endif

#endif