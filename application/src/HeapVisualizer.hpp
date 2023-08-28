#ifndef HEAPVISUALIZER_HPP
#define HEAPVISUALIZER_HPP

#include "MemoryManagement.hpp"
#include "HeapObserver.hpp" // Include your Observer class definition

#include <gvc/gvc.h>
#include <cgraph/cgraph.h> // Include appropriate Graphviz headers
#include <string>

class HeapVisualizer : public HeapObserver
{
public:
    HeapVisualizer()
    {
        MemoryManagement::getInstance().registerObserver(this);
    }

    ~HeapVisualizer()
    {
        MemoryManagement::getInstance().unregisterObserver(this);
    }

    void onUpdate() override
    {
        visualizeHeap();
    }

    void visualizeHeap()
    {
        // Create a new Graphviz graph
        Agraph_t *g = agopen("Heap", Agstrictdirected, nullptr);

        // Reduce node separation and rank separation
        agsafeset(g, "nodesep", "0.02", "");
        agsafeset(g, "ranksep", "0.02", "");

        // Get the current state of the heap from the memory manager
        auto blocks = MemoryManagement::getInstance().getBlocks();
        auto maxBlockSize = MemoryManagement::getInstance().getMaxBlockSize();

        if (maxBlockSizeVar < maxBlockSize)
        {
            maxBlockSizeVar = maxBlockSize;
        }

        // Create a node for each block
        for (const auto &block : blocks)
        {
            auto address = MemoryManagement::getInstance().getAddressOfGCOBjectBase(block.address);
            Agnode_t *n = nullptr;
            if (address)
            {
                n = agnode(g, const_cast<char *>(std::to_string(reinterpret_cast<uintptr_t>(address) % 1000).c_str()), TRUE);
            }
            else
            {
                n = agnode(g, const_cast<char *>(""), TRUE);
            }
            // Block size could be included in the label

            // Set fillcolor and style
            agsafeset(n, "fillcolor", block.isAllocated ? "green" : "white", "");
            agsafeset(n, "style", "filled", "");

            // Set shape to square
            agsafeset(n, "shape", "box", "");

            // Set width and height
            std::string width = std::to_string(static_cast<double>(block.size) / maxBlockSizeVar * 3);
            agsafeset(n, "width", const_cast<char *>(width.c_str()), "");
            agsafeset(n, "height", "1", ""); // height is fixed at 1
        }

        // Layout the graph
        gvLayout(gvc, g, "dot");

        // Render the graph to a file
        gvRenderFilename(gvc, g, "png", "heap.png");

        // Clean up
        gvFreeLayout(gvc, g);
        agclose(g);
    }

private:
    GVC_t *gvc = gvContext(); // Initialize a new Graphviz context
    static inline double maxBlockSizeVar = 0;
};

#endif // HEAPVISUALIZER_HPP