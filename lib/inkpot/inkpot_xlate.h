/*************************************************************************
 * Copyright (c) 2011 AT&T Intellectual Property 
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors: Details at https://graphviz.org
 *************************************************************************/

#ifndef INKPOT_XLATE_H
#define INKPOT_XLATE_H

#ifdef __cplusplus
extern "C" {
#endif

extern void hsva2rgba(double hsva[4], double rgba[4]);
extern void rgba2hsva(double rgba[4], double hsva[4]);
extern void rgba2cmyk(double rgba[4], double cmyk[4]);

#ifdef __cplusplus
}
#endif
#endif /* INKPOT_XLATE_H */
