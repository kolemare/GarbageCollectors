// GCVisualizer.hpp
#define WINDOWS

#ifndef GC_VISUALIZER_HPP
#define GC_VISUALIZER_HPP

#include "GCObserver.hpp"
#include "GarbageCollector.hpp"
#include <gvc/gvc.h>
#include <cgraph/cgraph.h>
#include <string>
#include <unordered_map>
#include "GCInvokeDisplaySingleton.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <cstdlib>

#ifdef WINDOWS

class GCVisualizer : public GCObserver
{
public:
    GCVisualizer(GarbageCollector &gc) : gc_(gc)
    {
        GCInvokeDisplaySingleton::getInstance();
        gc_.registerObserver(this);
    }

    ~GCVisualizer()
    {
        gc_.unregisterObserver(this);
    }

    void onUpdate() override
    {
        draw();
    }

    void draw()
    {
        GVC_t *gvc = gvContext();
        Agraph_t *graph = agopen(const_cast<char *>("GCVisualizer"), Agdirected, nullptr);

        std::unordered_map<GCObjectBase *, Agnode_t *> nodes;
        for (GCObjectBase *root : gc_.get_root_set())
        {
            drawNodes(graph, root, nodes);
        }

        const char *layout = "dot";
        const char *format = "png";
        const char *outputFilename = "gc_visualizer.png";

        gvLayout(gvc, graph, layout);
        gvRenderFilename(gvc, graph, format, outputFilename);
        gvFreeLayout(gvc, graph);
        agclose(graph);
        gvFreeContext(gvc);
    }

private:
    GarbageCollector &gc_;

    void drawNodes(Agraph_t *graph, GCObjectBase *node, std::unordered_map<GCObjectBase *, Agnode_t *> &nodes)
    {
        if (nodes.find(node) == nodes.end())
        {
            std::string label = "Node_" + std::to_string(reinterpret_cast<uintptr_t>(node));
            nodes[node] = agnode(graph, const_cast<char *>(label.c_str()), 1);
        }

        for (GCObjectBase *child : node->get_children())
        {
            if (nodes.find(child) == nodes.end())
            {
                drawNodes(graph, child, nodes);
            }
            agedge(graph, nodes[node], nodes[child], nullptr, 1);
        }
    }
};

#else

class GCVisualizer : public GCObserver
{
public:
    GCVisualizer(GarbageCollector &gc) : gc_(gc)
    {
        GCInvokeDisplaySingleton::getInstance();
        gc_.registerObserver(this);
    }

    ~GCVisualizer()
    {
        gc_.unregisterObserver(this);
    }

    void onUpdate() override
    {
        draw();
    }

    void draw()
    {
        std::ofstream dotFile("gc_visualizer.dot");
        dotFile << "digraph GCVisualizer {" << std::endl;

        std::unordered_map<GCObjectBase *, std::string> nodes;
        for (GCObjectBase *root : gc_.get_root_set())
        {
            drawNodes(dotFile, root, nodes);
        }

        dotFile << "}" << std::endl;
        dotFile.close();

        const char *command = "dot -Tpng gc_visualizer.dot -o gc_visualizer.png";
        int result = system(command);
        if (result != 0)
        {
            std::cerr << "Failed to execute Graphviz command" << std::endl;
        }
    }

private:
    GarbageCollector &gc_;

    void drawNodes(std::ofstream &dotFile, GCObjectBase *node, std::unordered_map<GCObjectBase *, std::string> &nodes)
    {
        if (nodes.find(node) == nodes.end())
        {
            std::string label = "Node_" + std::to_string(reinterpret_cast<uintptr_t>(node));
            nodes[node] = label;
            dotFile << label << "[label=\"" << label << "\"];" << std::endl;
        }

        for (GCObjectBase *child : node->get_children())
        {
            if (nodes.find(child) == nodes.end())
            {
                drawNodes(dotFile, child, nodes);
            }
            dotFile << nodes[node] << " -> " << nodes[child] << ";" << std::endl;
        }
    }
};

#endif // WINDOWS

#endif // GC_VISUALIZER_HPP
