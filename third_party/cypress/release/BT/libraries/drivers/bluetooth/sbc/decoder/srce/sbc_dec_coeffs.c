/******************************************************************************
**
**  File Name:   $RCSfile: sbc_dec_coeffs.c,v $
**
**  Description: This file contains the coefficient table used for Windowing
**                  in synthesis.
**
**  Revision :   $Id: sbc_dec_coeffs.c,v 1.4 2004/10/22 14:48:47 mjougit Exp $
**
**  Copyright (c) 2002-2008, Broadcom Corp., All Rights Reserved.
**  Broadcom Bluetooth Core. Proprietary and confidential.
**
******************************************************************************/

#include "sbc_decoder.h"

#if (SBC_ARM_OPT==FALSE && SBC_IPAQ_OPT==FALSE)

#if (SBC_IS_64_BITS_IN_WINDOWING==TRUE)
#define SBC_CONST_VAR (0x80000000)
#else
#define SBC_CONST_VAR (0x00008000)
#endif

const SINT32 coeffFor4SBs[] =
{
    (SINT32)(0 * SBC_CONST_VAR),
    (SINT32)(0.000536548976 * SBC_CONST_VAR),
    (SINT32)(0.00149188357 * SBC_CONST_VAR),
    (SINT32)(0.00273370904 * SBC_CONST_VAR),
    (SINT32)(0.00383720193 * SBC_CONST_VAR),
    (SINT32)(0.00389205149 * SBC_CONST_VAR),
    (SINT32)(0.00186581691 * SBC_CONST_VAR),
    (SINT32)(-0.00306012286 * SBC_CONST_VAR),
    (SINT32)(0.0109137620 * SBC_CONST_VAR),
    (SINT32)(0.0204385087 * SBC_CONST_VAR),
    (SINT32)(0.0288757392 * SBC_CONST_VAR),
    (SINT32)(0.0321939290 * SBC_CONST_VAR),
    (SINT32)(0.0258767811 * SBC_CONST_VAR),
    (SINT32)(0.00613245186 * SBC_CONST_VAR),
    (SINT32)(-0.0288217274 * SBC_CONST_VAR),
    (SINT32)(-0.0776463494 * SBC_CONST_VAR),
    (SINT32)(0.135593274 * SBC_CONST_VAR),
    (SINT32)(0.194987841 * SBC_CONST_VAR),
    (SINT32)(0.246636662 * SBC_CONST_VAR),
    (SINT32)(0.281828203 * SBC_CONST_VAR),
    (SINT32)(0.294315332 * SBC_CONST_VAR),
    (SINT32)(0.281828203 * SBC_CONST_VAR),
    (SINT32)(0.246636662 * SBC_CONST_VAR),
    (SINT32)(0.194987841 * SBC_CONST_VAR),
    (SINT32)(-0.135593274 * SBC_CONST_VAR),
    (SINT32)(-0.0776463494 * SBC_CONST_VAR),
    (SINT32)(-0.0288217274 * SBC_CONST_VAR),
    (SINT32)(0.00613245186 * SBC_CONST_VAR),
    (SINT32)(0.0258767811 * SBC_CONST_VAR),
    (SINT32)(0.0321939290 * SBC_CONST_VAR),
    (SINT32)(0.0288757392 * SBC_CONST_VAR),
    (SINT32)(0.0204385087 * SBC_CONST_VAR),
    (SINT32)(-0.0109137620 * SBC_CONST_VAR),
    (SINT32)(-0.00306012286 * SBC_CONST_VAR),
    (SINT32)(0.00186581691 * SBC_CONST_VAR),
    (SINT32)(0.00389205149 * SBC_CONST_VAR),
    (SINT32)(0.00383720193 * SBC_CONST_VAR),
    (SINT32)(0.00273370904 * SBC_CONST_VAR),
    (SINT32)(0.00149188357 * SBC_CONST_VAR),
    (SINT32)(0.000536548976 * SBC_CONST_VAR)
};

