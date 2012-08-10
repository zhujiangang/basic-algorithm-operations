package com.bao.sort;

import com.bao.sort.SortUtil.Sort;

public class TreeSort implements Sort
{

	@Override
	public void sort(int[] data)
	{		
		int[] ret = TreeSelectionSort(data);
		System.arraycopy(data, 0, ret, 0, data.length);
	}

	//http://baike.baidu.com/view/3108940.htm
	public static int[] TreeSelectionSort(int[] mData)
	{
		int TreeLong = mData.length * 4;
		int MinValue = -10000;
		int[] tree = new int[TreeLong]; // 树的大小
		int baseSize;
		int i;
		int n = mData.length;
		int max;
		int maxIndex;
		int treeSize;
		baseSize = 1;
		while(baseSize < n)
		{
			baseSize *= 2;
		}
		treeSize = baseSize * 2 - 1;
		for(i = 0; i < n; i++)
		{
			tree[treeSize - i] = mData[i];
		}
		for(; i < baseSize; i++)
		{
			tree[treeSize - i] = MinValue;
		}
		// 构造一棵树
		for(i = treeSize; i > 1; i -= 2)
		{
			tree[i / 2] = (tree[i] > tree[i - 1] ? tree[i] : tree[i - 1]);
		}
		n -= 1;
		while(n != -1)
		{
			max = tree[1];
			mData[n--] = max;
			maxIndex = treeSize;
			while(tree[maxIndex] != max)
			{
				maxIndex--;
			}
			tree[maxIndex] = MinValue;
			while(maxIndex > 1)
			{
				if(maxIndex % 2 == 0)
				{
					tree[maxIndex / 2] = (tree[maxIndex] > tree[maxIndex + 1] ? tree[maxIndex]
						: tree[maxIndex + 1]);
				}
				else
				{
					tree[maxIndex / 2] = (tree[maxIndex] > tree[maxIndex - 1] ? tree[maxIndex]
						: tree[maxIndex - 1]);
				}
				maxIndex /= 2;
			}
		}
		return mData;
	}
}
