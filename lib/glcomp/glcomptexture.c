/*************************************************************************
 * Copyright (c) 2011 AT&T Intellectual Property 
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors: Details at https://graphviz.org
 *************************************************************************/

#include <glcomp/glcomptexture.h>
#include <glcomp/glpangofont.h>

#include <common/memory.h>

#if 0
void glCompSetRemoveTexLabel(glCompSet * s, glCompFont * t)
{
}
#endif

static glCompTex *glCompSetAddNewTexture(glCompSet * s, int width,
					 int height, unsigned char *data,
					 int is2D,int fs)
{
    int Er, offset, ind;
    glCompTex *t;
    unsigned char *tarData;
    unsigned char *srcData;

    if (!data)
	return NULL;

    Er = 0;
    t = NEW(glCompTex);
    if (!is2D) {		/*use opengl texture */
	glEnable(GL_TEXTURE_2D);
	glShadeModel(GL_FLAT);
	glEnable(GL_DEPTH_TEST);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &t->id);	//get next id
	if (glGetError() != GL_NO_ERROR) {		/*for some opengl based error , texture couldnt be created */
	    /* drain the OpenGL error queue */
	    while (glGetError() != GL_NO_ERROR);
	    Er = 1;
	} else {
	    glBindTexture(GL_TEXTURE_2D, t->id);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
			    GL_NEAREST);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
			    GL_NEAREST);
	    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0,
			 GL_RGBA, GL_UNSIGNED_BYTE, data);
	    glDisable(GL_TEXTURE_2D);
	}
    }
    if (is2D && !Er) {
	t->data = N_NEW(4 * width * height, unsigned char);
	offset = 4;		//RGBA  mod,TO DO implement other modes 
	/*data upside down because of pango gl coord system */
	for (ind = 0; ind < height; ind++) {
	    srcData = data + (height - 1 - ind) * offset * width;
	    tarData = t->data + ind * offset * width;
	    memcpy(tarData, srcData, 4 * width);
	}
    }

    if (Er) {
	free(data);
	free(t);
	return NULL;
    }
    t->userCount = 1;
    t->width = (GLfloat) width;
    t->height = (GLfloat) height;
    if(s)
    {
	s->textureCount++;
	s->textures =
	realloc(s->textures, s->textureCount * sizeof(glCompTex *));
	s->textures[s->textureCount - 1] = t;
    }
    return t;


}

glCompTex *glCompSetAddNewTexImage(glCompSet * s, int width, int height,
				   unsigned char *data, int is2D)
{

    glCompTex *t;
    if (!data)
	return NULL;
    t = glCompSetAddNewTexture(s, width, height, data, is2D,-1);
    if (!t)
	return NULL;
    t->type = glTexImage;
    return t;

}




glCompTex *glCompSetAddNewTexLabel(glCompSet * s, char *def, int fs,
				   char *text, int is2D)
{
    int ind, Er, width, height;
//      int ind2=0;
    glCompTex *t;
    cairo_surface_t *surface = NULL;
    unsigned char *data;
    data = (unsigned char *) 0;
    Er = 0;
    if (!def)
	return NULL;
    /*first check if the same label with same font def created before
       if it was , return its id
     */
    for (ind = 0; ind < s->textureCount; ind++) {
	if (s->textures[ind]->type == glTexLabel) {
	    if ((strcmp(def, s->textures[ind]->def) == 0)
		&& (s->textures[ind]->type == glTexLabel)
		&& (strcmp(text, s->textures[ind]->text) == 0)
		&& (s->textures[ind]->fontSize==fs)) {
		s->textures[ind]->userCount++;
		return s->textures[ind];
	    }
	}
    }


    data = glCompCreatePangoTexture(def, fs, text, surface, &width, &height);
    if (!data)			/*pango error , */
	Er = 1;
    t = glCompSetAddNewTexture(s, width, height, data, is2D,fs);
    if (!t)
	Er = 1;
    cairo_surface_destroy(surface);

    if (Er) {
	free(data);
	free(t);
	return NULL;
    }

    t->def = strdup(def);
    t->text = strdup(text);
    t->type = glTexLabel;
    return t;
}

void glCompDeleteTexture(glCompTex * t)
{
    if (!t)
	return;
    t->userCount--;
    if (!t->userCount) {
	free(t->data);
	free(t->def);
	free(t->text);
	free(t);
    }
}
