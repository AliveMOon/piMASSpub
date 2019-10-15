#ifndef GPCSRC_H
#define GPCSRC_H

#include "piMASS.h"
//#include "gpcSCHL.h"
#include "gpcOPCD.h"

class gpcRES;
//#include "gpcres.h"

//#include "gpccrs.h"



#define gpdSRC_COLw 4
#define gpdPRGsep " \t\r\n\a .,:;!? =<> -+*/%^ &~|@#$ \\ \" \' ()[]{} "

//extern U1 gpaALFadd[]; //0x100];
//extern U1 gpsSTRpub[]; //[0x10000];

class gpcCRS;

enum gpeMASSsw:U8
{
	gpeMASSzero,
	gpeMASSsub = gpeMASSzero,
	gpeMASSret,
	gpeMASSentr,
	gpeMASSunsel,
	gpeMASSinp,
	gpeMASSpass,
	gpeMASSoff,
	gpeMASSmain,
	gpeMASSzn,

	gpeMASSalert,
	gpeMASSprg,
	gpeMASSgpu,
	gpeMASScrsL,
	gpeMASScrsR,


	gpeMASSsubMSK = 1<<gpeMASSsub,
	gpeMASSretMSK = 1<<gpeMASSret,
	gpeMASSentrMSK = 1<<gpeMASSentr,
	gpeMASSunselMSK = 1<<gpeMASSunsel,
	gpeMASSinpMSK = 1<<gpeMASSinp,
	gpeMASSpassMSK = 1<<gpeMASSpass,
	gpeMASSoffMSK = 1<<gpeMASSoff,
	gpeMASSmainMSK = 1<<gpeMASSmain,
	gpeMASSznMSK = 1<<gpeMASSzn,

	gpeMASSclrMSK = (1<<gpeMASSalert)-1,
	gpeMASSalertMSK,
	gpeMASSprgMSK = 1<<gpeMASSprg,
	gpeMASSgpuMSK = 1<<gpeMASSgpu,
	gpeMASScrslMSK = 1<<gpeMASScrsL,
	gpeMASScrsrMSK = 1<<gpeMASScrsR,


};
inline U4 gpfUTF8( const U1* pS, U1** ppS )
{
	U4 utf8 = *pS;
	if( !(utf8&0x80) )
	{
		if( !ppS )
			return utf8;

		*ppS = ((U1*)pS) + (!!utf8);
		return utf8;
	}

	if( (utf8&0xc0) == 0x80 )
	{
		// azaz hibás 0x40-nek igaznak kéne lenie
		while( (utf8&0xc0) == 0x80 )
		{
			pS++;
			utf8 = *pS;
			if( !utf8 )
			{
				if( !ppS )
					return 0;

				*ppS = (U1*)pS;
				return 0;
			}
		}

		if( !ppS )
			return '.';

		*ppS = (U1*)pS;
		return '.';
	}

	U4	u0 = utf8<<1, n = 0;
	U4	static const aADD[] = { 0, 		0x80,	0x800,	0x10000, 0x200000 };
	U4	static const aMSK[] = { 0x7F, 	0x7FF,	0xFFFF, 0x1FFFFF };

	while( u0&0x80 )
	{
		n++;
		u0 <<= 1;
		utf8 <<= 6;
		utf8 |= pS[n]&0x3f;
	}
	utf8 += aADD[n];
	utf8 &= aMSK[n];
	if( !ppS )
		return utf8;
	pS += n+1;
	*ppS = (U1*)pS;
	return utf8;
}

