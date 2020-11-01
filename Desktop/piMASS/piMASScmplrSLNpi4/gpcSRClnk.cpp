#include "gpcSRC.h"
#include "gpcSRClnk.h"
#include "gpccrs.h"
extern U1 gpaALFsub[];
extern char gpaALF_H_sub[];
#define mCD0 (pCD=CDsp.piCD())[0]


#define aaOPid gpaaOPid[OPgrp]
size_t gpCNST::strASM( char* pS, char* pALL, I8x4 *pM0, U4x4 *pL0   ) {
	char sBstr[] = "FFFFffffFFFFffff",
		*pBD, nB = sizeof(sBstr)-1;
	size_t n;
	switch( typ )
	{
		case gpeTYP_sA8:
			return gpfALF2STR( pS, obj.alf );
		case gpeTYP_sA8N:
			return obj.an2str( pS );
		case gpeTYP_U1:
			return sprintf( pS, "0x%0.2x", obj.uy );
		case gpeTYP_U2:
			return sprintf( pS, "0x%0.4x", obj.uy );
		case gpeTYP_U4:
			return sprintf( pS, "0x%0.8x", obj.uy );
		case gpeTYP_U8:
			return sprintf( pS, "0x%0.16llx", obj.uy );

		case gpeTYP_I1:
			return sprintf( pS, "0x%.3d", obj.num );
		case gpeTYP_I2:
			return sprintf( pS, "0x%.6d", obj.num );
		case gpeTYP_I4:
			return sprintf( pS, "0x%.9d", obj.num );
		case gpeTYP_I8:
			return sprintf( pS, "0x%.12lld", obj.num );

		case gpeTYP_D:
		case gpeTYP_F:
			return sprintf( pS, "%0.7f", obj.dy );
		case gpeTYP_STR:
			n = pM0[obj.uy].iMNn;
			if( n > nB )
				n = nB;
			gpmMcpy( sBstr, pALL+(pM0[obj.uy]).iMNi, n );
			sBstr[n]=0;
			return sprintf( pS, "\"%s...\"", sBstr );
		default:
			n = sprintf( pS, "0x%0.8llx,0x%0.8llx", obj.x, obj.y );
			break;
	}
	return n;
}
gpcO* gpcPIK::fnd( gpcO* pM, gpcO& dot, U4 nmID ) {
	gpcO* pO = NULL;
	if( !pM )
	{
		pO = gpmLZYvali( gpcO, &oLST );
		for( U4 i = 0, n = oLST.nLD(sizeof(*pO)); i < n; i++ )
		{
			if( pO[i].nmID != nmID )
				continue;
			return pO+i;
		}
		return NULL;
	}

	pO = (dot=pM);

	gpcC* pC = gpmLZYvali( gpcC, &cLST );
	if( !pC )
		return NULL;

	pC += dot.cID;
	U4x4	*pCL = pC->pLST;
	U4		*pKD = pC->pKID, szNM,
			ary;
	for( U4 i = 0, n = pC->nLST, iK = 0, off = 0; i < n; i++ )
	{
		szNM = pCL[i].x;
		ary = pCL[i].y;
		if( !ary )
		{
			ary = pCL[i].y = pCL[i].a4x2[1].area();
			if( !ary )
				ary = pCL[i].y = (pCL[i].a4x2[1] = 1).x;
		}
		if( (szNM>>4) != nmID )
		{
			// nem ez!
			szNM &= 0xf;
			if( szNM < gpeCsz_K )
			{
				off += ary*gpaCsz[szNM];
				continue;
			}

			off += ary*gpmLZYvali( gpcC, &cLST )[pKD[iK]].szOF;
			++iK;
			continue;
		}
		dot.nmID = szNM;
		dot.pALL += off;
		dot.an = pCL[i].a4x2[1];
		return &dot;
	}
	return NULL;
}





