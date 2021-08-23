/*************************************************************************
 * Copyright (c) 2011 AT&T Intellectual Property 
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors: Details at https://graphviz.org
 *************************************************************************/

#include <sparse/general.h>
#include <sparse/colorutil.h>
#include <stdio.h>

static int r2i(float r){
  /* convert a number in [0,1] to 0 to 255 */
  return (int)(255*r+0.5);
}

void rgb2hex(float r, float g, float b, char *cstring, const char *opacity) {
  sprintf(cstring, "#%02x%02x%02x", r2i(r), r2i(g), r2i(b));
  //set to semitransparent for multiple sets vis
  if (opacity && strlen(opacity) >= 2){
    cstring[7] = opacity[0];
    cstring[8] = opacity[1];
    cstring[9]='\0';
  } else {
    cstring[7] = '\0';
  }
}

static real Hue2RGB(real v1, real v2, real H) {
  if(H < 0.0) H += 1.0;
  if(H > 1.0) H -= 1.0;
  if((6.0*H) < 1.0) return (v1 + (v2 - v1) * 6.0 * H);
  if((2.0*H) < 1.0) return v2;
  if((3.0*H) < 2.0) return (v1 + (v2 - v1) * ((2.0/3.0) - H) * 6.0);
  return v1;
}

char * hue2rgb(real hue, char *color){
  real v1, v2, lightness = .5, saturation = 1;
  int red, blue, green;

  if(lightness < 0.5) 
    v2 = lightness * (1.0 + saturation);
  else
    v2 = (lightness + saturation) - (saturation * lightness);

  v1 = 2.0 * lightness - v2;

  red =   (int)(255.0 * Hue2RGB(v1, v2, hue + (1.0/3.0)) + 0.5);
  green = (int)(255.0 * Hue2RGB(v1, v2, hue) + 0.5);
  blue =  (int)(255.0 * Hue2RGB(v1, v2, hue - (1.0/3.0)) + 0.5);
  sprintf(color, "#%02x%02x%02x", red, green, blue);
  return color;
}