inline U8 gpfVAN( const U1* pU, const U1* pVAN, U8& nLEN, bool bDBG = false )
{
	// return nBYTE
	// nLEN = nCODE
	nLEN = 0;
	if( pU ? !*pU : true )
		return 0;

	U1 *pS = (U1*)pU;
	if( !pVAN )
	{
		// gpmSTRLEN!!
		while( *pS )
		{
			if( (*pS&0xc0) != 0x80 )
				nLEN++;	// csak a 0x80 asokat nem számoljuk bele mert azok tötike karakterek
			pS++;
		}
		return pS-pU;
	}
	U8 nVAN = gpfVAN( pVAN, NULL, nLEN ); // utf8len
	bool abVAN[0x80];
	gpmZ( abVAN );
	*abVAN = true;
	gpcLAZY* pTREE = NULL;
	U4 u4, nT = 0;

	if( nVAN == nLEN )
	{
		//van-ban nincsen UTF8 akkor turbó
		abVAN[pVAN[0]] = true;
		for( U8 v = 1; v < nLEN; v++ )
		{
			abVAN[pVAN[v]] = true;
		}

		while( *pS )
		{
			if( *pS >= 0x80 )
				break;
			if( abVAN[*pS] )
				return nLEN = pS-pU;

			pS++;
		}

		nLEN = pS-pU;
		if( !*pS )
			return nLEN;

		if( bDBG )
			nLEN = pS-pU;

	} else {
		U1	*pV = (U1*)pVAN,
			*pE = pV+nVAN;
		while( pV < pE )
		{
			u4 = gpfUTF8( pV, &pV );
			if( u4 < 0x80 )
			{
				abVAN[u4] = true;
				continue;
			}

			pTREE = pTREE->tree_add( u4, nT );
		}

		nLEN = 0;
	}
	U1* pSS;
	while( *pS )
	{
		u4 = gpfUTF8( pS, &pSS );
		if( u4 < 0x80 )
		{
			if( abVAN[u4] )
				break;
			pS++;
			nLEN++;
			continue;
		}

		if( pTREE->tree_fnd(u4, nT) < nT )
			break;

		pS = pSS;
		nLEN++;
	}

	return pS-pU;
}
inline U8 gpfUTFlen( U1* pU, U1* pUe, U4& col, U4& row, U1* pVAN = NULL )
{
	U8 nLEN = 0, nBYTE, nUTF8 = 0;
	if( !pVAN )
		pVAN = (U1*)"\t\r\n\a";
	col = row = 0;
	U1	*pS,
		aCHR[] = " ";
	U4 xx = 0, n;
    while( *pU ? pU < pUe : false )
    {
		nBYTE = gpfVAN( pU, pVAN, nLEN );
		nUTF8 += nLEN;
		xx += nLEN;
		if( col < xx )
			col = xx;

		pS = pU+nBYTE;
		aCHR[0] = *pS;
		n = gpmNINCS( pS, aCHR );
		switch( *pS )
		{
			case '\r':
				if( pS[1] != '\n' )
				{
					xx = 0;
					break;
				}
				n++;
				xx = 0;
				row++;
				break;
			case '\n':
				xx = 0;
				row += n;
				break;
			case '\a':
				row += n;
				xx = 0;
				break;
			case '\t':
				xx = (xx/4 + n)*4;
				if( col >= xx )
					break;

				col = xx;
				break;
			case ' ':
				xx++;
				break;
		}
		nUTF8 += n;
		pU = pS+n;

    }
	col++;
	row++;
	return nUTF8;
}
inline U8 gpfVANn( U1* pS, const U1* pVAN )
{
	if( !pVAN )
		return 0;
	U2 nV = strlen( (char*)pVAN );
	if( !nV )
		return 0;

	U4 n = 0, nVAN[0x80];
	gpmZ( nVAN );

	while( *pS )
	{
		if( *pS < 0x80 )
		{
			nVAN[*pS]++;
			pS++;
			continue;
		}

		(*nVAN)++;
		pS++;
	}

	for( U8 i = 0; i < nV; i++  )
	{
		if( pVAN[i] < 0x80 )
			n += nVAN[pVAN[i]];
	}

	return n;
}
U8 inline gpfABC( U1* p_str, U1* pE, U8& nLEN );

