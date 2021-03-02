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

U4 gpcCAMubi::getImageTypeSize( RASPICAM_FORMAT f )
{
    if( f != RASPICAM_FORMAT_RGB )
        return 0;

    return qryBUF.length; //infBUF.bytesused;
}

bool gpcCAMubi::openCAM()
{
    if( bOPEN )
        return bOPEN;

    ///  1. Open the device ---------------
    if(bSTDcout_V4l2){std::cout << "1. Open the device ---------------" << std::endl;}
    if( fd < 0 )
	{
        fd = open("/dev/video0",O_RDWR);
		if(fd < 0) {
			char sVIDx[0x80];
			for( U4 i = 1; i < 8; i++ ) {
				sprintf( sVIDx, "/dev/video%d", i );
				fd = open(sVIDx,O_RDWR);
				if(fd < 0)
					continue;
				break;
			}
		}
	}

    if(fd < 0) {
        perror("Failed to open device, OPEN");
        return bOPEN;
    } else
        wip = gpmMAX( 1, wip );

    ///  2. Ask the device if it can capture frames
    if( wip == 1 )
    {
        if(bSTDcout_V4l2){std::cout << "2. Ask the device if it can capture frames" << std::endl;}
        if(ioctl(fd, VIDIOC_QUERYCAP, &capBLTY) < 0){
            // something went wrong... exit
            perror("Failed to get device capabilities, VIDIOC_QUERYCAP");
            //return bOPEN;
        } else {
             if(bSTDcout_V4l2){std::cout << "driver:" << (char*)capBLTY.driver << std::endl;}
        }
        wip = 2;
    }

    bool bBREAK = false;
    /// 3. ------------------------------------------------------
    frmDeSC.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    int r = ioctl(fd,VIDIOC_ENUM_FMT,&frmDeSC);
    U4  pixFMT = 0,
        iWH0 = -1, iWH;
    I4  ar0 = wh0.area(), df0 = ar0,
        ar, df;
    U4x2 aWH[8];
    gpfMset( aWH, gpmN(aWH), &wh0, sizeof(*aWH) );
    while( r > -1 ) {
        if(bSTDcout_V4l2){std::cout << frmDeSC.description << std::endl;}
        frmSZ.type = frmDeSC.type;
        frmSZ.pixel_format = frmDeSC.pixelformat;
        frmSZ.index = 0;
        /// 4. ------------------------------------------------------
        r = ioctl(fd, VIDIOC_ENUM_FRAMESIZES, &frmSZ);

        while( r > -1 ) {
            if(frmSZ.type == V4L2_FRMSIZE_TYPE_DISCRETE) {
                if(bSTDcout_V4l2){std::cout  << "discrete w:" << frmSZ.discrete.width << " h:" << frmSZ.discrete.height << std::endl;}
                switch( frmSZ.pixel_format ) {
                    case V4L2_PIX_FMT_YUYV:
                        pixFMT = V4L2_PIX_FMT_YUYV;
                        iWH0 = 0;
                        break;
                    default:
                        frmSZ.index++;
                        r = ioctl(fd, VIDIOC_ENUM_FRAMESIZES, &frmSZ);
                        continue;
                }
                ar = frmSZ.discrete.width*frmSZ.discrete.height;
                df = ar0<ar ? ar-ar0 : ar0-ar;
                if( df0 > df )
                {
                    iWH = iWH0;

                    df0 = df;
                    aWH[iWH].x = frmSZ.discrete.width;
                    aWH[iWH].y = frmSZ.discrete.height;
                }
            } else {
                if(bSTDcout_V4l2){std::cout  << "-------- w:" << frmSZ.stepwise.min_width << " h:" << frmSZ.stepwise.min_height << std::endl
                                        << "-------- w:" << frmSZ.stepwise.max_width << " h:" << frmSZ.stepwise.max_height << std::endl;}
                switch( frmSZ.pixel_format ) {
                    case V4L2_PIX_FMT_RGB24:
                        bBREAK = true;
                        pixFMT = V4L2_PIX_FMT_RGB24;
                        iWH0 = 1;
                        break;
                    case V4L2_PIX_FMT_YUYV:
                        //bBREAK = true;
                        pixFMT = V4L2_PIX_FMT_YUYV;
                        iWH0 = 0;
                        break;
                    default:
                        frmSZ.index++;
                        r = ioctl(fd, VIDIOC_ENUM_FRAMESIZES, &frmSZ);
                        continue;
                }
                iWH = iWH0;
                aWH[iWH].x = gpmMAX( frmSZ.stepwise.min_width,    gpmMIN(aWH[iWH].x,frmSZ.stepwise.max_width) );
                aWH[iWH].y = gpmMAX( frmSZ.stepwise.min_height,   gpmMIN(aWH[iWH].y,frmSZ.stepwise.max_height) );
            }

            if( bBREAK ) break;

            frmSZ.index++;
            r = ioctl(fd, VIDIOC_ENUM_FRAMESIZES, &frmSZ);
        }
        if( bBREAK )
            break;

        frmDeSC.index++;
        r = ioctl(fd,VIDIOC_ENUM_FMT,&frmDeSC);
    }
    if( !pixFMT )
        return bOPEN;



    wip = 0;
    bBREAK = ( setCaptureSize( aWH[iWH0].x, aWH[iWH0].y, pixFMT ) < 0 );
    if( bBREAK )
        return bOPEN;

    //grab();

    return bOPEN = true;
}
int gpcCAMubi::setCaptureSize( U4 w, U4 h, U4 picFRM ) {
    if( wh == wh0 )
        return 0;

    /// 5. Set Image format
    if(bSTDcout_V4l2){std::cout << "5. Set Image format ----------" << std::endl;}
    gpmZ(format);
    format.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    format.fmt.pix.width = w;
    format.fmt.pix.height = h;
    format.fmt.pix.pixelformat = picFRM;
    format.fmt.pix.field = V4L2_FIELD_NONE;
    int res = ioctl(fd, VIDIOC_S_FMT, &format);
    if( res < 0){
        closeCAM();
        return res;
    }

    if( pBUFF ) {
        munmap(pBUFF, qryBUF.length);
    }

    /// 6. VIDIOC_REQBUFS ----------
    if(bSTDcout_V4l2){std::cout << "6. VIDIOC_REQBUFS ----------" << std::endl;}
    reqBUF.count = 1;                    // one request buffer
    reqBUF.type = format.type ;          // request a buffer wich we an use for capturing frames
    reqBUF.memory = V4L2_MEMORY_MMAP;
    res = ioctl(fd, VIDIOC_REQBUFS, &reqBUF);
    if( res < 0){
        return res;
    }

    if(bSTDcout_V4l2){std::cout << "7. VIDIOC_QUERYBUF ----------------" << std::endl;}
    /// 7. VIDIOC_QUERYBUF ----------
    qryBUF.type = format.type ;
    qryBUF.memory = V4L2_MEMORY_MMAP;
    qryBUF.index = 0;
    res = ioctl(fd, VIDIOC_QUERYBUF, &qryBUF);
    if( res < 0){
        return res;
    }

    pBUFF = (char*)mmap(    NULL, qryBUF.length,
                            PROT_READ | PROT_WRITE,
                            MAP_SHARED,
                            fd, qryBUF.m.offset    );

    /// 8. VIDIOC_STREAMON ----------------
    if(bSTDcout_V4l2){std::cout << " 8. VIDIOC_STREAMON ----------------" << std::endl;}
    type = format.type;
    res = ioctl(fd, VIDIOC_STREAMON, &type);
    if( res < 0 ){
        return res;
    }
    if( wip != 9 ) {
        /// 9. VIDIOC_QBUF ----------------
        if(bSTDcout_V4l2){std::cout << " 9. VIDIOC_QBUF ----------------" << std::endl;}
        infBUF.type = type;
        infBUF.memory = V4L2_MEMORY_MMAP;
        infBUF.index = 0;
        res = ioctl(fd, VIDIOC_QBUF, &infBUF);
        if( res < 0){
            return res;
        }
        wip = 9;
    }
    wh0 = U4x2(w,h);
    wh = wh0;
    return res;
}

