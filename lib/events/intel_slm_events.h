/*
 * Copyright (c) 2013 Google, Inc
 * Contributed by Stephane Eranian <eranian@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
 * PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
 * CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
 * OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * This file is part of libpfm, a performance monitoring support library for
 * applications on Linux.
 *
 * PMU: slm (Intel Silvermont)
 */

static const intel_x86_umask_t slm_icache[]={
   { .uname  = "ACCESSES",
     .udesc  = "Instruction fetches, including uncacheacble fetches",
     .ucode = 0x300,
     .uflags= INTEL_X86_NCOMBO | INTEL_X86_DFL,
   },
   { .uname  = "MISSES",
     .udesc  = "Count all instructions fetches that miss tha icache or produce memory requests. This includes uncacheache fetches. Any instruction fetch miss is counted only once and not once for every cycle it is outstanding",
     .ucode = 0x200,
     .uflags= INTEL_X86_NCOMBO,
   },
   { .uname  = "HIT",
     .udesc  = "Count all instructions fetches from the instruction cache",
     .ucode = 0x100,
     .uflags= INTEL_X86_NCOMBO,
   },
};

static const intel_x86_umask_t slm_uops_retired[]={
   { .uname  = "ANY",
     .udesc  = "Micro-ops retired",
     .ucode = 0x1000,
     .uflags= INTEL_X86_NCOMBO | INTEL_X86_DFL,
   },
   { .uname  = "MS",
     .udesc  = "Micro-ops retired that were supplied fro MSROM",
     .ucode = 0x0100,
     .uflags= INTEL_X86_NCOMBO,
   },
   { .uname  = "X87",
     .udesc  = "Micro-ops retired that used X87 hardware",
     .ucode = 0x0200,
     .uflags= INTEL_X86_NCOMBO,
   },
   { .uname  = "MUL",
     .udesc  = "Micro-ops retired that used MUL hardware",
     .ucode = 0x0400,
     .uflags= INTEL_X86_NCOMBO,
   },
   { .uname  = "DIV",
     .udesc  = "Micro-ops retired that used DIV hardware",
     .ucode = 0x0800,
     .uflags= INTEL_X86_NCOMBO,
   },
   { .uname  = "STALLED_CYCLES",
     .udesc  = "Cycles no micro-ops retired",
     .ucode = 0x1000 | INTEL_X86_MOD_INV | (0x1 << INTEL_X86_CMASK_BIT),
     .uflags= INTEL_X86_NCOMBO,
   },
   { .uname  = "STALLS",
     .udesc  = "Periods no micro-ops retired",
     .ucode = 0x1000 | INTEL_X86_MOD_EDGE | INTEL_X86_MOD_INV | (0x1 << INTEL_X86_CMASK_BIT),
     .uflags= INTEL_X86_NCOMBO,
   },
};

static const intel_x86_umask_t slm_inst_retired[]={
   { .uname  = "ANY_P",
     .udesc  = "Instructions retired using generic counter (precise event)",
     .ucode = 0x0,
     .uflags= INTEL_X86_PEBS | INTEL_X86_DFL,
   },
   { .uname  = "ANY",
     .udesc  = "Instructions retired using generic counter (precise event)",
     .uequiv = "ANY_P",
     .ucode = 0x0,
     .uflags= INTEL_X86_PEBS,
   },
};

static const intel_x86_umask_t slm_l2_reject_xq[]={
   { .uname  = "ALL",
     .udesc  = "Number of demand and prefetch transactions that the L2 XQ rejects due to a full or near full condition which likely indictes back pressure from the IDI link. The XQ may reject transactions fro mthe L2Q (non-cacheable requests), BBS (L2 misses) and WOB (L2 write-back victims)",
     .ucode = 0x000,
     .uflags= INTEL_X86_DFL,
   },
};

static const intel_x86_umask_t slm_machine_clears[]={
   { .uname  = "SMC",
     .udesc  = "Self-Modifying Code detected",
     .ucode = 0x100,
     .uflags= INTEL_X86_DFL,
   },
   { .uname  = "MEMORY_ORDERING",
     .udesc  = "Number of stalled cycles due to memory ordering",
     .ucode = 0x200,
   },
   { .uname  = "FP_ASSIST",
     .udesc  = "Number of stalled cycle due to FPU assist",
     .ucode = 0x400,
   },
   { .uname  = "ALL",
     .udesc  = "Count any the machine clears",
     .ucode = 0x800,
   },
   { .uname  = "ANY",
     .udesc  = "Count any the machine clears",
     .uequiv = "ALL",
     .ucode = 0x800,
   },
};

