#include "gpstd_x64.h"
#ifndef _MUTEX_
	#include <thread>
	#include <mutex>
#endif

#ifdef _DEBUG_MUTI
	std::mutex gp_mem_mutex;
#else
	volatile LONGLONG	gp_one_lock_trd_id = 0, gp_one_open = 0, gp_one_close = 0,  gp_one_err = 0;
	volatile LONGLONG	gp_arr_lock_trd_id = 0, gp_arr_open = 0, gp_arr_close = 0,  gp_arr_err = 0;
#endif // _DEBUG_MUTI



I8	n_alloc = 0,
	n_free = 0,
	n_use = 0, n_n_use_ONE = 0,
	a_alloc[0x100],
	a_free[0x100],
	a_diff[0x100],
	a_n_alloc_ONE[0x100],
	a_n_free_ONE[0x100],
	a_n_diff_ONE[0x100],

	n_n_use_ARRAY = 0,
	a_n_alloc_ARRAY[0x100],
	a_n_free_ARRAY[0x100],
	a_n_diff_ARRAY[0x100],
	n_big = 0;

#define GPD_MEM_PIX	14
char	a_memmap[1024*1024*2],
	*p_mem_low = NULL,
	*p_mem_hi = NULL,
	*p_memmap_end = a_memmap+1024*1024;

#ifndef _DEBUG_NEW

#define GPD_CACHE_ONE 0x40
#define GPD_CACHE_STR 0x400


ULL	gp_n_cache_ONE_out = 0,
	gp_n_cache_STR_out = 0,
	gp_n_cache_ARR_out = 0,
	gp_n_cache_ONE_alloc = 0x80000*GPD_CACHE_ONE,
	gp_n_cache_STR_alloc = gp_n_cache_ONE_alloc*2,
	gp_n_cache_ARR_alloc = gp_n_cache_ONE_alloc*4;

char	*gp_p_cache_ONE_alloc = (char*)_aligned_malloc(gp_n_cache_ONE_alloc+gp_n_cache_STR_alloc+gp_n_cache_ARR_alloc, 0x20),
	*gp_p_cache_ONE_end	= gp_p_cache_ONE_alloc+gp_n_cache_ONE_alloc,
	*gp_p_cache_STR_alloc = gp_p_cache_ONE_end,
	*gp_p_cache_STR_end   = gp_p_cache_STR_alloc+gp_n_cache_STR_alloc,
	*gp_p_cache_ARR_alloc = gp_p_cache_STR_end,
	*gp_p_cache_ARR_end	= gp_p_cache_ARR_alloc+gp_n_cache_ARR_alloc,

	*gp_p_cache_ONE_load	= gp_p_cache_ONE_alloc,
	*gp_p_cache_ONE_empty	= gp_p_cache_ONE_alloc,

	*gp_p_cache_STR_load  = gp_p_cache_STR_alloc,
	*gp_p_cache_STR_empty = gp_p_cache_STR_alloc,

	*gp_p_cache_ARR_load	= gp_p_cache_ARR_alloc,
	*gp_p_cache_ARR_empty = gp_p_cache_ARR_alloc;
#endif

void* GPF_alloc( size_t size, I8* p_n_alloc )
{
	GPS_ALLOC mem;
	mem.size = size + sizeof(GPS_ALLOC);
	ULL a16 = mem.size;
	if( a16&0xf )
		a16 += 0x10-(a16&0xf);

	mem.p_alloc = _aligned_malloc( a16, 0x10 );
	if( !mem.p_alloc )
		return NULL;

	GPS_ALLOC* p_out = (GPS_ALLOC*)mem.p_alloc;
	*p_out = mem;
	p_out++;

	#ifndef _MEMMAP_OFF
		(*p_n_alloc)++;
	
		int c = log2(size);
		if( c >= sizeof(a_alloc) )
			return p_out;
	
		a_alloc[c] += mem.size;
		p_n_alloc[c]++;
		
		c *= c;
		if( c > 0x7e )
			c = 0x7f;
		else
			c++;

		U4 n_map = size>>GPD_MEM_PIX;
		char *p_map = a_memmap + (((ULL)((U1*)mem.p_alloc))>>GPD_MEM_PIX);
		if( p_map >= p_memmap_end )
			return p_out;
		if( !p_mem_low )
			p_mem_low = p_map;
		else if( p_mem_low > p_map )
			p_mem_low = p_map;

		if( !p_mem_hi )
			p_mem_hi = p_map + n_map;
		else if( p_mem_hi < (p_map + n_map) )
			p_mem_hi = p_map + n_map;

		*p_map = 0x7f;
		p_map++;
		n_map--;
		if( p_memmap_end <= p_map+n_map )
			n_map = p_memmap_end-p_map;

		if( n_map < 1 )
			return p_out;
	
		gp_memset_u1( p_map, (U1)c, n_map );
	#endif

	return p_out;
}
__forceinline void GPF_delete( void *p_void, I8* p_n_free )
{
	if( !p_void )
		return;

	GPS_ALLOC	*p_alloc = (GPS_ALLOC*)p_void;
	p_alloc--;

	if( p_alloc->p_alloc != p_alloc )
		return;

	#ifdef _MEMMAP_OFF
		_aligned_free( p_alloc );
	#else
		ULL size = p_alloc->size;
		_aligned_free( p_alloc );
		(*p_n_free)++;

		int c = log2(size);
		if( c >= sizeof(a_free) )
			return;
		
		a_free[c] += size;
		p_n_free[c]++;
		
		U4 n_map = size>>GPD_MEM_PIX;
		c *= -c;
		if( c < -0x7e )
			c = -0x7f;
		else
			c--;

		char *p_map = a_memmap + (((ULL)((U1*)p_alloc))>>GPD_MEM_PIX);
		if( p_map >= p_memmap_end )
			return;

		*p_map = -0x7f;
		p_map++;
		if( n_map ? p_map >= p_memmap_end : true )
			return;
		n_map--;
		if( p_memmap_end <= p_map+n_map )
			n_map = p_memmap_end-p_map;

		if( n_map < 1 )
			return;
	
		gp_memset_u1( p_map, (U1)c, n_map );
	#endif
}

#ifdef _DEBUG_NEW
// CHACHE -- MOM ---------------------------------------------------------
	GPC_CHM_MOM::GPC_CHM_MOM( U4 b, U4 _n )
	{
		gp_zero( this, GPM_OFF( GPC_CHM_MOM, mtx ) );
		n = _n;
		n_byte = b;
		p_end = 
		p_empty = 
		p_free = 
		p_alloc = (GPC_CHM*)_aligned_malloc( GPD_CHM_N*sizeof(*p_alloc), 0x20);
		n_add = n_sub = 0;	
		p_end += GPD_CHM_N;
		gp_zero( p_alloc, GPD_CHM_N*sizeof(*p_alloc) );
	};
	U1* GPC_CHM_MOM::mom_add( void )
	{
		mtx.lock();
		if( !hnd )
			hnd = GetCurrentThread();
		else
			h2 = GetCurrentThread();
		int pri = GetThreadPriority( hnd );

		bool b_go = false;
		if( pri != THREAD_PRIORITY_TIME_CRITICAL )
			b_go = SetThreadPriority( hnd, THREAD_PRIORITY_TIME_CRITICAL );
		// talán a free-ben?
		U1* p_add = p_free->chm_add( this );
		if( p_add )
		{
			n_add++;
			// emptyben már sikerült
			if( b_go )
				SetThreadPriority( hnd, pri );
			hnd = 0;
			mtx.unlock();
			return p_add;
		} 
		
		if( p_free < p_empty )
		{
			// foghíj volt
			// vissza az p_empty-hez
			p_free = p_empty;
			p_add = p_empty->chm_add( this );
			if( p_add )
			{
				n_add++;
				if( b_go )
					SetThreadPriority( hnd, pri );
				hnd = 0;
				mtx.unlock();
				return p_add;
			}
			// empty-ben is elfogyott
		} 
		
		if( n_add-n_sub < n_empty )	// viszont kell lenie még üresnek
		for( p_free = p_alloc; p_free < p_empty; p_free++ )
		{
			p_add = p_free->chm_add( this );
			if( !p_add )
				continue;

			// talált foghíjat
			n_add++;
			if( b_go )
				SetThreadPriority( hnd, pri );
			hnd = 0;
			mtx.unlock();
			return p_add;
		}
		
		// nem talált foghíjat bõvítsük az empty-t
		p_empty++;
		if( p_empty >= p_end )
		{
			n_old = p_end-p_alloc;
			if( n_old > 0x100 )
			{
				if( b_go )
					SetThreadPriority( hnd, pri );
				hnd = 0;
				mtx.unlock();
				return NULL;
			}
			GPC_CHM* p_kill = p_alloc;
			p_alloc = (GPC_CHM*)_aligned_malloc( (n_old+GPD_CHM_N)*sizeof(*p_alloc), 0x10 );
			p_end =
			p_empty = 
			p_free = p_alloc + n_old;
			p_end += GPD_CHM_N;

			gp_memcpy( p_alloc, p_kill, n_old*sizeof(*p_alloc) );
			gp_zero( p_empty, GPD_CHM_N*sizeof(*p_alloc) );
				
			_aligned_free( p_kill );
		} else
			p_free = p_empty;

		p_add = p_free->chm_add( this );
		if( p_add )
			n_add++;
		if( b_go )
			SetThreadPriority( hnd, pri );
		hnd = 0;
		mtx.unlock();
		return p_add;
	}
	U1* GPC_CHM_MOM::mom_sub( U1* p_kill )
	{
		mtx.lock();
		if( !hnd )
			hnd = GetCurrentThread();
		else
			h2 = GetCurrentThread();
		int pri = GetThreadPriority( hnd );
		bool b_go = false;
		if( pri != THREAD_PRIORITY_TIME_CRITICAL )
			b_go = SetThreadPriority( hnd, THREAD_PRIORITY_TIME_CRITICAL );

		p_kill = p_free->chm_sub( p_kill );
		if( !p_kill )
		{
			n_sub++;
			if( b_go )
				SetThreadPriority( hnd, pri );
			hnd = 0;
			mtx.unlock();
			return NULL;
		}
		for( GPC_CHM* p_scan = p_alloc; p_scan <= p_empty; p_scan++ )
		{
			if( p_free == p_scan )
				continue;

			p_kill = p_scan->chm_sub( p_kill );
			if( p_kill )
				continue;
			
			if( p_free->n_use > p_scan->n_use )
				p_free = p_scan;
			
			n_sub++;
			if( b_go )
				SetThreadPriority( hnd, pri );
			hnd = 0;
			mtx.unlock();
			return NULL;
		}

		if( b_go )
			SetThreadPriority( hnd, pri );
		hnd = 0;
		mtx.unlock();
		return p_kill;
	}
	// CHACHE ---------------------------------------------------------------
	U1* GPC_CHM::chm_add( GPC_CHM_MOM* p_mom )
	{
		if( !p_alloc )
		{
			n_byte = p_mom->n_byte;
			p_empty = p_free = p_end = p_alloc = (U1*)_aligned_malloc( p_mom->n*(n_byte+1), 0x20 );
			if( !p_alloc )
				return NULL;
			
			p_end += p_mom->n*n_byte;
			p_mom->n_empty += p_mom->n;
			gp_zero( p_alloc, p_mom->n*(n_byte+1) );
		}
		

		if( p_free < p_empty )
		{
			n_add++;
			n_use = n_add-n_sub;
			U1* p_add = p_free;
			p_free = p_empty;
			p_end[(p_add-p_alloc)/n_byte] = 1;
			return p_add;
		}
		else if( p_empty >= p_end )
		{
			if( n_use < p_mom->n )
			{
				for( U1* p_sc = p_end, *p_e = p_sc+p_mom->n; p_sc < p_e; p_sc++ )
				{
					if( *p_sc )
						continue;
					n_add++;
					n_use = n_add-n_sub;
					*p_sc = 1;
					return p_alloc+(p_sc-p_end)*n_byte;
				}
			}
			return NULL;
		}
		n_add++;
		n_use = n_add-n_sub;
		U1* p_add = p_empty;
		p_empty += n_byte;
		p_free = p_empty;
		p_end[(p_add-p_alloc)/n_byte] = 1;
		return p_add; 
	}
	U1* GPC_CHM::chm_sub( U1*p_kill )
	{
		if( p_alloc ? p_kill < p_alloc : true ) // ha pl. NULL azonnal elhagyja a helyszint és NULL is egyben
			return p_kill;
		if( p_kill >= p_end )
			return p_kill;
		U1* p_sc = p_end+((p_kill-p_alloc)/n_byte);
		if( !*p_sc )
			return NULL;
		
		*p_sc = 0;
		
		n_sub++;
		n_use = n_add-n_sub;
		p_free = p_kill;
		return NULL;
	}

	

	GPC_CHM_MOM	a_one[] = {	//		// n	// size	// log2(size-1)	// log2(size-1)
							// 0	// 0	//
					{ 0x0010, 0x1000 },	// 1	// 16	// 3		// 0	
					{ 0x0020, 0x1000 },	// 2	// 32	// 4		// 1	
					{ 0x0040, 0x1000 },	// 3	// 64	// 5		// 2 	
					{ 0x0080, 0x1000 },	// 4	// 128	// 6		// 3 	
					{ 0x0100, 0x1000 },	// 5	// 256	// 7		// 4 	
					{ 0x0200, 0x1000 },	// 6	// 512	// 8		// 5 	
					{ 0x0400, 0x1000 },	// 7	// 1024	// 9		// 6 	
					//{ 0x0800, 0x1000 },	// 8	// 2048	// 10		// 7 	
					//{ 0x1000, 0x1000 },	// 9	// 4096	// 11		// 8 	
					//{ 0x2000, 0x1000 },	// 10	// 8192	// 12		// 9 	
					//{ 0x4000, 0x1000 },	// 11	// 16384	// 13		// 10 
				},
			a_arr[] = {	//		// n	// size	// log2(size-1)	// log2(size-1)
							// 0	// 0	//
					{ 0x0010, 0x1000 },	// 1	// 16	// 3		// 0	
					{ 0x0020, 0x1000 },	// 2	// 32	// 4		// 1	
					{ 0x0040, 0x1000 },	// 3	// 64	// 5		// 2 	
					{ 0x0080, 0x1000 },	// 4	// 128	// 6		// 3 	
					{ 0x0100, 0x1000 },	// 5	// 256	// 7		// 4 	
					{ 0x0200, 0x1000 },	// 6	// 512	// 8		// 5 	
					{ 0x0400, 0x1000 },	// 7	// 1024	// 9		// 6 	
					{ 0x0800, 0x1000 },	// 8	// 2048	// 10		// 7 	
					{ 0x1000, 0x1000 },	// 9	// 4096	// 11		// 8 	
					{ 0x2000, 0x1000 },	// 10	// 8192	// 12		// 9 	
					{ 0x4000, 0x1000 },	// 11	// 16384	// 13		// 10 	
				};
	
	void* operator new( size_t size )
	{
		if( !size )
			return NULL;
		
		if( size > a_one[GPM_N(a_one)-1].n_byte )
			return GPF_alloc( size, a_n_alloc_ONE );

		char l = 0;
		if( size > 0x10 )
			l = log2(size-1)-3;
		U1* p_new = a_one[l].mom_add();
		if( p_new )
			return p_new;

		return GPF_alloc( size, a_n_alloc_ONE );
	}
	void operator delete( void *p_void )
	{
		if( !p_void )
			return;

		U1* p_kill = (U1*)p_void;
		for( U1 l = 0; l < GPM_N(a_one); l++ )
		{
			p_kill = a_one[l].mom_sub(p_kill);
			if( !p_kill )
				return;
		}

		GPF_delete( p_void, a_n_alloc_ONE );
	}

	void* operator new[]( size_t size )
	{
		if( !size )
			return NULL;
		
		if( size > a_arr[GPM_N(a_arr)-1].n_byte )
			return GPF_alloc( size, a_n_alloc_ARRAY );

		char l = 0;
		if( size > 0x10 )
			l = log2(size-1)-3;
		U1* p_new = a_arr[l].mom_add();
		if( p_new )
			return p_new;

		return GPF_alloc( size, a_n_alloc_ARRAY );
	}
	
	void operator delete[](void *p_void)
	{
		if( !p_void )
			return;

		U1* p_kill = (U1*)p_void;
		for( U1 l = 0; l < GPM_N( a_arr ); l++ )
		{
			p_kill = a_arr[l].mom_sub(p_kill);
			if( !p_kill )
				return;
		}

		GPF_delete( p_void, a_n_free_ARRAY );
	}
