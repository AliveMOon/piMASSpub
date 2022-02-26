I84 gr2cr_1m( void ) const
	{
		double	rz = (((double)(z>>4)) + 0.25)*PI,
				ry = (((double)(y>>4)) + 0.25)*PI,
				ctgZ = cos(rz)/sin(rz),
				ctgY = cos(ry)/sin(ry),
				ctgZY2 = ctgZ*ctgZ + ctgY*ctgY,
				rx = (double)gpeQx / sqrt(1.0+ctgZY2), dif; 
		I84 cr;

		ctgZ *= -rx;
		ctgY *= -rx;
		ctgZY2 = ctgZ*ctgZ + ctgY*ctgY;
		dif = rx;
		rx = sqrt( (double)gpeQcr1m2 - ctgZY2 );
		dif -= rx;

		switch(w)
		{
			case 0:
				cr.y = -rx;
				cr.x =  ctgY;
				cr.z =  ctgZ;
				break;
			case 1:
				cr.x =  rx;
				cr.y =  ctgY;
				cr.z =  ctgZ;
				break;
			case 2:
				cr.z =  rx;
				cr.y =  ctgY;
				cr.x = -ctgZ;
				break;
			case 3:
				cr.y =  rx;
				cr.z = -ctgY;
				cr.x = -ctgZ;
				break;
			case 4:
				cr.x = -rx;
				cr.z = -ctgY;
				cr.y = -ctgZ;
				break;
			case 5:
				cr.z = -rx;
				cr.x = ctgY;
				cr.y = -ctgZ;
				break;
			default:
				// ERROR
				cr.null_xyz();
		}
		cr.w = sqrt(cr.qlen_xyz());
		return cr;
	}