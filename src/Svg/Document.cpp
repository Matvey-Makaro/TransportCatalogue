#include "Document.h"
#include "Objects.h"

using namespace Svg;

Document::Document() :
    _objects()
{}

void Document::Add(Object obj)
{
    _objects.push_back(std::move(obj));
}

void Document::Render(std::ostream &out)
{
    RenderHead(out);
    RenderObjects(out);
    RenderTail(out);
}

void Document::RenderHead(std::ostream &out)
{
    out << "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>";
    out << "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\">";
}

void Document::RenderObjects(std::ostream &out)
{
    for(const auto& object : _objects)
    {
        std::visit([&out](auto&& obj)
                   {
                       obj.Render(out);
                   }, object);
    }
}

void Document::RenderTail(std::ostream &out)
{
    out << "</svg>";
}