inline U1* gpfUTF8left( U1* pB, U1* pS, U1 n, U1 b )
{
	if( pB >= pS )
		return pB;

	if( b )
	{
		while( pS > pB )
		{
			if( pS[-1] == n || pS[-1] == b  )
				return pS;
			pS--;
		}

		return pB;
	}

	while( pS > pB )
	{
		if( pS[-1] == n )
			return pS;
		pS--;
	}

	return pB;
}
inline U1* gpfUTF8stpX( U1* pB, U1* pE, U4 x, U4 nT = 4 )
{
	if(!x)
		return pB;

	U4 xUP = 0;
	while( pB < pE )
	{
		if( xUP >= x )
			return pB;
		if( (pB[0]&0x80) )
		{
			xUP++;
			pB += 2;
			continue;
		}

		if( pB[0] == '\t' )
			xUP = ((xUP/nT)+1)*nT;
		else
			xUP++;

		pB++;
	}
	return pB;
}
inline U4 gpfUTF8rig( U1* pB, U1* pE, U4 nT = 4 )
{
	U4 r = 0;
	while( pB < pE )
	{
		if( (pB[0]&0x80) )
		{
			r++;
			pB += 2;
			continue;
		}

		if( pB[0] == '\t' )
			r = ((r/nT)+1)*nT;
		else
			r++;

		pB++;
	}
	return r;
}

U8 inline gpfSTR2U8( U1* p_str, U1** pp_str = NULL )
{
	if( !p_str )
		return 0;
	U8 u8 = strtol( (char*)p_str, (char**)&p_str, 10 );
	if( !u8 )
	{
		switch( *p_str )
		{
			case 'x':
			case 'X':
				p_str++;
				u8 = strtol( (char*)p_str, (char**)&p_str, 16 );
				break;
			case 'b':
			case 'B':
				p_str++;
				u8 = strtol( (char*)p_str, (char**)&p_str, 2 );
				break;
			case 'd':
			case 'D':
				p_str++;
				u8 = strtol( (char*)p_str, (char**)&p_str, 10 );
				break;
		}
	} /*else {
		u8 = (U8)log2(u8);
		switch( *p_str ) /// 1 2 4 8 16
		{
			case 'x':
			case 'X':
				p_str++;
				u8 = ((gpmSTR2U8( p_str, 16 )<<1)|1) << u8;
				break;
		}
	}*/
	if( pp_str )
		*pp_str = p_str;
	return u8;
}
#define gpmVAN( d, v, l ) gpfVAN( (d), (U1*)v, l )
I8 inline gpfSRC2I8( U1* p_str, U1** pp_str = NULL )
{
	if( !p_str )
		return 0;
	U8 nLEN;
    p_str += gpmVAN(p_str, "+-0123456789xXbBdD", nLEN );
	I8 i8 = strtol( (char*)p_str, (char**)&p_str, 10 );
	if( !i8 )
	{
		switch( *p_str )
		{
			case 'x':
			case 'X':
				p_str++;
				i8 = strtol( (char*)p_str, (char**)&p_str, 16 );
				break;
			case 'b':
			case 'B':
				p_str++;
				i8 = strtol( (char*)p_str, (char**)&p_str, 2 );
				break;
			case 'd':
			case 'D':
				p_str++;
				i8 = strtol( (char*)p_str, (char**)&p_str, 10 );
				break;
		}
	}
	if( pp_str )
		*pp_str = p_str;
	return i8;
}

class gpcLAY
{
public:
	gpeALF	id;		// 0
	gpeNET4	typ;	// 8
	void	*pVOID;	// 12
	gpcLAY() { gpmCLR; }

	void* pRE( U4 nZ, U4 nN, U4 oz, U4 oy )
	{
		if( !this )
			return NULL;

		void* pKILL = pVOID;
		nZ *= *(U1*)&typ;
		oz *= *(U1*)&typ;

		pVOID = (void*)(new U1[nZ*nN]);
		gpmZn( (U1*)pVOID, nZ*nN );

		for( U1* pS = (U1*)pKILL, *pD = (U1*)pVOID, *pSe = pS+oz*oy ; pS < pSe; pS += oz, pD += nZ  )
		{
			gpmMEMCPY( pD, pS, oz );
		}

		gpmDELary( pKILL );
		return pVOID;
	}

};
class gpcMAP
{
public:
	U4x4	mapZN44;	// xy load zw alloc
	U4		*pMAP, *pCOL, *pROW, nLAY;
	gpcLAY	*pLAY;