#else
void* operator new( size_t size )
{
	if( !size )
		return NULL;
	

	#ifdef _DEBUG_MUTI
	gp_mem_mutex.lock();
	#else
	ULL now_trd = (ULL)GetCurrentThreadId();
	volatile U4 n = 0;
	while( now_trd != InterlockedCompareExchange64( &gp_one_lock_trd_id, now_trd, 0 ) )
		n++;
	while( gp_one_close != gp_one_open )
	{
		gp_one_err++;
	}
	gp_one_open++;
	#endif
	if( gp_p_cache_ONE_alloc ? size > GPD_CACHE_STR : true )
	{
		void* p_out = GPF_alloc( size, a_n_alloc_ONE );
		#ifdef _DEBUG_MUTI
			gp_mem_mutex.unlock();
		#else
			gp_one_close++;
			InterlockedExchange64( &gp_one_lock_trd_id, 0 );
		#endif
		return p_out;
	}
	void* p_out = NULL;
		
	if( size <= GPD_CACHE_ONE )
	{
		if( gp_p_cache_ONE_empty >= gp_p_cache_ONE_end )
		{
			gp_n_cache_ONE_out++;
			p_out = GPF_alloc( size, a_n_alloc_ONE );
			#ifdef _DEBUG_MUTI
				gp_mem_mutex.unlock();	
			#else
				gp_one_close++;
				InterlockedExchange64( &gp_one_lock_trd_id, 0 );
			#endif
			return p_out;
		}

		p_out = gp_p_cache_ONE_empty;
		if( p_out < gp_p_cache_ONE_load )
		{
			gp_p_cache_ONE_empty = gp_p_cache_ONE_load;
			#ifdef _DEBUG_MUTI
				gp_mem_mutex.unlock();	
			#else
				gp_one_close++;
				InterlockedExchange64( &gp_one_lock_trd_id, 0 );
			#endif
			return p_out;
		}
		gp_p_cache_ONE_empty += GPD_CACHE_ONE;	
		gp_p_cache_ONE_load = gp_p_cache_ONE_empty;
		#ifdef _DEBUG_MUTI
			gp_mem_mutex.unlock();	
		#else
			gp_one_close++;
			InterlockedExchange64( &gp_one_lock_trd_id, 0 );
		#endif
		return p_out;
	}
	
	if( gp_p_cache_STR_empty >= gp_p_cache_STR_end )
	{
		gp_n_cache_STR_out++;
		p_out = GPF_alloc( size, a_n_alloc_ONE );
		#ifdef _DEBUG_MUTI
			gp_mem_mutex.unlock();	
		#else
			gp_one_close++;
			InterlockedExchange64( &gp_one_lock_trd_id, 0 );
		#endif
		return p_out;
	}

	p_out = gp_p_cache_STR_empty;
	if( p_out < gp_p_cache_STR_load )
	{
		gp_p_cache_STR_empty = gp_p_cache_STR_load;
		#ifdef _DEBUG_MUTI
			gp_mem_mutex.unlock();	
		#else
			gp_one_close++;
		I	nterlockedExchange64( &gp_one_lock_trd_id, 0 );
		#endif
		return p_out;
	}

	gp_p_cache_STR_empty = (gp_p_cache_STR_load += GPD_CACHE_STR);
	#ifdef _DEBUG_MUTI
		gp_mem_mutex.unlock();	
	#else
		gp_one_close++;
	I	nterlockedExchange64( &gp_one_lock_trd_id, 0 );
	#endif
	return p_out;
}
	
void operator delete( void *p_void )
{
	if( !p_void )
		return;
	#ifdef _DEBUG_MUTI
		gp_mem_mutex.lock();
	#else
		ULL now_trd = (ULL)GetCurrentThreadId();
		U4 n = 0;
		while( now_trd != InterlockedCompareExchange64( &gp_one_lock_trd_id, now_trd, 0 ) )
			n++;
		while( gp_one_close != gp_one_open )
		{
			gp_one_err++;
		}
		gp_one_open++;
	#endif
	if( p_void < gp_p_cache_ONE_alloc || p_void >= gp_p_cache_STR_end )
	{
		GPF_delete( p_void, a_n_alloc_ONE );
		#ifdef _DEBUG_MUTI
			gp_mem_mutex.unlock();
		#else
			gp_one_close++;
			InterlockedExchange64( &gp_one_lock_trd_id, 0 );
		#endif
		return;
	} 
	if( p_void < gp_p_cache_STR_alloc )
	{
		gp_p_cache_ONE_empty = (char*)p_void;
		#ifdef _DEBUG_MUTI
			gp_mem_mutex.unlock();
		#else
			gp_one_close++;
			InterlockedExchange64( &gp_one_lock_trd_id, 0 );
		#endif
		return;
	} 
	
	gp_p_cache_STR_empty = (char*)p_void;
	#ifdef _DEBUG_MUTI
		gp_mem_mutex.unlock();
	#else
		gp_one_close++;
		InterlockedExchange64( &gp_one_lock_trd_id, 0 );
	#endif
}

void* operator new[]( size_t size )
{
	if( !size )
		return NULL;
	#ifdef _DEBUG_MUTI
		gp_mem_mutex.lock();
	#else
		ULL now_trd = (ULL)GetCurrentThreadId();
		volatile U4 n = 0;
		while( now_trd != InterlockedCompareExchange64( &gp_arr_lock_trd_id, now_trd, 0 ) )
			n++;
		while( gp_arr_close != gp_arr_open )
		{
			gp_arr_err++;
		}
		gp_arr_open++;
	#endif
	void* p_out = NULL;
	if( gp_p_cache_ARR_alloc ? size > GPD_CACHE_ONE : true )
	{
		p_out = GPF_alloc( size, a_n_alloc_ARRAY );
		#ifdef _DEBUG_MUTI
			gp_mem_mutex.unlock();
		#else
			gp_arr_close++;
			InterlockedExchange64( &gp_arr_lock_trd_id, 0 );
		#endif
		return p_out;
	}
	
	if( gp_p_cache_ARR_empty >= gp_p_cache_ARR_end )
	{
		gp_n_cache_ARR_out++;
		p_out = GPF_alloc( size, a_n_alloc_ARRAY );
		#ifdef _DEBUG_MUTI
			gp_mem_mutex.unlock();
		#else
			gp_arr_close++;
			InterlockedExchange64( &gp_arr_lock_trd_id, 0 );
		#endif
		return p_out;
	}

	p_out = gp_p_cache_ARR_empty;
	if( p_out < gp_p_cache_ARR_load )
	{
		gp_p_cache_ARR_empty = gp_p_cache_ARR_load;
		#ifdef _DEBUG_MUTI
			gp_mem_mutex.unlock();
		#else
			gp_arr_close++;
			InterlockedExchange64( &gp_arr_lock_trd_id, 0 );
		#endif
		return p_out;
	}
	
	gp_p_cache_ARR_empty = (gp_p_cache_ARR_load += GPD_CACHE_ONE);
	#ifdef _DEBUG_MUTI
		gp_mem_mutex.unlock();
	#else
		gp_arr_close++;
		InterlockedExchange64( &gp_arr_lock_trd_id, 0 );
	#endif
	return p_out;
}

