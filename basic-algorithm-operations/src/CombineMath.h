#ifndef _COMBINE_MATH_H_
#define _COMBINE_MATH_H_


/************************************************************************/
/* P(n,n)                                                               */
/************************************************************************/
void perm(int in[], int begin, int end);


/************************************************************************/
/* P(n,m)                                                               */
/************************************************************************/
void perm(int in[], int n, int m, int out[], int depth, int used[]);


/************************************************************************/
/* collection. 2^n                                                      */
/************************************************************************/
void combine(int in[], int len, int inBegin, int out[], int outBegin);

/************************************************************************/
/* C(n, m) = C(n-1, m-1) + C(n-1, m)                                   */
/************************************************************************/
void combine1(int in[], int n, int m, int out[], int outLen, int depth);

/*************************************************************************/
/* C(n, m) = C(n-1, m-1) + C(n-2, m-1) + C(n-3, m-1) + .... + C(m-1, m-1)*/
/*************************************************************************/
void combine2(int in[], int n, int m, int out[], int outLen);

#endif