// Copyright (c) 2022, Rerito
// SPDX-License-Identifier: MIT

#pragma once

#include <Penjing/Meta/Access.hpp>

#include <Penjing/Storage/Bindings/ArrayList.hpp>
#include <Penjing/Storage/Store.hpp>

namespace Penjing {
namespace SuffixTree {

namespace Algorithm {

class MutatingTreeAlgorithm;

} // namespace Algorithm

namespace Core {

// The Tree class is a self-sufficient (generalized) suffix tree class:
// It holds the strings being added to the tree on top of the tree itself.
template<
    typename UnderlyingTree,
    typename StringStorageTraits =
        Storage::Bindings::ArrayList< std::allocator< UnderlyingTree >, 64u > >
class Tree
{
public:
    using NodeType = typename UnderlyingTree::NodeType;
    using StringType = typename UnderlyingTree::StringType;
    using StringViewType = typename UnderlyingTree::StringViewType;
    using CharType = typename UnderlyingTree::CharType;

private:
    using StringStorage = Storage::Store< StringType, StringStorageTraits >;

private:
    StringStorage _stringStorage;
    UnderlyingTree _nakedTree;

public:
    constexpr auto& mutableStringStorage(
        Meta::Access< Algorithm::MutatingTreeAlgorithm > const&) noexcept
    {
        return _stringStorage;
    }

    constexpr auto&
    mutableRoot(Meta::Access< Algorithm::MutatingTreeAlgorithm > const&) noexcept(
        noexcept(_nakedTree.mutableRoot()))
    {
        return _nakedTree.mutableRoot();
    }

    constexpr auto const& root() const noexcept(noexcept(_nakedTree.root()))
    {
        return _nakedTree.root();
    }

    constexpr auto
    nodeFactory(Meta::Access< Algorithm::MutatingTreeAlgorithm > const&) noexcept(
        noexcept(_nakedTree.nodeFactory()))
    {
        return _nakedTree.nodeFactory();
    }
};

} // namespace Core

} // namespace SuffixTree
} // namespace Penjing
