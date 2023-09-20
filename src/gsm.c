#include "gsm.h"
#include "fs.h"
#include <endian.h>
#include <string.h>

static uint8_t const comp128_table_0[512] = {
    0x66, 0xB1, 0xBA, 0xA2, 0x02, 0x9C, 0x70, 0x4B, 0x37, 0x19, 0x08, 0x0C,
    0xFB, 0xC1, 0xF6, 0xBC, 0x6D, 0xD5, 0x97, 0x35, 0x2A, 0x4F, 0xBF, 0x73,
    0xE9, 0xF2, 0xA4, 0xDF, 0xD1, 0x94, 0x6C, 0xA1, 0xFC, 0x25, 0xF4, 0x2F,
    0x40, 0xD3, 0x06, 0xED, 0xB9, 0xA0, 0x8B, 0x71, 0x4C, 0x8A, 0x3B, 0x46,
    0x43, 0x1A, 0x0D, 0x9D, 0x3F, 0xB3, 0xDD, 0x1E, 0xD6, 0x24, 0xA6, 0x45,
    0x98, 0x7C, 0xCF, 0x74, 0xF7, 0xC2, 0x29, 0x54, 0x47, 0x01, 0x31, 0x0E,
    0x5F, 0x23, 0xA9, 0x15, 0x60, 0x4E, 0xD7, 0xE1, 0xB6, 0xF3, 0x1C, 0x5C,
    0xC9, 0x76, 0x04, 0x4A, 0xF8, 0x80, 0x11, 0x0B, 0x92, 0x84, 0xF5, 0x30,
    0x95, 0x5A, 0x78, 0x27, 0x57, 0xE6, 0x6A, 0xE8, 0xAF, 0x13, 0x7E, 0xBE,
    0xCA, 0x8D, 0x89, 0xB0, 0xFA, 0x1B, 0x65, 0x28, 0xDB, 0xE3, 0x3A, 0x14,
    0x33, 0xB2, 0x62, 0xD8, 0x8C, 0x16, 0x20, 0x79, 0x3D, 0x67, 0xCB, 0x48,
    0x1D, 0x6E, 0x55, 0xD4, 0xB4, 0xCC, 0x96, 0xB7, 0x0F, 0x42, 0xAC, 0xC4,
    0x38, 0xC5, 0x9E, 0x00, 0x64, 0x2D, 0x99, 0x07, 0x90, 0xDE, 0xA3, 0xA7,
    0x3C, 0x87, 0xD2, 0xE7, 0xAE, 0xA5, 0x26, 0xF9, 0xE0, 0x22, 0xDC, 0xE5,
    0xD9, 0xD0, 0xF1, 0x44, 0xCE, 0xBD, 0x7D, 0xFF, 0xEF, 0x36, 0xA8, 0x59,
    0x7B, 0x7A, 0x49, 0x91, 0x75, 0xEA, 0x8F, 0x63, 0x81, 0xC8, 0xC0, 0x52,
    0x68, 0xAA, 0x88, 0xEB, 0x5D, 0x51, 0xCD, 0xAD, 0xEC, 0x5E, 0x69, 0x34,
    0x2E, 0xE4, 0xC6, 0x05, 0x39, 0xFE, 0x61, 0x9B, 0x8E, 0x85, 0xC7, 0xAB,
    0xBB, 0x32, 0x41, 0xB5, 0x7F, 0x6B, 0x93, 0xE2, 0xB8, 0xDA, 0x83, 0x21,
    0x4D, 0x56, 0x1F, 0x2C, 0x58, 0x3E, 0xEE, 0x12, 0x18, 0x2B, 0x9A, 0x17,
    0x50, 0x9F, 0x86, 0x6F, 0x09, 0x72, 0x03, 0x5B, 0x10, 0x82, 0x53, 0x0A,
    0xC3, 0xF0, 0xFD, 0x77, 0xB1, 0x66, 0xA2, 0xBA, 0x9C, 0x02, 0x4B, 0x70,
    0x19, 0x37, 0x0C, 0x08, 0xC1, 0xFB, 0xBC, 0xF6, 0xD5, 0x6D, 0x35, 0x97,
    0x4F, 0x2A, 0x73, 0xBF, 0xF2, 0xE9, 0xDF, 0xA4, 0x94, 0xD1, 0xA1, 0x6C,
    0x25, 0xFC, 0x2F, 0xF4, 0xD3, 0x40, 0xED, 0x06, 0xA0, 0xB9, 0x71, 0x8B,
    0x8A, 0x4C, 0x46, 0x3B, 0x1A, 0x43, 0x9D, 0x0D, 0xB3, 0x3F, 0x1E, 0xDD,
    0x24, 0xD6, 0x45, 0xA6, 0x7C, 0x98, 0x74, 0xCF, 0xC2, 0xF7, 0x54, 0x29,
    0x01, 0x47, 0x0E, 0x31, 0x23, 0x5F, 0x15, 0xA9, 0x4E, 0x60, 0xE1, 0xD7,
    0xF3, 0xB6, 0x5C, 0x1C, 0x76, 0xC9, 0x4A, 0x04, 0x80, 0xF8, 0x0B, 0x11,
    0x84, 0x92, 0x30, 0xF5, 0x5A, 0x95, 0x27, 0x78, 0xE6, 0x57, 0xE8, 0x6A,
    0x13, 0xAF, 0xBE, 0x7E, 0x8D, 0xCA, 0xB0, 0x89, 0x1B, 0xFA, 0x28, 0x65,
    0xE3, 0xDB, 0x14, 0x3A, 0xB2, 0x33, 0xD8, 0x62, 0x16, 0x8C, 0x79, 0x20,
    0x67, 0x3D, 0x48, 0xCB, 0x6E, 0x1D, 0xD4, 0x55, 0xCC, 0xB4, 0xB7, 0x96,
    0x42, 0x0F, 0xC4, 0xAC, 0xC5, 0x38, 0x00, 0x9E, 0x2D, 0x64, 0x07, 0x99,
    0xDE, 0x90, 0xA7, 0xA3, 0x87, 0x3C, 0xE7, 0xD2, 0xA5, 0xAE, 0xF9, 0x26,
    0x22, 0xE0, 0xE5, 0xDC, 0xD0, 0xD9, 0x44, 0xF1, 0xBD, 0xCE, 0xFF, 0x7D,
    0x36, 0xEF, 0x59, 0xA8, 0x7A, 0x7B, 0x91, 0x49, 0xEA, 0x75, 0x63, 0x8F,
    0xC8, 0x81, 0x52, 0xC0, 0xAA, 0x68, 0xEB, 0x88, 0x51, 0x5D, 0xAD, 0xCD,
    0x5E, 0xEC, 0x34, 0x69, 0xE4, 0x2E, 0x05, 0xC6, 0xFE, 0x39, 0x9B, 0x61,
    0x85, 0x8E, 0xAB, 0xC7, 0x32, 0xBB, 0xB5, 0x41, 0x6B, 0x7F, 0xE2, 0x93,
    0xDA, 0xB8, 0x21, 0x83, 0x56, 0x4D, 0x2C, 0x1F, 0x3E, 0x58, 0x12, 0xEE,
    0x2B, 0x18, 0x17, 0x9A, 0x9F, 0x50, 0x6F, 0x86, 0x72, 0x09, 0x5B, 0x03,
    0x82, 0x10, 0x0A, 0x53, 0xF0, 0xC3, 0x77, 0xFD,
};

