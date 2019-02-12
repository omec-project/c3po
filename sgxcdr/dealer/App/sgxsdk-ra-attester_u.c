//TODO: ADD LICENSE

#include <assert.h>
#include <stdlib.h>

#include <sgx_uae_service.h>

#include <ra.h>
#include <ias-ra.h>

/* Untrusted code to do remote attestation with the SGX SDK. */

void ocall_remote_attestation
(
    sgx_report_t* report,
    attestation_verification_report_t* attn_report
)
{
    // produce quote
    uint32_t quote_size;
    sgx_calc_quote_size(NULL, 0, &quote_size);
    
    sgx_quote_t* quote = (sgx_quote_t*) calloc(1, quote_size);
    static const sgx_spid_t spid = {{0xC2, 0xEB, 0x8C, 0x29,
				     0xAB, 0x26, 0x9C, 0x88,
				     0x7E, 0x59, 0x78, 0x93,
				     0xD0, 0x2C, 0xD8, 0xAB}};
    
    sgx_status_t status;
    status = sgx_get_quote(report,
                           SGX_UNLINKABLE_SIGNATURE,
                           &spid,
                           NULL,
                           NULL,
                           0,
                           NULL,
                           quote,
                           quote_size);
    assert(SGX_SUCCESS == status);

    // verify against IAS
    obtain_attestation_verification_report(quote, quote_size, attn_report);
}

void ocall_sgx_init_quote
(
    sgx_target_info_t* target_info
)
{
    sgx_epid_group_id_t gid;
    sgx_status_t status = sgx_init_quote(target_info, &gid);
    assert(status == SGX_SUCCESS);
}
