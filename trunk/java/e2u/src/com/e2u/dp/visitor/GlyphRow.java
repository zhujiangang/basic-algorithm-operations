package com.e2u.dp.visitor;

public class GlyphRow extends Glyph
{
	public void accept(Visitor visitor)
	{
		visitor.visitRow(this);
	}
}
