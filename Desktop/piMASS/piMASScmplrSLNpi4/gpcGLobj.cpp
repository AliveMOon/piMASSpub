#include "gpccrs.h"
#include "gpsGLSL.h"
typedef enum LWO_ID:U4 {
	LWO_ID_FORM	= MAKE_ID('F','O','R','M'),
	LWO_ID_LWO2	= MAKE_ID('L','W','O','2'),

	/**  PRIMARY CHUNK ID  **/
	LWO_ID_LAYR	= MAKE_ID('L','A','Y','R'),
	LWO_ID_PNTS	= MAKE_ID('P','N','T','S'),
	LWO_ID_VMAP	= MAKE_ID('V','M','A','P'),

	LWO_ID_VMAD	= MAKE_ID('V','M','A','D'),

	LWO_ID_POLS	= MAKE_ID('P','O','L','S'),
	LWO_ID_TAGS	= MAKE_ID('T','A','G','S'),
	LWO_ID_PTAG	= MAKE_ID('P','T','A','G'),
	LWO_ID_ENVL	= MAKE_ID('E','N','V','L'),
	LWO_ID_CLIP	= MAKE_ID('C','L','I','P'),
	LWO_ID_SURF	= MAKE_ID('S','U','R','F'),
	LWO_ID_BBOX	= MAKE_ID('B','B','O','X'),
	LWO_ID_DESC	= MAKE_ID('D','E','S','C'),
	LWO_ID_TEXT	= MAKE_ID('T','E','X','T'),
	LWO_ID_ICON	= MAKE_ID('I','C','O','N'),

	/**  POLS TYPE  **/
	LWO_ID_FACE	= MAKE_ID('F','A','C','E'),
	LWO_ID_CRVS	= MAKE_ID('C','U','R','V'),
	LWO_ID_PCHS	= MAKE_ID('P','T','C','H'),
	LWO_ID_MBAL	= MAKE_ID('M','B','A','L'),
	LWO_ID_BONE	= MAKE_ID('B','O','N','E'),

	/**  PTAG TYPE  **/
//	LWO_ID_SURF	= MAKE_ID('S','U','R','F'),
	LWO_ID_BNID	= MAKE_ID('B','N','I','D'),
	LWO_ID_SGMP	= MAKE_ID('S','G','M','P'),
	LWO_ID_PART	= MAKE_ID('P','A','R','T'),

	/**  IMAGE SUB-CHUNK ID  */
	LWO_ID_STIL	= MAKE_ID('S','T','I','L'),
	LWO_ID_ISEQ	= MAKE_ID('I','S','E','Q'),
	LWO_ID_ANIM	= MAKE_ID('A','N','I','M'),
	LWO_ID_XREF	= MAKE_ID('X','R','E','F'),
	LWO_ID_STCC	= MAKE_ID('S','T','C','C'),
	LWO_ID_CONT	= MAKE_ID('C','O','N','T'),
	LWO_ID_BRIT	= MAKE_ID('B','R','I','T'),
	LWO_ID_SATR	= MAKE_ID('S','A','T','R'),
	LWO_ID_HUE	= MAKE_ID('H','U','E',' '),
	LWO_ID_GAMM	= MAKE_ID('G','A','M','M'),
	LWO_ID_NEGA	= MAKE_ID('N','E','G','A'),
	LWO_ID_CROP	= MAKE_ID('C','R','O','P'),
	LWO_ID_ALPH	= MAKE_ID('A','L','P','H'),
	LWO_ID_COMP	= MAKE_ID('C','O','M','P'),
	LWO_ID_IFLT	= MAKE_ID('I','F','L','T'),
	LWO_ID_PFLT	= MAKE_ID('P','F','L','T'),

	/**  ENVELOPE SUB-CHUNK  **/
	LWO_ID_PRE	= MAKE_ID('P','R','E',' '),
	LWO_ID_POST	= MAKE_ID('P','O','S','T'),
	LWO_ID_KEY	= MAKE_ID('K','E','Y',' '),
	LWO_ID_SPAN	= MAKE_ID('S','P','A','N'),
	LWO_ID_CHAN	= MAKE_ID('C','H','A','N'),

	/**  SURFACE SUB-CHUNK ID  */
	LWO_ID_COLR	= MAKE_ID('C','O','L','R'),
	LWO_ID_DIFF	= MAKE_ID('D','I','F','F'),
	LWO_ID_LUMI	= MAKE_ID('L','U','M','I'),
	LWO_ID_SPEC	= MAKE_ID('S','P','E','C'),
	LWO_ID_REFL	= MAKE_ID('R','E','F','L'),
	LWO_ID_TRAN	= MAKE_ID('T','R','A','N'),
	LWO_ID_TRNL	= MAKE_ID('T','R','N','L'),
	LWO_ID_GLOS	= MAKE_ID('G','L','O','S'),
	LWO_ID_SHRP	= MAKE_ID('S','H','R','P'),
	LWO_ID_BUMP	= MAKE_ID('B','U','M','P'),
	LWO_ID_SIDE	= MAKE_ID('S','I','D','E'),
	LWO_ID_SMAN	= MAKE_ID('S','M','A','N'),
	LWO_ID_RFOP	= MAKE_ID('R','F','O','P'),
	LWO_ID_RIMG	= MAKE_ID('R','I','M','G'),
	LWO_ID_RSAN	= MAKE_ID('R','S','A','N'),
	LWO_ID_RIND	= MAKE_ID('R','I','N','D'),
	LWO_ID_CLRH	= MAKE_ID('C','L','R','H'),
	LWO_ID_TROP	= MAKE_ID('T','R','O','P'),
	LWO_ID_TIMG	= MAKE_ID('T','I','M','G'),
	LWO_ID_CLRF	= MAKE_ID('C','L','R','F'),
	LWO_ID_ADTR	= MAKE_ID('A','D','T','R'),
	LWO_ID_GLOW	= MAKE_ID('G','L','O','W'),
	LWO_ID_LINE	= MAKE_ID('L','I','N','E'),
	//LWO_ID_ALPH	= MAKE_ID('A','L','P','H'),
	LWO_ID_AVAL	= MAKE_ID('A','V','A','L'),
	LWO_ID_GVAL	= MAKE_ID('G','V','A','L'),
	LWO_ID_BLOK	= MAKE_ID('B','L','O','K'),
	LWO_ID_LCOL	= MAKE_ID('L','C','O','L'),
	LWO_ID_LSIZ	= MAKE_ID('L','S','I','Z'),
	LWO_ID_CMNT	= MAKE_ID('C','M','N','T'),

	/**  TEXTURE LAYER  **/
	//LWO_ID_CHAN	= MAKE_ID('C','H','A','N'),
	LWO_ID_TYPE	= MAKE_ID('T','Y','P','E'),
	LWO_ID_NAME	= MAKE_ID('N','A','M','E'),
	LWO_ID_ENAB	= MAKE_ID('E','N','A','B'),
	LWO_ID_OPAC	= MAKE_ID('O','P','A','C'),
	LWO_ID_FLAG	= MAKE_ID('F','L','A','G'),
	LWO_ID_PROJ	= MAKE_ID('P','R','O','J'),
	LWO_ID_STCK	= MAKE_ID('S','T','C','K'),
	LWO_ID_TAMP	= MAKE_ID('T','A','M','P'),

	/**  TEXTURE MAPPING  **/
	LWO_ID_TMAP	= MAKE_ID('T','M','A','P'),
	LWO_ID_AXIS	= MAKE_ID('A','X','I','S'),
	LWO_ID_CNTR	= MAKE_ID('C','N','T','R'),
	LWO_ID_SIZE	= MAKE_ID('S','I','Z','E'),
	LWO_ID_ROTA	= MAKE_ID('R','O','T','A'),
	LWO_ID_OREF	= MAKE_ID('O','R','E','F'),
	LWO_ID_FALL	= MAKE_ID('F','A','L','L'),
	LWO_ID_CSYS	= MAKE_ID('C','S','Y','S'),

	/**  IMAGE MAP  **/
	LWO_ID_IMAP	= MAKE_ID('I','M','A','P'),
	LWO_ID_IMAG	= MAKE_ID('I','M','A','G'),
	LWO_ID_WRAP	= MAKE_ID('W','R','A','P'),
	LWO_ID_WRPW	= MAKE_ID('W','R','P','W'),
	LWO_ID_WRPH	= MAKE_ID('W','R','P','H'),
//	LWO_ID_VMAP	= MAKE_ID('V','M','A','P'),
	LWO_ID_AAST	= MAKE_ID('A','A','S','T'),
	LWO_ID_PIXB	= MAKE_ID('P','I','X','B'),

	/**  PROCUDUAL TEXTURE  **/
	LWO_ID_PROC	= MAKE_ID('P','R','O','C'),
//	LWO_ID_COLR	= MAKE_ID('C','O','L','R'),
	LWO_ID_VALU	= MAKE_ID('V','A','L','U'),
	LWO_ID_FUNC	= MAKE_ID('F','U','N','C'),
	LWO_ID_FTPS	= MAKE_ID('F','T','P','S'),
	LWO_ID_ITPS	= MAKE_ID('I','T','P','S'),
	LWO_ID_ETPS	= MAKE_ID('E','T','P','S'),

	/**  GRADIENT **/
	LWO_ID_GRAD	= MAKE_ID('G','R','A','D'),
	LWO_ID_GRST	= MAKE_ID('G','R','S','T'),
	LWO_ID_GREN	= MAKE_ID('G','R','E','N'),

	/**  SHADER PLUGIN  */
	LWO_ID_SHDR	= MAKE_ID('S','H','D','R'),
	LWO_ID_DATA	= MAKE_ID('D','A','T','A'),


	/**  VMAP TYPE  **/
	LWO_ID_MNVW	= MAKE_ID('M','N','V','W'),
	LWO_ID_MORF	= MAKE_ID('M','O','R','F'),
	LWO_ID_TXUV	= MAKE_ID('T','X','U','V'),
	LWO_ID_WGHT	= MAKE_ID('W','G','H','T'),
	/**  saját id-k **/
	LWO_ID_LIGHT	= MAKE_ID('l','i','g','h'),
	LWO_ID_COLOR	= MAKE_ID('c','o','l','o'),
};



gpc3Dlst::gpc3Dlst(){
	gpmCLR;
	U8 nLEN;
	char* pS = (char*)gp_man_lws, *pSe;
	while( *pS ) {
		pSe = pS+gpmVAN( pS, "\r\n", nLEN );
		bnDCT.dctADD( pS, pSe-pS );
		pS = pSe+gpmNINCS(pSe, "\r\n" );
	}
	pS = (char*)gp_s_lws;
	while( *pS ) {
		pSe = pS+gpmVAN( pS, "\r\n", nLEN );
		lwsDCT.dctADD( pS, pSe-pS );
		pS = pSe+gpmNINCS(pSe, "\r\n" );
	}
}
class gpc3Dblnd {
public:
	U1x4 ix; U4x2 ps;
	F4	 wg;
	gpc3Dblnd(){};
	gpc3Dblnd* BLset( U1 x, float w ) {
		if( this ? w == 0.0 : true )
			return this;
		U1 u; float f;
		for( U4 i = 0; i < 4; i++ ) {
			if( wg.aXYZW[i] >= w )
				continue;
			u = ix.aXYZW[i];
			f = wg.aXYZW[i];
			wg.aXYZW[i] = w;
			ix.aXYZW[i] = x;
			if( f == 0.0 )
				break;
			w = f;
			x = u;
		}

		return this;
	}
	gpc3Dblnd* PRTset( U1 p ) {
		if( this ? !p : true )
			return this;
		ps.aU1x4[0].u4 <<= 8;
		ps.aU1x4[0].x = p;
		return this;
	}
	gpc3Dblnd* SRFset( U1 s ) {
		if( this ? !s : true )
			return this;
		ps.aU1x4[1].u4 <<= 8;
		ps.aU1x4[1].x = s;
		return this;
	}
};
class gpc3Dvx {
public:
	F4 xyzi, up;		//  0 16
	F2 uv; U4x2 ps;		// 32 40
						// 48
	gpc3Dvx(){}
	bool bNG( gpc3Dvx& b, float x = 0.0, float u = 0.0 ) {
		if(!this)
			return false;

		if( (uv-b.uv).qlen() >= u*u )
			return true;
		if( (xyzi-b.xyzi).qlen_xyz() >= x*x )
			return true;

		return false;
	}
	bool bGD( gpc3Dvx& b, float x = 0.0, float u = 0.0 ) { return !bNG(b,x,u); }
	char* pSTR( gpcLZY buf, const char* p_lwo, U4x2* pTG ) {
		if( !this )
			return buf.pCHAR();
		U8 s = -1;
		buf.lzyFRMT( s,
			" x:%4.3f,%4.3f,%4.3f"
			" i:%d %d %d %d"
			" n:%0.2f,%0.2f,%0.2f"
			" u:%0.3f,%0.3f"
			" p.x:%s:%s",
			xyzi.x, xyzi.y, xyzi.z,
			xyzi.aU1x4[3].x, xyzi.aU1x4[3].y, xyzi.aU1x4[3].z, xyzi.aU1x4[3].w,
			up.x, up.y, up.z,
			uv.x, uv.y,
			p_lwo+pTG[ps.aU1x4[0].x].i, p_lwo+pTG[ps.aU1x4[1].x].i
		);
		return buf.pCHAR();
	}
};
class gpc3Dfc {
public:
	U4x2	*pFC;
	U4		*pIX, iFC;
	gpcLZY	LST, IX, SUM;

