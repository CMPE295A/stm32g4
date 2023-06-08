/**
  @page ChaCha20-Poly1305_AEAD_EncryptDecrypt Cipher example

  @verbatim
  ******************************************************************************
  * @file    Applications/Cipher/ChaCha20-Poly1305_AEAD_EncryptDecrypt/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the ChaCha20-Poly1305_AEAD_EncryptDecrypt example.
  ******************************************************************************
  *
  * Copyright (c) 2018 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file in
  * the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
   @endverbatim

@par Example Description
This example describes how to use the STM32 Cryptographic Library to perform
authenticated encryption and decryption using the Chacha20-Poly1305 algorithm.

This example demonstrates two ways of using the cryptographic services:
1 - The single call method: algorithm configuration and execution is done via a single API call.
2 - The multiple calls method: algorithm configuration and execution is done in several API calls,
allowing in particular a piecemeal injection of data to process.

For each method, there are two examples provided:
a - An authenticated encryption of a known plaintext, followed by the verification of the generated ciphertext
b - An verified decryption of a known ciphertext, followed by the verification of the generated plaintext

In case of successful operations:
- the green led will be turned on
- the global variable glob_status will be set to PASSED
In case of failure:
- the green led will be toggled each 250 milliseconds in an infinity loop.
- the global variable glob_status will be set to FAILED

@note Vectors examples for Chacha20-Poly1305 are taken from:
"ChaCha20 and Poly1305 for IETF Protocols"
Available at:
 https://tools.ietf.org/html/rfc8439

@par Keywords

Cryptography, authentication, cipher, decipher, Chacha20, Poly1305, Cryptographic

@par Directory contents

  - Cipher/ChaCha20-Poly1305_AEAD_EncryptDecrypt/Inc/stm32g0xx_hal_conf.h    HAL configuration file
  - Cipher/ChaCha20-Poly1305_AEAD_EncryptDecrypt/Inc/stm32g0xx_it.h          Interrupt handlers header file
  - Cipher/ChaCha20-Poly1305_AEAD_EncryptDecrypt/Inc/main.h                        Header for main.c module
  - Cipher/ChaCha20-Poly1305_AEAD_EncryptDecrypt/Src/stm32g0xx_it.c          Interrupt handlers
  - Cipher/ChaCha20-Poly1305_AEAD_EncryptDecrypt/Src/stm32g0xx_hal_msp.c     HAL MSP module
  - Cipher/ChaCha20-Poly1305_AEAD_EncryptDecrypt/Src/main.c                        Main program
  - Cipher/ChaCha20-Poly1305_AEAD_EncryptDecrypt/Src/system_stm32g0xx.c      STM32G0xx system source file
  - Cipher/ChaCha20-Poly1305_AEAD_EncryptDecrypt/Src/cmox_low_level.c              CMOX low level services implementation

@par Hardware and Software environment

  - This example runs on STM32G0xx devices.

  - This example has been tested with NUCLEO-G071RB board and can be
    easily tailored to any other supported device and development board.

@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
