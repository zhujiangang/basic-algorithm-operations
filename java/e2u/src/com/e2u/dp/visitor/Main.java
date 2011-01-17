package com.e2u.dp.visitor;

import java.util.*;

public class Main
{
	public static void main(String[] args)
	{
		List<Glyph> glist = new ArrayList<Glyph>();
		
		glist.add(new Glyph());
		glist.add(new GlyphChar());
		glist.add(new GlyphRow());
		glist.add(new GlyphChar());
		glist.add(new Glyph());
		
		Visitor visitor1 = new SpellingChecker();
		Visitor visitor2 = new OutputVisitor();
		
		Glyph glyph = null;
		for(Iterator<Glyph> iter = glist.iterator(); iter.hasNext(); )
		{
			glyph = iter.next();
			glyph.accept(visitor1);
			glyph.accept(visitor2);
		}
	}
}