	gpc3Dfc(){}
	gpc3Dfc* pFACEswp( void* pV, U4 n ) {
		if(!this)
			return NULL;
		U1* pUi = (U1*)pV, *pUe = pUi+n;
		U4 *pFs;
		while( pUi < pUe ) {
			pFC = LST.pU4x2n( iFC = LST.nLD(sizeof(*pFC)) );
			pFC->i = IX.nLD(sizeof(U4));
			pFC->n = swp2(pUi)&0x03ff; pUi+=2;

			pFs = (U4*)SUM.Ux( pFC->n, sizeof(*pFs) );
			(*pFs)++;

			pIX = IX.pU4n(pFC->sum()) - pFC->n;
			for( U4 i = 0; i < pFC->n; i++ ) {
				if( *pUi == 0xff ) {
					pIX[i] = swp4( pUi )&0xFFffff; pUi+= 4;
				} else {
					pIX[i] = swp2( pUi ); pUi+= 2;
				}
			}
		}
		return this;
	}
	void COUT(){
		if(!this)
			return;
		U4 *pFs = (U4*)SUM.Ux( 0, sizeof(*pFs) );
		for( U4 i = 0, e = SUM.nLD(sizeof(*pFs)); i < e; i++ ) {
			if( !pFs[i] )
				continue;
			if(bSTDcout){ gpdCOUT << i << " " << pFs[i] << gpdENDL; }
		}
	}
};
class gpc3Dmap {
public:
	gpcLZY tri, pnt, fcl, fci, fps, fpsSRT;
	U4 id;
	U4x4 iVXn;

	gpc3Dmap(	U4 ix, gpcLZY& p,
				gpc3Dfc& fc,
				gpcLZY& b,
				gpcLZY& t,
				U4x2 *pIX2,
				gpcLZY& ps,
				void* p_u, void* p_nx
			) {
		fps = ps;
		U1 *pUi = (U1*)p_u, *pUnx = (U1*)p_nx;
		id = ix;
		fcl = fc.LST;
		fci = fc.IX;

		F4	*pPs = p.pF4(), v0, v1,v2;
		F2	*pTs = t.pF2()+pIX2->x;
		U4x2	*pFl = (U4x2*)fc.LST.p_alloc;

		U4	nP = p.nLD(sizeof(*pPs)),
			nF = fc.LST.nLD(sizeof(*pFl)),
			*pFi = fc.IX.pU4(), ip, uf;

		gpc3Dblnd* pB = (gpc3Dblnd*)b.p_alloc;

		gpc3Dvx	tmP, *pVX = ((gpc3Dvx*)pnt.Ux( nP, sizeof(tmP) ))-nP;
		for( U4 i = 0; i < nP; i++ ) {
			pVX[i].xyzi = pPs[i];
			pVX[i].xyzi.uXYZW[3] = pB[i].ix.u4;
			pVX[i].uv = pTs[i];
			pVX[i].ps = pB[i].ps;
		}


		for( U4 f = 0, i, ip, x,n; f < nF; f++ )
		for( x = pFl[f].x, n = pFl[f].y, i = 0; i < n; i++ ) {
			ip = pFi[i+x];
			v0 = pVX[ip].xyzi;
			v1 = (pVX[pFi[((i-1+n)%n)+x]].xyzi - v0).N3();
			v2 = (pVX[pFi[((i+1  )%n)+x]].xyzi - v0).N3();
			pVX[ip].up += v2.X3(v1).N3().xyz1();
		}

		for( U4 p = 0; p < nP; p++ )
			pVX[p].up = pVX[p].up.N3();

		while( pUi < pUnx ) {
			if( *pUi == 0xff ) {
				ip = swp4( pUi )&0xFFffff; pUi+=4;
			} else {
				ip = swp2( pUi ); pUi+=2;
			}
			if( *pUi == 0xff ) {
				uf = swp4( pUi )&0xFFffff; pUi+=4;
			} else {
				uf = swp2( pUi ); pUi+=2;
			}

			pFl = fcl.pU4x2( uf );
			pFi = fci.pU4( pFl->sum()+pIX2->x ) - pFl->y;

			for( U4 i = 0, ie = pFl->y, t, te; i < ie; i++ ) {
				if( pFi[i] != ip )
					continue;
				/// VMAD
				pVX = (gpc3Dvx*)pnt.Ux( ip, sizeof(tmP) );
				tmP = *pVX;
				tmP.uv.swpXYflpY( pUi ); pUi += sizeof(F2);


				for( t = p.nLD(sizeof(tmP)), te = pnt.nLD(sizeof(tmP)); t < te; t++ ) {
					if( ((gpc3Dvx*)pnt.Ux( pFi[i], sizeof(*pVX) ))->bGD(tmP, 0.0/4096.0, 0.0/1024.0 )  )
						break;
				}

				if( pFi[i] == t )
					break;

				pFi[i] = t;
				if( t < te )
					break;

				*(gpc3Dvx*)pnt.Ux( te, sizeof(*pVX) ) = tmP;
				break;
			}
		}


	}
	~gpc3Dmap() {
		U4 nTR = nTRI();
		gpc3Dtri* pTR = (gpc3Dtri*)tri.Ux( nTR, sizeof(*pTR) );
		for( U4 t = 0; t < nTR; t++ )
			pTR[t].null();
	}

	U4 nTRI() { return tri.nLD(sizeof(gpc3Dtri)); }
	gpc3Dtri* pTRI0( gpcLZY& buf, const void* pLWO = NULL, U4x2* pTG = NULL ) {
		if( nTRI() )
			return (gpc3Dtri*)tri.Ux( 0, sizeof(gpc3Dtri) );
		if( !pLWO )
			pTG = NULL;

		U4x2
				*pPRT = fps.pU4x2(),
				*pFl0 = fcl.pU4x2(),
				*pTMP = buf.pU4x2n(0,fps.nLD(sizeof(*pPRT))+1), //((U4x2*)buf.Ux( fps.nLD() + 1, 1 ))-(fps.nLD()+1),
				*pFli;

		U4	nFC = fps.nLD(sizeof(*pTMP)),
			*pFi0 = fci.pU4(),
			*pFii, *pU4;


		pPRT->median(nFC,pTMP,true);
		gpc3Dtri* pTR = NULL;
		U4x4* pSRF = NULL;


		for( U4 i = 0, p = -1, s = -1,nf; i < nFC; i++ ) {
			if( p != pPRT[i].prt ) {
				if( pTR ) {
					pSRF = pTR->pSRFadd(-1);
					if( pTG )
					if( bSTDcout_3D )
						gpdCOUT << " SRF:"	<< " " << ((char*)pLWO) + pTG[pSRF[-1].x].x
												<< " 1n:" << (pSRF[0].y-pSRF[-1].y)
												<< " 2n:" << (pSRF[0].z-pSRF[-1].z)/2
												<< " 3n:" << (pSRF[0].w-pSRF[-1].w)/3 << gpdENDL;
				}
				pTR = (gpc3Dtri*)tri.Ux( tri.nLD(sizeof(*pTR)), sizeof(*pTR) );
				pTR->prt = p = pPRT[i].prt;
				if( pTG )
				//if( bSTDcout_3D )
					gpdCOUT << "PART:" <<((char*)pLWO) + pTG[p].x << gpdENDL;
				s = -1;
				pSRF = NULL;
			}
			if( s != pPRT[i].srf ){
				s = pPRT[i].srf;
				if( pSRF ){
					pSRF = pTR->pSRFadd(s);
					if( pTG ) if( bSTDcout_3D )  gpdCOUT << " SRF:"	<< " " << ((char*)pLWO) + pTG[pSRF[-1].x].x
												<< " 1n:" << (pSRF[0].y-pSRF[-1].y)
												<< " 2n:" << (pSRF[0].z-pSRF[-1].z)/2
												<< " 3n:" << (pSRF[0].w-pSRF[-1].w)/3 << gpdENDL;
				} else
					pSRF = pTR->pSRFadd(s);
				if( pTG ) if( bSTDcout_3D )  gpdCOUT << " SRF:"	<< " " << ((char*)pLWO) + pTG[pSRF->x].x
												<< " 1p:" << pSRF->y
												<< " 2p:" << pSRF->z/2
												<< " 3p:" << pSRF->w/3 << gpdFLUSH;
			}

			pFli = pFl0+pPRT[i].i;
			pFii = pFi0 + pFli->x;
			switch( nf = pFli->y ) {
				case 0:
					break;
				case 1:
					pU4 = (U4*)pTR->p.pU4n( -1 ); //pTR->p.nLD(sizeof(*pU4)) );
					gpmMcpyOF( pU4, pFii, 1 );
					break;
				case 2:
					pU4 = (U4*)pTR->l.pU4n( -1, 2 ); //pTR->l.nLD(sizeof(*pU4)), 2 );
					gpmMcpyOF( pU4, pFii, 2 );
					break;
				case 3:
					pU4 = (U4*)pTR->t.pU4n( -1, 3 ); //pTR->t.nLD(sizeof(*pU4)), 3 );
					gpmMcpyOF( pU4, pFii, 3 );
					break;
				default:
					for( U4 i0 = *pFii, j = 1, je = nf-1; j < je; j++ ) {
						pU4 = (U4*)pTR->t.pU4n( -1, 3 ); //pTR->t.nLD(sizeof(*pU4)), 3 );
						pU4[0] = i0;
						pU4[1] = pFii[j];
						pU4[2] = pFii[j+1];
					}
					break;
			}
		}
		if( !pSRF )
			return (gpc3Dtri*)tri.Ux( 0, sizeof(gpc3Dtri) );

		pSRF = pTR->pSRFadd(-1);
		if( pTG )
		if( bSTDcout_3D )
			gpdCOUT << " SRF:"	<< " " << ((char*)pLWO) + pTG[pSRF[-1].x].x
									<< " 1n:" << (pSRF[0].y-pSRF[-1].y)
									<< " 2n:" << (pSRF[0].z-pSRF[-1].z)/2
									<< " 3n:" << (pSRF[0].w-pSRF[-1].w)/3 << gpdENDL;
		return (gpc3Dtri*)tri.Ux( 0, sizeof(gpc3Dtri) );
	}
};

class gpc3Dly {
public:
	char	sNAME[0x100];
	I4x4	id;
	gpcLZY	pnt, blnd, tx, MAP,
			srSUM, prSUM,
			bon, buf;
	gpc3Dfc fc;