static const intel_x86_umask_t slm_br_inst_retired[]={
   { .uname  = "ANY",
     .udesc  = "Any retired branch instruction (Precise Event)",
     .ucode = 0x0,
     .uflags= INTEL_X86_NCOMBO | INTEL_X86_DFL | INTEL_X86_PEBS,
   },
   { .uname  = "ALL_BRANCHES",
     .udesc  = "Any Retired branch instruction (Precise Event)",
     .uequiv = "ANY",
     .ucode = 0x0,
     .uflags= INTEL_X86_NCOMBO | INTEL_X86_PEBS,
   },
   { .uname  = "JCC",
     .udesc  = "JCC instructions retired (Precise Event)",
     .ucode = 0x7e00,
     .uflags= INTEL_X86_NCOMBO | INTEL_X86_PEBS,
   },
   { .uname  = "TAKEN_JCC",
     .udesc  = "Taken JCC instructions retired (Precise Event)",
     .ucode = 0xfe00,
     .uflags= INTEL_X86_NCOMBO | INTEL_X86_PEBS,
   },
   { .uname  = "CALL",
     .udesc  = "Near call instructions retired (Precise Event)",
     .ucode = 0xf900,
     .uflags= INTEL_X86_NCOMBO | INTEL_X86_PEBS,
   },
   { .uname  = "REL_CALL",
     .udesc  = "Near relative call instructions retired (Precise Event)",
     .ucode = 0xfd00,
     .uflags= INTEL_X86_NCOMBO | INTEL_X86_PEBS,
   },
   { .uname  = "IND_CALL",
     .udesc  = "Near indirect call instructions retired (Precise Event)",
     .ucode = 0xfb00,
     .uflags= INTEL_X86_NCOMBO | INTEL_X86_PEBS,
   },
   { .uname  = "RETURN",
     .udesc  = "Near ret instructions retired (Precise Event)",
     .ucode = 0xc00,
     .uflags= INTEL_X86_NCOMBO | INTEL_X86_PEBS,
   },
   { .uname  = "NON_RETURN_IND",
     .udesc  = "Number of near indirect jmp and near indirect call instructions retired (Precise Event)",
     .ucode = 0xeb00,
     .uflags= INTEL_X86_NCOMBO | INTEL_X86_PEBS,
   },
   { .uname  = "FAR_BRANCH",
     .udesc  = "Far branch instructions retired (Precise Event)",
     .uequiv = "FAR",
     .ucode = 0xbf00,
     .uflags= INTEL_X86_NCOMBO | INTEL_X86_PEBS,
   },
   { .uname  = "FAR",
     .udesc  = "Far branch instructions retired (Precise Event)",
     .ucode = 0xbf00,
     .uflags= INTEL_X86_NCOMBO | INTEL_X86_PEBS,
   },
};

static const intel_x86_umask_t slm_baclears[]={
   { .uname  = "ANY",
     .udesc  = "BACLEARS asserted",
     .uequiv = "ALL",
     .ucode = 0x100,
     .uflags= INTEL_X86_NCOMBO,
   },
   { .uname  = "ALL",
     .udesc  = "BACLEARS asserted",
     .ucode = 0x100,
     .uflags= INTEL_X86_DFL | INTEL_X86_NCOMBO,
   },
   { .uname  = "INDIRECT",
     .udesc  = "Number of baclears for indirect branches",
     .ucode = 0x200,
     .uflags= INTEL_X86_NCOMBO,
   },
   { .uname  = "UNCOND",
     .udesc  = "Number of baclears for unconditional branches",
     .ucode = 0x400,
     .uflags= INTEL_X86_NCOMBO,
   },
   { .uname  = "RETURN",
     .udesc  = "Number of baclears for return branches",
     .ucode = 0x800,
     .uflags= INTEL_X86_NCOMBO,
   },
   { .uname  = "COND",
     .udesc  = "Number of baclears for conditional branches",
     .ucode = 0x1000,
     .uflags= INTEL_X86_NCOMBO,
   },
};

static const intel_x86_umask_t slm_cpu_clk_unhalted[]={
   { .uname  = "CORE_P",
     .udesc  = "Core cycles when core is not halted",
     .ucode = 0x0,
     .uflags= INTEL_X86_NCOMBO | INTEL_X86_DFL,
   },
   { .uname  = "BUS",
     .udesc  = "Bus cycles when core is not halted. This event can give a measurement of the elapsed time. This events has a constant ratio with CPU_CLK_UNHALTED:REF event, which is the maximum bus to processor frequency ratio",
     .uequiv = "REF_P",
     .ucode = 0x100,
     .uflags= INTEL_X86_NCOMBO,
   },
   { .uname  = "REF_P",
     .udesc  = "Number of reference cycles that the core is not in a halted state. The core enters the halted state when it is running the HLT instruction. In mobile systems, the core frequency may change from time to time. This event is not affected by core frequency changes but counts as if the core is running a the same maximum frequency all the time",
     .ucode = 0x200,
     .uflags= INTEL_X86_NCOMBO,
   },
};

