#ifndef GPCPIC_H
#define GPCPIC_H

#include "piMASS.h"

//#include "gpcSRC.h"
//GLOVAL(void)
bool gpfSRFjpgSAVE( U1* pFILE, SDL_Surface* pSRF, I4 q );
gpcLZY* gpfSRF2JPG( gpcLZY* pBUFF, SDL_Surface* pSRF, I4 q );
#ifndef gpdSYSpi


class gpcCAMubi
{
    U1 frm;
    U4x2 wh;
    U4 nGRB;
    bool bOPEN;
public:
    void setFormat( RASPICAM_FORMAT f )
    {
        frm = f;
    }
    void setCaptureSize( U4 w, U4 h )
    {
        wh.x = w;
        wh.y = h;
    }
    bool open()
    {
        if( bOPEN )
            return bOPEN;

        return bOPEN = true;
    }
    U4 getImageTypeSize( RASPICAM_FORMAT f )
    {
        return 0;
    }
    U4 getWidth()
    {
        return wh.x;
    }
    U4 getHeight()
    {
        return wh.y;
    }
    U4 grab()
    {
       return nGRB++;
    }
		// Extract the image
    void* retrieve( void *pPIX, RASPICAM_FORMAT f )
    {
        return pPIX;
    }
    gpcCAMubi(){ gpmCLR; };
};
#endif

class gpcPICAM
{
	bool bCAMU;
public:
	gpdCAMu	cam;
	I4x2 wh;
	gpcPICAM(){
		// Allowable values: RASPICAM_FORMAT_GRAY,RASPICAM_FORMAT_RGB,RASPICAM_FORMAT_BGR,RASPICAM_FORMAT_YUV420

		#ifdef gpdSYSpi
			bCAMU = false;
			cam.setFormat(raspicam::RASPICAM_FORMAT_RGB);
		#else
			bCAMU = true;
			cam.setFormat( RASPICAM_FORMAT_RGB );
		#endif

		// Allowable widths: 320, 640, 1280
		// Allowable heights: 240, 480, 960
		// setCaptureSize(width,height)
		wh = I4x2( gpdRPI_WIDTH,gpdRPI_HEIGHT );
		cam.setCaptureSize( wh.x, wh.y );

		// Open camera
		//cout<<"Opening Camera..."<<endl;
		if( !cam.open() )
		{
			//cerr<<"Error opening camera"<<endl;return -1;
			return;
		}
		usleep(300*1000); // ? ez feltétlen kell
	};
	bool bGD()
	{
		return !bCAMU;
	}

};

class gpcPIC
{
public:
	I8x2			TnID, alfN;
	U1				sFILE[gpdMAX_PATH], *pFILE;
	gpcLZY			*pPACK;
	U4				id, iSRC, aiQC[2], nPIXall, nPIX,
					bppS, nPKavg;

	SDL_Surface		*pSRF, *pSHR, *pREF;

	SDL_Texture		*pTX, *pRTX;
	U1x4			*pLOCK;

	I4x4			xyOUT, xySRC, txWH;
	gpcPIC			*pSRC;
	bool			bTHRD;
	std::thread		T;
	//U1		*pPIX;

	gpcPIC(){ gpmCLR; pFILE = sFILE; };
	gpcPIC( I8x2 an, U4 i )
	{
		gpmCLR;
		id = i;
		TnID = an;
	}

	U1* getPIX()
	{
		if( !this )
			return NULL;

		return pSRF ? (U1*)pSRF->pixels : NULL;
	}
	U1* getPIX( gpcPICAM* pC, U4 qc );

	SDL_Surface* surDRW()
	{
		if( !pSHR )
			return pSRF;

		if( pSRF ? aiQC[0] <= aiQC[1] : true )
			return pSHR;

		return pSRF;
	}
	bool unLOCK( void ) {
		if( this ? (!pLOCK||!pTX) : true )
			return false;


		SDL_UnlockTexture(pTX);
		pLOCK = NULL;
		return true;
	}
	U1x4* u1x4LOCK( SDL_Renderer* pRNDR, U4x2 wh, int& ptch ) {
		if( !this )
			return NULL;
		if( pLOCK )
			return pLOCK;

		ptch = 0;

		if( pTX )
		{
			int w = 0, h = 0, acc = 0;
			U4 frm;
			SDL_QueryTexture( pTX, &frm, &acc, &w, &h );
			if(    SDL_PIXELFORMAT_RGBA8888  != frm
				|| wh.x != w || wh.y != h
				|| acc != SDL_TEXTUREACCESS_STREAMING )
				gpmSDL_FreeTX(pTX);
		}
		if( !pTX )
		{
			pTX = SDL_CreateTexture( pRNDR, SDL_PIXELFORMAT_RGBA8888,
											SDL_TEXTUREACCESS_STREAMING,
											 wh.x, wh.y );
			txWH.a4x2[1] = wh;
		}


		if( pTX ? SDL_LockTexture( pTX, NULL, (void**)&pLOCK, &ptch ) : true )
		{
			pLOCK = NULL;
			return NULL;
		}
		ptch /= sizeof(*pLOCK);
		return pLOCK;
	}