	gpcMAP(void)
	{
		gpmCLR;
	};
	~gpcMAP()
	{
		gpmDELary(pMAP);
	}
	U4* MAPalloc( U4x4& spcZN, U4x4& outZN, bool bRST )
	{
		// mCR -
		if(!this)
		{
			outZN = 0;
			return NULL;
		}

		if( mapZN44.x <= spcZN.x )
			mapZN44.x = spcZN.x+1;
		if( mapZN44.y <= spcZN.y )
			mapZN44.y = spcZN.y+1;

		outZN = mapZN44;
		if(	(mapZN44.z > mapZN44.x) && (mapZN44.w > mapZN44.y) )
		{
			if( bRST )
			{
				*pCOL = gpdSRC_COLw;
				*pROW = 1;
				gpfMEMSET( (pCOL+1), mapZN44.z-1, pCOL, sizeof(*pCOL) );
				gpfMEMSET( (pROW+1), mapZN44.w-1, pROW, sizeof(*pROW) );
			}
			return pMAP;
		}

		U4	*pK = pMAP,
			*pKC = pK ? pK+outZN.a4x2[1].area() : NULL, //AREAzw(),
			*pKR = pK ? pKC+outZN.z : NULL;

		mapZN44.z = max( mapZN44.z, gpmPAD( mapZN44.x, 0x10 ) );
		mapZN44.w = max( mapZN44.w, gpmPAD( mapZN44.y, 0x10 ) );

		U4 	nARE = mapZN44.a4x2[1].area(),
			nALL = nARE + mapZN44.a4x2[1].sum();

		pMAP = new U4[nALL];
		pROW = (
					pCOL = pMAP+nARE
				) + mapZN44.z;

		*pCOL = gpdSRC_COLw;
		*pROW = 1;
		*pMAP = 0;

		//gpmZn( pMAP, nARE );
		gpfMEMSET( (pMAP+1), nARE-1, pMAP, sizeof(*pMAP) );
		gpfMEMSET( (pCOL+1), mapZN44.z-1, pCOL, sizeof(*pCOL) );
		gpfMEMSET( (pROW+1), mapZN44.w-1, pROW, sizeof(*pROW) );

		if( pK )
		{
			if( !bRST )
			{
				gpmMEMCPY( pCOL, pKC, outZN.z );
				gpmMEMCPY( pROW, pKR, outZN.w );
			}

			for( U4* pS = pK, *pD = pMAP; pS < pKC; pS += outZN.z, pD += mapZN44.z  )
			{
				gpmMEMCPY( pD, pS, outZN.z );
			}
			gpmDELary(pK);
		}
		for( U4 i = 0; i < nLAY; i++ )
		{
			pLAY[i].pRE( mapZN44.z, mapZN44.w, outZN.z, outZN.y );
		}
		outZN = mapZN44;
		return pMAP;
	}
};


class gpcSRC
{
public:
    U1  	*pA, *pB;			// pA - alloc *pB - tartalom
    U8		nL,
			nA,			// ha nA == 0 nm mi foglaltuk
			bSW;		// pB = pA+iB()
    U4x4	spcZN, dim;
    U4		IX, retIX, nALFtg, strtD, endD;

    gpeALF	*pALFtg;
    gpcRES*	apRES[4];
	gpcLAZY	*pEXE,
			//*pRES,
			*pMINI,
			*pBIG;

	gpcMAP	*pMAP;

	U4 updt( void )
	{
		U8 nLEN = 0;
		pB = pA + gpfVAN( pA, (U1*)"\a", nLEN );
		nVER = nHD+1;

		//ifpB = pA + gpfVAN( pA, (U1*)"\a", nLEN );

		return nVER;
	}