	gpcLZYdct	mapDCTwg; gpcLZY mapIXwg, mapBLwg;
	gpcLZYdct	mapDCTtx; gpcLZY mapIXtx, mapF2tx;
	gpcLZYdct	mapDCTmr; gpcLZY mapIXmr, mapBLmr;
	gpcLZYdct	mapDCTsr; gpcLZY mapIXsr;
	gpcLZYdct	mapDCTpt; gpcLZY mapIXprt;
	gpcLZYdct	mapDCTcl;

	F4 piv, bbox[2];
	gpc3Dly(){
		gpmCLR;
	}
	gpc3Dly* pLYadd( gpcLZY& ly3D, U4* pU4i ) {
		if( !this ){
			gpc3Dly* pTHIS = (gpc3Dly*)ly3D.Ux( ly3D.nLD(sizeof(gpc3Dly)), sizeof(gpc3Dly) );
			if( !pTHIS )
				return NULL;

			return pTHIS->pLYadd( ly3D, pU4i );
		}

		id.w = swp4( pU4i ); pU4i++;
		id.x = id.w&0xffff;		// momID
		id.y = id.w>>16;		// layID


		piv.swpXYZ0( pU4i ); pU4i+=3;
		gpmSTRCPY( sNAME, pU4i );
		return this;
	}
	F4* pPNTSswp( const void* pV, U4 n ) {
		F4* pP0 = ((F4*)pnt.Ux( n, sizeof(*pP0) ))-n;
		if( !pV )
			return pP0;
		//float *pF = (float*)pV;
		for( U4 i = 0; i < n; i++ )
			pP0[i].swpXYZ0( ((U4*)pV) + i*3 );
		return pP0;
	}
	F4* pBBOXswp( const void* pV ) {
		U4 *pU = (U4*)pV;
		bbox[0].swpXYZ0( pU ); pU+=3;
		bbox[1].swpXYZ0( pU );
	}
	I4 pWGHTswp( const void* pV, U4 n, gpcLZYdct& bnDCT  ) {
		U4	nUi = gpmSTRLEN(pV), ip, uf,
			iBn, iBx = bnDCT.dctFND( pV, nUi, iBn );
		if( iBx >= iBn )
			return -1; /// ha nem csont egyenlőre nem kell

		I4 ix = mapDCTwg.nIX();
		mapDCTwg.dctADD( pV, nUi );

		if(ix >= mapDCTwg.nIX())
			return -1;

		char* pUi = ((char*)pV) + gpmPAD( nUi+1,2 ), *pUnx = ((char*)pV)+n;
		gpc3Dblnd* pBl;
		U4x2 *pIX2 = (U4x2*)mapIXwg.Ux( ix, sizeof(*pIX2) );
		pIX2->i = iBx; pIX2->n = 0;
		while( pUi < pUnx ) {
			if( *pUi&0x80 ) {
				ip = swp4( pUi )&0x7FFFffff; pUi+= 4;
			} else {
				ip = swp2( pUi ); pUi+= 2;
			}
			pBl = (gpc3Dblnd*)mapBLwg.Ux( ip, sizeof(*pBl) );
			uf = swp4( pUi );
			if(uf)
				pIX2->n++;
			pBl->BLset( iBx, *(float*)&uf ); pUi+= 4;
		}

		return ix;
	}
	I4 pTXUVswp( const void* pV, U4 n ) {
		F2* pTX;
		/// megreressük melyik csont a listában
		I4 ix = mapDCTtx.nIX(), nUi = gpmSTRLEN(pV), ip;
		mapDCTtx.dctADD( pV, nUi );
		//pUi += gpmPAD( nUi+1,2 );
		if( ix >= mapDCTtx.nIX() )
			return -1;

		char* pUi = ((char*)pV) + gpmPAD( nUi+1,2 ), *pUnx = ((char*)pV)+n;
		U4x2 *pIX2 = (U4x2*)mapIXtx.Ux( ix, sizeof(*pIX2) );
		pIX2->x = mapF2tx.nLD(sizeof(*pTX));
		while( pUi < pUnx ) {
			if( *pUi&0x80 ) {
				ip = swp4( pUi )&0x7FFFffff; pUi+= 4;
			} else {
				ip = swp2( pUi ); pUi+= 2;
			}
			pTX = (F2*)mapF2tx.Ux( ip, sizeof(*pTX) );
			pTX->swpXYflpY( pUi ); pUi += sizeof(*pTX);
		}
		pIX2->y = mapF2tx.nLD(sizeof(*pTX))-pIX2->x;
		return ix;
	}
	gpc3Dmap* pMAPswp( const void* pV, U4 n ){
		U4	nx, nUi = gpmSTRLEN(pV),

		ix = mapDCTtx.dctFND( pV, nUi, nx );
		if( ix >= nx )
			return NULL;
		char* pUi = ((char*)pV) + gpmPAD( nUi+1,2 ), *pUnx = ((char*)pV)+n;
		U4x2 *pIX2 = (U4x2*)mapIXtx.Ux( ix, sizeof(*pIX2) );
		gpc3Dmap** ppM = (gpc3Dmap**)MAP.Ux( ix, sizeof(*ppM) );
		gpmDEL( *ppM );
		return (*ppM) = new gpc3Dmap(	ix, pnt, fc,
										mapBLwg,
										mapF2tx, pIX2,
										mapIXprt,
										pUi, pUnx
									);
	}

	gpc3Dly* SRFswp( const void* pV, U4 n ) {
		char* pUi = ((char*)pV), *pUnx = ((char*)pV)+n;
		gpc3Dblnd* pBl;
		U4x2	*pSRF, srf, *pFl,
				*pPRT;
		U4 *pFi, nF = 0;
		/// PTAG { type[ID4], ( poly[VX], tag[U2] )* }
		while( pUi < pUnx ) {

			/// poly[VX]
			if( *pUi == 0xff ) {
				srf.i = swp4( pUi )&0xFFffff; pUi+= 4;
			} else {
				srf.i = swp2( pUi ); pUi+= 2;
			}
			/// tag[U2]
			srf.srf = swp2( pUi ); pUi += 2;

			(*(U4*)srSUM.Ux( srf.srf, sizeof(U4) ))++;
			pPRT = (U4x2*)mapIXprt.Ux( srf.i, sizeof(*pPRT) );
			pPRT->srf = srf.srf;
			// FACE
			pFl = (U4x2*)fc.LST.Ux( srf.i, sizeof(*pFl) );
			pFi = (U4*)fc.IX.Ux( pFl->sum(), sizeof(*pFi) ) - pFl->n;

			for( U4 i = 0; i < pFl->n; i++ ) {
				pBl = (gpc3Dblnd*)mapBLwg.Ux( pFi[i], sizeof(*pBl) );
				pBl->SRFset( srf.srf );
			}
			nF++;
		}
		return this;
	}
	gpc3Dly* SRFcout( gpcLZY& lzySRF, const void* pLWO, U4x2* pTG, U4 nGD = 1 ) {
		U1* pU0 = (U1*)pLWO;
		for( U4 i = 0, e = srSUM.nLD(sizeof(U4)), n; i < e; i++ ){
			n = *(U4*)(srSUM.Ux(i,sizeof(U4)));
			if( n < nGD )
				continue;
			gpc3Dsrf* pSRF = (gpc3Dsrf*)lzySRF.Ux( i, sizeof(*pSRF) );
			if( !pSRF->sNAME[0] )
				gpmSTRCPY( pSRF->sNAME, pU0+pTG[i].x );
			if( bSTDcout_3D )  gpdCOUT << i << " " << n << " " << pSRF->sNAME <<  gpdENDL;
		}
		return this;
	}

