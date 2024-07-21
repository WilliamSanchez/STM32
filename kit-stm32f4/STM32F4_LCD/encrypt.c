#include <encrypt.h>

state_t matrix;

void Cipher(state_t* matrix, const uint8_t* RoundKey){

    uint8_t round = 0;

    // 0-->> anadir la clave paso 0
    AddRoundKey(0, matrix,RoundKey);

    for (round = 1; round < Nr; ++round){
        // 1 -->> round 1
   	// sustitucion de los bytes
  	SubBytes(matrix);
  	// corrimiento de los bytes
  	ShiftRows(matrix);
  	// Mezcla de las columnas
  	MixColumns(matrix);
   	// anadir la clave paso 1
  	AddRoundKey(round, matrix,RoundKey);
    }

    // 10 -->> round 10
    // sustitucion de los bytes
    SubBytes(matrix);
    // corrimiento de los bytes
    ShiftRows(matrix);
    // anadir la clave paso 1
    AddRoundKey(Nr, matrix,RoundKey);

}


void AES_CBC_encrypt_buffer(struct AES_ctx *ctx, uint8_t* buf, uint32_t length)
{
    uintptr_t i;
    uint8_t *Iv = ctx->Iv;
    for (i = 0; i < length; i += AES_BLOCKLEN)
    {
	XorWithIv(buf, Iv);
        array2array2D(buf,&matrix);
        Cipher(&matrix, ctx->RoundKey);
        array2D2array( &matrix, buf);
	Iv = buf;
        buf += AES_BLOCKLEN;
    }   
    memcpy(ctx->Iv, Iv, AES_BLOCKLEN);
       
}

