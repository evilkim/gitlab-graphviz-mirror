/*************************************************************************
 * Copyright (c) 2011 AT&T Intellectual Property 
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors: Details at https://graphviz.org
 *************************************************************************/

#pragma once

#include <common/types.h>
#include <string>

namespace Visio
{
	/* Para VDX element */
	
	class Para
	{
	public:
		enum HorzAlign
		{
			horzLeft = 0,
			horzCenter = 1,
			horzRight = 2,
			horzJustify = 3,
			horzForce = 4
		};
		
		Para(HorzAlign horzAlign);
		
		/* output the para details */
		void Print(GVJ_t* job) const;
		
	private:
		HorzAlign _horzAlign;
	};
	
	/* Char VDX element */
	
	class Char
	{
	public:
		Char(double size, unsigned char red, unsigned char green, unsigned char blue);
		
		/* output the char details */
		void Print(GVJ_t* job) const;
		
	private:
		double _size;
		unsigned char _red;
		unsigned char _green;
		unsigned char _blue;
	};
	
	/* para marker + char marker + text */
	
	class Run
	{
	public:
		Run(boxf bounds, const char* text);
		
		boxf GetBounds() const;		/* bounding box -- used by text logic */
		
		/* given the text index, output the run */
		void Print(GVJ_t* job, unsigned int index) const;
		
	private:
		boxf _bounds;
		std::string _text;
	};
	
	/* Para, Char and Run details for each Graphviz text */
	
	class Text
	{
	public:
		static Text CreateText(GVJ_t* job, pointf p, textspan_t* span);
		
		boxf GetBounds() const;
		
		void Print(GVJ_t* job) const;
		void PrintRun(GVJ_t* job, unsigned int index) const;
		
	private:
		Text(const Para &para, const Char &chars, const Run &run);
		
		Para _para;
		Char _chars;
		Run _run;
	};
	
	/* Hyperlink VDX element */
	
	class Hyperlink
	{
	public:
		Hyperlink(const char* description, const char* address, const char* frame);
		
		/* given the id, whether it is default, output the hyperlink */
		void Print(GVJ_t* job, unsigned int id, bool isDefault) const;
		
	private:
		std::string _description;
		std::string _address;
		std::string _frame;
	};
}