static uint8_t const comp128_table_1[256] = {
    0x13, 0x0B, 0x50, 0x72, 0x2B, 0x01, 0x45, 0x5E, 0x27, 0x12, 0x7F, 0x75,
    0x61, 0x03, 0x55, 0x2B, 0x1B, 0x7C, 0x46, 0x53, 0x2F, 0x47, 0x3F, 0x0A,
    0x2F, 0x59, 0x4F, 0x04, 0x0E, 0x3B, 0x0B, 0x05, 0x23, 0x6B, 0x67, 0x44,
    0x15, 0x56, 0x24, 0x5B, 0x55, 0x7E, 0x20, 0x32, 0x6D, 0x5E, 0x78, 0x06,
    0x35, 0x4F, 0x1C, 0x2D, 0x63, 0x5F, 0x29, 0x22, 0x58, 0x44, 0x5D, 0x37,
    0x6E, 0x7D, 0x69, 0x14, 0x5A, 0x50, 0x4C, 0x60, 0x17, 0x3C, 0x59, 0x40,
    0x79, 0x38, 0x0E, 0x4A, 0x65, 0x08, 0x13, 0x4E, 0x4C, 0x42, 0x68, 0x2E,
    0x6F, 0x32, 0x20, 0x03, 0x27, 0x00, 0x3A, 0x19, 0x5C, 0x16, 0x12, 0x33,
    0x39, 0x41, 0x77, 0x74, 0x16, 0x6D, 0x07, 0x56, 0x3B, 0x5D, 0x3E, 0x6E,
    0x4E, 0x63, 0x4D, 0x43, 0x0C, 0x71, 0x57, 0x62, 0x66, 0x05, 0x58, 0x21,
    0x26, 0x38, 0x17, 0x08, 0x4B, 0x2D, 0x0D, 0x4B, 0x5F, 0x3F, 0x1C, 0x31,
    0x7B, 0x78, 0x14, 0x70, 0x2C, 0x1E, 0x0F, 0x62, 0x6A, 0x02, 0x67, 0x1D,
    0x52, 0x6B, 0x2A, 0x7C, 0x18, 0x1E, 0x29, 0x10, 0x6C, 0x64, 0x75, 0x28,
    0x49, 0x28, 0x07, 0x72, 0x52, 0x73, 0x24, 0x70, 0x0C, 0x66, 0x64, 0x54,
    0x5C, 0x30, 0x48, 0x61, 0x09, 0x36, 0x37, 0x4A, 0x71, 0x7B, 0x11, 0x1A,
    0x35, 0x3A, 0x04, 0x09, 0x45, 0x7A, 0x15, 0x76, 0x2A, 0x3C, 0x1B, 0x49,
    0x76, 0x7D, 0x22, 0x0F, 0x41, 0x73, 0x54, 0x40, 0x3E, 0x51, 0x46, 0x01,
    0x18, 0x6F, 0x79, 0x53, 0x68, 0x51, 0x31, 0x7F, 0x30, 0x69, 0x1F, 0x0A,
    0x06, 0x5B, 0x57, 0x25, 0x10, 0x36, 0x74, 0x7E, 0x1F, 0x26, 0x0D, 0x00,
    0x48, 0x6A, 0x4D, 0x3D, 0x1A, 0x43, 0x2E, 0x1D, 0x60, 0x25, 0x3D, 0x34,
    0x65, 0x11, 0x2C, 0x6C, 0x47, 0x34, 0x42, 0x39, 0x21, 0x33, 0x19, 0x5A,
    0x02, 0x77, 0x7A, 0x23,
};

