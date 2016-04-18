using System;
using System.Collections.Generic;
using System.Linq;

namespace Pyramid.Scrutinizer
{

    public static class Algorithms
    {
        public static bool IsCFGReducible( List<BasicBlock> Blocks )
        {
            BasicBlock[] blocks = Blocks.ToArray();
            HashSet<BasicBlock>[] Successors   = new HashSet<BasicBlock>[blocks.Length];
            HashSet<BasicBlock>[] Predecessors = new HashSet<BasicBlock>[blocks.Length];
            Dictionary<BasicBlock, int> Index  = new Dictionary<BasicBlock, int>();

            // For reference: http://www.cs.colostate.edu/~mstrout/CS553Fall06/slides/lecture13-control.pdf
            // A CFG is reducible if it can be reduced to a single node via T1/T2 transforms
            //  
            int nBlocks = 0;
            foreach (BasicBlock b in blocks)
            {
                Successors[nBlocks] = new HashSet<BasicBlock>(b.Successors);
                Predecessors[nBlocks] = new HashSet<BasicBlock>(b.Predecessors);
                Index.Add(b, nBlocks);
                nBlocks++;
            }

            while (nBlocks > 1)
            {
                // T1 transform: delete all self-loops
                for (int i = 0; i < nBlocks; i++)
                {
                    BasicBlock bi = blocks[i];
                    Successors[i].Remove(blocks[i]);
                    Predecessors[i].Remove(blocks[i]);
                }

                // find nodes having only one predecessor and merge them into their predecessor
                int n = nBlocks;
                nBlocks = 0;
                for (int i = 0; i < n; i++)
                {
                    if (Predecessors[i].Count == 1)
                    {
                        // merge this node with its single predecessor
                        BasicBlock victim = blocks[i];
                        BasicBlock replacement = Predecessors[i].ElementAt(0);

                        // link successor blocks to replacement
                        foreach (BasicBlock s in Successors[i])
                        {
                            var preds = Predecessors[Index[s]];
                            preds.Remove(victim);
                            preds.Add(replacement);
                        }

                        // link replacement to successors
                        var replacement_successors = Successors[Index[replacement]];
                        replacement_successors.Remove(victim);
                        replacement_successors.UnionWith(Successors[i]);
                    }
                    else
                    {
                        // compact the array
                        blocks[nBlocks++] = blocks[i];
                    }
                }

                // bail out if we failed to find a reducible node
                if (n == nBlocks)
                    return false;
            }

            return true;
        }


        public static List<BasicBlock> BuildBasicBlocks(List<IInstruction> ops)
        {
            // collect set of branch targets
            HashSet<IInstruction> BranchTargets = new HashSet<IInstruction>();
            foreach( IInstruction op in ops )
            {
                if( op is IJumpInstruction )
                {
                    IJumpInstruction it = op as IJumpInstruction;
                    BranchTargets.Add( it.Target );
                }
                else if( op is IBranchInstruction )
                {
                    IBranchInstruction it = op as IBranchInstruction;
                    BranchTargets.Add( it.IfTarget );
                    BranchTargets.Add( it.ElseTarget );
                }
            }

            // walk instruction list and split off blocks at branches or branch targets
            List<BasicBlock> Blocks = new List<BasicBlock>();
            int nInstructions = ops.Count;
            int i = 0;
            do
            {
                BasicBlock block = new BasicBlock();
                Blocks.Add(block);

                do
                {
                    IInstruction op = ops[i++];
                    block.AddInstruction(op);
                    op.Block = block;

                    // stop if we just added a branch/jump op
                    if (op is IJumpInstruction || op is IBranchInstruction)
                        break;

                    // stop if next instruction is a branch target
                    if (i<nInstructions && BranchTargets.Contains(ops[i]))
                        break;

                } while (i < nInstructions);
               
            } while (i < nInstructions);

            // construct CFG edges
            for( int b=0; b<Blocks.Count; b++ )
            {
                IInstruction op = Blocks[b].LastInstruction;
                if (op is IBranchInstruction)
                {
                    IBranchInstruction branch = op as IBranchInstruction;
                    if (branch.IfTarget != null)
                    {
                        branch.Block.AddSuccessor(branch.IfTarget.Block);
                        branch.IfTarget.Block.AddPredecessor(branch.Block);
                    }
                    if (branch.ElseTarget != null)
                    {
                        branch.Block.AddSuccessor(branch.ElseTarget.Block);
                        branch.ElseTarget.Block.AddPredecessor(branch.Block);
                    }
                }
                else if (op is IJumpInstruction)
                {
                    // unconditional branch or jump, add target as one and only successor
                    IJumpInstruction jmp = op as IJumpInstruction;
                    if (jmp.Target != null)
                    {
                        jmp.Block.AddSuccessor(jmp.Target.Block);
                        jmp.Target.Block.AddPredecessor(jmp.Block);
                    }
                }
                else if ( b < Blocks.Count-1 )
                {
                    // Block ends in something other than a branch or jump
                    //  add next block as successor
                    Blocks[b].AddSuccessor(Blocks[b + 1]);
                    Blocks[b + 1].AddPredecessor(Blocks[b]);
                }
            }
          
            return Blocks;
        }

