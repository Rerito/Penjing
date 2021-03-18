// Copyright (c) 2021, Rerito
// SPDX-License-Identifier: MIT

#include <ranges>

#include <BananaFixture.hpp>

namespace Penjing {
namespace SuffixTree {
namespace Test {

BananaFixture::BananaFixture()
    : _banana("banana$")
    , _nodes()
    , n_root(_nodes.at(0))
    , n_a(_nodes.at(1))
    , n_banana_(_nodes.at(2))
    , n_na(_nodes.at(3))
    , n_(_nodes.at(4))
    , n_a_(_nodes.at(5))
    , n_ana(_nodes.at(6))
    , n_na_(_nodes.at(7))
    , n_nana_(_nodes.at(8))
    , n_ana_(_nodes.at(9))
    , n_anana_(_nodes.at(10))
{
    auto start = std::ranges::begin(_banana);
    auto end = std::ranges::end(_banana);

    addTransition(n_root, {start + 1, start + 2}, &n_a);
    addTransition(n_root, {start, end}, &n_banana_);
    addTransition(n_root, {start + 2, start + 4}, &n_na);
    addTransition(n_root, {start+6, end}, &n_);

    addTransition(n_a, {end - 1, end}, &n_a_);
    addTransition(n_a, {start + 2, start + 4}, &n_ana);

    addTransition(n_na, {end - 1, end}, &n_na_);
    addTransition(n_na, {end - 3, end}, &n_nana_);

    addTransition(n_ana, {end - 1, end}, &n_ana_);
    addTransition(n_ana, {end - 3, end}, &n_anana_);
}

} // namespace Test
} // namespace SuffixTree
} // namespace Penjing
