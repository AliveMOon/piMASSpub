#ifndef GPCSRCINST_H_INCLUDED
#define GPCSRCINST_H_INCLUDED

#define _b c(gpeCsz_b)
#define _w c(gpeCsz_w)
#define _l c(gpeCsz_l)
#define _q c(gpeCsz_q)
#define _4 c(gpeCsz_4)

#define _ptr c(gpeCsz_ptr)
#define _a c(gpeCsz_a)
#define _c c(gpeCsz_c)

#define _B c(gpeCsz_B)
#define _W c(gpeCsz_W)
#define _L c(gpeCsz_L)
#define _Q c(gpeCsz_Q)

#define _f c(gpeCsz_f)
#define _d c(gpeCsz_d)

#define _K c(gpeCsz_K)
#define _OFF c(gpeCsz_OFF)

#define _nop	pMEM->inst( gpeOPid_nop )
#define _move	pMEM->inst( gpeOPid_mov )
#define _add	pMEM->inst( gpeOPid_add )
#define _sub	pMEM->inst( gpeOPid_sub )

#define _mul	pMEM->inst( gpeOPid_mul )
#define _div	pMEM->inst( gpeOPid_div )
#define _rem	pMEM->inst( gpeOPid_rem )
#define _swap	pMEM->inst( gpeOPid_SWAP )
#define _extB	pMEM->inst( gpeOPid_EXTB )
#define _ext	pMEM->inst( gpeOPid_EXT )
#define _extL	pMEM->inst( gpeOPid_EXTL )

#define A0 An(0)
#define A1 An(1)
#define A2 An(2)
#define A3 An(3)
#define A4 An(4)
#define A5 An(5)
#define A6 An(6)
#define A7 An(7)

#define IA0I IAnI(0)
#define IA1I IAnI(1)
#define IA2I IAnI(2)
#define IA3I IAnI(3)
#define IA4I IAnI(4)
#define IA5I IAnI(5)
#define IA6I IAnI(6)
#define IA7I IAnI(7)


#define D0 Dn(0)
#define D1 Dn(1)
#define D2 Dn(2)
#define D3 Dn(3)
#define D4 Dn(4)
#define D5 Dn(5)
#define D6 Dn(6)
#define D7 Dn(7)


#endif // GPCSRCINST_H_INCLUDED
