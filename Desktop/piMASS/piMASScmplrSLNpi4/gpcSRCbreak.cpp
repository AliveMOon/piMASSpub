#include "gpcSRC.h"
#include "gpccrs.h"

extern U1 gpaALFsub[];
extern char gpaALF_H_sub[];
gpcSCOOP** gpcLZY::ppSCP( int i ) {	// NULL - ha nincs elég
 	return (gpcSCOOP**)pU1i( i, sizeof(gpcSCOOP*) );
}

gpcSCOOP** gpcLZY::ppSCPn( int i, int n ) {	// i < 0  újat ad a végéről
	return (gpcSCOOP**)Ux(
							( i<0 ? nLD(sizeof(gpcSCOOP*)) : i ),
							sizeof(gpcSCOOP*)*n,
							true, sizeof(gpcSCOOP*)
						);
}

size_t gpcLZY::npSCP( size_t x ) {
	return ( x ? nLD(x*sizeof(gpcSCOOP*)) : nLD(sizeof(gpcSCOOP*)) );
}

gpcSCOOP::~gpcSCOOP(){
	size_t n = this ? lzySCP.npSCP() : 0;
	if( !n )
		return;

	for( size_t i = 0; i < n; i++ )
		gpmDEL( *lzySCP.ppSCP(i) );
};

void gpcSCOOP::rst( U1* pUTF ) {
	lzyMiN.lzyRST();

	lzyDCT.rst();
	lzyLiNK.lzyRST();
	lzyCNST.lzyRST();
	lzyASM.lzyRST();
	for( size_t i = 0, n = lzySCP.npSCP(); i < n; i++  )
		gpmDEL( *lzySCP.ppSCP(i) );
	lzySCP.lzyRST();
	nDCT = nLiNK = nCNST = nMiN = nINS;
	pALL = pUTF;	// ezt a gpcSRC::srcBRK adja
}


