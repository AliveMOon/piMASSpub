#ifndef GPCPIC_H
#define GPCPIC_H

#include "piMASS.h"

bool inline gpfSAVEjpg( U1* pFILE, SDL_Surface* pSRF, I4 q )
{
	struct jpeg_compress_struct cinfo;
	struct jpeg_error_mrg jerr;

	FILE* pOUT;
	JSAMPROW aROW[1];

	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_compress(&cinfo);
	if(!(pOUT = fopen(pFILE, "wb")))
		return false;

	jpeg_stdio_dest(&cinfo, pOUT);
    cinfo.image_width = pSRF->w;
    cinfo.image_height = pSRF->h;
    cinfo.input_components = 3;
    cinfo.in_color_space = JCS_RGB;

    jpeg_set_defaults(&cinfo);
    jpeg_set_quality( &cinfo, q, true );
    jpeg_start_compress(&cinfo,true);

	int rowSTRD = pSRF->w*3;
	while(cinfo.next_scanline < cinfo.image_height ) {
		aROW[0] = ((U1*)pSRF->pixels)+(cinfo.next_scanline*rowSTRD);
        jpeg_write_scanlines(&cinfo, aROW, 1 );
	}

    jpeg_finish_compress(&cinfo);
    fcolose(pFILE);
    jpeg_destroy_compress(&cinfo);

	return true;
}

class gpcPICAM
{
public:
	raspicam::RaspiCam	cam;
	I4x2				wh;
	gpcPICAM(){
		// Allowable values: RASPICAM_FORMAT_GRAY,RASPICAM_FORMAT_RGB,RASPICAM_FORMAT_BGR,RASPICAM_FORMAT_YUV420
		cam.setFormat(raspicam::RASPICAM_FORMAT_RGB);

		// Allowable widths: 320, 640, 1280
		// Allowable heights: 240, 480, 960
		// setCaptureSize(width,height)
		wh = I4x2( gpdRPI_WIDTH,gpdRPI_HEIGHT );
		cam.setCaptureSize( wh.x, wh.y );

		// Open camera
		//cout<<"Opening Camera..."<<endl;
		if ( !cam.open())
		{
			//cerr<<"Error opening camera"<<endl;return -1;
			return;
		}
		usleep(300*1000); // ? ez feltétlen kell
	};


};

class gpcPIC
{
public:
	I8x2			TnID, alfN;
	U1				sFILE[gpdMAX_PATH], *pFILE;
	U4				id, iSRC, iQC, nPIXall, nPIX, bppS;
	SDL_Surface		*pSRF;
	I4x4			xyOUT, xySRC;
	gpcPIC			*pSRC;

	U1		*pPIX;

	gpcPIC(){ gpmCLR; pFILE = sFILE; };
	gpcPIC( I8x2 an )
	{
		gpmCLR;
		TnID = an;
	}

	gpcPIC* BAYER( SDL_Surface *pSRF )
	{
		if( !pSRF )
			return this;

		if( !this )
		{
			gpcPIC* pTHIS = new gpcPIC;
			return pTHIS->BAYER( pSRF );
		}

		xyOUT.z = pSRF->w;
		xyOUT.w = pSRF->h;
		nPIX = xyOUT.a4x2[1].area()*4;
        if( nPIXall < nPIX )
        {
			nPIXall = nPIX;
			pPIX = new U1[nPIXall];
        }

		U1		*pS = (U1*)pSRF->pixels;
		U1x4	*pD = (U1x4*)pPIX;
		bppS = (U4)pSRF->format->BytesPerPixel;
		U4	piS = pSRF->pitch,
			piD = pSRF->w/2;

		switch(bppS) {
			case 1:
				pD[0] = U1x4( pS[0], ((U2)pS[1]+pS[piS])/2,  pS[piS+1], 0xff );

				break;

			case 2:
				//return *(Uint16 *)p;
				break;

			case 3:
				/*if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
					return p[0] << 16 | p[1] << 8 | p[2];
				else
					return p[0] | p[1] << 8 | p[2] << 16;*/
				break;

			case 4:
				//return *(Uint32 *)p;
				break;

			default:
				break;       /* shouldn't happen, but avoids warnings */
		}

		return this;
	}
	U1* getPIX( gpcPICAM* pC, U4 qc )
	{
		if( !this )
			return NULL;
		if( !pC )
			return pPIX;
		if( pC->cam.open() )
		{
			usleep(300*1000);
		}
		if( iQC >= qc )
			return pPIX;

        nPIX = pC->cam.getImageTypeSize( raspicam::RASPICAM_FORMAT_RGB );
		xyOUT.a4x2[1] = I4x2( pC->cam.getWidth(), pC->cam.getHeight() );

		if( nPIXall < nPIX )
		{
			nPIXall = nPIX;
			gpmDELary( pPIX );
			pPIX = new U1[nPIXall];
		}
		pC->cam.grab();
		// Extract the image
		pC->cam.retrieve(
							pPIX,
							raspicam::RASPICAM_FORMAT_IGNORE
										//RASPICAM_FORMAT_YUV420 );
										//RASPICAM_FORMAT_RGB );
						);

        I4x2& wh = xyOUT.a4x2[1];
        U4 rmask, gmask, bmask, amask;
		#if SDL_BYTEORDER == SDL_BIG_ENDIAN
		  int shift = (req_format == STBI_rgb) ? 8 : 0;
		  rmask = 0xff000000 >> shift;
		  gmask = 0x00ff0000 >> shift;
		  bmask = 0x0000ff00 >> shift;
		  amask = 0x000000ff >> shift;
		#else // little endian, like x86
		  rmask = 0x000000ff;
		  gmask = 0x0000ff00;
		  bmask = 0x00ff0000;
		  amask = 0; //(req_format == STBI_rgb) ? 0 : 0xff000000;
		#endif

		SDL_Surface	*pNEW = SDL_CreateRGBSurfaceFrom(
														pPIX, wh.x, wh.y, 24, wh.x*3,
														rmask, gmask, bmask, amask
													),
					*pGD = SDL_ConvertSurfaceFormat( pNEW, SDL_PIXELFORMAT_RGBA8888, 0);
		if( pNEW )
		{
			SDL_FreeSurface(pNEW);
			SDL_FreeSurface(pSRF);
			pSRF = pGD; //pNEW;
		}
		iQC = qc;
		return pPIX;
	}

};

class gpcPICall
{
	gpcPIC	**ppPIC, *pPIC;
	U4		nPICall, iPICfr, nPICld;
public:
	U4 iFND( U1* pS );

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
				gpmMEMCPY( ppPIC, ppKILL, iPICfr ); // mert nGTld == iGTfr+1
				gpmDELary(ppKILL);
			}
		}
		return ppPIC[iPICfr] = pPIC = new gpcPIC( an );
	}
};



#endif // GPCPIC_H