const SINT32 coeffFor8SBs[] =
{
    (SINT32)(0.00000000 * SBC_CONST_VAR),
    (SINT32)(0.000156575398 * SBC_CONST_VAR),
    (SINT32)(0.000343256425 * SBC_CONST_VAR),
    (SINT32)(0.000554620202 * SBC_CONST_VAR),
    (SINT32)(0.000823919506 * SBC_CONST_VAR),
    (SINT32)(0.00113992507 * SBC_CONST_VAR),
    (SINT32)(0.00147640169 * SBC_CONST_VAR),
    (SINT32)(0.00178371725 * SBC_CONST_VAR),
    (SINT32)(0.00201182542 * SBC_CONST_VAR),
    (SINT32)(0.00210371989 * SBC_CONST_VAR),
    (SINT32)(0.00199454554 * SBC_CONST_VAR),
    (SINT32)(0.00161656283 * SBC_CONST_VAR),
    (SINT32)(0.000902154502 * SBC_CONST_VAR),
    (SINT32)(-0.000178805361 * SBC_CONST_VAR),
    (SINT32)(-0.00164973098 * SBC_CONST_VAR),
    (SINT32)(-0.00349717454 * SBC_CONST_VAR),
    (SINT32)(0.00565949473 * SBC_CONST_VAR),
    (SINT32)(0.00802941163 * SBC_CONST_VAR),
    (SINT32)(0.0104584443 * SBC_CONST_VAR),
    (SINT32)(0.0127472335 * SBC_CONST_VAR),
    (SINT32)(0.0146525263 * SBC_CONST_VAR),
    (SINT32)(0.0159045603 * SBC_CONST_VAR),
    (SINT32)(0.0162208471 * SBC_CONST_VAR),
    (SINT32)(0.0153184106 * SBC_CONST_VAR),
    (SINT32)(0.0129371806 * SBC_CONST_VAR),
    (SINT32)(0.00885757540 * SBC_CONST_VAR),
    (SINT32)(0.00292408442 * SBC_CONST_VAR),
    (SINT32)(-0.00491578024 * SBC_CONST_VAR),
    (SINT32)(-0.0146404076 * SBC_CONST_VAR),
    (SINT32)(-0.0261098752 * SBC_CONST_VAR),
    (SINT32)(-0.0390751381 * SBC_CONST_VAR),
    (SINT32)(-0.0531873032 * SBC_CONST_VAR),
    (SINT32)(0.0679989431 * SBC_CONST_VAR),
    (SINT32)(0.0829847578 * SBC_CONST_VAR),
    (SINT32)(0.0975753918 * SBC_CONST_VAR),
    (SINT32)(0.111196689 * SBC_CONST_VAR),
    (SINT32)(0.123264548 * SBC_CONST_VAR),
    (SINT32)(0.133264415 * SBC_CONST_VAR),
    (SINT32)(0.140753505 * SBC_CONST_VAR),
    (SINT32)(0.145389847 * SBC_CONST_VAR),
    (SINT32)(0.146955068 * SBC_CONST_VAR),
    (SINT32)(0.145389847 * SBC_CONST_VAR),
    (SINT32)(0.140753505 * SBC_CONST_VAR),
    (SINT32)(0.133264415 * SBC_CONST_VAR),
    (SINT32)(0.123264548 * SBC_CONST_VAR),
    (SINT32)(0.111196689 * SBC_CONST_VAR),
    (SINT32)(0.0975753918 * SBC_CONST_VAR),
    (SINT32)(0.0829847578 * SBC_CONST_VAR),
    (SINT32)(-0.0679989431 * SBC_CONST_VAR),
    (SINT32)(-0.0531873032 * SBC_CONST_VAR),
    (SINT32)(-0.0390751381 * SBC_CONST_VAR),
    (SINT32)(-0.0261098752 * SBC_CONST_VAR),
    (SINT32)(-0.0146404076 * SBC_CONST_VAR),
    (SINT32)(-0.00491578024 * SBC_CONST_VAR),
    (SINT32)(0.00292408442 * SBC_CONST_VAR),
    (SINT32)(0.00885757540 * SBC_CONST_VAR),
    (SINT32)(0.0129371806 * SBC_CONST_VAR),
    (SINT32)(0.0153184106 * SBC_CONST_VAR),
    (SINT32)(0.0162208471 * SBC_CONST_VAR),
    (SINT32)(0.0159045603 * SBC_CONST_VAR),
    (SINT32)(0.0146525263 * SBC_CONST_VAR),
    (SINT32)(0.0127472335 * SBC_CONST_VAR),
    (SINT32)(0.0104584443 * SBC_CONST_VAR),
    (SINT32)(0.00802941163 * SBC_CONST_VAR),
    (SINT32)(-0.00565949473 * SBC_CONST_VAR),
    (SINT32)(-0.00349717454 * SBC_CONST_VAR),
    (SINT32)(-0.00164973098 * SBC_CONST_VAR),
    (SINT32)(-0.000178805361 * SBC_CONST_VAR),
    (SINT32)(0.000902154502 * SBC_CONST_VAR),
    (SINT32)(0.00161656283 * SBC_CONST_VAR),
    (SINT32)(0.00199454554 * SBC_CONST_VAR),
    (SINT32)(0.00210371989 * SBC_CONST_VAR),
    (SINT32)(0.00201182542 * SBC_CONST_VAR),
    (SINT32)(0.00178371725 * SBC_CONST_VAR),
    (SINT32)(0.00147640169 * SBC_CONST_VAR),
    (SINT32)(0.00113992507 * SBC_CONST_VAR),
    (SINT32)(0.000823919506 * SBC_CONST_VAR),
    (SINT32)(0.000554620202 * SBC_CONST_VAR),
    (SINT32)(0.000343256425 * SBC_CONST_VAR),
    (SINT32)(0.000156575398 * SBC_CONST_VAR)
};
#endif