	U4 nCLR(void)
	{
		return gpmOFF( gpcSRC, nVER )-gpmOFF( gpcSRC, pALFtg );
	}
	U1* pSRCalloc( void )
	{
		bool bHD = false, bMINI = bHD ? false : !!pMINI;

		return bMINI ? pMINI->p_alloc : pA;
	}
	U1* pSRCstart( void ) // U4x4* pCx2 )
	{
		// bHD akkor igaz, ha szerkesztés alatt van a rublika
		// és ráadásul a \a elöt6t van a cursor
		bool bHD = false, bMINI = bHD ? false : !!pMINI;

		U1	*pC = bMINI ? //(U1*)
							pMINI->p_alloc : pA;
		dim.w = bMINI ? pMINI->n_load : nL;


		if( !bHD )
		{
			pC += iB()+1;
			dim.w -= (pC-pA);
		}

		return pC;
	}

	U8 CRSminiCR( I4x2& cr ) {
		if( !this )
		{
			cr.null();
			return 0;
		}
		cr.y = min( cr.y, dim.y-1 );

		//if( !cr.sum() )
		//	return pC - pSRCalloc();;
		I4x4 cxy = 0;
		U1 sC[] = " ", *pROW = pSRCstart(), *pCe, *pC;
		U4 n;
		for( pC = pROW, pCe = pC+dim.w; pC < pCe; pC++ )
		{
			if( cxy.y >= cr.y )
				break;

			switch( *pC )
			{
				case '\r':
					if( pC[1] != '\n' )
					{
						cxy.x = 0;
						continue;
					}
					pC++;
					pROW = pC+1;
					cxy.x = 0;
					cxy.y++;
					continue;
				case '\n':
					pROW = pC+1;
					cxy.x = 0;
					cxy.y++;
					continue;
				case '\a':
					pROW = pC+1;
					cxy.y++;
					cxy.x = 0;
					continue;
				case '\t':
					sC[0] = *pC;
					n = gpmNINCS( pC+1, sC );
					cxy.x = 0 + (cxy.x/4 + n)*4 + 4;
					pC += n;
					continue;
				case ' ':
					cxy.x++;
					continue;
			}

			if( *pC < ' ' )
				continue;
			if( *pC & 0x80 )
				pC++;
			cxy.x++;
		}

		cxy.x = 0;
		for( pC = pROW; pC < pCe; pC++ )
		{
			if( cxy.x >= cr.x )
			{
				break;
			}
			else if( *pC&0x80 )
			{
				pC++;
			}

			switch( *pC )
			{
				case '\n':
				case '\r':
				case '\a':
					return pC - pSRCalloc();

				case '\t':
					sC[0] = *pC;
					n = gpmNINCS( pC+1, sC );
					cxy.x =(cxy.x/4 + n)*4 + 4;
					pC += n;
					continue;
				case ' ':
					cxy.x++;
					continue;
			}

			cxy.x++;
		}
		return pC - pSRCalloc();
	}
	I4x4 CRSmini( U1x4* pO, U4x4* pCx2, I4x4 xy, I4 fx, I4 fy, I4 fz, U4* pC64, gpcCRS& crs, gpeCLR bg, gpeCLR fr, gpeCLR ch );

	U4x4 CRSdim( U4x4* pCRS2 )
	{
		if( !this )
			return U4x4( 4, 1 );
		U1* pC = pSRCstart( ); //pCRS2 );
        dim.z = gpfUTFlen( pC, pC+dim.w, dim.x, dim.y ); // x oszlop y sor

		return dim;
	}

    bool qBLD( void )
    {
		if( nVER > nBLD )
			return true;	// már kérte valaki

		nVER = max( nHD, nBLD )+1;
		return false; // mi kérjük elösször
    }

