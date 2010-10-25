package com.e2u.tree;

import java.util.Stack;  
import com.e2u.util.ArrayUtil;  
import com.e2u.util.OutputUtil;  
public class BinTree {  
    private BinNode root = null;  
    public BinTree()  
    {  
    }  
    public BinTree(int data[])  
    {  
        if(data == null || data.length == 0)  
        {  
            root = null;  
            return;  
        }  
        root = buildInOrder(data, 0, data.length - 1);  
    }  
      
    private BinNode buildInOrder(int data[], int start, int end)  
    {  
        if(start > end)  
        {  
            return null;  
        }  
        int mid = (start + end) >> 1;  
          
        BinNode lc = buildInOrder(data, start, mid - 1);  
        BinNode rc = buildInOrder(data, mid + 1, end);  
        return new BinNode(data[mid], lc, rc);  
    }  
    public BinNode getRoot() {  
        return root;  
    }  
    public void setRoot(BinNode root) {  
        this.root = root;  
    }  
    public void insert(BinNode parent, int value, boolean isLeft)  
    {  
        if(parent == null)  
        {  
            root = new BinNode(value);  
            return;  
        }  
        if(isLeft)  
        {  
            parent.lc = new BinNode(value);  
        }  
        else  
        {  
            parent.rc = new BinNode(value);  
        }  
    }  
      
    public BinNode insert(int value)  
    {  
        if(root == null)  
        {  
            root = new BinNode(value);  
            return root;  
        }  
          
        BinNode t = root;  
        BinNode parent = null;  
          
        do  
        {  
            parent = t;  
            if(value <= t.value)  
            {  
                t = t.lc;  
            }  
            else  
            {  
                t = t.rc;  
            }  
        }while(t != null);  
          
        BinNode insertNode = new BinNode(value);  
        if(value <= parent.value)  
        {  
            parent.lc = insertNode;  
        }  
        else  
        {  
            parent.rc = insertNode;  
        }  
        return insertNode;  
    }  
      
    public void delete(int value)  
    {  
        //Does this tree has nodes?  
        if(root == null)  
        {  
            return;  
        }  
          
        BinNode parent = null;  
        BinNode t = root;  
          
        //Find the node with the value that same to the parameter "value"  
        while(t != null)  
        {  
            if(value == t.value)  
            {  
                break;  
            }  
            else if(value < t.value)  
            {  
                parent = t;  
                t = t.lc;  
            }  
            else  
            {  
                parent = t;  
                t = t.rc;  
            }  
        }  
          
        //There's no node with parameter "value"  
        if(t == null)  
        {  
            return;  
        }  
          
        //We find the node with value of input parameter "value"  
          
        //(1) Replace its value with biggest left child node's value, then delete that node  
          
        //(2) If there's no any left children of this node (t), make the parent of this node   
        //point to its right child directly  
          
        //Biggest left child  
        BinNode biggestLc = t.lc;  
          
        //No left child  
        if(biggestLc == null)  
        {  
            //t is the root node  
            if(parent == null)  
            {  
                root = t.rc;  
            }  
            else  
            {  
                if(t == parent.lc)  
                {  
                    parent.lc = t.rc;  
                }  
                else  
                {  
                    parent.rc = t.rc;  
                }  
            }  
            return;  
        }  
          
        //The parent of biggest left child node  
        BinNode parentOfBiggestLc = t;  
          
        //The biggest left child should be the rightmost node of t.lc  
        while(biggestLc.rc != null)  
        {  
            parentOfBiggestLc = biggestLc;  
            biggestLc = biggestLc.rc;  
        }  
        //Find the biggest left child, replace the value of node t with its value, then delete this node  
        t.value = biggestLc.value;  
          
        //The left child of t has no left child  
        if(t.lc == biggestLc)  
        {  
            t.lc = biggestLc.lc;  
        }  
        else  
        {  
            parentOfBiggestLc.rc = biggestLc.lc;  
        }  
    }  
    public void preOrder()  
    {  
        preOrder(new OutputVisitor());  
        System.out.println();  
    }  
    public void preOrder(Visitor visitor)  
    {  
        if(root != null)  
        {  
            preOrderHelper(root, visitor);  
        }  
    }  
    private void preOrderHelper(BinNode t, Visitor visitor)  
    {  
        visitor.visit(t);  
        if(t.lc != null)  
        {  
            preOrderHelper(t.lc, visitor);  
        }  
        if(t.rc != null)  
        {  
            preOrderHelper(t.rc, visitor);  
        }  
    }  
    public void preOrderStack()  
    {  
        preOrderStack(new OutputVisitor());  
        System.out.println();  
    }  
    public void preOrderStack(Visitor visitor)  
    {  
        BinNode t = root;  
        Stack<BinNode> s = new Stack<BinNode>();  
          
        while(t != null || !s.empty())  
        {  
            while(t != null)  
            {  
                visitor.visit(t);  
                  
                s.push(t);  
                t = t.lc;  
            }  
            t = s.pop().rc;  
        }  
    }  
    public void inOrder()  
    {  
        inOrder(new OutputVisitor());  
        System.out.println();  
    }  
    public void inOrder(Visitor visitor)  
    {  
        if(root != null)  
        {  
            inOrderHelper(root, visitor);  
        }  
    }  
    private void inOrderHelper(BinNode t, Visitor visitor)  
    {      
        if(t.lc != null)  
        {  
            inOrderHelper(t.lc, visitor);  
        }  
        visitor.visit(t);  
        if(t.rc != null)  
        {  
            inOrderHelper(t.rc, visitor);  
        }  
    }  
      