	gpc3Dly* PRTswp( const void* pV, U4 n ) {
		char* pUi = ((char*)pV), *pUnx = ((char*)pV)+n;
		gpc3Dblnd* pBl;
		U4x2* pF, *pPRT, prt, *pFl;
		U4 *pFi, nF = mapIXprt.nLD(sizeof(*pPRT)), ix;
		/// PTAG { type[ID4], ( poly[VX], tag[U2] )* }
		while( pUi < pUnx ) {

			/// poly[VX]
			if( *pUi == 0xff ) {
				prt.i = swp4( pUi )&0xFFffff; pUi+= 4;
			} else {
				prt.i = swp2( pUi ); pUi+= 2;
			}
			/// tag[U2]
			prt.prt = swp2( pUi ); pUi += 2;

			(*(U4*)prSUM.Ux( prt.prt, sizeof(U4) ))++;

			pPRT = (U4x2*)mapIXprt.Ux( prt.i, sizeof(*pPRT) );
			pPRT->i = prt.i;
			pPRT->prt = prt.prt;

			pFl = (U4x2*)fc.LST.Ux( prt.i, sizeof(*pFl) );
			pFi = (U4*)fc.IX.Ux( pFl->sum(), sizeof(*pFi) ) - pFl->n;

			for( U4 i = 0; i < pFl->n; i++ ) {
				pBl = (gpc3Dblnd*)mapBLwg.Ux( pFi[i], sizeof(*pBl) );
				pBl->PRTset( prt.prt );
			}

			nF++;
		}
		return this;
	}
	gpc3Dly* PRTcout( const void* pLWO, U4x2* pTG, U4 nGD = 1 ) {
		U1* pU0 = (U1*)pLWO;
		for( U4 i = 0, e = prSUM.nLD(sizeof(U4)), n; i < e; i++ ){
			n = *(U4*)(prSUM.Ux(i,sizeof(U4)));
			if( n < nGD )
				continue;
			if( bSTDcout_3D )  gpdCOUT << i << " " << n << " " << (char*)(pU0+pTG[i].x) <<  gpdENDL;
		}
		return this;
	}
	U4 nMAP( const void* pLWO = NULL, U4x2* pTG = NULL ) {
		if(!this)
			return 0;
		U4 nM = MAP.nLD(sizeof(gpc3Dmap*));
		if( !nM )
			return 0;

		gpc3Dmap	**ppM = (gpc3Dmap**)MAP.ppVOID(), //MAP.Ux( 0, sizeof(*ppM) ),
					*pM = NULL;
		for( U4 m = 0; m < nM; m++ ){
			pM = ppM[m];
			if( !pM )
				continue;
			pM->pTRI0( buf, pLWO, pTG );
		}

		return nM;
	}
	gpc3Dmap* pMAP( U4 ix ) {
		if(!this)
			return NULL;
		U4 nM = MAP.nLD(sizeof(gpc3Dmap*));
		if( nM < ix )
			return NULL;

		gpc3Dmap **ppM = (gpc3Dmap**)MAP.ppVOID();
		return ppM ? ppM[ix] : NULL;
	}
};
gpc3Ditm* gpc3Dgym::p3DitmADD( U4 i, U4 id ) {
	if( i > n3Ditm() )
		i = n3Ditm();
	gpc3Ditm* pITM = (gpc3Ditm*)itmLST.Ux( i, sizeof(*pITM) );

	pITM->momIX = pITM->itmIX = i;
	pITM->momID = pITM->itmID = id;
	return pITM;
}
gpc3Ditm* gpc3Dgym::p3Dii( U4 i ) {
	if( i > n3Ditm() )
		return NULL;

	return (gpc3Ditm*)itmLST.Ux( i, sizeof(gpc3Ditm) );
}
float gpc3Dcnl::valKEYs( U1 c, float s ){
	U4	aiK[2] = { 0, nKEY(c) },
		nK = aiK[1],
		iK;

	if( !nK )
		return 0.0f;
	gpc3Dkey* pK0 = (gpc3Dkey*)aKEY[c].p_alloc;
	if( pK0->sec >= s )
		return pK0->val;
	else if( pK0[nK-1].sec <= s )
		return pK0[nK-1].val;

	while( nK ) {
		iK = (aiK[1]+aiK[0])/2;
		if( pK0[iK].sec == s )
			return pK0[iK].val;

		if( pK0[iK].sec > s ) {
			// ez nagy
			if(iK<=aiK[0]) return pK0->val;	// kisebb nincs

			iK--;
			if(pK0[iK].sec == s )
				return pK0[iK].val;
			if(pK0[iK].sec <= s )
				return	pK0[iK]*s; 	// ez az <=

			// nagyobb
			aiK[1] = iK;
			nK = aiK[1]-aiK[0];
			continue;
		}

		// ez kicsi
		if( iK+1 >= aiK[1] )
			return pK0[iK]*s;	// köv nincs

		iK++;
		if( pK0[iK].sec == s )	// köv az
				return pK0[iK].val;
		else if( pK0[iK].sec > s )
				return pK0[iK-1]*s; // ez kicsi köv nagyobb

		aiK[0] = iK;
		nK = aiK[1]-aiK[0];
	}
	return 0.0f;
}
gpc3Dkey* gpc3Dcnl::pKEYinsIX( U1 c, I4 i, I4 nE, float s ){
	gpc3Dkey* pK0 = (gpc3Dkey*)aKEY[c].Ux(nE-1,sizeof(*pK0));
	if( !pK0 )
		return NULL;
	pK0 -= nE-1;
	pK0[i].sec = s;
	return pK0+i;
}
gpc3Dkey* gpc3Dcnl::pKEYins( U1 c, float s ){
	if( !nKEY(c) ){
		if( this ? c>8 : true )
			return NULL;
		gpc3Dkey* pK0 = (gpc3Dkey*)aKEY[c].Ux(0,sizeof(*pK0));
		pK0->sec = s;
		return pK0;
	}

	gpc3Dkey* pK0 = (gpc3Dkey*)aKEY[c].p_alloc;
	U4	aiK[2] = { 0, nKEY(c) },
		nK = aiK[1],
		iK;

	if( pK0[nK-1].sec == s )
		return pK0+nK-1;
	else if( pK0[nK-1].sec < s ) {	// köv nincs
		pK0 = (gpc3Dkey*)aKEY[c].Ux(nK,sizeof(*pK0));
		pK0->sec = s;
		return pK0;
	}

	while( nK ) {
		iK = (aiK[1]+aiK[0])/2;
		if( pK0[iK].sec == s )
			return pK0+iK;

		if( pK0[iK].sec > s ) {
			// ez nagy
			if(iK<=aiK[0]) {	// kisebb nincs
				iK=aiK[0];
				U8 strt = iK*sizeof(*pK0);
				aKEY[c].lzyINS( NULL,sizeof(*pK0), strt, 0 );
				pK0 = (gpc3Dkey*)aKEY[c].Ux(iK,sizeof(*pK0));
				pK0->sec = s;
				return pK0;
			}
			iK--;
			if(pK0[iK].sec <= s )
				return pK0+iK; 	// ez az <=

			// nagyobb
			aiK[1] = iK;
			nK = aiK[1]-aiK[0];
			continue;
		}

		// ez kicsi
		if( iK+1 >= aiK[1] ) {	// köv nincs
			pK0 = (gpc3Dkey*)aKEY[c].Ux(aiK[1],sizeof(*pK0));
			pK0->sec = s;
			return pK0;
		}
		iK++;
		if( pK0[iK].sec == s )	// köv az
				return pK0+iK;
		else if( pK0[iK].sec > s ) {
			U8 strt = iK*sizeof(*pK0);
			aKEY[c].lzyINS( NULL,sizeof(*pK0), strt, 0 );
			pK0 = (gpc3Dkey*)aKEY[c].Ux(iK,sizeof(*pK0));
			pK0->sec = s;
			return pK0;
		}

		aiK[0] = iK;
		nK = aiK[1]-aiK[0];
	}
	return NULL;
}
gpc3Dkey& gpc3Dkey::operator = ( const char* pS ) {
	U8 nL;
	char* pE = (char*)pS + gpmVAN(pS,"\r\n}", nL );
	int i = 0;
	while( pS < pE ) {
		pS += gpmNINCS(pS, " \t" );
		switch( i ) {
			case 0:
				val = gpmSTR2D( pS );
				break;
			case 1:
				sec = gpmSTR2D( pS );
				break;
			case 2:
				spn = gpmSTR2D( pS ); break;
			default:
				aP[(i-3)%6] = gpmSTR2D( pS ); break;
		}
		i++;
	}
	return *this;
}
size_t sLWOfloat( char* pBUFF, float f ) {
	char* pBi = pBUFF;
	int d = f, dd = 0;
	f -= d;
	pBi += sprintf( pBi, " %d", d );

	f *= 10.0;
	d = f;
	if( !d )
		return pBi-pBUFF;
	*pBi = '.';
	pBi++;
	f -= d;
	f *= 10.0;
	while( f != 0 ) {
		d = f;
		f -= d;
		f *= 10.0;

		dd*=10;
		dd+=d;
	}

	pBi += sprintf( pBi, "%d", dd );

	return pBi-pBUFF;
}
size_t gpc3Dkey::sOUT( char* pBUFF, const char* pEND ) {
	if( !this )
		return 0;
	//char* pBi = pBUFF;
	return sprintf( pBUFF, " %f %f %d %f %f %f %f %f %f%s", val, sec, spn, aP[0], aP[1], aP[2], aP[3], aP[4], aP[5] ,pEND );
}
#define gpdITMis (pBON 	?   pBON 				\
						: ( pITM	? pITM 		\
						: ( pNULL	? pNULL 	\
						: ( pLIG	? pLIG 		\
						: ( pCAM	? pCAM 		\
						:   NULL 				\
							)))))
gpc3Dgym::gpc3Dgym( I4 i, const char* pP, gpeALF alf ) {
	gpmCLR;
	U8 nLEN;
	id.x = i;
	id.a8x2[0].b = alf;
	id.z = pP ? gpmVAN(pP," \t\r\n\"\a",nLEN) : 0;
	gpmSTRCPY( sPATH, pP );
	pFILE = strrchr(sPATH,'/' );
	if( pFILE )
		pFILE++;
	else
		pFILE = sPATH+gpmSTRLEN(sPATH);
	gpmSTRCPY( sPUB, sPATH );
	pPUB = sPUB+(pFILE-sPATH);
}

gpc3Dgym* gpc3Dgym::pLWS( gpcLZY& lws, gpcLZYdct& lwsDCT, gpcLZYdct& bnDCT ) {
	if( !this )
		return NULL;
	if( bSTDcout_3D )  gpdCOUT << "gpc3Dgym::pLWS " << sPATH << gpdENDL;
	U8 s = lws.nLD();
	if( !s )
		return this;
	U4x4* pU4x4;
	char	*pS = (char*)lws.p_alloc, *pSe = pS+s, *pA, *pCH, *pDC,
			sDBG[0x100];
	GPE_LWS_COM si;
	U4 nD, iD, nS, nL, iCNL = 0, nENV = 0, iENV, nA;
	olcb = I4x4(-1,-1,-1,-1);	/// obj liht cam bone
	gpc3Dkey* pKEY;
	gpc3Ditm	*pITM = NULL, *pNULL = NULL,	// 0x10000000
				*pLIG = NULL,					// 0x20000000
				*pCAM = NULL,					// 0x30000000
				*pBON = NULL,					// 0x40000000
				*pMOM = NULL, *pIS = NULL,
				*pROOT = NULL, *pI0 = NULL;
	I8 momID;
	for( pS += gpmNINCS( pS, " \t\r\n" ); pS < pSe; pS += gpmNINCS( pS, " \t\r\n" ) )
	{
		si = (GPE_LWS_COM)lwsDCT.dctFND( pDC=pS, nS = pSe-pS, nD );
		pU4x4 = lwsDCT.pIXi((U4)si);
		if( !pU4x4 ) {
			pS += gpmVAN( pS, "\r\n", s );
			continue;
		}

		pA = pS + pU4x4->y;
		nA = gpmNINCS( pA, " \t\r\n" );
		pS = pA + gpmVAN( pA, "\r\n", s );
		if( !nA )
			continue;

		switch( si ) {
			case GPE_LWS_COM_LoadObjectLayer:{
				pNULL = pBON = NULL;
				olcb.x++;
				olcb.w=-1;
				pMOM = (pITM = p3DitmADD( -1, gpeLWSiTYP_OBJ|olcb.x ));
				pITM->layID = gpfSTR2I8(pA, &pA);
				pITM->pNAME(pA);
			} break;
			case GPE_LWS_COM_AddBone:{
				olcb.w++;
				pMOM = pBON = p3DitmADD( -1, gpeLWSiTYP_BON| (olcb.w<<16) | (pITM->itmID&0xffff) );
			} break;
			case GPE_LWS_COM_AddNullObject:{
				pBON = pITM = NULL;
				olcb.x++;
				olcb.w=-1;
				pMOM = pNULL = p3DitmADD( -1, gpeLWSiTYP_OBJ|olcb.x );
				pNULL->pNAME(pA);
				pNULL->bNULL = true;
			} break;
			case GPE_LWS_COM_AddLight:{
				pBON = pITM = pNULL = NULL;
				olcb.y++;
				olcb.w=-1;
				pMOM = pLIG = p3DitmADD( -1, gpeLWSiTYP_LIG|olcb.y );

			} break;
			case GPE_LWS_COM_AddCamera:{
				pBON = pITM = pNULL = pLIG = NULL;
				olcb.z++;
				olcb.w=-1;
				pMOM = pCAM = p3DitmADD( -1, gpeLWSiTYP_CAM|olcb.y );

			} break;
			case GPE_LWS_COM_NumChannels:{} break;
			case GPE_LWS_COM_Channel:{
				iCNL = gpfSTR2I8(pA, &pA);
			} break;
			case GPE_LWS_COM_C_open:{
				pS = pA;
			} break;
			case GPE_LWS_COM_Envelope:{
				pA += gpmNINCS(pA," \t\"\r\n");
				nENV = gpfSTR2I8(pA, &pA);
				pS = pA + gpmVAN( pA, "\r\n", s );
				iENV = 0;
			} break;
			case GPE_LWS_COM_Key:{
				gpc3Dkey key = pA;
				pIS = gpdITMis;
				if( !pIS )
					break;
				pKEY = pIS->cnl.pKEYinsIX(iCNL, iENV, nENV, key.sec );
				iENV++;
				//pKEY = pIS->cnl.pKEYins(iCNL, key.sec );	/// key.sec>=33.5 && key.sec<=33.6 && iCNL == 3
				if( pKEY )
					*pKEY = key;
			} break;
			case GPE_LWS_COM_C_close: {
				break;
				pIS = gpdITMis;
				if( pIS )
					if( bSTDcout_3D )  gpdCOUT << " " << nENV << "/" << pIS->cnl.nKEY(iCNL) << " c:" << iCNL << gpdFLUSH;

			} break;
			case GPE_LWS_COM_ParentItem: {
				if( !pMOM )
					continue;
				pMOM->momID = momID = gpfSTR2I8( pA, &pA, NULL, true );
				break;

				while( pMOM->momID ) {
					pMOM--;
					if( pMOM->itmID != momID )
						continue;

					pIS = gpdITMis;
					if( !pIS )
						break;
					pIS->momIX = pMOM->itmIX;
					pIS->nLEV = pMOM->nLEV+1;
					if( pIS == pBON ){
						pI0 = pBON-pBON->itmIX;
						pROOT = pIS;
						while( (pROOT->itmID&gpeLWSiTYP_TYP) == gpeLWSiTYP_BON ) {
							pROOT = pI0+pROOT->momIX;
						}
						pROOT->pBONadd( pIS );
					}
					break;
				}
				pMOM = NULL;

				if( !bSTDcout_3D )
					break;
				gpdCOUT	<< pIS->itmIX	<< ". l:"	<< pIS->nLEV << " "
							<< pIS->sNAME	<<  " 0x"	<<  std::hex << pIS->itmID
											<<  " 0x"	<<  std::hex << pIS->momID
											<< (pNULL ? "NULL":"") << gpdENDL;
			} break;

			case GPE_LWS_COM_BoneName:{
				pBON->pNAME(pA);
				pBON->mxIX = bnDCT.dctFND( pBON->pNAME(), gpmSTRLEN(pBON->pNAME()), iD );
			} break;
			case GPE_LWS_COM_BoneRestPosition:{ if( !pBON ) break;
				pBON->rstXYZ.sXYZW( pA, &pA );
			} break;
			case GPE_LWS_COM_BoneRestDirection:{ if( !pBON ) break;
				pBON->rstYPR.sXYZW( pA, &pA );
				pBON->rstYPR *= PI/180.0f;
			} break;
			case GPE_LWS_COM_BoneRestLength:{ if( !pBON ) break;
				pBON->rstWHD.sXYZW( pA, &pA );
			} break;
			case GPE_LWS_COM_Plugin: {
				char* pEP = strstr( pS, "EndPlugin" );
				if( !pEP )
					break;
				pS = pEP+gpmVAN( pEP, "\r\n", s );
			} break;
			default: {
			} break;
		}

	}

	for( U4 i = 0, j, k, ni = n3Ditm(); i < ni; i++ ) {
		pIS = p3Dii(i);

		if( pIS->itmID == pIS->momID )
			pIS->momIX = pIS->itmIX;
		else for( j = 0; j < ni; j++ ) {
			pMOM = p3Dii(j);
			if( !pMOM )
				break;
			if( pMOM->itmID != pIS->momID )
				continue;

			/// is MOM
			pIS->momIX = pMOM->itmIX;
			if( pMOM->nLEV ? true : (pMOM->momID == pMOM->itmID) ) {
				pIS->nLEV = pMOM->nLEV+1;
				break;
			}

			pMOM->stkIX = pMOM->itmIX;
			while( pMOM->momID != pMOM->itmID ) {
				for( U4 k = 0; k < ni; k++ ) {
					pROOT = p3Dii(k);
					if( !pROOT )
						break;
					if( pROOT->itmID != pMOM->momID )
						continue;
					pROOT->stkIX = pMOM->itmIX;
					pMOM = pROOT;
					break;
				}
				if( pMOM->nLEV )
					break;
			}
			while( pMOM->stkIX != pMOM->itmIX ) {
				pROOT = p3Dii(pMOM->stkIX);
				pROOT->nLEV = pMOM->nLEV+1;
				pMOM = pROOT;
			}

			pIS->nLEV = pMOM->nLEV+1;
			break;
		}

		if( (pIS->itmID&gpeLWSiTYP_TYP) == gpeLWSiTYP_BON ){
			/// is BONE
			pI0 = pIS-pIS->itmIX;
			pROOT = pIS;
			while( (pROOT->itmID&gpeLWSiTYP_TYP) == gpeLWSiTYP_BON ) {
				pROOT = pI0+pROOT->momIX;
			}
			pROOT->pBONadd( pIS );
		}
		if( !bSTDcout_3D )
				break;
		gpdCOUT	<< pIS->itmIX	<< ". l:"	<< pIS->nLEV << " "
				<< pIS->sNAME	<<  " 0x"	<<  std::hex << pIS->itmID
								<<  " 0x"	<<  std::hex << pIS->momID
								<< (pNULL ? "NULL":"") << gpdENDL;
	}
	return this;
}
gpc3Dgym* gpc3Dlst::p3DgymADD( gpeALF alf, const char* pP ) {
	if( !this )
		return NULL;

	gpc3Dgym **pp3Dgym = (gpc3Dgym**)lst3Dgym.Ux( 0, sizeof(gpc3Dgym*) );//, *p3D;
	I4 i3D = 0, e3D = lst3Dgym.nLD(sizeof(gpc3Dgym*));
	for( I4 n3D = e3D; i3D < n3D; i3D++ ) {
		if( !pp3Dgym[i3D] ) {
			if( e3D > i3D )
				e3D = i3D;
			continue;
		}
		if( pp3Dgym[i3D]->id.a8x2[0].b == alf )
			 return pp3Dgym[i3D];
	}
	pp3Dgym = (gpc3Dgym**)lst3Dgym.Ux( e3D, sizeof(gpc3Dgym*) );
	pp3Dgym[0] = new gpc3Dgym( e3D, pP, alf );
	return pp3Dgym[0];
}
I4 gpcGL::iLWS( gpeALF a, const char* pPATH, gpcLZY& rd ) {
	if( !this )
		return -1;

	if( !p3Dlst )
		p3Dlst = new gpc3Dlst;

	gpc3Dgym* p3Dgym = p3Dlst->p3DgymADD( a, pPATH )->pLWS(rd, p3Dlst->lwsDCT, p3Dlst->bnDCT );
	return p3Dgym ? p3Dgym->id.x : -1;
}

