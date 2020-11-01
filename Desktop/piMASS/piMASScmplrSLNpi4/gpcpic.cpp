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

#ifdef gpdSYSubi
//#ifndef gpdSYSpi

U4 gpcCAMubi::getImageTypeSize( RASPICAM_FORMAT f )
{
    if( f != RASPICAM_FORMAT_RGB )
        return 0;

    return queryBuffer.length; //bufferinfo.bytesused;
}

bool gpcCAMubi::openCAM()
{
    if( bOPEN )
        return bOPEN;

    ///  1. Open the device ---------------
    fd = open("/dev/video0",O_RDWR);
    if(fd < 0){
        perror("Failed to open device, OPEN");
        return bOPEN;
    }

    //  2. Ask the device if it can capture frames
    if(ioctl(fd, VIDIOC_QUERYCAP, &capability) < 0){
        // something went wrong... exit
        perror("Failed to get device capabilities, VIDIOC_QUERYCAP");
        return bOPEN;
    } else {
         std::cout << "driver:" << (char*)capability.driver << std::endl;
    }

    bool bGD = false;
    /// 3. ------------------------------------------------------
    fmtdesc.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    int r = ioctl(fd,VIDIOC_ENUM_FMT,&fmtdesc);
    while( r > -1 ) {
        std::cout << fmtdesc.description << std::endl;
        frmsize.type = fmtdesc.type;
        frmsize.pixel_format = fmtdesc.pixelformat;
        frmsize.index = 0;
        /// 4. ------------------------------------------------------
        r = ioctl(fd, VIDIOC_ENUM_FRAMESIZES, &frmsize);
        while( r > -1 ) {
            if(frmsize.type == V4L2_FRMSIZE_TYPE_DISCRETE)
                std::cout   << "discrete w:" << frmsize.discrete.width << " h:" << frmsize.discrete.height << std::endl;
            else  {
                std::cout   << "-------- w:" << frmsize.stepwise.min_width << " h:" << frmsize.stepwise.min_height << std::endl
                            << "-------- w:" << frmsize.stepwise.max_width << " h:" << frmsize.stepwise.max_height << std::endl;
                if( frmsize.pixel_format == V4L2_PIX_FMT_RGB24 )
                {
                    wh0.x = gpmMAX( frmsize.stepwise.min_width,     gpmMIN(wh0.x,frmsize.stepwise.max_width) );
                    wh0.y = gpmMAX( frmsize.stepwise.min_height,    gpmMIN(wh0.y,frmsize.stepwise.max_height) );
                    bGD = true;
                    break;
                }
            }
            if( bGD )
                break;
            frmsize.index++;
            r = ioctl(fd, VIDIOC_ENUM_FRAMESIZES, &frmsize);
        }
        if( bGD )
            break;

        fmtdesc.index++;
        r = ioctl(fd,VIDIOC_ENUM_FMT,&fmtdesc);
    }



    if( !bGD )
    {
        if( fd > -1 )
            close(fd);

        return bOPEN = true;
    }

    bGD = ( setCaptureSize( wh0.x, wh0.y, frmsize.pixel_format ) > -1 );
    if( !bGD )
        return bOPEN;

    grab();

    return bOPEN = true;
}
int gpcCAMubi::setCaptureSize( U4 w, U4 h, U4 picFRM ) {
    if( wh == wh0 )
        return 0;

    /// 5. Set Image format
    std::cout << "5. Set Image format ----------" << std::endl;
    format.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    format.fmt.pix.width = w;
    format.fmt.pix.height = h;
    format.fmt.pix.pixelformat = picFRM;
    format.fmt.pix.field = V4L2_FIELD_NONE;
    int res = ioctl(fd, VIDIOC_S_FMT, &format);
    if( res < 0){
        return res;
    }

    if( pBUFF ) {
        munmap(pBUFF, queryBuffer.length);
    }

    /// 6. VIDIOC_REQBUFS ----------
    std::cout << "6. VIDIOC_REQBUFS ----------" << std::endl;
    requestBuffer.count = 1;                    // one request buffer
    requestBuffer.type = format.type ;          // request a buffer wich we an use for capturing frames
    requestBuffer.memory = V4L2_MEMORY_MMAP;
    res = ioctl(fd, VIDIOC_REQBUFS, &requestBuffer);
    if( res < 0){
        return res;
    }

    std::cout << "7. VIDIOC_QUERYBUF ----------------" << std::endl;
    /// 7. VIDIOC_QUERYBUF ----------
    queryBuffer.type = format.type ;
    queryBuffer.memory = V4L2_MEMORY_MMAP;
    queryBuffer.index = 0;
    res = ioctl(fd, VIDIOC_QUERYBUF, &queryBuffer);
    if( res < 0){
        return res;
    }

    pBUFF = (char*)mmap(    NULL, queryBuffer.length,
                            PROT_READ | PROT_WRITE,
                            MAP_SHARED,
                            fd, queryBuffer.m.offset    );

     /// 8. VIDIOC_STREAMON ----------------
    std::cout << " 8. VIDIOC_STREAMON ----------------" << std::endl;
    type = format.type;
    res = ioctl(fd, VIDIOC_STREAMON, &type);
    if( res < 0 ){
        return res;
    }


    wh = wh0;
    return res;
}

