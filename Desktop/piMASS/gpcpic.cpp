#include "gpcpic.h"
#include "gpcSRC.h"
bool gpfSRFjpgSAVE( U1* pFILE, SDL_Surface* pSRF, I4 q )
{
	if( pSRF ? (pFILE ? !*pFILE : false ) : true )
		return false;

	struct jpeg_compress_struct cinfo;
	struct jpeg_error_mgr jerr;

	FILE* pOUT;
	JSAMPROW aROW[1];

	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_compress(&cinfo);
	if( !(pOUT = fopen( (char*)pFILE, "wb")) )
		return false;

	jpeg_stdio_dest(&cinfo, pOUT);
    cinfo.image_width = pSRF->w;
    cinfo.image_height = pSRF->h;
    if( pSRF->format->BytesPerPixel != 3 )
		return false;

    cinfo.input_components = pSRF->format->BytesPerPixel; // 3;
    cinfo.in_color_space = JCS_RGB;

    jpeg_set_defaults(&cinfo);
    jpeg_set_quality( &cinfo, q, true );
    jpeg_start_compress(&cinfo,true);

	int rowSTRD = pSRF->pitch;
	while(cinfo.next_scanline < cinfo.image_height ) {
		aROW[0] = ((U1*)pSRF->pixels)+(cinfo.next_scanline*rowSTRD);
        jpeg_write_scanlines(&cinfo, aROW, 1 );
	}

    jpeg_finish_compress(&cinfo);
    fclose(pOUT);
    jpeg_destroy_compress(&cinfo);

	return true;
}

std::vector<JOCTET> jpgBUFF;
#define gpdJPGsiz 0x4000

void my_init_destination( j_compress_ptr pINF )
{
	jpgBUFF.resize(gpdJPGsiz);
	pINF->dest->next_output_byte = &jpgBUFF[0];
	pINF->dest->free_in_buffer = jpgBUFF.size();
}
boolean my_empty_output_buffer(j_compress_ptr pINF )
{
	size_t oSIZ = jpgBUFF.size();
	jpgBUFF.resize(oSIZ+gpdJPGsiz);
	pINF->dest->next_output_byte = &jpgBUFF[oSIZ];
	pINF->dest->free_in_buffer = jpgBUFF.size()-oSIZ;
	return true;
}
void my_term_destination(j_compress_ptr pINF )
{
	jpgBUFF.resize(jpgBUFF.size()-pINF->dest->free_in_buffer);
}

gpcLZY* gpfSRF2JPG( gpcLZY* pBUFF, SDL_Surface* pSRF, I4 q )
{
	if( !pSRF )
		return pBUFF;

	U8 s = -1, nMEM = 0;
	JSAMPROW aROW[1];
	struct jpeg_compress_struct INF;
	struct jpeg_error_mgr jerr;
	INF.err = jpeg_std_error(&jerr);
	struct jpeg_destination_mgr dst;
	//INF->dest = jpeg_stdio_dest( &dst );
	jpeg_create_compress(&INF);
	//jpeg_mem_dest(&INF, )


	if( !INF.dest )
		INF.dest = &dst;

	INF.dest->init_destination = &my_init_destination;
	INF.dest->empty_output_buffer = &my_empty_output_buffer;
	INF.dest->term_destination = &my_term_destination;

	INF.image_width = pSRF->w;
    INF.image_height = pSRF->h;
    if( pSRF->format->BytesPerPixel != 3 )
		return pBUFF;

	INF.input_components = pSRF->format->BytesPerPixel; // 3;
    INF.in_color_space = JCS_RGB;

    jpeg_set_defaults(&INF);
    jpeg_set_quality( &INF, q, true );
    jpeg_start_compress(&INF, true);

	int rowSTRD = pSRF->pitch;
	while(INF.next_scanline < INF.image_height ) {
		aROW[0] = ((U1*)pSRF->pixels)+(INF.next_scanline*rowSTRD);
        jpeg_write_scanlines(&INF, aROW, 1 );
	}

    jpeg_finish_compress(&INF);
	 pBUFF = pBUFF->lzyADD( &jpgBUFF[0], jpgBUFF.size(), s = -1 );
	/*U1	*pMEM;
	jpeg_mem_dest( &INF, pMEM, &nMEM );
    pBUFF = pBUFF->lzyADD( pMEM, nMEM, s = -1 );
    gpmDELary(pMEM);*/


	return pBUFF;
}
class gpcTHRD_CAM
{
public:
	gpcPICAM* pC;
	SDL_Surface* pSRF;
	gpcTHRD_CAM(){ gpmCLR; };
};
void call_cam( gpcTHRD_CAM* pTC )
{
	gpcTHRD_CAM cpy = *pTC;
	gpdCAMu& cam = cpy.pC->cam;
	U1* pPIX = cpy.pSRF ? (U1*)cpy.pSRF->pixels : NULL;
	while( cpy.pC )
	{
		usleep(1000/30);
		cam.grab();
		//memcpy ( cpy.pSRF->pixels, cpy.pC->cam.callback_data._buffData.data, getImageTypeSize( RASPICAM_FORMAT_IGNORE ) );
		cam.retrieve(
							pPIX,
							#ifdef gpdSYSpi
                                raspicam::RASPICAM_FORMAT_IGNORE
							#else
                                RASPICAM_FORMAT_IGNORE
							#endif
										//RASPICAM_FORMAT_YUV420 );
										//RASPICAM_FORMAT_RGB );
						);
	}
}

