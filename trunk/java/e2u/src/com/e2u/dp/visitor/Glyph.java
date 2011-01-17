package com.e2u.dp.visitor;

public class Glyph
{
	public void accept(Visitor visitor)
	{
		visitor.visitGlyph(this);
	}
}
