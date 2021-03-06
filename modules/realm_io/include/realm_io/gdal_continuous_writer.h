/**
* This file is part of OpenREALM.
*
* Copyright (C) 2018 Alexander Kern <laxnpander at gmail dot com> (Braunschweig University of Technology)
* For more information see <https://github.com/laxnpander/OpenREALM>
*
* OpenREALM is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* OpenREALM is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with OpenREALM. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef OPENREALM_GDAL_CONTINUOUS_WRITER_H
#define OPENREALM_GDAL_CONTINUOUS_WRITER_H

#include <realm_core/worker_thread_base.h>
#include <realm_io/gis_export.h>

namespace realm
{

namespace io
{

class GDALContinuousWriter : public WorkerThreadBase
{
public:
  using Ptr = std::shared_ptr<GDALContinuousWriter>;

public:
  GDALContinuousWriter(const std::string &thread_name, int64_t sleep_time, bool verbose);

  bool requestSaveGeoTIFF(const CvGridMap::Ptr &map,
                          const uint8_t &zone,
                          const std::string &filename,
                          bool do_build_overview = false,
                          bool do_split_save = false,
                          GDALProfile gdal_profile = GDALProfile::COG);

private:
  struct QueueElement
  {
    CvGridMap::Ptr map;
    uint8_t zone;
    std::string filename;
    bool do_build_overview;
    bool do_split_save;
    GDALProfile gdal_profile;

    using Ptr = std::shared_ptr<QueueElement>;
  };

  int _queue_size;

  std::mutex _mutex_save_requests;
  std::deque<QueueElement::Ptr> _save_requests;

  bool process() override;

  void reset() override;

  void finishCallback() override;

};

}

} // namespace realm

#endif //OPENREALM_GDAL_CONTINUOUS_WRITER_H
