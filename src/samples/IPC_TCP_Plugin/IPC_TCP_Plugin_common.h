#ifndef IPC_TCP_PLUGIN_COMMON_H
#define IPC_TCP_PLUGIN_COMMON_H

#include <string>

#define BUILD_DLL

extern void(*g_Log)(std::string const &, int);

#endif