gpc3D* gpc3D::pLWO( gpcLZY& lwo, gpcLZYdct& bnDCT ) {
	if( !this )
		return NULL;
	/*if( !lwo.nLD() )
		return this;*/
	I8 i8w = lwo.nSUM();
	if( !i8w )
		return this;
	if( id.w == i8w )
		return this;

	if( nRDY == nBLD )
		return this;
	nRDY = nBLD;

	U4* pU4 = lwo.pU4(), *pU4i = pU4;
	if( *pU4i != LWO_ID_FORM )
		return this;
	pU4i++;
	U4	n = swp4(pU4i),
		chunk[2],
		nUi, ix, nx, ip, uf;
	chunk[1] = 0;
	pU4i++;
	if( *pU4i != LWO_ID_LWO2 )
		return this;
	pU4i++;

	U8 s;
	p_lwo = p_lwo->lzyADD( lwo.p_alloc, lwo.nLD(), s=0 );
	nUi = pU4i-pU4;
	pU4 = p_lwo->pU4(); pU4i = pU4+nUi;

	id.w = i8w;

	char	*pU0 = (char*)pU4,
			*pUnx = (char*)pU4i,
			*pUe = pUnx+n,
			*pUi;
	gpc3Dly* pLY = NULL;
	gpc3Dmap* pMAP = NULL;
	U4x2* pTG = NULL;
	while( pUnx < pUe ){
		pU4i = (U4*)pUnx;
		chunk[0] = *pU4i; pU4i++;
		n = swp4(pU4i); pU4i++;


		pUi = (char*)pU4i;
		pUnx = pUi+n;
		if( bSTDcout_3D )  gpdCOUT << pUi-pU0 << " LWO_ID_"<< (char*)chunk << ": "  << gpdFLUSH; //endl;
		switch( chunk[0] ) {
			case LWO_ID_TAGS: {
					while( pUi < pUnx ) {
						pTG = (U4x2*)tgIX.Ux( tgIX.nLD(sizeof(U4x2)), sizeof(U4x2) );
						pTG->i = pUi-pU0;
						pTG->n = gpmSTRLEN(pUi);
						pUi += gpmPAD( pTG->y+1,2 );
					}
					pTG = (U4x2*)tgIX.Ux( 0, sizeof(*pTG) );
				} break;
			case LWO_ID_LAYR: {
					if( pLY ) {

					}
					pLY = ((gpc3Dly*)NULL)->pLYadd(ly3D,pU4i);
				} break;
			case LWO_ID_PNTS: pLY->pPNTSswp( pU4i, (pUnx-pUi)/(sizeof(float)*3) ); break;
			case LWO_ID_BBOX: pLY->pBBOXswp(pU4i); break;
			case LWO_ID_POLS: {
					pUi += 4;
					switch( *pU4i ) {
						case LWO_ID_FACE: {
								if( bSTDcout_3D )  gpdCOUT << "\tLWO_ID_FACE:"<<  gpdENDL;
								pLY->fc.pFACEswp(pUi,pUnx-pUi)->COUT();
							} break;
						case LWO_ID_BONE: {
								if( bSTDcout_3D )  gpdCOUT << "\tLWO_ID_BONE:"<<  gpdENDL;
							} break;
						default: break;
					}
				} break;
			case LWO_ID_VMAP: {
					pUi += 4;
					U2 d = swp2(pUi); pUi += 2;
					U4 ix = 0, nUi, iD;

					switch( *pU4i ) {
						case LWO_ID_MNVW: {
							} break;
						case LWO_ID_WGHT: {
								if( bSTDcout_3D )  gpdCOUT << "\tLWO_ID_WGHT:"<<  gpdFLUSH;
								I4 ix = pLY->pWGHTswp( pUi, pUnx-pUi, bnDCT );
								U4x2 *pIX2 = (U4x2*)pLY->mapIXwg.Ux( ix, sizeof(*pIX2) );
								if( bSTDcout_3D )  gpdCOUT << pLY->mapDCTwg.sSTRix(ix,"ERR")  << " iBx:" << pIX2->x << " n:" << pIX2->y << gpdENDL;
							} break;
						case LWO_ID_TXUV: {
								if( bSTDcout_3D )  gpdCOUT << "\tLWO_ID_TXUV:"<<  gpdFLUSH;
								I4 ix = pLY->pTXUVswp( pUi, pUnx-pUi );
								U4x2 *pIX2 = (U4x2*)pLY->mapIXtx.Ux( ix, sizeof(*pIX2) );
								if( bSTDcout_3D )  gpdCOUT << pLY->mapDCTtx.sSTRix(ix,"ERR") << " iTX:" << pIX2->x << " nTX:" << pIX2->y << gpdENDL;
							} break;
						case LWO_ID_MORF: {
								if( bSTDcout_3D )  gpdCOUT << "\tLWO_ID_MORF:"<<  gpdFLUSH;
								ix = pLY->mapDCTmr.nIX();
								nUi = gpmSTRLEN(pUi);
								pLY->mapDCTmr.dctADD( pUi, nUi );
								pUi += gpmPAD( nUi+1,2 );
								if( ix >= pLY->mapDCTmr.nIX() )
									break;

							} break;
						default: break;
					}
				} break;
			case LWO_ID_VMAD: {
					pUi += 4;
					U2 d = swp2(pUi); pUi += 2;
					U4 iD, *pFi;
					U4x2* pFl;
					switch( *pU4i ) {
						case LWO_ID_MNVW: {
							} break;
						case LWO_ID_WGHT: {
							} break;
						case LWO_ID_TXUV: {
								if( bSTDcout_3D )  gpdCOUT << "\tLWO_ID_TXUV:"<<  gpdENDL;
								pMAP = pLY->pMAPswp( pUi, pUnx-pUi );
								if( !pMAP )
									break;
								if( bSTDcout_3D )  gpdCOUT << pLY->mapDCTtx.sSTRix(pMAP->id,"ERR") << " nP:" << pLY->pnt.nLD(sizeof(F4)) << " nPm:" << pMAP->pnt.nLD(sizeof(gpc3Dvx)) << gpdENDL;
							} break;
						case LWO_ID_MORF: {
							} break;
						default: break;
					}
				} break;

			case LWO_ID_PTAG: {
					pUi += 4;
					switch( *pU4i )
					{
						case LWO_ID_COLR:
							break;
						case LWO_ID_SURF: {
								if( bSTDcout_3D )  gpdCOUT << "\tLWO_ID_SURF:"<<  gpdENDL;
								pLY->SRFswp( pUi, pUnx-pUi )->SRFcout( lzySRF, pU0, pTG );
							} break;
						case LWO_ID_PART: {
								if( bSTDcout_3D )  gpdCOUT << "\tLWO_ID_PART:"<<  gpdENDL;
								pLY->PRTswp( pUi, pUnx-pUi )->PRTcout( pU0, pTG );
							} break;
						default: break;
					}
				} break;
			case LWO_ID_CLIP: {
					pUi += 4;
					if( pU4i[1] != LWO_ID_STIL )
						break;
					pUi = (char*)(pU4i+2);
					nUi = swp2(pUi); pUi += 2;
					char* pUp = strrchr( (char*)pUi, '/' );
					if( !pUp )
						break;
					pUp++;
					ix = pLY->mapDCTcl.nIX();
					pLY->mapDCTcl.dctADD( pUp, nUi-(pUp-pUi));

					if( bSTDcout_3D )  gpdCOUT << pLY->mapDCTcl.sSTRix(ix,"ERR") << gpdENDL;
				} break;
			case LWO_ID_SURF: {
					nUi = gpmSTRLEN(pUi);
					for( ix = 0; ix < tgIX.nLD(sizeof(*pTG)); ix++ ) {
						if( pTG[ix].y != nUi )
							continue;
						if( gpmMcmp( pUi, pU0+pTG[ix].x, nUi ) < nUi )
							continue;
						break;
					}

					U4 iS = pLY->mapDCTsr.nIX();
					pLY->mapDCTsr.dctADD( pUi, nUi );
					pUi+= gpmPAD( nUi+1,2 );
					pUi+= sizeof(U2);
					gpc3Dsrf* pSRF = pSRFadd(ix);
					if( !pSRF->sNAME[0] )
						gpmSTRCPY( pSRF->sNAME, pU0+pTG[ix].x );

					if( bSTDcout_3D )  gpdCOUT << ix << " " << iS << " "<< *(U4*)(pLY->srSUM.Ux(ix,sizeof(U4))) << " " << pSRF->sNAME <<  gpdENDL;
					char* pSUB, *pSS;
					while( pUi < pUnx ) {
						chunk[0] = *(U4*)pUi; pUi += sizeof(U4);
						n = swp2( pUi ); pUi += sizeof(U2);
						pSUB = pUi;
						pUi += n;
						if( bSTDcout_3D )  gpdCOUT << (char*)chunk << " "<< n << gpdENDL;
						switch( chunk[0] ) {
							case LWO_ID_BLOK: {
									while( pSUB < pUi ) {
										chunk[0] = *(U4*)pSUB; pSUB += sizeof(U4);
										n = swp2( pSUB ); pSUB += sizeof(U2);
										if( bSTDcout_3D )  gpdCOUT << " " << (char*)chunk << " " << n << gpdFLUSH;
										pSS = pSUB;
										pSUB += n;
										switch( chunk[0] ) {
											case LWO_ID_IMAG: {
													pSRF->imag = swp2(pSS)-1;
													gpmSTRCPY( pSRF->sPIC, pLY->mapDCTcl.sSTRix(pSRF->imag,"ERR") );
													char* pPNT = strrchr( pSRF->sPIC, '.' );
													if( pPNT )
														sprintf( pPNT, ".png" );
													if( bSTDcout_3D )  gpdCOUT << " " << pSRF->imag << " " << pSRF->sPIC <<  gpdENDL;
												} break;
											case LWO_ID_VMAP: {
													gpmSTRCPY( pSRF->sVMAP, pSS );
													pSRF->imap = pLY->mapDCTtx.dctFND( pSRF->sVMAP, gpmSTRLEN(pSRF->sVMAP), n );
													if( bSTDcout_3D )  gpdCOUT << " " << pSRF->imap << " " << pLY->mapDCTtx.sSTRix(pSRF->imap,"ERR") <<  gpdENDL;
												} break;

											default: break;
										}
									}
								} break;
							default: break;

						}
					}
					if( bSTDcout_3D )  gpdCOUT << gpdENDL;
				} break;
			default: break;
		}

	}



	if( p_lwo ){
		// RESET

	}




	return this;
}
gpc3D* gpc3Dlst::p3DobjADD( gpeALF alf, const char* pP ) {
	if( !this )
		return NULL;

	gpc3D	**pp3D = (gpc3D**)lst3D.Ux( 0, sizeof(gpc3D*) );//, *p3D;
	I4 i3D = 0, e3D = lst3D.nLD(sizeof(gpc3D*));
	for( I4 n3D = e3D; i3D < n3D; i3D++ ) {
		if( !pp3D[i3D] ) {
			if( e3D > i3D )
				e3D = i3D;
			continue;
		}
		if( pp3D[i3D]->id.a8x2[0].b == alf )
			return pp3D[i3D];
	}
	pp3D = (gpc3D**)lst3D.Ux( e3D, sizeof(gpc3D*) );
	pp3D[0] = new gpc3D( e3D, pP, alf );
	return pp3D[0];
}
I4 gpcGL::iLWO( gpeALF a, const char* pPATH, gpcLZY& rd ) {
	if( !this )
		return -1;

	if( !p3Dlst )
		p3Dlst = new gpc3Dlst;

	gpc3D* p3D = p3Dlst->p3DobjADD( a, pPATH )->pLWO(rd, p3Dlst->bnDCT );
	return p3D ? p3D->id.x : -1; //id;
}
char* gpc3Ditm::pNAME( char* pN ) {
	if( !this )
		return NULL;
	if( alfNM )
		return sNAME+nNAME;
	U8 s;
	pN += gpmNINCS(pN," \t\"");
	U4 n = gpmVAN(pN," \t\"\r\n",s);
	gpmMcpy( sNAME, pN, n )[n] = 0;

	char* pCH = strrchr(sNAME,'/');
	if( pCH ) {
		nNAME = pCH ? pCH - sNAME : 0;
		pCH += gpmNINCS(pCH,"/");
		nNAME = pCH-sNAME;
	} else
		nNAME = 0;
	//if( sNAME[nNAME] == '/' )
	//	nNAME++;

	alfNM = gpfSTR2ALF( (U1*)sNAME+nNAME, (U1*)sNAME+nNAME+gpmSTRLEN(sNAME+nNAME), NULL, '_' );
	return sNAME+nNAME;
}
gpc3D* gpc3Ditm::p3Dobj( gpcGL* pGL, const char* pP, char *pF ) {
	if( !this )
		return NULL;
	if( p3D ? true : !pP )
		return p3D;

	if( !pF ) {
		pF = strrchr( (char*)pP, '/' );
		if( !pF ){
			pF = (char*)pP+gpmSTRLEN(pP);
			pF += sprintf( pF, "/" );
		} else {
			pF++;
		}
	}

	U8 s;
	gpcLZY rd;
	char sPATH[0x100], *pFL = (char*)gpmMcpy( sPATH, pP, pF-pP ) + (pF-pP);
	gpmSTRCPY( pFL, pNAME() );
	while( gpmACE(sPATH,4) < 0 ) {
		pFL[-1] = 0;
		pFL = strrchr(sPATH,'/');
		if(!pFL)
			return p3D;
		pFL++;
		gpmSTRCPY( pFL, pNAME() );
	}
	rd.lzyRD( sPATH, s = 0 );
	I4 i = pGL->iLWO( alfNM, sPATH, rd );
	p3D = pGL->p3Dlst->p3Dix( i );
	return p3D;
}
/**
"zeroDB/scr/,	cam.eye.yz, pxSCR.xyr, 0, 1,
				cam.eye.xx, pxSCR.xyr, 0, 2,
				cam.trg.yz, pxSCR.xyr, 0, 3,
				man.pos.yz, pxSCR.xyr, 0, 9"

0.zerodb6 1.scr10 2.,12
	0			1			2			3		4			5			6		7		8
	 3.cam 16	 4.eye 20	 5.yz 23	 6., 24	 7.pxscr 30	 8.xyr 34	 9., 35	10., 38	11., 41
	12.cam 69	13.eye 73	14.xx 76	15., 77	16.pxscr 83	17.xyr 87	18., 88	19., 91	20., 94
	21.cam122	22.trg126	23.yz129	24.,130	25.pxscr136	26.xyr140	27.,141	28.,144	29.,147
	30.man175	31.pos179	32.yz182	33.,183	34.pxscr189	35.xyr193	36.,194	37.,197
	38."200

nAT:39
*/




