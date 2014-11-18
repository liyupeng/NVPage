/*
 * Copyright (c) 2010-2014 ARM Limited
 * All rights reserved
 *
 * The license below extends only to copyright in the software and shall
 * not be construed as granting a license to any other intellectual
 * property including but not limited to intellectual property relating
 * to a hardware implementation of the functionality of the software
 * licensed hereunder.  You may use the software subject to the license
 * terms below provided that you ensure that this notice is replicated
 * unmodified and in its entirety in all distributions of the software,
 * modified or unmodified, in source code or in binary form.
 *
 * Copyright (c) 2013 Amin Farmahini-Farahani
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met: redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer;
 * redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution;
 * neither the name of the copyright holders nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Authors: Andreas Hansson
 *          Ani Udipi
 *          Neha Agarwal
 */

#include "mem/pram_ctrl.hh"
#include "debug/BusAddrRanges.hh"
#include <iostream>
#include <inttypes.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

using namespace std;

PRAMCtrl::PRAMCtrl(const PRAMCtrlParams* p) :
    DRAMCtrl(p), image_file_name((p->image_file).c_str())
{
    // cout << image_file_name << endl;
}

void
PRAMCtrl::RecoverFromBackingStore()
{
    // printf("RecoverFromBackingStore say hello from PRAMCtrl: start @ %ld, size is %ld\n", range.start(), range.size());
    
    if (range.interleaved())
        panic("Cannot recover from backing store for interleaved range %s\n", range.to_string());

    // perform the open
    int fd;
	fd = open(image_file_name, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
	if (fd < 0) {
	    perror("open");
        fatal("Could not open pram image file %s.\n", image_file_name);
	}
	
	// perform the stat
	struct stat st;
	stat(image_file_name, &st);
	uint64_t image_file_length = st.st_size;

    // perform the truncate
	if (image_file_length != range.size()) {
	    int result;
	    result = truncate(image_file_name, range.size());
	    if (result < 0) {
	        perror("truncate");
            fatal("Could not truncate pram image file %s.\n", image_file_name);
	    }
	    if (image_file_length == 0) {
	        cout << "Warning: new pram image file within zero bytes will be used." << endl;
	    } else if (image_file_length < range.size()) {
	        cout << "Warning: pram image file length is small than pram size. Zero byte will use to fill the rest." << endl;
	    } else {
	        cout << "Warning: pram image file length is large than pram size. The rest of image will be dropped." << endl;
	    }
	}

    // perform the actual mmap
    DPRINTF(BusAddrRanges, "Recovering from backing store for range %s with size %d\n", range.to_string(), range.size());
    int map_flags = MAP_SHARED;
    uint8_t* pmem = (uint8_t*) mmap(NULL, range.size(), PROT_READ | PROT_WRITE, map_flags, fd, 0);

    if (pmem == (uint8_t*) MAP_FAILED) {
        perror("mmap");
        fatal("Could not mmap %d bytes for range %s!\n", range.size(), range.to_string());
    }
    
    close(fd);

    // point the memory to their backing store
    DPRINTF(BusAddrRanges, "Mapping memory %s to backing store\n", name());
    setBackingStore(pmem);
}

void
PRAMCtrl::PersistToBackingStore()
{
    // printf("PersistToBackingStore say hello from PRAMCtrl: start @ %ld, size is %ld\n", range.start(), range.size());

    int result = munmap((void *)pmemAddr, range.size());
    if (result < 0) {
        perror("munmap");
        fatal("Could not munmap %d bytes for range %s!\n", range.size(), range.to_string());
	}
}

PRAMCtrl*
PRAMCtrlParams::create()
{
    return new PRAMCtrl(this);
}