    bool bSUB( gpcMASS& mass )
    {
		if( !this )
			return false;

		hd( mass );

		// beljebb lép
		return bSW&gpeMASSsubMSK;
    }
    bool bRET( gpcMASS& mass )
    {
		if( !this )
			return false;

		hd( mass );

		// kijebb lép
		return bSW&gpeMASSretMSK;
    }
    bool bENTR( gpcMASS& mass, U4x4& _spc, U4 x = 0 )
    {
		if( !this )
			return false;


		hd( mass );

		// új sort kezd a táblázatban
		if( bSW&gpeMASSznMSK )
		{
			_spc = spcZN;
			_spc.x++;
			return false;
		}
		if( !(bSW&gpeMASSentrMSK) )
			return false;

		spcZN.x = x;
		spcZN.y++;
		_spc = spcZN;
		_spc.x++;
		return true;

    }
    bool bUNsel( gpcMASS& mass )
    {
		if( !this )
			return false;

		hd( mass );

		// pointerrel ne lehessen kijelölni
		// pl. rajzolásnál hasznos, meg gombnál
		return bSW&gpeMASSunselMSK;
    }

	bool bIN( gpcMASS& mass )
    {
		if( !this )
			return false;

		// input rublika
		hd( mass );

		return bSW&gpeMASSinpMSK;
    }

    bool bPASS( gpcMASS& mass )
    {
		if( !this )
			return false;

		hd( mass );

		// csillagokat kell írni a betű helyett?

		return bSW&gpeMASSpassMSK;
    }
    bool bALERT( void )
    {
		if( !this )
			return false;

		return bSW&gpeMASSalertMSK;
    }
    bool bMAIN( gpcMASS& mass, bool bDBG = false )
    {
		if( !this )
			return false;
		hd( mass );

		bool bM = bSW&gpeMASSmainMSK;
		if( !bM )
			return false;

		cmpi( mass, bDBG );

		return true;
    }
    void hd( gpcMASS& mass, gpeALF* pTGpub = NULL );
    void cmpi( gpcMASS& mass, bool bDBG );
	void cmpi_undo00( gpcMASS& mass, bool bDBG );
    void cmpi_SKELETON( gpcMASS& mass, bool bDBG );

    //void cmpi_trash2( gpcMASS& mass, bool bDBG );
    //void cmpi_trash( gpcMASS& mass, bool bDBG );

    gpcSRC();
    virtual ~gpcSRC();
	U8 iB( void )
	{
		if( !pA || !nL )
			return 0;

		if( pB < pA )
			pB = pA;

		U8 i = pB-pA;
		if( i )
		if( i >= nL )
		{
			if( i == nL )
				return nL; // ezzel jelezük, ha valaki már kereste
							// azaz nem 0

			pB = ( pA ? pA+nL : NULL );
			return nL;
		}

		if( pB[i] == '\a' )
			return i;	// ez a fasza

		// még van esély keressük meg, hátha há
		U8 nLEN;
		i = gpfVAN( pA, (U1*)"\a", nLEN );
		if( i )
		if( i >= nL )
		{
			if( i == nL )
				return nL;	// i = nL -> ezzel jelezük, hogy kerestük de biza nem vót // szar

			pB = ( pA ? pA+nL : NULL );
			return nL;
		}

		// na meguszta talált egyet
		pB = pA+i;
		return i;
	}
	gpcSRC& reset( U1* pC, U1* pE, U1** ppSRC, U4x4& spcZN, U4 nADD = 1 );

    gpcSRC& SRCcpy( U1* pC, U1* pE );


    gpcSRC( gpcSRC& B );
    gpcSRC& operator = ( gpcSRC& B );

protected:
    U4	nVER, nBLD, nHD;
private:
};

// #include "gpcSCHL.h"
class gpcCLASS
{

	gpcLAZY	*paLZY,
			*pLST, *pFND;
	I8	nLST, nCLASS,
		idFND, ixFND;