void operator delete[](void *p_void)
{
	if (!p_void)
		return;
	#ifdef _DEBUG_MUTI
		gp_mem_mutex.lock();
	#else
		ULL now_trd = (ULL)GetCurrentThreadId();

		volatile U4 n = 0;
		while( now_trd != InterlockedCompareExchange64( &gp_arr_lock_trd_id, now_trd, 0 ) )
			n++;
		while( gp_arr_close != gp_arr_open )
		{
			gp_arr_err++;
		}
		gp_arr_open++;
	#endif
	
	if( !gp_p_cache_ARR_alloc )
	{
		GPF_delete( p_void, a_n_alloc_ARRAY );
		#ifdef _DEBUG_MUTI
			gp_mem_mutex.unlock();
		#else
			gp_arr_close++;
			InterlockedExchange64( &gp_arr_lock_trd_id, 0 );
		#endif
		return;
	}
	if( p_void < gp_p_cache_ARR_alloc || p_void >= gp_p_cache_ARR_end )
	{
		GPF_delete( p_void, a_n_alloc_ARRAY );
		#ifdef _DEBUG_MUTI
			gp_mem_mutex.unlock();
		#else
			gp_arr_close++;
			InterlockedExchange64( &gp_arr_lock_trd_id, 0 );
		#endif
		return; 
	}
	gp_p_cache_ARR_empty = (char*)p_void;
	#ifdef _DEBUG_MUTI
		gp_mem_mutex.unlock();
	#else
		gp_arr_close++;
		InterlockedExchange64( &gp_arr_lock_trd_id, 0 );
	#endif
}
#endif

char* GPF_MEM_GetMap( char*& p_lo, char*& p_hi )
{
	p_lo = p_mem_low;
	p_hi = p_mem_hi;
	p_mem_hi = p_mem_low = NULL;
	return a_memmap;
}
void GPF_MEM_Reset(void)
{
	memset(a_alloc, 0, sizeof(a_alloc));
	memset(a_free, 0, sizeof(a_free));
	memset(a_diff, 0, sizeof(a_diff));
	memset(a_n_alloc_ONE, 0, sizeof(a_n_alloc_ONE));
	memset(a_n_free_ONE, 0, sizeof(a_n_free_ONE));
	memset(a_n_diff_ONE, 0, sizeof(a_n_diff_ONE));
	memset(a_n_alloc_ARRAY, 0, sizeof(a_n_alloc_ARRAY));
	memset(a_n_free_ARRAY, 0, sizeof(a_n_free_ARRAY));
	memset(a_n_diff_ARRAY, 0, sizeof(a_n_diff_ARRAY));
	memset(a_memmap, 0, sizeof(a_memmap));
}

I8 GPF_MEM_NUSE(void)
{
	n_n_use_ONE = n_use = 0;
	memset(a_diff, 0, sizeof(a_diff));
	memset(a_n_diff_ONE, 0, sizeof(a_n_diff_ONE));
	memset(a_n_diff_ARRAY, 0, sizeof(a_n_diff_ARRAY));
	for( U4 i = 0; i < 32; i++ )
	{
		a_diff[i] += a_alloc[i] - a_free[i];
		n_use += a_diff[i];
		
		a_n_diff_ONE[i] += a_n_alloc_ONE[i] - a_n_free_ONE[i];
		n_n_use_ONE += a_n_diff_ONE[i];

		a_n_diff_ARRAY[i] += a_n_alloc_ARRAY[i] - a_n_free_ARRAY[i];
		n_n_use_ARRAY += a_n_diff_ARRAY[i];
	}
	return n_use;
}

U4 GPF_q2Q( U4 q )
{
	U4 Q =	 ( (q&0xf0)>>4 )
		|( (q&0x00003f000)>>( 8) )
		|( (q&0x00fc00000)>>(12) )
		|( (q&0x070000000)>>(12) );
	return Q;
}
U4 GPF_q2a0( U4 q )
{
	U4 a0 =	   (q&0x000000f)
		|( (q&0x0000f00)>>( 4) )
		|( (q&0x03c0000)>>(10) );
	return a0;
}


bool	gp_b_consol = false;

 U4 GPF_Bx7_LEN_xyz( U4 e0, U4 e1 )
{
	U4 sum = 0;
	if( (e1&0x7f) > (e0&0x7f) )
		sum += (e1&0x7f) - (e0&0x7f);
	else
		sum += (e0&0x7f) - (e1&0x7f);

	if( (e1&0x3f80) > (e0&0x3f80) )
		sum += ((e1&0x3f80) - (e0&0x3f80))>>7;
	else
		sum += ((e0&0x3f80) - (e1&0x3f80))>>7;

	if( (e1&0x1fc000) > (e0&0x1fc000) )
		sum += ((e1&0x1fc000) - (e0&0x1fc000))>>14;
	else
		sum += ((e0&0x1fc000) - (e1&0x1fc000))>>14;

	return sum;
}

 ULL* gpf_ull_cpy( ULL* p_dst, ULL* p_src, ULL n_src, ULL d_stp, ULL d_up, ULL w, ULL z )
 {
	n_src /= sizeof(ULL);
	d_stp /= sizeof(ULL);
	d_up /= sizeof(ULL);
	w /= sizeof(ULL);

	ULL	*p_se = p_src+n_src, *p_sz = p_src,
		*p_dz, *p_dy = p_dst,
		zw = z*w,
		W = w * sizeof(*p_src);
	while( p_src < p_se )
	{
		p_sz += zw;
		
		p_dz = p_dy;
		p_dy += d_up;

		while( p_src < p_sz )
		{
			gp_memcpy( p_dz, p_src, W );
			p_dz += d_stp;
			p_src += w;
		}
	}
	return p_dst;
 }
bool GPF_CON_on( void )
{
	if( !gp_b_consol )
		gp_b_consol = AllocConsole();

	return gp_b_consol;
}
I8 gp_con_n = 0;
void GPF_CON_Write( char *p_str )
{
	gp_con_n++;
	if( !gp_b_consol || (p_str ? !*p_str : true) )
		return;
	
	size_t i;
	unsigned long write;
	i = gp_strlen( p_str )+1; 
	
	//GPF_PRI_LO();
	WriteConsoleA(GetStdHandle(STD_OUTPUT_HANDLE), p_str, i, &write, 0);
	//GPF_PRI_HI();
}
I8 GPF_CON_n( void )
{
	return gp_con_n;
}
//void GPF_CON_null( void )
//{
//	if( !gp_b_consol )
//		return;
//	//void cls( HANDLE hConsole )
//	COORD coordScreen = { 0, 0 };
//	SetConsoleCursorPosition( GetStdHandle(STD_OUTPUT_HANDLE), coordScreen );
//}
void GPF_CON_pos( U2 x, U2 y )
{
	if( !gp_b_consol )
		return;
	//void cls( HANDLE hConsole )
	COORD coordScreen = { x, y };
	SetConsoleCursorPosition( GetStdHandle(STD_OUTPUT_HANDLE), coordScreen );
}
void GPF_CON_cls( void )
{
	if( !gp_b_consol )
		return;
	//void cls( HANDLE hConsole )
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD coordScreen = { 0, 0 };    /* here's where we'll home the
	                                    cursor */ 
	DWORD cCharsWritten;
	CONSOLE_SCREEN_BUFFER_INFO csbi; /* to get buffer info */ 
	DWORD dwConSize;                 /* number of character cells in
	                                    the current buffer */ 
	
	/* get the number of character cells in the current buffer */ 
	
	BOOL bSuccess = GetConsoleScreenBufferInfo( hConsole, &csbi );
	//PERR( bSuccess, "GetConsoleScreenBufferInfo" );
	dwConSize = csbi.dwSize.X * csbi.dwSize.Y;
	
	/* fill the entire screen with blanks */ 
	
	bSuccess = FillConsoleOutputCharacter( hConsole, (TCHAR) ' ',
	   dwConSize, coordScreen, &cCharsWritten );
	//PERR( bSuccess, "FillConsoleOutputCharacter" );
	
	/* get the current text attribute */ 
	
	bSuccess = GetConsoleScreenBufferInfo( hConsole, &csbi );
	//PERR( bSuccess, "ConsoleScreenBufferInfo" );
	
	/* now set the buffer's attributes accordingly */ 
	
	bSuccess = FillConsoleOutputAttribute( hConsole, csbi.wAttributes,
	   dwConSize, coordScreen, &cCharsWritten );
	//PERR( bSuccess, "FillConsoleOutputAttribute" );
	
	/* put the cursor at (0, 0) */ 
	
	bSuccess = SetConsoleCursorPosition( hConsole, coordScreen );
	//PERR( bSuccess, "SetConsoleCursorPosition" );
   
 }
void GPF_CON_LST_write( GPE_CLST n, GPE_GT_CnS b_cNs, char* p_str, U4 data )
{
	switch( n )
	{
		case GPE_CLST_GIVTRD_nDEC:
			return;
	}
	if( !gp_b_consol )
		return;
	if( n == GPE_CLST_STT_LST )
		GPF_CON_cls();
	else 
		GPF_CON_pos( 0, n );
	GPF_CON_FORMAT_Write( "[ %0.2d.] %s", n, b_cNs ? "CLNT |- ":"" );

	char	a_str[0x200],
		c = sprintf( a_str, "%s %0.8x", p_str, data );
	if( c > 70 )
	{
		c = 70;
		GPM_STRCPY( a_str+c-4, "... " );
	}
	//char* p_clnt = b_cNs ? "CLNT - " : "\t\tSERV - ";
	if(b_cNs)
		GPF_CON_pos( 15, n );
	else {
		GPF_CON_pos( 71, n );
		GPF_CON_Write( " -| SERV" );
		GPF_CON_pos( 71-c, n );
	}
	GPF_CON_Write( a_str );

	GPF_CON_pos( 0, 100 );
}
char gp_s_con_buff[10240];
void GPF_CON_FORMAT_Write(char *p_format, ...)
{
	if( !gp_b_consol )
		return;
	va_list vl;
	va_start(vl, p_format);
	
	if( vsprintf(gp_s_con_buff, p_format, vl) > 0 )
		GPF_CON_Write(gp_s_con_buff);
	else
		GPF_CON_Write("ERROR-FW");
	va_end(vl);
}