static const intel_x86_umask_t slm_mem_uop_retired[]={
   { .uname  = "LD_DCU_MISS",
     .udesc  = "Number of load uops retired that miss in L1 data cache. Note that prefetch misses will not be counted",
     .ucode = 0x100,
   },
   { .uname  = "LD_L2_HIT",
     .udesc  = "Number of load uops retired that hit L2 (Precise Event)",
     .ucode = 0x200,
     .uflags= INTEL_X86_PEBS,
   },
   { .uname  = "LD_L2_MISS",
     .udesc  = "Number of load uops retired that missed L2 (Precise Event)",
     .ucode = 0x400,
     .uflags= INTEL_X86_PEBS,
   },
   { .uname  = "LD_DTLB_MISS",
     .udesc  = "Number of load uops retired that had a DTLB miss (Precise Event)",
     .ucode = 0x800,
     .uflags= INTEL_X86_PEBS,
   },
   { .uname  = "LD_UTLB_MISS",
     .udesc  = "Number of load uops retired that had a UTLB miss",
     .ucode = 0x1000,
   },
   { .uname  = "HITM",
     .udesc  = "Number of load uops retired that got data from the other core or from the other module and the line was modified (Precise Event)",
     .ucode = 0x2000,
     .uflags= INTEL_X86_PEBS,
   },
   { .uname  = "ANY_LD",
     .udesc  = "Number of load uops retired",
     .ucode = 0x4000,
   },
   { .uname  = "ANY_ST",
     .udesc  = "Number of store uops retired",
     .ucode = 0x8000,
   },
};

static const intel_x86_umask_t slm_page_walks[]={
   { .uname  = "D_SIDE_CYCLES",
     .udesc  = "Number of cycles when a D-side page walk is in progress. Page walk duration divided by number of page walks is the average duration of page-walks",
     .ucode = 0x100,
     .uflags= INTEL_X86_NCOMBO,
   },
   { .uname  = "D_SIDE_WALKS",
     .udesc  = "Number of D-side page walks",
     .ucode = 0x100 | INTEL_X86_MOD_EDGE | (1ULL << INTEL_X86_CMASK_BIT),
     .modhw = _INTEL_X86_ATTR_E |  _INTEL_X86_ATTR_C,
     .uflags= INTEL_X86_NCOMBO,
   },
   { .uname  = "I_SIDE_CYCLES",
     .udesc  = "Number of cycles when a I-side page walk is in progress. Page walk duration divided by number of page walks is the average duration of page-walks",
     .ucode = 0x200,
     .uflags= INTEL_X86_NCOMBO,
   },
   { .uname  = "I_SIDE_WALKS",
     .udesc  = "Number of I-side page walks",
     .ucode = 0x200 | INTEL_X86_MOD_EDGE | (1ULL << INTEL_X86_CMASK_BIT),
     .modhw = _INTEL_X86_ATTR_E |  _INTEL_X86_ATTR_C,
     .uflags= INTEL_X86_NCOMBO,
   },
};

static const intel_x86_umask_t slm_llc_rqsts[]={
   { .uname  = "MISS",
     .udesc  = "Number of L2 cache misses",
     .ucode = 0x4100,
     .uflags= INTEL_X86_NCOMBO,
   },
   { .uname  = "ANY",
     .udesc  = "Number of L2 cache references",
     .ucode = 0x4f00,
     .uflags= INTEL_X86_NCOMBO | INTEL_X86_DFL,
   },
};

static const intel_x86_umask_t slm_rehabq[]={
   { .uname  = "LD_BLOCK_ST_FORWARD",
     .udesc  = "Number of retired loads that were prohibited from receiving forwarded data from the store because of address mismatch (Precise Event)",
     .ucode = 0x0100,
     .uflags= INTEL_X86_NCOMBO | INTEL_X86_PEBS,
   },
   { .uname  = "LD_BLOCK_STD_NOTREADY",
     .udesc  = "Number of times forward was technically possible but did not occur because the store data was not available at the right time",
     .ucode = 0x0200,
     .uflags= INTEL_X86_NCOMBO,
   },
   { .uname  = "ST_SPLITS",
     .udesc  = "Number of retired stores that experienced cache line boundary splits",
     .ucode = 0x0400,
     .uflags= INTEL_X86_NCOMBO,
   },
   { .uname  = "LD_SPLITS",
     .udesc  = "Number of retired loads that experienced cache line boundary splits (Precise Event)",
     .ucode = 0x0800,
     .uflags= INTEL_X86_NCOMBO | INTEL_X86_PEBS,
   },
   { .uname  = "LOCK",
     .udesc  = "Number of retired memory operations with lock semantics. These are either implicit locked instructions such as XCHG or instructions with an explicit LOCK prefix",
     .ucode = 0x1000,
     .uflags= INTEL_X86_NCOMBO,
   },
   { .uname  = "STA_FULL",
     .udesc  = "Number of retired stores that are delayed becuase there is not a store address buffer available",
     .ucode = 0x2000,
     .uflags= INTEL_X86_NCOMBO,
   },
   { .uname  = "ANY_LD",
     .udesc  = "Number of load uops reissued from RehabQ",
     .ucode = 0x4000,
     .uflags= INTEL_X86_NCOMBO,
   },
   { .uname  = "ANY_ST",
     .udesc  = "Number of store uops reissued from RehabQ",
     .ucode = 0x8000,
     .uflags= INTEL_X86_NCOMBO,
   },
};

