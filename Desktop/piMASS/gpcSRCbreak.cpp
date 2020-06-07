#include "gpcSRC.h"
extern U1 gpaALFsub[];
extern char gpaALF_H_sub[];
static char gpsNoWord[] = {
							"\\ \t\a\r\n*&/%+-|~^!=$.,:;\'\"{}[]"
						};
/*inline U4x2 lenMILL( U4x2 pos, U4x2&cr, U1* pUi, U1* pUie )
{
	for( ; pUi < pUie; pUi++ )
	{
		if( *pUi&0x80 )
		{
			if( !(*pUi&0x40) )
				continue;
			pos.x++;
		}
		else switch( *pUi )
		{
			case '\r':
				pos.x = 0;
				break;
			case '\n':
				pos.x = 0;
				pos.y++;
				break;
			case '\a':
				pos.x = 0;
				pos.y++;
				break;
			case '\t':
				pos.x = ((pos.x/4)+1)*4;
				break;
			default:
				pos.x++;
				break;
		}
		cr.mx( pos );
	}
	return pos;
}*/

U4x4 gpcSRC::SRCmill( bool bNoMini, const char* pVAN )
{
	if( !this )
		return U4x4( gpdSRC_COLw, gpdSRC_ROWw );

	U1	*pUTF = pSRCstart( bNoMini ),
		*pUi = pUTF, *pUj = pUTF, //*pUk = pUTF,
		*pUe = pUi+dim.w,
		sIZE[] = " \a";
	if( !pVAN )
	{
		dim.z = gpfUTFlen( pUTF, pUe, dim.x, dim.y );
		return dim;
	}
	U8 nU;
	U4 nS, nA, nN, nO, nX, u8;
	I8 nSTR;
	double d8;
	bool bABC;
	SCOOP.rst( pUTF );
	dim.xyz_( 0 );
	U4x2 pos(0);
	for( nS = gpmNINCS( pUi, pVAN ); pUi < pUe; nS = gpmNINCS( pUi, " \t\r\n" ) )
	{
		if( nS )
		{
			// elrakjuk kell
			SCOOP.DCTadd( pos, pUTF, pUi, nS );
			// ebben van a " \t\r\n"
			pos = lenMILL(pos,dim,pUi,pUi+nS);
			pUi += nS;
			if( pUi >= pUe )
				break;
		}

		if( bABC = gpmbABC(*pUi, gpaALF_H_sub) )
		{
			nA = gpmVAN( pUi, gpsNoWord, nU );
			SCOOP.DCTadd( pos, pUTF, pUi, nA );
			pos.x += nU;
			dim.z += nU;
			dim.a4x2[0].mx( pos );
			pUi += nA;
			continue;
		}

		if( gpmbNUM( *pUi ) )
		{
			pUj = pUi;
			u8 = gpfSTR2U8( pUj, &pUj );
			if( pUj < pUe ? *pUj == '.' : false )
			{
				d8 = u8;
				d8 += gpmSTR2D( pUj );
			}
			nN = pUj-pUi;
			SCOOP.DCTadd( pos, pUTF, pUi, nN );
			pos.x += nN;
			dim.z += nN;
			dim.a4x2[0].mx( pos );
			pUi=pUj;
			continue;
		}
		switch( sIZE[0] = *pUi )
		{
			case '\"':
			case '\'':
				pUj = pUi+1;
				while( pUj < pUe )
				{
					pUj += gpmVAN( pUj, sIZE, nU );
					if( pUj >= pUe ? true : (*pUj!=*sIZE) )
						break;

					pUj++;
					if( pUj[-2] != '\\' )
						break;

					pUj += gpmVAN( pUj, sIZE, nU );
				}
				nSTR = pUj-pUi;
				SCOOP.STRadd( pos, pUTF, pUi, nO );
				pos = lenMILL(pos,dim,pUi,pUj);
				pUi = pUj;
				continue;
			case '/':
				pUj = pUi+1;
				if( pUj < pUe )
				{
					switch(*pUj)
					{
						case '*':
							pUj++;
							nSTR = pUe-pUj;
							if( nSTR < 2 )
							{
								pUj = pUe;
								break;
							}
							nSTR = gpfMM( pUj, nSTR, (U1*)"*/", 2 );
							if( pUj+nSTR >= pUe )
								pUj = pUe;
							else
								pUj += nSTR+2;
							break;
						case '/':
							pUj += gpmVAN( pUi, "\a\r\n", nU );
							break;
						default:
							pUj = pUi;
							break;
					}
				}
				nSTR = pUj-pUi;
				if( !nSTR )
					break;
				SCOOP.NOTEadd( pos, pUTF, pUi, nO );
				pos = lenMILL(pos,dim,pUi,pUj);
				pUi = pUj;
				continue;
			default:
				break;
		}

		nO = gpmVAN( pUi, " \t\r\n", nU );
		SCOOP.DCTadd( pos, pUTF, pUi, nO );
		pos.x += nO;
		dim.a4x2[0].mx( pos );
		dim.z += nU;
		pUi += nO;
	}
	dim.a4x2[0]+=1;
	//dim.z = gpfUTFlen( pUTF, pUe, dim.x, dim.y );
	return dim;
}
