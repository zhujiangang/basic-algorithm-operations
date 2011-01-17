package com.e2u.dp.visitor;

public class SpellingChecker implements Visitor
{
	public void visitGlyph(Glyph glyph)
	{
		System.out.println("Spelling Checker Visitor: " + glyph);
	}
	public void visitChar(GlyphChar gchar)
	{
		System.out.println("Spelling Checker Visitor: " + gchar);		
	}
	public void visitRow(GlyphRow grow)
	{
		System.out.println("Spelling Checker Visitor: " + grow);		
	}
}