	SDL_Texture* surDRWtx( SDL_Renderer* pRNDR ) {
		SDL_Surface* pS = surDRW(); /*pREF;
		if( pSHR )
		{
			if( pSRF )
			{
				if( aiQC[0] > aiQC[1] )
					pS = pSRF;
				else
					pS = pSHR;
			} else
				pS = pSHR;
		} else if( pSRF )
			pS = pSRF;*/

		if( pREF == pS )
			return pTX;


		int //w = 0, h = 0,
			acc = 0;
		U4 frm;
		pREF = pS;

		if( pREF ? pTX : NULL )
		{
			SDL_QueryTexture( pTX, &frm, &acc, &txWH.z, &txWH.w );
			if( (pREF->format ? pREF->format->format : 0)  != frm
				|| pREF->w != txWH.z || pREF->h != txWH.w
				|| acc != SDL_TEXTUREACCESS_STREAMING )
			{
				gpmSDL_FreeTX(pTX);
				pTX = SDL_CreateTexture( pRNDR, pREF->format->format,
												SDL_TEXTUREACCESS_STREAMING,
												pREF->w, pREF->h );
				txWH.z = pREF->w;
				txWH.w = pREF->h;
			}

			if( pTX )
			{
				void* pLOCK;
				int pitch;
				if( SDL_LockTexture( pTX, NULL, &pLOCK, &pitch ) )
					return pTX;

				memcpy( pLOCK, pREF->pixels, pitch*pREF->h );
				SDL_UnlockTexture(pTX);
			}
			return pTX;
		}

		return pTX = SDL_CreateTextureFromSurface( pRNDR, pREF );
	}
};

class gpcPICall
{
	gpcPIC	**ppPIC, *pPIC;
	U4		nPICall, iPICfr, nPICld;
public:
	U4		alfFND( U1* pS );
	gpcPIC*	aluFND( gpcALU& alu );
//	{
//		U4 i =	alu.bSTR() ?
//				PIC.alfFND( (U1*)alu.pDAT ) 	// ez a kép neve
//				: alu.u8();
//
//	}
	gpcPIC*	PIC( U4 i )
	{
		if( pPIC ? pPIC->id == i : false )
			return pPIC;

		if( i < nPICld )
		{
			if( ppPIC[i] )
				return pPIC = ppPIC[i];
		}
		return NULL;
	}
	gpcPIC*	PIC( I8x2 an )
	{
		if( pPIC )
		if( pPIC->TnID == an )
			return pPIC;

		pPIC = NULL;
		if( iPICfr > nPICld )
			iPICfr = nPICld;
		else if( !ppPIC )
			iPICfr = nPICld = nPICall = 0;
		else if( ppPIC[iPICfr] )
			iPICfr = nPICld;

		for( U4 p = 0; p < nPICld; p++ )
		{
			if( !ppPIC[p] )
			{
				if( iPICfr > p )
					iPICfr = p;
				continue;
			}

			if( ppPIC[p]->TnID != an )
				continue;

			pPIC = ppPIC[p];
			return pPIC;
		}
		if( iPICfr >= nPICld )
		{
			nPICld++;
			if( nPICld >= nPICall )
			{
				nPICall += 0x10;
				gpcPIC	**ppKILL = ppPIC;
				ppPIC = new gpcPIC*[nPICall];
				gpmMcpyOF( ppPIC, ppKILL, iPICfr ); // mert nGTld == iGTfr+1
				gpmDELary(ppKILL);
			}
		}
		return ppPIC[iPICfr] = pPIC = new gpcPIC( an, iPICfr );
	}
};



#endif // GPCPIC_H
