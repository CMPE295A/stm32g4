/**
  ******************************************************************************
  * @file    ECC/EDDSA_SignVerify/Src/main.c
  * @author  MCD Application Team
  * @brief   This example provides a short description of how to use the
  *          STM32 Cryptographic Library
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file in
  * the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include <string.h>
#include "main.h"
#include "cmox_crypto.h"

/* Global variables ----------------------------------------------------------*/
/* ECC context */
cmox_ecc_handle_t Ecc_Ctx;
/* ECC working buffer */
uint8_t Working_Buffer[2200];

__IO TestStatus glob_status = FAILED;

/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/** Extract from Edwards-Curve Digital Signature Algorithm (EdDSA)
  * -----TEST 1024

   ALGORITHM:
   Ed25519

   SECRET KEY:
   f5e5767cf153319517630f226876b86c
   8160cc583bc013744c6bf255f5cc0ee5

   PUBLIC KEY:
   278117fc144c72340f67d0f2316e8386
   ceffbf2b2428c9c51fef7c597f1d426e

   MESSAGE (length 1023 bytes):
   08b8b2b733424243760fe426a4b54908
   632110a66c2f6591eabd3345e3e4eb98
   fa6e264bf09efe12ee50f8f54e9f77b1
   e355f6c50544e23fb1433ddf73be84d8
   79de7c0046dc4996d9e773f4bc9efe57
   38829adb26c81b37c93a1b270b20329d
   658675fc6ea534e0810a4432826bf58c
   941efb65d57a338bbd2e26640f89ffbc
   1a858efcb8550ee3a5e1998bd177e93a
   7363c344fe6b199ee5d02e82d522c4fe
   ba15452f80288a821a579116ec6dad2b
   3b310da903401aa62100ab5d1a36553e
   06203b33890cc9b832f79ef80560ccb9
   a39ce767967ed628c6ad573cb116dbef
   efd75499da96bd68a8a97b928a8bbc10
   3b6621fcde2beca1231d206be6cd9ec7
   aff6f6c94fcd7204ed3455c68c83f4a4
   1da4af2b74ef5c53f1d8ac70bdcb7ed1
   85ce81bd84359d44254d95629e9855a9
   4a7c1958d1f8ada5d0532ed8a5aa3fb2
   d17ba70eb6248e594e1a2297acbbb39d
   502f1a8c6eb6f1ce22b3de1a1f40cc24
   554119a831a9aad6079cad88425de6bd
   e1a9187ebb6092cf67bf2b13fd65f270
   88d78b7e883c8759d2c4f5c65adb7553
   878ad575f9fad878e80a0c9ba63bcbcc
   2732e69485bbc9c90bfbd62481d9089b
   eccf80cfe2df16a2cf65bd92dd597b07
   07e0917af48bbb75fed413d238f5555a
   7a569d80c3414a8d0859dc65a46128ba
   b27af87a71314f318c782b23ebfe808b
   82b0ce26401d2e22f04d83d1255dc51a
   ddd3b75a2b1ae0784504df543af8969b
   e3ea7082ff7fc9888c144da2af58429e
   c96031dbcad3dad9af0dcbaaaf268cb8
   fcffead94f3c7ca495e056a9b47acdb7
   51fb73e666c6c655ade8297297d07ad1
   ba5e43f1bca32301651339e22904cc8c
   42f58c30c04aafdb038dda0847dd988d
   cda6f3bfd15c4b4c4525004aa06eeff8
   ca61783aacec57fb3d1f92b0fe2fd1a8
   5f6724517b65e614ad6808d6f6ee34df
   f7310fdc82aebfd904b01e1dc54b2927
   094b2db68d6f903b68401adebf5a7e08
   d78ff4ef5d63653a65040cf9bfd4aca7
   984a74d37145986780fc0b16ac451649
   de6188a7dbdf191f64b5fc5e2ab47b57
   f7f7276cd419c17a3ca8e1b939ae49e4
   88acba6b965610b5480109c8b17b80e1
   b7b750dfc7598d5d5011fd2dcc5600a3
   2ef5b52a1ecc820e308aa342721aac09
   43bf6686b64b2579376504ccc493d97e
   6aed3fb0f9cd71a43dd497f01f17c0e2
   cb3797aa2a2f256656168e6c496afc5f
   b93246f6b1116398a346f1a641f3b041
   e989f7914f90cc2c7fff357876e506b5
   0d334ba77c225bc307ba537152f3f161
   0e4eafe595f6d9d90d11faa933a15ef1
   369546868a7f3a45a96768d40fd9d034
   12c091c6315cf4fde7cb68606937380d
   b2eaaa707b4c4185c32eddcdd306705e
   4dc1ffc872eeee475a64dfac86aba41c
   0618983f8741c5ef68d3a101e8a3b8ca
   c60c905c15fc910840b94c00a0b9d0

   SIGNATURE:
   0aab4c900501b3e24d7cdf4663326a3a
   87df5e4843b2cbdb67cbf6e460fec350
   aa5371b1508f9f4528ecea23c436d94b
   5e8fcd4f681e30a6ac00a9704a188a03

  */
