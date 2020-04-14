/*
    Copyright (C) 2018 Carl Hetherington <cth@carlh.net>

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

#include "mono_picture_asset.h"
#include "stereo_picture_asset.h"
#include "sound_asset.h"
#include "stereo_picture_asset.h"
#include "smpte_subtitle_asset.h"
#include "atmos_asset.h"
#include "compose.hpp"
#include "asset_factory.h"
#include <boost/shared_ptr.hpp>

using boost::shared_ptr;
using namespace dcp;

shared_ptr<Asset>
dcp::asset_factory (boost::filesystem::path path, bool ignore_incorrect_picture_mxf_type)
{
	/* XXX: asdcplib does not appear to support discovery of read MXFs standard
	   (Interop / SMPTE)
	*/

	ASDCP::EssenceType_t type;
	if (ASDCP::EssenceType (path.string().c_str(), type) != ASDCP::RESULT_OK) {
		throw ReadError ("Could not find essence type");
	}
	switch (type) {
	case ASDCP::ESS_UNKNOWN:
	case ASDCP::ESS_MPEG2_VES:
		throw ReadError ("MPEG2 video essences are not supported");
	case ASDCP::ESS_JPEG_2000:
		try {
			return shared_ptr<MonoPictureAsset> (new MonoPictureAsset (path));
		} catch (dcp::MXFFileError& e) {
			if (ignore_incorrect_picture_mxf_type && e.number() == ASDCP::RESULT_SFORMAT) {
				/* Tried to load it as mono but the error says it's stereo; try that instead */
				return shared_ptr<StereoPictureAsset> (new StereoPictureAsset (path));
			} else {
				throw;
			}
		}
	case ASDCP::ESS_PCM_24b_48k:
	case ASDCP::ESS_PCM_24b_96k:
		return shared_ptr<SoundAsset> (new SoundAsset (path));
	case ASDCP::ESS_JPEG_2000_S:
		return shared_ptr<StereoPictureAsset> (new StereoPictureAsset (path));
	case ASDCP::ESS_TIMED_TEXT:
		return shared_ptr<SMPTESubtitleAsset> (new SMPTESubtitleAsset (path));
	case ASDCP::ESS_DCDATA_DOLBY_ATMOS:
		return shared_ptr<AtmosAsset> (new AtmosAsset (path));
	default:
		throw ReadError (String::compose ("Unknown MXF essence type %1 in %2", int(type), path.string()));
	}

	return shared_ptr<Asset>();
}
