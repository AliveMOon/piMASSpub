#include "gpcSRC.h"
extern U1 gpaALFadd[];
I8x4 gpaCAGEboxMSK[] = {
	{0,1,1,0},
	{1,0,1,0},
	{1,1,0,0},

};
I8x4 I4x4::operator * ( int b ) const
{
	return I8x4( a4x2[0]*b, a4x2[1]*b );
}
I8x4 I4x4::operator * ( I8 b ) const
{
	return I8x4( a4x2[0]*b, a4x2[1]*b );
}
I8x4 I4x4::operator * ( float b ) const { return *this * (I8)b; }
I8x4 I4x4::operator * ( double b ) const { return *this * (I8)b; }

I8x4 I4x4::operator & (const I4x2& b) const
{
	return I8x4( a4x2[0]&b, a4x2[1]&b );
}
I8x4 I4x4::operator & (const U4x2& b) const
{
	return I8x4( a4x2[0]&b, a4x2[1]&b );
}
I8x4 I4x4::operator & (const I4x4& b ) const
{
	return I8x4( a4x2[0]&b.a4x2[0], a4x2[1]&b.a4x2[1] );
}
I4x4::I4x4( const I8x4 b ) { x = b.x; y = b.y; z = b.z; w = b.w; }
//I4x4::I4x4( const I8x4& b ) { x = b.x; y = b.y; z = b.z; w = b.w; }
I4x4::I4x4( const F4 f4 )
{
	x = f4.x;
	y = f4.y;
	z = f4.z;
	w = f4.w;
}
I4x4& I4x4::operator = ( const F4 f4 )
{
	x = f4.x;
	y = f4.y;
	z = f4.z;
	w = f4.w;
	return *this;
}
I4x4 I4x4::mmABC( const I4x4& b, float alf, float mm ) const {
	F4 radA = *this, radB = b;
	radA /= alf;
	radB /= alf;
	F2	i0( sin(radA.x), cos(radA.x) ),
		i1 = i0.right(),
		t( sin(radB.x), cos(radB.x) );

	radB.x = acos(t*i0);
	if( t*i1 < 0.0 )
		radB.x = -radB.x;

	i0 = F2( sin(radA.y), cos(radA.y) );
	i1 = i0.right();
	t = F2( sin(radB.y), cos(radB.y) );

	radB.y = acos(t*i0);
	if( t*i1 < 0.0 )
		radB.y = -radB.y;

	i0 = F2( sin(radA.z), cos(radA.z) );
	i1 = i0.right();
	t = F2( sin(radB.z), cos(radB.z) );

	radB.z = acos(t*i0);
	if( t*i1 < 0.0 )
		radB.z = -radB.z;

	radB *= mm;
	radB.w = radB.abs0().mx();
	return radB;
}
/*I4x4 I4x4::chkABC( const I4x4& b, float alf, float mm ) const
{
	F4 fa = *this, fb = b;
	// radiánokat csinálunk
	fa /= alf*180.0/PI;
	fb /= alf*180.0/PI;
	fb = F4( cos(fa.x)-cos(fb.x), cos(fa.y)-cos(fb.y), cos(fa.z)-cos(fb.z) )*mm;
	fb.w = sqrt(fb.qlen_xyz());

	return fb;
}*/
I4x4 I4x4::ABC2xyz( I4x4 txyz, const I4x4& iABC ) const {

	F4x4 iMX;
	iMX.ABC(iABC, degX(180.0/PI) ).t = txyz-*this;

    txyz.w = txyz.abs0().mx().x ? iMX.t.abs0().mx() : 0;
	if( txyz.w )
		iMX.t.xyz_( iMX.z*sqrt(iMX.t.qlen_xyz()) );
	else {
		iMX.t.xyz_( iMX.z*mmX(200) );
		if( (iMX.z.z <= -COSSIN45) && (z > 0) )
			iMX.t.xyz_( iMX.t*(float(z)/(-iMX.t.z)) );
	}

	txyz.xyz_(iMX.t);
	txyz.w = txyz.abs0().mx().x;
	return txyz+xyz0();
}
I4x4 I4x4::TSrBOX( I4x4 T, I8 r )
{
	// a vektorokból korábban ki kell vonni az origot
	if( T.xyz0() == xyz0() )
		return xyz0();
	I4x4 D4 = T.xyz0()-xyz0();
	I4x2 SabsMX = abs0().mx();

	if( SabsMX.x <= r )
		return xyz0();		// ben van a dobozban, nehogy megmozduljon
							// marad ahol van ez hiba

	if( ((I8)D4.aXYZW[SabsMX.y]*(I8)aXYZW[SabsMX.y]) > 0 )
		return T.xyz0();	// ha pozició és az írány szorzata pozitív azaz azonos írányú akkor távolodik
							// az jó távolodjon
	if( !D4.aXYZW[SabsMX.y] )
		return T.xyz0(); 	// soha nem éri el a dobozt valahol a végtelenben vagy azon túl egy kicsivel

	I8x4	D8 = D4;

	D8 *= (SabsMX.x-r);
	D8 /= D4.aXYZW[SabsMX.y] > 0 ? D4.aXYZW[SabsMX.y] : -D4.aXYZW[SabsMX.y];
	D8 += xyz0();

	if( (D8&gpaCAGEboxMSK[SabsMX.y]).abs0().mx().x > r )
		return T.xyz0();

	return D8;
}
I4x4 I4x4::TSrBALL( I4x4 T, I8 r ) {
	// a vektorokból korábban ki kell vonni az origot
	if( T.xyz0() == xyz0() )
		return xyz0();
	I8x4	S8 = xyz0(),
			T8 = T.xyz0();
	I8	ss = S8.qlen_xyz(),
		s = sqrt(ss),
		rr = r*r;
	if( ss < rr )
	{
		if( ss < 32*32 )
			return xyz0();

		return (S8*r)/s;	// azért kell () ne hogy az r/s-t csinálja korábban
	}

	I8x4 D8 = T8-S8;
	I8 	dd = D8.qlen_xyz(),
		d = sqrt(dd),
		b = (S8*D8)/d,
		bb = b*b;
	if( b > 0 )
		return T.xyz0();// kifele megy had menjen

	/*if( bb < rr )
		return T.xyz0();*/

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
I4x4 I4x4::mxR( I8 r ) {
	I8x4 S8 = *this;
	I8 s = S8.qlen_xyz();
	if( s < r*r )
		return *this;

	S8 *= r;
	S8 /= sqrt(s);
	return S8;
}
I4x4 I4x4::drop( const I4x4 T, const I4x4 up, I8 d, I8 ti, I8 tn ) const
{
	if( ti <= 0 )
		return *this;
	if( ti >= tn )
		return T;
	if(d<0)
		d*=-1;
	I8x4	u4 = up.qlen_xyz() ? up.xyz0() : I4x4(0,0,1),
			s4 = xyz0(),
			t4 = T.xyz0(),
			r4 = t4-s4;
	I8 	uu = u4.qlen_xyz(), u = sqrt(uu),
		h = (u4*r4)/u;

	I8x4	h4 = (u4*h)/u,
			w4 = r4-h4;
	I8	hh = h*h, ah = h < 0 ? -h : h,
		ww = w4.qlen_xyz(), w = sqrt(w),
		a = d+ah, aa = a*a,

		vv = aa-hh, v = sqrt(vv),
		vw = ((v*w)/(v+a)),

		b = w-vw;

	I4x4 out = *this ;
	if( h < 0 )
		out += h4 + ((w4*vw)/w);
	else
		out += (w4*b)/w;

	double	sa = double(ah)/double(a),
			// ha h<0 lefelé dobjuk
			rad0 = h < 0 ? asin(sa) : 0,
			rad1 = h < 0 ? PI : PI-asin(sa),
			radA = (rad1-rad0)*double(ti)/double(tn) + rad0;

	out += (u4*(I8)(double(a)*sin(radA)))/u;
	if(!b)
		return out;

	out += (w4*(I8)(double(b)*sin(radA-PIp2)))/w;

	return out;
}