U4 gpcSCOOP::STRadd(	U4x2 pos, U1* pS, size_t nUi, U4 color, //U4 iSTR,
						gpcSRC* pSRC, gpcLZYdct& dOP, bool bNoMini, U1 selID, const char* pVAN ) {
	if( !nUi )
		return 0;

	if( !nUi )
		return nMN();

	// typ U4x4.w:
	/// x[7s,6f,5r,4str : 3-0 nBYTE = 1<<(x&0xf) ]
	/// yz[ dimXY ] 	, w nBYTE //= 1<<(x&0xf)
	//U1x4 typ(0x10,1,1,0);

	rMN.iMNindt = U4x4( pS-pALL, nUi, -1, gpeTYP_STR );
	rMN.rMNpos = pos;
	rMN.rMNclr = color;
	U8 s = -1;
	lzyMiN.lzyADD( &rMN, sizeof(rMN), s );

	gpcSCOOP** ppSCP = lzySCP.ppSCPn(-1);
	/// *(ppSCP[-1])
	if( !(*ppSCP) )
		(*ppSCP) = new gpcSCOOP;

	pSRC->srcBRK(	dOP, bNoMini, selID, pVAN,
					pos+U4x2(1,0), pS+1, nUi-2, *ppSCP );
	return nMN();
}
/// 5. srcBREAK scp.MN scp.DCT scp.LNK
U4x4 gpcSRC::srcBRK( 	gpcLZYdct& dOP,
						bool bNoMini, U1 selID, const char* pVAN,
						U4x2 pos, U1* pUi, size_t nUi, gpcSCOOP* pSCPin ) {
	if( !this )
		return U4x4( gpdSRC_COLw, gpdSRC_ROWw );

	U1	*pALL = pSRCalloc( bNoMini, selID ),
		*pUTF = pUi ? pUi : pSRCstart( bNoMini, selID ),
		*pS = pUTF, *pUj = pUTF,
		*pUe = pS+(nUi?nUi:dim.w),
		*pNULL = pALL+dim.w,
		sIZE[] = " \a";
	if( !pVAN ) {
		dim.z = gpfUTFlen( pUTF, pUe, dim.x, dim.y );
		return dim;
	}

	gpeOPid opID;
	size_t nU;
	U4 nS, nA, nN, nO, nX, u8, nSTP, nOx, iSTR = 0;
	I8 nSTR = 0;
	double d8;
	bool bABC;
	U1 iSCP = pMINI ? pMINI->p_alloc == pALL : false;
	if( pDBG ? pDBG->p_alloc == pALL : false )
		iSCP = 2;

	dim.xyz_(0);
	U4	clrABC = U1x4(0,0,gpeCLR_blue2).u4,
		clrSTR = U1x4(0,0,gpeCLR_violet).u4,
		clrNUM = U1x4(0,0,gpeCLR_orange).u4,
		clrOPERA = U1x4(0,0,gpeCLR_green2).u4,
		clrNOTE = U1x4(0,0,gpeCLR_green).u4,
		clrBREAK = U1x4(0,0,gpeCLR_red2).u4;
	gpcSCOOP* pSCP = pSCPin;

	if( pSCP ) {
		pSCP->rst( aSCP[iSCP].pALL );
	}
	else {
		pSCP = aSCP+iSCP;
		if( pSCP->bGDhs( pALL, &pUe ) )
			return (dim = pSCP->dim);
		pSCP->rst( pALL );
		/// berakjuk a 0-t gyökérnek
		pSCP->DCTadd(pos,pNULL,1,clrABC);
	}

#ifdef stdON
	if(bSTDcout){gpdCOUT << stdBREAK "BRK" << (int)iSCP << stdRESET << gpdENDL;}
#endif
	for( nS = gpmNINCS( pS, pVAN ); pS < pUe; nS = gpmNINCS( pS, " \t\r\n" ) ) {
		//if(bSTDcout){gpdCOUT << stdBREAK "." << (int)(pS-pALL) << stdRESET << gpdENDL;}
		if( nS ) {
			// elrakjuk kell
			pSCP->DCTadd(pos,pS,nS,clrBREAK);
			/// ebben van a " \t\r\n"
			pos = lenMILL(pos,dim,pS,pS+nS);
			pS += nS;
			if( pS >= pUe )
				break;
		}

		if( bABC = gpmbABC(*pS, gpaALF_H_sub) ) {
			nA = gpmVAN( pS, gpsNoWord, &nU );
			pSCP->DCTadd(pos,pS,nA,clrABC);
			pos.x += nU;
			dim.z += nU;
			dim.a4x2[0].mx( pos );
			pS += nA;
			continue;
		}

		if( gpmbNUM( *pS ) ) {
			pUj = pS;
			u8 = gpfSTR2U8( pUj, &pUj );
			if( pUj < pUe ? *pUj == '.' : false ) {
				d8 = u8;
				d8 += gpmSTR2D( pUj );
			}
			nN = pUj-pS;
			pSCP->DCTadd(pos,pS,nN,clrNUM);
			pos.x += nN;
			dim.z += nN;
			dim.a4x2[0].mx( pos );
			pS=pUj;
			continue;
		}
		switch( sIZE[0] = *pS ) {
			case '\\':
				pUj = pS+gpmNINCS( pS, sIZE );
				while( pUj < pUe ) {
					pUj++;
					if( pUj < pUe ? *pUj == '\\' : false ) {
						pUj += gpmNINCS( pUj, sIZE );
						continue;
					}
					break;
				}
				nS = pUj-pS;
				if( nS ) {
					// elrakjuk kell
					pSCP->DCTadd(pos,pS,nS,clrBREAK);
					/// ebben van a " \t\r\n"
					pos = lenMILL(pos,dim,pS,pS+nS);
					pS += nS;
					if( pS >= pUe )
						break;
				}
				continue;
			case '\"':
			case '\'':
				pUj = pS+1;
				while( pUj < pUe ) {
					pUj += gpmVAN( pUj, sIZE, &nU );
					if( pUj >= pUe ? true : (*pUj!=*sIZE) )
						break;

					pUj++;
					if( pUj[-2] != '\\' )
						break;

					pUj += gpmVAN( pUj, sIZE, &nU );
				}
				nSTR = pUj-pS;
				pSCP->STRadd(	pos, pS, nSTR, clrSTR,
								this, dOP, bNoMini, selID, pVAN );
				pos = lenMILL(pos,dim,pS,pUj);
				pS = pUj;
				continue;
			case '/':
				pUj = pS+1;
				if( pUj < pUe ) {
					switch(*pUj) {
						case '*':
							pUj++;
							nSTR = pUe-pUj;
							if( nSTR < 2 ) {
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
							pUj += gpmVAN( pS, "\a\r\n", &nU );
							break;
						default:
							pUj = pS;
							break;
					}
				}
				nSTR = pUj-pS;
				if( !nSTR )
					break;
				pSCP->NOTEadd(pos,pS,nSTR,clrNOTE);
				pos = lenMILL(pos,dim,pS,pUj);
				pS = pUj;
				continue;
			default:
				break;
		}

		//nO = gpmVAN( pS, " \t\r\n", nU );
		nO = gpmNINCS( pS, gpsOPERA );
		if( !nO ) {
			pS++; // gpmVAN( pS, pVAN, nU );
			continue;
		}

		opID = (gpeOPid)dOP.dctMILLfnd( pS, nO, nOx = (U4)gpeOPid_jsr );
		nSTP = dOP.nSTRix(opID);
		if( nO > nSTP )
			nO = nSTP;

		pSCP->DCTadd(pos,pS,nO,clrOPERA);
		pos.x += nO;
		dim.a4x2[0].mx( pos );
		dim.z += nU;
		pS += nO;
	}
	dim.a4x2[0]+=1;
#ifdef stdON
	if(bSTDcout) { gpdCOUT << stdBREAK "oBRK" << (int)iSCP << stdRESET << gpdENDL; }
#endif
	return (pSCP->dim = dim);
}

U1 gpcSRC::srcSCN( gpcCRS& crs, bool bNoMini, U1 selID ) {
	if( !this )
		return 0;

	I4	i, j, n, sub = 0;
	I4x2	cxy = 0, Cxy;
	U1	oo = 0, cC = 0, NX, preC,
		*pALL	= pSRCalloc( bNoMini, selID ),
		*pSTRT	= pSRCstart( bNoMini, selID ),
		b01		=  (((U1)(this==crs.apSRC[0]))<<1)
				  | ((U1)(this==crs.apSRC[1]));

	bool	bON = false,
			bONpre, bSEL = false,
			bSTR=false,
			bONorSTR = false;

	U1 iSCP = pMINI ? pMINI->p_alloc == pALL : false;
	if( pDBG ? pDBG->p_alloc == pALL : false )
		iSCP = 2;

	I8x4 *pM0 = aSCP[iSCP].pMN();
	U4 tSTR = U1x4(0x10,1,1).typ().u4;
	for( U4 nM = aSCP[iSCP].nMN(), m = 0; m < nM; m++ )
	{
		cxy = pM0[m].rMNpos;
		i = pM0[m].iS;
		j = pM0[m].nS+i;

		if( (crs.iSTR.y < i) && (crs.iSTR.y > j) )
			continue;
		bSTR = (tSTR==pM0[m].iMNindt.w);
		for( ; i <= j; i++ )
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
				if( bONpre != bON )
				{
					// most lett BE kapcsolva
					crs.aCRSonPG[0].a4x2[0] = Cxy;
					oo |= 1;
				}
				if( crs.iSTR.y==crs.iSTR.x)
					bON = !((b01&1)&&(i>crs.iSTR.y));
				else
					bON = !((b01&1)&&(i>=crs.iSTR.y));
				if( !bON )
				{
					// most lett KI kapcsolba
					crs.aCRSonPG[1].a4x2[0] = Cxy;
					oo |= 2;
					return oo;
				}
			}
			if( i >= j )
				break;

			bONorSTR = bON||bSTR;
			if( cC&0x80 ) {
				if( cC&0x40 )
					continue;
				cxy.x++;
				NX = pALL[i+1];
			}
			else switch( cC ) {
				case '\r':
					sub = 0;
					cxy.x = 0;
					if(!bON)
						continue;
					cC = 'r';
					break;
				case '\n':
					sub = 0;
					cxy.x = 0;
					cxy.y++;
					if(!bON)
						continue;
					cC = 'n';
					break;
				case '\a':
					cxy.x = 0;
					cxy.y++;
					if(!bON)
						continue;
					cC = '.';
					break;
				case '\t':
					cxy.x = ((cxy.x/4)+1)*4;
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
							//cr = Cxy*trafo;
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



I4x2 gpcSRC::srcFORm(	U1x4* pO,
						U1 *pALL, I4x2 cxy, I4x2 trafo,
						I4x2 xy, I4x2 fWH,
						I4 i, I4 n, U4 clr,
						gpcMiNshr& bSHR, gpcCRS& crs ) {
	I4x2 Cxy;
	U1x4 chr = 0;
	U1 cC, nx;
	U4 cn;
	I4 cr;
	for( I4 j = i+n; i < j; i++ ) {
		chr.u4 = clr;
		chr.w = 0;
		bSHR.bONpre = bSHR.bON;
		if(i==crs.iSTR.x)
			bSHR.bON = (bSHR.b01==3);

		cC = pALL[i];
		Cxy = cxy;
		Cxy.x += bSHR.sub;
		if( bSHR.bON ) {
			if( crs.iSTR.y == crs.iSTR.x )
				bSHR.bON = !((bSHR.b01&1)&&(i>crs.iSTR.y));
			else
				bSHR.bON = !((bSHR.b01&1)&&(i>=crs.iSTR.y));
			if( bSHR.bON )
				chr.z |= 0x80;
		}

		cr = 1;
		nx = 0;
		bSHR.bONorSTR = bSHR.bON||bSHR.bSTR;
		if( cC&0x80 ) {
			if( cC&0x40 )
				continue;
			cxy.x++;
			nx = pALL[i+1];
		}
		else switch( cC ) {
			case '\r':
				bSHR.sub = 0;
				cxy.x = xy.x;
				if(!bSHR.bON)
					continue;
				cC = 'r';
				break;
			case '\n':
				bSHR.sub = 0;
				cxy.x = xy.x;
				cxy.y++;
				if(!bSHR.bON)
					continue;
				cC = 'n';
				break;
			case '\a':
				cxy.x = xy.x;
				cxy.y++;
				if(!bSHR.bON)
					continue;
				cC = '.';
				break;
			case '\t':
				cxy.x = xy.x + (((cxy.x-xy.x)/4)+1)*4;
				if(!bSHR.bON)
					continue;
				cn = cxy.x-Cxy.x;
				cC = '.';
				break;
			default:
				if(!bSHR.bONorSTR)
				if( cC == '_' || cC == '#' ) {
					if(Cxy.x > 0) {
						cr = Cxy*trafo;
						if( cC=='_' ) {
							pO[cr-1].y = 1;
							if( (pO[cr-1].w/0x20)%2 )
								cxy.x++;
							else
								bSHR.sub--;
						}
						else {
							pO[cr-1].y = 2;
							bSHR.sub--;
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
		if( cC <= ' ' ) {
			if(!bSHR.bON)
				continue;
			cC = (cC!=' ') ? '.' : ' ';
		}


		pO[cr] = chr;
		pO[cr].w = cC-' ';
		if(!nx)
			continue;
		pO[cr].w += (nx&4)>>2;
	}
	return cxy;
}
I4x2 gpcSRC::srcRDY(
							U1x4* pO,

							I4x2 xy, I4x2 fWH,

							I4 fz, I4 zz,

							gpcCRS& crs,
							bool bNoMini, U1 selID
						) {
	if( !this )
		return xy;
	if( (fWH.mn()<=0) || (xy.x>=fWH.x) || (xy.y>=fWH.y) )
		return xy;
	I4 cr, i, j, n, sub = 0;
	I4x2	cxy = xy, //Cxy,
			trafo(1,zz); //, lxy;
	U1	cC, NX,
		*pALL	= pSRCalloc( bNoMini, selID ),
		*pSTRT	= pSRCstart( bNoMini, selID );

	gpcMiNshr bSHR(
					  (((U1)(this==crs.apSRC[0]))<<1)
					| ((U1)(this==crs.apSRC[1]))
				);

	U1 iSCP = pMINI ? (pMINI->p_alloc == pALL) : false;
	if( pDBG ? pDBG->p_alloc == pALL : false )
		iSCP = 2;

	I8x4	*pM0 = aSCP[iSCP].pMN(), *pMs0;
	U4	clrABC = U1x4(0,0,gpeCLR_blue2).u4,
		clrSTR = U1x4(0,0,gpeCLR_violet).u4,
		clrNUM = U1x4(0,0,gpeCLR_orange).u4,
		clrOPERA = U1x4(0,0,gpeCLR_green2).u4,
		clrNOTE = U1x4(0,0,gpeCLR_green).u4,
		clrBREAK = U1x4(0,0,gpeCLR_red2).u4;

	U4 clr, tSTR = U1x4(0x10,1,1).typ().u4, cn;
	U1x4 chr = 0;
	size_t iSTR = -1;
	gpcSCOOP* pSCP;

	for( U4 nM = aSCP[iSCP].nMN(), nMs, m = 0; m < nM; m++ ) {
		cxy = xy+pM0[m].rMNpos;
		if(cxy.y>=fWH.y)
			break;

		i = pM0[m].iS;
		n = pM0[m].nS;
		bSHR.bSTR = ((chr=pM0[m].rMNclr) == clrSTR);
			// (tSTR == pM0[m].iMNindt.w);

		if( bSHR.bSTR ) {
			iSTR++;
			if( pSCP = aSCP[iSCP].pSCP(iSTR) )
			if( (nMs = pSCP->nMN()) > 1 )
			if( pMs0 = pSCP->pMN() ) {
				if( (cxy.mn()>=0) && (cxy.x<fWH.x) && (cxy.y<fWH.y) ) {
					bSHR.bONpre = bSHR.bON;
					if(i==crs.iSTR.x)
						bSHR.bON = (bSHR.b01==3);
					if( bSHR.bON ) {
						if( crs.iSTR.y == crs.iSTR.x )
							bSHR.bON = !((bSHR.b01&1)&&(i>crs.iSTR.y));
						else
							bSHR.bON = !((bSHR.b01&1)&&(i>=crs.iSTR.y));
						if( bSHR.bON )
							chr.z |= 0x80;
					}
					cr = cxy*trafo;
					pO[cr] = chr;
					pO[cr].w = '\"'-' ';
				}

				for( U4 m = 0; m < nMs; m++ ) {
					cxy = xy+pMs0[m].rMNpos;
					if(cxy.y>=fWH.y)
						break;

					//i = pMs0[m].iS;
					//n = pMs0[m].nS;
					bSHR.bSTR = ((chr=pM0[m].rMNclr) == clrSTR);
						//(tSTR == pMs0[m].iMNindt.w);
					chr = pMs0[m].rMNclr;
					chr.z |= 4<<4;
					cxy = srcFORm( 	pO,
									pALL, cxy, trafo,
									xy, fWH,
									pMs0[m].iS, pMs0[m].nS, chr.u4, bSHR, crs );
				}

				//if(false)
				if( (cxy.mn()>=0) && (cxy.x<fWH.x) && (cxy.y<fWH.y) ) {
					bSHR.bONpre = bSHR.bON;
					if(i==crs.iSTR.x)
						bSHR.bON = (bSHR.b01==3);
					if( bSHR.bON ) {
						if( crs.iSTR.y == crs.iSTR.x )
							bSHR.bON = !((bSHR.b01&1)&&(i>crs.iSTR.y));
						else
							bSHR.bON = !((bSHR.b01&1)&&(i>=crs.iSTR.y));

						if( bSHR.bON )
							chr.z |= 0x80; // GREEN
					}
					cr = cxy*trafo;
					pO[cr] = chr; //pM0[m].rMNclr;
					pO[cr].w = '\"'-' ';
				}

				continue;
			}
		}
		srcFORm( 	pO,
					pALL, cxy, trafo,
					xy, fWH,
					i, n, pM0[m].rMNclr, bSHR, crs );
	}
	return xy;
}

