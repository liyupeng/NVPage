/*
 * Copyright (c) 2012-2014 ARM Limited
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

/**
 * @file
 * DRAMCtrl declaration
 */

#ifndef __MEM_PRAM_CTRL_HH__
#define __MEM_PRAM_CTRL_HH__

#include "mem/dram_ctrl.hh"
#include "params/PRAMCtrl.hh"

/**
 * PRAMCtrl is a kind of DRAMCtrl which can maintain data during power off
 *
 * The DRAM controller is a basic single-channel memory controller
 * aiming to mimic a high-level DRAM controller and the most important
 * timing constraints associated with the DRAM. The focus is really on
 * modelling the impact on the system rather than the DRAM itself,
 * hence the focus is on the controller model and not on the
 * memory. By adhering to the correct timing constraints, ultimately
 * there is no need for a memory model in addition to the controller
 * model.
 *
 * As a basic design principle, this controller is not cycle callable,
 * but instead uses events to decide when new decisions can be made,
 * when resources become available, when things are to be considered
 * done, and when to send things back. Through these simple
 * principles, we achieve a performant model that is not
 * cycle-accurate, but enables us to evaluate the system impact of a
 * wide range of memory technologies, and also collect statistics
 * about the use of the memory.
 */
class PRAMCtrl : public DRAMCtrl
{
  private:

    const char *image_file_name;

  public:

    PRAMCtrl(const PRAMCtrlParams* p);

    void RecoverFromBackingStore();

    void PersistToBackingStore();

};

#endif //__MEM_PRAM_CTRL_HH__
