//#include "gpcSRC.h"
#include "gpccrs.h"
#include "gpsGLSL.h"

F4& F4::operator *= ( const F4x4& b ) {
	return *this = b.x*x + b.y*y + b.z*z + b.t*w;
}
F4 F4::operator * ( const F4x4& b ) const {
	return b.x*x + b.y*y + b.z*z + b.t*w;
}

F4& F4::operator /= ( const F4x4& b ) {
	x = *this * b.x;
	y = *this * b.y;
	z = *this * b.z;
	*this -= b.t;
	return *this;
}
F4 F4::operator / ( const F4x4& b ) const {
	return F4( *this*b.x, *this*b.y, *this*b.z, w ) - b.t;
}

F2& F2::swpXY( const void* pV ) {
	((U1x4*)&x)->wzyx(pV,2);
	return *this;
}
F2& F2::sXY( const char* p_str, char** pp_str ) {
	U8 nLEN;
	gpmCLR;
	const char* pPAT = "+-0123456789.,";
	p_str += gpmVAN( p_str, pPAT, nLEN );
	x = gpmSTR2D(p_str);
	if( *p_str == ',' )
		p_str++;

	p_str += gpmVAN( p_str, pPAT, nLEN );
	y = gpmSTR2D(p_str);
	if( *p_str == ',' )
		p_str++;

	if( !pp_str )
		return *this;
	(*pp_str) = (char*)p_str;
	return *this;
}

F4& F4::swpXYZ0( const void* pV ) {
	((U1x4*)this)->wzyx(pV,3);
	return *this;
}

F4& F4::sXYZW( const char* p_str, char** pp_str ) {
	U8 nLEN;
	gpmCLR;
	const char* pPAT = "+-0123456789.,";
	p_str += gpmVAN( p_str, pPAT, nLEN );
	x = gpmSTR2D(p_str);
	if( *p_str == ',' )
		p_str++;

	p_str += gpmVAN( p_str, pPAT, nLEN );
	y = gpmSTR2D(p_str);
	if( *p_str == ',' )
		p_str++;

	p_str += gpmVAN( p_str, pPAT, nLEN );
	z = gpmSTR2D(p_str);
	if( *p_str == ',' )
		p_str++;

	p_str += gpmVAN( p_str, pPAT, nLEN );
	w = gpmSTR2D(p_str);
	if( *p_str == ',' )
		p_str++;
	if( !pp_str )
		return *this;
	(*pp_str) = (char*)p_str;
	return *this;
}

F4& F4::abLOAD( char* pS, U4 nS, gpeALF alfV, U1** ppV, size_t* pVn ) {
	//gpmCLR;
	F4 d(0.0), a, b;
	int nPLS = gpmNINCS( pS, "." );
	if( nPLS >= nS )
		return *this;

	pS += nPLS;
	nS -= nPLS;
	char* pSRC = NULL, *pI;
	switch( alfV ) {
		case gpeALF_XYR:
			a = ppV[1] ? *(I4x4*)ppV[1] : I4x4(0);
			b = ppV[0] ? *(I4x4*)ppV[0] : I4x4(0);
			d = a-b;
			std::cout	<< a.x << " " << a.y << " "<< a.z << "\n"
						<< b.x << " " << b.y << " "<< b.z << std::endl;
			d.w = 0.0;
			pSRC = "xyr";
			break;
		default: break;
	}
	if( !pSRC )
		return *this;

	for( int i = 0, c; i < nS; i++ ) {
		c = pS[i];
		if( c >= '0' && c <= '9' ) {
			aXYZW[i%4] = c;
			continue;
		}
		switch(c){
			case '_':
				continue;
			default:
				pI = strchr( pSRC, pS[i] );
				if(!pI)
					continue;
				aXYZW[i%4] = d.aXYZW[(pI-pSRC)%4];
		}
	}

	return *this;
}

F4x4& F4x4::lat( F4 e, F4 c, F4 u ) {
	gpmCLR;
	//e.x *= -1.0;
	t = c-e;
	z = t.N3();
	t.w = 1.0;
	y = u.N3();

	x = y.X3(z.N3()).N3();
	y = z.X3(x).N3();
	//*this = T4x4();
	return *this ;
}
