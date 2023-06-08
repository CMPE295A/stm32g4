---
pagetitle: X-Cube-CRYPTOLIB Projects
lang: en
header-includes: <link rel="icon" type="image/x-icon" href="../_htmresc/favicon.png" />
---

::: {.row}
::: {.col-sm-12}

# <mark>X-Cube-CRYPTOLIB</mark> Projects

The application samples are located under `STM32CubeExpansion_CRYPTOLIB_VX.Y.Z/Projects/Target`, and all of them have the same structure:

  - `/[CMx]/Inc` header files of the application.
  - `/[CMx]/Src` source code of the application.
  - `/EWARM`, `/MDK-ARM`, and `/STM32CubeIDE` pre-configured projects for each toolchain.
  - `readme.txt` describing the application behavior and the environment required to run the example.

Note: refer to section "Development Toolchains and Compilers" and "Supported Devices and boards" of the Firmware package [release notes []{.icon-link} ](../Release_Notes.html) to know about the SW/HW environment used for the Firmware development and validation.

----

The table below contains the list of the provided applications.


Type                  Project Name                             Description                                                                                                 NUCLEO-G071RB          NUCLEO-G474RE      NUCLEO-H563ZI         NUCLEO-H743ZI      NUCLEO-L073RZ      NUCLEO-L152RE      NUCLEO-L476RG      NUCLEO-L552ZE-Q    NUCLEO-U575ZI-Q       NUCLEO-WL55JC      P-NUCLEO-WB55.Nucleo    NUCLEO-WBA52CGB    
--------------------  ---------------------------------------  ---------------------------------------------------------------------------------------------------------   ---------------------  -----------------  -----------------     -----------------  -----------------  -----------------  -----------------  -----------------  --------------------  -----------------  ----------------------  -----------------  
Cipher                AES CBC EncryptDecrypt                   Encryption and decryption using the AES CBC algorithm                                                       Yes                    Yes                Yes []{.icon-st-add}  Yes                Yes                Yes                Yes                Yes                Yes                   Yes                Yes                     Yes []{.icon-st-add}  
Cipher                AES GCM AEAD EncryptDecrypt              Authenticated encryption and decryption using the AES GCM algorithm                                         Yes                    Yes                Yes []{.icon-st-add}  Yes                Yes                Yes                Yes                Yes                Yes                   Yes                Yes                     Yes []{.icon-st-add}  
Cipher                ChaCha20-Poly1305 AEAD EncryptDecrypt    Authenticated encryption and decryption using the Chacha20-Poly1305 algorithm                               Yes                    Yes                Yes []{.icon-st-add}  Yes                Yes                Yes                Yes                Yes                Yes                   Yes                Yes                     Yes []{.icon-st-add}  
Cipher                SM4 CTR EncryptDecrypt                   Encryption and decryption using the SM4 CTR algorithm                                                       Yes                    Yes                Yes []{.icon-st-add}  Yes                Yes                Yes                Yes                Yes                Yes                   Yes                Yes                     Yes []{.icon-st-add}  
DRBG                  Random Generation                        Generation of random numbers using the DRBG module                                                          Yes                    Yes                Yes []{.icon-st-add}  Yes                Yes                Yes                Yes                Yes                Yes                   Yes                Yes                     Yes []{.icon-st-add}  
ECC                   ECDH SharedSecretGeneration              Establishment of a shared secret using the ECDH algorithm over SECP256R1 curve                              Yes                    Yes                Yes []{.icon-st-add}  Yes                Yes                Yes                Yes                Yes                Yes                   Yes                Yes                     Yes []{.icon-st-add}  
ECC                   ECDSA SignVerify                         Signature and verification of a message using the ECDSA algorithm over SECP256R1 curve                      Yes                    Yes                Yes []{.icon-st-add}  Yes                Yes                Yes                Yes                Yes                Yes                   Yes                Yes                     Yes []{.icon-st-add}  
ECC                   EDDSA SignVerify                         Signature and verification of a message using the EdDSA algorithm over Ed25519 curve                        Yes                    Yes                Yes []{.icon-st-add}  Yes                Yes                Yes                Yes                Yes                Yes                   Yes                Yes                     Yes []{.icon-st-add}  
ECC                   SM2 SignVerify                           Signature and verification of a message using the SM2 algorithm                                             Yes                    Yes                Yes []{.icon-st-add}  Yes                Yes                Yes                Yes                Yes                Yes                   Yes                Yes                     Yes []{.icon-st-add}  
Hash                  SHA2 Digest                              Hash a message using the SHA256 algorithm                                                                   Yes                    Yes                Yes []{.icon-st-add}  Yes                Yes                Yes                Yes                Yes                Yes                   Yes                Yes                     Yes []{.icon-st-add}  
Hash                  SHA3 Digest                              Hash a message using the SHA3-512 algorithm                                                                 Yes                    Yes                Yes []{.icon-st-add}  Yes                Yes                Yes                Yes                Yes                Yes                   Yes                Yes                     Yes []{.icon-st-add}  
Hash                  SHAKE Digest                             Hash a message using the SHAKE algorithm                                                                    Yes                    Yes                Yes []{.icon-st-add}  Yes                Yes                Yes                Yes                Yes                Yes                   Yes                Yes                     Yes []{.icon-st-add}  
Hash                  SM3 Digest                               Hash a message using the SM3 algorithm                                                                      Yes                    Yes                Yes []{.icon-st-add}  Yes                Yes                Yes                Yes                Yes                Yes                   Yes                Yes                     Yes []{.icon-st-add}  
MAC                   AES CMAC_AuthenticateVerify              Generation and verification of a message authentication code of plaintext using the AES CMAC algorithm      Yes                    Yes                Yes []{.icon-st-add}  Yes                Yes                Yes                Yes                Yes                Yes                   Yes                Yes                     Yes []{.icon-st-add}  
MAC                   HMAC SHA2_AuthenticateVerify             Generation and verification of a message authentication code of plaintext using the HMAC-SHA256 algorithm   Yes                    Yes                Yes []{.icon-st-add}  Yes                Yes                Yes                Yes                Yes                Yes                   Yes                Yes                     Yes []{.icon-st-add}  
MAC                   KMAC AuthenticateVerify                  Generation and verification of a message authentication code of plaintext using the KMAC algorithm          Yes                    Yes                Yes []{.icon-st-add}  Yes                Yes                Yes                Yes                Yes                Yes                   Yes                Yes                     Yes []{.icon-st-add}  
RSA                   PKCS1v1.5 SignVerify                     Signature and verification of a message using the RSA PKCS#1 v1.5 compliant algorithm                       Yes                    Yes                Yes []{.icon-st-add}  Yes                Yes                Yes                Yes                Yes                Yes                   Yes                Yes                     Yes []{.icon-st-add}  
RSA                   PKCS1v2.2 EncryptDecrypt                 Encryption and decryption of a message using the RSA PKCS#1 v2.2 compliant algorithm                        Yes                    Yes                Yes []{.icon-st-add}  Yes                Yes                Yes                Yes                Yes                Yes                   Yes                Yes                     Yes []{.icon-st-add}  
RSA                   PKCS1v2.2 SignVerify                     Signature and verification of a message using the RSA PKCS#1 v2.2 compliant algorithm                       Yes                    Yes                Yes []{.icon-st-add}  Yes                Yes                Yes                Yes                Yes                Yes                   Yes                Yes                     Yes []{.icon-st-add}  


:::
:::

<footer class="sticky">
For complete documentation on <mark>STM32</mark> microcontrollers please visit <http://www.st.com/stm32>
</footer>