	gpcLAZY** ppCLASS( I8 ix )
	{
		if( ix >= nLST )
			return NULL;

		gpcLAZY** ppC = NULL;
		if( paLZY )
			ppC = (gpcLAZY**)paLZY->p_alloc;

		nCLASS = ppC ? paLZY->n_load/sizeof(pFND) : 0;

		if( nCLASS <= ix )
		{
			I8	nALL = paLZY ? paLZY->n_alloc/sizeof(pFND) : 0,
				nADD;
			if( nALL <= ix )
			{
				nALL = ix+1;
				nADD = nALL-nCLASS;

				U8 s = -1;

				paLZY = paLZY->lzy_add( NULL, nADD*sizeof(pFND), s );
				ppC = NULL;
				if( paLZY )
					ppC = (gpcLAZY**)paLZY->p_alloc;
				else
					return NULL;

				if( !ppC )
					return NULL;

				gpmZn( ppC+nCLASS, nADD );
				nCLASS = paLZY->n_load/sizeof(pFND) ;
			}
		}
		return ppC+ix;
	}
	gpcCLASS( I8 id )
	{
		gpmCLR;
		I8 n = 0;
		pLST = pLST->tree_add( id, n );
		if( !pLST )
			return;

		nLST = n;
	}
public:
	gpcCLASS( void )
	{
		gpmCLR;
	}

	gpcLAZY* pGET( I8 ix )
	{
		if( ix >= nLST )
			return NULL;

		gpcLAZY** ppC = ppCLASS( ix );
		if( !ppC )
			return NULL;

		return *ppC;
	}
	gpcLAZY** ppGET( I8 ix )
	{
		if( ix >= nLST )
			return NULL;

		return ppCLASS( ix );
	}

	I8 fnd( I8 id )
	{
		if( id ? !this : true )
			return nLST;

		if( nLST )
		if( idFND == id )
			return ixFND;

		return pLST->tree_fnd(id, nLST);
	}
	gpcLAZY* p_fnd( I8 id, I8& ix )
	{
		ix = 0;
		if( id ? !this : true )
			return NULL;
		if( !nLST )
			return 0;

		if( idFND == id )
		{
			ix = ixFND;
			return pFND;
		}

		ix = pLST->tree_fnd(id, nLST);
		if(ix >= nLST)
			return NULL;

		gpcLAZY** ppC = ppCLASS( ix );
		if( ppC ? !*ppC : true )
		{
			ix = nLST;
			return NULL;
		}

		ixFND = ix;
		idFND = id;
		return pFND = *ppC;
	}
	gpcCLASS* add( I8 id, I8& ix, I8& n )
	{
		if( !id )
		{
			ix = n = (this ? nLST: 0);
			return this;
		}
		if( !this )
		{
			gpcCLASS* pSCHL = new gpcCLASS(id); ///
			ix = 0;
			n = 1;
			return pSCHL;
		}

		ix = pLST->tree_fnd( id, nLST );
		if( ix < nLST )
		{
			n = nLST;
			return this;
		}

		pLST = pLST->tree_add(id, nLST);
		n = nLST;
		/*if( ix >= nLST ) // is és nLST - ha továbbra is egyenlő akkor nem tudta hozzá adni
			return this;

		ppCLASS( ix );
		if( nCLASS > ix )
			return this;

		U8 s = -1;
		nALLOC = nLST;
		pKIDS = pKIDS->lzy_add( NULL, sizeof(*pKIDS), s );*/
		return this;
	}
};

class gpcMASS
{
	gpcCLASS	*pTG;
	gpcLAZY		*pSRCc,
				*pLST;
	U4			nLST, xADD, xFND, nALLOC, nSP, nOP0, nOP1, iMAIN,
				aSPix[0x100];
	gpcSRC		*pFND,
				*apSP[0x100];
	U4x4 		aSP44[0x100];

	gpcSRC** ppSRC( void )
	{
		return (gpcSRC**)(pSRCc ? pSRCc->p_alloc : NULL);
	}
public:
	gpcMAP		mapCR;

	gpeALF		aTGwip[0x100];
	gpcOPCD		aPRG[0x1000];
	U1			asPRG[0x1000],
				*pPUB; //, nDICT;

	// CMPL ----------------------------
	gpcLAZY		CMPL;
	gpcCMPL		PC;
	U4			aPC[0x100], iPC,
				aiDAT[0x100], alDAT;

	// CPLD ----------------------------



	gpcLAZY		*apDICTopcd[0x1000];
	gpcLZYdct	*apDICTix[0x1000];
	U4			 anDICTix[0x1000],
				//aLEVsp[0x100],
				rstLEV, iLEV, nLEV, topLEV;

