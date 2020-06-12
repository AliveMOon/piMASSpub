#include "gpcSRC.h"
#include "gpccrs.h"

extern U1 gpaALFsub[];
extern char gpaALF_H_sub[];
static char gpsNoWord[] = {
							"\\ \t\a\r\n*&/%+-|~^?!=$.,:;\'\"{}[]()"
						};
static char gpsOPERA[] = {
							"\\*&/%+-|~^?!=$.,:;{}[]()"
						};

#define SCOOP aSCOOP[bMN]
U4x4 gpcSRC::SRCmill( bool bNoMini, const char* pVAN ) {
	if( !this )
		return U4x4( gpdSRC_COLw, gpdSRC_ROWw );

	U1	*pALL = pSRCalloc( bNoMini ),
		*pUTF = pSRCstart( bNoMini ),
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
	bool bABC,
		 bMN = pMINI ? pMINI->p_alloc == pALL : false;

	aSCOOP[bMN].rst( pALL );
	dim.xyz_( 0 );
	U4x2 pos(0);
	U4	clrABC = U1x4(0,0,gpeCLR_blue2).u4,
		clrSTR = U1x4(0,0,gpeCLR_violet).u4,
		clrNUM = U1x4(0,0,gpeCLR_orange).u4,
		clrOPERA = U1x4(0,0,gpeCLR_green2).u4,
		clrNOTE = U1x4(0,0,gpeCLR_green).u4,
		clrBREAK = U1x4(0,0,gpeCLR_red2).u4;

	for( nS = gpmNINCS( pUi, pVAN ); pUi < pUe; nS = gpmNINCS( pUi, " \t\r\n" ) )
	{
		if( nS )
		{
			// elrakjuk kell
			SCOOP.DCTadd(pos,pUi,nS,clrBREAK);
			// ebben van a " \t\r\n"
			pos = lenMILL(pos,dim,pUi,pUi+nS);
			pUi += nS;
			if( pUi >= pUe )
				break;
		}

		if( bABC = gpmbABC(*pUi, gpaALF_H_sub) )
		{
			nA = gpmVAN( pUi, gpsNoWord, nU );
			SCOOP.DCTadd(pos,pUi,nA,clrABC);
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
			SCOOP.DCTadd(pos,pUi,nN,clrNUM);
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
				SCOOP.STRadd(pos,pUi,nSTR,clrSTR);
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
				SCOOP.NOTEadd(pos,pUi,nSTR,clrNOTE);
				pos = lenMILL(pos,dim,pUi,pUj);
				pUi = pUj;
				continue;
			default:
				break;
		}

		//nO = gpmVAN( pUi, " \t\r\n", nU );
		nO = gpmNINCS( pUi, gpsOPERA );
		SCOOP.DCTadd(pos,pUi,nO,clrOPERA);
		pos.x += nO;
		dim.a4x2[0].mx( pos );
		dim.z += nU;
		pUi += nO;
	}
	dim.a4x2[0]+=1;
	return dim;
}

bool gpcSRC::SRCmnMILLscn(
							I4x2 xy, I4x2 fWH,

							I4 fz, I4 zz,

							gpcCRS& crs,
							bool bNoMini
						) {
	if( !this )
		return false;

	I4 cr, i, j, n, sub = 0;
	I4x2	cxy = xy, Cxy, trafo(1,zz);
	U1	cC = 0, NX, preC,
		*pALL	= pSRCalloc( bNoMini ),
		*pSTRT	= pSRCstart( bNoMini ),
		b01		=  (((U1)(this==crs.apSRC[0]))<<1)
				  | ((U1)(this==crs.apSRC[1]));

	bool	bON = false,
			bONpre, bSEL = false, bSTR=false,
			bONorSTR = false,
			bMN = pMINI ? pMINI->p_alloc == pALL : false;
	I8x4	*pM0 = (I8x4*)SCOOP.mini.p_alloc;

	for( U4 nM = SCOOP.nMN(), m = 0; m < nM; m++ )
	{
		cxy = xy+pM0[m].rMNpos;
		i = pM0[m].rMNinxt.x;
		j = pM0[m].rMNinxt.y+i;

		if( (crs.iSTR.y < i) && (crs.iSTR.y > j) )
			continue;

		for( ; i < j; i++ )
		{
			bONpre = bON;
			if(i==crs.iSTR.x)
				bON = (b01==3);
			preC = cC-' ';
			cC = pALL[i];
			Cxy = cxy;
			Cxy.x += sub;

			if( bON )
			{
				if( crs.iSTR.y==crs.iSTR.x)
					bON = !((b01&1)&&(i>crs.iSTR.y));
				else
					bON = !((b01&1)&&(i>=crs.iSTR.y));
				if( !bON )
				{
					crs.crsOFF.a4x2[0] = Cxy;
					return true;
				}
			}

			if( cC&0x80 )
			{
				if( cC&0x40 )
					continue;
				cxy.x++;
				NX = pALL[i+1];
			}
			else switch( cC )
			{
				case '\r':
					sub = 0;
					cxy.x = xy.x;
					if(!bON)
						continue;
					cC = 'r';
					break;
				case '\n':
					sub = 0;
					cxy.x = xy.x;
					cxy.y++;
					if(!bON)
						continue;
					cC = 'n';
					break;
				case '\a':
					cxy.x = xy.x;
					cxy.y++;
					if(!bON)
						continue;
					cC = '.';
					break;
				case '\t':
					cxy.x = xy.x + (((cxy.x-xy.x)/4)+1)*4;
					if(!bON)
						continue;
					cC = '.';
					break;
				default:
					if(!bONorSTR)
					if( cC == '_' || cC == '#' )
					{
						if(Cxy.x > 0)
						{
							cr = Cxy*trafo;
							if( cC=='_' )
							{
								//pO[cr-1].y = 1;
								if( (preC/0x20)%2 )
									cxy.x++;
								else
									sub--;
							} else {
								//pO[cr-1].y = 2;
								sub--;
							}
						}
						continue;
					}
					cxy.x++;
					break;
			}


		}
	}
	return false;
}

