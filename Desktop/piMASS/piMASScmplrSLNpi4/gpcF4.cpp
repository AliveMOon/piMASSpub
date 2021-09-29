//#include "gpcSRC.h"
#include "gpccrs.h"
#include "gpsGLSL.h"
F4& F4::gr2core( const I4x2 xy, double rw ) {
	double	rx = ::PI*((double(xy.x)/(rw*2)) + 0.25),
			ry = ::PI*((double(xy.y)/(rw*2)) + 0.25),
			ctgX = ::cos(rx)/::sin(rx),
			ctgY = ::cos(ry)/::sin(ry),
			ctgXY2 = ctgX*ctgX + ctgY*ctgY,
			r0 = rw / ::sqrt(1.0+ctgXY2);

	ctgX *= r0;
	ctgY *= r0;
	ctgXY2 = ctgX*ctgX + ctgY*ctgY;
	r0 = ::sqrt( rw*rw - ctgXY2 );

	x = ctgX;
	y = ctgY;
	z = r0;

	w = ::sqrt(qlen_xyz());
	return *this;
}

F4& F4::gr2cyli( const I4x2 xy, double rw ) {
	double	rx = ::PI*((double(xy.x)/(rw*2)) + 0.25);

	x = -::cos(rx);
	y = ((double(xy.y)*2.0/double(rw)) - 1.0)*COSSIN45;
	z = ::sin(rx);

	w = ::sqrt(qlen_xz());
	return *this;
}
F4 F4::N3( void ) const { /// NORMALIZE
	float l = sqrtf(qlen_xyz());
	return F4( x/l, y/l, z/l );
}
F4 F4::X3( F4 b ) const {	/// CROSS PRODUCT
	return F4(
					y * b.z - z * b.y,
					z * b.x - x * b.z,
					x * b.y - y * b.x
			);
}
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
F2& F2::cnt2pot( I8 Cx, I8 Cy, float w, float r, U4 c, U4 m ) {
    float   RADx = (float(Cx/2)/float(c*m))*PI2,
            RADy = (float(Cy/2)/float(c*m))*PI2;
    w /= 2.0;
    F2  Ay=F2(cos(RADy),sin(RADy))*r - w, // +F2(-w,-w)
        Ax=F2(sin(RADx),-cos(RADx))*r + F2(-w,w),
        AxyH = (Ax-Ay)/2.0;
    float AxyQL = AxyH.qlen();
    *this = AxyH.right()*sqrt((r*r-AxyQL)/AxyQL)+AxyH+Ay;
    return *this;
}
F2& F2::pot2cnt( I8& Cx, I8& Cy, float w, float r, U4 c, U4 m, float trn ) {
    w /= 2.0; // motor távolság fele
    trn += PIp2;
    // -- > +
    F2  ARMxy = (*this + w)/2.0, // -F2(-w,-w) azaz +w
        Ay = ARMxy,
        Ax;
    float Aql = ARMxy.qlen();
    Ay += ARMxy.right()
        * sqrt((r*r-Aql)/Aql);

    Ax  = ARMxy = (*this - F2(-w,w))/2.0,
    Aql = ARMxy.qlen();

    Ax += ARMxy.right()
        * sqrt((r*r-Aql)/Aql);
    float   RADy = trn-acos(Ay.y/r),
            RADx = trn-acos(Ax.x/r);
    Cx = float(c*m)*RADx/PI2;
    Cy = float(c*m)*RADy/PI2;
    // kétszer anyit számolok 0-fent 1-lent
    Cx*=2;
    Cy*=2;
    return *this;
}
F2& F2::swpXY( const void* pV ) {
	((U1x4*)&x)->wzyx(pV,2);
	return *this;
}
F2& F2::swpXYflpY( const void* pV ) {
	((U1x4*)&x)->wzyx(pV,2);
	y = 1-y;
	return *this;
}
F2& F2::sXY( const char* p_str, char** pp_str ) {
	//U8 nLEN;
	gpmCLR;
	const char* pPAT = "+-0123456789.,";
	p_str += gpmVAN( p_str, pPAT, NULL ); //, nLEN );
	x = gpmSTR2D(p_str);
	if( *p_str == ',' )
		p_str++;

	p_str += gpmVAN( p_str, pPAT, NULL ); //, nLEN );
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
	//U8 nLEN;
	gpmCLR;
	const char* pPAT = "+-0123456789.,";
	p_str += gpmVAN( p_str, pPAT, NULL ); //, nLEN );
	x = gpmSTR2D(p_str);
	if( *p_str == ',' )
		p_str++;

	p_str += gpmVAN( p_str, pPAT, NULL ); //, nLEN );
	y = gpmSTR2D(p_str);
	if( *p_str == ',' )
		p_str++;

	p_str += gpmVAN( p_str, pPAT, NULL ); //, nLEN );
	z = gpmSTR2D(p_str);
	if( *p_str == ',' )
		p_str++;

	p_str += gpmVAN( p_str, pPAT, NULL ); //, nLEN );
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
			gpdCOUT	<< a.x << " " << a.y << " "<< a.z << "\n"
						<< b.x << " " << b.y << " "<< b.z << gpdENDL;
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
F4x4 F4x4::T3x3( float s ) {
	F4x4 c = *this;
	if( s == 0.0 ){
		c.z = c.y = c.x = 0.0;
		return c;
	}

	c.x.col4x3(&x.x);
	c.y.col4x3(&x.y);
	c.z.col4x3(&x.z);
	if( s == 1.0 )
		return c;

	c.x *= s;
	c.y *= s;
	c.z *= s;
	return c;
}
F4x4& F4x4::latR( F4 e, F4 c, F4 u ) {
	gpmCLR;

	z.aF2[0] = e.aF2[0];
	z.aF2[0].x *= -1.0f;
	float r = sqrt(z.aF2[0].qlen());

	x = u.N3().X3(z/r);
	z.aF2[0] *= e.aF2[1].x/r;
	z.z = e.aF2[1].y;

	t.z = sqrt(z.qlen_xyz());
	z /= t.z;
	x /= sqrt(x.qlen());
	y = z.X3(x).N3();

	*this = T3x3(1.0/t.z);
	t.z = -0.5;
	return *this ;
}

F4x4 F4x4::inv( void ) {
	F4x4 mxi; //dst;
	float tmp[12], src[16], det;
	float* p_src16 = (float*) this;
	float* p_dst16 = (float*) &mxi;

	 /* transpose matrix */
	/*   sor -> oszlop  */
	src[0 +  0]		= p_src16[0 * 4 + 0];
	src[0 +  4]		= p_src16[0 * 4 + 1];
	src[0 +  8]		= p_src16[0 * 4 + 2];
	src[0 + 12]		= p_src16[0 * 4 + 3];

	src[1 +  0]		= p_src16[1 * 4 + 0];
	src[1 +  4]		= p_src16[1 * 4 + 1];
	src[1 +  8]		= p_src16[1 * 4 + 2];
	src[1 + 12]		= p_src16[1 * 4 + 3];

	src[2 +  0]		= p_src16[2 * 4 + 0];
	src[2 +  4]		= p_src16[2 * 4 + 1];
	src[2 +  8]		= p_src16[2 * 4 + 2];
	src[2 + 12]		= p_src16[2 * 4 + 3];

	src[3 +  0]		= p_src16[3 * 4 + 0];
	src[3 +  4]		= p_src16[3 * 4 + 1];
	src[3 +  8]		= p_src16[3 * 4 + 2];
	src[3 + 12]		= p_src16[3 * 4 + 3];

	/* calculate pairs for first 8 elements (cofactors) */

	tmp[ 6] = src[ 8] * src[15];
	tmp[ 8] = src[ 8] * src[14];
	tmp[10] = src[ 8] * src[13];

	tmp[ 2] = src[ 9] * src[15];
	tmp[ 4] = src[ 9] * src[14];
	tmp[11] = src[ 9] * src[12];

	tmp[ 0] = src[10] * src[15];
	tmp[ 5] = src[10] * src[13];
	tmp[ 9] = src[10] * src[12];

	tmp[ 1] = src[11] * src[14];
	tmp[ 3] = src[11] * src[13];
	tmp[ 7] = src[11] * src[12];

		/* calculate first 8 elements (cofactors) */
		p_dst16[0]  = tmp[0]*src[5] + tmp[3]*src[6] + tmp[4]*src[7];
		p_dst16[0] -= tmp[1]*src[5] + tmp[2]*src[6] + tmp[5]*src[7];

		p_dst16[1]  = tmp[1]*src[4] + tmp[6]*src[6] + tmp[9]*src[7];
		p_dst16[1] -= tmp[0]*src[4] + tmp[7]*src[6] + tmp[8]*src[7];

		p_dst16[2]  = tmp[2]*src[4] + tmp[7]*src[5] + tmp[10]*src[7];
		p_dst16[2] -= tmp[3]*src[4] + tmp[6]*src[5] + tmp[11]*src[7];

		p_dst16[3]  = tmp[5]*src[4] + tmp[8]*src[5] + tmp[11]*src[6];
		p_dst16[3] -= tmp[4]*src[4] + tmp[9]*src[5] + tmp[10]*src[6];



		p_dst16[4]  = tmp[1]*src[1] + tmp[2]*src[2] + tmp[5]*src[3];
		p_dst16[4] -= tmp[0]*src[1] + tmp[3]*src[2] + tmp[4]*src[3];

		p_dst16[5]  = tmp[0]*src[0] + tmp[7]*src[2] + tmp[8]*src[3];
		p_dst16[5] -= tmp[1]*src[0] + tmp[6]*src[2] + tmp[9]*src[3];

		p_dst16[6]  = tmp[3]*src[0] + tmp[6]*src[1] + tmp[11]*src[3];
		p_dst16[6] -= tmp[2]*src[0] + tmp[7]*src[1] + tmp[10]*src[3];

		p_dst16[7]  = tmp[4]*src[0] + tmp[9]*src[1] + tmp[10]*src[2];
		p_dst16[7] -= tmp[5]*src[0] + tmp[8]*src[1] + tmp[11]*src[2];

	/* calculate pairs for second 8 elements (cofactors) */
	tmp[6]	= src[0]*src[7];
	tmp[8]	= src[0]*src[6];
	tmp[10]	= src[0]*src[5];

	tmp[2]	= src[1]*src[7];
	tmp[4]	= src[1]*src[6];
	tmp[11]	= src[1]*src[4];

	tmp[0]	= src[2]*src[7];
	tmp[5]	= src[2]*src[5];
	tmp[9]	= src[2]*src[4];

	tmp[1]	= src[3]*src[6];
	tmp[3]	= src[3]*src[5];
	tmp[7]	= src[3]*src[4];

		/* calculate second 8 elements (cofactors) */
		p_dst16[ 8]	 = tmp[ 0]*src[13] + tmp[ 3]*src[14] + tmp[ 4]*src[15];
		p_dst16[ 8]	-= tmp[ 1]*src[13] + tmp[ 2]*src[14] + tmp[ 5]*src[15];

		p_dst16[ 9]	 = tmp[ 1]*src[12] + tmp[ 6]*src[14] + tmp[ 9]*src[15];
		p_dst16[ 9]	-= tmp[ 0]*src[12] + tmp[ 7]*src[14] + tmp[ 8]*src[15];

		p_dst16[10]	 = tmp[ 2]*src[12] + tmp[ 7]*src[13] + tmp[10]*src[15];
		p_dst16[10]	-= tmp[ 3]*src[12] + tmp[ 6]*src[13] + tmp[11]*src[15];

		p_dst16[11]	 = tmp[ 5]*src[12] + tmp[ 8]*src[13] + tmp[11]*src[14];
		p_dst16[11]	-= tmp[ 4]*src[12] + tmp[ 9]*src[13] + tmp[10]*src[14];



		p_dst16[12]	 = tmp[ 2]*src[10] + tmp[ 5]*src[11] + tmp[ 1]*src[ 9];
		p_dst16[12]	-= tmp[ 4]*src[11] + tmp[ 0]*src[ 9] + tmp[ 3]*src[10];

		p_dst16[13]	 = tmp[ 8]*src[11] + tmp[ 0]*src[ 8] + tmp[ 7]*src[10];
		p_dst16[13]	-= tmp[ 6]*src[10] + tmp[ 9]*src[11] + tmp[ 1]*src[ 8];

		p_dst16[14]	 = tmp[ 6]*src[ 9] + tmp[11]*src[11] + tmp[ 3]*src[ 8];
		p_dst16[14]	-= tmp[10]*src[11] + tmp[ 2]*src[ 8] + tmp[ 7]*src[ 9];

		p_dst16[15]	 = tmp[10]*src[10] + tmp[ 4]*src[ 8] + tmp[ 9]*src[ 9];
		p_dst16[15]	-= tmp[ 8]*src[ 9] + tmp[11]*src[10] + tmp[ 5]*src[ 8];

	/* calculate determinant */
	det = src[0]*p_dst16[0] + src[1]*p_dst16[1] + src[2]*p_dst16[2] + src[3]*p_dst16[3];

	/* calculate matrix inverse */
	//if(det >= -0.0000001 && det <= 0.0000001) return dst;
	//det = 1.0f / det;
	mxi /= det;

	/*p_dst16[ 0] *= det;
	p_dst16[ 1] *= det;
	p_dst16[ 2] *= det;
	p_dst16[ 3] *= det;
	p_dst16[ 4] *= det;
	p_dst16[ 5] *= det;
	p_dst16[ 6] *= det;
	p_dst16[ 7] *= det;
	p_dst16[ 8] *= det;
	p_dst16[ 9] *= det;
	p_dst16[10] *= det;
	p_dst16[11] *= det;
	p_dst16[12] *= det;
	p_dst16[13] *= det;
	p_dst16[14] *= det;
	p_dst16[15] *= det;*/

	return mxi;
}
