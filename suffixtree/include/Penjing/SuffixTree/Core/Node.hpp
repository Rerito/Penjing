// Copyright (c) 2021, Rerito
// SPDX-License-Identifier: MIT

#pragma once

#include <functional>
#include <optional>
#include <stdexcept>

#include <Penjing/Meta/Access.hpp>
#include <Penjing/Meta/CustomizationPoints/Mapped.hpp>
#include <Penjing/Meta/Memory.hpp>
#include <Penjing/Meta/ValueType.hpp>

#include "Transition.hpp"
#include "UnsafeTag.hpp"

namespace Penjing {
namespace SuffixTree {

namespace Algorithm {

class MutatingNodeAlgorithm;

}

namespace Core {

template< typename Str, typename StrView, typename NodeTraits >
class Node
{
public:
    using CharType = Meta::ValueType< Str >;
    using StringType = Str;
    using StringViewType = StrView;
    using NodePtr = std::add_pointer_t< Node >;

    using NodeConstRefW = std::reference_wrapper< Node const >;
    using NodeRefW = std::reference_wrapper< Node >;

    // Transition related type aliases
    using TransitionType = Transition< StrView, NodePtr >;
    using TransitionConstRefW = std::reference_wrapper< TransitionType const >;
    using TransitionRefW = std::reference_wrapper< TransitionType >;

    using TransitionContainerType =
        typename NodeTraits::template StorageType< CharType, TransitionType >;

private:
    NodePtr _parent;
    NodePtr _suffixLink;
    TransitionContainerType _transitions;

public:
    constexpr Node() = default;
    constexpr explicit Node(NodePtr parent)
        : _parent(std::move(parent))
    {
    }

    constexpr std::optional< NodeConstRefW > parent() const
    {
        if (nullptr == _parent) {
            return {};
        }

        return std::cref(*_parent);
    }

    constexpr std::optional< NodeConstRefW > suffixLink() const noexcept
    {
        if (nullptr == _suffixLink) {
            return {};
        }

        return std::cref(*_suffixLink);
    }

    constexpr std::optional< NodeRefW > suffixLink(
        Meta::Access< Algorithm::MutatingNodeAlgorithm >) noexcept
    {
        if (nullptr == _suffixLink) {
            return {};
        }

        return std::ref(*_suffixLink);
    }

    constexpr auto transitions() const noexcept
    {
        using std::begin;
        using std::end;

        return std::make_pair(begin(_transitions), end(_transitions));
    }

    constexpr auto childrenCount() const
        noexcept(noexcept(std::size(_transitions)))
    {
        return std::size(_transitions);
    }

    constexpr std::optional< TransitionConstRefW > transition(CharType start)
        const noexcept(noexcept(Meta::mappedAt(_transitions, start)))
    {
        return Meta::mappedAt(_transitions, start);
    }

    constexpr auto const& transition(CharType start, UnsafeTag) const
        noexcept(noexcept(Meta::unsafeMappedAt(_transitions, start)))
    {
        return Meta::unsafeMappedAt(_transitions, start);
    }

    constexpr auto operator[](CharType start) const
        noexcept(noexcept(transition(start)))
    {
        return transition(start);
    }

    constexpr std::optional< TransitionRefW > mutableTransition(
        Meta::Access< Algorithm::MutatingNodeAlgorithm >,
        CharType start) noexcept(noexcept(Meta::mappedAt(_transitions, start)))
    {
        return Meta::mappedAt(_transitions, start);
    }

    constexpr auto& mutableTransition(
        Meta::Access< Algorithm::MutatingNodeAlgorithm >,
        CharType start,
        UnsafeTag) noexcept(noexcept(Meta::unsafeMappedAt(_transitions, start)))
    {
        return Meta::unsafeMappedAt(_transitions, start);
    }

    constexpr TransitionRefW addTransition(
        Meta::Access< Algorithm::MutatingNodeAlgorithm >,
        StringViewType label,
        NodePtr target)
    {
        return addTransition(std::move(label), std::move(target));
    }

    constexpr void setLink(
        Meta::Access< Algorithm::MutatingNodeAlgorithm >,
        NodePtr link) noexcept(std::is_nothrow_move_assignable_v< NodePtr >)
    {
        setLink(std::move(link));
    }

    constexpr void setParent(
        Meta::Access< Algorithm::MutatingNodeAlgorithm >,
        NodePtr parent) noexcept(std::is_nothrow_move_assignable_v< NodePtr >)
    {
        setParent(std::move(parent));
    }

private:
    constexpr TransitionRefW addTransition(StringViewType label, NodePtr target)
    {
        using std::begin;
        using std::empty;

        if (empty(label)) {
            throw std::invalid_argument("label: expecting non empty label");
        }

        auto newTransition = _transitions.emplace(
            *begin(label),
            TransitionType{std::move(label), std::move(target)});

        if (newTransition.second) {
            return std::ref(newTransition.first->second);
        }

        throw std::invalid_argument(
            "label: an existing transition clashes with the given label");
    }

    constexpr void setLink(NodePtr link) noexcept(
        std::is_nothrow_move_assignable_v< NodePtr >)
    {
        _suffixLink = std::move(link);
    }

    constexpr void setParent(NodePtr parent) noexcept(
        std::is_nothrow_move_assignable_v< NodePtr >)
    {
        _parent = std::move(parent);
    }
};

} // namespace Core
} // namespace SuffixTree
} // namespace Penjing
