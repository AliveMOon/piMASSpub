#ifndef piMASS_H_INCLUDED
#define piMASS_H_INCLUDED
//~ MIT License
//
//~ Copyright (c) 2019 AliveMOon
//
//~ Permission is hereby granted, free of charge, to any person obtaining a copy
//~ of this software and associated documentation files (the "Software"), to deal
//~ in the Software without restriction, including without limitation the rights
//~ to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//~ copies of the Software, and to permit persons to whom the Software is
//~ furnished to do so, subject to the following conditions:
//
//~ The above copyright notice and this permission notice shall be included in all
//~ copies or substantial portions of the Software.
//
//~ THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//~ IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//~ FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//~ AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//~ LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//~ OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//~ SOFTWARE.

#include <exception>

#include <errno.h>
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <ctime>
#include <fstream>
#include <iostream>
#include <raspicam/raspicam.h>
#include <unistd.h> // for usleep()
#include <inttypes.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <ifaddrs.h>
#include <stdarg.h>
#include <sys/ioctl.h>
#include <sys/select.h>
#include <netdb.h>
#include <SDL.h>			//-lSDL2
#include <SDL2/SDL_image.h>

#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include "X11/bitmaps/gray"
#include <X11/Xatom.h>
//#include <X11/Xmu/CurUtil.h>
#include <X11/Xcursor/Xcursor.h>

//#include <bits/stdc++.h>

using namespace std;
typedef unsigned char	U1;
typedef volatile U1		vU1;
typedef unsigned short	U2;
typedef uint32_t		U4;	//unsigned int	U4;
typedef uint64_t		U8;

//typedef float		float;
typedef double		F8;

typedef char		I1;
typedef short		I2;
typedef int			I4;
typedef int64_t		I8;
typedef I8			LL;

typedef int			SOCKET;
typedef sockaddr	SOCKADDR;
typedef sockaddr_in	SOCKADDR_IN; //struct	sockaddr_in	addrinfo;

class float2;
class float3;
class float4;
class float4x4;
class double4;
class U24;
class U44;
class U82;
class U84;
class I44;
class I84;


#endif // piMASS_H_INCLUDED
