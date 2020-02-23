#include "gpcSRC.h"
extern U1 gpaALFadd[];
I8x4 gpaCAGEboxMUL[] = {
	{0,1,1,0},
	{1,0,1,0},
	{1,1,0,0},

};
I4x4::I4x4( const I8x4& b ) { x = b.x; y = b.y; z = b.z; w = b.w; }
I4x4 I4x4::TSrBOX( I4x4 T, I4 r )
{
	// a vektorokból korábban ki kell vonni az origot
	if( T.xyz_() == xyz_() )
		return xyz_();
	I4x4 D4 = T.xyz_()-xyz_();
	I4x2 SabsMX = xyz_().ABS().mx();

	if( SabsMX.x <= r )
		return xyz_();		// ben van a dobozban, nehogy megmozduljon
							// marad ahol van ez hiba

	if( D4.aXYZW[SabsMX.y]*aXYZW[SabsMX.y] > 0 )
		return T.xyz_();	// ha pozició és az írány szorzata pozitív azaz azonos írányú akkor távolodik
							// az jó távolodjon
	if( !D4.aXYZW[SabsMX.y] )
		return T.xyz_(); 	// soha nem éri el a dobozt valahol a végtelenben vagy azon túl egy kicsivel

	I8x4	D8 = D4;

	D8 *= (SabsMX.x-r);
	D8 /= D4.aXYZW[SabsMX.y] > 0 ? D4.aXYZW[SabsMX.y] : -D4.aXYZW[SabsMX.y];
	D8 += xyz_();

	if( (D8&gpaCAGEboxMUL[SabsMX.y]).ABS().mx().x > r )
		return T.xyz_();

	return D8;
}
I4x4 I4x4::TSrBALL( I4x4 T, I4 r )
{
	// a vektorokból korábban ki kell vonni az origot
	if( T.xyz_() == xyz_() )
		return xyz_();
	I8x4	S8 = xyz_(),
			T8 = T.xyz_();
	I8	ss = S8.qlen_xyz(),
		s = sqrt(ss),
		rr = r*r;
	if( ss < rr )
	{
		if( ss < 32*32 )
			return xyz_();

		return (S8*r)/sqrt(ss);	// azért kell () ne hogy az r/s-t csinálja korábban
	}

	I8x4 D8 = T8-S8;
	I8 	dd = D8.qlen_xyz(),
		d = sqrt(dd),
		b = (S8*D8)/d,
		bb = b*b;

	if( bb < rr )
		return T.xyz_();

	I8 mm = ss - bb,
		m = sqrt(mm);
	if( mm > rr )
		return T;
	b = sqrt(bb) - sqrt(rr-mm);
	if( b > d )
		return T;
	D8 *= b;
	D8 /= d;
	T8 = D8+S8;
	I8 chk = sqrt(T8.qlen_xyz());
	return T8;
}
I4x4 I4x4::mxR( I4 r )
{
	I8x4 S8 = *this;
	I8 s = S8.qlen_xyz();
	if( s < r*r )
		return *this;

	S8 *= r;
	S8 /= sqrt(s);
	return S8;
}
