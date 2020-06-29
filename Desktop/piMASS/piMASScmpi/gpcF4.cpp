#include "gpcSRC.h"

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