static const intel_x86_umask_t slm_offcore_response[]={
   { .uname  = "DMND_DATA_RD",
     .udesc  = "Request: number of demand and DCU prefetch data reads of full and partial cachelines as well as demand data page table entry cacheline reads. Does not count L2 data read prefetches or instruction fetches",
     .ucode = 1ULL << (0 + 8),
     .grpid = 0,
   },
   { .uname  = "DMND_RFO",
     .udesc  = "Request: number of demand and DCU prefetch reads for ownership (RFO) requests generated by a write to data cacheline. Does not count L2 RFO prefetches",
     .ucode = 1ULL << (1 + 8),
     .grpid = 0,
   },
   { .uname  = "DMND_IFETCH",
     .udesc  = "Request: number of demand and DCU prefetch instruction cacheline reads. Does not count L2 code read prefetches",
     .ucode = 1ULL << (2 + 8),
     .grpid = 0,
   },
   { .uname  = "WB",
     .udesc  = "Request: number of writebacks (modified to exclusive) transactions",
     .ucode = 1ULL << (3 + 8),
     .grpid = 0,
   },
   { .uname  = "PF_L2_DATA_RD",
     .udesc  = "Request: number of data cacheline reads generated by L2 prefetchers",
     .ucode = 1ULL << (4 + 8),
     .grpid = 0,
   },
   { .uname  = "PF_RFO",
     .udesc  = "Request: number of RFO requests generated by L2 prefetchers",
     .ucode = 1ULL << (5 + 8),
     .grpid = 0,
   },
   { .uname  = "PF_IFETCH",
     .udesc  = "Request: number of code reads generated by L2 prefetchers",
     .ucode = 1ULL << (6 + 8),
     .grpid = 0,
   },
   { .uname  = "PARTIAL_READ",
     .udesc  = "Request: number of demand reads of partial cachelines (including UC, WC)",
     .ucode = 1ULL << (7 + 8),
     .grpid = 0,
   },
   { .uname  = "PARTIAL_WRITE",
     .udesc  = "Request: number of demand RFO requests to write to partial cache lines (includes UC, WT, WP)",
     .ucode = 1ULL << (8 + 8),
     .grpid = 0,
   },
   { .uname  = "UC_IFETCH",
     .udesc  = "Request: number of UC instruction fetches",
     .ucode = 1ULL << (9 + 8),
     .grpid = 0,
   },
   { .uname  = "BUS_LOCKS",
     .udesc  = "Request: number bus lock and split lock requests",
     .ucode = 1ULL << (10 + 8),
     .grpid = 0,
   },
   { .uname  = "STRM_ST",
     .udesc  = "Request: number of streaming store requests",
     .ucode = 1ULL << (11 + 8),
     .grpid = 0,
   },
   { .uname  = "SW_PREFETCH",
     .udesc  = "Request: number of software prefetch requests",
     .ucode = 1ULL << (12 + 8),
     .grpid = 0,
   },
   { .uname  = "PF_L1_DATA_RD",
     .udesc  = "Request: number of data cacheline reads generated by L1 prefetchers",
     .ucode = 1ULL << (13 + 8),
     .grpid = 0,
   },
   { .uname  = "PARTIAL_STRM_ST",
     .udesc  = "Request: number of partial streaming store requests",
     .ucode = 1ULL << (14 + 8),
     .grpid = 0,
   },
   { .uname  = "OTHER",
     .udesc  = "Request: counts one any other request that crosses IDI, including I/O",
     .ucode = 1ULL << (15+8),
     .grpid = 0,
   },
   { .uname  = "ANY_IFETCH",
     .udesc  = "Request: combination of PF_IFETCH | DMND_IFETCH | UC_IFETCH",
     .uequiv = "PF_IFETCH:DMND_IFETCH:UC_IFETCH",
     .ucode =  (1ULL << 6 | 1ULL << 2 | 1ULL << 9)  << 8,
     .grpid = 0,
   },
   { .uname  = "ANY_REQUEST",
     .udesc  = "Request: combination of all request umasks",
     .uequiv = "DMND_DATA_RD:DMND_RFO:DMND_IFETCH:WB:PF_L2_DATA_RD:PF_RFO:PF_IFETCH:PARTIAL_READ:PARTIAL_WRITE:UC_IFETCH:BUS_LOCKS:STRM_ST:SW_PREFETCH:PF_L1_DATA_RD:PARTIAL_STRM_ST:OTHER",
     .ucode = 0xffff00,
     .uflags= INTEL_X86_NCOMBO | INTEL_X86_DFL,
     .grpid = 0,
   },
   { .uname  = "ANY_DATA",
     .udesc  = "Request: combination of DMND_DATA | PF_L1_DATA_RD | PF_L2_DATA_RD",
     .uequiv = "DMND_DATA_RD:PF_L1_DATA_RD:PF_L2_DATA_RD",
     .ucode =  (1ULL << 0 | 1ULL << 4 | 1ULL << 13) << 8,
     .grpid = 0,
   },
   { .uname  = "ANY_RFO",
     .udesc  = "Request: combination of DMND_RFO | PF_RFO",
     .uequiv = "DMND_RFO:PF_RFO",
     .ucode =  (1ULL << 1 | 1ULL << 5) << 8,
     .grpid = 0,
   },
   { .uname  = "ANY_RESPONSE",
     .udesc  = "Response: count any response type",
     .ucode = 1ULL << (16+8),
     .uflags= INTEL_X86_NCOMBO | INTEL_X86_DFL | INTEL_X86_EXCL_GRP_GT,
     .grpid = 1,
   },
   { .uname  = "L2_HIT",
     .udesc  = "Supplier: counts L2 hits in M/E/S states",
     .ucode = 1ULL << (18+8),
     .grpid = 1,
   },
   { .uname  = "SNP_NONE",
     .udesc  = "Snoop: counts number of times no snoop-related information is available",
     .ucode = 1ULL << (31+8),
     .grpid = 2,
   },
   { .uname  = "SNP_MISS",
     .udesc  = "Snoop: counts number of times a snoop was needed and it missed all snooped caches",
     .ucode = 1ULL << (33+8),
     .grpid = 2,
   },
   { .uname  = "SNP_HIT",
     .udesc  = "Snoop: counts number of times a snoop hits in the other module where no modified copies were found in the L1 cache of the other core",
     .ucode = 1ULL << (34+8),
     .grpid = 2,
   },
   { .uname  = "SNP_HITM",
     .udesc  = "Snoop: counts number of times a snoop hits in the other module where modified copies were found in the L1 cache of the other core",
     .ucode = 1ULL << (36+8),
     .grpid = 2,
   },
   { .uname  = "NON_DRAM",
     .udesc  = "Snoop:  counts number of times target was a non-DRAM system address. This includes MMIO transactions",
     .ucode = 1ULL << (37+8),
     .grpid = 2,
   },
   { .uname  = "SNP_ANY",
     .udesc  = "Snoop: any snoop reason",
     .ucode = 0x7dULL << (31+8),
     .uequiv = "SNP_NONE:SNP_MISS:SNP_HIT:SNP_HITM:NON_DRAM",
     .uflags= INTEL_X86_DFL,
     .grpid = 2,
   },
};