const uint8_t Message[] =
{
  0x08, 0xb8, 0xb2, 0xb7, 0x33, 0x42, 0x42, 0x43, 0x76, 0x0f, 0xe4, 0x26, 0xa4, 0xb5, 0x49, 0x08,
  0x63, 0x21, 0x10, 0xa6, 0x6c, 0x2f, 0x65, 0x91, 0xea, 0xbd, 0x33, 0x45, 0xe3, 0xe4, 0xeb, 0x98,
  0xfa, 0x6e, 0x26, 0x4b, 0xf0, 0x9e, 0xfe, 0x12, 0xee, 0x50, 0xf8, 0xf5, 0x4e, 0x9f, 0x77, 0xb1,
  0xe3, 0x55, 0xf6, 0xc5, 0x05, 0x44, 0xe2, 0x3f, 0xb1, 0x43, 0x3d, 0xdf, 0x73, 0xbe, 0x84, 0xd8,
  0x79, 0xde, 0x7c, 0x00, 0x46, 0xdc, 0x49, 0x96, 0xd9, 0xe7, 0x73, 0xf4, 0xbc, 0x9e, 0xfe, 0x57,
  0x38, 0x82, 0x9a, 0xdb, 0x26, 0xc8, 0x1b, 0x37, 0xc9, 0x3a, 0x1b, 0x27, 0x0b, 0x20, 0x32, 0x9d,
  0x65, 0x86, 0x75, 0xfc, 0x6e, 0xa5, 0x34, 0xe0, 0x81, 0x0a, 0x44, 0x32, 0x82, 0x6b, 0xf5, 0x8c,
  0x94, 0x1e, 0xfb, 0x65, 0xd5, 0x7a, 0x33, 0x8b, 0xbd, 0x2e, 0x26, 0x64, 0x0f, 0x89, 0xff, 0xbc,
  0x1a, 0x85, 0x8e, 0xfc, 0xb8, 0x55, 0x0e, 0xe3, 0xa5, 0xe1, 0x99, 0x8b, 0xd1, 0x77, 0xe9, 0x3a,
  0x73, 0x63, 0xc3, 0x44, 0xfe, 0x6b, 0x19, 0x9e, 0xe5, 0xd0, 0x2e, 0x82, 0xd5, 0x22, 0xc4, 0xfe,
  0xba, 0x15, 0x45, 0x2f, 0x80, 0x28, 0x8a, 0x82, 0x1a, 0x57, 0x91, 0x16, 0xec, 0x6d, 0xad, 0x2b,
  0x3b, 0x31, 0x0d, 0xa9, 0x03, 0x40, 0x1a, 0xa6, 0x21, 0x00, 0xab, 0x5d, 0x1a, 0x36, 0x55, 0x3e,
  0x06, 0x20, 0x3b, 0x33, 0x89, 0x0c, 0xc9, 0xb8, 0x32, 0xf7, 0x9e, 0xf8, 0x05, 0x60, 0xcc, 0xb9,
  0xa3, 0x9c, 0xe7, 0x67, 0x96, 0x7e, 0xd6, 0x28, 0xc6, 0xad, 0x57, 0x3c, 0xb1, 0x16, 0xdb, 0xef,
  0xef, 0xd7, 0x54, 0x99, 0xda, 0x96, 0xbd, 0x68, 0xa8, 0xa9, 0x7b, 0x92, 0x8a, 0x8b, 0xbc, 0x10,
  0x3b, 0x66, 0x21, 0xfc, 0xde, 0x2b, 0xec, 0xa1, 0x23, 0x1d, 0x20, 0x6b, 0xe6, 0xcd, 0x9e, 0xc7,
  0xaf, 0xf6, 0xf6, 0xc9, 0x4f, 0xcd, 0x72, 0x04, 0xed, 0x34, 0x55, 0xc6, 0x8c, 0x83, 0xf4, 0xa4,
  0x1d, 0xa4, 0xaf, 0x2b, 0x74, 0xef, 0x5c, 0x53, 0xf1, 0xd8, 0xac, 0x70, 0xbd, 0xcb, 0x7e, 0xd1,
  0x85, 0xce, 0x81, 0xbd, 0x84, 0x35, 0x9d, 0x44, 0x25, 0x4d, 0x95, 0x62, 0x9e, 0x98, 0x55, 0xa9,
  0x4a, 0x7c, 0x19, 0x58, 0xd1, 0xf8, 0xad, 0xa5, 0xd0, 0x53, 0x2e, 0xd8, 0xa5, 0xaa, 0x3f, 0xb2,
  0xd1, 0x7b, 0xa7, 0x0e, 0xb6, 0x24, 0x8e, 0x59, 0x4e, 0x1a, 0x22, 0x97, 0xac, 0xbb, 0xb3, 0x9d,
  0x50, 0x2f, 0x1a, 0x8c, 0x6e, 0xb6, 0xf1, 0xce, 0x22, 0xb3, 0xde, 0x1a, 0x1f, 0x40, 0xcc, 0x24,
  0x55, 0x41, 0x19, 0xa8, 0x31, 0xa9, 0xaa, 0xd6, 0x07, 0x9c, 0xad, 0x88, 0x42, 0x5d, 0xe6, 0xbd,
  0xe1, 0xa9, 0x18, 0x7e, 0xbb, 0x60, 0x92, 0xcf, 0x67, 0xbf, 0x2b, 0x13, 0xfd, 0x65, 0xf2, 0x70,
  0x88, 0xd7, 0x8b, 0x7e, 0x88, 0x3c, 0x87, 0x59, 0xd2, 0xc4, 0xf5, 0xc6, 0x5a, 0xdb, 0x75, 0x53,
  0x87, 0x8a, 0xd5, 0x75, 0xf9, 0xfa, 0xd8, 0x78, 0xe8, 0x0a, 0x0c, 0x9b, 0xa6, 0x3b, 0xcb, 0xcc,
  0x27, 0x32, 0xe6, 0x94, 0x85, 0xbb, 0xc9, 0xc9, 0x0b, 0xfb, 0xd6, 0x24, 0x81, 0xd9, 0x08, 0x9b,
  0xec, 0xcf, 0x80, 0xcf, 0xe2, 0xdf, 0x16, 0xa2, 0xcf, 0x65, 0xbd, 0x92, 0xdd, 0x59, 0x7b, 0x07,
  0x07, 0xe0, 0x91, 0x7a, 0xf4, 0x8b, 0xbb, 0x75, 0xfe, 0xd4, 0x13, 0xd2, 0x38, 0xf5, 0x55, 0x5a,
  0x7a, 0x56, 0x9d, 0x80, 0xc3, 0x41, 0x4a, 0x8d, 0x08, 0x59, 0xdc, 0x65, 0xa4, 0x61, 0x28, 0xba,
  0xb2, 0x7a, 0xf8, 0x7a, 0x71, 0x31, 0x4f, 0x31, 0x8c, 0x78, 0x2b, 0x23, 0xeb, 0xfe, 0x80, 0x8b,
  0x82, 0xb0, 0xce, 0x26, 0x40, 0x1d, 0x2e, 0x22, 0xf0, 0x4d, 0x83, 0xd1, 0x25, 0x5d, 0xc5, 0x1a,
  0xdd, 0xd3, 0xb7, 0x5a, 0x2b, 0x1a, 0xe0, 0x78, 0x45, 0x04, 0xdf, 0x54, 0x3a, 0xf8, 0x96, 0x9b,
  0xe3, 0xea, 0x70, 0x82, 0xff, 0x7f, 0xc9, 0x88, 0x8c, 0x14, 0x4d, 0xa2, 0xaf, 0x58, 0x42, 0x9e,
  0xc9, 0x60, 0x31, 0xdb, 0xca, 0xd3, 0xda, 0xd9, 0xaf, 0x0d, 0xcb, 0xaa, 0xaf, 0x26, 0x8c, 0xb8,
  0xfc, 0xff, 0xea, 0xd9, 0x4f, 0x3c, 0x7c, 0xa4, 0x95, 0xe0, 0x56, 0xa9, 0xb4, 0x7a, 0xcd, 0xb7,
  0x51, 0xfb, 0x73, 0xe6, 0x66, 0xc6, 0xc6, 0x55, 0xad, 0xe8, 0x29, 0x72, 0x97, 0xd0, 0x7a, 0xd1,
  0xba, 0x5e, 0x43, 0xf1, 0xbc, 0xa3, 0x23, 0x01, 0x65, 0x13, 0x39, 0xe2, 0x29, 0x04, 0xcc, 0x8c,
  0x42, 0xf5, 0x8c, 0x30, 0xc0, 0x4a, 0xaf, 0xdb, 0x03, 0x8d, 0xda, 0x08, 0x47, 0xdd, 0x98, 0x8d,
  0xcd, 0xa6, 0xf3, 0xbf, 0xd1, 0x5c, 0x4b, 0x4c, 0x45, 0x25, 0x00, 0x4a, 0xa0, 0x6e, 0xef, 0xf8,
  0xca, 0x61, 0x78, 0x3a, 0xac, 0xec, 0x57, 0xfb, 0x3d, 0x1f, 0x92, 0xb0, 0xfe, 0x2f, 0xd1, 0xa8,
  0x5f, 0x67, 0x24, 0x51, 0x7b, 0x65, 0xe6, 0x14, 0xad, 0x68, 0x08, 0xd6, 0xf6, 0xee, 0x34, 0xdf,
  0xf7, 0x31, 0x0f, 0xdc, 0x82, 0xae, 0xbf, 0xd9, 0x04, 0xb0, 0x1e, 0x1d, 0xc5, 0x4b, 0x29, 0x27,
  0x09, 0x4b, 0x2d, 0xb6, 0x8d, 0x6f, 0x90, 0x3b, 0x68, 0x40, 0x1a, 0xde, 0xbf, 0x5a, 0x7e, 0x08,
  0xd7, 0x8f, 0xf4, 0xef, 0x5d, 0x63, 0x65, 0x3a, 0x65, 0x04, 0x0c, 0xf9, 0xbf, 0xd4, 0xac, 0xa7,
  0x98, 0x4a, 0x74, 0xd3, 0x71, 0x45, 0x98, 0x67, 0x80, 0xfc, 0x0b, 0x16, 0xac, 0x45, 0x16, 0x49,
  0xde, 0x61, 0x88, 0xa7, 0xdb, 0xdf, 0x19, 0x1f, 0x64, 0xb5, 0xfc, 0x5e, 0x2a, 0xb4, 0x7b, 0x57,
  0xf7, 0xf7, 0x27, 0x6c, 0xd4, 0x19, 0xc1, 0x7a, 0x3c, 0xa8, 0xe1, 0xb9, 0x39, 0xae, 0x49, 0xe4,
  0x88, 0xac, 0xba, 0x6b, 0x96, 0x56, 0x10, 0xb5, 0x48, 0x01, 0x09, 0xc8, 0xb1, 0x7b, 0x80, 0xe1,
  0xb7, 0xb7, 0x50, 0xdf, 0xc7, 0x59, 0x8d, 0x5d, 0x50, 0x11, 0xfd, 0x2d, 0xcc, 0x56, 0x00, 0xa3,
  0x2e, 0xf5, 0xb5, 0x2a, 0x1e, 0xcc, 0x82, 0x0e, 0x30, 0x8a, 0xa3, 0x42, 0x72, 0x1a, 0xac, 0x09,
  0x43, 0xbf, 0x66, 0x86, 0xb6, 0x4b, 0x25, 0x79, 0x37, 0x65, 0x04, 0xcc, 0xc4, 0x93, 0xd9, 0x7e,
  0x6a, 0xed, 0x3f, 0xb0, 0xf9, 0xcd, 0x71, 0xa4, 0x3d, 0xd4, 0x97, 0xf0, 0x1f, 0x17, 0xc0, 0xe2,
  0xcb, 0x37, 0x97, 0xaa, 0x2a, 0x2f, 0x25, 0x66, 0x56, 0x16, 0x8e, 0x6c, 0x49, 0x6a, 0xfc, 0x5f,
  0xb9, 0x32, 0x46, 0xf6, 0xb1, 0x11, 0x63, 0x98, 0xa3, 0x46, 0xf1, 0xa6, 0x41, 0xf3, 0xb0, 0x41,
  0xe9, 0x89, 0xf7, 0x91, 0x4f, 0x90, 0xcc, 0x2c, 0x7f, 0xff, 0x35, 0x78, 0x76, 0xe5, 0x06, 0xb5,
  0x0d, 0x33, 0x4b, 0xa7, 0x7c, 0x22, 0x5b, 0xc3, 0x07, 0xba, 0x53, 0x71, 0x52, 0xf3, 0xf1, 0x61,
  0x0e, 0x4e, 0xaf, 0xe5, 0x95, 0xf6, 0xd9, 0xd9, 0x0d, 0x11, 0xfa, 0xa9, 0x33, 0xa1, 0x5e, 0xf1,
  0x36, 0x95, 0x46, 0x86, 0x8a, 0x7f, 0x3a, 0x45, 0xa9, 0x67, 0x68, 0xd4, 0x0f, 0xd9, 0xd0, 0x34,
  0x12, 0xc0, 0x91, 0xc6, 0x31, 0x5c, 0xf4, 0xfd, 0xe7, 0xcb, 0x68, 0x60, 0x69, 0x37, 0x38, 0x0d,
  0xb2, 0xea, 0xaa, 0x70, 0x7b, 0x4c, 0x41, 0x85, 0xc3, 0x2e, 0xdd, 0xcd, 0xd3, 0x06, 0x70, 0x5e,
  0x4d, 0xc1, 0xff, 0xc8, 0x72, 0xee, 0xee, 0x47, 0x5a, 0x64, 0xdf, 0xac, 0x86, 0xab, 0xa4, 0x1c,
  0x06, 0x18, 0x98, 0x3f, 0x87, 0x41, 0xc5, 0xef, 0x68, 0xd3, 0xa1, 0x01, 0xe8, 0xa3, 0xb8, 0xca,
  0xc6, 0x0c, 0x90, 0x5c, 0x15, 0xfc, 0x91, 0x08, 0x40, 0xb9, 0x4c, 0x00, 0xa0, 0xb9, 0xd0
};
const uint8_t Private_Key[] = /* Secret Key | Public Key */
{
  0xf5, 0xe5, 0x76, 0x7c, 0xf1, 0x53, 0x31, 0x95, 0x17, 0x63, 0x0f, 0x22, 0x68, 0x76, 0xb8, 0x6c,
  0x81, 0x60, 0xcc, 0x58, 0x3b, 0xc0, 0x13, 0x74, 0x4c, 0x6b, 0xf2, 0x55, 0xf5, 0xcc, 0x0e, 0xe5,
  0x27, 0x81, 0x17, 0xfc, 0x14, 0x4c, 0x72, 0x34, 0x0f, 0x67, 0xd0, 0xf2, 0x31, 0x6e, 0x83, 0x86,
  0xce, 0xff, 0xbf, 0x2b, 0x24, 0x28, 0xc9, 0xc5, 0x1f, 0xef, 0x7c, 0x59, 0x7f, 0x1d, 0x42, 0x6e
};
const uint8_t Public_Key[] =
{
  0x27, 0x81, 0x17, 0xfc, 0x14, 0x4c, 0x72, 0x34, 0x0f, 0x67, 0xd0, 0xf2, 0x31, 0x6e, 0x83, 0x86,
  0xce, 0xff, 0xbf, 0x2b, 0x24, 0x28, 0xc9, 0xc5, 0x1f, 0xef, 0x7c, 0x59, 0x7f, 0x1d, 0x42, 0x6e
};
const uint8_t Known_Signature[] =
{
  0x0a, 0xab, 0x4c, 0x90, 0x05, 0x01, 0xb3, 0xe2, 0x4d, 0x7c, 0xdf, 0x46, 0x63, 0x32, 0x6a, 0x3a,
  0x87, 0xdf, 0x5e, 0x48, 0x43, 0xb2, 0xcb, 0xdb, 0x67, 0xcb, 0xf6, 0xe4, 0x60, 0xfe, 0xc3, 0x50,
  0xaa, 0x53, 0x71, 0xb1, 0x50, 0x8f, 0x9f, 0x45, 0x28, 0xec, 0xea, 0x23, 0xc4, 0x36, 0xd9, 0x4b,
  0x5e, 0x8f, 0xcd, 0x4f, 0x68, 0x1e, 0x30, 0xa6, 0xac, 0x00, 0xa9, 0x70, 0x4a, 0x18, 0x8a, 0x03
};