//};

U4 gpc3D::nLY() { return ly3D.nLD(sizeof(gpc3Dly)); }
gpc3Dly* gpc3D::pLYix( U4 i ) {
	if( this ? nLY()<=i : true )
		return NULL;
	return (gpc3Dly*)ly3D.Ux( i, sizeof(gpc3Dly) );
}
U4x4 gpcGL::VBOobj( U4x4& iVXn, const gpc3Dvx* pVX, U4 nD ) {
	//Create VBO
	if( iVXn.y )
		return iVXn;
	iVXn.y++;
	GLenum e = 0;
	if( iVXn.x )
		glDeleteBuffers(1,  &iVXn.x );

	iVXn.a4x2[1] = U4x2( sizeof(*pVX), nD );
	GLsizeiptr n_byte = iVXn.a4x2[1].area();
	glGenBuffers( 1, &iVXn.x );
	glBindBuffer( GL_ARRAY_BUFFER, iVXn.x );
	glBufferData( GL_ARRAY_BUFFER, n_byte, pVX, GL_STATIC_DRAW ); gpfGLerr( " glBufferData( GL_ARRAY_BUFFER" );
	/* (e = glGetError());
	if( e ) if( bSTDcout_3D )  gpdCOUT << std::hex << e << " glBufferData( GL_ARRAY_BUFFER" <<  gpdENDL; */
	glBindBuffer( GL_ARRAY_BUFFER, 0 );
	return iVXn;
}
gpc3D* gpc3D::prt2ix( gpcGL* pGL ) {
	if( !this )
		return NULL;

	gpc3Dly* pLY = NULL;
	gpc3Dmap* pM = NULL;
	gpc3Dtri* pT0 = NULL;
	gpc3Dvx* pVX = NULL;
	U4 i = 0,n,m, nMAP, t,nT;
	for( n = nLY(); i < n; i++ ) {
		pLY = pLYix(i);
		nMAP = pLY->nMAP( p_lwo->p_alloc, tgIX.pU4x2()  );
		if( !nMAP )
			continue;

		for( m = 0; m < nMAP; m++ ){
			pM = pLY->pMAP(m);
			if( pGL ? !pM : true )
				continue;

			pVX = (gpc3Dvx*)pM->pnt.p_alloc;
			if( !pM->iVXn.y )
				pGL->VBOobj( pM->iVXn, pVX, pM->pnt.nLD(sizeof(*pVX)) );

			nT = pM->nTRI();
			pT0 = pM->pTRI0( pLY->buf );
			for( t = 0; t < nT; t++ ) {
				if( pT0[t].aIIXN[2].y )
					continue;
				if( pGL->IBOobj( pT0[t].aIIXN[2], pT0[t].t.pU4(), pT0[t].t.nU4(3), 3 ).y != 1 )
					continue;

				continue;

				for( U4 i = 0, n = pT0[t].t.nU4(3), *pIX = pT0[t].t.pU4(), j=0, je;  i < n; i++ )
				for( je = j+3; j < je; j++ )
					if( bSTDcout_3D )
						gpdCOUT << j << " " << pIX[j] << pVX[pIX[j]].pSTR( *(pLY->buf.lzyRST()), p_lwo->pCHAR(), tgIX.pU4x2() ) << gpdENDL;
			}
		}
	}
	return this;
}
gpcGL* gpcGL::glDRW( I4x2 xy, I4x2 wh ) {
	if( !this )
		return NULL;
	GLenum e = 0;

	if( aUniID[gpeUniID_tgPX] > -1 )
	if( pPICrtx ) {
		glUniform2f( aUniID[gpeUniID_tgPX], (float)pPICrtx->txWH.z, (float)pPICrtx->txWH.w ); gpfGLerr();
	} else {
		glUniform2f( aUniID[gpeUniID_tgPX], (float)trgWHpx.x, (float)trgWHpx.y ); gpfGLerr();
	}

	if( aUniID[gpeUniID_DIVxy] > -1 ) {
		glUniform2f( aUniID[gpeUniID_DIVxy], (float)xy.x, (float)xy.y ); gpfGLerr();
	}
	if( aUniID[gpeUniID_FRMwh] > -1 ) {
		glUniform2f( aUniID[gpeUniID_FRMwh], (float)wh.x, (float)wh.y ); gpfGLerr();
	}


	glBindBuffer( GL_ARRAY_BUFFER, aVXid[0] ); gpfGLerr();
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, aIXid[0] ); gpfGLerr();

	glVertexAttribPointer( ATvxID, 2, GL_FLOAT, GL_FALSE, sizeof(F4), 0 ); gpfGLerr();
	glEnableVertexAttribArray( ATvxID ); gpfGLerr();
	glVertexAttribPointer( ATuvID, 2, GL_FLOAT, GL_FALSE, sizeof(F4), gpmGLBOFF(sizeof(F2)) ); gpfGLerr();
	glEnableVertexAttribArray( ATuvID ); gpfGLerr();


	glDrawElements( GL_TRIANGLE_FAN, 4, GL_UNSIGNED_INT, NULL ); gpfGLerr();

	glDisableVertexAttribArray( ATvxID );
	glDisableVertexAttribArray( ATuvID );

	glBindBuffer( GL_ARRAY_BUFFER, 0 );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
	glBindVertexArray( 0 );

	return this;
}
gpc3Dsrf* gpc3D::pSRFadd( U4 i ) {
	gpc3Dsrf* pSRF = this ? (gpc3Dsrf*)lzySRF.Ux(i,sizeof(gpc3Dsrf)) : NULL;
	if( !pSRF )
		return NULL;
	if( pSRF->ix == i )
		return pSRF;

	pSRF->ix = i;
	return pSRF;
}
gpc3Dsrf* gpc3D::pSRFi( U4 i ) {
	if( this ? (i>=nSRF()) : true )
		return NULL;

	return pSRFadd(i);
}
gpcPIC* gpcGL::pPICsrf( gpc3Dsrf* pSRF, char* pPATH ) { //, char* pFILE ) {
	if( this ? !pSRF : true )
		return NULL;
	char sALF[0x100];
	gpcPIC* pPIC = NULL;
	if( !pSRF->an.alf ) {
		pSRF->an.num = 14;
		pSRF->an = (U1*)gpfSTRdrop( sALF, pSRF->sPIC, NULL, "_ \t" );
	}

	pPIC = pMASS->PIC.PIC(pSRF->an);

	if( pPIC ? !!pPIC->pSRF : true )
		return pPIC;
	// load
	pPIC->pPICrd( pPATH, pSRF->sPIC );
	return pPIC;

}

