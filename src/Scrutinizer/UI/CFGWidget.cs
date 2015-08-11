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
            public Node( BasicBlock bl )
            {
                Block = bl;
            }
            public Node(Graph sub, Loop loop)
            {
                SubGraph = sub;
                m_Loop = loop;
            }

            public Loop m_Loop = null;
            public Graph SubGraph = null;
            public BasicBlock Block = null;
        };
        

        private class Graph
        {
            private Dictionary<Node, List<Node>> m_OutgoingEdges = new Dictionary<Node, List<Node>>();
            private Dictionary<Node, List<Node>> m_IncomingEdges = new Dictionary<Node, List<Node>>();
        
            public void AddNode(Node n)
            {
                m_OutgoingEdges.Add(n, new List<Node>());
                m_IncomingEdges.Add(n, new List<Node>());
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

            public void CombineNodes( IEnumerable<Node> loopNodes, Loop l )
            {
                Graph sub      = new Graph();
                Node superNode = new Node(sub,l);

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

            public IEnumerable<Node> Nodes { get { return m_OutgoingEdges.Keys;  } }

            public List<Node> Preorder()
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
            List<TreeNode> kids = new List<TreeNode>();
            
            List<Node> Nodes = g.Preorder();
            foreach (Node n in Nodes)
            {
                TreeNode t = new TreeNode();
                t.Tag = n;
                parent.Nodes.Add(t);

                if (n.SubGraph != null)
                {
                    t.Text = "loop";
                    BuildTree(t, n.SubGraph);
                }
                else
                {
                    if (n.Block.LastInstruction is IBranchInstruction)
                    {
                        IBranchInstruction branch = n.Block.LastInstruction as IBranchInstruction;
                        if (branch.Category == BranchCategory.BREAK_BRANCH)
                            t.Text = "break";
                        else
                            t.Text = "branch";
                    }
                    else
                    {
                        t.Text = "node";
                    }
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
                Node n = new Node(b);
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
                Node[] loopNodes = new Node[l.BlockCount];
                int n = 0;
                foreach (BasicBlock b in l.Blocks)
                    loopNodes[n++] = NodeMap[b];

                g.CombineNodes(loopNodes,l);
            }


            // now populate the tree view with loops and branches
            TreeNode root = new TreeNode("program");
            BuildTree(root, g);
            treeView1.Nodes.Add(root);
        }



        public delegate void BlockSelectionHandler(object sender, BasicBlock SelectedBlock);
        public delegate void LoopSelectionHandler(object sender, Loop SelectedLoop);
        public delegate void SelectionClearedHandler(object sender);

        public event BlockSelectionHandler BlockSelected;
        public event LoopSelectionHandler LoopSelected;
        public event SelectionClearedHandler SelectionCleared;


        public BasicBlock SelectedBlock { get; private set; }
        public Loop SelectedLoop { get; private set; }

        private void treeView1_AfterSelect(object sender, TreeViewEventArgs e)
        {
            if (treeView1.SelectedNode == null)
            {
                if (SelectionCleared != null)
                {
                    SelectedLoop  = null;
                    SelectedBlock = null;
                    SelectionCleared(this);
                }
            }

            object tag = treeView1.SelectedNode.Tag;
            if (tag == null)
            {
                if (SelectionCleared != null)
                {
                    SelectedLoop = null;
                    SelectedBlock = null;
                    SelectionCleared(this);
                }
            }
            else
            {
                Node n = tag as Node;
                if (n.m_Loop != null)
                {
                    if (LoopSelected != null)
                    {
                        SelectedLoop = n.m_Loop;
                        SelectedBlock = null;
                        LoopSelected(this, n.m_Loop);
                    }
                }
                else
                {
                    if (BlockSelected != null)
                    {
                        SelectedLoop = null;
                        SelectedBlock = null;
                        BlockSelected(this, n.Block);
                    }
                }
            }
        }

        private void treeView1_Leave(object sender, EventArgs e)
        {
            treeView1.SelectedNode = null;
            if (SelectionCleared != null)
            {
                SelectedLoop = null;
                SelectedBlock = null;
                SelectionCleared(this);
            }
        }
    }
}
