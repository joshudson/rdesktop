/*
 * Copyright (C) Joshua Hudson 2017
 */

#include <X11/Xlib.h>
#include <stdint.h>
#include <stdio.h>

#define energy_blue	1024
#define energy_green	768
#define energy_red	512
#define energy_scale_2n 10

extern int g_surge;

void SurgeAdjustImage(XImage *img)
{
	size_t buflen, avenergy, bpp, i, numerator;
	uint_fast64_t energy = 0;

	bpp = img->bitmap_pad / 8;
	if (bpp < 3) return ; /* Not yet implemented */
	buflen = img->width * img->height * bpp;

	for (i = 0; i < buflen; i += bpp) {
		energy += ((unsigned char)img->data[i + 2] * energy_red +
			(unsigned char)img->data[i + 1] * energy_green +
			(unsigned char)img->data[i] * energy_blue) >> energy_scale_2n;
		
	}
	avenergy = (size_t)(energy / (img->width * img->height * 3));
	if (avenergy < g_surge) return;

	numerator = (g_surge << 8) / avenergy;
	for (i = 0; i < buflen; i += 1) {
		img->data[i] = ((unsigned char)img->data[i] * numerator) >> 8;
	}
}
