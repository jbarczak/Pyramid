using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Pyramid.Scrutinizer.UI
{
    /// <summary>
    ///  Control which is responsible for visualizing the control flow graph and
    ///    allowing users to select blocks
    ///    
    /// </summary>
    public partial class CFGWidget : UserControl
    {
        public CFGWidget()
        {
            InitializeComponent();
        }

        private class Node
        {
            public string NodeType = "node";
        };
        
        private class LoopNode : Node
        {
            public LoopNode(Graph sub, Loop loop) 
            {
                NodeType = "loop";
                SubGraph = sub;
                m_Loop = loop;
            }

            public Loop m_Loop = null;
            public Graph SubGraph = null;
        
        }

        private class LeafNode : Node
        {
            public LeafNode( BasicBlock bl )
            {
                Block = bl;
                NodeType = "node";
            }
            public BasicBlock Block = null;
        }

        private class BranchNode : Node
        {
            public Graph IfGraph;
            public Graph ElseGraph;
            public LeafNode Test;
            public List<Node> OwnedNodes;
            public BasicBlock IfTarget;
            public BasicBlock ElseTarget;

            public BranchNode( LeafNode test )
            {
                Test = test;
                IfGraph = new Graph();
                ElseGraph = new Graph();
                OwnedNodes = new List<Node>();
                OwnedNodes.Add(test);
                NodeType = "branch";

                List<BasicBlock> succs = new List<BasicBlock>(test.Block.Successors);
                IfTarget   = succs[0];
                ElseTarget = succs[1];
            }
        }

        private class SkipNode : Node
        {
            public Graph BranchGraph;
            public LeafNode Test;

            public SkipNode(LeafNode test, Graph gr)
            {
                Test = test;
                BranchGraph = gr;
                NodeType = "skip";
            }
        }

        private class Graph
        {
            private Dictionary<Node, List<Node>> m_OutgoingEdges = new Dictionary<Node, List<Node>>();
            private Dictionary<Node, List<Node>> m_IncomingEdges = new Dictionary<Node, List<Node>>();
        
            public void AddNode(Node n)
            {
                m_OutgoingEdges.Add(n, new List<Node>());
                m_IncomingEdges.Add(n, new List<Node>());
            }

            public void AddNodes(IEnumerable<Node> nodes)
            {
                foreach (Node n in nodes)
                {
                    m_OutgoingEdges.Add(n, new List<Node>());
                    m_IncomingEdges.Add(n, new List<Node>());
                }
            }

            public void RemoveNodes(IEnumerable<Node> nodes)
            {
                foreach (Node n in nodes)
                {
                    m_OutgoingEdges.Remove(n);
                    m_IncomingEdges.Remove(n);
                }
            }

            public bool ContainsNode( Node n )
            {
                return m_OutgoingEdges.ContainsKey(n);
            }

            public void AddEdge( Node from, Node to )
            {
                m_OutgoingEdges[from].Add(to);
                m_IncomingEdges[to].Add(from);
            }

            public Dictionary<Node,Node> Dominators( List<Node> nodes )
            {
                Node[] objs = nodes.ToArray();
                Node[][] preds = new Node[objs.Length][];
                for (int i = 0; i < objs.Length; i++)
                    preds[i] = m_IncomingEdges[objs[i]].ToArray();

                return Algorithms.FindDominators(objs, preds);
            }

            public Dictionary<Node, Node> PostDominators(List<Node> nodes)
            {
                Node[] objs = nodes.ToArray();
                Node[][] preds = new Node[objs.Length][];
                for (int i = 0; i < objs.Length; i++)
                    preds[i] = m_OutgoingEdges[objs[i]].ToArray();

                return Algorithms.FindDominators(objs, preds);
            }

            public void TransferEdgesToSubgraph( Graph g )
            {
                foreach( Node n in g.Nodes )
                {
                    foreach (Node e in m_OutgoingEdges[n])
                        if( g.ContainsNode(e))
                            g.AddEdge(n, e);
                }
            }

            public void CombineNodes( IEnumerable<Node> loopNodes, Node superNode, Graph sub )
            {
                // transfer nodes to subgraph
                foreach (Node b in loopNodes)
                    sub.AddNode(b);
                
                // transfer edges to subgraph 
                foreach( Node n in loopNodes )
                    foreach( Node e in m_OutgoingEdges[n] )
                        if (sub.ContainsNode(e))
                            sub.AddEdge(n, e);

                
                // add super-node to graph
                AddNode(superNode);

                // find edges from collapsing nodes to non-collapsing one
                // replace these with corresponding edges on the super-node
                foreach( Node n in loopNodes )
                {
                    foreach( Node s in m_OutgoingEdges[n])
                    {
                        if (!sub.ContainsNode(s))
                        {
                            m_IncomingEdges[s].Remove(n);
                            AddEdge(superNode, s);
                        }
                    }
                }
   
                // remove the merged nodes from our graph
                foreach (Node n in loopNodes)
                {
                    m_IncomingEdges.Remove(n);
                    m_OutgoingEdges.Remove(n);
                }


                // find all nodes with an edge to a node that was just removed
                //  re-point these edges at the super-node
                foreach( KeyValuePair<Node,List<Node>> p in m_OutgoingEdges )
                {
                    List<Node> children = p.Value;
                    for (int i = 0; i < children.Count; i++)
                    {
                        if (!m_OutgoingEdges.ContainsKey(children[i]))
                        {
                            children[i] = superNode;
                            m_IncomingEdges[superNode].Add(p.Key);
                        }
                    }
                }
            }

            public IEnumerable<Node> ChildrenOf( Node n )
            {
                return m_OutgoingEdges[n];
            }

            public IEnumerable<Node> Nodes { get { return m_OutgoingEdges.Keys;  } }

            public List<Node> ReversePostOrder()
            {
                List<Node> OrderedNodes     = new List<Node>();
                HashSet<Node> AddedNodes = new HashSet<Node>();

                // collect source nodes
                List<Node> Frontier = new List<Node>();
                foreach (KeyValuePair<Node,List<Node>> p in m_IncomingEdges)
                    if( p.Value.Count == 0 )
                        Frontier.Add(p.Key);

                do
                {
                    // add frontier nodes to ordered list
                    OrderedNodes.AddRange(Frontier);
                    foreach( Node n in Frontier )
                        AddedNodes.Add(n);
                    
                    // search for new frontier nodes and add them to frontier list
                    //  a node goes on the frontier if it is not added but all 
                    //  its predecessors are
                    Frontier.Clear();
                    foreach( KeyValuePair<Node,List<Node>> p in m_IncomingEdges )
                    {
                        if (AddedNodes.Contains(p.Key))
                            continue;

                        int i = 0;
                        while( i < p.Value.Count )
                        {
                            if (!AddedNodes.Contains(p.Value[i]))
                                break;
                            i++;
                        }

                        // node is ready to go into the frontier once all its predecessors have been ordered
                        if (i == p.Value.Count )
                            Frontier.Add(p.Key);
                    }

                } while( Frontier.Count > 0 );

                return OrderedNodes;
            }
        };




        private void BuildTree( TreeNode parent, Graph g)
        {
            List<Node> Nodes = g.ReversePostOrder();

            for( int i=0; i<Nodes.Count; i++ )
            {
                Node n = Nodes[i];
                TreeNode t = new TreeNode();
                t.Tag = n;
                t.Text = n.NodeType;
                parent.Nodes.Add(t);

                if (n is LoopNode )
                {
                    BuildTree(t, (n as LoopNode).SubGraph);
                }
                else if( n is SkipNode )
                {
                    BuildTree(t, (n as SkipNode).BranchGraph);
                }
                else if( n is BranchNode )
                {
                    BranchNode br = n as BranchNode;
                    TreeNode nIf = new TreeNode();
                    TreeNode nElse = new TreeNode();
                    nIf.Text = "IF";
                    nElse.Text = "ELSE";
                    nIf.Tag   = br.IfTarget;
                    nElse.Tag = br.ElseTarget;
                    t.Nodes.Add(nIf);
                    t.Nodes.Add(nElse);

                    BuildTree(nIf, br.IfGraph);
                    BuildTree(nElse, br.ElseGraph);
                }
            }

        }

       

        private void BuildBranchNodes(Graph g)
        {
            // lay out the dag nodes in order
            List<Node> Nodes = g.ReversePostOrder();

            // Figure out dominance
            Dictionary<Node, Node> IDOM = g.Dominators(Nodes);
           
            for( int i=0; i<Nodes.Count; i++ )
            {
                Node n = Nodes[i];
                if (n is LoopNode)
                {
                    // descend into loop nodes
                    BuildBranchNodes((n as LoopNode).SubGraph);
                }
                else
                {
                    LeafNode leaf = n as LeafNode;
                    BasicBlock bl = leaf.Block;

                    // mark break, branch, and continue nodes as such
                    if (bl.LastInstruction is IBranchInstruction)
                    {
                        IBranchInstruction branch = bl.LastInstruction as IBranchInstruction;
                        if (branch.Category == BranchCategory.BREAK_BRANCH)
                            leaf.NodeType = "break";
                        else if (branch.Category == BranchCategory.CONTINUE_BRANCH)
                            leaf.NodeType = "continue";
                        else if (branch.Category == BranchCategory.LOOPSKIP_BRANCH)
                            leaf.NodeType = "node";
                        else if (branch.Category == BranchCategory.SKIP_BRANCH)
                            leaf.NodeType = "skip";
                        else
                            leaf.NodeType = "branch";
                    }
                    else
                    {
                        // mark continue nodes as such
                        if (bl.InnerMostLoop != null && bl.Successors.First() == bl.InnerMostLoop.Header)
                            n.NodeType = "continue";
                    }
                }
            }


            int k = 0;
            while( k < Nodes.Count )
            {
                if( Nodes[k] is LeafNode && Nodes[k].NodeType.Equals("branch") )
                {
                    BranchNode br = new BranchNode((Nodes[k] as LeafNode));

                    List<Node> descendents = new List<Node>(g.ChildrenOf(Nodes[k]));
                    Graph[] branchGraphs = new Graph[2];
                    branchGraphs[0] = br.IfGraph;
                    branchGraphs[1] = br.ElseGraph;
                   
                    for (int k0 = k + 1; k0 < Nodes.Count; k0++)
                    {
                        Node n = Nodes[k0];
                        for (int j = 0; j < descendents.Count; j++)
                        {
                            if (Algorithms.Dominates(descendents[j], n, IDOM))
                                branchGraphs[j].AddNode(n);
                        }
                    }

                    br.OwnedNodes.AddRange(branchGraphs[0].Nodes);
                    br.OwnedNodes.AddRange(branchGraphs[1].Nodes);

                    Graph branchGraph = new Graph();
                    g.CombineNodes(br.OwnedNodes, br, branchGraph);
                    branchGraph.TransferEdgesToSubgraph(branchGraphs[0]);
                    branchGraph.TransferEdgesToSubgraph(branchGraphs[1]);
                    
                    // do this recursively on the if/else branches
                    BuildBranchNodes(br.IfGraph);
                    BuildBranchNodes(br.ElseGraph);

                    // start over
                    k = 0;
                    Nodes = g.ReversePostOrder();
                }
                else if (Nodes[k] is LeafNode && Nodes[k].NodeType.Equals("skip"))
                {
                    Dictionary<Node, Node> PDOM = g.PostDominators(Nodes);
                    
                    // find nodes to put into the sub-graph
                    // these are all the nodes which are skipped
                    //  nodes are skipped if they are post-dominated by the convergence node (which post-dominates the test)
                    Node joinPoint = PDOM[Nodes[k]];
                    Graph branchGraph = new Graph();
                    for (int k0 = k + 1; Nodes[k0] != joinPoint; k0++)
                    {
                        Node n = Nodes[k0];
                        if (Algorithms.Dominates(joinPoint, Nodes[k0], PDOM))
                            branchGraph.AddNode(Nodes[k0]);
                    }

                    SkipNode sk = new SkipNode((Nodes[k] as LeafNode), branchGraph);

                    // now make a graph containing both test node and all skipped nodes
                    //  combine these into one skip node
                    List<Node> ownedNodes = new List<Node>(branchGraph.Nodes);
                    ownedNodes.Add(Nodes[k]);
                    Graph tmpGraph = new Graph();
                    g.CombineNodes(ownedNodes, sk, tmpGraph);
                    tmpGraph.TransferEdgesToSubgraph(branchGraph);

                    // do this recursively on the skipped nodes
                    BuildBranchNodes(sk.BranchGraph);

                    // start over
                    k = 0;
                    Nodes = g.ReversePostOrder();
                }
                else
                {
                    k++;
                }
            }
        }


        public void SetProgram(List<Loop> loops, List<BasicBlock> blocks)
        {
            // build DAG of basic blocks
            //  excluding backedges
            Graph g = new Graph();
            Dictionary<BasicBlock, Node> NodeMap = new Dictionary<BasicBlock, Node>();
            foreach (BasicBlock b in blocks)
            { 
                Node n = new LeafNode(b);
                g.AddNode(n);
                NodeMap.Add(b,n);
            }

            foreach (BasicBlock b in blocks)
                foreach (BasicBlock s in b.Successors)
                    if (!s.Dominates(b))
                        g.AddEdge(NodeMap[b], NodeMap[s]);

            
            // walk over loops inner to outer
            //  for each loop, replace its contents with a single "super-node"
            foreach( Loop l in loops )
            {
                HashSet<Node> loopNodes = new HashSet<Node>();
                foreach (BasicBlock b in l.Blocks )
                    loopNodes.Add(NodeMap[b]);

                LoopNode superNode = new LoopNode(new Graph(), l);

                g.CombineNodes(loopNodes,superNode,superNode.SubGraph );

                // all blocks in the loop should now be represented by the super-node
                foreach (BasicBlock b in l.Blocks)
                    NodeMap[b] = superNode;
            }


            BuildBranchNodes(g);


            // now populate the tree view with loops and branches
            TreeNode root = new TreeNode("program");
            BuildTree(root, g);
            treeView1.Nodes.Add(root);
        }



        public delegate void SelectionClearedHandler(object sender);
        public delegate void BlockSelectedHandler(object sender, BasicBlock bl );
        public delegate void LoopSelectionHandler(object sender, Loop SelectedLoop);
        public delegate void BranchSelectedHandler(object sender, BasicBlock BranchBlock, IBranchInstruction branch);
        public delegate void BranchTargetSelectedHandler(object sender, BasicBlock TargetBlock );

        public event BlockSelectedHandler           BlockSelected;
        public event LoopSelectionHandler           LoopSelected;
        public event BranchSelectedHandler          BranchSelected;
        public event BranchTargetSelectedHandler    BranchTargetSelected;
        public event SelectionClearedHandler        SelectionCleared;

        public Loop SelectedLoop { get; private set;  }
        public IBranchInstruction SelectedBranch { get; private set; }
        public BasicBlock SelectedBlock { get; private set; }

        private void treeView1_AfterSelect(object sender, TreeViewEventArgs e)
        {
            this.SelectedLoop   = null;
            this.SelectedBranch = null;
            this.SelectedBlock  = null;

            if (treeView1.SelectedNode == null)
            {
                if (SelectionCleared != null)
                {
                    SelectionCleared(this);
                    return;
                }
            }

            object tag = treeView1.SelectedNode.Tag;
            if (tag == null)
            {
                if (SelectionCleared != null)
                {
                    SelectionCleared(this);
                    return;
                }
            }
           
            Node n = tag as Node;
            if ( n is LoopNode )
            {
                Loop l = (n as LoopNode).m_Loop;
                SelectedLoop = l;
                    
                if (LoopSelected != null)
                    LoopSelected(this, l);
            }
            else if( n is BranchNode )
            {
                BranchNode br = n as BranchNode;
                BasicBlock bl = (br.Test as LeafNode).Block;
                SelectedBranch = bl.LastInstruction as IBranchInstruction;
                SelectedBlock  = bl;

                if( BranchSelected != null )
                    BranchSelected(this, bl, SelectedBranch);
            }
            else if( n is SkipNode )
            {
                SkipNode sk = n as SkipNode;
                BasicBlock bl = (sk.Test as LeafNode).Block;
                SelectedBranch = bl.LastInstruction as IBranchInstruction;
                SelectedBlock = bl;

                if (BranchSelected != null)
                    BranchSelected(this, bl, SelectedBranch);
            }
            else if( n is LeafNode )
            {
                SelectedBlock = (n as LeafNode).Block;
                if( BlockSelected != null )
                    BlockSelected(this, SelectedBlock);
            }
            else if (tag is BasicBlock)
            {
                SelectedBlock = tag as BasicBlock;
                if (BranchTargetSelected != null )
                    BranchTargetSelected(this, SelectedBlock);

            }
            
        }

        private void treeView1_Leave(object sender, EventArgs e)
        {
            treeView1.SelectedNode = null;
 
        }
    }
}
