#pragma once

#include <iostream>
#include <vector>
#include "Objects.h"


namespace Svg
{

class Document
{
public:
    Document();
    Document(const Document& other) = default;
    Document(Document&& other) = default;

    Document& operator=(const Document& other) = default;
    Document& operator=(Document&& other) = default;

    void Add(Object obj);
    void Render(std::ostream& out);

private:
    void RenderHead(std::ostream& out);
    void RenderObjects(std::ostream& out);
    void RenderTail(std::ostream& out);

private:
    std::vector<Object> _objects;
};

}
