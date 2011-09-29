/*
 * Copyright (C) 2011 Jonas Weber <mail@jonasw.de>
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

#include "../test.h"

#include <functional>


BOOST_AUTO_TEST_SUITE( xpath )

BOOST_AUTO_TEST_CASE ( create_context )
{
    xml::tree_parser parser("xpath/data/01.xml");
    xml::xpath::context ctxt(parser.get_document());
}

BOOST_AUTO_TEST_CASE ( evaluate )
{
    xml::tree_parser parser("xpath/data/01.xml");
    xml::xpath::context ctxt(parser.get_document());
    xml::xpath::node_set ns = ctxt.evaluate("//child");

    BOOST_CHECK(!ns.empty());
}

BOOST_AUTO_TEST_CASE( evaluate_with_ns )
{
    xml::tree_parser parser("xpath/data/02.xml");
    xml::xpath::context ctxt(parser.get_document());
    
    xml::xpath::node_set ns1 = ctxt.evaluate("//child");
    BOOST_CHECK(ns1.empty());

    ctxt.registerNamespace("p", "href");
    xml::xpath::node_set ns2 = ctxt.evaluate("//p:child");
    BOOST_CHECK(!ns2.empty());

    BOOST_CHECK(ns2.count() == 3);
}


BOOST_AUTO_TEST_CASE( node_set_iterators)
{
    xml::tree_parser parser("xpath/data/02.xml");
    xml::xpath::context ctxt(parser.get_document());
    ctxt.registerNamespace("p", "href");
    xml::xpath::node_set ns = ctxt.evaluate("//p:child");

    BOOST_CHECK(ns.begin() == ns.begin());
    assert(!ns.empty());
    BOOST_CHECK(ns.begin() != ns.end());

    xml::xpath::node_set::iterator it = ns.begin();
    BOOST_CHECK(it++ == ns.begin());
    BOOST_CHECK(it != ns.begin());
    it = ns.begin();
    BOOST_CHECK(++it != ns.begin());
    
    it = ns.end();
    BOOST_CHECK(++it == ns.end() && it++ == ns.end());


    for (it = ns.begin(); it != ns.end(); ++it)
    {
        BOOST_CHECK(it->get_name() != NULL && (*it).get_name() != NULL && strcmp((*it).get_name(), it->get_name()) == 0);
    }
}
BOOST_AUTO_TEST_CASE (node_set_contains)
{
    xml::tree_parser parser("xpath/data/02.xml");
    xml::xpath::context ctxt(parser.get_document());

    ctxt.registerNamespace("p", "href");

    xml::xpath::node_set ns = ctxt.evaluate("//p:child");
    assert(ns.count() == 3);

    xml::node& root = parser.get_document().get_root_node();
    xml::node::iterator it = root.begin(); ++it;
    xml::node& nSC = *it;
    BOOST_CHECK(ns.contains(nSC));
}

BOOST_AUTO_TEST_SUITE_END()