static uint8_t const comp128_table_2[128] = {
    0x34, 0x32, 0x2C, 0x06, 0x15, 0x31, 0x29, 0x3B, 0x27, 0x33, 0x19, 0x20,
    0x33, 0x2F, 0x34, 0x2B, 0x25, 0x04, 0x28, 0x22, 0x3D, 0x0C, 0x1C, 0x04,
    0x3A, 0x17, 0x08, 0x0F, 0x0C, 0x16, 0x09, 0x12, 0x37, 0x0A, 0x21, 0x23,
    0x32, 0x01, 0x2B, 0x03, 0x39, 0x0D, 0x3E, 0x0E, 0x07, 0x2A, 0x2C, 0x3B,
    0x3E, 0x39, 0x1B, 0x06, 0x08, 0x1F, 0x1A, 0x36, 0x29, 0x16, 0x2D, 0x14,
    0x27, 0x03, 0x10, 0x38, 0x30, 0x02, 0x15, 0x1C, 0x24, 0x2A, 0x3C, 0x21,
    0x22, 0x12, 0x00, 0x0B, 0x18, 0x0A, 0x11, 0x3D, 0x1D, 0x0E, 0x2D, 0x1A,
    0x37, 0x2E, 0x0B, 0x11, 0x36, 0x2E, 0x09, 0x18, 0x1E, 0x3C, 0x20, 0x00,
    0x14, 0x26, 0x02, 0x1E, 0x3A, 0x23, 0x01, 0x10, 0x38, 0x28, 0x17, 0x30,
    0x0D, 0x13, 0x13, 0x1B, 0x1F, 0x35, 0x2F, 0x26, 0x3F, 0x0F, 0x31, 0x05,
    0x25, 0x35, 0x19, 0x24, 0x3F, 0x1D, 0x05, 0x07,
};

