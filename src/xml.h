/*
    Copyright (C) 2013-2014 Carl Hetherington <cth@carlh.net>

    This file is part of libdcp.

    libdcp is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    libdcp is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with libdcp.  If not, see <http://www.gnu.org/licenses/>.

    In addition, as a special exception, the copyright holders give
    permission to link the code of portions of this program with the
    OpenSSL library under certain conditions as described in each
    individual source file, and distribute linked combinations
    including the two.

    You must obey the GNU General Public License in all respects
    for all of the code used other than OpenSSL.  If you modify
    file(s) with this exception, you may extend this exception to your
    version of the file(s), but you are not obligated to do so.  If you
    do not wish to do so, delete this exception statement from your
    version.  If you delete this exception statement from all source
    files in the program, then also delete it here.
*/

#ifndef LIBDCP_XML_H
#define LIBDCP_XML_H

#include "exceptions.h"
#include <libcxml/cxml.h>

namespace dcp
{

template <class T>
boost::shared_ptr<T>
optional_type_child (cxml::Node const & node, std::string name)
{
	std::list<boost::shared_ptr<cxml::Node> > n = node.node_children (name);
	if (n.size() > 1) {
		throw XMLError ("duplicate XML tag");
	} else if (n.empty ()) {
		return boost::shared_ptr<T> ();
	}

	return boost::shared_ptr<T> (new T (n.front ()));
}

template <class T>
boost::shared_ptr<T> type_child (boost::shared_ptr<const cxml::Node> node, std::string name) {
	return boost::shared_ptr<T> (new T (node->node_child (name)));
}

template <class T>
boost::shared_ptr<T>
optional_type_child (boost::shared_ptr<const cxml::Node> node, std::string name)
{
	return optional_type_child<T> (*node.get(), name);
}

template <class T>
std::list<boost::shared_ptr<T> >
type_children (cxml::Node const & node, std::string name)
{
	std::list<boost::shared_ptr<cxml::Node> > n = node.node_children (name);
        std::list<boost::shared_ptr<T> > r;
        for (typename std::list<boost::shared_ptr<cxml::Node> >::iterator i = n.begin(); i != n.end(); ++i) {
		r.push_back (boost::shared_ptr<T> (new T (*i)));
	}
	return r;
}

template <class T>
std::list<boost::shared_ptr<T> >
type_children (boost::shared_ptr<const cxml::Node> node, std::string name)
{
	return type_children<T> (*node.get(), name);
}

template <class T>
std::list<boost::shared_ptr<T> >
type_grand_children (cxml::Node const & node, std::string name, std::string sub)
{
	boost::shared_ptr<const cxml::Node> p = node.node_child (name);
	return type_children<T> (p, sub);
}

template <class T>
std::list<boost::shared_ptr<T> >
type_grand_children (boost::shared_ptr<const cxml::Node> node, std::string name, std::string sub)
{
	return type_grand_children<T> (*node.get(), name, sub);
}

}

#endif
