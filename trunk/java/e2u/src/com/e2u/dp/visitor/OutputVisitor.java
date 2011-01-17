package com.e2u.dp.visitor;

public class OutputVisitor implements Visitor
{
	public void visitGlyph(Glyph glyph)
	{
		System.out.println("Output Visitor: " + glyph);
	}
	public void visitChar(GlyphChar gchar)
	{
		System.out.println("Output Visitor: " + gchar);		
	}
	public void visitRow(GlyphRow grow)
	{
		System.out.println("Output Visitor: " + grow);		
	}
}