U1* gpcPIC::getPIX( gpcPICAM* pC, U4 qc )
{
	if( !this )
		return NULL;

	if( !pC )
		return pSRF ? (U1*)pSRF->pixels : NULL;

	if( pC->cam.open() )
	{
		usleep(3*1000);
	}
	if( iQC >= qc )
		return NULL; //pSRF ? (U1*)pSRF->pixels : NULL;

	if( bT )
	{
		iQC = qc+gpdRPI_tOUT;
		return pSRF ? (U1*)pSRF->pixels : NULL;
	}

	nPIX = pC->cam.getImageTypeSize(
                            #ifdef gpdSYSpi
                                raspicam::RASPICAM_FORMAT_RGB
							#else
                                RASPICAM_FORMAT_RGB
							#endif
							 );
	xyOUT.a4x2[1] = I4x2( pC->cam.getWidth(), pC->cam.getHeight() );

	if( nPIXall < nPIX )
	{
		nPIXall = nPIX;
		SDL_FreeSurface(pSRF);
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
		pSRF = SDL_CreateRGBSurface( 0, xyOUT.z, xyOUT.w, 24, rmask, gmask, bmask, amask );

		/*	SDL_CreateRGBSurfaceFrom(
										pPIX, xyOUT.a4x2[1].x, xyOUT.a4x2[1].y, 24, xyOUT.a4x2[1].x*3,
										0, 0, 0, 0
									);*/

	}

	if( pSRF )
	{
		gpcTHRD_CAM trd;
		trd.pC = pC;
		trd.pSRF = pSRF;

		if( !bT )
		{
			T = std::thread( call_cam, &trd );
			usleep(3*1000);
			bT = true;
		}
		if( !bT )
		{
			pC->cam.grab();
			pC->cam.retrieve(
								(U1*)pSRF->pixels,
								#ifdef gpdSYSpi
                                    raspicam::RASPICAM_FORMAT_IGNORE
                                #else
                                    RASPICAM_FORMAT_IGNORE
                                #endif
											//RASPICAM_FORMAT_YUV420 );
											//RASPICAM_FORMAT_RGB );
							);
		}

	}


	iQC = qc;
	return pSRF ? (U1*)pSRF->pixels : NULL;
}

U4 gpcPICall::alfFND( U1* pS )
{
	I8x2 TnID(0,14);
	TnID = pS;
	TnID.num = gpfSTR2U8( pS+TnID.num, &pS );

	for( U4 p = 0; p < nPICld; p++ )
	{
		if( !ppPIC[p] )
		{
			if( iPICfr > p )
				iPICfr = p;
			continue;
		}

		if( ppPIC[p]->TnID != TnID )
			continue;

		return p;
	}
	return nPICld;
}
