package com.bao.lc.site.s3.bean;

import com.bao.lc.util.Filter;

public interface ComparableFilter<F, C> extends Filter<F>, Comparable<C>
{

}