//GPCI4 GPCI4::cr2gr_1m_o( GPCI4 cr )
//{
//	GPCI4	ar = cr*-1,
//		ar2 = ar*ar;
//	double	axx = acos( (double)ar.x/sqrt((double)(ar2.x+ar2.y)) ) * (((double)gpe_giv_y1m)/PI) - (double)gpe_giv_z1m,
//		axy = acos( (double)ar.y/sqrt((double)(ar2.y+ar2.z)) ) * (((double)gpe_giv_y1m)/PI) - (double)gpe_giv_z1m,
//		axz = acos( (double)ar.z/sqrt((double)(ar2.y+ar2.z)) ) * (((double)gpe_giv_y1m)/PI) - (double)gpe_giv_z1m;
//	GPCI4 ax( axx, axy, axz, 0 );
//	ax.w = ax.maxi_absabs_xyz();
//	ax *= GPCI4( gpe_giv_lo1m, gpe_giv_lo1m, gpe_giv_lo1m, 1);
//	ax /= ax.w;
//	*this = gpe_giv_lo1m;
//	if( ax.x <= -gpe_giv_lo1m )
//	{
//		// 4 
//		// - L -
//		if( ax.y > -gpe_giv_lo1m && ax.y <= gpe_giv_lo1m )
//		if( ax.z > -gpe_giv_lo1m && ax.z <= gpe_giv_lo1m )
//		{
//			z = (double)gpe_giv_lo1m-axy; // ez miért minus?
//			y = (double)gpe_giv_lo1m-axz;
//			//z -= axis.y;
//			//y -= axis.z;
//			w = 4;
//			goto esc;
//		}
//	}
//	else if( ax.x >= gpe_giv_lo1m )
//	{
//		// 1 
//		// - R -
//		if( ax.y >= -gpe_giv_lo1m && ax.y < gpe_giv_lo1m )
//		if( ax.z >= -gpe_giv_lo1m && ax.z < gpe_giv_lo1m )
//		{
//			z = (double)gpe_giv_lo1m+axy;
//			y = (double)gpe_giv_lo1m+axz;
//			//z += axis.z;
//			//y += axis.y;
//			w = 1;
//			goto esc;
//		}
//	}
//
//	if( ax.y <= -gpe_giv_lo1m )
//	{
//		// 0 
//		// - D -
//		if(  ax.x >= -gpe_giv_lo1m && ax.x < gpe_giv_lo1m )
//		if(  ax.z >= -gpe_giv_lo1m && ax.z < gpe_giv_lo1m )
//		{
//			z = (double)gpe_giv_lo1m+axz;
//			y = (double)gpe_giv_lo1m+axx;
//			//z += axis.x;
//			//y += axis.z;
//			w = 0;
//			goto esc;
//		}
//	}
//	else if( ax.y >= gpe_giv_lo1m )
//	{
//		// 3
//		// - U -
//		if( ax.x > -gpe_giv_lo1m && ax.x <= gpe_giv_lo1m )
//		if( ax.z > -gpe_giv_lo1m && ax.z <= gpe_giv_lo1m )
//		{
//			z = (double)gpe_giv_lo1m-axx;
//			y = (double)gpe_giv_lo1m-axz;
//			//z -= axis.x;
//			//y -= axis.z;
//			w = 3;
//			goto esc;
//		}
//	}
//
//	if( ax.z <= -gpe_giv_lo1m )
//	{
//		// 5 
//		// - B -
//		if( ax.y >  -gpe_giv_lo1m && ax.y <=  gpe_giv_lo1m )
//		if( ax.x >= -gpe_giv_lo1m && ax.x <  gpe_giv_lo1m  )
//		{
//			z = (double)gpe_giv_lo1m-axy;
//			y = (double)gpe_giv_lo1m+axx;
//			//z -= axis.y;
//			//y += axis.x;
//			w = 5;
//			goto esc;
//		}
//	}
//	else if( ax.z >= gpe_giv_lo1m )
//	{
//		// 2 
//		// - F -
//		if( ax.x >  -gpe_giv_lo1m && ax.x <=  gpe_giv_lo1m )
//		if( ax.y >= -gpe_giv_lo1m && ax.y <   gpe_giv_lo1m )
//		{
//			z = (double)gpe_giv_lo1m-axx;
//			y = (double)gpe_giv_lo1m+axy;
//			//z -= axis.x;
//			//y += axis.y;
//			w = 2;
//			goto esc;
//		}
//	}
//	return *this;
//esc:
//	x = 0;
//	//x = (air-gr2cr_1m()).len_xyz();
//	//if( x%gpe_giv_1m )
//	//{
//	//	x += gpe_giv_1m-x%gpe_giv_1m;
//	//}
//	return *this;
//}
GPCI4 GPCI4::cr2gr_1m_o( GPCI4 cr )
{

	GPCV3 axyz = cr.qube_ax();
	GPCI4 ax = axyz;
	ax.w = ax.maxi_absabs_xyz();
	ax *= GPCI4( gpe_giv_cr1m, gpe_giv_cr1m, gpe_giv_cr1m, 1);
	ax /= ax.w;
	*this = gpe_giv_cr1m;

	if( ax.x < 0 )
	{
		// 4 
		// - L -
		if( ax.y > -gpe_giv_cr1m && ax.y <= gpe_giv_cr1m )
		if( ax.z > -gpe_giv_cr1m && ax.z <= gpe_giv_cr1m )
		{
			z -= axyz.y; // miért lesz negatív?
			y -= axyz.z;
			w = 4;
			goto esc;
		}
	}
	else if( ax.x > 0 )
	{
		// 1 
		// - R -
		if( ax.y >= -gpe_giv_cr1m && ax.y < gpe_giv_cr1m )
		if( ax.z >= -gpe_giv_cr1m && ax.z < gpe_giv_cr1m )
		{
			z += axyz.z;
			y += axyz.y;
			w = 1;
			goto esc;
		}
	}

	if( ax.y < 0 )
	{
		// 0 
		// - D -
		if(  ax.x >= -gpe_giv_cr1m && ax.x < gpe_giv_cr1m )
		if(  ax.z >= -gpe_giv_cr1m && ax.z < gpe_giv_cr1m )
		{
			z += axyz.z;
			y += axyz.x;
			w = 0;
			goto esc;
		}
	}
	else if( ax.y > 0 )
	{
		// 3
		// - U -
		if( ax.x > -gpe_giv_cr1m && ax.x <= gpe_giv_cr1m )
		if( ax.z > -gpe_giv_cr1m && ax.z <= gpe_giv_cr1m )
		{
			z -= axyz.x;
			y -= axyz.z;
			w = 3;
			goto esc;
		}
	}

	if( ax.z < 0 )
	{
		// 5 
		// - B -
		if( ax.y >  -gpe_giv_cr1m && ax.y <=  gpe_giv_cr1m )
		if( ax.x >= -gpe_giv_cr1m && ax.x <  gpe_giv_cr1m  )
		{
			z -= axyz.y;
			y += axyz.x;
			w = 5;
			goto esc;
		}
	}
	else if( ax.z > 0 )
	{
		// 2 
		// - F -
		if( ax.x >  -gpe_giv_cr1m && ax.x <=  gpe_giv_cr1m )
		if( ax.y >= -gpe_giv_cr1m && ax.y <   gpe_giv_cr1m )
		{
			z -= axyz.x;
			y += axyz.y;
			w = 2;
			goto esc;
		}
	}
	
esc:
	x = 0;
	return *this;
}
U1 GPCI4::n_RADgr( const GPCI4& p1, const GPCI4& p0, I8 div )
{
	GPCI4 dm = p0.ins_45dgr( this[3], this[2], this[1], this[0], p1, div, false );
	I8 n = dm.x == dm.y ? 1 : 2;
	if( n == 1 )
		return 1;

	return 2;

	dm = this[2].gr2ar( div, 1 ).ins_45dgr( this[5], this[4], this[3], this[2], p1, div, false );
	n += (dm.x == dm.y ? 0 : 1);
	if( n == 2 )
		return 2;

	return 3;
}
GPCI4 GPCI4::cr2gr_1m( GPCI4 cr, bool b_dbg )
{
	x = cr.len_xyz()-gpe_giv_cr1m;
	y = z = gpe_giv_cr1m;
	GPCI4 ax = cr.qube_ax(); //axyz;
	
	if( ax.x <= -gpe_giv_cr1m )
	{
		// 4 - L -
		if( ax.y > -gpe_giv_cr1m && ax.y <= gpe_giv_cr1m )
		if( ax.z > -gpe_giv_cr1m && ax.z <= gpe_giv_cr1m )
		{
			z -= ax.y; // miért lesz negatív?
			y -= ax.z;
			w = 4;
			return *this;
		}
	}
	else if( ax.x >= gpe_giv_cr1m )
	{
		// 1 - R -
		if( ax.y >= -gpe_giv_cr1m && ax.y < gpe_giv_cr1m )
		if( ax.z >= -gpe_giv_cr1m && ax.z < gpe_giv_cr1m )
		{
			z += ax.z;
			y += ax.y;
			w = 1;
			return *this;
		}
	}

	if( ax.y <= -gpe_giv_cr1m )
	{
		// 0 - D -
		if(  ax.x >= -gpe_giv_cr1m && ax.x < gpe_giv_cr1m )
		if(  ax.z >= -gpe_giv_cr1m && ax.z < gpe_giv_cr1m )
		{
			z += ax.z;
			y += ax.x;
			w = 0;
			return *this;
		}
	}
	else if( ax.y >= gpe_giv_cr1m )
	{
		// 3 - U -
		if( ax.x > -gpe_giv_cr1m && ax.x <= gpe_giv_cr1m )
		if( ax.z > -gpe_giv_cr1m && ax.z <= gpe_giv_cr1m )
		{
			z -= ax.x;
			y -= ax.z;
			w = 3;
			return *this;
		}
	}

	if( ax.z <= -gpe_giv_cr1m )
	{
		// 5 - B -
		if( ax.y >  -gpe_giv_cr1m && ax.y <= gpe_giv_cr1m )
		if( ax.x >= -gpe_giv_cr1m && ax.x <  gpe_giv_cr1m  )
		{
			z -= ax.y;
			y += ax.x;
			w = 5;
			return *this;
		}
	}
	else if( ax.z >= gpe_giv_cr1m )
	{
		// 2 - F -
		if( ax.x >  -gpe_giv_cr1m && ax.x <=  gpe_giv_cr1m )
		if( ax.y >= -gpe_giv_cr1m && ax.y <   gpe_giv_cr1m )
		{
			z -= ax.x;
			y += ax.y;
			w = 2;
			return *this;
		}
	}
esc:
	w = 6;
	return *this;
}
GPCI4 GPCI4::cr2gr_sid_1m6( GPCI4 air, char sid, GPCV3& axyz )
{
	GPCI4	ax = axyz;
	ax.w = ax.maxi_absabs_xyz();
	ax *= GPCI4( gpe_giv_lo1m, gpe_giv_lo1m, gpe_giv_lo1m, 1);
	ax /= ax.w;
	
	*this = gpe_giv_lo1m;
	switch( w = sid )
	{
		case 0:	// - D -
			if( abs(axyz.z) > gpe_giv_lo1m ) 
				axyz.z = ax.z;
			if( abs(axyz.x) > gpe_giv_lo1m ) 
				axyz.x = ax.x;
			z += axyz.z;
			y += axyz.x;
			break;
		case 1: 
			// - R -
			if( abs(axyz.z) > gpe_giv_lo1m ) 
				axyz.z = ax.z;
			if( abs(axyz.y) > gpe_giv_lo1m ) 
				axyz.y = ax.y;
			z += axyz.z;
			y += axyz.y;
			break;
		case 2: 
			// - F -
			if( abs(axyz.x) > gpe_giv_lo1m ) 
				axyz.x = ax.x;
			if( abs(axyz.y) > gpe_giv_lo1m ) 
				axyz.y = ax.y;
			z -= axyz.x;
			y += axyz.y;
			break;
		case 3:
			// - U -
			if( abs(axyz.x) > gpe_giv_lo1m ) 
				axyz.x = ax.x;
			if( abs(axyz.z) > gpe_giv_lo1m ) 
				axyz.z = ax.z;
			z -= axyz.x;
			y -= axyz.z;
			break;
		case 4: 
			// - L -
			if( abs(axyz.z) > gpe_giv_lo1m ) 
				axyz.z = ax.z;
			if( abs(axyz.y) > gpe_giv_lo1m ) 
				axyz.y = ax.y;
			z -= axyz.y; // miért lesz negatív?
			y -= axyz.z;
			break;
		case 5: 
			// - B -
			if( abs(axyz.x) > gpe_giv_lo1m ) 
				axyz.x = ax.x;
			if( abs(axyz.y) > gpe_giv_lo1m ) 
				axyz.y = ax.y;
			z -= axyz.y;
			y += axyz.x;
			break;
	}
esc:
	x = 0;
	return *this;
}
GPCI4 GPCI4::cr2gr_sid_1m( GPCI4 cr, char sid, bool b_dbg )
{
	cr2gr_1m( cr, b_dbg );
	I8 gz = z, gy = y;
	switch( sid )
	{
		case 0:	// - D -
			switch( w )
			{
				case 0:
					break;
				case 1:
					y += gpe_giv_z1m;
					break;
				case 2:
					z = gy + gpe_giv_z1m;
					y = gpe_giv_z1m1s - gz;
					break;
				case 3:
					break;
				case 4:
					z = gpe_giv_z1m1s - gy;
					y = gz - gpe_giv_z1m;
					break;
				case 5:
					z -= gpe_giv_z1m;
					break;
			}
			break;
		case 1:	// - R -
			switch( w )
			{
				case 0:
					y -= gpe_giv_z1m;
					break;
				case 1:
					break;
				case 2:
					z += gpe_giv_z1m;
					break;
				case 3:
					z = gpe_giv_z1m1s - gy;
					y = gz + gpe_giv_z1m;
					break;
				case 4:
					break;
				case 5:
					z = gy - gpe_giv_z1m;
					y = gpe_giv_z1m1s - gz;
					break;
			}
			break;
		case 2: 
			// - F -
			switch( w )
			{
				case 0:
					z = gpe_giv_z1m1s - gy;
					y = gz - gpe_giv_z1m;
					break;
				case 1:
					z -= gpe_giv_z1m;
					break;
				case 2:
					break;
				case 3:
					y += gpe_giv_z1m;
					break;
				case 4:
					z = gy + gpe_giv_z1m;
					y = gpe_giv_z1m1s - gz;
					break;
				case 5:
					break;
			}
			break;
		case 3:
			// - U -
			switch( w )
			{
				case 0:
					break;
				case 1:
					z = gy - gpe_giv_z1m;
					y = gpe_giv_z1m1s - gz;
					break;
				case 2:
					y -= gpe_giv_z1m;
					break;
				case 3:
					break;
				case 4:
					z += gpe_giv_z1m;
					break;
				case 5:
					z =  gpe_giv_z1m1s - gy;
					y =  gz + gpe_giv_z1m;
					break;
			}
			break;
		case 4: 
			// - L -
			switch( w )
			{
				case 0:
					z = gy + gpe_giv_z1m;
					y = gpe_giv_z1m1s - gz;
					break;
				case 1:
					break;
				case 2:
					z = gpe_giv_z1m1s - gy;
					y = gz;
					break;
				case 3:
					z -= gpe_giv_z1m;
					break;
				case 4:
					break;
				case 5:
					y += gpe_giv_z1m;
					break;
			}
			break;
		case 5: 
			// - B -
			switch( w )
			{
				case 0:
					z += gpe_giv_z1m;
					break;
				case 1:
					z = gpe_giv_z1m1s - gy;
					y = gz + gpe_giv_z1m;
					break;
				case 2:
					break;
				case 3:
					z = gy - gpe_giv_z1m;
					y = gpe_giv_z1m1s - gz;
					break;
				case 4:
					y -= gpe_giv_z1m;
					break;
				case 5:
					break;
			}
			break;
	}
esc:
	w = sid;
	x = 0;
	return *this;
}
GPCI4 GPCI4::gr2cr_1m( void ) const
{
	double	rz = (((double)z/(double)gpe_giv_y1m) + 0.25)*PI,
		ry = (((double)y/(double)gpe_giv_y1m) + 0.25)*PI,
		ctgZ = cos(rz)/sin(rz),
		ctgY = cos(ry)/sin(ry),
		ctgZY2 = ctgZ*ctgZ + ctgY*ctgY,
		rx = (double)gpe_giv_cr1m / sqrt(1.0+ctgZY2), dif; 
	GPCI4 cr;

	ctgZ *= -rx;
	ctgY *= -rx;
	ctgZY2 = ctgZ*ctgZ + ctgY*ctgY;
	dif = rx;
	rx = sqrt( (double)gpe_giv_cr1m2 - ctgZY2 );
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
	cr.w = cr.len_xyz();
	return cr;
}


