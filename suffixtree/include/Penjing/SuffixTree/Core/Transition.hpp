// Copyright (c) 2021, Rerito
// SPDX-License-Identifier: MIT

#pragma once

#include <cassert>
#include <type_traits>
#include <utility>

namespace Penjing {
namespace SuffixTree {
namespace Core {

namespace Traits {

namespace Details {

template< typename T >
struct NodeTypeFromPtr
{
    using Type = typename T::element_type;
};

template< typename T >
struct NodeTypeFromPtr< T* >
{
    using Type = std::decay_t< T >;
};

} // namespace Details

template< typename T >
using NodeTypeFromPtr = typename Details::NodeTypeFromPtr< T >::Type;

} // namespace Traits

template< typename StrView, typename NodePtr >
class Transition
{
public:
    using NodePtrType = NodePtr;
    using NodeType = Traits::NodeTypeFromPtr< NodePtrType >;
    using NodeConstReference =
        std::add_lvalue_reference_t< std::add_const_t< NodeType > >;
    using NodeReference = std::add_lvalue_reference_t< NodeType >;

private:
    StrView _label;
    NodePtr _target;

public:
    Transition(StrView label, NodePtr&& target)
        : _label(std::move(label))
        , _target(std::move(target))
    {}

    StrView const& label() const noexcept { return _label; }

    NodeConstReference operator*() const noexcept
    {
        assert(!!_target);
        return *_target;
    }

    NodeReference operator*() noexcept
    {
        return const_cast< NodeReference >(
            const_cast< Transition const& >(*this).operator*());
    }

    NodePtr const& target() const noexcept { return _target; }

    StrView& label() noexcept { return _label; }

    NodePtr& target() noexcept { return _target; }
};

} // namespace Core
} // namespace SuffixTree
} // namespace Penjing
