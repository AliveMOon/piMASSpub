#include "gpccrs.h"
#include "gpsGLSL.h"
typedef enum LWO_ID:U4
{
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
char gp_man_lws[] =
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
	"bone_head\n\0"
;
class gpc3Dblnd {
public:
	U1x4 ix, srf, part;
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
	gpc3Dblnd* SRFset( U1 s ) {
		if( this ? !s : true )
			return this;
		srf.u4 <<= 8;
		srf.x = s;
		return this;
	}
};
class gpc3Dvx {
public:
	F4 xyzi, up;
	F2 uv;
	gpc3Dvx(){}
	bool bad( gpc3Dvx& b, float x = 0.0, float u = 0.0 ) {
		if(!this)
			return false;

		if( (uv-b.uv).qlen() > u*u )
			return true;
		if( (xyzi-b.xyzi).qlen_xyz() > x*x )
			return true;

		return false;
	}
	bool good( gpc3Dvx& b, float x = 0.0, float u = 0.0 ) { return !bad(b,x,u); }
};
class gpc3Dmap {
public:
	gpcLZY pnt, fcl, fci;

	gpc3Dmap(	gpcLZY& p,
				gpcLZY& fl, gpcLZY& fi,
				gpcLZY& b,
				gpcLZY& t, U4x2 *pIX2,
				void* p_u, void* p_nx
			) {
		U1 *pUi = (U1*)p_u, *pUnx = (U1*)p_nx;
		fcl = fl;
		fci = fi;
		F4* pPs = (F4*)p.p_alloc, v0, v1,v2;
		F2* pTs = ((F2*)t.p_alloc)+pIX2->x;
		U4x2 *pF = (U4x2*)fl.p_alloc;
		U4	nP = p.nLD(sizeof(*pPs)), nF = fl.nLD(sizeof(*pF)),
			*pFi = (U4*)fi.p_alloc, ip, uf;
		gpc3Dblnd* pB = (gpc3Dblnd*)b.p_alloc;
		gpc3Dvx	tmP,
				*pPd = (gpc3Dvx*)pnt.Ux( nP, sizeof(tmP) )-nP, *pPip;
		for( U4 i = 0; i < nP; i++ ){
			pPd[i].xyzi = pPs[i];
			pPd[i].xyzi.uXYZW[3] = pB[i].ix.u4;
			pPd[i].uv = pTs[i];
		}
		for( U4 f = 0, i, ip, x,n; f < nF; f++ ){
			for( x = pF[f].x, n = pF[f].y, i = 0; i < n; i++ ) {
				ip = pFi[i+x];
				v0 = pPd[ip].xyzi;
				v1 = (pPd[pFi[((i-1+n)%n)+x]].xyzi - v0).N3();
				v2 = (pPd[pFi[((i+1)%n)+x]].xyzi - v0).N3();
				pPd[ip].up += v2.X3(v1).N3().xyz1();
			}
		}
		for( U4 p = 0; p < nP; p++ ) {
			pPd[p].up = pPd[p].up.N3();
		}

		while( pUi < pUnx ) {
			if( *pUi&0x80 ) {
				ip = swp4( pUi )&0x7FFFffff; pUi+= 4;
			} else {
				ip = swp2( pUi ); pUi+= 2;
			}
			if( *pUi&0x80 ) {
				uf = swp4( pUi )&0x7FFFffff; pUi+= 4;
			} else {
				uf = swp2( pUi ); pUi+= 2;
			}
			pF = (U4x2*)fcl.Ux( uf, sizeof(*pF) );
			pFi = (U4*)fci.Ux( pF->sum()+pIX2->x, sizeof(*pFi) ) - pF->y;
			for( U4 i = 0, t, te; i < pF->y; i++ ) {
				if( pFi[i] != ip )
					continue;
				pPd = (gpc3Dvx*)pnt.Ux( ip, sizeof(tmP) );
				tmP = *pPd;
				tmP.uv.swpXY( pUi ); pUi += sizeof(F2);
				for( t = p.nLD(sizeof(tmP)), te = pnt.nLD(sizeof(tmP)); t < te; t++ ) {
					if( ((gpc3Dvx*)pnt.Ux( pFi[i], sizeof(*pPd) ))->good(tmP, 1.0/256.0, 1.0/256.0 )  )
						break;
				}

				if( pFi[i] == t )
					break;
				pFi[i] = t;
				if( t < te )
					break;


				pPip = (gpc3Dvx*)pnt.Ux( te, sizeof(*pPd) );
				*pPip = tmP;


				break;
			}
		}

	}
};

class gpc3Dly {
public:
	char	sNAME[0x100];
	I4x4	id;
	gpcLZY	pnt, blnd, tx,
			fcLST, fcIX, fcSUM, srSUM, prSUM,
			bon;
	gpcLZYdct	mapDCTmn;
	gpcLZYdct	mapDCTwg; gpcLZY mapIXwg, mapBLwg;
	gpcLZYdct	mapDCTtx; gpcLZY mapIXtx, mapF2tx, MAP;
	gpcLZYdct	mapDCTmr; gpcLZY mapIXmr, mapBLmr;
	gpcLZYdct	mapDCTsr;
	gpcLZYdct	mapDCTpt; gpcLZY mapIXprt;

	F4 piv, bbox[2];
	gpc3Dly(){
		gpmCLR;
	}
};
class gpc3D {
public:
	char sPATH[gpdMAX_PATH];
	I8x4 id;
	gpcLZY	*p_lwo,
			ly3D,
			tgIX;

	~gpc3D(){
		gpmDEL(p_lwo);
	}
	gpc3D( I4 i, const char* pP, gpeALF alf ) {
		gpmCLR;
		U8 nLEN;
		id.x = i;
		id.a8x2[0].b = alf;
		id.z = pP ? gpmVAN(pP," \t\r\n\"\a",nLEN) : 0;
		gpmSTRCPY( sPATH, pP );
	}
	gpc3D* pLWO( gpcLZY& lwo, gpcLZYdct& dctBN );
};
class gpc3Dlst {
public:
	gpcLZY lst3D;
	gpcLZYdct bonLST;

	~gpc3Dlst(){
		gpc3D	**pp3D = (gpc3D**)lst3D.Ux( 0, sizeof(gpc3D*) );
		I4 i3D = 0, e3D = lst3D.nLD(sizeof(gpc3D*));
		for( I4 n3D = e3D; i3D < n3D; i3D++ ) {
			gpmDEL( pp3D[i3D] );
		}
	}
	gpc3Dlst(){
		gpmCLR;
		U8 nLEN;
		char* pS = gp_man_lws, *pSe;
		while( *pS ) {
			pSe = pS+gpmVAN( pS, "\r\n", nLEN );
			bonLST.dctADD( pS, pSe-pS );
			pS = pSe+gpmNINCS(pSe, "\r\n" );
		}
	}
	gpc3D* p3D( I4 i ) { return ((gpc3D**)lst3D.Ux( i, sizeof(gpc3D*) ))[0]; }
	gpc3D* p3D( gpeALF alf, const char* pP ) {
		if( !this )
			return NULL;

		gpc3D	**pp3D = (gpc3D**)lst3D.Ux( 0, sizeof(gpc3D*) );
		I4 i3D = 0, e3D = lst3D.nLD(sizeof(gpc3D*));
		for( I4 n3D = e3D; i3D < n3D; i3D++ ) {
			if( !pp3D[i3D] ) {
				if( e3D > i3D )
					e3D = i3D;
				continue;
			}
			if( pp3D[i3D]->id.a8x2[0].a != alf )
				continue;
			return pp3D[i3D];
		}
		pp3D = (gpc3D**)lst3D.Ux( e3D, sizeof(gpc3D*) );
		return (*pp3D) = new gpc3D( e3D, pP, alf );
	}
};

F2& F2::swpXY( const void* pV ) {
	((U1x4*)&x)->wzyx(pV,2);
	return *this;
}
F2& F2::sXY( const char* p_str, char** pp_str ) {
	U8 nLEN;
	gpmCLR;
	const char* pPAT = "+-0123456789.,";
	p_str += gpmVAN( p_str, pPAT, nLEN );
	x = gpmSTR2D(p_str);
	if( *p_str == ',' )
		p_str++;

	p_str += gpmVAN( p_str, pPAT, nLEN );
	y = gpmSTR2D(p_str);
	if( *p_str == ',' )
		p_str++;

	if( !pp_str )
		return *this;
	(*pp_str) = (char*)p_str;
	return *this;
}

F4& F4::swpXYZ0( const void* pV ) {
	((U1x4*)&x)->wzyx(pV,3);
	return *this;
}

F4& F4::sXYZW( const char* p_str, char** pp_str ) {
	U8 nLEN;
	gpmCLR;
	const char* pPAT = "+-0123456789.,";
	p_str += gpmVAN( p_str, pPAT, nLEN );
	x = gpmSTR2D(p_str);
	if( *p_str == ',' )
		p_str++;

	p_str += gpmVAN( p_str, pPAT, nLEN );
	y = gpmSTR2D(p_str);
	if( *p_str == ',' )
		p_str++;

	p_str += gpmVAN( p_str, pPAT, nLEN );
	z = gpmSTR2D(p_str);
	if( *p_str == ',' )
		p_str++;

	p_str += gpmVAN( p_str, pPAT, nLEN );
	w = gpmSTR2D(p_str);
	if( *p_str == ',' )
		p_str++;
	if( !pp_str )
		return *this;
	(*pp_str) = (char*)p_str;
	return *this;
}

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
	U4 n = swp4(pU4i), chunk, nUi, ix, nx, ip, uf;
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
	I4x2* pTG;
	while( pUnx < pUe ){
		pU4i = (U4*)pUnx;
		chunk = *pU4i; pU4i++;
		n = swp4(pU4i); pU4i++;


		pUi = (char*)pU4i;
		pUnx = pUi+n;
		switch( chunk ) {
			case LWO_ID_TAGS: {

					while( pUi < pUnx ) {
						pTG = (I4x2*)tgIX.Ux( tgIX.nLD(sizeof(I4x2)), sizeof(I4x2) );
						pTG->x = pUi-pU0;
						pTG->y = gpmSTRLEN(pUi);
						pUi += gpmPAD( pTG->y+1,2 );
					}
					pTG = (I4x2*)tgIX.Ux( 0, sizeof(I4x2) );
				} break;
			case LWO_ID_LAYR: {
					if( pLY ) {

					}
					pLY = (gpc3Dly*)ly3D.Ux( ly3D.nLD(sizeof(gpc3Dly)), sizeof(gpc3Dly) );
					pLY->id.w = swp4( pU4i ); pU4i++;
					pLY->id.x = pLY->id.w&0xffff;	// momID
					pLY->id.y = pLY->id.w>>16;		// layID


					pLY->piv.swpXYZ0( pU4i ); pU4i+=3;
					pUi = (char*)pU4i;
					if( pUi < pUnx )
						gpmSTRCPY( pLY->sNAME, pUi );
				} break;
			case LWO_ID_PNTS: {
					n = (pUnx-pUi)/(sizeof(float)*3);
					F4* pP0 = ((F4*)pLY->pnt.Ux( n, sizeof(F4) ))-n;
					for( U4 i = 0; i < n; i++ )
						pP0[i].swpXYZ0( pU4i + i*3 );
				} break;
			case LWO_ID_BBOX: {
					pLY->bbox[0].swpXYZ0( pU4i ); pU4i+=3;
					pLY->bbox[1].swpXYZ0( pU4i ); pU4i+=3;
				} break;
			case LWO_ID_POLS: {
					pUi += 4;
					switch( *pU4i ) {
						case LWO_ID_FACE: {
								U4x2* pFl;
								U4 *pFs,*pFi;
								while( pUi < pUnx ) {
									pFl = (U4x2*)pLY->fcLST.Ux( pLY->fcLST.nLD(sizeof(*pFl)), sizeof(*pFl) );
									pFl->x = pLY->fcIX.nLD(sizeof(U4));
									pFl->y = swp2(pUi)&0x03ff; pUi+=2;

									pFs = (U4*)pLY->fcSUM.Ux( pFl->y, sizeof(*pFs) );
									(*pFs)++;

									pFi = (U4*)pLY->fcIX.Ux( pFl->sum(), sizeof(*pFi) ) - pFl->y;
									for( U4 i = 0; i < pFl->y; i++ ) {
										if( *pUi&0x80 ) {
											pFi[i] = swp4( pUi )&0x7FFFffff; pUi+= 4;
										} else {
											pFi[i] = swp2( pUi ); pUi+= 2;
										}
									}
								}
								std::cout << std::endl;
								pFs = (U4*)pLY->fcSUM.Ux( 0, sizeof(*pFs) );
								for( U4 i = 0, e = pLY->fcSUM.nLD(sizeof(*pFs)); i < e; i++ ) {
									if( !pFs[i] )
										continue;
									if(bSTDcout){ std::cout << i << " " << pFs[i] << std::endl; }

								}

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
								gpc3Dblnd* pBl;
								/// megreressük melyik csont a listában
								nUi = gpmSTRLEN(pUi);
								U4 iBn, iBx = dctBN.dctFND( pUi, nUi, iBn );
								if( iBx >= iBn )
									break;

								ix = pLY->mapDCTwg.nIX();
								pLY->mapDCTwg.dctADD( pUi, nUi );
								pUi += gpmPAD( nUi+1,2 );
								if(ix>=pLY->mapDCTwg.nIX())
									break;

								U4x2 *pIX2 = (U4x2*)pLY->mapIXwg.Ux( ix, sizeof(*pIX2) );
								pIX2->x = iBx;
								pIX2->y = 0;
								while( pUi < pUnx ) {
									if( *pUi&0x80 ) {
										ip = swp4( pUi )&0x7FFFffff; pUi+= 4;
									} else {
										ip = swp2( pUi ); pUi+= 2;
									}
									pBl = (gpc3Dblnd*)pLY->mapBLwg.Ux( ip, sizeof(*pBl) );
									uf = swp4( pUi );
									if(uf)
										pIX2->y++;
									pBl->BLset( iBx, *(float*)&uf ); pUi+= 4;
								}

								std::cout << pLY->mapDCTwg.sSTRix(ix,"ERR")  << " iBx:" << pIX2->x << " n:" << pIX2->y << std::endl;
							} break;
						case LWO_ID_TXUV: {
								F2* pTX;
								/// megreressük melyik csont a listában
								ix = pLY->mapDCTtx.nIX();
								nUi = gpmSTRLEN(pUi);
								pLY->mapDCTtx.dctADD( pUi, nUi );
								pUi += gpmPAD( nUi+1,2 );
								if( ix >= pLY->mapDCTtx.nIX() )
									break;

								U4x2 *pIX2 = (U4x2*)pLY->mapIXtx.Ux( ix, sizeof(*pIX2) );
								pIX2->x = pLY->mapF2tx.nLD(sizeof(*pTX));
								while( pUi < pUnx ) {
									if( *pUi&0x80 ) {
										ip = swp4( pUi )&0x7FFFffff; pUi+= 4;
									} else {
										ip = swp2( pUi ); pUi+= 2;
									}
									pTX = (F2*)pLY->mapF2tx.Ux( ip, sizeof(*pTX) );
									pTX->swpXY( pUi ); pUi += sizeof(*pTX);
								}
								pIX2->y = pLY->mapF2tx.nLD(sizeof(*pTX))-pIX2->x;
								std::cout << pLY->mapDCTtx.sSTRix(ix,"ERR") << " iTX:" << pIX2->x << " nTX:" << pIX2->y << std::endl;
							} break;
						case LWO_ID_MORF: {
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
								nUi = gpmSTRLEN(pUi);
								ix = pLY->mapDCTtx.dctFND( pUi, nUi, nx );
								if( ix >= nx )
									break;
								pUi += gpmPAD( nUi+1,2 );
								U4x2 *pIX2 = (U4x2*)pLY->mapIXtx.Ux( ix, sizeof(*pIX2) );

								gpc3Dmap** ppM = (gpc3Dmap**)pLY->MAP.Ux( ix, sizeof(*ppM) );
								gpmDEL( *ppM );
								pMAP =
								(*ppM) = new gpc3Dmap(	pLY->pnt,
														pLY->fcLST, pLY->fcIX,
														pLY->mapBLwg,
														pLY->mapF2tx, pIX2,
														pUi, pUnx
													);
								if( !pMAP )
									break;
								std::cout << pLY->mapDCTtx.sSTRix(ix,"ERR") << " nP:" << pLY->pnt.nLD(sizeof(F4)) << " nPm:" << pMAP->pnt.nLD(sizeof(gpc3Dvx)) << std::endl;
							} break;
						case LWO_ID_MORF: {
							} break;
						default: break;
					}
				} break;
			case LWO_ID_CLIP: {
					pUi += 4;
					if( pU4i[1] != LWO_ID_STIL )
						break;

				} break;
			case LWO_ID_SURF: {
					pUi += 4;
					break;
					while( pUi < pUnx ) {

					}
				} break;
			case LWO_ID_PTAG: {
					pUi += 4;
					switch( *pU4i )
					{
						case LWO_ID_COLR:
							break;
						case LWO_ID_SURF: {
								gpc3Dblnd* pBl;
								U4x2* pF;
								U4 iF, *pFi, nF = 0;
								while( pUi < pUnx ) {
									iF = swp2( pUi ); pUi += 2;
									if( *pUi&0x80 ) {
										ix = swp4( pUi )&0x7FFFffff; pUi+= 4;
									} else {
										ix = swp2( pUi ); pUi+= 2;
									}
									(*(U4*)pLY->srSUM.Ux( ix, sizeof(U4) ))++;

									pF = (U4x2*)pLY->fcLST.Ux( iF, sizeof(*pF) );
									pFi = (U4*)pLY->fcIX.Ux( pF->sum(), sizeof(*pFi) ) - pF->y;
									for( U4 i = 0; i < pF->y; i++ ) {
										pBl = (gpc3Dblnd*)pLY->mapBLwg.Ux( pFi[i], sizeof(*pBl) );
										pBl->SRFset( ix );
									}
									nF++;
								}
								for( U4 i = 0, e = pLY->srSUM.nLD(sizeof(U4)); i < e; i++ ){
									std::cout << i << " " << (char*)(pU0+pTG[i].x) << " " << *(U4*)(pLY->srSUM.Ux(i,sizeof(U4))) << std::endl;
								}
							} break;
						case LWO_ID_PART: {
								U4x2* pF, *pPRT;
								U4 iF, *pFi, nF = 0;
								while( pUi < pUnx ) {
									iF = swp2( pUi ); pUi += 2;
									if( *pUi&0x80 ) {
										ix = swp4( pUi )&0x7FFFffff; pUi+= 4;
									} else {
										ix = swp2( pUi ); pUi+= 2;
									}
									(*(U4*)pLY->prSUM.Ux( ix, sizeof(U4) ))++;

									pPRT = (U4x2*)pLY->mapIXprt.Ux( nF, sizeof(U4) );
									pPRT->y = ix;
									pPRT->x = nF;
									nF++;
								}

								for( U4 i = 0, e = pLY->prSUM.nLD(sizeof(U4)); i < e; i++ ){
									std::cout << i << " " << (char*)(pU0+pTG[i].x) << " " << *(U4*)(pLY->prSUM.Ux(i,sizeof(U4))) << std::endl;
								}
							} break;
						default: break;
					}
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
	I4 id = -1;

	if( !p3Dlst )
		p3Dlst = new gpc3Dlst;

	gpc3D* p3D = p3Dlst->p3D( a, pPATH )->pLWO(rd, p3Dlst->bonLST );
	return id;
}