U4 gpcCAMubi::grab()
{
    int res;
    if( wip == 9 ) {
        /// 10. VIDIOC_DQBUF --------------------
        if(bSTDcout_V4l2){std::cout << "10.1 VIDIOC_DQBUF --------------------" << std::endl;}
        res = ioctl(fd, VIDIOC_DQBUF, &infBUF);
        if( res < 0){
            return nGRB++;
        }
        if(bSTDcout_V4l2){std::cout << "10.2 VIDIOC_DQBUF --------------------" << std::endl;}
        wip = 10;
    }

    return nGRB++;
}
void* gpcCAMubi::retrieve( void *pPIX, RASPICAM_FORMAT f )
{
    U1* pRGB = (U1*)pPIX;
    if(bSTDcout_V4l2){std::cout << "CPY" << std::endl;}
    int res ;
    switch( format.fmt.pix.pixelformat ){ //frmSZ.pixel_format ) {
        case V4L2_PIX_FMT_YUYV: {
//                                      U               V
// 4x4            start + 0:	Y'00	Cb00	Y'01	Cr00	Y'02	Cb01	Y'03	Cr01
//                start + 8:	Y'10	Cb10	Y'11	Cr10	Y'12	Cb11	Y'13	Cr11
//                start + 16:	Y'20	Cb20	Y'21	Cr20	Y'22	Cb21	Y'23	Cr21
//                start + 24:	Y'30	Cb30	Y'31	Cr30	Y'32	Cb31	Y'33	Cr31
                I4 y1,cb,y2,cr, sub;
                for( U4 s = 0, d = 0; s < qryBUF.length-4; s+=4, d+=6 )
                {
                    y1 = ((U1*)pBUFF)[s]-16;
                    cb = ((U1*)pBUFF)[s+1]-128;
                    y2 = ((U1*)pBUFF)[s+2]-16;
                    cr = ((U1*)pBUFF)[s+3]-128;
                    sub = ((cr+cb)/2);
                    pRGB[d+0] = gpmMAX(0,gpmMIN(255,cr+y1));
                    pRGB[d+1] = gpmMAX(0,gpmMIN(255,y1-sub));
                    pRGB[d+2] = gpmMAX(0,gpmMIN(255,cb+y1));

                    pRGB[d+3] = gpmMAX(0,gpmMIN(255,cr+y2));
                    pRGB[d+4] = gpmMAX(0,gpmMIN(255,y2-sub));
                    pRGB[d+5] = gpmMAX(0,gpmMIN(255,cb+y2));

                }
            } break;
        default:
            gpmMcpy(pPIX, pBUFF, qryBUF.length );
            break;
    }
    if( wip != 9 ) {
        wip = 9;
        /// 9.1 VIDIOC_QBUF ----------------
        infBUF.type = type;
        infBUF.memory = V4L2_MEMORY_MMAP;
        infBUF.index = 0;
        if(bSTDcout_V4l2){std::cout << " 9.1 VIDIOC_QBUF ----------------" << std::endl;}
        res = ioctl(fd, VIDIOC_QBUF, &infBUF);
        if(res < 0)
            return NULL;
        if(bSTDcout_V4l2){std::cout << " 9.2 VIDIOC_QBUF ----------------" << std::endl;}
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
	if( nPIXall != nPIX ) {
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
	}

	if( pSRF ) {
#ifndef gpdSYSpi
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
        //pC->cam.grab();
        pREF = NULL;
		aiQC[0] = qc+ gpdTCP_tOUT;
		return pSRF ? (U1*)pSRF->pixels : NULL;
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
	pS += gpmNINCS(pS," \t\"");
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
