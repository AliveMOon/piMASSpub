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

gpcLAZY* gpfSRF2JPG( gpcLAZY* pBUFF, SDL_Surface* pSRF, I4 q )
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
	 pBUFF = pBUFF->lzy_add( &jpgBUFF[0], jpgBUFF.size(), s = -1 );
	/*U1	*pMEM;
	jpeg_mem_dest( &INF, pMEM, &nMEM );
    pBUFF = pBUFF->lzy_add( pMEM, nMEM, s = -1 );
    gpmDELary(pMEM);*/


	return pBUFF;
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
