#include "gpcpic.h"
#include "gpcSRC.h"
bool gpfSAVEjpg( U1* pFILE, SDL_Surface* pSRF, I4 q )
{
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
    fclose(pOUT);
    jpeg_destroy_compress(&cinfo);

	return true;
}

U4 gpcPICall::iFND( U1* pS )
{
	I8x2 alfN(0,14);
	alfN = pS;
	alfN.num = gpfSTR2U8( pS+alfN.num, &pS );

	for( U4 p = 0; p < nPICld; p++ )
	{
		if( !ppPIC[p] )
		{
			if( iPICfr > p )
				iPICfr = p;
			continue;
		}

		if( ppPIC[p]->alfN != alfN )
			continue;

		return p;
	}
	return nPICld;
}