/* Computed data buffer */
uint8_t Computed_Signature[CMOX_ECC_ED25519_SIG_LEN];

/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
static void Error_Handler(void);
/* Functions Definition ------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  cmox_ecc_retval_t retval;
  size_t computed_size;
  /* Fault check verification variable */
  uint32_t fault_check = CMOX_ECC_AUTH_FAIL;

  /* STM32WBxx HAL library initialization:
       - Configure the Flash prefetch
       - Systick timer is configured by default as source of time base, but user 
         can eventually implement his proper time base source (a general purpose 
         timer for example or other time source), keeping in mind that Time base 
         duration should be kept 1ms since PPP_TIMEOUT_VALUEs are defined and 
         handled in milliseconds basis.
       - Set NVIC Group Priority to 4
       - Low Level Initialization
     */
  HAL_Init();

  /* Configure the System clock */
  SystemClock_Config();


  /* Configure LED3 */
  BSP_LED_Init(LED3);

  /* Initialize cryptographic library */
  if (cmox_initialize(NULL) != CMOX_INIT_SUCCESS)
  {
    Error_Handler();
  }

  /* Construct a ECC context, specifying mathematics implementation and working buffer for later processing */
  /* Note: CMOX_ECC256_MATH_FUNCS refer to the default mathematics implementation
   * selected in cmox_default_config.h. To use a specific implementation, user can
   * directly choose:
   * - CMOX_MATH_FUNCS_SMALL to select the mathematics small implementation
   * - CMOX_MATH_FUNCS_FAST to select the mathematics fast implementation
   * - CMOX_MATH_FUNCS_SUPERFAST256 to select the mathematics fast implementation optimized for 256 bits computation
   */
  cmox_ecc_construct(&Ecc_Ctx, CMOX_ECC256_MATH_FUNCS, Working_Buffer, sizeof(Working_Buffer));

  /* Compute directly the signature passing all the needed parameters */
  /* Note: CMOX_ECC_CURVE_ED25519 refer to the default ED25519 definition
   * selected in cmox_default_config.h. To use a specific definition, user can
   * directly choose:
   * - CMOX_ECC_ED25519_OPT_LOWMEM to select the optimized low RAM usage definition (slower computing)
   * - CMOX_ECC_ED25519_HIGHMEM to select the high RAM usage definition (medium computing)
   * - CMOX_ECC_ED25519_OPT_HIGHMEM to select the optimized high RAM usage definition (faster computing)
   */
  retval = cmox_eddsa_sign(&Ecc_Ctx,                                  /* ECC context */
                           CMOX_ECC_CURVE_ED25519,                    /* ED25519 ECC curve selected */
                           Private_Key, sizeof(Private_Key),          /* Private key for signature */
                           Message, sizeof(Message),                  /* Message to sign */
                           Computed_Signature, &computed_size);       /* Data buffer to receive signature */

  /* Verify API returned value */
  if (retval != CMOX_ECC_SUCCESS)
  {
    Error_Handler();
  }

  /* Verify generated data size is the expected one */
  if (computed_size != sizeof(Known_Signature))
  {
    Error_Handler();
  }

  /* Verify generated data are the expected ones */
  if (memcmp(Computed_Signature, Known_Signature, computed_size) != 0)
  {
    Error_Handler();
  }

  /* Cleanup context */
  cmox_ecc_cleanup(&Ecc_Ctx);

  /* Construct a ECC context, specifying mathematics implementation and working buffer for later processing */
  /* Note: CMOX_ECC256_MATH_FUNCS refer to the default mathematics implementation
   * selected in cmox_default_config.h. To use a specific implementation, user can
   * directly choose:
   * - CMOX_MATH_FUNCS_SMALL to select the mathematics small implementation
   * - CMOX_MATH_FUNCS_FAST to select the mathematics fast implementation
   * - CMOX_MATH_FUNCS_SUPERFAST256 to select the mathematics fast implementation optimized for 256 bits computation
   */
  cmox_ecc_construct(&Ecc_Ctx, CMOX_ECC256_MATH_FUNCS, Working_Buffer, sizeof(Working_Buffer));

  /* Verify directly the signature passing all the needed parameters */
  /* Note: CMOX_ECC_CURVE_ED25519 refer to the default ED25519 definition
   * selected in cmox_default_config.h. To use a specific definition, user can
   * directly choose:
   * - CMOX_ECC_ED25519_OPT_LOWMEM to select the optimized low RAM usage definition (slower computing)
   * - CMOX_ECC_ED25519_HIGHMEM to select the high RAM usage definition (medium computing)
   * - CMOX_ECC_ED25519_OPT_HIGHMEM to select the optimized high RAM usage definition (faster computing)
   */
  retval = cmox_eddsa_verify(&Ecc_Ctx,                                  /* ECC context */
                             CMOX_ECC_CURVE_ED25519,                    /* ED25519 ECC curve selected */
                             Public_Key, sizeof(Public_Key),            /* Public key for verification */
                             Message, sizeof(Message),                  /* Message to verify */
                             Known_Signature, sizeof(Known_Signature),  /* Data buffer to receive signature */
                             &fault_check);                             /* Fault check variable:
                                                            to ensure no fault injection occurs during this API call */

  /* Verify API returned value */
  if (retval != CMOX_ECC_AUTH_SUCCESS)
  {
    Error_Handler();
  }
  /* Verify Fault check variable value */
  if (fault_check != CMOX_ECC_AUTH_SUCCESS)
  {
    Error_Handler();
  }

  /* Cleanup context */
  cmox_ecc_cleanup(&Ecc_Ctx);

  /* No more need of cryptographic services, finalize cryptographic library */
  if (cmox_finalize(NULL) != CMOX_INIT_SUCCESS)
  {
    Error_Handler();
  }

  /* Turn on LED3 in an infinite loop in case of ECC EdDSA operations are successful */
  BSP_LED_On(LED3);
  glob_status = PASSED;
  while (1)
  {}
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow :
  *            System Clock source            = PLL (MSI)
  *            SYSCLK(Hz)                     = 64000000
  *            HCLK(Hz)                       = 64000000
  *            HCLK1 Prescaler                = 1
  *            HCKL2 Prescaler                = 2
  *            HCKLS Prescaler                = 1
  *            APB1 Prescaler                 = 1
  *            APB2 Prescaler                 = 1
  *            MSI Frequency(Hz)              = 4000000
  *            PLL_M                          = 1
  *            PLL_N                          = 32
  *            PLL_P                          = 5
  *            PLL_Q                          = 4
  *            PLL_R                          = 2
  *            Flash Latency(WS)              = 3
  */
void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct ={0};
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};

  /* Activate PLL with HSI as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.MSICalibrationValue = RCC_MSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
  RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV1;
  RCC_OscInitStruct.PLL.PLLN = 32;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV5;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV4;
  if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    /* Initialization Error */
    while (1);
  }

  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2
     clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2 | RCC_CLOCKTYPE_HCLK2 | RCC_CLOCKTYPE_HCLK4);
  RCC_ClkInitStruct.SYSCLKSource   = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider  = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.AHBCLK2Divider = RCC_SYSCLK_DIV2;
  RCC_ClkInitStruct.AHBCLK4Divider = RCC_SYSCLK_DIV1;
  if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    /* Initialization Error */
    while (1);
  }
}



/**
  * @brief  This function is executed in case of error occurrence
  * @param  None
  * @retval None
  */
static void Error_Handler(void)
{
  /* User may add here some code to deal with this error */
  /* Toggle LED3 @2Hz to notify error condition */
  while (1)
  {
    BSP_LED_Toggle(LED3);
    HAL_Delay(250);
  }
}

#ifdef USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {}
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
