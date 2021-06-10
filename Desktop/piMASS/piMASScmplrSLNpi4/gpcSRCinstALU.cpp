#include "gpcSRC.h"
#include "gpcSRCinst.h"
#include "gpcSRClnk.h"
#include "gpccrs.h"
extern U1 gpaALFsub[];
extern char gpaALF_H_sub[];



gpINST* gpMEM::instALU() {
	//if(bSTDcout){gpdCOUT << stdALU "+";
	gpINST& ins = pINST[pc];
	pc++;

	U4 sOF = gpaCsz[ins.cID];
	I8 off;
	U1* p_src, *p_dst, *pALL = lzyMEM.p_alloc, *pPC = ((U1*)&ins);
	switch( ins.mS ) {
		case gpeEA_Dn: p_src = (U1*)&(pD[ins.iS]); break;
		case gpeEA_An: p_src = (U1*)&(pA[ins.iS]); break;

		case gpeEA_IAnI:
		case gpeEA_IAnIp:
		case gpeEA_sIAnI:
		case gpeEA_d16IAnI:
		case gpeEA_d16IAnDnI: {
				//p_src = pALL;
				switch( ins.mS ) {
					case gpeEA_IAnI: 						off = pA[ins.iS]; break;
						//p_src = pALL + pA[ins.iS]; break;
					case gpeEA_IAnIp: 						off = pA[ins.iS]; pA[ins.iS]+=sOF; break;
						//p_src = pALL + pA[ins.iS];
						//pA[ins.iS]+=sOF;
						//break;
					case gpeEA_sIAnI:						pA[ins.iS]-=sOF; off = pA[ins.iS]; break;
						//pA[ins.iS]-=sOF;
						//p_src = pALL + pA[ins.iS];
						//break;
					case gpeEA_d16IAnI:						off = pA[ins.iS] + ins.a8x2.x; break;
						//p_src = pALL + pA[ins.iS] + ins.a8x2.x;
						//break;
					case gpeEA_d16IAnDnI:					off = pA[ins.iS] + pD[ins.xS] + ins.a8x2.x; break;
						//p_src = pALL + pA[ins.iS] + pD[ins.xS] + ins.a8x2.x;
						//break;
					default:
						off = -1;
						break;
				}
				if( (off>0) && (off<lzyMEM.nLD()) )
					p_src = pALL + off;
				else
					p_src = NULL;
			} break;
		case gpeEA_d16IPcI:
			p_src = pPC + ins.a8x2.x;
			break;
		case gpeEA_d16IPcDnI:
			p_src = pPC + pD[ins.iS] + ins.a8x2.x;
			break;
		case gpeEA_W:
		case gpeEA_L:
			p_src = (U1*)&ins.a8x2.x;
			break;

		case gpeEA_OFF:
		default:
			p_src = NULL;
			break;
	}


	switch( ins.mD ) {
		case gpeEA_Dn: p_dst = (U1*)&(pD[ins.iD]); break;
		case gpeEA_An: p_dst = (U1*)&(pA[ins.iD]); break;

		case gpeEA_IAnI:
		case gpeEA_IAnIp:
		case gpeEA_sIAnI:
		case gpeEA_d16IAnI:
		case gpeEA_d16IAnDnI: {
				switch( ins.mD ) {
					case gpeEA_IAnI: 		off = pA[ins.iD]; break;
					case gpeEA_IAnIp: 		off = pA[ins.iD]; pA[ins.iD]+=sOF; break;
					case gpeEA_sIAnI:		pA[ins.iD]-=sOF; off = pA[ins.iD]; break;
					case gpeEA_d16IAnI:		off = pA[ins.iD] + ins.a8x2.y; break;
					case gpeEA_d16IAnDnI:	off = pA[ins.iD] + pD[ins.xD] + ins.a8x2.y; break;
					default:
						off = -1;
						break;
				}
				if( (off>0) && (off<lzyMEM.nLD()) )
					p_dst = pALL + off;
				else
					p_dst = NULL;
			} break;

		/*case gpeEA_IAnI: p_dst = pALL + pA[ins.iD]; break;
		case gpeEA_IAnIp:
			p_dst = pALL + pA[ins.iD];
			pA[ins.iD]+=sOF;
			break;
		case gpeEA_sIAnI:
			pA[ins.iD]-=sOF;
			p_dst = pALL + pA[ins.iD];
			break;
		case gpeEA_d16IAnI:
			p_dst = pALL + pA[ins.iD] + ins.a8x2.y;
			break;
		case gpeEA_d16IAnDnI:
			p_dst = pALL + pA[ins.iD] + pD[ins.xD] + ins.a8x2.y;
			break;*/

		case gpeEA_d16IPcI:
			p_dst = pPC + ins.a8x2.y;
			break;
		case gpeEA_d16IPcDnI:
			p_dst = pPC + pD[ins.iD] + ins.a8x2.y;
			break;
		case gpeEA_W:
		case gpeEA_L:
			p_dst = (U1*)&ins.a8x2.y;
			break;

		case gpeEA_OFF:
		default:
			p_dst = NULL;
			break;
	}

	switch(gpaOPgrp[ins.op]) {
		case gpeOPid_CMP:{
				ccr = gpeCCR_null;
				switch( ins.cID ) {
					case gpeCsz_f:
					case gpeCsz_d: {
							if( ins.cID == gpeCsz_f )
								d8 = *((float*)p_dst) - *((float*)p_src);
							else
								d8 = *((double*)p_dst) - *((double*)p_src);
							if( d8 != 0.0 ) {
								if( d8 < 0.0 )
									ccr = gpeCCR_n;
							} else
								ccr = gpeCCR_z;
						} break;

					default: {
						switch( ins.cID ) {
							case gpeCsz_Q: i8 = *((I8*)p_dst) - *((I8*)p_src); break;
							case gpeCsz_q: i8 = *((U8*)p_dst) - *((U8*)p_src); break;
							case gpeCsz_L: i8 = *((I4*)p_dst) - *((I4*)p_src); break;
							case gpeCsz_4:
							case gpeCsz_l: i8 = *((U4*)p_dst) - *((U4*)p_src); break;
							case gpeCsz_W: i8 = *((I2*)p_dst) - *((I2*)p_src); break;
							case gpeCsz_w: i8 = *((U2*)p_dst) - *((U2*)p_src); break;
							case gpeCsz_B: i8 = *((I1*)p_dst) - *((I1*)p_src); break;
							case gpeCsz_b: i8 = *((U1*)p_dst) - *((U1*)p_src); break;
							default: break;
						}
						if( i8 ) {
							if( i8 < 0 )
								ccr = gpeCCR_n;
						} else
							ccr = gpeCCR_z;

					} break;
				}

			} break;
		case gpeOPid_SEQ:{
				if( !p_src  )
					break;
				p_dst = p_src;
				switch(ins.op) {
					case gpeOPid_SEQ:{
							bool b = ccr&gpeCCR_z;
							if( !b ) {
								gpmZn( p_dst, sOF );
								break;
							}

							switch( ins.cID ) {
								case gpeCsz_Q: *((I8*)p_dst) = 0x7fffFFFFffffFFFF; break;
								case gpeCsz_q: *((U8*)p_dst) = 0xffffFFFFffffFFFF; break;
								case gpeCsz_L: *((I4*)p_dst) = 0x7fffFFFF; break;
								case gpeCsz_4:
								case gpeCsz_l: *((U4*)p_dst) = 0xffffFFFF; break;
								case gpeCsz_W: *((I2*)p_dst) = 0x7FFF; break;
								case gpeCsz_w: *((U2*)p_dst) = 0xFFFF; break;
								case gpeCsz_B: *((I1*)p_dst) = 0x7F; break;
								case gpeCsz_b: *((U1*)p_dst) = 0xFF; break;
								default: break;
							}
						} break;
					case gpeOPid_SNE:{
							bool b = ccr&gpeCCR_z;
							if( b ) {
								gpmZn( p_dst, sOF );
								break;
							}

							switch( ins.cID ) {
								case gpeCsz_Q: *((I8*)p_dst) = 0x7fffFFFFffffFFFF; break;
								case gpeCsz_q: *((U8*)p_dst) = 0xffffFFFFffffFFFF; break;
								case gpeCsz_L: *((I4*)p_dst) = 0x7fffFFFF; break;
								case gpeCsz_4:
								case gpeCsz_l: *((U4*)p_dst) = 0xffffFFFF; break;
								case gpeCsz_W: *((I2*)p_dst) = 0x7FFF; break;
								case gpeCsz_w: *((U2*)p_dst) = 0xFFFF; break;
								case gpeCsz_B: *((I1*)p_dst) = 0x7F; break;
								case gpeCsz_b: *((U1*)p_dst) = 0xFF; break;
								default: break;
							}
						} break;
					default: break;
				}
			} break;
		case gpeOPid_EXT:{
				switch(ins.op) {
					case gpeOPid_EXTB:
						if( p_src ) {
							*(I8*)p_src = *(I1*)p_src;
						} break;
					case gpeOPid_EXT:
						if( p_src ) {
							*(I8*)p_src = *(I2*)p_src;
						} break;
					case gpeOPid_EXTL:
						if( p_src ) {
							*(I8*)p_src = *(I4*)p_src;
						} break;
				}

			} break;
		case gpeOPid_entry:{
				switch(ins.op) {
					case gpeOPid_dot:
						// find OBJ
						break;
					case gpeOPid_jsr: {
							instJSR( pALL + pA[0], ins );
						} break;
					default: break;
				}
			} break;
		case gpeOPid_mov:{
				if( !p_dst || !p_src )
					break;
				switch(ins.op) {
					case gpeOPid_mov:
					default:
						gpmMcpy( p_dst, p_src, sOF );
						break;
				}
			} break;
		case gpeOPid_mul:{
				if( !p_dst || !p_src )
					break;
				U8 mx = gpaCszMX[ins.cID];
				switch( ins.cID ) {
					case gpeCsz_Q: gpmMUL( I8, ins.op, mx ); break;
					case gpeCsz_q: gpmMUL( U8, ins.op, mx ); break;
					case gpeCsz_L: gpmMUL( int, ins.op, mx ); break;
					case gpeCsz_l: gpmMUL( U4, ins.op, mx ); break;
					case gpeCsz_W: gpmMUL( I2, ins.op, mx ); break;
					case gpeCsz_w: gpmMUL( U2, ins.op, mx ); break;
					case gpeCsz_B: gpmMUL( I1, ins.op, mx ); break;
					case gpeCsz_b: gpmMUL( U1, ins.op, mx ); break;
					case gpeCsz_f: gpmFMUL( F, ins.op, mx ); break;
					case gpeCsz_d: gpmFMUL( D, ins.op, mx ); break;
					case gpeCsz_4: gpmFMUL( U1x4, ins.op, mx ); break;
					default: break;
				}
			} break;
		case gpeOPid_add:{
				if( !p_dst || !p_src )
					break;

				switch( ins.cID ) {
					case gpeCsz_Q: gpmADD( I8, ins.op ); break;
					case gpeCsz_q: gpmADD( U8, ins.op ); break;
					case gpeCsz_L:{
							switch(ins.op) {
								case gpeOPid_or:
									*((I4*)p_dst) |= *((I4*)p_src);
									break;
								case gpeOPid_sub:
									*((I4*)p_dst) -= *((I4*)p_src);
									break;
								default:
									*((I4*)p_dst) += *((I4*)p_src);
									break;
							}
						} break;
					case gpeCsz_l:{
							switch(ins.op) {
								case gpeOPid_or:
									*((U4*)p_dst) |= *((U4*)p_src);
									break;
								case gpeOPid_sub:
									*((U4*)p_dst) -= *((U4*)p_src);
									break;
								default:
									*((U4*)p_dst) += *((U4*)p_src);
									break;
							}
						} break;
					case gpeCsz_W: gpmADD( I2, ins.op ); break;
					case gpeCsz_w: gpmADD( U2, ins.op ); break;
					case gpeCsz_B: gpmADD( I1, ins.op ); break;
					case gpeCsz_b: gpmADD( U1, ins.op ); break;
					case gpeCsz_f: gpmADD( F, ins.op ); break;
					case gpeCsz_d: gpmADD( D, ins.op ); break;
					case gpeCsz_4: gpmADD( U1x4, ins.op ); break;
					default: break;
				}
			} break;
		case gpeOPid_sub:{
				if( !p_dst || !p_src )
					break;
				switch(ins.op) {
					default:
						pD[9] = (*p_dst == *p_src);
						break;
				}
			} break;
		default: break;
	}
	return &ins;
}

