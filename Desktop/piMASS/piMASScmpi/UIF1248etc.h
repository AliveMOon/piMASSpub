#pragma once
#ifndef UIF1248etc_H
#define UIF1248etc_H

#define NULL nullptr
#ifdef _WIN64
	typedef unsigned char		U1;
	typedef volatile U1			vU1;
	typedef unsigned short		U2;
	typedef unsigned int		U4;
	typedef unsigned __int64	U8;

	typedef double		F8;

	typedef char		I1;
	typedef short		I2;
	typedef int			I4;
	typedef __int64		I8;
	typedef I8			LL;
#else
	typedef unsigned char	U1;
	typedef volatile U1		vU1;
	typedef unsigned short	U2;
	typedef uint32_t		U4;	//unsigned int	U4;
	typedef uint64_t		U8;

	typedef double		F8;

	typedef signed char		I1;
	typedef signed short	I2;
	typedef int				I4;
	typedef int64_t			I8;
	typedef I8				LL;
#endif

#endif