        /// <summary>
        ///  Test whether node a dominates node b
        ///     That is, a is equal to be, or an ancestor of b in the dom tree
        /// </summary>
        /// <typeparam name="T"></typeparam>
        /// <param name="a"></param>
        /// <param name="b"></param>
        /// <param name="IDOM"></param>
        /// <returns></returns>
        public static bool Dominates<T>( T a, T b, Dictionary<T,T> IDOM ) where T : class
        {
            if (a == b)
                return true;

            T n = IDOM[b];
            while( n != null && n != a )
                n = IDOM[n];
            
            return n == a;
        }


        /// <summary>
        ///  Find immediate dominators for each node in a graph
        /// </summary>
        /// <param name="nodes"></param>
        /// <param name="edges"></param>
        /// <returns></returns>

        public static Dictionary<T,T> FindDominators<T>( T[] nodes, T[][] predecessors ) where T : class
        {
            if( nodes.Length == 0 )
                return new Dictionary<T,T>();

            // Algorithm here is based on Keith Kooper's COMP512 lecture nodes
            //   from: http://www.cs.rice.edu/~keith/512/2011/Lectures/
            //
            // This could be made redonkulously fast using bit vectors, but we're already using C# so why bother...

            // dominator of the start node is the start itself
            //Dom(n0) = {n0}
            //// for all other nodes, set all nodes as the dominators
            //for each n in N - {n0}
            //    Dom(n) = N;
            //// iteratively eliminate nodes that are not dominators
            //while changes in any Dom(n)
            //    for each n in N - {n0}:  // JDB: Note, just N also works...
            //        Dom(n) = {n} union with intersection over all p in pred(n) of Dom(p)


            Dictionary<T,HashSet<T>> doms = new Dictionary<T,HashSet<T>>();
            

            
            // dominator of start node is the node itself
            //  dom-set of other nodes initially set to entire graph
            T start = nodes[0];
            foreach (T b in nodes)
            {
                if (b == start)
                {
                    HashSet<T> s = new HashSet<T>();
                    s.Add(b);
                    doms.Add(b,s);
                }
                else
                {
                    doms.Add(b, new HashSet<T>(nodes));
                }
            }

            //// iteratively eliminate nodes that are not dominators
            //while changes in any Dom(n)
            //    for each n in N - {n0}:  // JDB: Note, just N also works...
            //        Dom(n) = {n} union with intersection over all p in pred(n) of Dom(p)
            int changes;
            do
            {
                changes = 0;
                for (int i = 0; i < nodes.Length; i++  )
                {
                    HashSet<T> s = new HashSet<T>();
                    T[] preds = predecessors[i];
                    if (preds.Length > 0)
                    {
                        foreach (T d in doms[preds[0]])
                            s.Add(d);
                        for (int j = 1; j < preds.Length; j++)
                            s.IntersectWith(doms[preds[j]]);
                    }

                    T n = nodes[i];
                    s.Add(n);

                    if (!s.SetEquals(doms[n]))
                        changes++;

                    doms[n] = s;
                }

            } while (changes != 0);


            // find immediate dominator for each block
            //  IDOM[b] is the node from doms[b] with highest dominator count
            //    excluding the node itself
            //
            //  IDOM[start] = null
            //
            Dictionary<T, T> IDOM = new Dictionary<T, T>();
            for( int i=0; i<nodes.Length; i++ )
            {
                IDOM.Add(nodes[i],default(T));
                int depth = 0;
                T b = nodes[i];
                foreach (T d in doms[b])
                {
                    if (d == b)
                        continue;

                    int count = doms[d].Count;
                    if (depth < count)
                    {
                        IDOM[nodes[i]] = d;
                        depth = count;
                    }
                }
            }

            return IDOM;
        }