U4 gpcCAMubi::grab()
{
    //if( !bOPEN )
    //   return nGRB;
    std::cout << "grab" << std::endl;
    int res;
    /// 9. VIDIOC_QBUF ----------------
    bufferinfo.type = type;
    bufferinfo.memory = V4L2_MEMORY_MMAP;
    bufferinfo.index = 0;
    std::cout << " 9. VIDIOC_QBUF ----------------" << std::endl;
    res = ioctl(fd, VIDIOC_QBUF, &bufferinfo);
    if( res < 0){
        return res;
    }
    /// 10. VIDIOC_DQBUF --------------------
    std::cout << "10. VIDIOC_DQBUF --------------------" << std::endl;
    res = ioctl(fd, VIDIOC_DQBUF, &bufferinfo);
    if( res < 0){
        return nGRB++;
    }
    std::cout   << "Buffer has: " << (double)bufferinfo.bytesused / 1024
                << " KBytes of data" << std::endl;

    return nGRB++;
}
void* gpcCAMubi::retrieve( void *pPIX, RASPICAM_FORMAT f )
{
    std::cout << "CPY" << std::endl;
    int res ;
    gpmMcpy(pPIX, pBUFF, queryBuffer.length );
    return pPIX;

    /// 9.2 VIDIOC_QBUF ----------------
    bufferinfo.type = type;
    bufferinfo.memory = V4L2_MEMORY_MMAP;
    bufferinfo.index = 0;
    std::cout << " 9.2 VIDIOC_QBUF ----------------" << std::endl;
    res = ioctl(fd, VIDIOC_QBUF, &bufferinfo);
    if( res < 0){
        return pPIX;
    }

    return pPIX;

    /// 11. VIDIOC_STREAMOFF --------------------
    std::cout << "11. VIDIOC_STREAMOFF --------------------" << std::endl;
    type = bufferinfo.type;
    res = ioctl(fd, VIDIOC_STREAMOFF, &type);
    if( res < 0 ){
        perror("Could not end streaming, VIDIOC_STREAMOFF");
    }
    return pPIX;
}

#endif // gpdSYSpi

class gpcTHRD_CAM {
public:
	gpcPICAM* pC;
	SDL_Surface* pSRF;
	gpcTHRD_CAM(){ gpmCLR; };
};
void call_cam( gpcTHRD_CAM* pTC ) {
	gpcTHRD_CAM cpy = *pTC;
	//SDL_Surface srf =
	gpdCAMu& cam = cpy.pC->cam;
	U1	*pPIX, *pDAT;

	U4 iCNT = 0, p = cpy.pSRF->pitch, p2 = p*2, h = cpy.pSRF->h, e = p*h;
	while( cpy.pC )
	{
		pPIX = cpy.pSRF ? (U1*)cpy.pSRF->pixels : NULL;
		usleep(1000/30);
		cam.grab();
		if( !pPIX )
			continue;
		#ifdef gpdSYSpi
		pDAT = cam.getImageBufferData();
		#else
		pDAT = NULL;
		#endif

		if( pDAT )
		{
			memcpy( pPIX, pDAT, p*h );
			/*for( U4 i = p*(iCNT&1); i < e; i += p2 )
				memcpy( pPIX+i, pDAT+i, p );*/
			iCNT++;
			continue;
		}
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

	if( pC->cam.openCAM() )
	{
		usleep(1*1000);
	}
	if( aiQC[0] >= qc )
		return NULL; //pSRF ? (U1*)pSRF->pixels : NULL;

	if( bTHRD )
	{
		pREF = NULL;
		aiQC[0] = qc+ gpdTCP_tOUT;
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

	if( nPIXall != nPIX )
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
#ifndef gpdSYSpi
        if( pC->cam.bLIVE() > 0 )
        {
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
            pC->cam.grab();
            pREF = NULL;
            aiQC[0] = qc+ gpdTCP_tOUT;
            return pSRF ? (U1*)pSRF->pixels : NULL;
        }
#else
        gpcTHRD_CAM trd;
        trd.pC = pC;
        trd.pSRF = pSRF;

        if( !bTHRD )
        {
            T = std::thread( call_cam, &trd );
            usleep(3*1000);
            bTHRD = true;
        }

		if( !bTHRD )
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
#endif

	}


	aiQC[0] = qc;
	return pSRF ? (U1*)pSRF->pixels : NULL;
}

U4 gpcPICall::alfFND( U1* pS ) {
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
gpcPIC*	gpcPICall::aluFND( gpcALU& alu ) {
	U4 i =	alu.bSTR() ?
			alfFND( (U1*)alu.pDAT ) 	// ez a kép neve
			: alu.u8();

	if( pPIC = PIC(i) )
		return pPIC;

	I8x2 alfN(0,14);
	U1* pS = (U1*)alu.pDAT;
	alfN = pS;
	alfN.num = gpfSTR2U8( pS+alfN.num, &pS );
	return PIC( alfN );
}