GPS_IDXnOP gp_a_idxNop[] = {
	{ 1, 0, 0, -0.325 },
	{ 1, 0, 0,  0.5 },
	{ 1, 0, 0,  1.325 },

	{ 2, 0, 0, -0.325 },
	{ 2, 0, 0,  0.5 },
	{ 2, 0, 0,  1.325 },

	{ 3, 2, 2, -0.325 },
	{ 3, 2, 2,  0.5 },
	{ 3, 2, 2,  1.325 },

	{ 6, 2, 2, -0.325 },
	{ 6, 2, 2,  0.5 },
	{ 6, 2, 2,  1.325 },

	{ 1, 3, 3, -0.325 },
	{ 1, 3, 3,  0.5 },
	{ 1, 3, 3,  1.325 },

	{ 7, 3, 3, -0.325 },
	{ 7, 3, 3,  0.5 },
	{ 7, 3, 3,  1.325 },

	{ 5, 7, 7, -0.325 },
	{ 5, 7, 7,  0.5 },
	{ 5, 7, 7,  1.325 },

	{ 6, 7, 7, -0.325 },
	{ 6, 7, 7,  0.5 },
	{ 6, 7, 7,  1.325 },

	{ 1, 5, 5, -0.325 },
	{ 1, 5, 5,  0.5 },
	{ 1, 5, 5,  1.325 },

	{ 4, 5, 5, -0.325 },
	{ 4, 5, 5,  0.5 },
	{ 4, 5, 5,  1.325 },

	{ 0, 4, 4, -0.325 },
	{ 0, 4, 4,  0.5 },
	{ 0, 4, 4,  1.325 },

	{ 6, 4, 4, -0.325 },
	{ 6, 4, 4,  0.5 },
	{ 6, 4, 4,  1.325 },
};
GPC_GIO_VX32C* GPC_GIO_VX32C::vx324_gr( GPCV3* p_v3n9, GPCI4& gr, I8 mul, I8 div, GPCV4& hi, GPCV4& lo, I8 l )
{
	// -----------------------------------
	// gr CUBE
	// a jelölö négyzetet hozza létre
	//------------------------------------
	GPC_GIO_VX32C *p_vxe = this;
	p_v3n9[0].gr_qube( gr, mul, div, l ); 
	for( I8 i = 0; i < GPM_N(gp_a_idxNop); i += 3 )
	{
		p_vxe->pos = (
			(p_v3n9[gp_a_idxNop[i+0].bas]-p_v3n9[gp_a_idxNop[i+0].sub])*gp_a_idxNop[i+0].mul
			+p_v3n9[gp_a_idxNop[i+0].sub]
			-p_v3n9[8]
			)*0.6 + p_v3n9[8];
		p_vxe++;
		p_vxe->pos = (
			(p_v3n9[gp_a_idxNop[i+1].bas]-p_v3n9[gp_a_idxNop[i+1].sub])*gp_a_idxNop[i+1].mul
			+p_v3n9[gp_a_idxNop[i+1].sub]
			-p_v3n9[8]
			)*0.6 + p_v3n9[8];
		p_vxe++;
		p_vxe->pos = p_vxe[-1].pos;
		p_vxe++;
		p_vxe->pos = (
			(p_v3n9[gp_a_idxNop[i+2].bas]-p_v3n9[gp_a_idxNop[i+2].sub])*gp_a_idxNop[i+2].mul
			+ p_v3n9[gp_a_idxNop[i+2].sub]
			-p_v3n9[8]
			)*0.6 + p_v3n9[8];
		p_vxe++;

		p_vxe[-4].rgba = p_vxe[-1].rgba = lo;
		p_vxe[-2].rgba = p_vxe[-3].rgba = hi;
	}
	return p_vxe;
}


#define GPD_GIV_STP 2
#define GPD_GIV_PLUS 1

void* GPF_MEM_GetTex( U4* p_heapmap )
{
	char *p_lo, *p_hi, *p_heap = GPF_MEM_GetMap(p_lo, p_hi), c, h;
	if (U4 n = p_hi - p_lo)
	for (U4 *p_i = (U4*)(p_heapmap + (1024 * 1024 - 1) - (p_lo - p_heap)), *p_e = p_i - n; p_i > p_e; p_i--)
	{
		c = *p_lo;
		h = p_lo[1024*1024];
		p_lo++;
		if (!c)
			*p_i = 0xff000000;
		else if (c < 0)
		{
			if( h == 1 )
				*p_i = 0xff008080;
			else
				*p_i = 0xff00ff00 | (0xff + c);
		} else {
			if( h == 1 )
				*p_i = 0xff0000ff;
			else
				*p_i = 0xffff0000 | (0xff - c);
		}
	}
	return p_heapmap;
}




void* cpy_up(void* p_dst, void* p_src, ULL n)
{
	char	*p_s = (char*)p_src + n,
		*p_d = (char*)p_dst + n;

	while (p_d > p_dst)
	{
		p_d--;
		p_s--;
		*p_d = *p_s;
	}

	return p_dst;
}
void* cpy_down(void* p_dst, void* p_src, ULL n)
{
	for (char *p_s = (char*)p_src, *p_se = p_s + n, *p_d = (char*)p_dst, *p_de = p_d + n; p_d < p_de; p_d++, p_s++)
	{
		*p_d = *p_s;
	}
	return p_dst;
}

U4 GPF_SWAP_U4( char* p_s )
{
	U4 tmp;
	char* p_d = (char*)&tmp;
	p_d[3] = p_s[0];
	p_d[2] = p_s[1];
	p_d[1] = p_s[2];
	p_d[0] = p_s[3];
	return tmp;
}
float GPF_SWAP_F4( char* p_s )
{
	float tmp;
	char* p_d = (char*)&tmp;
	p_d[3] = p_s[0];
	p_d[2] = p_s[1];
	p_d[1] = p_s[2];
	p_d[0] = p_s[3];
	return tmp;
}
U2 GPF_SWAP_U2( U2 u2 )
{
	return GPF_SWAP_U2( (char*)&u2 );
	//U2 tmp;
	//char* p_d = (char*)&tmp;
	//p_d[1] = p_s[0];
	//p_d[0] = p_s[1];
	//return tmp;
}
U2 GPF_SWAP_U2( char* p_s )
{
	U2 tmp;
	char* p_d = (char*)&tmp;
	p_d[1] = p_s[0];
	p_d[0] = p_s[1];
	return tmp;
}
U1* GPF_MEM_skip( U1* p_dst, ULL nbyte )
{
	U1	* p_d = p_dst,
		* p_de = p_dst+nbyte,
		n1 = ((ULL)p_d)&0x7;
	if( n1 )
	{
		U1 *p_d1e = p_d+n1;
		while( p_d < p_d1e )
		{
			if( *p_d );
				return p_d;
			p_d++;
		}
		if( p_d >= p_de )
			return p_d;
	}
	ULL n8 = (p_de-p_d)/8;
	if( n8 )
	{
		ULL	*p_d8 = (ULL*)p_d,
			*p_d8e = p_d8+n8;
		while( p_d8 < p_d8e )
		{
			if( *p_d8 )
				break;
			p_d8++;
		}
		p_d = (U1*)p_d8;
		if( p_d >= p_de )
			return p_d;
	}

	while( p_d < p_de )
	{
		if( *p_d );
			return p_d;
		p_d++;
	}
	return p_d;
}
U1* GPF_MEM_or( U1* p_dst, U1 u1, ULL nbyte )
{
	U1	* p_d = p_dst,
		* p_de = p_dst+nbyte,
		n1 = ((ULL)p_d)&0x7;
	if( n1 )
	{
		U1 *p_d1e = p_d+n1;
		while( p_d < p_d1e )
		{
			*p_d |= u1;
			p_d++;
		}
		if( p_d >= p_de )
			return p_dst;
	}

	ULL n8 = (p_de-p_d)/8;
	if( n8 )
	{
		ULL	u8 = 0x0101010101010101*u1,
			*p_d8 = (ULL*)p_d,
			*p_d8e = p_d8+n8;

		while( p_d8 < p_d8e )
		{
			*p_d8 |= u8;
			p_d8++;
		}
		p_d = (U1*)p_d8;
		if( p_d >= p_de )
			return p_dst;
	}

	while( p_d < p_de )
	{
		*p_d |= u1;
		p_d++;
	}
	return p_dst;
}
U1* GPF_MEM_and( U1* p_dst, U1 u1, ULL nbyte )
{
	U1	* p_d = p_dst,
		* p_de = p_dst+nbyte,
		n1 = ((ULL)p_d)&0x7;
	if( n1 )
	{
		U1 *p_d1e = p_d+n1;
		while( p_d < p_d1e )
		{
			*p_d &= u1;
			p_d++;
		}
		if( p_d >= p_de )
			return p_dst;
	}

	ULL n8 = (p_de-p_d)/8;
	if( n8 )
	{
		ULL	u8 = 0x0101010101010101*u1,
			*p_d8 = (ULL*)p_d,
			*p_d8e = p_d8+n8;

		while( p_d8 < p_d8e )
		{
			*p_d8 &= u8;
			p_d8++;
		}
		p_d = (U1*)p_d8;
		if( p_d >= p_de )
			return p_dst;
	}

	while( p_d < p_de )
	{
		*p_d &= u1;
		p_d++;
	}
	return p_dst;
}
U1* GPF_MEM_max( U1* p_dst, U1* p_1, U1* p_2, ULL n_byte )
{
	U1 light;
	for( ULL i = 0; i < n_byte; i++ )
	{
		light = p_1[i];
		if( light < p_2[i] )
			light = p_2[i];

		p_dst[i] = light; 
	}
	return p_dst;
}
void* gpf_memXinc( void* p_d, void* p_s, ULL ynz, ULL w16, ULL Ws1X16 )
{
	ULL a = ynz+w16+Ws1X16;
	return p_d;
}
//void GPF_PRI_LO( void )
//{
//	return;
//	HANDLE h = GetCurrentThread();
//	int pri = GetThreadPriority( h );
//
//	if( pri != THREAD_PRIORITY_ABOVE_NORMAL )
//	while( !SetThreadPriority( h, THREAD_PRIORITY_ABOVE_NORMAL ) ) 
//	{
//		Sleep(0);
//	}
//}
//
//void GPF_PRI_HI( void )
//{
//	return;
//	HANDLE h = GetCurrentThread();
//	int pri = GetThreadPriority( h );
//
//	if( pri != THREAD_PRIORITY_TIME_CRITICAL )
//	while( !SetThreadPriority( h, THREAD_PRIORITY_TIME_CRITICAL ) ) 
//	{
//		Sleep(0);
//	}
//}