        public static void FindDominators(List<BasicBlock> Blocks)
        {
            BasicBlock[] nodes = Blocks.ToArray();
            BasicBlock[][] preds = new BasicBlock[nodes.Length][];
            BasicBlock[][] succs = new BasicBlock[nodes.Length][];
            for (int i = 0; i < nodes.Length; i++)
            {
                preds[i] = nodes[i].Predecessors.ToArray();
                succs[i] = nodes[i].Successors.ToArray();
            }

            // dominators
            Dictionary<BasicBlock,BasicBlock> IDOM = FindDominators(nodes, preds);
            for( int i=0; i<nodes.Length; i++ )
                nodes[i].ImmediateDominator = IDOM[nodes[i]];

            // post-dominators
            Dictionary<BasicBlock,BasicBlock> PDOM = FindDominators( nodes, succs );
            for (int i = 0; i < nodes.Length; i++)
                nodes[i].PostDominator = PDOM[nodes[i]];
        }



        public static List<Loop> FindLoops( List<BasicBlock> blocks )
        {
            // Great reference:  T Mowry's slides on loop invariant code motion
            // http://www.cs.cmu.edu/afs/cs/academic/class/15745-s11/public/lectures/L7-LICM.pdf
            //
            // We assume a reversible flow graph
            //

            // 1.  Find back edges
            List<KeyValuePair<BasicBlock, BasicBlock>> BackEdges = new List<KeyValuePair<BasicBlock, BasicBlock>>();
            foreach (BasicBlock b in blocks)
                foreach( BasicBlock s in b.Successors )
                    if (s.Dominates(b))
                        BackEdges.Add(new KeyValuePair<BasicBlock, BasicBlock>(b, s));
            
            // 2.  Identify natural loop for each backedge
            //    by walking graph upwards to dominator
            //
            //  The natural loop of a back edge is the set of blocks dominated by the head
            //    and which can reach the tail without traversing any backedge
            //
            HashSet<BasicBlock>[] LoopSets = new HashSet<BasicBlock>[BackEdges.Count];
            for (int i = 0; i < BackEdges.Count; i++)
                LoopSets[i] = new HashSet<BasicBlock>();

            Stack<BasicBlock> S = new Stack<BasicBlock>();
            for( int i=0; i<BackEdges.Count; i++ )
            {
                BasicBlock header = BackEdges[i].Value;
                S.Push(BackEdges[i].Key);
                LoopSets[i].Add(BackEdges[i].Key);

                do
                {
                    BasicBlock n = S.Pop();
                    if (n != header)
                    {
                        foreach (BasicBlock p in n.Predecessors)
                        {
                            if (!LoopSets[i].Contains(p))
                            {
                                LoopSets[i].Add(p);
                                S.Push(p);
                            }
                        }
                    }
                } while (S.Count > 0);
            }

            //
            // 3.  Merge loops with same header wherever one is not a proper subset of the other
            //
            List<Loop> loops = new List<Loop>();
            for (int i = 0; i < BackEdges.Count; i++)
            {
                BasicBlock header = BackEdges[i].Value;
                HashSet<BasicBlock> loop = LoopSets[i];
                if (loop == null)
                    continue; // this loop was merged earlier

                // find other loops with same header and merge with them with in with this one
                for (int j = i + 1; j < BackEdges.Count; j++)
                {
                    if (BackEdges[j].Value == header)
                    {
                        HashSet<BasicBlock> otherLoop = LoopSets[j];
                        if (loop.IsProperSubsetOf(otherLoop) )
                            continue;
                        if (otherLoop.IsProperSupersetOf(loop)) 
                            continue;

                        LoopSets[i].UnionWith(LoopSets[j]);
                        LoopSets[j] = null;
                    }
                }
                loops.Add(new Loop(header, loop));

            }

            
            // Determine nestedness for remaining loops
            //   Loops are now either disjoint, or one is fully nested in the other
            // loop i is nested in loop j if j's header dominates i's header
            //   or they have the same header and j is larger
            int nLoops = loops.Count;
            List<int>[] LoopAncestors = new List<int>[nLoops];
            for (int i = 0; i < nLoops; i++)
            {
                List<int> ancestors = new List<int>();
                Loop li = loops[i];

                for (int j = 0; j < nLoops; j++)
                {
                    if (i == j)
                        continue;

                    Loop lj = loops[j];
                    if (li.Header != lj.Header )
                    {
                        if (lj.Header.Dominates(li.Header))
                            ancestors.Add(j);
                    }
                    else
                    {
                        if (lj.BlockCount > li.BlockCount )
                            ancestors.Add(j);
                    }
                }

                LoopAncestors[i] = ancestors;
            }

            int[] DescendentCounts = new int[nLoops];
            for (int i = 0; i < nLoops; i++ )
                foreach (int k in LoopAncestors[i])
                    DescendentCounts[k]++;
            

            // Find innermost parent for each loop, 
            //  which is the one with the lowest descendent count
            for (int i = 0; i < nLoops; i++)
            {
                List<int> ancestors = LoopAncestors[i];
                if ( ancestors.Count != 0 )
                {
                    int nImmediateParent = ancestors[0];
                    for (int j = 1; j < ancestors.Count; j++)
                        if (DescendentCounts[ancestors[j]] < DescendentCounts[nImmediateParent])
                            nImmediateParent = ancestors[j];

                    loops[i].Parent = loops[nImmediateParent];
                }
            }

            // sort loops inner to outer
            loops.Sort(
                delegate(Loop x, Loop y)
                {
                    if (x == y)
                        return 0;
                    while (true)
                    {
                        x = x.Parent;
                        if (x == null)
                            return 1;
                        else if (x == y)
                            return -1;
                    }
                }
            );

            // Identify innermost loop for each block
            foreach( Loop l in loops )
            {
                foreach (BasicBlock b in l.Blocks)
                {
                    if (b.InnerMostLoop == null)
                        b.InnerMostLoop = l;
                    else if (l.IsNestedIn(b.InnerMostLoop))
                        b.InnerMostLoop = l;
                }
            }

            return loops;
            
        }


