package com.e2u.dp.visitor;

public class GlyphChar extends Glyph
{
	public void accept(Visitor visitor)
	{
		visitor.visitChar(this);
	}
}
