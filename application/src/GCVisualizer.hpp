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
#include <algorithm>
#include <vector>
#include <iomanip>
#include <tuple>
#include <random>

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
    static std::unordered_map<std::string, std::string> nodeColors;
    static std::vector<std::string> availableColors;

    void drawNodes(Agraph_t *graph, GCObjectBase *node, std::unordered_map<GCObjectBase *, Agnode_t *> &nodes)
    {
        if (nodes.find(node) == nodes.end())
        {
            std::string label = "Node_" + std::to_string(reinterpret_cast<uintptr_t>(node));
            nodes[node] = agnode(graph, const_cast<char *>(label.c_str()), 1);
            std::string color = getColorForNode(label);
            agsafeset(nodes[node], const_cast<char *>("fillcolor"), const_cast<char *>(color.c_str()), const_cast<char *>("black"));
            agsafeset(nodes[node], const_cast<char *>("style"), const_cast<char *>("filled"), const_cast<char *>(""));
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

    std::string getColorForNode(const std::string &label)
    {
        auto it = nodeColors.find(label);
        if (it == nodeColors.end())
        {
            if (availableColors.empty())
            {
                // If no colors are available, use a default color
                return "black";
            }
            else
            {
                std::string newColor = availableColors.back();
                availableColors.pop_back();
                nodeColors[label] = newColor;
                return newColor;
            }
        }
        else
        {
            return it->second;
        }
    }

    static std::vector<std::string> generateColors(size_t count)
    {
        std::vector<std::string> colors;
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> dist(0, 255);

        for (size_t i = 0; i < count; ++i)
        {
            int r = dist(gen);
            int g = dist(gen);
            int b = dist(gen);
            colors.push_back(rgbToHexString(std::make_tuple(r, g, b)));
        }
        return colors;
    }

    static std::tuple<int, int, int> hslToRgb(float h, float s, float l)
    {
        float r, g, b;

        if (s == 0.0f)
        {
            r = g = b = l;
        }
        else
        {
            auto hueToRgb = [](float p, float q, float t)
            {
                if (t < 0)
                    t += 1;
                if (t > 1)
                    t -= 1;
                if (t < 1.0f / 6)
                    return p + (q - p) * 6 * t;
                if (t < 1.0f / 2)
                    return q;
                if (t < 2.0f / 3)
                    return p + (q - p) * (2.0f / 3 - t) * 6;
                return p;
            };

            float q = l < 0.5f ? l * (1 + s) : l + s - l * s;
            float p = 2 * l - q;
            r = hueToRgb(p, q, h + 1.0f / 3);
            g = hueToRgb(p, q, h);
            b = hueToRgb(p, q, h - 1.0f / 3);
        }

        return std::make_tuple(static_cast<int>(r * 255 + 0.5f), static_cast<int>(g * 255 + 0.5f), static_cast<int>(b * 255 + 0.5f));
    }

    static std::string rgbToHexString(const std::tuple<int, int, int> &rgb)
    {
        std::ostringstream oss;
        oss << "#" << std::hex << std::setw(2) << std::setfill('0') << std::get<0>(rgb)
            << std::setw(2) << std::setfill('0') << std::get<1>(rgb)
            << std::setw(2) << std::setfill('0') << std::get<2>(rgb);
        return oss.str();
    }
};

std::unordered_map<std::string, std::string> GCVisualizer::nodeColors;
std::vector<std::string> GCVisualizer::availableColors = GCVisualizer::generateColors(100);

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
        if (result == 0)
        {
        }
        else
        {
            std::cerr << "Failed to execute Graphviz command" << std::endl;
        }
    }

private:
    GarbageCollector &gc_;
    static std::unordered_map<std::string, std::string> nodeColors;
    static std::vector<std::string> availableColors;

    void drawNodes(std::ofstream &dotFile, GCObjectBase *node, std::unordered_map<GCObjectBase *, std::string> &nodes)
    {
        if (nodes.find(node) == nodes.end())
        {
            std::string label = "Node_" + std::to_string(reinterpret_cast<uintptr_t>(node));
            nodes[node] = label;
            std::string color = getColorForNode(label);
            dotFile << label << "[label=\"" << label << "\", fillcolor=\"" << color << "\", style=filled];" << std::endl;
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

    std::string getColorForNode(const std::string &label)
    {
        auto it = nodeColors.find(label);
        if (it == nodeColors.end())
        {
            if (availableColors.empty())
            {
                // If no colors are available, use a default color
                return "black";
            }
            else
            {
                std::string newColor = availableColors.back();
                availableColors.pop_back();
                nodeColors[label] = newColor;
                return newColor;
            }
        }
        else
        {
            return it->second;
        }
    }

    static std::vector<std::string> generateColors(size_t count)
    {
        std::vector<std::string> colors;
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> dist(0, 255);

        for (size_t i = 0; i < count; ++i)
        {
            int r = dist(gen);
            int g = dist(gen);
            int b = dist(gen);
            colors.push_back(rgbToHexString(std::make_tuple(r, g, b)));
        }
        return colors;
    }

    static std::tuple<int, int, int> hslToRgb(float h, float s, float l)
    {
        float r, g, b;
        if (s == 0.0f)
        {
            r = g = b = l;
        }
        else
        {
            auto hueToRgb = [](float p, float q, float t)
            {
                if (t < 0)
                    t += 1;
                if (t > 1)
                    t -= 1;
                if (t < 1.0f / 6)
                    return p + (q - p) * 6 * t;
                if (t < 1.0f / 2)
                    return q;
                if (t < 2.0f / 3)
                    return p + (q - p) * (2.0f / 3 - t) * 6;
                return p;
            };

            float q = l < 0.5f ? l * (1 + s) : l + s - l * s;
            float p = 2 * l - q;
            r = hueToRgb(p, q, h + 1.0f / 3);
            g = hueToRgb(p, q, h);
            b = hueToRgb(p, q, h - 1.0f / 3);
        }

        return std::make_tuple(static_cast<int>(r * 255 + 0.5f), static_cast<int>(g * 255 + 0.5f), static_cast<int>(b * 255 + 0.5f));
    }

    static std::string rgbToHexString(const std::tuple<int, int, int> &rgb)
    {
        std::ostringstream oss;
        oss << "#" << std::hex << std::setw(2) << std::setfill('0') << std::get<0>(rgb)
            << std::setw(2) << std::setfill('0') << std::get<1>(rgb)
            << std::setw(2) << std::setfill('0') << std::get<2>(rgb);
        return oss.str();
    }
};

std::unordered_map<std::string, std::string> GCVisualizer::nodeColors;
std::vector<std::string> GCVisualizer::availableColors = GCVisualizer::generateColors(100);

#endif // WINDOWS

#endif // GC_VISUALIZER_HPP