static const intel_x86_umask_t slm_br_misp_inst_retired[]={
   { .uname  = "ALL_BRANCHES",
     .udesc  = "All mispredicted branches (Precise Event)",
     .uequiv = "ANY",
     .ucode = 0x0000, /* architectural encoding */
     .uflags= INTEL_X86_NCOMBO | INTEL_X86_PEBS,
   },
   { .uname  = "ANY",
     .udesc  = "All mispredicted branches (Precise Event)",
     .ucode = 0x0000, /* architectural encoding */
     .uflags= INTEL_X86_NCOMBO | INTEL_X86_PEBS | INTEL_X86_DFL,
   },
   { .uname  = "JCC",
     .udesc  = "Number of mispredicted conditional branch instructions retired (Precise Event)",
     .ucode = 0x7e00,
     .uflags= INTEL_X86_NCOMBO | INTEL_X86_PEBS,
   },
   { .uname  = "NON_RETURN_IND",
     .udesc  = "Number of mispredicted non-return branch instructions retired (Precise Event)",
     .ucode = 0xeb00,
     .uflags= INTEL_X86_NCOMBO | INTEL_X86_PEBS,
   },
   { .uname  = "FAR",
     .udesc  = "Number of mispredicted far branch instructions retired (Precise Event)",
     .ucode  = 0xbf00,
     .uflags = INTEL_X86_NCOMBO | INTEL_X86_PEBS,
   },
   { .uname  = "RETURN",
     .udesc  = "Number of mispredicted return branch instructions retired (Precise Event)",
     .ucode = 0xf700,
     .uflags= INTEL_X86_NCOMBO | INTEL_X86_PEBS,
   },
   { .uname  = "CALL",
     .udesc  = "Number of mispredicted call branch instructions retired (Precise Event)",
     .ucode = 0xf900,
     .uflags= INTEL_X86_NCOMBO | INTEL_X86_PEBS,
   },
   { .uname  = "IND_CALL",
     .udesc  = "Number of mispredicted indirect call branch instructions retired (Precise Event)",
     .ucode = 0xfb00,
     .uflags= INTEL_X86_NCOMBO | INTEL_X86_PEBS,
   },
   { .uname  = "REL_CALL",
     .udesc  = "Number of mispredicted relative call branch instructions retired (Precise Event)",
     .ucode = 0xfd00,
     .uflags= INTEL_X86_NCOMBO | INTEL_X86_PEBS,
   },
   { .uname  = "TAKEN_JCC",
     .udesc  = "Number of mispredicted taken conditional branch instructions retired (Precise Event)",
     .ucode = 0xfe00,
     .uflags= INTEL_X86_NCOMBO | INTEL_X86_PEBS,
   },
};