void* GPF_MEM_set( void* p_dst, I8 n, void* p_src, I8 n_byte )
{
	switch( n_byte )
	{
		case 0:
			return p_dst;
		case 1:
			return gp_memset_u1( p_dst, *(U1*)p_src, n );
		case 2:
			return gp_memset_u2( p_dst, *(U2*)p_src, n*2 );
		case 4:
			return gp_memset_u4( p_dst, *(U4*)p_src, n*4 );
		case 8:
			return gp_memset_q( p_dst, *(U8*)p_src, n*8 );
		default:
			if( n_byte > 0x100 )
				break;

			if( n_byte&0xf )
				break;
			
			switch( n_byte )
			{
				case 0x10:
					return gp_memset_0x10( p_dst, p_src, n*0x10 );
				case 0x20:
					return gp_memset_0x20( p_dst, p_src, n*0x20 );
				case 0x30:
					return gp_memset_0x30( p_dst, p_src, n*0x30 );
				case 0x40:
					return gp_memset_0x40( p_dst, p_src, n*0x40 );
				case 0x50:
					return gp_memset_0x50( p_dst, p_src, n*0x50 );
				case 0x60:
					return gp_memset_0x60( p_dst, p_src, n*0x60 );
				case 0x70:
					return gp_memset_0x70( p_dst, p_src, n*0x70 );
				case 0x80:
					return gp_memset_0x80( p_dst, p_src, n*0x80 );
				case 0x90:
					return gp_memset_0x90( p_dst, p_src, n*0x90 );
				case 0xa0:
					return gp_memset_0xa0( p_dst, p_src, n*0xa0 );
				case 0xb0:
					return gp_memset_0xb0( p_dst, p_src, n*0xb0 );
				case 0xc0:
					return gp_memset_0xc0( p_dst, p_src, n*0xc0 );
				case 0xd0:
					return gp_memset_0xd0( p_dst, p_src, n*0xd0 );
				case 0xe0:
					return gp_memset_0xe0( p_dst, p_src, n*0xe0 );
				case 0xf0:
					return gp_memset_0xf0( p_dst, p_src, n*0xf0 );
				case 0x100:
					return gp_memset_0x100( p_dst, p_src, n*0x100 );
			}
	}
	//if( gp_qz( p_src, n_byte ) )
	//	return gp_zero( p_dst, n*n_byte );
		
	char	*p_d = (char*)p_dst,
		*p_s = (char*)p_src,
		*p_e = p_d+n*n_byte;
	
	gp_memcpy( p_d, p_s, n_byte );
	p_s = p_d;
	p_d += n_byte;
	//I8 nt;
	while( p_d < p_e)
	{
		//nt = n/GPD_MEMCPY;
		gp_memcpy( p_d, p_s, n_byte );

		p_d += n_byte;

		n_byte *= 2;
		if( n_byte > p_e-p_d )
			n_byte = p_e-p_d;
	}
	return p_dst;
}
I8 GPF_ALFA_find( char* p_begin, char* p_end, GPE_ALFA alfa )
{
	if( !p_begin || p_begin >= p_end )
		return 0;
	
	if( !alfa )
		return GPD_STRLEN( p_begin );
	
	char s_str[0x20], s_van[] = "  \a", *p_found = p_begin, *p_fe;
	I8 n = GPF_ALFA2STR( s_str, alfa );
	if( n < 1 )
		return 0;
	
	GPE_ALFA found;
	s_van[0] = *s_str;
	s_van[1] = *s_str-GPD_UP;
	p_found += GPF_STR_VAN( p_begin, p_end, s_van );

	while( p_found < p_end )
	{
		p_fe = p_found+n;
		if( p_fe > p_end )
			break;
		found = GPF_STR2ALFA( p_found, p_fe, NULL );
		if( found == alfa )
		{
			return p_found-p_begin;
		} else 
			p_found++;
		p_found += GPF_STR_VAN( p_found, p_end, s_van );
	}

	return p_end-p_begin;
}
I8 GPF_ALFA_n( char* p_begin, char* p_end, GPE_ALFA alfa )
{
	if( !p_begin || p_begin >= p_end )
		return 0;
	if( !alfa )
		return 0;

	p_end = p_begin+GPD_VAN( p_begin, "\a" );
	if( p_begin >= p_end )
		return 0;
	
	char s_str[0x100], s_van[] = "  \a", *p_found = p_begin, *p_fe;
	I8 n = GPF_ALFA2STR( s_str, alfa ), n_alfa = 0;
	if( n < 1 )
		return 0;
	
	GPE_ALFA found;
	s_van[0] = *s_str;
	s_van[1] = *s_str-GPD_UP;
	p_found += GPF_STR_VAN( p_begin, p_end, s_van );
	while( p_found < p_end )
	{
		p_fe = p_found+n;
		if( p_fe > p_end )
			break;
		found = GPF_STR2ALFA( p_found, p_fe, NULL );
		if( found == alfa )
		{
			n_alfa++;
			p_found += n;
		} else 
			p_found++;
		p_found += GPF_STR_VAN( p_found, p_end, s_van );
	}

	return n_alfa;
}
U1 gp_a_alfa_add[0x100];
U1* GPF_a_alfa_init( void )
{
	for( int i = 0; i < 0x100; i++ )
	{
		if( i >= 'A' && i <= 'Z')
		{
			// egész biztos kisbetüt többet használnak
			gp_a_alfa_add[i] = 'A'-1;//GPD_UP;
			continue;
		} 
		else if( i >= 'a' && i <= 'z' )
		{
			gp_a_alfa_add[i] = 'a'-1;
			continue;
		}
		gp_a_alfa_add[i] = 0;
	}
	return gp_a_alfa_add;
}
GPE_ALFA GPF_STR2ALFA(char* p_str, char* p_end, char** pp_str )
{
	if( p_str ? !*p_str : true )
	{
		if( pp_str )
			*pp_str = p_str;
	
		return GPE_ALFA_zero;
	}

	I8 out = 0;
	U1 c;
	if( p_end < p_str )
		p_end = p_str + GPD_STRLEN(p_str);

	while( p_str < p_end)
	{
		c = *(U1*)p_str;
		if( !gp_a_alfa_add[c] )
			break;
		c -= gp_a_alfa_add[c];
		//if (c >= 'A' && c <= 'Z')
		//{
		//	// egész biztos kisbetüt többet használnak
		//	c += GPD_UP;
		//}
		//if (c >= 'a' && c <= 'z')
		//{
		//	c -= 'a' - 1;
		//}
		//else
		//	break;

		out *= GPD_ALFA;
		out += c;

		p_str++;
	}

	if (pp_str)
		*pp_str = p_str;

	return (GPE_ALFA)out;
}

ULL GPF_ALFA2STR(char* p_out, I8 d0)
{
	if( !p_out )
		return 0;
	if( !d0 )
	{
		*p_out = 0;
		return 0;
	}
	char	gp_s_buff[0x40],
		*p_buff = gp_s_buff + 0x3f,
		*p_end = p_buff;

	*p_end = 0;
	bool b_minus = false;
	if( d0 < 0 )
	{
		b_minus = true;
		d0 *= -1;
	}

	I8 d1;
	while( d0 )
	{
		d1 = d0;
		d0 = (d0-1)/GPD_ALFA;
		p_buff--;
		*p_buff = (d1-d0*GPD_ALFA)+'\`';
		//d1 = d0%GPD_ALFA;
		//d1 += '@';
		//p_buff--;
		//*p_buff = d1;
		//d0 /= GPD_ALFA;
	}

	if( b_minus )
	{
		p_buff--;
		*p_buff = '-';
	}
	U2 n = p_end-p_buff;
	gp_memcpy( p_out, p_buff, n+1 );
	return n;
}
I8 GPF_STR2I8( char* p_str, char** pp_str )
{
	if( !p_str )
		return 0;
	I8 i8 = strtol( p_str, &p_str, 10 );
	if( !i8 )
	{
		switch( *p_str )
		{
			case 'x':
			case 'X':
				p_str++;
				i8 = strtol( p_str, &p_str, 16 );
				break;
			case 'b':
			case 'B':
				p_str++;
				i8 = strtol( p_str, &p_str, 2 );
				break;
			case 'd':
			case 'D':
				p_str++;
				i8 = strtol( p_str, &p_str, 10 );
				break;
		}
	}
	if( pp_str )
		*pp_str = p_str;
	return i8;
}

