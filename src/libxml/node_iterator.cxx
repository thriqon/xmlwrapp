/*
 * Copyright (C) 2001-2003 Peter J Jones (pjones@pmade.org)
 *               2009      Vaclav Slavik <vslavik@gmail.com>
 * All Rights Reserved
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name of the Author nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS''
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR
 * OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
 * USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

// definition include
#include "node_iterator.h"
#include "pimpl_base.h"

// xmlwrapp includes
#include "xmlwrapp/node.h"
#include "xmlwrapp/nodes_view.h"

// standard includes
#include <algorithm>
#include <cassert>

// libxml includes
#include <libxml/tree.h>

namespace xml
{

using namespace impl;

// ------------------------------------------------------------------------
// impl::nipimpl
// ------------------------------------------------------------------------

// node::iterator pimpl
struct impl::nipimpl : public pimpl_base<impl::nipimpl>
{
    node_iterator it;

    nipimpl() {};
    nipimpl(xmlNodePtr ptr) : it(ptr) {}
    nipimpl(const nipimpl& other) : it(other.it) {}
};

// ------------------------------------------------------------------------
// xml::impl::node_iterator - real iterator class
// ------------------------------------------------------------------------

node* impl::node_iterator::get() const
{
    fake_node_.set_node_data(node_);
    return& fake_node_;
}


// ------------------------------------------------------------------------
// xml::node::iterator wrapper iterator class
// ------------------------------------------------------------------------

node::iterator::iterator(void *data)
{
    pimpl_ = new nipimpl(static_cast<xmlNodePtr>(data));
}


node::iterator::iterator(const iterator& other)
{
    pimpl_ = other.pimpl_ ? new nipimpl(*(other.pimpl_)) : 0;
}


node::iterator& node::iterator::operator=(const iterator& other)
{
    iterator tmp(other);
    swap(tmp);
    return *this;
}


void node::iterator::swap(iterator& other)
{
    std::swap(pimpl_, other.pimpl_);
}


node::iterator::~iterator()
{
    delete pimpl_;
}


node::iterator::reference node::iterator::operator*() const
{
    return *(pimpl_->it.get());
}


node::iterator::pointer node::iterator::operator->() const
{
    return pimpl_->it.get();
}


node::iterator& node::iterator::operator++()
{
    pimpl_->it.advance();
    return *this;
}


node::iterator node::iterator::operator++(int)
{
    iterator tmp(*this);
    ++(*this);
    return tmp;
}


void* node::iterator::get_raw_node() const
{
    return pimpl_ ? pimpl_->it.get_raw_node() : 0;
}


// ------------------------------------------------------------------------
// xml::node::const_iterator wrapper iterator class
// ------------------------------------------------------------------------

node::const_iterator::const_iterator(void *data)
{
    pimpl_ = new nipimpl(static_cast<xmlNodePtr>(data));
}


node::const_iterator::const_iterator(const const_iterator& other)
{
    pimpl_ = other.pimpl_ ? new nipimpl(*(other.pimpl_)) : 0;
}


node::const_iterator::const_iterator(const iterator& other)
{
    pimpl_ = other.pimpl_ ? new nipimpl(*(other.pimpl_)) : 0;
}


node::const_iterator& node::const_iterator::operator=(const const_iterator& other)
{
    const_iterator tmp(other);
    swap(tmp);
    return *this;
}


void node::const_iterator::swap(const_iterator& other)
{
    std::swap(pimpl_, other.pimpl_);
}


node::const_iterator::~const_iterator()
{
    delete pimpl_;
}


node::const_iterator::reference node::const_iterator::operator*() const
{
    return *(pimpl_->it.get());
}


node::const_iterator::pointer node::const_iterator::operator->() const
{
    return pimpl_->it.get();
}


node::const_iterator& node::const_iterator::operator++()
{
    pimpl_->it.advance();
    return *this;
}


node::const_iterator node::const_iterator::operator++(int)
{
    const_iterator tmp(*this);
    ++(*this);
    return tmp;
}


void* node::const_iterator::get_raw_node() const
{
    return pimpl_ ? pimpl_->it.get_raw_node() : 0;
}

// ------------------------------------------------------------------------
// xml::nodes_view::iterator
// ------------------------------------------------------------------------

nodes_view::iterator::iterator(const iterator& other)
{
    pimpl_ = other.pimpl_ ? new nipimpl(*(other.pimpl_)) : 0;
    advance_func_ = other.advance_func_;
}


nodes_view::iterator&
nodes_view::iterator::operator=(const iterator& other)
{
    iterator tmp(other);
    swap(tmp);
    return *this;
}


nodes_view::iterator::~iterator()
{
    delete pimpl_;
}


nodes_view::iterator::reference
nodes_view::iterator::operator*() const
{
    return *(pimpl_->it.get());
}


nodes_view::iterator::pointer
nodes_view::iterator::operator->() const
{
    return pimpl_->it.get();
}


nodes_view::iterator& nodes_view::iterator::operator++()
{
    assert( advance_func_ );
    pimpl_->it.advance(*advance_func_);
    return *this;
}


nodes_view::iterator nodes_view::iterator::operator++(int)
{
    iterator tmp(*this);
    ++(*this);
    return tmp;
}


void* nodes_view::iterator::get_raw_node() const
{
    return pimpl_ ? pimpl_->it.get_raw_node() : 0;
}


nodes_view::iterator::iterator(void *data, impl::iter_advance_functor *advance_func)
{
    assert( advance_func );
    pimpl_ = new nipimpl(static_cast<xmlNodePtr>(data));
    advance_func_ = advance_func;
}


void nodes_view::iterator::swap(iterator& other)
{
    std::swap(pimpl_, other.pimpl_);
    std::swap(advance_func_, other.advance_func_);
}

// ------------------------------------------------------------------------
// xml::nodes_view::const_iterator
// ------------------------------------------------------------------------

nodes_view::const_iterator::const_iterator(const const_iterator& other)
{
    pimpl_ = other.pimpl_ ? new nipimpl(*(other.pimpl_)) : 0;
    advance_func_ = other.advance_func_;
}


nodes_view::const_iterator::const_iterator(const iterator& other)
{
    pimpl_ = other.pimpl_ ? new nipimpl(*(other.pimpl_)) : 0;
    advance_func_ = other.advance_func_;
}


nodes_view::const_iterator&
nodes_view::const_iterator::operator=(const const_iterator& other)
{
    const_iterator tmp(other);
    swap(tmp);
    return *this;
}


nodes_view::const_iterator&
nodes_view::const_iterator::operator=(const iterator& other)
{
    const_iterator tmp(other);
    swap(tmp);
    return *this;
}


nodes_view::const_iterator::~const_iterator()
{
    delete pimpl_;
}


nodes_view::const_iterator::reference
nodes_view::const_iterator::operator*() const
{
    return *(pimpl_->it.get());
}


nodes_view::const_iterator::pointer
nodes_view::const_iterator::operator->() const
{
    return pimpl_->it.get();
}


nodes_view::const_iterator& nodes_view::const_iterator::operator++()
{
    assert( advance_func_ );
    pimpl_->it.advance(*advance_func_);
    return *this;
}


nodes_view::const_iterator nodes_view::const_iterator::operator++(int)
{
    const_iterator tmp(*this);
    ++(*this);
    return tmp;
}


void* nodes_view::const_iterator::get_raw_node() const
{
    return pimpl_ ? pimpl_->it.get_raw_node() : 0;
}


nodes_view::const_iterator::const_iterator(void *data, impl::iter_advance_functor *advance_func)
{
    assert( advance_func );
    pimpl_ = new nipimpl(static_cast<xmlNodePtr>(data));
    advance_func_ = advance_func;
}


void nodes_view::const_iterator::swap(nodes_view::const_iterator& other)
{
    std::swap(pimpl_, other.pimpl_);
    std::swap(advance_func_, other.advance_func_);
}

} // namespace xml
