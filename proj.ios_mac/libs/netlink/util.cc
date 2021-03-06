/*
    NetLink Sockets: Networking C++ library
    Copyright 2012 Pedro Francisco Pareja Ruiz (PedroPareja@Gmail.com)

    This file is part of NetLink Sockets.

    NetLink Sockets is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    NetLink Sockets is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with NetLink Sockets. If not, see <http://www.gnu.org/licenses/>.

*/


#include <netlink/util.h>


long long NL_NAMESPACE_NAME::getTime() {

    #ifdef OS_WIN32

        SYSTEMTIME now;
        GetSystemTime(&now);
        long long wHour = now.wHour;
        long long wMinute = now.wMinute;
        long long wSecond = now.wSecond;
        long long wMilliseconds = now.wMilliseconds;
        long long milisec = wHour *3600*1000 + wMinute *60*1000 + now.wSecond *1000 + wMilliseconds;
        return(milisec);

    #else

        struct timeval now;
        gettimeofday(&now, NULL);
        long long tv_sec = now.tv_sec;
        long long milisec = tv_sec * 1000 + now.tv_usec / 1000.0;
        return(milisec);

    #endif

}