int aglVXi4[] = {
	gpmOFF(gpc3Dvx,xyzi),		// 00
	gpmOFF(gpc3Dvx,xyzi.w),		// 01
	gpmOFF(gpc3Dvx,up),			// 02
	gpmOFF(gpc3Dvx,uv),			// 03
	gpmOFF(gpc3Dvx,ps),			// 04
	sizeof(gpc3Dvx),			// 05
	6,7,8,9,0xa,0xb,0xc,0xd,0xe,0xf,
};

gpcGL* gpcGL::glDRW3D( U4 l, gpc3D* p3D, U8 msk ) {
	if( msk ? !p3D : true )
		return this;
	//p3Dobj =
	//if( !p3D )
	//	return this;

	p3D->prt2ix( this );
	U4x4* pSRFx4;
	gpc3Dly* pLY;
	gpc3Dmap* pM;
	gpc3Dtri* pT0;
	gpcPIC* pPIC;

	GLenum e = 0;

	U4 n = p3D->nLY(), i = 0;
	if( l <= n ) {
		n = l;
		i = n-1;
	}
	for( U4 nMAP; i < n; i++ ) {
		pLY = p3D->pLYix(i);
		nMAP = pLY->nMAP( p3D->p_lwo->p_alloc, p3D->tgIX.pU4x2() );
		if( !nMAP )
			continue;

		for( U4 m = 0, nT; m < nMAP; m++ ){
			pM = pLY->pMAP(m);
			if( pM ? !pM->iVXn.y : true )
				continue;

			nT = pM->nTRI();
			pT0 = pM->pTRI0( pLY->buf );

	/// VERTEX ------------------------------------------
			glBindBuffer( GL_ARRAY_BUFFER, pM->iVXn.x );  gpfGLerr();

			onATscn(aglVXi4);

			for( U4 t = 0; t < nT; t++, msk >>= 1 ) {
				if( !msk )
					break;
				if( (msk&1) ? !pT0[t].aIIXN[2].y : true )
					continue;

				aglVXi4[6] = pT0[t].aIIXN[2].a4x2[1].area();

				pSRFx4 = pT0[t].pSRF(0);
				if( !pSRFx4 )
					continue;
	/// ELEMENT ------------------------------------------
				glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, pT0[t].aIIXN[2].x );  gpfGLerr( "GL_ELEMENT_ARRAY_BUFFER");
				for( U4 s = 0, ns = pT0[t].nSRF(), n; s < ns; s++ ) {
					aglVXi4[0x7] = pSRFx4[s].w;
					aglVXi4[0x8] = pSRFx4[s+1].w;
					aglVXi4[0x9] = aglVXi4[8]-aglVXi4[7];
					aglVXi4[0xa] = aglVXi4[0x9]/3;
					if( aglVXi4[0xa] < 1 )
						continue;
					aglVXi4[0xb] = aglVXi4[0x7]*sizeof(U4);
					pPIC = pPICsrf( p3D->pSRFi(pSRFx4[s].x), p3D->sPATH );
					if( !pPIC )
						continue;
					glSETtx( 0, pPIC->surDRWtx(pRNDR), pPIC->txWH.a4x2[1] );


					glDrawElements( gpaDRWmod[3], /// GL_TRIANGLES,
								aglVXi4[0x9],
								GL_UNSIGNED_INT,
								(void*)aglVXi4[0xb] );
				}
				glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 ); gpfGLerr();
			}

			offATscn();

			glBindBuffer( GL_ARRAY_BUFFER, 0 ); gpfGLerr();
			glBindVertexArray( 0 ); gpfGLerr();
		}
	}
	return this;
}

gpeALF alfSCN0[] = {
	gpeALF_CAM,
	gpeALF_MAN,
	gpeALF_MRK,
};
gpeALF alfSCN1[] = {
	gpeALF_EYE,
	gpeALF_TRG,
	gpeALF_POS,
	gpeALF_ABC,
};
gpdSTATICoff sSCNdec1[] = "eye trg pos abc \"";
gpdSTATICoff gpsGLSLvx3D[] = //{
"#version 120																		\n"
"attribute	vec3	v_vx;															\n"
"attribute	vec4	v_ix;															\n"
"attribute	vec3	v_up;															\n"
"attribute	vec2	v_uv;															\n"
"attribute	ivec2	v_ps;															\n"
"varying	vec3	fr_uv;															\n"
"varying	vec3	fr_up;															\n"
"varying	vec2	fr_ps;															\n"
"uniform vec2 tgPX;																	\n"
"uniform vec2 nBON;																	\n"
"uniform mat4 aMX[20], aMXi[20];													\n"
"void main() {																		\n"
"   int  	ix =  int(v_ix.x*255);													\n"
" 	if(nBON.x<1) ix = 0; 															\n"
"   mat4   	mxi = aMXi[ix], mx = aMX[ix]*mxi;										\n"
"	vec3	vmx = (mx*vec4(v_vx,1.0)).xyz, vmx2 = (vmx+v_vx)/2.0,					\n"
"			mv	= (gl_ProjectionMatrix*gl_ModelViewMatrix*vec4(vmx,1.0)).xyz,		\n"
//"			mv	= (gl_ProjectionMatrix*gl_ModelViewMatrix*vec4(v_vx,1.0)).xyz,		\n"
"			xyz	= vec3(1.0, tgPX.x/tgPX.y, 1.0/250.0), 								\n"
"			p	= mv*xyz.xxz + vec3( 0.0, 0.0, 1.0 ); 								\n"
"	vec2 d = vec2( ((1.0/p.z)-0.5f)*2.0f, 1.0f ), sb = vec2(0,1.0);					\n"
"	p = p*d.xxy*xyz.xyx - sb.xxy;													\n"
"	gl_Position		= vec4( p*-1.0, 1.0 );											\n"
"	gl_Position.y	+= 0.5;															\n"
"	fr_uv			= vec3( v_uv, p.z ); //*vec2(1,-1)+vec2(0,1), p.z );		\n"
"	fr_up			= v_up;														\n"
"	fr_ps			= v_ps;														\n"
"}																					\n\0";
//};
gpdSTATICoff gpsGLSLfrg3D[] = //{
"#version 120																\n"
"varying vec3 fr_uv;														\n"
"varying vec3 fr_up;														\n"
"varying vec2 fr_ps;														\n"
"uniform sampler2D	tex0;					// MINI_CHAR_xXy_zXw.png		\n"
"uniform sampler2D	tex1;					// MINI_CHAR_xXy_zXw.png		\n"
"uniform vec4 		aCNL[8];				// CNL							\n"
"void main()																\n"
"{																			\n"
"   if( gl_FrontFacing ) discard;											\n"
"	vec2 mx = gl_FragCoord.xy/2;											\n"
"   float xx	=  mx.x-floor(mx.x)											\n"
"				+ (mx.y-floor(mx.y)) * 2.0;									\n"
"   if( true ) // xx < 1 )															\n"
		gpdGUIfr3Dc0
"   else if( xx < 2 )														\n"
		gpdGUIfr3Du
"   else if( xx < 3 )														\n"
		gpdGUIfr3Dp
"   else 																	\n"
		gpdGUIfr3Dc0
	gpdGUIfr3Dd
"}																			\n"
"\n\0";
//};
F4x4* gpc3Ditm::pMXl( float s ) {
	if( sec == s ) {
		mxW = mxL;
		return &mxL;
	}
	float v;
	F4 xyz = rstXYZ, ypr = rstYPR, scl = rstWHD;
	sec = s;

	if( sec > 0.25)
	for( U1 c = 0; c < 6; c++ ){
		v = cnl.valKEYs(c,sec);
		switch( c ) {
			case 0: xyz.x = v; break;
			case 1: xyz.y = v; break;
			case 2: xyz.z = v; break;
			case 3: ypr.ry = v; break;
			case 4: ypr.rp = v; break;
			case 5: ypr.rr = v; break;
			case 6: scl.x = v; break;
			case 7: scl.y = v; break;
			case 8: scl.z = v; break;
		}
	}

	if( bSTDcout_3D )  gpdCOUT	<< sNAME	<< " " << xyz.x << " " << xyz.y << " " << xyz.z
							<< " " << (ypr.ry*i80PI) << " " << (ypr.rp*i80PI) << " " << (ypr.rr*i80PI)
							<< gpdENDL;
	mxL.ypr(ypr);
	mxL.t.xyz_( xyz );
	mxW = mxL;
	return &mxL;
}
F4x4* gpc3Dtrk::pGYM( F4x4* pMX, gpc3Dgym* p3Dg, gpc3Ditm* p3Di, I8 ms, gpeACT act ) {
	if( !p3Di )
		return pMX;
	F4 xyz, ypr, scl(1,1,1);
	float sec = 0.125; //, v;
	switch( act ) {
		case gpeACT_DEF:
			if( mSEC >= ms )
				break;

			mSEC = ms;
			break;
		default:
			if( p3Di->ACT != act ) {
				p3Di->ACT = act;
				mSEC = ms;
			}
			sec = gpaACT_man[act].sec(ms-mSEC);
			break;
	}
	if( bSTDcout_3D )  gpdCOUT << "\r\n TRK: " << sec << gpdFLUSH;
	/// mxL ez az ITEM ---------------------
	p3Di->pMXl(sec);
	p3Di->stkIX = p3Di->itmIX;

	gpc3Ditm	*p3Di0 = p3Dg->p3Dii(0),
				*p3Diu = NULL,
				*p3Dim	= ((p3Di->itmIX != p3Di->momIX)
						? p3Di0+p3Di->momIX : NULL),
				*p3Dib,*p3Dibm;
	U4			*pBix, nB;
	if( !p3Dim )
		p3Diu = p3Di; // ez a ROOT nincs anya
	else {
		while( p3Dim ) {
			p3Dim->stkIX = p3Diu ? p3Diu->itmIX : p3Di->itmIX;
			if( (p3Dim->sec == sec) && (p3Dim->ACT == act) ) {
				if( p3Diu )
					break;
				p3Diu = p3Dim;
				break;
			}

			/// mxL MOM ---------------------
			p3Dim->ACT = act;
			p3Dim->pMXl(sec);

			p3Diu = p3Dim;
			if(p3Diu->itmIX == p3Diu->momIX)
				break;
			p3Dim = p3Di0+p3Diu->momIX;
		}
	}

	p3Dim = NULL;
	while( p3Diu ) {
		if( p3Dim )
			p3Diu->mxW = p3Diu->mxL*p3Dim->mxW;
		else {
			p3Dim	= ((p3Diu->itmIX != p3Diu->momIX)
						? p3Di0+p3Diu->momIX : NULL);
			if( p3Dim )
				p3Diu->mxW = p3Diu->mxL*p3Dim->mxW;
		}

		if( nB = p3Diu->nBON() ) {
			pBix = p3Diu->pBONadd();
			for( U4 i = 0; i < nB; i++ ) {
				p3Dib = p3Di0+pBix[i];
				p3Dib->ACT = act;
				p3Dib->pMXl(sec);
			}
			for( U4 i = 0; i < nB; i++ ) {
				p3Dib = p3Di0+pBix[i];
				p3Dibm = p3Di0+p3Dib->momIX;
				p3Dib->mxW =  p3Dib->mxL*p3Dibm->mxW;
			}
		}
		if( p3Diu == p3Di )
			break; // felért ehez

		p3Dim = p3Diu;
		p3Diu = p3Di0+p3Diu->stkIX;

	}

	*pMX = p3Di->mxW;
	pBix = p3Di->pBONadd();
	if( !pBix )
		return pMX;
	nB = p3Di->nBON();
	for( U4 i = 0; i < nB; i++ ) {
		p3Dib = p3Di0+pBix[i];
		pMX[p3Dib->mxIX] = p3Dib->mxW;
	}

	return pMX;
}
U4* gpc3Ditm::pBONadd( gpc3Ditm* pBON ) {
	if( !this )
		return NULL;
	if( !pBON )
		return nBON() ? bonLST.pU4() : NULL;

	U4	*pBix = bonLST.pU4(), nB = bonLST.nLD(sizeof(U4));
	if( pBix )
	for( U4 i = 0; i < nB; i++ ) {
		if( pBix[i] != pBON->itmIX )
			continue;
		return pBix+i;
	}
	pBix = bonLST.pU4n(-1);
	*pBix = pBON->itmIX;
	return pBix;
}