	U4 relLEV( void )
	{
		return iLEV-rstLEV;
	}

	gpcCMPL* piLEVmom( void )
	{
		return CMPL.pPC( aPC[iLEV-1] );
	}
	gpcCMPL* piLEViPC( void )
	{
		return CMPL.pPC( aPC[iLEV] );
	}
	U4 incLEV( void )
	{

		aPC[iLEV] = iPC;
		U4 iDT = aiDAT[iLEV];

		iLEV++;

		aiDAT[iLEV] = iDT;
		aPC[iLEV] = iPC;

		nLEV = iLEV+1;
		return iLEV;
	};
	U4 decLEV( void )
	{
		iPC = aPC[iLEV];
		if( iLEV > rstLEV )
			iLEV--;
		else
			iLEV = rstLEV;
		while( asPRG[iLEV] == ')' )
		{
			//cout << ")";
			asPRG[iLEV] = 0;
			if( iLEV > rstLEV )
				iLEV--;
		}
		nLEV = iLEV+1;
		return iLEV;
	};

	//void reset_o( void );
	U1* msRST( U1* pPUB );
	void tag_add( gpeALF tg, U4 iKID )
	{
		I8 ix, n;
		U8 s = -1;

        pTG = pTG->add( (I8)tg, ix, n );

        gpcLAZY* pLZY = pTG->pGET( ix ); 	///
		if( !pLZY )
		{
			*pTG->ppGET( ix ) = pLZY = new gpcLAZY;
		}
		pLZY->lzy_add( (U4*)&iKID, sizeof(U4), s, 8 );
	}
	void tag_sub( gpeALF tg, U4 iKID )
	{
		if( !pTG )
			return;
		I8 ix;
		gpcLAZY* pLZY = pTG->p_fnd( tg, ix );
		if( !pLZY )
			return;

		U4	*paLZY = (U4*)pLZY->p_alloc;
		U8 nKID = pLZY->n_load/sizeof(ix);
		for( U8 i = 0; i < nKID; i++ )
		{
			if( paLZY[i] != iKID )
				continue;
			nKID--;
			if( !nKID )
				continue;

			paLZY[i] = paLZY[nKID];
			i--;
		}
		if( !nKID )
		{
			gpmDEL( *pTG->ppGET( ix ) );
			return;
		}
		pLZY->n_load = nKID*sizeof(iKID);
	}

	/// gpcMASS:: find in main.cpp
	gpcSRC* SRCnew( gpcSRC& tmp, U1* pS, I4x2 an );
	bool HTMLsave( U1* pPATH, U1* pFILE, U1* pNAME, bool bALT );
	bool SRCsave( U1* pPATH, U1* pFILE );
	U1* justDOit( U1* sKEYbuff, I4x4& mouseXY, U4* pKT, I4x4& SRCxycr, I4x4& SRCin );


	gpcMASS&	null();
	gpcMASS&	operator = ( const gpcMASS& b );
	/// -------------------



	gpcMASS(){ gpmCLR; }
	gpcMASS( const U1* pU, U8 nU );
	gpcMASS( const gpcMASS& b )
	{
		*this = b;
	}


	virtual ~gpcMASS();
	gpcSRC* get( U4 i )
	{
		if( i >= nLST )
			return NULL;
		gpcSRC** ppS = ppSRC();
		return ppS ? ppS[i] : NULL;
	}
	gpcSRC* SRCfnd( U4 xfnd )
	{
		if(!this)
			return NULL;

		if( nLST )
		if( xFND == xfnd )
			return pFND;

		U4 iFND = pLST->tree_fnd(xfnd, nLST);
		if(iFND >= nLST)
			return NULL;

		gpcSRC** ppS = ppSRC();
		if( ppS ? !ppS[iFND] : true )
			return pFND;

		pFND = ppS[iFND];
		xFND = xfnd;

		return pFND;
	}

	gpcSRC* SRCadd( gpcSRC* pSRC, U4 xfnd, U4& is, U4& n );



};

#endif // GPCSRC_H
