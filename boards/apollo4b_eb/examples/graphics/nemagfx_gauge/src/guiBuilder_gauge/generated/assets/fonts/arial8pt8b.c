//*****************************************************************************
//
//! @file arial8pt8b.c
//
//*****************************************************************************

//*****************************************************************************
//
// Copyright (c) 2021, Ambiq Micro, Inc.
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice,
// this list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright
// notice, this list of conditions and the following disclaimer in the
// documentation and/or other materials provided with the distribution.
//
// 3. Neither the name of the copyright holder nor the names of its
// contributors may be used to endorse or promote products derived from this
// software without specific prior written permission.
//
// Third party software included in this distribution is subject to the
// additional license terms as defined in the /docs/licenses directory.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//
// This is part of revision b0-release-20210111-833-gc25608de46 of the AmbiqSuite Development Package.
//
//*****************************************************************************

#ifndef ARIAL8PT8B_C
#define ARIAL8PT8B_C

#include "arial8pt8b.h"

#ifndef NEMA_GPU_MEM
#define NEMA_GPU_MEM
#endif // NEMA_GPU_MEM

/* This will be read by the GPU only */
static uint8_t NEMA_GPU_MEM arial8pt8bBitmaps[] =
{
    // 0x20 - 0x7f
    0x0C, 0xFF, 0x24, 0x0B, 0xFF, 0x23, 0x02, 0xFD, 0x17, 0x00, 0xEF, 0x06,
    0x00, 0xD4, 0x00, 0x00, 0xB1, 0x00, 0x00, 0x00, 0x00, 0x04, 0xFF, 0x18,
    0x80, 0x9C, 0xB8, 0x64, 0x77, 0x94, 0xB0, 0x5C, 0x4E, 0x6F, 0x89, 0x35,
    0x00, 0x00, 0xCF, 0x00, 0x67, 0x69, 0x00, 0x19, 0xB5, 0x00, 0x9A, 0x35,
    0xE4, 0xFF, 0xFF, 0xFF, 0xFF, 0xFC, 0x00, 0x87, 0x49, 0x0A, 0xC5, 0x00,
    0x00, 0xBA, 0x15, 0x3A, 0x95, 0x00, 0xE4, 0xFF, 0xFF, 0xFF, 0xFF, 0xFC,
    0x24, 0xAC, 0x00, 0xA4, 0x2C, 0x00, 0x59, 0x76, 0x00, 0xCD, 0x02, 0x00,
    0x02, 0x96, 0xF2, 0xF2, 0x86, 0x00, 0x50, 0xC5, 0x5F, 0x69, 0xD8, 0x40,
    0x52, 0xB4, 0x47, 0x50, 0x00, 0x00, 0x05, 0x9C, 0xEF, 0xB2, 0x36, 0x00,
    0x00, 0x00, 0x4F, 0xA2, 0xE4, 0x40, 0x00, 0x00, 0x44, 0x50, 0x71, 0x8F,
    0x7C, 0xAE, 0x52, 0x62, 0xBF, 0x68, 0x0B, 0xA3, 0xF4, 0xF2, 0x9A, 0x05,
    0x00, 0x00, 0x44, 0x50, 0x00, 0x00, 0x05, 0xBA, 0xF1, 0x69, 0x00, 0x00,
    0x9E, 0x2A, 0x00, 0x00, 0x49, 0xA4, 0x1C, 0xDB, 0x00, 0x2D, 0x9A, 0x00,
    0x00, 0x00, 0x48, 0xA3, 0x1B, 0xDA, 0x00, 0xAD, 0x1C, 0x00, 0x00, 0x00,
    0x06, 0xBC, 0xF1, 0x66, 0x3F, 0x89, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0xB7, 0x12, 0xAB, 0xF2, 0x81, 0x00, 0x00, 0x00, 0x00, 0x54,
    0x77, 0x32, 0xB9, 0x18, 0xD8, 0x0A, 0x00, 0x00, 0x04, 0xBD, 0x09, 0x30,
    0xB8, 0x17, 0xD9, 0x0A, 0x00, 0x00, 0x67, 0x64, 0x00, 0x00, 0xA8, 0xF3,
    0x7E, 0x00, 0x00, 0x12, 0xBE, 0xF4, 0x9D, 0x01, 0x00, 0x00, 0x00, 0x76,
    0xA6, 0x1B, 0xDC, 0x31, 0x00, 0x00, 0x00, 0x50, 0xBC, 0x1B, 0xD8, 0x11,
    0x00, 0x00, 0x00, 0x16, 0xE4, 0xEC, 0x2D, 0x00, 0x00, 0x00, 0x1B, 0xD9,
    0x5A, 0xCE, 0x6E, 0x59, 0x3D, 0x00, 0x79, 0x94, 0x00, 0x1F, 0xE6, 0xE8,
    0x36, 0x00, 0x57, 0xDA, 0x20, 0x1C, 0xB6, 0xF9, 0x41, 0x00, 0x01, 0x86,
    0xEA, 0xE4, 0x7F, 0x48, 0xBB, 0x01, 0x84, 0x94, 0x7D, 0x8D, 0x58, 0x68,
    0x00, 0x01, 0xAA, 0x0F, 0x00, 0x62, 0x72, 0x00, 0x00, 0xD5, 0x17, 0x00,
    0x26, 0xD5, 0x00, 0x00, 0x49, 0xB7, 0x00, 0x00, 0x48, 0xB1, 0x00, 0x00,
    0x24, 0xD1, 0x00, 0x00, 0x00, 0xD5, 0x16, 0x00, 0x00, 0x62, 0x76, 0x00,
    0x00, 0x02, 0xAE, 0x0F, 0x18, 0xA3, 0x00, 0x00, 0x00, 0x84, 0x51, 0x00,
    0x00, 0x28, 0xC8, 0x00, 0x00, 0x00, 0xE9, 0x16, 0x00, 0x00, 0xCB, 0x39,
    0x00, 0x00, 0xC7, 0x36, 0x00, 0x00, 0xE4, 0x14, 0x00, 0x27, 0xC3, 0x00,
    0x00, 0x8A, 0x4F, 0x00, 0x18, 0xA8, 0x00, 0x00, 0x00, 0x41, 0x78, 0x00,
    0x79, 0xA5, 0xB1, 0xA2, 0x0E, 0xB0, 0xD3, 0x1C, 0x0F, 0x9C, 0x72, 0x3D,
    0x00, 0x00, 0x40, 0xAC, 0x00, 0x00, 0x00, 0x00, 0x40, 0xAC, 0x00, 0x00,
    0x64, 0xFF, 0xFF, 0xFF, 0xFF, 0xD0, 0x00, 0x00, 0x40, 0xAC, 0x00, 0x00,
    0x00, 0x00, 0x40, 0xAC, 0x00, 0x00, 0x04, 0xFF, 0x14, 0x00, 0xA7, 0x09,
    0x04, 0x9A, 0x00, 0xA8, 0xFF, 0xFF, 0x50, 0xFF, 0x18, 0x00, 0x00, 0xC6,
    0x01, 0x00, 0x26, 0xA1, 0x00, 0x00, 0x6F, 0x58, 0x00, 0x00, 0xB5, 0x12,
    0x00, 0x09, 0xBD, 0x00, 0x00, 0x4A, 0x7D, 0x00, 0x00, 0x93, 0x34, 0x00,
    0x00, 0xC6, 0x01, 0x00, 0x00, 0x00, 0x63, 0xE8, 0xEC, 0x63, 0x00, 0x23,
    0xE7, 0x2B, 0x28, 0xE2, 0x23, 0x6B, 0x97, 0x00, 0x00, 0x89, 0x76, 0x85,
    0x79, 0x00, 0x00, 0x6D, 0x92, 0x87, 0x78, 0x00, 0x00, 0x6D, 0x90, 0x6B,
    0x94, 0x00, 0x00, 0x8A, 0x76, 0x24, 0xE6, 0x2D, 0x28, 0xE3, 0x2E, 0x00,
    0x62, 0xE5, 0xEC, 0x6F, 0x00, 0x00, 0x0E, 0xD5, 0x18, 0x35, 0xD2, 0xFA,
    0x18, 0x9A, 0x3E, 0xE0, 0x18, 0x00, 0x00, 0xE0, 0x18, 0x00, 0x00, 0xE0,
    0x18, 0x00, 0x00, 0xE0, 0x18, 0x00, 0x00, 0xE0, 0x18, 0x00, 0x00, 0xE0,
    0x18, 0x02, 0x8F, 0xEC, 0xEE, 0x92, 0x02, 0x55, 0xBF, 0x16, 0x1A, 0xCB,
    0x5B, 0x00, 0x00, 0x00, 0x00, 0x86, 0x76, 0x00, 0x00, 0x00, 0x15, 0xDE,
    0x29, 0x00, 0x00, 0x19, 0xCD, 0x5B, 0x00, 0x00, 0x29, 0xD3, 0x49, 0x00,
    0x00, 0x16, 0xD8, 0x2D, 0x00, 0x00, 0x00, 0x8E, 0xFF, 0xFF, 0xFF, 0xFF,
    0x88, 0x00, 0x80, 0xEE, 0xE9, 0x72, 0x00, 0x43, 0xCA, 0x1E, 0x22, 0xE8,
    0x25, 0x00, 0x00, 0x02, 0x34, 0xE0, 0x17, 0x00, 0x00, 0x8D, 0xF8, 0xA2,
    0x03, 0x00, 0x00, 0x00, 0x0A, 0xBD, 0x6D, 0x00, 0x00, 0x00, 0x00, 0x74,
    0x94, 0x5F, 0xBA, 0x1C, 0x21, 0xD3, 0x55, 0x02, 0x90, 0xEE, 0xE8, 0x7D,
    0x00, 0x00, 0x00, 0x00, 0xA1, 0x84, 0x00, 0x00, 0x00, 0x58, 0xEE, 0x84,
    0x00, 0x00, 0x1F, 0xC9, 0x7A, 0x84, 0x00, 0x03, 0xBE, 0x31, 0x70, 0x84,
    0x00, 0x80, 0x73, 0x00, 0x70, 0x84, 0x00, 0xDC, 0xFF, 0xFF, 0xFF, 0xFF,
    0x98, 0x00, 0x00, 0x00, 0x70, 0x84, 0x00, 0x00, 0x00, 0x00, 0x70, 0x84,
    0x00, 0x00, 0xB5, 0xFF, 0xFF, 0xFF, 0x50, 0x00, 0xE6, 0x15, 0x00, 0x00,
    0x00, 0x16, 0xEA, 0x00, 0x00, 0x00, 0x00, 0x48, 0xE5, 0xD1, 0xF0, 0x9C,
    0x06, 0x00, 0x00, 0x00, 0x1A, 0xBD, 0x75, 0x00, 0x00, 0x00, 0x00, 0x64,
    0xA0, 0x64, 0xC0, 0x1E, 0x1C, 0xC5, 0x62, 0x03, 0x95, 0xEE, 0xE7, 0x83,
    0x02, 0x00, 0x4D, 0xD9, 0xF2, 0x97, 0x01, 0x1F, 0xED, 0x42, 0x12, 0xC4,
    0x51, 0x6D, 0x95, 0x00, 0x00, 0x00, 0x00, 0x8F, 0x9A, 0xD3, 0xF1, 0x97,
    0x03, 0x93, 0xDB, 0x25, 0x16, 0xC0, 0x64, 0x7B, 0x97, 0x00, 0x00, 0x6B,
    0x92, 0x30, 0xE5, 0x32, 0x16, 0xC3, 0x5D, 0x00, 0x5E, 0xDF, 0xF3, 0x93,
    0x02, 0x7C, 0xFF, 0xFF, 0xFF, 0xFF, 0x9C, 0x00, 0x00, 0x00, 0x17, 0xD9,
    0x24, 0x00, 0x00, 0x00, 0xA9, 0x6A, 0x00, 0x00, 0x00, 0x31, 0xDB, 0x03,
    0x00, 0x00, 0x00, 0x9D, 0x6D, 0x00, 0x00, 0x00, 0x03, 0xEC, 0x17, 0x00,
    0x00, 0x00, 0x2E, 0xDC, 0x00, 0x00, 0x00, 0x00, 0x54, 0xB1, 0x00, 0x00,
    0x00, 0x00, 0x74, 0xEA, 0xED, 0x7F, 0x00, 0x23, 0xE6, 0x22, 0x1D, 0xDF,
    0x34, 0x1D, 0xE3, 0x20, 0x1A, 0xDB, 0x2D, 0x00, 0x8D, 0xFF, 0xFF, 0x9B,
    0x00, 0x4B, 0xCF, 0x1F, 0x1C, 0xC7, 0x58, 0x82, 0x7D, 0x00, 0x00, 0x68,
    0x99, 0x4F, 0xD6, 0x26, 0x19, 0xC3, 0x66, 0x00, 0x80, 0xE9, 0xEC, 0x90,
    0x02, 0x00, 0x82, 0xEB, 0xE6, 0x69, 0x00, 0x4F, 0xD8, 0x25, 0x1C, 0xCE,
    0x3A, 0x81, 0x80, 0x00, 0x00, 0x7B, 0x87, 0x55, 0xCE, 0x1E, 0x1A, 0xCB,
    0x9E, 0x01, 0x91, 0xF0, 0xD9, 0x8F, 0x98, 0x00, 0x00, 0x00, 0x00, 0x8B,
    0x78, 0x3F, 0xC7, 0x19, 0x39, 0xE9, 0x27, 0x00, 0x8D, 0xF2, 0xE0, 0x57,
    0x00, 0xFF, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF,
    0x18, 0x04, 0xFF, 0x14, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x04, 0xFF, 0x14, 0x00, 0xA7, 0x09, 0x04, 0x9A,
    0x00, 0x00, 0x00, 0x00, 0x1E, 0x7E, 0xAF, 0x07, 0x57, 0xB9, 0xD4, 0x7A,
    0x1E, 0x64, 0xF6, 0x68, 0x00, 0x00, 0x00, 0x07, 0x58, 0xBA, 0xD4, 0x79,
    0x1E, 0x00, 0x00, 0x00, 0x1E, 0x7F, 0xAF, 0x64, 0xFF, 0xFF, 0xFF, 0xFF,
    0xD0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x64, 0xFF, 0xFF, 0xFF, 0xFF,
    0xD0, 0x5C, 0xA8, 0x45, 0x02, 0x00, 0x00, 0x07, 0x53, 0xB0, 0xDB, 0x80,
    0x20, 0x00, 0x00, 0x00, 0x26, 0xCD, 0xD0, 0x07, 0x52, 0xAF, 0xDB, 0x81,
    0x20, 0x5C, 0xA9, 0x46, 0x02, 0x00, 0x00, 0x02, 0x90, 0xED, 0xEE, 0x9E,
    0x07, 0x5A, 0xC4, 0x22, 0x15, 0xB9, 0x71, 0x00, 0x00, 0x00, 0x00, 0xA2,
    0x68, 0x00, 0x00, 0x00, 0x84, 0xBF, 0x07, 0x00, 0x00, 0x36, 0xD5, 0x0B,
    0x00, 0x00, 0x00, 0x6F, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x88, 0x94, 0x00, 0x00, 0x00, 0x00, 0x0A, 0x76, 0xD0,
    0xF3, 0xEF, 0xBD, 0x4B, 0x00, 0x00, 0x00, 0x10, 0xCA, 0x9F, 0x36, 0x0B,
    0x10, 0x4E, 0xD0, 0x74, 0x00, 0x00, 0x9D, 0x6A, 0x16, 0xB7, 0xEF, 0x8D,
    0xDC, 0x24, 0xD1, 0x20, 0x16, 0xC5, 0x00, 0xBF, 0x84, 0x0E, 0x9A, 0xE3,
    0x00, 0x67, 0x6A, 0x51, 0x7B, 0x2E, 0xDE, 0x03, 0x00, 0x4B, 0xAE, 0x00,
    0x53, 0x77, 0x60, 0x6E, 0x54, 0xAE, 0x00, 0x00, 0x82, 0x79, 0x00, 0x9F,
    0x3F, 0x3F, 0x9D, 0x2E, 0xE3, 0x1C, 0x47, 0xF3, 0x5A, 0x6E, 0xB5, 0x00,
    0x03, 0xD6, 0x30, 0x88, 0xF2, 0xA6, 0x9D, 0xF5, 0xA1, 0x0C, 0x00, 0x00,
    0x3A, 0xE3, 0x7C, 0x2C, 0x0A, 0x06, 0x1D, 0x58, 0xC5, 0x55, 0x00, 0x00,
    0x1E, 0x92, 0xD8, 0xF7, 0xF9, 0xDE, 0x9C, 0x29, 0x00, 0x00, 0x00, 0x00,
    0x2C, 0xFF, 0x53, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x8E, 0xAD, 0xBA,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0xE7, 0x1D, 0xE6, 0x23, 0x00, 0x00,
    0x00, 0x00, 0x50, 0xC1, 0x00, 0x94, 0x89, 0x00, 0x00, 0x00, 0x00, 0xB1,
    0x6B, 0x00, 0x3F, 0xEA, 0x06, 0x00, 0x00, 0x16, 0xFB, 0xFF, 0xFF, 0xFF,
    0xFF, 0x58, 0x00, 0x00, 0x73, 0xBC, 0x00, 0x00, 0x00, 0x86, 0xC0, 0x00,
    0x00, 0xD3, 0x4E, 0x00, 0x00, 0x00, 0x17, 0xF7, 0x28, 0x30, 0xFF, 0xFF,
    0xFF, 0xEF, 0x95, 0x06, 0x30, 0xD8, 0x00, 0x01, 0x1D, 0xDA, 0x51, 0x30,
    0xD8, 0x00, 0x00, 0x1D, 0xB2, 0x17, 0x30, 0xFF, 0xFF, 0xFF, 0xFF, 0xD9,
    0x20, 0x30, 0xD8, 0x00, 0x00, 0x19, 0xC1, 0x9B, 0x30, 0xD8, 0x00, 0x00,
    0x00, 0x64, 0xB2, 0x30, 0xD8, 0x00, 0x00, 0x13, 0xB8, 0x76, 0x30, 0xFF,
    0xFF, 0xFF, 0xEB, 0x9E, 0x07, 0x00, 0x0C, 0x91, 0xE6, 0xF4, 0xBF, 0x2F,
    0x00, 0x00, 0xBB, 0xA4, 0x1E, 0x0A, 0x68, 0xE9, 0x0E, 0x38, 0xE7, 0x05,
    0x00, 0x00, 0x00, 0x62, 0x1D, 0x68, 0xB3, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x67, 0xAB, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x34, 0xE3, 0x04,
    0x00, 0x00, 0x00, 0x97, 0x47, 0x00, 0xBF, 0xA1, 0x1C, 0x0D, 0x6F, 0xE8,
    0x13, 0x00, 0x11, 0xA0, 0xEB, 0xF4, 0xBE, 0x2E, 0x00, 0x28, 0xFF, 0xFF,
    0xFF, 0xF2, 0xAC, 0x16, 0x00, 0x28, 0xE4, 0x00, 0x01, 0x15, 0x9F, 0xBE,
    0x00, 0x28, 0xE4, 0x00, 0x00, 0x00, 0x07, 0xED, 0x29, 0x28, 0xE4, 0x00,
    0x00, 0x00, 0x00, 0xC2, 0x52, 0x28, 0xE4, 0x00, 0x00, 0x00, 0x00, 0xC2,
    0x48, 0x28, 0xE4, 0x00, 0x00, 0x00, 0x0C, 0xEF, 0x1F, 0x28, 0xE4, 0x00,
    0x01, 0x18, 0xA4, 0xB4, 0x00, 0x28, 0xFF, 0xFF, 0xFE, 0xEB, 0xA2, 0x10,
    0x00, 0x20, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x90, 0x20, 0xE8, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x20, 0xE8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0x44, 0x20, 0xE8, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x20, 0xE8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0xE8, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x20, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xC0, 0x18, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0x38, 0x18, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18,
    0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0xFF, 0xFF, 0xFF, 0xFF, 0xA4,
    0x00, 0x18, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0xF0, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x18, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0xF0,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x80, 0xDD, 0xF9, 0xDE, 0x75,
    0x00, 0x00, 0xA2, 0xBE, 0x31, 0x0B, 0x39, 0xD7, 0x63, 0x24, 0xEE, 0x0F,
    0x00, 0x00, 0x00, 0x39, 0x5A, 0x5D, 0xB5, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x5E, 0xB4, 0x00, 0x00, 0x78, 0xFF, 0xFF, 0xE0, 0x29, 0xED, 0x0B,
    0x00, 0x00, 0x00, 0x28, 0xE0, 0x00, 0xA4, 0xBB, 0x2F, 0x08, 0x29, 0x8E,
    0xD9, 0x00, 0x04, 0x79, 0xD8, 0xF9, 0xE5, 0x98, 0x1D, 0x20, 0xEC, 0x00,
    0x00, 0x00, 0x00, 0xFC, 0x10, 0x20, 0xEC, 0x00, 0x00, 0x00, 0x00, 0xFC,
    0x10, 0x20, 0xEC, 0x00, 0x00, 0x00, 0x00, 0xFC, 0x10, 0x20, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0x10, 0x20, 0xEC, 0x00, 0x00, 0x00, 0x00, 0xFC,
    0x10, 0x20, 0xEC, 0x00, 0x00, 0x00, 0x00, 0xFC, 0x10, 0x20, 0xEC, 0x00,
    0x00, 0x00, 0x00, 0xFC, 0x10, 0x20, 0xEC, 0x00, 0x00, 0x00, 0x00, 0xFC,
    0x10, 0xF8, 0x10, 0xF8, 0x10, 0xF8, 0x10, 0xF8, 0x10, 0xF8, 0x10, 0xF8,
    0x10, 0xF8, 0x10, 0xF8, 0x10, 0x00, 0x00, 0x00, 0x64, 0xA4, 0x00, 0x00,
    0x00, 0x64, 0xA4, 0x00, 0x00, 0x00, 0x64, 0xA4, 0x00, 0x00, 0x00, 0x64,
    0xA4, 0x00, 0x00, 0x00, 0x64, 0xA4, 0x95, 0x4B, 0x00, 0x66, 0xA0, 0x7C,
    0x9D, 0x0F, 0xAA, 0x7E, 0x12, 0xBD, 0xF7, 0xBE, 0x14, 0x30, 0xD8, 0x00,
    0x00, 0x0A, 0xB9, 0xA4, 0x03, 0x30, 0xD8, 0x00, 0x0F, 0xC4, 0x9C, 0x02,
    0x00, 0x30, 0xD8, 0x15, 0xCE, 0x94, 0x01, 0x00, 0x00, 0x30, 0xE7, 0xD8,
    0xF8, 0x25, 0x00, 0x00, 0x00, 0x30, 0xFD, 0x70, 0x72, 0xCA, 0x05, 0x00,
    0x00, 0x30, 0xD8, 0x00, 0x02, 0xC1, 0x86, 0x00, 0x00, 0x30, 0xD8, 0x00,
    0x00, 0x22, 0xF1, 0x3F, 0x00, 0x30, 0xD8, 0x00, 0x00, 0x00, 0x67, 0xE2,
    0x11, 0x30, 0xD8, 0x00, 0x00, 0x00, 0x00, 0x30, 0xD8, 0x00, 0x00, 0x00,
    0x00, 0x30, 0xD8, 0x00, 0x00, 0x00, 0x00, 0x30, 0xD8, 0x00, 0x00, 0x00,
    0x00, 0x30, 0xD8, 0x00, 0x00, 0x00, 0x00, 0x30, 0xD8, 0x00, 0x00, 0x00,
    0x00, 0x30, 0xD8, 0x00, 0x00, 0x00, 0x00, 0x30, 0xFF, 0xFF, 0xFF, 0xFF,
    0xB8, 0x30, 0xFF, 0x8E, 0x00, 0x00, 0x00, 0x40, 0xFF, 0x54, 0x30, 0xDF,
    0xD9, 0x01, 0x00, 0x00, 0x98, 0xD5, 0x54, 0x30, 0xD4, 0xB0, 0x38, 0x00,
    0x04, 0xC0, 0xAC, 0x54, 0x30, 0xD4, 0x5D, 0x8E, 0x00, 0x47, 0x81, 0xAC,
    0x54, 0x30, 0xD4, 0x10, 0xDC, 0x01, 0x9E, 0x2D, 0xAC, 0x54, 0x30, 0xD4,
    0x00, 0xB7, 0x3E, 0xC8, 0x00, 0xAC, 0x54, 0x30, 0xD4, 0x00, 0x64, 0xD2,
    0x85, 0x00, 0xAC, 0x54, 0x30, 0xD4, 0x00, 0x14, 0xFC, 0x31, 0x00, 0xAC,
    0x54, 0x28, 0xFE, 0x39, 0x00, 0x00, 0x00, 0xF4, 0x0C, 0x28, 0xEC, 0xC8,
    0x04, 0x00, 0x00, 0xF4, 0x0C, 0x28, 0xD8, 0x77, 0x77, 0x00, 0x00, 0xF4,
    0x0C, 0x28, 0xD8, 0x04, 0xC8, 0x21, 0x00, 0xF4, 0x0C, 0x28, 0xD8, 0x00,
    0x39, 0xB5, 0x00, 0xF4, 0x0C, 0x28, 0xD8, 0x00, 0x00, 0x98, 0x57, 0xF4,
    0x0C, 0x28, 0xD8, 0x00, 0x00, 0x10, 0xCF, 0xF8, 0x0C, 0x28, 0xD8, 0x00,
    0x00, 0x00, 0x58, 0xFF, 0x0C, 0x00, 0x0A, 0x90, 0xE7, 0xF6, 0xC9, 0x4A,
    0x00, 0x00, 0x00, 0xB8, 0xB3, 0x28, 0x0A, 0x4E, 0xE9, 0x4B, 0x00, 0x38,
    0xE6, 0x0A, 0x00, 0x00, 0x00, 0x59, 0xCB, 0x00, 0x6C, 0xB1, 0x00, 0x00,
    0x00, 0x00, 0x11, 0xFD, 0x04, 0x6C, 0xAC, 0x00, 0x00, 0x00, 0x00, 0x18,
    0xFD, 0x04, 0x35, 0xE5, 0x0A, 0x00, 0x00, 0x00, 0x5C, 0xCD, 0x00, 0x00,
    0xAF, 0xB4, 0x28, 0x0D, 0x52, 0xEA, 0x4E, 0x00, 0x00, 0x07, 0x88, 0xE4,
    0xF7, 0xCA, 0x4D, 0x00, 0x00, 0x28, 0xFF, 0xFF, 0xFF, 0xF8, 0xB7, 0x17,
    0x28, 0xE4, 0x00, 0x00, 0x09, 0x9B, 0xA0, 0x28, 0xE4, 0x00, 0x00, 0x00,
    0x45, 0xCF, 0x28, 0xE4, 0x00, 0x00, 0x12, 0xA1, 0x9E, 0x28, 0xFF, 0xFF,
    0xFF, 0xF5, 0xB8, 0x19, 0x28, 0xE4, 0x00, 0x00, 0x00, 0x00, 0x00, 0x28,
    0xE4, 0x00, 0x00, 0x00, 0x00, 0x00, 0x28, 0xE4, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x0C, 0x92, 0xE7, 0xF5, 0xC5, 0x44, 0x00, 0x00, 0x02, 0xC0,
    0xA7, 0x23, 0x0A, 0x57, 0xEE, 0x40, 0x00, 0x44, 0xDA, 0x05, 0x00, 0x00,
    0x00, 0x69, 0xBE, 0x00, 0x7A, 0x9E, 0x00, 0x00, 0x00, 0x00, 0x21, 0xF3,
    0x00, 0x7B, 0x9E, 0x00, 0x00, 0x00, 0x00, 0x29, 0xF2, 0x00, 0x45, 0xDB,
    0x05, 0x00, 0x4B, 0x20, 0x6C, 0xB8, 0x00, 0x02, 0xC0, 0xA7, 0x23, 0x6D,
    0xE5, 0xEB, 0x32, 0x00, 0x00, 0x0C, 0x92, 0xE8, 0xF1, 0xA3, 0x90, 0xCB,
    0x0B, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x38, 0x00, 0x24, 0xFF,
    0xFF, 0xFF, 0xFC, 0xE4, 0x6A, 0x00, 0x24, 0xE8, 0x00, 0x00, 0x04, 0x3F,
    0xF8, 0x12, 0x24, 0xE8, 0x00, 0x00, 0x04, 0x49, 0xF8, 0x0F, 0x24, 0xFF,
    0xFF, 0xFF, 0xFF, 0xDD, 0x5A, 0x00, 0x24, 0xE8, 0x00, 0x0F, 0xB1, 0x83,
    0x00, 0x00, 0x24, 0xE8, 0x00, 0x00, 0x0E, 0xE5, 0x56, 0x00, 0x24, 0xE8,
    0x00, 0x00, 0x00, 0x5C, 0xE5, 0x0B, 0x24, 0xE8, 0x00, 0x00, 0x00, 0x01,
    0xC8, 0x83, 0x00, 0x51, 0xD6, 0xF8, 0xD8, 0x89, 0x04, 0x14, 0xED, 0x3B,
    0x06, 0x1D, 0xC0, 0x64, 0x22, 0xE7, 0x14, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x82, 0xEF, 0xB8, 0x77, 0x23, 0x00, 0x00, 0x00, 0x0F, 0x50, 0x93, 0xED,
    0x4E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x63, 0xB8, 0x5C, 0xCA, 0x2C, 0x08,
    0x10, 0x80, 0x8F, 0x02, 0x7A, 0xDB, 0xFA, 0xED, 0x9A, 0x0A, 0xBC, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0x80, 0x00, 0x00, 0x24, 0xE4, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x24, 0xE4, 0x00, 0x00, 0x00, 0x00, 0x00, 0x24, 0xE4, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x24, 0xE4, 0x00, 0x00, 0x00, 0x00, 0x00, 0x24,
    0xE4, 0x00, 0x00, 0x00, 0x00, 0x00, 0x24, 0xE4, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x24, 0xE4, 0x00, 0x00, 0x00, 0x24, 0xE8, 0x00, 0x00, 0x00, 0x00,
    0xFC, 0x10, 0x24, 0xE8, 0x00, 0x00, 0x00, 0x00, 0xFC, 0x10, 0x24, 0xE8,
    0x00, 0x00, 0x00, 0x00, 0xFC, 0x10, 0x24, 0xE8, 0x00, 0x00, 0x00, 0x00,
    0xFC, 0x10, 0x22, 0xE9, 0x00, 0x00, 0x00, 0x01, 0xFD, 0x0E, 0x11, 0xF8,
    0x04, 0x00, 0x00, 0x1A, 0xFA, 0x02, 0x00, 0xCD, 0x83, 0x15, 0x15, 0x9C,
    0xB9, 0x00, 0x00, 0x25, 0xBB, 0xF4, 0xF2, 0xB0, 0x1A, 0x00, 0xC4, 0x56,
    0x00, 0x00, 0x00, 0x09, 0xED, 0x14, 0x62, 0xAC, 0x00, 0x00, 0x00, 0x55,
    0xAB, 0x00, 0x0C, 0xEA, 0x0B, 0x00, 0x00, 0xAE, 0x49, 0x00, 0x00, 0x9F,
    0x57, 0x00, 0x0E, 0xDB, 0x03, 0x00, 0x00, 0x3E, 0xAC, 0x00, 0x60, 0x84,
    0x00, 0x00, 0x00, 0x00, 0xD1, 0x0B, 0xB7, 0x23, 0x00, 0x00, 0x00, 0x00,
    0x7B, 0x66, 0xBA, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1B, 0xE9, 0x5D, 0x00,
    0x00, 0x00, 0xBB, 0x50, 0x00, 0x00, 0x8C, 0xF4, 0x06, 0x00, 0x00, 0xE6,
    0x20, 0x79, 0x8B, 0x00, 0x00, 0xD0, 0xC4, 0x40, 0x00, 0x25, 0xDB, 0x00,
    0x36, 0xC5, 0x00, 0x19, 0xCD, 0x6C, 0x86, 0x00, 0x63, 0x95, 0x00, 0x03,
    0xE8, 0x07, 0x5F, 0x87, 0x25, 0xCC, 0x00, 0xA0, 0x50, 0x00, 0x00, 0xB1,
    0x3A, 0xA5, 0x40, 0x00, 0xDD, 0x15, 0xD9, 0x0E, 0x00, 0x00, 0x6F, 0x8F,
    0xE0, 0x06, 0x00, 0x98, 0x8D, 0xC4, 0x00, 0x00, 0x00, 0x2C, 0xFF, 0xB2,
    0x00, 0x00, 0x52, 0xFF, 0x7F, 0x00, 0x00, 0x00, 0x00, 0xE8, 0x6B, 0x00,
    0x00, 0x0F, 0xFB, 0x3A, 0x00, 0x00, 0x41, 0xE7, 0x14, 0x00, 0x00, 0x71,
    0xAF, 0x00, 0x00, 0x8D, 0xAA, 0x00, 0x30, 0xDD, 0x12, 0x00, 0x00, 0x07,
    0xD3, 0x68, 0xD6, 0x41, 0x00, 0x00, 0x00, 0x00, 0x2F, 0xFC, 0x8D, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x61, 0xED, 0xC9, 0x04, 0x00, 0x00, 0x00, 0x28,
    0xE7, 0x26, 0xC2, 0x82, 0x00, 0x00, 0x08, 0xD1, 0x5F, 0x00, 0x20, 0xED,
    0x39, 0x00, 0x96, 0xA8, 0x00, 0x00, 0x00, 0x5E, 0xDD, 0x0C, 0xA4, 0x9D,
    0x00, 0x00, 0x00, 0x49, 0xDC, 0x0B, 0x13, 0xE8, 0x41, 0x00, 0x0B, 0xDD,
    0x3C, 0x00, 0x00, 0x53, 0xDB, 0x0A, 0x90, 0x8B, 0x00, 0x00, 0x00, 0x00,
    0xAB, 0xC2, 0xD3, 0x07, 0x00, 0x00, 0x00, 0x00, 0x16, 0xF9, 0x3C, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0xF0, 0x1C, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0xF0, 0x1C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF0, 0x1C, 0x00,
    0x00, 0x00, 0x54, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x50, 0x00, 0x00, 0x00,
    0x00, 0x56, 0xD8, 0x0F, 0x00, 0x00, 0x00, 0x31, 0xE7, 0x28, 0x00, 0x00,
    0x00, 0x15, 0xDF, 0x4C, 0x00, 0x00, 0x00, 0x05, 0xC2, 0x7B, 0x00, 0x00,
    0x00, 0x00, 0x97, 0xAB, 0x00, 0x00, 0x00, 0x00, 0x66, 0xCF, 0x0B, 0x00,
    0x00, 0x00, 0x00, 0xC8, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x74, 0x40, 0xFF,
    0xE0, 0x40, 0xB8, 0x00, 0x40, 0xB8, 0x00, 0x40, 0xB8, 0x00, 0x40, 0xB8,
    0x00, 0x40, 0xB8, 0x00, 0x40, 0xB8, 0x00, 0x40, 0xB8, 0x00, 0x40, 0xB8,
    0x00, 0x40, 0xFF, 0xE0, 0xC6, 0x01, 0x00, 0x00, 0x93, 0x34, 0x00, 0x00,
    0x4A, 0x7D, 0x00, 0x00, 0x09, 0xBD, 0x00, 0x00, 0x00, 0xB5, 0x12, 0x00,
    0x00, 0x6F, 0x58, 0x00, 0x00, 0x26, 0xA1, 0x00, 0x00, 0x00, 0xC5, 0x01,
    0xCC, 0xFF, 0x58, 0x00, 0xA0, 0x58, 0x00, 0xA0, 0x58, 0x00, 0xA0, 0x58,
    0x00, 0xA0, 0x58, 0x00, 0xA0, 0x58, 0x00, 0xA0, 0x58, 0x00, 0xA0, 0x58,
    0x00, 0xA0, 0x58, 0xCC, 0xFF, 0x58, 0x00, 0x16, 0xF6, 0x34, 0x00, 0x00,
    0x86, 0x8F, 0xAD, 0x00, 0x0E, 0xD7, 0x06, 0xC0, 0x2A, 0x78, 0x7F, 0x00,
    0x51, 0xA2, 0x2C, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x3C, 0x37, 0xEA,
    0x05, 0x00, 0x97, 0x50, 0x01, 0x8A, 0xE2, 0xF6, 0xA9, 0x03, 0x48, 0xC6,
    0x12, 0x10, 0xC4, 0x44, 0x00, 0x56, 0x9E, 0xC9, 0xFB, 0x57, 0x5F, 0xD1,
    0x60, 0x34, 0xAE, 0x58, 0x8D, 0x99, 0x09, 0x45, 0xEF, 0x5E, 0x26, 0xD2,
    0xF2, 0xB0, 0x8B, 0x84, 0x48, 0xB0, 0x00, 0x00, 0x00, 0x00, 0x48, 0xB0,
    0x00, 0x00, 0x00, 0x00, 0x48, 0xBF, 0xBB, 0xED, 0x91, 0x03, 0x48, 0xF5,
    0x3A, 0x18, 0xCC, 0x5B, 0x48, 0xAC, 0x00, 0x00, 0x64, 0x9B, 0x48, 0xA9,
    0x00, 0x00, 0x66, 0x96, 0x48, 0xF0, 0x37, 0x1B, 0xD0, 0x52, 0x48, 0xB2,
    0xCC, 0xEE, 0x86, 0x00, 0x00, 0x7A, 0xEA, 0xEC, 0x6E, 0x00, 0x44, 0xDB,
    0x24, 0x25, 0xE5, 0x25, 0x84, 0x7C, 0x00, 0x00, 0x00, 0x00, 0x7F, 0x7B,
    0x00, 0x00, 0x00, 0x00, 0x40, 0xD7, 0x20, 0x1E, 0xCC, 0x37, 0x00, 0x78,
    0xE8, 0xEC, 0x77, 0x00, 0x00, 0x00, 0x00, 0x00, 0xA4, 0x54, 0x00, 0x00,
    0x00, 0x00, 0xA4, 0x54, 0x01, 0x91, 0xF3, 0xC7, 0xB7, 0x54, 0x54, 0xCF,
    0x1A, 0x33, 0xF1, 0x54, 0x94, 0x6F, 0x00, 0x00, 0xA3, 0x54, 0x93, 0x70,
    0x00, 0x00, 0x9E, 0x54, 0x4F, 0xD5, 0x1E, 0x2E, 0xED, 0x54, 0x00, 0x86,
    0xF2, 0xD2, 0xAF, 0x54, 0x00, 0x71, 0xE5, 0xEB, 0x8A, 0x01, 0x40, 0xBC,
    0x19, 0x11, 0xAD, 0x5E, 0x84, 0xFF, 0xFF, 0xFF, 0xFF, 0x9C, 0x87, 0x7F,
    0x00, 0x00, 0x00, 0x00, 0x45, 0xE4, 0x2D, 0x19, 0xB6, 0x6B, 0x00, 0x76,
    0xE4, 0xF1, 0x98, 0x03, 0x00, 0xAA, 0xF9, 0x57, 0x0A, 0xF6, 0x12, 0x01,
    0xE4, 0xFF, 0xFF, 0x08, 0x0C, 0xEC, 0x00, 0x00, 0x0C, 0xEC, 0x00, 0x00,
    0x0C, 0xEC, 0x00, 0x00, 0x0C, 0xEC, 0x00, 0x00, 0x0C, 0xEC, 0x00, 0x00,
    0x00, 0x8C, 0xF2, 0xCA, 0xA2, 0x60, 0x51, 0xD0, 0x1B, 0x31, 0xEC, 0x60,
    0x96, 0x68, 0x00, 0x00, 0x96, 0x60, 0x93, 0x68, 0x00, 0x00, 0x95, 0x60,
    0x4F, 0xCC, 0x1A, 0x2C, 0xE9, 0x5F, 0x01, 0x87, 0xEC, 0xC5, 0xB2, 0x56,
    0x00, 0x00, 0x00, 0x23, 0xE3, 0x2C, 0x33, 0xD5, 0xFA, 0xE8, 0x75, 0x00,
    0x48, 0xB0, 0x00, 0x00, 0x00, 0x00, 0x48, 0xB0, 0x00, 0x00, 0x00, 0x00,
    0x48, 0xC0, 0xB5, 0xF5, 0xAF, 0x06, 0x48, 0xF5, 0x44, 0x13, 0xD2, 0x4B,
    0x48, 0xB8, 0x00, 0x00, 0x9A, 0x5F, 0x48, 0xB0, 0x00, 0x00, 0x98, 0x60,
    0x48, 0xB0, 0x00, 0x00, 0x98, 0x60, 0x48, 0xB0, 0x00, 0x00, 0x98, 0x60,
    0x44, 0xB4, 0x00, 0x00, 0x44, 0xB4, 0x44, 0xB4, 0x44, 0xB4, 0x44, 0xB4,
    0x44, 0xB4, 0x44, 0xB4, 0x00, 0x48, 0xB0, 0x00, 0x00, 0x00, 0x00, 0x48,
    0xB0, 0x00, 0x48, 0xB0, 0x00, 0x48, 0xB0, 0x00, 0x48, 0xB0, 0x00, 0x48,
    0xB0, 0x00, 0x48, 0xB0, 0x04, 0x65, 0xA5, 0x64, 0xF0, 0x4F, 0x44, 0xB4,
    0x00, 0x00, 0x00, 0x00, 0x44, 0xB4, 0x00, 0x00, 0x00, 0x00, 0x44, 0xB4,
    0x00, 0x81, 0xB7, 0x07, 0x44, 0xB4, 0x7A, 0xBB, 0x08, 0x00, 0x44, 0xF1,
    0xFF, 0x47, 0x00, 0x00, 0x44, 0xD8, 0x54, 0xD7, 0x07, 0x00, 0x44, 0xB4,
    0x00, 0xAA, 0x81, 0x00, 0x44, 0xB4, 0x00, 0x19, 0xEA, 0x29, 0x4C, 0xAC,
    0x4C, 0xAC, 0x4C, 0xAC, 0x4C, 0xAC, 0x4C, 0xAC, 0x4C, 0xAC, 0x4C, 0xAC,
    0x4C, 0xAC, 0x48, 0xA7, 0xC2, 0xF5, 0x70, 0x7C, 0xF4, 0xCA, 0x12, 0x48,
    0xF4, 0x3B, 0x24, 0xF6, 0x7D, 0x0E, 0xB6, 0x63, 0x48, 0xBA, 0x00, 0x00,
    0xE5, 0x1B, 0x00, 0x81, 0x74, 0x48, 0xB0, 0x00, 0x00, 0xE4, 0x14, 0x00,
    0x80, 0x74, 0x48, 0xB0, 0x00, 0x00, 0xE4, 0x14, 0x00, 0x80, 0x74, 0x48,
    0xB0, 0x00, 0x00, 0xE4, 0x14, 0x00, 0x80, 0x74, 0x48, 0xBC, 0xC8, 0xF2,
    0xA9, 0x05, 0x48, 0xF4, 0x35, 0x17, 0xD6, 0x49, 0x48, 0xBB, 0x00, 0x00,
    0x9D, 0x5B, 0x48, 0xB0, 0x00, 0x00, 0x9C, 0x5C, 0x48, 0xB0, 0x00, 0x00,
    0x9C, 0x5C, 0x48, 0xB0, 0x00, 0x00, 0x9C, 0x5C, 0x00, 0x7D, 0xE9, 0xE9,
    0x86, 0x01, 0x4E, 0xD5, 0x23, 0x1C, 0xCA, 0x5F, 0x90, 0x6D, 0x00, 0x00,
    0x59, 0xA2, 0x92, 0x6D, 0x00, 0x00, 0x5B, 0xA7, 0x50, 0xD5, 0x22, 0x1B,
    0xCA, 0x68, 0x00, 0x80, 0xE8, 0xEF, 0x8E, 0x03, 0x48, 0xAE, 0xC6, 0xF4,
    0x99, 0x03, 0x48, 0xF5, 0x3E, 0x18, 0xCA, 0x60, 0x48, 0xAB, 0x00, 0x00,
    0x60, 0xA0, 0x48, 0xA6, 0x00, 0x00, 0x62, 0x9D, 0x48, 0xF0, 0x31, 0x19,
    0xCC, 0x57, 0x48, 0xC4, 0xC4, 0xF1, 0x88, 0x01, 0x48, 0xB0, 0x00, 0x00,
    0x00, 0x00, 0x48, 0xB0, 0x00, 0x00, 0x00, 0x00, 0x01, 0x8E, 0xF3, 0xCD,
    0xA5, 0x54, 0x52, 0xD1, 0x1B, 0x37, 0xF1, 0x54, 0x91, 0x70, 0x00, 0x00,
    0xA1, 0x54, 0x88, 0x70, 0x00, 0x00, 0x9A, 0x54, 0x41, 0xD5, 0x1F, 0x2C,
    0xEB, 0x54, 0x00, 0x76, 0xEA, 0xCF, 0xBC, 0x54, 0x00, 0x00, 0x00, 0x00,
    0xA4, 0x54, 0x00, 0x00, 0x00, 0x00, 0xA4, 0x54, 0x48, 0xB7, 0xE0, 0x9B,
    0x48, 0xEF, 0x22, 0x10, 0x48, 0xBD, 0x00, 0x00, 0x48, 0xB0, 0x00, 0x00,
    0x48, 0xB0, 0x00, 0x00, 0x48, 0xB0, 0x00, 0x00, 0x11, 0xBC, 0xF4, 0xCB,
    0x33, 0x00, 0x6E, 0x9C, 0x07, 0x63, 0xBC, 0x00, 0x37, 0xF3, 0xC3, 0x7B,
    0x16, 0x00, 0x00, 0x1E, 0x70, 0xBA, 0xDC, 0x00, 0x80, 0x95, 0x09, 0x27,
    0xF2, 0x07, 0x0F, 0xB5, 0xF5, 0xDC, 0x55, 0x00, 0x0A, 0xA0, 0x00, 0x18,
    0xDC, 0x00, 0xD0, 0xFF, 0xD8, 0x18, 0xDC, 0x00, 0x18, 0xDC, 0x00, 0x18,
    0xDC, 0x00, 0x15, 0xE5, 0x09, 0x02, 0xC7, 0xE0, 0x4C, 0xAC, 0x00, 0x00,
    0xA4, 0x54, 0x4C, 0xAC, 0x00, 0x00, 0xA4, 0x54, 0x4C, 0xAC, 0x00, 0x00,
    0xA4, 0x54, 0x4B, 0xAE, 0x00, 0x00, 0xAD, 0x54, 0x37, 0xE4, 0x1D, 0x39,
    0xEE, 0x54, 0x01, 0xA1, 0xF3, 0xC4, 0x9E, 0x54, 0xAE, 0x53, 0x00, 0x00,
    0xCD, 0x31, 0x51, 0xAB, 0x00, 0x25, 0xD5, 0x00, 0x06, 0xE4, 0x0C, 0x7E,
    0x79, 0x00, 0x00, 0x98, 0x5A, 0xD4, 0x1F, 0x00, 0x00, 0x3B, 0xCE, 0xC1,
    0x00, 0x00, 0x00, 0x00, 0xDD, 0x65, 0x00, 0x00, 0xD3, 0x27, 0x00, 0xA4,
    0x95, 0x00, 0x38, 0xB5, 0x88, 0x68, 0x00, 0xCE, 0xD0, 0x00, 0x7E, 0x69,
    0x3E, 0xA9, 0x24, 0xA6, 0xD6, 0x11, 0xC4, 0x1E, 0x04, 0xD9, 0x65, 0x66,
    0x98, 0x5D, 0xCC, 0x00, 0x00, 0xA8, 0xD8, 0x27, 0x53, 0xE2, 0x85, 0x00,
    0x00, 0x5D, 0xE7, 0x00, 0x11, 0xFD, 0x39, 0x00, 0x6F, 0xB8, 0x00, 0x33,
    0xDE, 0x0D, 0x01, 0xC1, 0x58, 0xCA, 0x43, 0x00, 0x00, 0x22, 0xF5, 0x9A,
    0x00, 0x00, 0x00, 0x3F, 0xF3, 0xC0, 0x01, 0x00, 0x0B, 0xDC, 0x43, 0xC1,
    0x6C, 0x00, 0x96, 0x95, 0x00, 0x23, 0xE9, 0x22, 0xA3, 0x65, 0x00, 0x00,
    0xBC, 0x39, 0x41, 0xC0, 0x00, 0x15, 0xDA, 0x00, 0x01, 0xDB, 0x1D, 0x68,
    0x83, 0x00, 0x00, 0x7C, 0x74, 0xBF, 0x29, 0x00, 0x00, 0x1C, 0xD8, 0xCC,
    0x00, 0x00, 0x00, 0x00, 0xC2, 0x72, 0x00, 0x00, 0x04, 0x20, 0xE7, 0x16,
    0x00, 0x00, 0x54, 0xEF, 0x65, 0x00, 0x00, 0x00, 0x90, 0xFF, 0xFF, 0xFF,
    0xFF, 0x15, 0x00, 0x00, 0x00, 0x99, 0x87, 0x00, 0x00, 0x00, 0x7F, 0xAA,
    0x01, 0x00, 0x00, 0x63, 0xC7, 0x08, 0x00, 0x00, 0x4B, 0xD2, 0x16, 0x00,
    0x00, 0x00, 0xC7, 0xFF, 0xFF, 0xFF, 0xFF, 0x44, 0x00, 0x3C, 0xEC, 0x6B,
    0x00, 0x8E, 0x6E, 0x00, 0x00, 0x9A, 0x50, 0x00, 0x11, 0xD2, 0x2B, 0x00,
    0xB0, 0x9C, 0x00, 0x00, 0x15, 0xD9, 0x1E, 0x00, 0x00, 0x9F, 0x4B, 0x00,
    0x00, 0x96, 0x54, 0x00, 0x00, 0x86, 0x75, 0x00, 0x00, 0x2B, 0xE5, 0x6B,
    0xD8, 0xD8, 0xD8, 0xD8, 0xD8, 0xD8, 0xD8, 0xD8, 0xD8, 0xD8, 0xBE, 0xC8,
    0x07, 0x00, 0x04, 0xC2, 0x37, 0x00, 0x00, 0xA6, 0x42, 0x00, 0x00, 0x84,
    0x84, 0x04, 0x00, 0x0D, 0xDB, 0x5C, 0x00, 0x68, 0x92, 0x05, 0x00, 0xA2,
    0x45, 0x00, 0x00, 0xA8, 0x3F, 0x00, 0x05, 0xC6, 0x2F, 0x00, 0xBE, 0xBD,
    0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x14, 0x13, 0xD4, 0xDF, 0x6E,
    0x0F, 0x97, 0x6B, 0x2D, 0x32, 0xB7, 0xF3, 0x5B, 0x11, 0x00, 0x00, 0x00,
    0x00, 0x00, 0xA0, 0xFF, 0xFF, 0xFF, 0xFF, 0xE0, 0x2C, 0x00, 0x00, 0x00,
    0x00, 0x2C, 0x2C, 0x00, 0x00, 0x00, 0x00, 0x2C, 0x2C, 0x00, 0x00, 0x00,
    0x00, 0x2C, 0x2C, 0x00, 0x00, 0x00, 0x00, 0x2C, 0x2C, 0x00, 0x00, 0x00,
    0x00, 0x2C, 0x2C, 0x00, 0x00, 0x00, 0x00, 0x2C, 0xA0, 0xFF, 0xFF, 0xFF,
    0xFF, 0xE0
    };

    /* This struct will be read by the CPU only */
    static const nema_glyph_t arial8pt8bGlyphs0[] = {
    {     0,   0,   3,    0,    1 },   // 0x20 ' '
    {     0,   3,   3,    0,   -7 },   // 0x21 '!'
    {    24,   4,   4,    0,   -7 },   // 0x22 '"'
    {    36,   6,   6,    0,   -7 },   // 0x23 '#'
    {    84,   6,   6,    0,   -7 },   // 0x24 '$'
    {   138,  10,  10,    0,   -7 },   // 0x25 '%'
    {   218,   8,   7,    0,   -7 },   // 0x26 '&'
    {   282,   2,   2,    0,   -7 },   // 0x27 '''
    {   288,   4,   4,    0,   -7 },   // 0x28 '('
    {   328,   4,   4,    0,   -7 },   // 0x29 ')'
    {   368,   4,   4,    0,   -7 },   // 0x2A '*'
    {   384,   6,   6,    0,   -5 },   // 0x2B '+'
    {   414,   3,   3,    0,    0 },   // 0x2C ','
    {   423,   4,   4,    0,   -2 },   // 0x2D '-'
    {   427,   2,   3,    1,    0 },   // 0x2E '.'
    {   429,   4,   3,    0,   -7 },   // 0x2F '/'
    {   461,   6,   6,    0,   -7 },   // 0x30 '0'
    {   509,   4,   6,    1,   -7 },   // 0x31 '1'
    {   541,   6,   6,    0,   -7 },   // 0x32 '2'
    {   589,   6,   6,    0,   -7 },   // 0x33 '3'
    {   637,   6,   6,    0,   -7 },   // 0x34 '4'
    {   685,   6,   6,    0,   -7 },   // 0x35 '5'
    {   733,   6,   6,    0,   -7 },   // 0x36 '6'
    {   781,   6,   6,    0,   -7 },   // 0x37 '7'
    {   829,   6,   6,    0,   -7 },   // 0x38 '8'
    {   877,   6,   6,    0,   -7 },   // 0x39 '9'
    {   925,   2,   3,    1,   -5 },   // 0x3A ':'
    {   937,   3,   3,    0,   -5 },   // 0x3B ';'
    {   961,   6,   6,    0,   -5 },   // 0x3C '<'
    {   991,   6,   6,    0,   -4 },   // 0x3D '='
    {  1009,   6,   6,    0,   -5 },   // 0x3E '>'
    {  1039,   6,   6,    0,   -7 },   // 0x3F '?'
    {  1087,  11,  11,    0,   -7 },   // 0x40 '@'
    {  1197,   9,   7,   -1,   -7 },   // 0x41 'A'
    {  1269,   7,   7,    0,   -7 },   // 0x42 'B'
    {  1325,   8,   8,    0,   -7 },   // 0x43 'C'
    {  1389,   8,   8,    0,   -7 },   // 0x44 'D'
    {  1453,   7,   7,    0,   -7 },   // 0x45 'E'
    {  1509,   7,   7,    0,   -7 },   // 0x46 'F'
    {  1565,   8,   9,    0,   -7 },   // 0x47 'G'
    {  1629,   8,   8,    0,   -7 },   // 0x48 'H'
    {  1693,   2,   3,    1,   -7 },   // 0x49 'I'
    {  1709,   5,   6,    0,   -7 },   // 0x4A 'J'
    {  1749,   8,   7,    0,   -7 },   // 0x4B 'K'
    {  1813,   6,   6,    0,   -7 },   // 0x4C 'L'
    {  1861,   9,   9,    0,   -7 },   // 0x4D 'M'
    {  1933,   8,   8,    0,   -7 },   // 0x4E 'N'
    {  1997,   9,   9,    0,   -7 },   // 0x4F 'O'
    {  2069,   7,   7,    0,   -7 },   // 0x50 'P'
    {  2125,   9,   9,    0,   -7 },   // 0x51 'Q'
    {  2206,   8,   8,    0,   -7 },   // 0x52 'R'
    {  2270,   7,   7,    0,   -7 },   // 0x53 'S'
    {  2326,   7,   7,    0,   -7 },   // 0x54 'T'
    {  2382,   8,   8,    0,   -7 },   // 0x55 'U'
    {  2446,   8,   7,    0,   -7 },   // 0x56 'V'
    {  2510,  11,  10,    0,   -7 },   // 0x57 'W'
    {  2598,   8,   7,    0,   -7 },   // 0x58 'X'
    {  2662,   8,   7,    0,   -7 },   // 0x59 'Y'
    {  2726,   7,   7,    0,   -7 },   // 0x5A 'Z'
    {  2782,   3,   3,    0,   -7 },   // 0x5B '['
    {  2812,   4,   3,    0,   -7 },   // 0x5C '\'
    {  2844,   3,   3,    0,   -7 },   // 0x5D ']'
    {  2874,   5,   5,    0,   -7 },   // 0x5E '^'
    {  2894,   8,   6,   -1,    2 },   // 0x5F '_'
    {  2902,   3,   4,    0,   -7 },   // 0x60 '`'
    {  2908,   6,   6,    0,   -5 },   // 0x61 'a'
    {  2944,   6,   6,    0,   -7 },   // 0x62 'b'
    {  2992,   6,   6,    0,   -5 },   // 0x63 'c'
    {  3028,   6,   6,    0,   -7 },   // 0x64 'd'
    {  3076,   6,   6,    0,   -5 },   // 0x65 'e'
    {  3112,   4,   3,    0,   -7 },   // 0x66 'f'
    {  3144,   6,   6,    0,   -5 },   // 0x67 'g'
    {  3192,   6,   6,    0,   -7 },   // 0x68 'h'
    {  3240,   2,   2,    0,   -7 },   // 0x69 'i'
    {  3256,   3,   2,   -1,   -7 },   // 0x6A 'j'
    {  3286,   6,   6,    0,   -7 },   // 0x6B 'k'
    {  3334,   2,   2,    0,   -7 },   // 0x6C 'l'
    {  3350,   9,   9,    0,   -5 },   // 0x6D 'm'
    {  3404,   6,   6,    0,   -5 },   // 0x6E 'n'
    {  3440,   6,   6,    0,   -5 },   // 0x6F 'o'
    {  3476,   6,   6,    0,   -5 },   // 0x70 'p'
    {  3524,   6,   6,    0,   -5 },   // 0x71 'q'
    {  3572,   4,   4,    0,   -5 },   // 0x72 'r'
    {  3596,   6,   6,    0,   -5 },   // 0x73 's'
    {  3632,   3,   3,    0,   -7 },   // 0x74 't'
    {  3656,   6,   6,    0,   -5 },   // 0x75 'u'
    {  3692,   6,   6,    0,   -5 },   // 0x76 'v'
    {  3728,   8,   8,    0,   -5 },   // 0x77 'w'
    {  3776,   6,   6,    0,   -5 },   // 0x78 'x'
    {  3812,   6,   6,    0,   -5 },   // 0x79 'y'
    {  3860,   6,   6,    0,   -5 },   // 0x7A 'z'
    {  3896,   4,   4,    0,   -7 },   // 0x7B '{'
    {  3936,   1,   3,    1,   -7 },   // 0x7C '|'
    {  3946,   4,   4,    0,   -7 },   // 0x7D '}'
    {  3986,   6,   6,    0,   -5 },   // 0x7E '~'
    {  4010,   6,   8,    1,   -7 },   // 0x7F
    {  4058,   0,   0,    0,    0 }
};

/* This struct will be read by the CPU only */
static const nema_font_range_t arial8pt8b_ranges[] =
{
    {0x00000020, 0x0000007f, arial8pt8bGlyphs0},
    {0, 0, NULL}
};

/* This struct will be read by the CPU only */
nema_font_t arial8pt8b =
{
    {
        .base_virt = (void *) arial8pt8bBitmaps,
        .base_phys = (uintptr_t) arial8pt8bBitmaps,
        .size      = (int) sizeof(arial8pt8bBitmaps)
    },
    arial8pt8b_ranges,
    (int)sizeof(arial8pt8bBitmaps),
    arial8pt8bBitmaps,
    0,
    3, 12, 8, 8
};
#endif // ARIAL8PT8B_C