F4x4 aMX[0x40], aMXi[0x40];
gpcGL* gpcGL::glSCENE( gpMEM* pMEM, char* pS ) {
	*sPUB = 0;
	int nD1 = scnDEC1.nLD() ? scnDEC1.nLD() : scnDEC1.nAT(sSCNdec1,sizeof(sSCNdec1));

	U8 nLEN;
	char* pSe = pS+gpmVAN( pS, "\"", nLEN ), *pN, *pNx, *pU1;
	gpcLZY	AT;
	int nAT = AT.nAT( pS, pSe-pS," \r\n\t:+" ), i0, i1, iAs, iF4;
	if(!nAT)
		return this;
	F4	aCAM[0x20],
		*pMAN = aCAM+(gpmN(aCAM)/2);
	gpmZ(aCAM);
	U1 *apV[8]; size_t aVn[8], nU1;
	gpDBitm	*apI[2], *pI0;
	I8x2	*pAT = AT.pI8x2(),
			*pD1 = scnDEC1.pI8x2(), aaS;

	gpDBlst *pIl = pMEM->pMASS->iDBu( pMEM, pS, sPUB, sPUB + sprintf( sPUB, "./" ) );
	gpeALF alfD;
	GLenum e = 0;
	I8x4 prv(-1,-1,-1,-1);
	if( bSTDcout_3D )  gpdCOUT << "\033[2J\033[1;1H";
	for( U4 iAT = pAT[0].aALFvan( alfSCN0, nAT, gpmN(alfSCN0) ), iAnx, iAT1;
			iAT < nAT;
			iAT += iAnx ){
		if( pAT[iAT].alf == gpeALF_MRK )
			break;
		*sPUB = 0;
		iAT1 = iAT+1;
		iAnx = pAT[iAT1].aALFvan( alfSCN0, nAT-iAT1, gpmN(alfSCN0) )+1;
		pNx = pS+pAT[iAT+iAnx].num;

		pN = pS+pAT[iAT].num;
		pN += gpmVAN( pN, "0123456789ATuvID",nLEN );
		if( pN > pNx )
			continue;
		prv.a8x2[0].x = gpmSTR2U8(pN,10);
		if( prv.a8x2[0].y != prv.a8x2[0].x ) {
			apI[0] = pIl->pITMid(prv.a8x2[0].x);
			if( !apI[0] )
				continue;
			prv.a8x2[0].y = prv.a8x2[0].x;
		}

		pN += gpmVAN( pN, "0123456789",nLEN );
		if( pN > pNx )
			continue;
		prv.a8x2[1].x = gpmSTR2U8(pN,10);
		if( prv.a8x2[1].y != prv.a8x2[1].x ) {
			apI[1] = pIl->pITMid(prv.a8x2[1].x);
			if( !apI[1] )
				continue;
			prv.a8x2[1].y = prv.a8x2[1].x;
		}

		alfD = pAT[iAT+2].alf;
		iAs = pAT[iAT+3].alf == gpeALF_CM ? iAT+5:iAT+4;
		aaS.b = pAT[iAs].alf;
		aaS.a = pAT[iAs-1].alf;

		nU1 = pAT[iAT+2].num-pAT[iAT+1].num;
		pU1 = pS+pAT[iAT+1].num;

		apV[0] = apI[0]->pAB(aaS,aVn[0]); //apV[0].str(sPUB);
		apV[1] = apI[1]->pAB(aaS,aVn[1]); //apV[1].str(sPUB);

		iF4 = pD1->alfFND(pAT[iAT+1].alf,nD1);
		switch( pAT[iAT].alf ) {
			case gpeALF_CAM:
						if( bSTDcout_3D )  gpdCOUT <<  (apI[0] ? apI[0]->ID : (I8)-1) << " " <<  (apI[1] ? apI[1]->ID : (I8)-1) << " " << gpdENDL;
						aCAM[iF4].abLOAD( pU1, nU1, aaS.b, apV, aVn );
						break;
			case gpeALF_MAN: pMAN[iF4].abLOAD( pU1, nU1, aaS.b, apV, aVn ); break;
			default: break;
		}
	}
	pI0 = pIl->pITM();
	I8x2 vf( gpeALF_SCENE, (I8)5 );
	if( !pMEM->pMgl )
		return this;
	GLSLset( vf, gpsGLSLfrg3D, gpsGLSLvx3D );
	glUseProgram( gProgID ); gpfGLerr( " glUseProgram( gProgID" );

	if( aUniID[gpeUniID_tgPX] > -1 )
	if( pPICrtx ) {
		glUniform2f( aUniID[gpeUniID_tgPX], (float)pPICrtx->txWH.z, (float)pPICrtx->txWH.w ); gpfGLerr();
	} else {
		glUniform2f( aUniID[gpeUniID_tgPX], (float)trgWHpx.x, (float)trgWHpx.y ); gpfGLerr();
	}

	F4x4 view, wMX = 1.0, uMX, *pRSTmx;
	uMX.x = F4( 1.0, 0.0, 0.0 );
	uMX.y = F4( 0.0, 0.0, 1.0 );
	uMX.z = F4( 0.0,-1.0, 0.0 );
	uMX.t = F4( 0.0, 0.0, 0.0, 1.0 );

	glMatrixMode(GL_MODELVIEW);
	aMX[0] = uMX;
	glLoadMatrixf((const GLfloat*)&aMX[0]);

	glMatrixMode(GL_PROJECTION);
    {
        F4	eye = aCAM[0], // /16.0f,
			cntr = 0,
			up = { 0.f, 0.f, 1.f };
		eye *= sqrt(eye.qlen_xyz())/1024.0;
		view.latR( eye, cntr, up );
    }
    glLoadMatrixf((const GLfloat*)&view);
    view.iSTRf4x4(sPUB,",", "\r\n" );
    if( *sPUB )
		if( bSTDcout_3D )  gpdCOUT << sPUB << gpdENDL;
	gpc3Dgym* p3Dg;
	gpc3Ditm* p3Di;
	gpc3Dtrk* p3Dt = NULL;
	size_t n3Dt;

	for( int i = 0, n = pIl->nITM(); i < n; i++ ) {
		apV[0] = pI0[i].fndXB( gpeALF_LWS );
		if( apV[0] ) {
			p3Dg = p3Dlst->p3DgymIX( *(I4*)apV[0] );
			if( !p3Dg ) continue;

			apV[2] = pI0[i].fndXB( gpeALF_TRK );
			if( !apV[2] ) {
				I8x2 ab( gpeALF_MAN, gpeALF_TRK );
				p3Dt = (gpc3Dtrk*)pI0[i].pAB( ab, n3Dt );
			} else
				p3Dt = (gpc3Dtrk*)apV[2];

			for( U4 i = 0, ni = p3Dg->n3Ditm(); i < ni; i++ ){
				p3Di = p3Dg->p3Dii(i);
				if( p3Di ? p3Di->bNULL : true )
					continue; //break;
				switch( p3Di->itmID&gpeLWSiTYP_TYP ){
					case gpeLWSiTYP_OBJ: {
							if( p3Di->bNULL ) {
								//p3Dt->pGYM( aMX+1, p3Dg, p3Di, pMEM->pWIN->mSEC.x, gpeACT_WALK );
								//aMX[0] = aMX[1]*uMX;
								break;
							}
							p3Dt->pGYM( aMX+1, p3Dg, p3Di, pMEM->pWIN->mSEC.x, gpeACT_WALK );
							/// MODELVIEW
							glMatrixMode(GL_MODELVIEW);
							aMX[0] = aMX[1]*uMX;
							glLoadMatrixf((const GLfloat*)&aMX[0]);

							p3Dobj = p3Di->p3Do();
							if( !p3Dobj ) {
								p3Dobj = p3Di->p3Dobj( this, p3Dg->sPUB, p3Dg->pPUB );
								if( !p3Dobj )
									break;
							}

							GLint n3Db = p3Di->nBON();
							if( n3Db ) {
								n3Db = p3Dobj->nMX();
								if( n3Db ) {
									pRSTmx = p3Dobj->pMX( 0 );

									if( bSTDcout_3D ) if( false )
									for( U4 i = 1; i < n3Db; i++ ) {
										pRSTmx[i].iSTRf4x4( sPUB, ", ", "\r\n" );
										gpdCOUT << sPUB << gpdENDL;
									}

								} else {
									pRSTmx = p3Dobj->pMX( p3Di->nBON()+2 );
									n3Db = p3Dobj->nMX();

									p3Dt->pGYM( aMXi+1, p3Dg, p3Di, 0, gpeACT_DEF );
									for( U4 i = 1; i < n3Db; i++ )
										pRSTmx[i] = aMXi[i].inv();
									pRSTmx[0] = pRSTmx[1];
								}
								if( aUniID[gpeUniID_aMX] > -1 )
									glUniformMatrix4fv( aUniID[gpeUniID_aMX], 19, GL_FALSE, (const GLfloat*)&aMX[1] );
								if( aUniID[gpeUniID_aMXi] > -1 )
									glUniformMatrix4fv( aUniID[gpeUniID_aMXi], 19, GL_FALSE, (const GLfloat*)(pRSTmx+1) );
							}
							if( aUniID[gpeUniID_nBON] > -1 )
								//glUniform1i( aUniID[gpeUniID_nBON], n3Db ); gpfGLerr();
								glUniform2f( aUniID[gpeUniID_nBON], (float)n3Db, (float)n3Db ); gpfGLerr();

							if( !p3Dnull )
							if( strstr(p3Di->sNAME, "eye" ) )
								p3Dnull = p3Dobj;

							glDRW3D( p3Di->layID, p3Dobj, -1 ); //p3Di->nBON() ? 0x9 : -1 );
						} break;
					case gpeLWSiTYP_BON:
						break;
						if( !p3Dnull )
							break;

						glMatrixMode(GL_MODELVIEW);
						aMX[0] = p3Di->mxW*uMX;
						glLoadMatrixf((const GLfloat*)&aMX[0]);

						glDRW3D( 1, p3Dnull, -1 );
						break;
					case gpeLWSiTYP_LIG:
					case gpeLWSiTYP_CAM:
					default: break;
				}
			}
		} else {
			apV[0] = pI0[i].fndXB( gpeALF_LWO );
			if( !apV[0] ) continue;

			gpc3D* p3D = p3Dlst->p3Dix( *(I4*)apV[0] );
			if( !p3D ) continue;

			glDRW3D( 1, p3D, -1 );
		}
	}
	return this;
}

