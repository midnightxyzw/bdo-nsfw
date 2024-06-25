/*
 * Header file for the ICE encryption library.
 *
 * Written by Matthew Kwan - July 1996
 */

#ifndef _ICE_H
#define _ICE_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct ice_key_struct	ICE_KEY;
ICE_KEY	*ice_key_create (int n);
void	ice_key_destroy (ICE_KEY *ik);
void	ice_key_set (ICE_KEY *ik, const unsigned char *k);
void	ice_key_encrypt (const ICE_KEY *ik,
			const unsigned char *ptxt, unsigned char *ctxt);
void	ice_key_decrypt (const ICE_KEY *ik,
			const unsigned char *ctxt, unsigned char *ptxt);
int	ice_key_key_size (const ICE_KEY *ik);
int	ice_key_block_size (const ICE_KEY *ik);

#ifdef __cplusplus
}
#endif

#endif
