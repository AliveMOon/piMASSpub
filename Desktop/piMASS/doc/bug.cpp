I8 GPS_ROADx64::bug( U4 *p_bug, U4 *p_t, U4 *p_te, U4* p_idx, I8 bob_w, I8 bob_h )
{
	if( *p_bug < '0' )
		return 0;
		
	I8 n_bug = p_bug-p_t;
	if( p_idx[n_bug] )
		return 0;

	I8	a_dir[] = { -bob_w,+1,+bob_w,-1 },
		a_row[] = { -bob_w,0,+bob_w,0 },
		x = n_bug%bob_w,y= n_bug/bob_w,n_io = 0;

	if( x < 1 || x > bob_w-2 )
		return 0;
	if( y < 1 || y > bob_h-2 )
		return 0;

	U4	*p_tl = p_bug-x,
		*p_tr = p_tl+bob_w;
		
	p_row = p_rowe;
	n_step = 0;

		
	lurd.z = lurd.x = x;
	lurd.w = lurd.y = y;
		
	bool b_in, b_prev = true;
	char dir = 1, rule;
		
	U4	*p_start = p_bug,
		*p_prev = p_bug;
	U2	*p_half;
	dir = 1;
	p_bug++;
	while( p_start != p_bug || ((n_io = p_rowe-p_row)&1) )
	{
		rule =	(p_bug>=p_t)	// 1 ha false fönt van kint
			| ((p_bug>=p_tl)<<1)	// 2 ha false balra van kint
			| ((p_bug< p_tr)<<2)	// 4 ha false jobra van kint
			| ((p_bug< p_te)<<3)	// 8 ha false lent van kint
			;

		b_in = false;
		if( (rule == 0xf) )
		{
			// rule good azaz 0xf akkor
			b_in = (*p_bug == in);
		} 

		x = p_bug-p_t;
		if( b_in != b_prev )
		if( dir&1 )
		{
			*p_rowe = x;
			p_rowe++;
		}

		// out	   >jobb
		//	   x--x  x----x
		// -----------|--|--|--+ |
		//	   |  x--xx---x
		// in         |  >bal| |
		//            |      x---x
		//            x        | |
		//          start    x---x
		//                   : :

		if( b_in )
		{
			if( b_in != b_prev )
			{
				n_step++;
			}
			x = p_bug-p_t;
			y = x/bob_w;
			x %= bob_w;

			if( lurd.x > x )
				lurd.x = x;
			if( lurd.y > y )
				lurd.y = y;
			if( lurd.z < x )
				lurd.z = x;
			if( lurd.w < y )
				lurd.w = y;

			dir--;
			if( dir < 0 )
				dir = 3;
		} else {
			dir++;
			dir %= 4;
		}
			
		b_prev = b_in;
		p_prev = p_bug;
			
		p_bug += a_dir[dir];
		p_tl += a_row[dir];
		p_tr = p_tl+bob_w;
	}
	lurd += GPCI4( 0,0,1,1 );
		
	n_io = p_rowe-p_row;

	if( n_io&1 )
	{
		if( p_rowe[-1] == *p_row )
			p_rowe--;

		n_io = p_rowe-p_row;
		if( n_io&1 )
		{
			p_rowe = p_row;
			bug( p_start+1, p_t, p_te, p_idx, bob_w, bob_h );
			return 0;
		}
	}

	GPC_IIx64* p_ii = new GPC_IIx64[n_io];
	for( I8 i = 0; i < n_io; i++ )
	{
		p_ii[i].i = i;
		p_ii[i].vi = p_row[i];
	}
	p_ii->median( n_io, true );

	n_area = 0;
	for( I8 i = 0, a,b, n; i < n_io; i+=2 )
	{
		a = p_row[p_ii[i].i];
		b = p_row[p_ii[i+1].i];
		n = b-a;
		n_area += n;
		if( n < 4 )
		{
			for( ; a < b; a++ )
			{
				p_idx[a] = n_idx;
			}
		} else
			GPF_MEM_set( p_idx+a, n, &n_idx, sizeof(*p_idx) );
	}

	GPM_DELARRAY( p_ii );
	return n_step;
}