static uint8_t const comp128_table_3[64] = {
    0x01, 0x05, 0x1D, 0x06, 0x19, 0x01, 0x12, 0x17, 0x11, 0x13, 0x00,
    0x09, 0x18, 0x19, 0x06, 0x1F, 0x1C, 0x14, 0x18, 0x1E, 0x04, 0x1B,
    0x03, 0x0D, 0x0F, 0x10, 0x0E, 0x12, 0x04, 0x03, 0x08, 0x09, 0x14,
    0x00, 0x0C, 0x1A, 0x15, 0x08, 0x1C, 0x02, 0x1D, 0x02, 0x0F, 0x07,
    0x0B, 0x16, 0x0E, 0x0A, 0x11, 0x15, 0x0C, 0x1E, 0x1A, 0x1B, 0x10,
    0x1F, 0x0B, 0x07, 0x0D, 0x17, 0x0A, 0x05, 0x16, 0x13,
};

static uint8_t const comp128_table_4[32] = {
    0x0F, 0x0C, 0x0A, 0x04, 0x01, 0x0E, 0x0B, 0x07, 0x05, 0x00, 0x0E,
    0x07, 0x01, 0x02, 0x0D, 0x08, 0x0A, 0x03, 0x04, 0x09, 0x06, 0x00,
    0x03, 0x02, 0x05, 0x06, 0x08, 0x09, 0x0B, 0x0D, 0x0F, 0x0C,
};

/* These are the COMP128 compression tables. */
static uint8_t const *const comp128_table[5U] = {
    comp128_table_0, comp128_table_1, comp128_table_2,
    comp128_table_3, comp128_table_4,
};