        static public void ClassifyBranches(  List<IInstruction> ops )
        {
            // It can be shown (I think) that, given our irreducible graph/nested loop structure
            //  for any branch node n in loop L
            //  at least one of N's two edges must be to a node also in L
            //
            // This follows from the construction of the loops
            //   The loop set is the set of nodes dominated by the header
            //    having paths to the header using only nodes in the loop set
            //  
            //
            // Note that it is still possible to have a branch that descends into one of two different
            //  nested sub-loops
        

            // We can thus classify branch instructions into two types:
            //   - Fork
            //     * both targets are nested within the containing block's loop
            //
            //   - Break branch
            //     * At most one target is outside the containing block's loop
            //
           
            foreach( IInstruction op in ops )
            {
                if (!(op is IBranchInstruction))
                    continue;

                IBranchInstruction branch = op as IBranchInstruction;
                Loop ifLoop    = branch.IfTarget.Block.InnerMostLoop;
                Loop elseLoop  = branch.ElseTarget.Block.InnerMostLoop;
                Loop blockLoop = op.Block.InnerMostLoop;

                if (blockLoop == null)
                {
                    branch.Category = BranchCategory.FORK_BRANCH;
                }
                else
                {
                    if (ifLoop == null && elseLoop == null)
                        branch.Category = BranchCategory.FORK_BRANCH;
                    else if (ifLoop == null && elseLoop != null)
                        branch.Category = BranchCategory.BREAK_BRANCH;
                    else if (ifLoop != null && elseLoop == null)
                        branch.Category = BranchCategory.BREAK_BRANCH;
                    else if (ifLoop.IsNestedIn(blockLoop) && elseLoop.IsNestedIn(blockLoop))
                        branch.Category = BranchCategory.FORK_BRANCH;
                    else if (ifLoop.IsNestedIn(blockLoop) || elseLoop.IsNestedIn(blockLoop))
                        branch.Category = BranchCategory.BREAK_BRANCH;
                    else
                        throw new System.Exception("Ooops");
                }


                // A "fork branch" can, in turn, be classified as:
                //     - continue (conditional jump back to header)
                //     - skip (jumps directly to branch's post-dominator)
                //     - fork (everything else)
                if( branch.Category == BranchCategory.FORK_BRANCH )
                {
                    if (blockLoop != null && (branch.IfTarget == blockLoop.Header || branch.ElseTarget == blockLoop.Header))
                    {
                        branch.Category = BranchCategory.CONTINUE_BRANCH;
                    }

                    if( branch.IfTarget.Block == branch.Block.PostDominator ||
                        branch.ElseTarget.Block == branch.Block.PostDominator )
                    {
                        branch.Category = BranchCategory.SKIP_BRANCH;

                        // a skip branch that jumps around a loop is yet another special case
                        if( branch.IfTarget.Block != branch.Block.PostDominator && branch.IfTarget.Block.InnerMostLoop != blockLoop )
                            branch.Category = BranchCategory.LOOPSKIP_BRANCH;
                        else if (branch.ElseTarget.Block != branch.Block.PostDominator && branch.ElseTarget.Block.InnerMostLoop != blockLoop)
                            branch.Category = BranchCategory.LOOPSKIP_BRANCH;
                        
                    }
                }
            }


        }