static const intel_x86_umask_t slm_no_alloc_cycles[]={
   { .uname  = "ANY",
     .udesc  = "Number of cycles when the front-end does not provide any instructions to be allocated for any reason",
     .ucode = 0x3f00,
     .uflags= INTEL_X86_NCOMBO | INTEL_X86_DFL,
   },
   { .uname  = "NOT_DELIVERED",
     .udesc  = "Number of cycles when the front-end does not provide any instructions to be allocated but the back-end is not stalled",
     .ucode = 0x5000,
     .uflags= INTEL_X86_NCOMBO,
   },
};

static const intel_x86_umask_t slm_rs_full_stall[]={
   { .uname  = "MEC",
     .udesc  = "Number of cycles when the allocation pipeline is stalled due to the RS for the MEC cluster is full",
     .ucode = 0x0100,
   },
   { .uname  = "IEC_PORT0",
     .udesc  = "Number of cycles when the allocation pipeline is stalled due to the RS for port 0 integer cluster is full",
     .ucode = 0x0200,
   },
   { .uname  = "IEC_PORT1",
     .udesc  = "Number of cycles when the allocation pipeline is stalled due to the RS for port 1 integer cluster is full",
     .ucode = 0x0400,
   },
   { .uname  = "FPC_PORT0",
     .udesc  = "Number of cycles when the allocation pipeline is stalled due to the RS for port 0 floating-pointer cluster is full",
     .ucode = 0x0800,
   },
   { .uname  = "FPC_PORT1",
     .udesc  = "Number of cycles when the allocation pipeline is stalled due to the RS for port 1 floating-pointer cluster is full",
     .ucode = 0x1000,
   },
   { .uname  = "ANY",
     .udesc  = "Number of cycles when the allocation pipeline is stalled due any one of the RS being full",
     .ucode = 0x1f00,
     .uflags= INTEL_X86_NCOMBO | INTEL_X86_DFL,
   },
};

static const intel_x86_umask_t slm_cycles_div_busy[]={
   { .uname  = "ANY",
     .udesc  = "Number of cycles the divider is busy",
     .ucode = 0x0100,
     .uflags= INTEL_X86_NCOMBO | INTEL_X86_DFL,
   },
};

static const intel_x86_umask_t slm_ms_decoded[]={
   { .uname  = "ENTRY",
     .udesc  = "Number of times the MSROM starts a flow of uops",
     .ucode = 0x0100,
     .uflags= INTEL_X86_NCOMBO | INTEL_X86_DFL,
   },
};

static const intel_x86_umask_t slm_decode_restriction[]={
   { .uname  = "PREDECODE_WRONG",
     .udesc  = "Number of times the prediction (from the predecode cache) for intruction length is incorrect",
     .ucode = 0x0100,
     .uflags= INTEL_X86_NCOMBO | INTEL_X86_DFL,
   },
};