int32_t gsm_select_res(swicc_fs_st const *const fs,
                       swicc_disk_tree_st *const tree,
                       swicc_fs_file_st *const file, uint8_t *const buf_res,
                       uint16_t *const buf_res_len)
{
    switch (file->hdr_item.type)
    {
    case SWICC_FS_ITEM_TYPE_FILE_MF:
    case SWICC_FS_ITEM_TYPE_FILE_ADF:
    /**
     * @brief Not sure if this operation can select applications.
     */
    case SWICC_FS_ITEM_TYPE_FILE_DF: {
        /* Response parameters data. */
        uint32_t const mem_free = UINT32_MAX - fs->va.cur_tree->len;
        /**
         * "Total amount of memory of the selected directory which is not
         * allocated to any of the DFs or EFs under the selected directory."
         * Safe cast due to check in same statement.
         */
        uint16_t const mem_free_short =
            htobe16(mem_free > UINT16_MAX ? UINT16_MAX : (uint16_t)mem_free);
        /* "File ID." */
        uint16_t const file_id = htobe16(file->hdr_file.id);
        /* "Type of file." */
        uint8_t file_type;
        switch (file->hdr_item.type)
        {
        case SWICC_FS_ITEM_TYPE_FILE_MF:
            file_type = 0x01;
            break;
        case SWICC_FS_ITEM_TYPE_FILE_ADF:
        case SWICC_FS_ITEM_TYPE_FILE_DF:
            file_type = 0x02;
            break;
        default:
            __builtin_unreachable();
        }
        /* Length of the GSM specific data. */
        uint8_t const gsm_data_len =
            10U; /* Everything except the last portion which is optional. */

        /* GSM specific data. */
        /* "File characteristics." (TS 51.011 Rel4 9.2.1 Detail 1) */
        /**
         * LSB>MSB
         *    1b Clock stop             = 0 (not allowed)
         *  + 1b Auth. alg. clock freq. = 1 (13/4 MHz)
         *  + 1b Clock stop             = 0 (high not preferred)
         *  + 1b Clock stop             = 0 (low not preferred)
         *  + 3b Supply voltage ind.    = 011 (1.8V Technology SIM; supports 3V/5V)
         *  + 1b CHV1                   = 1 (disabled)
         */
        uint8_t const file_characteristic = 0b10110010;
        uint32_t df_child_count_tmp;
        uint32_t ef_child_count_tmp;
        int32_t const ret_count = sim_fs_file_child_count(
            tree, file, false, &df_child_count_tmp, &ef_child_count_tmp);
        if (ret_count != 0 || df_child_count_tmp > UINT8_MAX ||
            ef_child_count_tmp > UINT8_MAX)
        {
            /**
             * @todo NV modified but can't indicate this, not sure what
             * to do here.
             */
            return -1;
        }
        /**
         * "Number of DFs which are a direct child of the current
         * directory."
         * Safe cast due to check in 'if' before.
         */
        uint8_t const df_child_count = (uint8_t)df_child_count_tmp;
        /**
         * "Number of EFs which are a direct child of the current
         * directory."
         * Safe cast due to check in 'if' before.
         */
        uint8_t const ef_child_count = (uint8_t)ef_child_count_tmp;
        /**
         * "Number of CHVs, UNBLOCK CHVs and administrative codes."
         */
        /**
         * Status of a secret code:
         * LSB>MSB
         *   4b Number of false presentations remaining = 0
         * + 3b RFU = 0
         * + 1b Secret code initialized = 1 (initialized)
         */
        uint8_t const code_count = 4U; /* 4 CHVs (PIN1 PIN2 PUK ADM). */
        /* "CHV1 status." */
        uint8_t const chv1_status = 0b10000011;
        /* "UNBLOCK CHV1 status." */
        uint8_t const chv1_unblock_status = 0b10001010;
        /* "CHV2." */
        uint8_t const chv2_status = 0b10000011;
        /* "UNBLOCK CHV2." */
        uint8_t const chv2_unblock_status = 0b10001010;

        /* Create the response in the response buffer. */
        {
            /* Check if destination buffer can fit the response. */
            if (*buf_res_len < 23U)
            {
                return -1;
            }
            /**
             * Using response buffer as temporary storage for complete
             * response.
             */
            memset(&buf_res[0U], 0U, 2U);
            memcpy(&buf_res[2U], &mem_free_short, 2U);
            memcpy(&buf_res[4U], &file_id, 2U);
            memcpy(&buf_res[6U], &file_type, 1U);
            memset(&buf_res[7U], 0U, 5U);
            memcpy(&buf_res[12U], &gsm_data_len, 1U);
            memcpy(&buf_res[13U], &file_characteristic, 1U);
            memcpy(&buf_res[14U], &df_child_count, 1U);
            memcpy(&buf_res[15U], &ef_child_count, 1U);
            memcpy(&buf_res[16U], &code_count, 1U);
            memset(&buf_res[17U], 0, 1U);
            memcpy(&buf_res[18U], &chv1_status, 1U);
            memcpy(&buf_res[19U], &chv1_unblock_status, 1U);
            memcpy(&buf_res[20U], &chv2_status, 1U);
            memcpy(&buf_res[21U], &chv2_unblock_status, 1U);
            memset(&buf_res[22U], 0U, 1U);
            *buf_res_len = 23U;
        }
        return 0;
    }
    case SWICC_FS_ITEM_TYPE_FILE_EF_TRANSPARENT:
    case SWICC_FS_ITEM_TYPE_FILE_EF_LINEARFIXED:
    case SWICC_FS_ITEM_TYPE_FILE_EF_CYCLIC: {
        /**
         * "File size (for transparent EF: the length of the body part of
         * the EF) (for linear fixed or cyclic EF: record length multiplied
         * by the number of records of the EF)."
         */
        uint16_t file_size_be;
        uint8_t rcrd_length;
        if (file->hdr_item.type == SWICC_FS_ITEM_TYPE_FILE_EF_TRANSPARENT)
        {
            /* Safe cast since same statement checks for overflow. */
            file_size_be = htobe16(file->data_size > UINT16_MAX
                                       ? UINT16_MAX
                                       : (uint16_t)file->data_size);
        }
        else
        {
            uint32_t rcrd_cnt;
            swicc_ret_et const ret_rcrd_cnt =
                swicc_disk_file_rcrd_cnt(tree, file, &rcrd_cnt);
            if (ret_rcrd_cnt != SWICC_RET_SUCCESS || rcrd_cnt > UINT16_MAX)
            {
                /**
                 * @todo NV modified but can't indicate this, not sure what
                 * to do here.
                 */
                return -1;
            }
            uint8_t const rcrd_size =
                file->hdr_item.type == SWICC_FS_ITEM_TYPE_FILE_EF_LINEARFIXED
                    ? file->hdr_spec.ef_linearfixed.rcrd_size
                    : file->hdr_spec.ef_cyclic.rcrd_size;
            /**
             * Safe cast since record count is <= UINT16_MAX and record size
             * is uint8 so can't overflow.
             */
            file_size_be = htobe16((uint16_t)(rcrd_cnt * rcrd_size));
            rcrd_length = rcrd_size;
        }
        /* "File ID." */
        uint16_t const file_id_be = htobe16(file->hdr_file.id);
        /* "Type of file." */
        uint8_t const file_type = 0x04;
        /**
         * Byte 7 (index 7, standard counts from 1 so there it's byte 8) is
         * RFU for linear-fixed and transparent EFs but for cyclic all bits
         * except b6 (index 6, in standard they index bits from 1 so it's b7
         * there) are RFU. b6=1 means INCREASE is allowed on this file.
         */
        uint8_t const b7 =
            file->hdr_item.type == SWICC_FS_ITEM_TYPE_FILE_EF_CYCLIC ? 0x01
                                                                     : 0x00;
        /* "Access conditions." */
        uint8_t const access_cond[3U] = {0x00, 0x00, 0x00};
        /* "File status." */
        /**
         * All bits except b0 are RFU and shall be 0. b0=0 if invalidated,
         * b0=0, if not invalidated b0=1.
         */
        uint8_t const file_status =
            file->hdr_item.lcs == SWICC_FS_LCS_OPER_ACTIV ? 0x01 : 0x00;
        /* Length of the remainder of the response. */
        uint8_t const data_extra_len = 2U; /* Everything. */
        /* "Structure of EF." */
        uint8_t ef_structure;
        switch (file->hdr_item.type)
        {
        case SWICC_FS_ITEM_TYPE_FILE_EF_TRANSPARENT:
            ef_structure = 0x00;
            break;
        case SWICC_FS_ITEM_TYPE_FILE_EF_LINEARFIXED:
            ef_structure = 0x01;
            break;
        case SWICC_FS_ITEM_TYPE_FILE_EF_CYCLIC:
            ef_structure = 0x03;
            break;
        default:
            __builtin_unreachable();
        }
        rcrd_length = 0;

        /* Create the response in the response buffer. */
        {
            /* Check if destination buffer can fit the response. */
            if ((file->hdr_item.type ==
                     SWICC_FS_ITEM_TYPE_FILE_EF_TRANSPARENT &&
                 *buf_res_len < 15U + file->data_size) ||
                *buf_res_len < 15)
            {
                return -1;
            }

            /**
             * Using response buffer as temporary storage for complete
             * response.
             */
            memset(&buf_res[0U], 0U, 2U);
            memcpy(&buf_res[2U], &file_size_be, 2U);
            memcpy(&buf_res[4U], &file_id_be, 2U);
            memcpy(&buf_res[6U], &file_type, 1U);
            memcpy(&buf_res[7U], &b7, 1U);
            memcpy(&buf_res[8U], access_cond, 3U);
            memcpy(&buf_res[11U], &file_status, 1U);
            memcpy(&buf_res[12U], &data_extra_len, 1U);
            memcpy(&buf_res[13U], &ef_structure, 1U);
            memcpy(&buf_res[14U], &rcrd_length, 1U);
            *buf_res_len = 15U;
        }
        return 0;
    }
    default:
        return -1;
        break;
    }
}

