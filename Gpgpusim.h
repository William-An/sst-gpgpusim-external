// Copyright 2009-2019 NTESS. Under the terms
// of Contract DE-NA0003525 with NTESS, the U.S.
// Government retains certain rights in this software.
//
// Copyright (c) 2009-2019, NTESS
// All rights reserved.
//
// Portions are copyright of other developers:
// See the file CONTRIBUTORS.TXT in the top level directory
// the distribution for more information.
//
// This file is part of the SST software package. For license
// information, see the LICENSE file in the top level directory of the
// distribution.

// SST includes
#include <sst/core/sst_types.h>
#include <sst/core/link.h>
#include <sst/core/event.h>
#include <sst/core/output.h>
#include <sst/core/timeConverter.h>
#include <sst/core/interfaces/simpleMem.h>
#include <sst/core/component.h>
#include <sst/core/elementinfo.h>
#include <../ariel/ariel_shmem.h>

// Other Includes
#include "mempool.h"
#include "host_defines.h"
#include "builtin_types.h"
#include "driver_types.h"
#include "cuda_runtime_api.h"
#include "Gpgpusim_Event.h"


#include <cstring>
#include <string>
#include <fstream>
#include <sstream>
#include <map>

#include <stdio.h>
#include <stdint.h>
#include <poll.h>


using namespace std;
using namespace SST;
using namespace SST::Interfaces;
using namespace SST::GpgpusimComponent;

namespace SST {
    namespace GpgpusimComponent {
        class Gpgpusim : public SST::Component {
            public:
                Gpgpusim( SST::ComponentId_t id, SST::Params& params);
                void setup()  {};
                void finish() {};

                // NEW HANDLERS
                void cpuHandler( SST::Event* e );
                void memoryHandler(SimpleMem::Request* event);

                void gpuCacheHandler(SimpleMem::Request* event);

                void handleCPUWriteRequest(size_t txSize, uint64_t pAddr);
                void handleCPUReadRequest(size_t txSize, uint64_t pAddr);

                // TODO Need separate GPU memHierarchy first
                void handleReadRequest();
                void commitReadRequest();

                // TODO Need separate GPU memHierarchy first
                void handleWriteRequest();
                void commitWriteRequest();

                bool is_SST_buffer_full(unsigned core_id);
                void send_read_request_SST(unsigned core_id, uint64_t address, size_t size, void* mem_req);
                void send_write_request_SST(unsigned core_id, uint64_t address, size_t size, void* mem_req);
                void SST_callback_memcpy_H2D_done();
		void SST_callback_memcpy_D2H_done();

                bool tick(SST::Cycle_t x);
                cudaMemcpyKind memcpyKind;
                bool is_stalled = false;
                unsigned int transferNumber;
                std::vector<uint64_t> physicalAddresses;
                size_t totalTransfer;
                size_t ackTransfer;
                size_t remainingTransfer;
                uint64_t baseAddress;
                uint64_t currentAddress;
                std::vector<uint8_t> dataAddress;
                uint32_t pending_transactions_count = 0;
                uint32_t maxPendingTransCore;

                ~Gpgpusim() { };
                SST_ELI_REGISTER_COMPONENT(
                        Gpgpusim,
                        "Gpgpusim",
                        "Gpgpusim",
                        SST_ELI_ELEMENT_VERSION(3,2,0),
                        "GPGPU simulator",
                        COMPONENT_CATEGORY_PROCESSOR
                        )

                    SST_ELI_DOCUMENT_PARAMS(
                            {"verbose", "Verbosity for debugging. Increased numbers for increased verbosity.", "0"},
                            {"clock", "Internal Controller Clock Rate.", "1.0 Ghz"},
                            {"latency", "The time to be spent to service a memory request", "1000"},
                            {"num_nodes", "number of disaggregated nodes in the system", "1"},
                            {"num_cores", "Number of GPUs", "1"},
                            {"maxtranscore", "Maximum number of pending transactions", "16"},
		            {"maxcachetrans", "Maximum number of pending cache transactions", "512"},
                            )

                    // Optional since there is nothing to document
                    SST_ELI_DOCUMENT_STATISTICS(
                            )

                    SST_ELI_DOCUMENT_PORTS(
                            {"requestLink%(num_cores)d", "Handle CUDA API calls", { "GpgpusimComponent.GpgpusimEvent", "" } },
                            {"requestMemLink%(num_cores)d", "Link to CPU memH (cache)", {} },
			    {"requestGPUCacheLink%(num_cores)d", "Link to GPU memH (cache)", {} }
                            )

                    // Optional since there is nothing to document
                    SST_ELI_DOCUMENT_SUBCOMPONENT_SLOTS(
                            )

            private:
					struct cache_req_params {
						cache_req_params( unsigned m_core_id,  void* mem_fetch, SimpleMem::Request* req)
						{
							core_id = m_core_id;
							mem_fetch_pointer = mem_fetch;
							original_sst_req = req;
						}
						void* mem_fetch_pointer;
						unsigned core_id;
						SimpleMem::Request* original_sst_req;
					};
                    Gpgpusim();  // for serialization only
                    Gpgpusim(const Gpgpusim&); // do not implement
                    void operator=(const Gpgpusim&); // do not implement
                    int cpu_core_count;
                    int gpu_core_count;
                    uint32_t pending_transaction_count = 0;
                    std::unordered_map<SimpleMem::Request::id_t, SimpleMem::Request*>* pendingTransactions;
                    SimpleMem** gpu_to_cpu_cache_links;
                    Link** gpu_to_core_links;
                    unsigned int latency; // The page fault latency/ the time spent by Gpgpusim to service a memory allocation request

                    SimpleMem** gpu_to_cache_links;
                    uint32_t maxPendingCacheTrans;
                    std::unordered_map<SimpleMem::Request::id_t, struct cache_req_params>* gpuCachePendingTransactions;
                    uint32_t* numPendingCacheTransPerCore;

                    Output* output;
        };
    }
}
