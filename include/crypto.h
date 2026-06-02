#ifndef CRYPTO_H
#define CRYPTO_H

void crypto_caesar_cipher(char *text, int shift);
void crypto_send_encrypted(int fd, const char *msg);

#endif
