#include "ice.h"
#include <string.h> // memcpy

#ifdef _WIN32
#define ICE_API __declspec(dllexport)
#else
#define ICE_API
#endif

extern "C" ICE_API void decrypt_inplace(unsigned char *text, size_t length) {
  auto key = (const unsigned char
                  *)"\x51\xF3\x0F\x11\x04\x24\x6A\x00"; // this is the BDO
                                                        // encryption key
  auto ice = ice_key_create(0);
  ice_key_set(ice, key);

  // decrypt in 8-byte blocks
  unsigned char buffer[8];
  for (size_t i = 0; (i + 8) < length; i += 8) {
    ice_key_decrypt(ice, (unsigned char *)text + i, buffer);
    memcpy(text + i, buffer, 8);
  }

  ice_key_destroy(ice);
}
