package com.e2u.dp.visitor;

public interface Visitor
{
	public void visitGlyph(Glyph glyph);
	public void visitChar(GlyphChar gchar);
	public void visitRow(GlyphRow grow);
}
