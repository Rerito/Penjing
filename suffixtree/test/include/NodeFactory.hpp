// Copyright (c) 2021, Rerito
// SPDX-License-Identifier: MIT

#pragma once

#include <memory>
#include <vector>

namespace Penjing {
namespace SuffixTree {
namespace Test {

template< typename Node >
class NodeFactory
{
public:
    template< typename... Args >
    Node& operator()(Args&&... args)
    {
        _nodes.push_back(
            std::make_unique< Node >(std::forward< Args >(args)...));

        return *_nodes.back();
    }

private:
    std::vector< std::unique_ptr< Node > > _nodes;
};

} // namespace Test
} // namespace SuffixTree
} // namespace Penjing
