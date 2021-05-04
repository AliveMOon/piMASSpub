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

char gp_man_lws[] = //{
	"manus\n"
	"bone_mother\n"
	"bone_groin\n"
	"bone_waist\n"
	"bone_throax\n"

	"bone_l.upperarm\n"
	"bone_l.forearm\n"
	"bone_l.hand\n"

	"bone_r.upperarm\n"
	"bone_r.forearm\n"
	"bone_r.hand\n"

	"bone_l.thigh\n"
	"bone_l.shin\n"
	"bone_l.feet\n"

	"bone_r.thigh\n"
	"bone_r.shin\n"
	"bone_r.feet\n"

	"bone_neck\n"
	"bone_head\n\0";
//};

gpc3Dlst::gpc3Dlst(){
	gpmCLR;
	U8 nLEN;
	char* pS = gp_man_lws, *pSe;
	while( *pS ) {
		pSe = pS+gpmVAN( pS, "\r\n", nLEN );
		bonLST.dctADD( pS, pSe-pS );
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
class gpc3Dsrf {
public:
	char sNAME[0x100],
		 sPIC[0x100],
		 sVMAP[0x100];
	I4 imag, imap;
	gpc3Dsrf(){};
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
			if(bSTDcout){ std::cout << i << " " << pFs[i] << std::endl; }
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

		gpc3Dvx	tmP, *pPd = ((gpc3Dvx*)pnt.Ux( nP, sizeof(tmP) ))-nP;
		for( U4 i = 0; i < nP; i++ ) {
			pPd[i].xyzi = pPs[i];
			pPd[i].xyzi.uXYZW[3] = pB[i].ix.u4;
			pPd[i].uv = pTs[i];
			pPd[i].ps = pB[i].ps;
		}


		for( U4 f = 0, i, ip, x,n; f < nF; f++ )
		for( x = pFl[f].x, n = pFl[f].y, i = 0; i < n; i++ ) {
			ip = pFi[i+x];
			v0 = pPd[ip].xyzi;
			v1 = (pPd[pFi[((i-1+n)%n)+x]].xyzi - v0).N3();
			v2 = (pPd[pFi[((i+1  )%n)+x]].xyzi - v0).N3();
			pPd[ip].up += v2.X3(v1).N3().xyz1();
		}

		for( U4 p = 0; p < nP; p++ )
			pPd[p].up = pPd[p].up.N3();

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
				pPd = (gpc3Dvx*)pnt.Ux( ip, sizeof(tmP) );
				tmP = *pPd;
				tmP.uv.swpXY( pUi ); pUi += sizeof(F2);


				for( t = p.nLD(sizeof(tmP)), te = pnt.nLD(sizeof(tmP)); t < te; t++ ) {
					if( ((gpc3Dvx*)pnt.Ux( pFi[i], sizeof(*pPd) ))->bGD(tmP, 0.0/4096.0, 0.0/1024.0 )  )
						break;
				}

				if( pFi[i] == t )
					break;

				pFi[i] = t;
				if( t < te )
					break;

				*(gpc3Dvx*)pnt.Ux( te, sizeof(*pPd) ) = tmP;
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
	gpc3Dtri* pTRI( gpcLZY& buf, const void* pLWO = NULL, U4x2* pTG = NULL ) {
		if( nTRI() )
			return (gpc3Dtri*)tri.Ux( 0, sizeof(gpc3Dtri) );
		if( !pLWO )
			pTG = NULL;

		U4x2 	*pTMP = ((U4x2*)buf.Ux( fps.nLD() + 1, 1 ))-(fps.nLD()+1),
				*pPRT = fps.pU4x2(),//(U4x2*)fps.p_alloc,
				*pFl0 = fcl.pU4x2(),
				*pFli;
		U4	nFC = fps.nLD(sizeof(*pTMP)),
			*pFi0 = fci.pU4(),
			*pFii, *pU4;


		pPRT->median(nFC,pTMP,true);
		gpc3Dtri* pTR = NULL;
		U4x4* pSRF;


		for( U4 i = 0, p = -1, s = -1,nf; i < nFC; i++ ) {
			if( p != pPRT[i].prt ) {
				if( pTR ) {
					pSRF = pTR->pSRF(-1);
					if( pTG ) std::cout << " SRF:"	<< " " << ((char*)pLWO) + pTG[pSRF[-1].x].x
												<< " 1n:" << (pSRF[0].y-pSRF[-1].y)
												<< " 2n:" << (pSRF[0].z-pSRF[-1].z)/2
												<< " 3n:" << (pSRF[0].w-pSRF[-1].w)/3 << std::endl;
				}
				pTR = (gpc3Dtri*)tri.Ux( tri.nLD(sizeof(*pTR)), sizeof(*pTR) );
				pTR->prt = p = pPRT[i].prt;
				if( pTG ) std::cout << "PART:" <<((char*)pLWO) + pTG[p].x << std::endl;
				s = -1;
				pSRF = 0;
			}
			if( s != pPRT[i].srf ){
				s = pPRT[i].srf;
				if( pSRF ){
					pSRF = pTR->pSRF(s);
					if( pTG ) std::cout << " SRF:"	<< " " << ((char*)pLWO) + pTG[pSRF[-1].x].x
												<< " 1n:" << (pSRF[0].y-pSRF[-1].y)
												<< " 2n:" << (pSRF[0].z-pSRF[-1].z)/2
												<< " 3n:" << (pSRF[0].w-pSRF[-1].w)/3 << std::endl;
				} else
					pSRF = pTR->pSRF(s);
				if( pTG ) std::cout << " SRF:"	<< " " << ((char*)pLWO) + pTG[pSRF->x].x
												<< " 1p:" << pSRF->y
												<< " 2p:" << pSRF->z/2
												<< " 3p:" << pSRF->w/3 << std::flush;
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

		pSRF = pTR->pSRF(-1);
		if( pTG ) std::cout << " SRF:"	<< " " << ((char*)pLWO) + pTG[pSRF[-1].x].x
												<< " 1n:" << (pSRF[0].y-pSRF[-1].y)
												<< " 2n:" << (pSRF[0].z-pSRF[-1].z)/2
												<< " 3n:" << (pSRF[0].w-pSRF[-1].w)/3 << std::endl;
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

	gpcLZYdct	mapDCTmn;
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
	I4 pWGHTswp( const void* pV, U4 n, gpcLZYdct& dctBN  ) {
		U4	nUi = gpmSTRLEN(pV), ip, uf,
			iBn, iBx = dctBN.dctFND( pV, nUi, iBn );
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
			pTX->swpXY( pUi ); pUi += sizeof(*pTX);
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
			std::cout << i << " " << n << " " << pSRF->sNAME <<  std::endl;
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
			std::cout << i << " " << n << " " << (char*)(pU0+pTG[i].x) <<  std::endl;
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
			pM->pTRI( buf, pLWO, pTG );
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




gpc3D* gpc3D::pLWO( gpcLZY& lwo, gpcLZYdct& dctBN ) {
	if( !this )
		return NULL;
	U8 s;
	if( !lwo.nLD() )
		return this;
	U4* pU4 = (U4*)lwo.p_alloc, *pU4i = pU4;
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
	I8 i8w = lwo.nSUM();
	if( id.w == i8w )
		return this;
	char	*pU0 = (char*)lwo.p_alloc,
			*pUnx = (char*)pU4i,
			*pUe = pUnx+n,
			*pUi;
	gpc3Dly* pLY = NULL;
	gpc3Dmap* pMAP = NULL;
	U4x2* pTG;
	while( pUnx < pUe ){
		pU4i = (U4*)pUnx;
		chunk[0] = *pU4i; pU4i++;
		n = swp4(pU4i); pU4i++;


		pUi = (char*)pU4i;
		pUnx = pUi+n;
		std::cout << pUi-pU0 << " LWO_ID_"<< (char*)chunk << ": "  << std::flush; //endl;
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
								std::cout << "\tLWO_ID_FACE:"<<  std::endl;
								pLY->fc.pFACEswp(pUi,pUnx-pUi)->COUT();
							} break;
						case LWO_ID_BONE: {
								std::cout << "\tLWO_ID_BONE:"<<  std::endl;
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
								std::cout << "\tLWO_ID_WGHT:"<<  std::flush;
								I4 ix = pLY->pWGHTswp( pUi, pUnx-pUi, dctBN );
								U4x2 *pIX2 = (U4x2*)pLY->mapIXwg.Ux( ix, sizeof(*pIX2) );
								std::cout << pLY->mapDCTwg.sSTRix(ix,"ERR")  << " iBx:" << pIX2->x << " n:" << pIX2->y << std::endl;
							} break;
						case LWO_ID_TXUV: {
								std::cout << "\tLWO_ID_TXUV:"<<  std::flush;
								I4 ix = pLY->pTXUVswp( pUi, pUnx-pUi );
								U4x2 *pIX2 = (U4x2*)pLY->mapIXtx.Ux( ix, sizeof(*pIX2) );
								std::cout << pLY->mapDCTtx.sSTRix(ix,"ERR") << " iTX:" << pIX2->x << " nTX:" << pIX2->y << std::endl;
							} break;
						case LWO_ID_MORF: {
								std::cout << "\tLWO_ID_MORF:"<<  std::flush;
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
								std::cout << "\tLWO_ID_TXUV:"<<  std::endl;
								gpc3Dmap* pMAP = pLY->pMAPswp( pUi, pUnx-pUi );
								if( !pMAP )
									break;
								std::cout << pLY->mapDCTtx.sSTRix(pMAP->id,"ERR") << " nP:" << pLY->pnt.nLD(sizeof(F4)) << " nPm:" << pMAP->pnt.nLD(sizeof(gpc3Dvx)) << std::endl;
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
								std::cout << "\tLWO_ID_SURF:"<<  std::endl;
								pLY->SRFswp( pUi, pUnx-pUi )->SRFcout( lzySRF, pU0, pTG );
							} break;
						case LWO_ID_PART: {
								std::cout << "\tLWO_ID_PART:"<<  std::endl;
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

					std::cout << pLY->mapDCTcl.sSTRix(ix,"ERR") << std::endl;
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
					gpc3Dsrf* pSRF = (gpc3Dsrf*)lzySRF.Ux( ix, sizeof(*pSRF) );
					if( !pSRF->sNAME[0] )
						gpmSTRCPY( pSRF->sNAME, pU0+pTG[ix].x );

					std::cout << ix << " " << iS << " "<< *(U4*)(pLY->srSUM.Ux(ix,sizeof(U4))) << " " << pSRF->sNAME <<  std::endl;
					char* pSUB, *pSS;
					while( pUi < pUnx ) {
						chunk[0] = *(U4*)pUi; pUi += sizeof(U4);
						n = swp2( pUi ); pUi += sizeof(U2);
						pSUB = pUi;
						pUi += n;
						std::cout << (char*)chunk << " "<< n << std::endl;
						switch( chunk[0] ) {
							case LWO_ID_BLOK: {
									while( pSUB < pUi ) {
										chunk[0] = *(U4*)pSUB; pSUB += sizeof(U4);
										n = swp2( pSUB ); pSUB += sizeof(U2);
										std::cout << " " << (char*)chunk << " " << n << std::flush;
										pSS = pSUB;
										pSUB += n;
										switch( chunk[0] ) {
											case LWO_ID_IMAG: {
													pSRF->imag = swp2(pSS)-1;
													gpmSTRCPY( pSRF->sPIC, pLY->mapDCTcl.sSTRix(pSRF->imag,"ERR") );
													std::cout << " " << pSRF->imag << " " << pSRF->sPIC <<  std::endl;
												} break;
											case LWO_ID_VMAP: {
													gpmSTRCPY( pSRF->sVMAP, pSS );
													pSRF->imap = pLY->mapDCTtx.dctFND( pSRF->sVMAP, gpmSTRLEN(pSRF->sVMAP), n );
													std::cout << " " << pSRF->imap << " " << pLY->mapDCTtx.sSTRix(pSRF->imap,"ERR") <<  std::endl;
												} break;

											default: break;
										}
									}
								} break;
							default: break;

						}
					}
					std::cout << std::endl;
				} break;
			default: break;
		}

	}



	if( p_lwo ){
		// RESET

	}
	p_lwo = p_lwo->lzyADD( lwo.p_alloc, lwo.nLD(), s=0 );
	id.w = i8w;




	return this;
}

I4 gpcGL::iLWO( gpeALF a, const char* pPATH, gpcLZY& rd ) {
	if( !this )
		return -1;

	if( !p3Dlst )
		p3Dlst = new gpc3Dlst;

	gpc3D* p3D = p3Dlst->p3D( a, pPATH )->pLWO(rd, p3Dlst->bonLST );
	return p3D ? p3D->id.x : -1; //id;
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
	glBufferData( GL_ARRAY_BUFFER, n_byte, pVX, GL_STATIC_DRAW ); (e = glGetError());
	if( e ) std::cout << std::hex << e << " glBufferData( GL_ARRAY_BUFFER" <<  std::endl;
	glBindBuffer( GL_ARRAY_BUFFER, 0 );
	return iVXn;
}
gpc3D* gpc3D::prt2ix( gpcGL* pGL ) {
	gpc3Dly* pLY;
	//gpc3Dtri* pTR;
	for( U4 i = 0, n = nLY(), nTR, nMAP; i < n; i++ ) {
		pLY = pLYix(i);
		nMAP = pLY->nMAP( p_lwo->p_alloc, tgIX.pU4x2()  );
		if( !nMAP )
			continue;
		gpc3Dmap* pM;
		gpc3Dtri* pT0;
		for( U4 m = 0, nT; m < nMAP; m++ ){
			pM = pLY->pMAP(m);
			if( pGL ? !pM : true )
				continue;
			gpc3Dvx* pVX = (gpc3Dvx*)pM->pnt.p_alloc;

			//pM->iVXn =
			if( !pM->iVXn.y )
				pGL->VBOobj( pM->iVXn, pVX, pM->pnt.nLD(sizeof(*pVX)) );

			nT = pM->nTRI();
			pT0 = pM->pTRI( pLY->buf );
			for( U4 t = 0; t < nT; t++ ) {
				if( pT0[t].aIIXN[2].y )
					continue;
				if( pGL->IBOobj( pT0[t].aIIXN[2], pT0[t].t.pU4(), pT0[t].t.nU4(3), 3 ).y != 1 )
					continue;

				for( U4 i = 0, n = pT0[t].t.nU4(3), *pIX = pT0[t].t.pU4(), j=0, je;  i < n; i++ )
				for( je = j+3; j < je; j++ )
					std::cout << j << " " << pIX[j] << pVX[pIX[j]].pSTR( *(pLY->buf.lzyRST()), p_lwo->pCHAR(), tgIX.pU4x2() ) << std::endl;
			}
		}
	}
	return this;
}
gpcGL* gpcGL::glDRW( I4x2 xy, I4x2 wh ) {
	if( !this )
		return NULL;
	GLenum e = 0;

	if( aUniID[0] > -1 )
	if( pPICrtx ) {
		glUniform2f( aUniID[0], (float)pPICrtx->txWH.z, (float)pPICrtx->txWH.w ); gpfGLerr();
	} else {
		glUniform2f( aUniID[0], (float)trgWHpx.x, (float)trgWHpx.y ); gpfGLerr();
	}

	if( aUniID[1] > -1 ) {
		glUniform2f( aUniID[1], (float)xy.x, (float)xy.y ); gpfGLerr();
	}
	if( aUniID[2] > -1 ) {
		glUniform2f( aUniID[2], (float)wh.x, (float)wh.y ); gpfGLerr();
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
gpcGL* gpcGL::glDRW3D( gpc3D* p, U4 msk ) {
	if( !msk )
		return this;
	p3D = p->prt2ix( this );
	if( !p3D )
		return this;

	gpc3Dly* pLY;
	gpc3Dmap* pM;
	gpc3Dtri* pT0;
	U4x4 glU4x4;
	GLenum e = 0;
	for( U4 i = 0, n = p3D->nLY(), nMAP; i < n; i++ ) {
		pLY = p3D->pLYix(i);
		nMAP = pLY->nMAP( p3D->p_lwo->p_alloc, p3D->tgIX.pU4x2() );
		if( !nMAP )
			continue;

		for( U4 m = 0, nT; m < nMAP; m++ ){
			pM = pLY->pMAP(m);
			if( pM ? !pM->iVXn.y : true )
				continue;
			glU4x4.x = gpmOFF(gpc3Dvx,xyzi);
			glU4x4.y = gpmOFF(gpc3Dvx,uv);
			glU4x4.z = sizeof(gpc3Dvx);
			nT = pM->nTRI();
			pT0 = pM->pTRI( pLY->buf );

	{ /// VERTEX ------------------------------------------
				glBindBuffer( GL_ARRAY_BUFFER, pM->iVXn.x );  gpfGLerr();
				/// XYZ
				glVertexAttribPointer( ATvxID, 3, GL_FLOAT, GL_FALSE, glU4x4.z, gpmGLBOFF(glU4x4.x) );  gpfGLerr();
				glEnableVertexAttribArray( ATvxID ); gpfGLerr();
				/// UV
				glVertexAttribPointer( ATuvID, 2, GL_FLOAT, GL_FALSE, glU4x4.z, gpmGLBOFF(glU4x4.y) );  gpfGLerr();
				glEnableVertexAttribArray( ATuvID ); (e = glGetError());  gpfGLerr();
	}
			for( U4 t = 0; t < nT; t++, msk >>= 1 ) {
				if( (msk&1) ? !pT0[t].aIIXN[2].y : true )
					continue;
				glU4x4.w = pT0[t].aIIXN[2].a4x2[1].area();


	/// ELEMENT ------------------------------------------
				glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, pT0[t].aIIXN[2].x );  gpfGLerr();
				glDrawElements( //GL_TRIANGLES,
								gpaDRWmod[3], /// GL_TRIANGLES,
								glU4x4.w,
								GL_UNSIGNED_INT,
								NULL ); (e = glGetError());  gpfGLerr();
				glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 ); gpfGLerr();
			}
			glDisableVertexAttribArray( ATvxID ); gpfGLerr();
			glDisableVertexAttribArray( ATuvID ); gpfGLerr();
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
char	sSCNdec1[] = "eye trg pos abc \"";
gpcGL* gpcGL::glSCENE( gpMEM* pMEM, char* pS ) {
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
	U1 *apV[2]; size_t aVn[2], nU1;
	gpITM	*apI[2], *pI0;
	I8x2	*pAT = AT.pI8x2(),
			*pD1 = scnDEC1.pI8x2(), aaS;

	///pAT->alfCON( sPUB,nAT);

	gpITMlst *pIl = pMEM->pMASS->iDBu( pMEM, pS, sPUB, sPUB + sprintf( sPUB, "./" ) );
	gpeALF alfD;
	GLenum e = 0;
	I8x4 prv(-1,-1,-1,-1);
	std::cout << "\033[2J\033[1;1H";
	//std::cout <<  "\033[2J" << std::flush;
	//system("clear");
	for( U4 iAT = pAT[0].aALFvan( alfSCN0, nAT, gpmN(alfSCN0) ), iAnx, iAT1;
			iAT < nAT;
			iAT += iAnx ){
		if( pAT[iAT].alf == gpeALF_MRK )
			break;
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

		//aaS.a = pAT[iAT+3].alf == gpeALF_CM ? pAT[iAT+4].alf:pAT[iAT+3].alf;
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
						std::cout <<  (apI[0] ? apI[0]->ID : (I8)-1) << " " <<  (apI[1] ? apI[1]->ID : (I8)-1) << " " << std::endl;
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

	F4x4 view;
	glMatrixMode(GL_MODELVIEW);
    {
        F4	eye = aCAM[0]/64.0f,
			cntr = 0,
			up = { 0.f, 0.f, 1.f };
		view.lat( eye, cntr, up );
       // mat4x4_look_at( view, eye, cntr, up );
    }
    glLoadMatrixf((const GLfloat*)&view);
    view.str(sPUB,",", "\r\n" );
	std::cout << sPUB << std::endl;
	for( int i = 0, n = pIl->nITM(); i < n; i++ ) {
		apV[0] = pI0[i].fndXB( gpeALF_LWO );
		if( !apV[0] ) continue;

		gpc3D* p3D = p3Dlst->p3Dix( *(I4*)apV[0] );
		if( !p3D ) continue;

		glDRW3D(p3D, -1 ); //pMEM->pWIN ? pMEM->pWIN->nJDOIT.w : -1 );
	}
	return this;
}