static const intel_x86_entry_t intel_slm_pe[]={
{ .name   = "UNHALTED_CORE_CYCLES",
  .desc   = "Unhalted core cycles",
  .modmsk = INTEL_V2_ATTRS,
  .cntmsk = 0x200000003ull,
  .code = 0x3c,
},
{ .name   = "UNHALTED_REFERENCE_CYCLES",
  .desc   = "Unhalted reference cycle",
  .modmsk = INTEL_FIXED3_ATTRS,
  .cntmsk = 0x400000000ull,
  .code = 0x0300, /* pseudo encoding */
  .flags = INTEL_X86_FIXED,
},
{ .name   = "INSTRUCTION_RETIRED",
  .desc   = "Instructions retired",
  .modmsk = INTEL_V2_ATTRS,
  .cntmsk = 0x100000003ull,
  .code = 0xc0,
},
{ .name   = "INSTRUCTIONS_RETIRED",
  .desc   = "This is an alias for INSTRUCTION_RETIRED",
  .modmsk = INTEL_V2_ATTRS,
  .equiv = "INSTRUCTION_RETIRED",
  .cntmsk = 0x10003,
  .code = 0xc0,
},
{ .name   = "LLC_REFERENCES",
  .desc   = "Last level of cache references",
  .modmsk = INTEL_V2_ATTRS,
  .cntmsk = 0x3,
  .code = 0x4f2e,
},
{ .name   = "LAST_LEVEL_CACHE_REFERENCES",
  .desc   = "This is an alias for LLC_REFERENCES",
  .modmsk = INTEL_V2_ATTRS,
  .equiv = "LLC_REFERENCES",
  .cntmsk = 0x3,
  .code = 0x4f2e,
},
{ .name   = "LLC_MISSES",
  .desc   = "Last level of cache misses",
  .modmsk = INTEL_V2_ATTRS,
  .cntmsk = 0x3,
  .code = 0x412e,
},
{ .name   = "LAST_LEVEL_CACHE_MISSES",
  .desc   = "This is an alias for LLC_MISSES",
  .modmsk = INTEL_V2_ATTRS,
  .equiv = "LLC_MISSES",
  .cntmsk = 0x3,
  .code = 0x412e,
},
{ .name   = "BRANCH_INSTRUCTIONS_RETIRED",
  .desc   = "Branch instructions retired",
  .modmsk = INTEL_V2_ATTRS,
  .equiv = "BR_INST_RETIRED:ANY",
  .cntmsk = 0x3,
  .code = 0xc4,
},
{ .name   = "MISPREDICTED_BRANCH_RETIRED",
  .desc   = "Mispredicted branch instruction retired",
  .equiv  = "BR_MISP_INST_RETIRED",
  .modmsk = INTEL_V2_ATTRS,
  .cntmsk = 0x3,
  .code = 0xc5,
  .flags= INTEL_X86_PEBS,
},
/* begin model specific events */
{ .name   = "DECODE_RESTRICTION",
  .desc   = "Instruction length prediction delay",
  .modmsk = INTEL_V2_ATTRS,
  .cntmsk = 0x3,
  .code = 0xe9,
  .ngrp = 1,
  .numasks = LIBPFM_ARRAY_SIZE(slm_decode_restriction),
  .umasks = slm_decode_restriction,
},
{ .name   = "L2_REJECT_XQ",
  .desc   = "Rejected L2 requests to XQ",
  .modmsk = INTEL_V2_ATTRS,
  .cntmsk = 0x3,
  .code = 0x30,
  .numasks = LIBPFM_ARRAY_SIZE(slm_l2_reject_xq),
  .ngrp = 1,
  .umasks = slm_l2_reject_xq,
},
{ .name   = "ICACHE",
  .desc   = "Instruction fetches",
  .modmsk = INTEL_V2_ATTRS,
  .cntmsk = 0x3,
  .code = 0x80,
  .numasks = LIBPFM_ARRAY_SIZE(slm_icache),
  .ngrp = 1,
  .umasks = slm_icache,
},
{ .name   = "UOPS_RETIRED",
  .desc   = "Micro-ops retired",
  .modmsk = INTEL_V2_ATTRS,
  .cntmsk = 0x3,
  .code = 0xc2,
  .numasks = LIBPFM_ARRAY_SIZE(slm_uops_retired),
  .ngrp = 1,
  .umasks = slm_uops_retired,
},
{ .name   = "INST_RETIRED",
  .desc   = "Instructions retired",
  .modmsk = INTEL_V2_ATTRS,
  .cntmsk = 0x3,
  .code = 0xc0,
  .flags= INTEL_X86_PEBS,
  .numasks = LIBPFM_ARRAY_SIZE(slm_inst_retired),
  .ngrp = 1,
  .umasks = slm_inst_retired,
},
{ .name   = "CYCLES_DIV_BUSY",
  .desc   = "Cycles the divider is busy",
  .modmsk = INTEL_V2_ATTRS,
  .cntmsk = 0x3,
  .code = 0xcd,
  .numasks = LIBPFM_ARRAY_SIZE(slm_cycles_div_busy),
  .ngrp = 1,
  .umasks = slm_cycles_div_busy,
},
{ .name   = "RS_FULL_STALL",
  .desc   = "RS full",
  .modmsk = INTEL_V2_ATTRS,
  .cntmsk = 0x3,
  .code = 0xcb,
  .numasks = LIBPFM_ARRAY_SIZE(slm_rs_full_stall),
  .ngrp = 1,
  .umasks = slm_rs_full_stall,
},
{ .name   = "LLC_RQSTS",
  .desc   = "L2 cache requests",
  .modmsk = INTEL_V2_ATTRS,
  .cntmsk = 0x3,
  .code = 0x2e,
  .numasks = LIBPFM_ARRAY_SIZE(slm_llc_rqsts),
  .ngrp = 1,
  .umasks = slm_llc_rqsts,
},
{ .name   = "MACHINE_CLEARS",
  .desc   = "Self-Modifying Code detected",
  .modmsk = INTEL_V2_ATTRS,
  .cntmsk = 0x3,
  .code = 0xc3,
  .numasks = LIBPFM_ARRAY_SIZE(slm_machine_clears),
  .ngrp = 1,
  .umasks = slm_machine_clears,
},
{ .name   = "BR_INST_RETIRED",
  .desc   = "Retired branch instructions",
  .modmsk = INTEL_V2_ATTRS,
  .cntmsk = 0x3,
  .code = 0xc4,
  .numasks = LIBPFM_ARRAY_SIZE(slm_br_inst_retired),
  .flags= INTEL_X86_PEBS,
  .ngrp = 1,
  .umasks = slm_br_inst_retired,
},
{ .name   = "BR_MISP_INST_RETIRED",
  .desc   = "Mispredicted retired branch instructions",
  .modmsk = INTEL_V2_ATTRS,
  .cntmsk = 0x3,
  .code = 0xc5,
  .flags= INTEL_X86_PEBS,
  .numasks = LIBPFM_ARRAY_SIZE(slm_br_misp_inst_retired),
  .ngrp = 1,
  .umasks = slm_br_misp_inst_retired,
},
{ .name   = "MS_DECODED",
  .desc   = "MS decoder",
  .modmsk = INTEL_V2_ATTRS,
  .cntmsk = 0x3,
  .code = 0xe7,
  .numasks = LIBPFM_ARRAY_SIZE(slm_ms_decoded),
  .ngrp = 1,
  .umasks = slm_ms_decoded,
},
{ .name   = "BACLEARS",
  .desc   = "Branch address calculator",
  .modmsk = INTEL_V2_ATTRS,
  .cntmsk = 0x3,
  .code = 0xe6,
  .numasks = LIBPFM_ARRAY_SIZE(slm_baclears),
  .ngrp = 1,
  .umasks = slm_baclears,
},
{ .name   = "NO_ALLOC_CYCLES",
  .desc   = "Front-end allocation",
  .modmsk = INTEL_V2_ATTRS,
  .cntmsk = 0x3,
  .code = 0xca,
  .numasks = LIBPFM_ARRAY_SIZE(slm_no_alloc_cycles),
  .ngrp = 1,
  .umasks = slm_no_alloc_cycles,
},
{ .name   = "CPU_CLK_UNHALTED",
  .desc   = "Core cycles when core is not halted",
  .modmsk = INTEL_V2_ATTRS,
  .cntmsk = 0x3,
  .code = 0x3c,
  .numasks = LIBPFM_ARRAY_SIZE(slm_cpu_clk_unhalted),
  .ngrp = 1,
  .umasks = slm_cpu_clk_unhalted,
},
{ .name   = "MEM_UOP_RETIRED",
  .desc   = "Retired loads micro-ops",
  .modmsk = INTEL_V2_ATTRS,
  .cntmsk = 0x3,
  .code = 0x4,
  .flags= INTEL_X86_PEBS,
  .numasks = LIBPFM_ARRAY_SIZE(slm_mem_uop_retired),
  .ngrp = 1,
  .umasks = slm_mem_uop_retired,
},
{ .name   = "PAGE_WALKS",
  .desc   = "Number of page-walks executed",
  .modmsk = INTEL_V2_ATTRS,
  .cntmsk = 0x3,
  .code = 0x5,
  .numasks = LIBPFM_ARRAY_SIZE(slm_page_walks),
  .ngrp = 1,
  .umasks = slm_page_walks,
},
{ .name   = "CORE_REJECT",
  .desc   = "Demand and L1 prefetcher requests rejected by L2",
  .modmsk = INTEL_V2_ATTRS,
  .cntmsk = 0x3,
  .code = 0x31,
},
{ .name   = "REHABQ",
  .desc   = "Memory reference queue",
  .modmsk = INTEL_V2_ATTRS,
  .cntmsk = 0x3,
  .code = 0x03,
  .flags= INTEL_X86_PEBS,
  .numasks = LIBPFM_ARRAY_SIZE(slm_rehabq),
  .ngrp = 1,
  .umasks = slm_rehabq,
},
{ .name   = "OFFCORE_RESPONSE_0",
  .desc   = "Offcore response event (must provide at least one request type and either any_response or any combination of supplier + snoop)",
  .modmsk = INTEL_V2_ATTRS,
  .cntmsk = 0xf,
  .code = 0x01b7,
  .flags= INTEL_X86_NHM_OFFCORE,
  .numasks = LIBPFM_ARRAY_SIZE(slm_offcore_response),
  .ngrp = 3,
  .umasks = slm_offcore_response,
},
{ .name   = "OFFCORE_RESPONSE_1",
  .desc   = "Offcore response event (must provide at least one request type and either any_response or any combination of supplier + snoop)",
  .modmsk = INTEL_V2_ATTRS,
  .cntmsk = 0xf,
  .code = 0x02b7,
  .flags= INTEL_X86_NHM_OFFCORE,
  .numasks =  LIBPFM_ARRAY_SIZE(slm_offcore_response),
  .ngrp = 3,
  .umasks = slm_offcore_response, /* identical to actual umasks list for this event */
},
};