char* GPF_STR( char* p_kill, char* p_str )
{
	if( !p_str )
	{
		GPM_DELARRAY( p_kill );
		return NULL;
	}

	GPS_ALLOC	*p_mem = p_kill ? (GPS_ALLOC*)p_kill : NULL,
		*p_alloc = p_mem ? p_mem - 1 : NULL;
	
	I8 n_str = GPD_STRLEN( p_str )+1, n_diff = 0; 
	
	if( p_alloc )
	if( p_alloc->p_alloc == p_alloc )
	{
		n_diff = p_alloc->size - n_str;
		if( n_diff < 0 || n_diff > 0x100 )
		{
			GPM_DELARRAY( p_kill );
		}
	}

	if( !p_kill )
		p_kill = new char[n_str];

	if( p_kill )
		GPM_STRCPY( p_kill, p_str );

	return p_kill;
}
char* GPF_STR_icmp( char* p_kill, char* p_str )
{
	if( !p_str )
	{
		GPM_DELARRAY( p_kill );
		return NULL;
	}
	if( p_kill )
	{
		if( !_stricmp( p_kill, p_str ) )
			return p_kill;
	}

	return GPF_STR( p_kill, p_str );
}
char* GPF_STRnSTRn( char* p_dst, ULL n_dst,  char* p_src, ULL n_src )
{
	if( p_dst ? !n_dst : true )
	{
		n_dst = GPD_STRLEN( p_dst );
		if( !n_dst )
			return NULL;
	}
	if( p_src ? !n_src : true )
	{
		n_src = GPD_STRLEN( p_src );
		if( !n_src )
			return NULL;
	}
	if( n_src > n_dst )
		return NULL;

	char* p_end = p_dst+n_src, *p_e;
	if( n_src == 1 )
	{
		while( p_dst < p_end )
		{
			if( *p_dst == *p_src )
				return p_dst;
			p_dst++;
		}
		return NULL;
	}
	
	ULL n;
	while( p_dst+n_src <= p_end )
	{
		if( *p_dst != *p_src )
		{
			p_dst++;
			continue;
		}
		
		p_e = p_dst+1;
		for( n = 1; n < n_src; n++ )
		{
			if( p_dst[n] == *p_src )
				p_e = p_dst+n;

			if( p_dst[n] != p_src[n] )
				break;
		}
		if( n == n_src )
			return p_dst;

		p_dst = p_e;
	}
	return NULL;
}
char* GPF_STR_replace(char* p_str, char* p_end, char* p_o, char* p_n )
{
	if( !p_str || !p_o || !p_n )
		return p_str;
	
	if( p_str >= p_end )
		return NULL;

	if( !*p_o || !*p_n )
		return p_str;
	I8 n_o = GPD_STRLEN(p_o),n_n = GPD_STRLEN(p_n);
	if( n_o > n_n )
		n_o = n_n;

	U1 s_new[0x100];
	gp_zero(s_new, sizeof(s_new));
	
	for( U1* p_v = (U1*)p_o, *p_e = p_v + n_o; p_v < p_e; p_v++ )
	{
		s_new[*p_v] = *p_n;
		p_n++;
	}
	p_n = p_str;
	while( p_n < p_end )
	{
		if( s_new[*p_n] )
			*p_n = s_new[*p_n];
		p_n++;
	}
	return p_str;
}
I8 GPF_STR_VAN( char* p_str, char* p_end, char* p_van )
{
	if( !p_str )
		return 0;
	if( p_str >= p_end && !p_van )
		return p_end - p_str;
	
	bool ab_van[0x100];
	gp_zero( ab_van, sizeof(ab_van) );
	for (U1* p_v = (U1*)p_van, *p_e = p_v + GPD_STRLEN(p_van); p_v < p_e; p_v++)
	{
		ab_van[*p_v] = true;
	}
	*ab_van = true;

	p_van = p_str; // csak takarékosság, p_begin-t ne keljen foglalni
	while( p_str < p_end )
	{
		if( ab_van[*p_str] )
			return p_str-p_van;
		p_str++;
	}
	return p_str-p_van;
}
I8 GPF_VAN_NINCS( char* p_str, char* p_vn )
{
	if( !p_str )
		return NULL;

	if( !p_vn )
		return GPD_STRLEN( p_str );

	char* p_begin = p_str;
	p_str += GPD_VAN( p_str, p_vn );
	p_str += GPD_NINCS( p_str, p_vn );

	return p_str-p_begin;
}
I8 GPF_STR_BLOCK( char* p_str, char* p_end, U1 end, char* p_comment )
{
	I8 n_end = p_end-p_str, n_find;
	if( !end )
	{
		// ha az end == 0 azt jelenti a p_str->p_end között 0-t keres
		n_find = GPD_STRLEN( p_str );
		if( n_end < n_find  )
			return n_find;

		return n_find;
	}

	char s_van[0x100];
	gp_zero( s_van, sizeof(s_van) );
	
	if( *p_str == end )
	{
		// ha egyenlõ pl. " vagy '
		*s_van = end;
		return GPF_STR_VAN( p_str, p_end, s_van );
	} 

	char* p_begin = p_str;
	s_van[*(U1*)p_str] = 1;
	s_van[end] = -1;
	p_str++;
	I8 n_deep = 1;
	
	if( p_comment )
	while( *p_comment )
	{
		s_van[*(U1*)p_comment] = 2;
		p_comment++; 
	}

	*s_van = 3;
	while( p_str < p_end )
	{
		switch( s_van[*(U1*)p_str] )
		{
				
			case -1:	// out
				n_deep--;
				if( !n_deep )
					return p_str-p_begin;
				break;
			case 0:	// normal step
				break;
			case 1:	// in
				n_deep++;
				break;
			case 3:	//end
				return n_end;
				// komment
			default:
				p_str += GPF_VAN_NINCS( p_str, "\r\n" );
		}

		p_str++;
	}

	return n_end;
}
I8 GPF_STR_VISZ(char* p_str, char* p_start, char* p_van )
{
	if( !p_str )
		return 0;
	
	if( !p_start )
		p_start = p_str+GPD_STRLEN(p_str);

	if( p_str >= p_start )
		return p_start - p_str;

	bool ab_van[0x100];
	gp_zero(ab_van, sizeof(ab_van));
	for (U1* p_v = (U1*)p_van, *p_e = p_v + GPD_STRLEN(p_van); p_v < p_e; p_v++)
	{
		ab_van[*p_v] = true;
	}

	while( p_start > p_str )
	{
		p_start--;
		if( ab_van[*p_start] )
			return p_start - p_str;
	}

	return 0;
}
I8 GPF_STR_VISZ_NINCS(char* p_str, char* p_start, char* p_nincs )
{
	if( !p_str )
		return 0;
	if( !p_start )
		p_start = p_str+GPD_STRLEN(p_str);
	if( p_str >= p_start )
		return p_start - p_str;

	U1 ab_nincs[0x100];
	gp_memset_u1( ab_nincs, true, sizeof(ab_nincs));
	//memset(ab_nincs, true, sizeof(ab_nincs));
	for( U1* p_n = (U1*)p_nincs, *p_e = p_n + GPD_STRLEN(p_nincs); p_n < p_e; p_n++ )
	{
		ab_nincs[*p_n] = false;
	}

	while( p_start > p_str )
	{
		p_start--;
		if( ab_nincs[*p_start] )
			return (p_start - p_str);
	}

	return 0;
}
int GPF_STR_len( char* p_str, char* p_end, int& x, int& y, int& t )
{
	I8 len = 0, xx = 0;
	x = y = 1;
	t = 0;
	if (!p_str)
		return 0;

	I8 o = GPD_STRLEN(p_str);
	if( p_end < p_str )
		p_end = p_str + o;
	
	char* p_row = p_str;
	while (p_str < p_end)
	{
		len = GPD_VAN( p_str, "\t\r\n\a" );
		if( len )
		{
			xx += len;
			p_str += len;
			continue;
		}

		if (*p_str == '\t')
		{
			xx = (4 + xx) & 0xfffffffffffffffc;
			p_str++;
			continue;
		}

		if( x < xx )
			x = xx;
		xx = 1;
		y += *p_str == '\a' ? 2 : 1;
		
		p_str++;
		p_row = p_str;
	}
	if (x < xx)
		x = xx;
	t = x / 4;
	return o;
}
I8 GPF_STR_lenI8( char* p_str, char* p_end, I8& x, I8& y, I8& t )
{
	I8 len = 0, xx = 0;
	x = y = 1;
	t = 0;
	if (!p_str)
		return 0;

	I8 o = GPD_STRLEN(p_str);
	if( p_end < p_str )
		p_end = p_str + o;
	
	char* p_row = p_str;
	while (p_str < p_end)
	{
		len = GPD_VAN( p_str, "\t\r\n\a" );
		if( len )
		{
			xx += len;
			p_str += len;
			continue;
		}

		if (*p_str == '\t')
		{
			xx = (4 + xx) & 0xfffffffffffffffc;
			p_str++;
			continue;
		}

		if( x < xx )
			x = xx;
		xx = 1;
		y += *p_str == '\a' ? 2 : 1;
		
		p_str++;
		p_row = p_str;
	}
	if (x < xx)
		x = xx;
	t = x / 4;
	return o;
}
U1* GPF_U1_replace( U1* p_dst, U1* p_u1, U1* p_u1e, U1* p_r0, I8 n_r0 )
{
	if( !n_r0 )
		return (U1*)gp_memcpy( p_dst, p_u1, p_u1e-p_u1 );

	U1 a_rep[0x100];
	for( U2 i = 0; i < 0x100; i++ )
	{
		a_rep[i] = i;
	}
	n_r0 &= ~1;
	for( I8 i = 0; i < n_r0; i += 2 )
	{
		a_rep[p_r0[i]] = p_r0[i+1]; 
	}

	for( U1* p_u = p_u1, *p_d = p_dst; p_u < p_u1e; p_u++, p_d++ )
	{
		if( *p_d == a_rep[*p_u] )
			continue;

		*p_d = a_rep[*p_u];
	}

	return p_dst;
}
ULL gp_a_mask0x10[] = 
{
	0xffffffffffffffff, 0xffffffffffffffff, 0xffffffffffffffff, 0xffffffffffffffff, 
	0xffffffffffffffff, 0xffffffffffffffff, 0xffffffffffffff00, 0xffffffffffffffff, 
				       
	0xffffffffffffffff, 0xffffffffffffffff, 0xffffffffffff0000, 0xffffffffffffffff, 
	0xffffffffffffffff, 0xffffffffffffffff, 0xffffffffffff0000, 0xffffffffffffffff, 
				       
	0xffffffffffffffff, 0xffffffffffffffff, 0xffffffff00000000, 0xffffffffffffffff, 
	0xffffffffffffffff, 0xffffffffffffffff, 0xffffffff00000000, 0xffffffffffffffff, 
	0xffffffffffffffff, 0xffffffffffffffff, 0xffffffff00000000, 0xffffffffffffffff, 
	0xffffffffffffffff, 0xffffffffffffffff, 0xffffffff00000000, 0xffffffffffffffff, 
				       
	0xffffffffffffffff, 0xffffffffffffffff, 0x0000000000000000, 0xffffffffffffffff, 
	0xffffffffffffffff, 0xffffffffffffffff, 0x0000000000000000, 0xffffffffffffffff, 
	0xffffffffffffffff, 0xffffffffffffffff, 0x0000000000000000, 0xffffffffffffffff, 
	0xffffffffffffffff, 0xffffffffffffffff, 0x0000000000000000, 0xffffffffffffffff, 
	0xffffffffffffffff, 0xffffffffffffffff, 0x0000000000000000, 0xffffffffffffffff, 
	0xffffffffffffffff, 0xffffffffffffffff, 0x0000000000000000, 0xffffffffffffffff, 
	0xffffffffffffffff, 0xffffffffffffffff, 0x0000000000000000, 0xffffffffffffffff, 
	0xffffffffffffffff, 0xffffffffffffffff, 0x0000000000000000, 0xffffffffffffffff, 
};
U1* GPF_U1_set2x2x2( U1* p_d, I8 n_d, I8 n_z, U1 set )
{
	I8	n_y = n_z*n_z;
	U1	*p_dnx, *p_snxz, *p_dnxz, *p_out = p_d;
	for( I8 y = 0; y < n_d; y++ )
	{
		p_dnxz = p_d+n_y;
		for( I8 z = 0; z < n_d; z++ )
		{
			p_dnx = p_d+n_z;
			if( !set )
				gp_zero( p_d, n_d );
			else
				gp_memset_u1( p_d, set, n_d );
			p_d = p_dnx;	
					
		}
		p_d = p_dnxz;
	}
	return p_out;
}
__declspec(align(16)) ULL gp_a_mask[2];
GPC3DLO* GPC3DLO::lo_fill( ULL a2, GPE_STF* p_stuff, U1* p_light, I8 n_z, I8 n_line )
{
	if( !this )
		return this;
	ULL	shl = a2&0xf,
		a20 = a2-shl;
	bool	b_mask = false;
	if( !p_stuff )
	{
		b_mask = true;
		gp_memcpy( gp_a_mask, ((U1*)(gp_a_mask0x10 + n_z*4 + 2)) - shl, 0x10 );
		gp_ddd_mask( a_stf+a20, gp_a_mask, n_z );
		//GPF_U1_set2x2x2( (U1*)(a_3d+a2), n_z, n_line, 0 );
	} else {
		GPF_U1_erode_smpl( (U1*)(a_stf+a2), n_z, n_line, (U1*)p_stuff );
	}
				
	if( !p_light )
	{
		if( !b_mask )
			gp_memcpy( gp_a_mask, ((U1*)(gp_a_mask0x10 + n_z*4 + 2)) - shl, 0x10 );
		
		gp_ddd_mask( a_light+a20, gp_a_mask, n_z );
		//GPF_U1_set2x2x2( a_light+a2, n_z, n_line, 0 );
		return this;
	}
		
	GPF_U1_dilate_2x2x2( a_light+a2, n_z, n_line, p_light );
	return this;
}
U1* GPF_U1_erode_smpl( U1* p_d, I8 n_d, I8 n_z, U1* p_s )
{
	I8	n_y = n_z*n_z;
	U1	*p_ssnxz, *p_dnx, *p_ssnx, *p_dnxz, *p_out = p_d;
	for( I8 y = 0; y < n_d; y++ )
	{
		p_ssnxz = p_s+n_y*2;
		p_dnxz = p_d+n_y;
		for( I8 z = 0; z < n_d; z++ )
		{
			p_ssnx = p_s+n_z*2,
			p_dnx = p_d+n_z;
			for( I8 x = 0; x < n_d; x++ )
			{
				if(	  *p_s		&& p_s[0x001]	&& p_s[0x010]	&& p_s[0x011] 
					&& p_s[0x100]	&& p_s[0x101]	&& p_s[0x110]	&& p_s[0x111] )
					*p_d = *p_s;
				else
					*p_d = 0;
				p_s+=2;	//._._
				p_d++;	//..
			}
			p_s = p_ssnx;
			p_d = p_dnx;	
					
		}
		p_s = p_ssnxz;	
		p_d = p_dnxz;
	}
	return p_out;
}
U1* GPF_U1_erode_hard( U1* p_d, I8 n_d, I8 n_z, U1* p_s )
{
	I8	n_y = n_z*n_z;
	U1	*p_snx, *p_dnx, *p_snxz, *p_dnxz, *p_out = p_d;
	for( I8 y = 0; y < n_d; y++ )
	{
		p_snxz = p_s+n_y*2;
		p_dnxz = p_d+n_y;
		for( I8 z = 0; z < n_d; z++ )
		{
			p_snx = p_s+n_z*2,
			p_dnx = p_d+n_z;
			for( I8 x = 0; x < n_d; x++ )
			{
				*p_d = GPF_U1_erode_2x2x2_hard( p_s, 1, n_y, n_z );
				//if( *p_d )
				//{
				//	GPF_U1_EQU_2x2x2( p_s, 1, n_y, n_z, *p_d );
				//}
				p_s+=2;	//._._
				p_d++;	//..
			}
			p_s = p_snx;	
			p_d = p_dnx;	
					
		}
		p_s = p_snxz;	
		p_d = p_dnxz;
	}
	return p_out;
}
U1* GPF_U1_erode_soft( U1* p_d, I8 n_d, I8 n_z, U1* p_s )
{
	I8	n_y = n_z*n_z;
	U1	*p_snx, *p_dnx, *p_snxz, *p_dnxz, *p_out = p_d;
	for( I8 y = 0; y < n_d; y++ )
	{
		p_snxz = p_s+n_y*2;
		p_dnxz = p_d+n_y;
		for( I8 z = 0; z < n_d; z++ )
		{
			p_snx = p_s+n_z*2,
			p_dnx = p_d+n_z;
			for( I8 x = 0; x < n_d; x++ )
			{
				*p_d = GPF_U1_erode_2x2x2_soft( p_s, 1, n_y, n_z );
				//if( *p_d )
				//{
				//	GPF_U1_EQU_2x2x2( p_s, 1, n_y, n_z, *p_d );
				//}
				p_s+=2;	//._._
				p_d++;	//..
			}
			p_s = p_snx;	
			p_d = p_dnx;	
					
		}
		p_s = p_snxz;	
		p_d = p_dnxz;
	}
	return p_out;
}
U1* GPF_U1_dilate_2x2x2( U1* p_d, I8 n_d, I8 n_z, U1* p_s )
{
	I8	n_y = n_z*n_z;
	U1	*p_snx, *p_dnx, *p_snxz, *p_dnxz, *p_out = p_d;
	for( I8 y = 0; y < n_d; y++ )
	{
		p_snxz = p_s+n_y*2;
		p_dnxz = p_d+n_y;
		for( I8 z = 0; z < n_d; z++ )
		{
			p_snx = p_s+n_z*2,
			p_dnx = p_d+n_z;
			for( I8 x = 0; x < n_d; x++ )
			{
				*p_d = GPF_U1_dilate_2x2x2( p_s, 1, n_y, n_z );
				p_s+=2;	//._._
				p_d++;	//..
			}
			p_s = p_snx;	
			p_d = p_dnx;	
					
		}
		p_s = p_snxz;	
		p_d = p_dnxz;
	}
	return p_out;
}

