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

class gpc3D {
public:
	char sPATH[gpdMAX_PATH];
	I8x4 id;
	gpcLZY	*p_lwo;
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
	gpc3D* pLWO( gpcLZY& lwo );
};
class gpc3Dlst {
public:
	gpcLZY lst3D;

	~gpc3Dlst(){
		gpc3D	**pp3D = (gpc3D**)lst3D.Ux( 0, sizeof(gpc3D*) );
		I4 i3D = 0, e3D = lst3D.nLD(sizeof(gpc3D*));
		for( I4 n3D = e3D; i3D < n3D; i3D++ ) {
			gpmDEL( pp3D[i3D] );
		}
	}
	gpc3Dlst(){ gpmCLR; }
	gpc3D* p3D( I4 i ) { return ((gpc3D**)lst3D.Ux( i, sizeof(gpc3D*) ))[0]; }
	gpc3D* p3D(  gpeALF alf, const char* pP ) {
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
inline U4 swp4( const void* p ) {
	register U4 u = *(U4*)p;
	return    (u<<24)
			|((u&0xFF000000)>>24)
			|((u&0xFF0000)>>8)
			|((u&0xFF00)<<8);
}
inline void* pad2( const void* p ) {
		register size_t u = (size_t)(char*)p;
		return (void*)((u&1) ? (u+1) : u );
}
gpc3D* gpc3D::pLWO( gpcLZY& lwo ) {
	if( !this )
		return NULL;
	U8 s;
	if( !lwo.nLD() )
		return this;
	U4* pU4 = (U4*)lwo.p_alloc, *pU4i = pU4;
	if( *pU4i != LWO_ID_FORM )
		return this;
	pU4i++;
	U4 n = swp4(pU4i);
	pU4i++;
	if( *pU4i != LWO_ID_LWO2 )
		return this;

	I8 i8w = lwo.nSUM();
	if( id.w == i8w )
		return this;


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

	gpc3D* p3D = p3Dlst->p3D( a, pPATH )->pLWO(rd);
	return id;
}