    public void inOrderStack()  
    {  
        inOrderStack(new OutputVisitor());  
        System.out.println();  
    }  
    public void inOrderStack(Visitor visitor)  
    {  
        BinNode t = root;  
        Stack<BinNode> s = new Stack<BinNode>();  
          
        while(t != null || !s.empty())  
        {  
            while(t != null)  
            {  
                s.push(t);  
                t = t.lc;  
            }  
            t = s.pop();  
            visitor.visit(t);  
            t = t.rc;  
        }  
    }  
    public void postOrder()  
    {  
        postOrder(new OutputVisitor());  
        System.out.println();  
    }  
    public void postOrder(Visitor visitor)  
    {  
        if(root != null)  
        {  
            postOrderHelper(root, visitor);  
        }  
    }  
    private void postOrderHelper(BinNode t, Visitor visitor)  
    {          
        if(t.lc != null)  
        {  
            postOrderHelper(t.lc, visitor);  
        }          
        if(t.rc != null)  
        {  
            postOrderHelper(t.rc, visitor);  
        }  
        visitor.visit(t);  
    }  
    public void postOrderStack()  
    {  
        postOrderStack(new OutputVisitor());  
        System.out.println();  
    }  
    public void postOrderStack(Visitor visitor)  
    {  
        BinNode t = root;  
        Stack<BinNode> s = new Stack<BinNode>();  
          
        BinNode p = null;  
        while(t != null || !s.empty())  
        {  
            while(t != null)  
            {  
                s.push(t);  
                t = t.lc;  
            }  
              
            p = null;  
            t = s.peek();  
              
            while(p == t.rc)  
            {  
                t = s.pop();  
                visitor.visit(t);              
                  
                p = t;                  
                if(s.isEmpty())  
                {  
                    return;  
                }  
                t = s.peek();  
            }  
            t = t.rc;  
        }  
    }  
    
    public void findPath(BinNode t, int sum, Stack st, boolean flag)
    {
    	if(t == null)
    	{
    		return;
    	}
    	st.push(t);
    	if(t.value == sum)
    	{
    		System.out.println(st);
    	}
    	findPath(t.lc, sum - t.value, st, true);
//    	System.out.println("before rc, t = " + t.value + ", st = " + st);
    	findPath(t.rc, sum - t.value, st, true);
//    	System.out.println("after rc, t = " + t.value + ", st = " + st);
    	st.pop();
    	
    	if(!flag)
    	{
        	findPath(t.lc, sum, st, false);
        	findPath(t.rc, sum, st, false);   		
    	}
    }
      
    public static class BinNode  
    {  
        public int value = Integer.MIN_VALUE;  
        public BinNode lc = null;  
        public BinNode rc = null;  
          
        public BinNode(int value, BinNode left, BinNode right)  
        {  
            this.value = value;  
            this.lc = left;  
            this.rc = right;  
        }  
        public BinNode(int value)  
        {  
            this(value, null, null);  
        }  
        public String toString()
        {
        	return String.valueOf(value);
        }
    }  
      
    public static interface Visitor  
    {  
        public void visit(BinNode node);  
    }  
      
    public static class OutputVisitor implements Visitor  
    {  
        public void visit(BinNode node)  
        {  
            System.out.print(node.value + " ");  
            System.out.flush();  
        }  
    }  
      
      
    public static BinTree buildTree(int data[])  
    {  
        BinTree binTree = new BinTree();  
        for(int i = 0; i < data.length; i++)  
        {  
            binTree.insert(data[i]);  
        }  
        return binTree;  
    }  
    public static void test1()  
    {  
//        int[] data = ArrayUtil.generate(20, 101);  
//        int[] data = ArrayUtil.generateInSeq(4, 1); 
    	int[] data = {4, 3, 5, 2, 1};
//        Arrays.sort(data);  
        ArrayUtil.output(data);  
        OutputUtil.outputLineSeparator();  
          
        BinTree binTree = new BinTree(data);  
//        BinTree binTree = buildTree(data);  
        binTree.preOrder();  
        binTree.preOrderStack();  
        OutputUtil.outputLineSeparator();  
          
        binTree.inOrder();  
        binTree.inOrderStack();  
        OutputUtil.outputLineSeparator();  
          
        binTree.postOrder();  
        binTree.postOrderStack();  
        OutputUtil.outputLineSeparator();  
          
//        testDelete(binTree, data[5]);  
//        testDelete(binTree, data[6]);  
//        testDelete(binTree, data[7]);  
//        testDelete(binTree, 15); 
        
        binTree.findPath(binTree.getRoot(), 7, new Stack(), false);
    }  
    private static void testDelete(BinTree binTree, int value)  
    {  
        binTree.delete(value);  
        binTree.preOrder();  
        binTree.inOrder();  
        OutputUtil.outputLineSeparator();  
    }  
    public static void main(String[] args) {  
        test1();  
    }  
}  