I4x2 gpcSRC::SRCmnMILL(
							U1x4* pO,

							I4x2 xy, I4x2 fWH,

							I4 fz, I4 zz,

							gpcCRS& crs,
							bool bNoMini
						) {
	if( !this )
		return xy;
	if( (fWH.mn()<=0) || (xy.x>=fWH.x) || (xy.y>=fWH.y) )
		return xy;
	I4 cr, i, j, n, sub = 0;
	I4x2	cxy = xy, Cxy, trafo(1,zz);
	U1	cC, NX,
		*pALL	= pSRCalloc( bNoMini ),
		*pSTRT	= pSRCstart( bNoMini ),
		b01		=  (((U1)(this==crs.apSRC[0]))<<1)
				  | ((U1)(this==crs.apSRC[1]));

	bool	bON = false,
			bONpre, bSEL = false, bSTR=false,
			bONorSTR = false,
			bMN = pMINI ? pMINI->p_alloc == pALL : false;
	I8x4	*pM0 = (I8x4*)SCOOP.mini.p_alloc;

	U4 clr, tSTR = U1x4(0x10,1,1).typ().u4, cn;
	U1x4 c = 0;

	for( U4 nM = SCOOP.nMN(), m = 0; m < nM; m++ )
	{
		cxy = xy+pM0[m].rMNpos;
		if(cxy.y>=fWH.y)
			break;
		i = pM0[m].rMNinxt.x;
		n = pM0[m].rMNinxt.y;
		bSTR = tSTR == pM0[m].rMNinxt.w;
		clr = pM0[m].rMNclr;
		for( j = i+n; i < j; i++ )
		{
			c.u4 = clr;
			c.w = 0;
			bONpre = bON;
			if(i==crs.iSTR.x)
				bON = (b01==3);

			cC = pALL[i];
			Cxy = cxy;
			Cxy.x += sub;

			if( bON )
			{
				if( crs.iSTR.y==crs.iSTR.x)
					bON = !((b01&1)&&(i>crs.iSTR.y));
				else
					bON = !((b01&1)&&(i>=crs.iSTR.y));
				if( bON )
					c.z |= 0x10;
				else
					crs.crsOFF.a4x2[0] = Cxy;
			}

			cn = 1;
			NX = 0;
			bONorSTR = bON||bSTR;
			if( cC&0x80 )
			{
				if( cC&0x40 )
					continue;
				cxy.x++;
				NX = pALL[i+1];
			}
			else switch( cC )
			{
				case '\r':
					sub = 0;
					cxy.x = xy.x;
					if(!bON)
						continue;
					cC = 'r';
					break;
				case '\n':
					sub = 0;
					cxy.x = xy.x;
					cxy.y++;
					if(!bON)
						continue;
					cC = 'n';
					break;
				case '\a':
					cxy.x = xy.x;
					cxy.y++;
					if(!bON)
						continue;
					cC = '.';
					break;
				case '\t':
					cxy.x = xy.x + (((cxy.x-xy.x)/4)+1)*4;
					if(!bON)
						continue;
					cn = cxy.x-Cxy.x;
					cC = '.';
					break;
				default:
					if(!bONorSTR)
					if( cC == '_' || cC == '#' )
					{
						if(Cxy.x > 0)
						{
							cr = Cxy*trafo;
							if( cC=='_' )
							{
								pO[cr-1].y = 1;
								if( (pO[cr-1].w/0x20)%2 )
									cxy.x++;
								else
									sub--;
							} else {
								pO[cr-1].y = 2;
								sub--;
							}
						}
						continue;
					}
					cxy.x++;
					break;
			}

			if( (Cxy.mn()<0) || (Cxy.x>=fWH.x) || (Cxy.y>=fWH.y) )
				continue;

			cr = Cxy*trafo;
			if( cC <= ' ' )
			{
				if(!bON)
					continue;
				cC = (cC!=' ') ? '.' : ' ';
			}


			pO[cr] = c;
			pO[cr].w = cC-' ';
			if(!NX)
				continue;
			pO[cr].w += (NX&4)>>2;
		}
	}
	return xy;
}