        public static void AssignLabels(List<IInstruction> ops)
        {
            int nBlock = 0;
            BasicBlock bl = null;
            for( int i=0; i<ops.Count; i++ )
            {
                if (ops[i].Block != bl)
                {
                    ops[i].Label = String.Format("Block_{0}", nBlock++);
                    bl = ops[i].Block;
                }
            }

        }



        public static List<IInstruction> DoTrace(List<IInstruction> ops, List<BasicBlock> blocks, List<Loop> loops, HashSet<IInstruction> takenBranches )
        {
            List<IInstruction> Exec = new List<IInstruction>();
            Dictionary<Loop, int> LoopCounts = new Dictionary<Loop, int>();
            foreach (Loop l in loops)
                LoopCounts.Add(l, 0);



            BasicBlock b = blocks.First();
            do
            {
                // execute all instructions in current block
                Exec.AddRange(b.Instructions);

                // if block ends in a branch, determine which edge to take
                IInstruction blockEnd = b.LastInstruction;
                BasicBlock next = null;
                if (blockEnd is IBranchInstruction)
                {
                    IBranchInstruction branch = blockEnd as IBranchInstruction;
                    if( branch.Category == BranchCategory.BREAK_BRANCH )
                    {
                        // take a break branch as soon as the iteration count reaches zero
                        BasicBlock exit = branch.IfTarget.Block;
                        BasicBlock noexit = branch.ElseTarget.Block;
                        if (exit.InnerMostLoop == b.InnerMostLoop)
                        {
                            exit = branch.ElseTarget.Block;
                            noexit = branch.IfTarget.Block;
                        }

                        
                        if (LoopCounts[b.InnerMostLoop] >= b.InnerMostLoop.DesiredIterations)
                        {
                            LoopCounts[b.InnerMostLoop] = 0;
                            next = exit;
                        }
                        else
                        {
                            next = noexit;
                        }

                    }
                    else if( branch.Category == BranchCategory.LOOPSKIP_BRANCH )
                    {
                        // don't enter unless the loop's iteration count is non-zero
                        BasicBlock loop;
                        BasicBlock noloop;
                        if (branch.IfTarget.Block.InnerMostLoop != branch.Block.InnerMostLoop)
                        {
                            loop   = branch.IfTarget.Block;
                            noloop = branch.ElseTarget.Block;
                        }
                        else
                        {
                            loop   = branch.ElseTarget.Block;
                            noloop = branch.IfTarget.Block;
                        }

                        if (loop.InnerMostLoop.DesiredIterations > 0)
                            next = loop;
                        else
                            next = noloop;
                    }
                    else if( branch.Category == BranchCategory.CONTINUE_BRANCH )
                    {
                        // do NOT take a continue branch if the count's up
                        BasicBlock go = branch.IfTarget.Block;
                        BasicBlock nogo = branch.ElseTarget.Block;
                        if (nogo == b.InnerMostLoop.Header)
                        {
                            go = branch.ElseTarget.Block;
                            nogo = branch.IfTarget.Block; // swap if needed
                        }

                        if (LoopCounts[b.InnerMostLoop] >= b.InnerMostLoop.DesiredIterations)
                            next = nogo;
                        else
                            next = go;
                    }
                    else
                    {
                        if (takenBranches.Contains(branch))
                            next = branch.IfTarget.Block;
                        else
                            next = branch.ElseTarget.Block;
                    }
                }
                else if( blockEnd is IJumpInstruction )
                {
                    // if block ends in a jump, then jump
                    IJumpInstruction jump = blockEnd as IJumpInstruction;
                    next = jump.Target.Block;
                }
                else
                {
                    // otherwise, proceed to next successor block
                    if (b.SuccessorCount > 0)
                        next = b.Successors.First();
                }

                // increment loop count every time we pass a loop header
                if( b.InnerMostLoop != null && b.InnerMostLoop.Header == b )
                    LoopCounts[b.InnerMostLoop]++;

                b = next;

            } while (b != null);


            return Exec;
        }
    };



   

}