void gsm_algo(uint8_t const ki[const 16], uint8_t const rand[const 16],
              uint8_t res[const 12])
{
    uint8_t x[32U] = {0U};
    uint8_t bit[128U] = {0U};
    uint32_t m, n, y, z, bit_next;

    for (uint8_t i = 16U; i < 32U; i++)
    {
        x[i] = rand[i - 16U];
    }

    for (uint32_t i = 1U; i < 9U; i++)
    {
        /* Load key into first 16 bytes of input. */
        for (uint8_t j = 0; j < 16; j++)
        {
            x[j] = ki[j];
        }

        /* Perform substitutions. */
        for (uint8_t j = 0U; j < 5U; j++)
        {
            for (uint8_t k = 0U; k < (1U << j); k++)
            {
                for (uint8_t l = 0U; l < (1U << (4U - j)); l++)
                {
                    m = l + k * (1U << (5U - j));
                    n = m + (1U << (4U - j));
                    y = (x[m] + 2U * x[n]) % (1U << (9U - j));
                    z = (2U * x[m] + x[n]) % (1U << (9U - j));
                    x[m] = comp128_table[j][y];
                    x[n] = comp128_table[j][z];
                }
            }
        }
        /* Form bits from bytes. */
        for (uint8_t j = 0U; j < 32U; j++)
        {
            for (uint8_t k = 0U; k < 4U; k++)
            {
                bit[4 * j + k] = (uint8_t)(x[j] >> (3U - k)) & 1U;
            }
        }
        /* Permutation but not on the last loop. */
        if (i < 8)
        {
            for (uint8_t j = 0U; j < 16U; j++)
            {
                x[j + 16U] = 0U;
                for (uint8_t k = 0U; k < 8U; k++)
                {
                    bit_next = ((8U * j + k) * 17U) % 128U;
                    x[j + 16U] |= (uint8_t)(bit[bit_next] << (7 - k));
                }
            }
        }
    }

    for (uint8_t i = 0U; i < 4U; i++)
    {
        res[i] = (uint8_t)((x[2U * i] << 4U) | x[2U * i + 1U]);
    }
    for (uint8_t i = 0U; i < 6U; i++)
    {
        res[4 + i] =
            (uint8_t)((x[2U * i + 18U] << 6U) | (x[2U * i + 18U + 1U] << 2U) |
                      (x[2U * i + 18U + 2U] >> 2U));
    }
    res[4U + 6U] =
        (uint8_t)((x[2 * 6U + 18U] << 6U) | (x[2U * 6U + 18U + 1U] << 2U));
    res[4U + 7U] = 0U;
}
