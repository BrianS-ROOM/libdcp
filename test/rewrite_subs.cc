/*
    Copyright (C) 2014-2019 Carl Hetherington <cth@carlh.net>

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

#include <iostream>
#include "dcp.h"
#include "cpl.h"
#include "reel.h"
#include "subtitle_asset.h"
#include "reel_subtitle_asset.h"
#include "exceptions.h"

using std::cout;
using std::cerr;
using std::list;
using std::string;
using boost::shared_ptr;
using namespace dcp;

/** Load a DCP then re-write its subtitle XML or MXF in-place */
int
main (int argc, char* argv[])
{
	try {
		if (argc < 2) {
			cerr << "Syntax: " << argv[0] << " <dcp>\n";
			exit (EXIT_FAILURE);
		}

		DCP* dcp = new DCP (argv[1]);
		dcp->read ();

		list<shared_ptr<CPL> > cpls = dcp->cpls ();
		for (list<boost::shared_ptr<CPL> >::iterator i = cpls.begin(); i != cpls.end(); ++i) {

			list<shared_ptr<Reel> > reels = (*i)->reels ();
			for (list<shared_ptr<Reel> >::iterator j = reels.begin(); j != reels.end(); ++j) {

				if ((*j)->main_subtitle()) {
					(*j)->main_subtitle()->asset()->write ((*j)->main_subtitle()->asset()->file().get());
				}
			}
		}
	}

	catch (FileError& e)
	{
		cerr << e.what() << " (" << e.filename() << ") when reading " << argv[1] << "\n";
		exit (EXIT_FAILURE);
	}
	catch (ReadError& e)
	{
		cerr << e.what() << " when reading " << argv[1] << "\n";
		exit (EXIT_FAILURE);
	}

	return 0;
}