char* GPF_STR_replace(char* p_str, ULL n_str, char o, char c)
{
	if (!p_str)
		return p_str;

	for (char *p_s = p_str, *p_e = p_s + n_str; p_s < p_e; p_s++ )
	if (*p_s == o)
		*p_s = c;
	
	return p_str;
}
static char gp_s_ABC[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
bool GPF_STR_VAN_ABC( char* p_str )
{
	return ( GPD_VAN( p_str, gp_s_ABC ) < GPD_STRLEN( p_str ) );
}
char* GPF_STR_PLUS_MINUS( char* p_buff, I8 n )
{
	if( !p_buff )
		return NULL;

	char* p_str = p_buff;
	*p_buff = 0;

	if( n == 0 )
		return p_buff;
	
	p_str += sprintf( p_str, "%s%lld", n < 0 ? "-":"+", abs(n) );

	return p_buff;
}
U4* GPF_HISTI( U4* p_histi, U1* p_pat, I8 n )
{
	if( !p_histi )
		p_histi = new U4[0x100];
	for( U1 *p_u = p_pat, *p_ue = p_u + n; p_u < p_ue; p_u++ )
	{
		p_histi[*p_u]++;
	}
	return p_histi;
}
float GPF_NOP( float f1, float f2, float f3 )
{
	return f1;
}
float GPF_MEDIAN( float f1, float f2, float f3 )
{
	return max( min(f1,f2) , min(f2,f3) );
}
float GPF_AVGR( float f1, float f2, float f3 )
{
	return (f1+f2+f3)/3.0f;
}
float GPF_MIN_MAX_MIDLE( float f1, float f2, float f3 )
{
	return (min(min(f1,f2),min(f2,f3)) + max(max(f1,f2),max(f2,f3)))/2.0f;
}

double GPC_FIx64::avarage( ULL n )
{
	// vigyázz ez sorrendezi az értékeket
	if( !this || n < 1 )
		return 0.0f;

	if( n < 2 )
	{
		return this[0].v;
	}
	else if( n < 3 )
	{
		return ((this[1].v-this[0].v)/2.0f + this[0].v);
	}

	double avgr = 0.0f;
	for( I8 j = 0; j < n; j++ ) 
		avgr += this[j].v;

	return (avgr / (double)n);
}

double GPC_FIx64::deviation( ULL n, double& avgr )
{
	// vigyázz ez sorrendezi az értékeket
	if( !this || n < 1 )
		return 0.0f;

	avgr = avarage(n);
	double	devi = 0.0f,
		dif;

	for( I8 i = 0; i < n; i++ )
	{
		dif = avgr-this[i].v;
		devi += dif*dif;
	}
	return sqrt(devi/(double)n);
}

double GPC_FIx64::median( ULL n, bool b_inc )
{
	// b_inc == true - incrementált növekvõ sorban leszenk
	// b_inc == false - dekrementáslt csökkenõ sorban leszenk (nem definiálod akkor ez, azaz csökenõ )
	if( !this || n < 1 )
		return 0.0f;
	
	if( n < 2 )
		return this->v;
	
	I8 i, j, l, r;
	GPC_FIx64* p_tree = new GPC_FIx64[n+1], x;
	
	r = n;
	while( r >= 1 )
	{
		// az öszes elem számát "r" elosztom 2-tõvel kerekítés nélkül
		// ezzel tudom, hogy a soron következõ szint meddig csökkenhet "l" lesz a küszöb
		l = ldiv( r, 2).quot;
		while( r > 0 )
		{
			// a következõ elemet berakom az x-be
			x = this[r-1];

			i = r;
			if ( i*2 <= n )
			{
				// i mutatja majd azt a helyet ahonva az x et be akarnám rakni
				while ( i*2 <= n )
				{
					j = i*2;
					// azt jelenti, hogy az i nek van ága
					if ( j+1 <= n )
						if ( p_tree[j+1].v < p_tree[j].v )
							j++; // azt jelenti, hogy két ága is volt, és a magasabb indexün kissebb volt az érték
					
					if( x.v > p_tree[j].v )
					{
						// azt jelenti hogy az x nagyobb volt mint az ág ezért lejebb rakom a tartalmát
						p_tree[i] = p_tree[j];
						// és következõ ciklusban az ágról akarom folytatni
						i = j;
					} else {
						// azt jelenti, nincs ennél magasabb szám az ágakon
						//p_tree[i] = x;
						break;
					}
				}//while
				p_tree[i] = x;
			} else {
				p_tree[r] = x;
			}

			// r-et csökkentem jöhet a következõ elem
			r--;
		}
	}
	l = ldiv( r, 2).quot;
	r = n;
	i = 1;
	while ( r >= 1 )
	{
		x = p_tree[r];
		p_tree[r] = p_tree[1];
		r--;
		l = 1;
		while ( l <= r )
		{
			i = l*2;
			if ( i <= r )
			{
				if ( i+1 <= r )
					if ( p_tree[i+1].v < p_tree[i].v )
						i++;
				
				if ( x.v > p_tree[i].v )
				{
					p_tree[l] = p_tree[i];
				} else {
					p_tree[l] = x;
					break;
				}
			} else {
				p_tree[l] = x;
				break;
			}
			l = i;
		}//while
		
	}
	

	if( b_inc )
	{
		for( ULL i = 0; i < n; i++ )
		{
			this[i] = p_tree[n-i];
		}
		GPM_DELARRAY( p_tree );
		if( n < 3 )
			return avarage( n );
		return this[n/2].v;
	}

	GPC_FIx64* p_src = p_tree+1;
	for( I8 i = 0; i < n; i++ )
	{
		this[i] = p_src[i];
	}
	GPM_DELARRAY( p_tree );
	if( n < 3 )
		return avarage( n );

	return this[n/2].v;
}


I8 GPC_IIx64::avarage( ULL n )
{
	// vigyázz ez sorrendezi az értékeket
	if( !this || n < 1 )
		return 0.0f;

	if( n < 2 )
	{
		return this[0].vi;
	}
	else if( n < 3 )
	{
		return ((this[1].vi-this[0].vi)/2.0f + this[0].vi);
	}

	double avgr = 0.0f;
	for( ULL j = 0; j < n; j++ ) 
		avgr += this[j].vi;

	return (I8)(avgr / (double)n);
}

I8 GPC_IIx64::deviation( ULL n, float& avgr )
{
	// vigyázz ez sorrendezi az értékeket
	if( !this || n < 1 )
		return 0.0f;

	avgr = avarage(n);
	double	devi = 0.0f,
		dif;

	for( I8 i = 0; i < n; i++ )
	{
		dif = avgr-this[i].vi;
		devi += dif*dif;
	}
	return (I8)sqrt(devi/(double)n);
}

I8 GPC_IIx64::median( ULL n, bool b_inc )
{
	// b_inc == true - incrementált növekvõ sorban leszenk
	// b_inc == false - dekrementáslt csökkenõ sorban leszenk (nem definiálod akkor ez, azaz csökenõ )
	if( !this || n < 1 )
		return 0;
	
	if( n < 2 )
		return this->vi;
		
	ULL i, j, l, r;
	GPC_IIx64* p_tree = new GPC_IIx64[n+1],
		x;
	
	r = n;
	while( r >= 1 )
	{
		// az öszes elem számát "r" elosztom 2-tõvel kerekítés nélkül
		// ezzel tudom, hogy a soron következõ szint meddig csökkenhet "l" lesz a küszöb
		l = ldiv( r, 2).quot;
		while( r > 0 )
		{
			// a következõ elemet berakom az x-be
			x = this[r-1];

			i = r;
			if ( i*2 <= n )
			{
				// i mutatja majd azt a helyet ahonva az x et be akarnám rakni
				while ( i*2 <= n )
				{
					j = i*2;
					// azt jelenti, hogy az i nek van ága
					if ( j+1 <= n )
						if ( p_tree[j+1].vi < p_tree[j].vi )
							j++; // azt jelenti, hogy két ága is volt, és a magasabb indexün kissebb volt az érték
					
					if( x.vi > p_tree[j].vi )
					{
						// azt jelenti hogy az x nagyobb volt mint az ág ezért lejebb rakom a tartalmát
						p_tree[i] = p_tree[j];
						// és következõ ciklusban az ágról akarom folytatni
						i = j;
					} else {
						// azt jelenti, nincs ennél magasabb szám az ágakon
						//p_tree[i] = x;
						break;
					}
				}//while
				p_tree[i] = x;
			} else {
				p_tree[r] = x;
			}

			// r-et csökkentem jöhet a következõ elem
			r--;
		}
	}
	l = ldiv( r, 2).quot;
	r = n;
	i = 1;
	while ( r >= 1 )
	{
		x = p_tree[r];
		p_tree[r] = p_tree[1];
		r--;
		l = 1;
		while ( l <= r )
		{
			i = l*2;
			if ( i <= r )
			{
				if ( i+1 <= r )
					if ( p_tree[i+1].vi < p_tree[i].vi )
						i++;
				
				if ( x.vi > p_tree[i].vi )
				{
					p_tree[l] = p_tree[i];
				} else {
					p_tree[l] = x;
					break;
				}
			} else {
				p_tree[l] = x;
				break;
			}
			l = i;
		}//while
		
	}
	
	if( b_inc )
	{
		for( I8 i = 0; i < n; i++ )
		{
			this[i] = p_tree[n-i];
		}
		GPM_DELARRAY( p_tree );
		if( n < 3 )
			return avarage( n );

		return this[n/2].vi;
	}

	GPC_IIx64* p_src = p_tree+1;
	for( I8 i = 0; i < n; i++ )
	{
		this[i] = p_src[i];
	}
	GPM_DELARRAY( p_tree );
	if( n < 3 )
		return avarage( n );

	return this[n/2].vi;
}

long double GPF_Ramanujan( long double a, long double b )
{
	if( a < 0.01 ) return 4.0 * b;
	if( b < 0.01 ) return 4.0 * a;
	return PI * ( 3*(a+b) - sqrt((3.0*a + b) * ( a + 3.0*b )) );
}