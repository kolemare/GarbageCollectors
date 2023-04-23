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
        std::ofstream dotfile("gc_visualizer.dot");
        dotfile << "digraph GCVisualizer {\n";

        std::unordered_map<GCObjectBase *, std::string> nodes;
        for (GCObjectBase *root : gc_.get_root_set())
        {
            drawNodes(dotfile, root, nodes);
        }

        dotfile << "}\n";
        dotfile.close();

        const char *dot = "dot";
        const char *format = "png";
        const char *inputFilename = "gc_visualizer.dot";
        const char *outputFilename = "gc_visualizer.png";

        char cmd[1024];
        snprintf(cmd, sizeof(cmd), "%s -T%s %s -o %s", dot, format, inputFilename, outputFilename);
        system(cmd);
    }

private:
    GarbageCollector &gc_;

    void drawNodes(std::ofstream &dotfile, GCObjectBase *node, std::unordered_map<GCObjectBase *, std::string> &nodes)
    {
        if (nodes.find(node) == nodes.end())
        {
            std::string label = "Node_" + std::to_string(reinterpret_cast<uintptr_t>(node));
            nodes[node] = label;
            dotfile << "  " << label << " [label=\"" << label << "\"];\n";
        }

        for (GCObjectBase *child : node->get_children())
        {
            if (nodes.find(child) == nodes.end())
            {
                drawNodes(dotfile, child, nodes);
            }
            dotfile << "  " << nodes[node] << " -> " << nodes[child] << ";\n";
        }
    }
};

#endif // WINDOWS

#endif // GC_VISUALIZER_HPP
