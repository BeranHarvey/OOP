#pragma once

#include <vector>
#include <string>
#include <format>

#include "image.h"
#include "pgm.h"
#include "debug.h"

template <typename ValueType>
class Dataset
{
  public:
    Dataset () = default;
    Dataset (const std::vector<std::string>& filenames) { load (filenames); }

    void load (const std::vector<std::string>& filenames);

    unsigned int size () const { return m_slices.size(); }

    Image<ValueType>&       operator[] (int n)       { return m_slices[n]; }
    const Image<ValueType>& operator[] (int n) const { return m_slices[n]; }

    std::vector<float> get_lvbp_timecourse () const;

    struct temporal_gradient {
      std::vector<float> timecourse;
      int thresh_frame;
    };

    temporal_gradient get_temporal_grad (const std::vector<float>& S);

  private:
    std::vector<Image<ValueType>> m_slices;
    std::vector<Image<ValueType>> m_masks;
};




template <typename ValueType>
void Dataset<ValueType>::load (const std::vector<std::string>& filenames)
{
  m_slices.clear();
  m_masks.clear();

  if (filenames.empty())
    throw std::runtime_error ("no filenames supplied when loading dataset");

  for (const auto& fname : filenames)
    m_slices.push_back (load_pgm (fname));

  // check that dimensions all match up:
  for (unsigned int n = 1; n < m_slices.size(); ++n) {
    if ( (m_slices[n].width() != m_slices[n-1].width()) ||
         (m_slices[n].height() != m_slices[n-1].height()) )
      throw std::runtime_error ("dimensions do not match across slices");
  }
  
  // create mask for each image
  for (unsigned int i = 0; i < m_slices.size(); ++i) {
    m_masks.push_back (create_mask (m_slices[i]));

    if ( (m_masks[i].width() != m_slices[i].width()) ||
         (m_masks[i].height() != m_slices[i].height()) ) {
      throw std::runtime_error ("dimensions of mask do not match slice");
    }

  }
  debug::log (std::format (
    "created {} masks of size {}x{}",
    m_masks.size(), m_masks[0].width(), m_masks[0].height()));
  
  debug::log (std::format (
      "loaded {} slices of size {}x{}",
      m_slices.size(), m_slices[0].width(), m_slices[0].height()));
}




template <typename ValueType>
std::vector<float> Dataset<ValueType>::get_lvbp_timecourse () const
{
  std::vector<float> S(size());

  // find average signal S[d] in lvbp region for each frame 
  for (unsigned d = 0; d < size(); ++d) {
    const auto slice = m_slices[d];
    const auto mask = m_masks[d];

    float sum = 0.0;
    int count = 0;

    for (int y = 0; y < slice.height(); ++y) {
      for (int x = 0; x < slice.width(); ++x) {
        if (mask(x,y) == 1) {
          sum += slice(x,y);
          ++count;
        }
      }
    }
    
    if (count > 0) {
      S[d] = sum / count;
    } else {
      S[d] = 0.0;
    }
  }

  debug::log (std::format (
      "found lvbp timecourse of S[d] average for {} slices", size()));

  return S;
}




template <typename ValueType>
Dataset<ValueType>::temporal_gradient Dataset<ValueType>::get_temporal_grad (const std::vector<float>& S)
{
  temporal_gradient temp_struct;
  temp_struct.timecourse = std::vector<float> (size());

  // find gradient timecourse and frame where threshold is met
  bool thresh_exceed = false;
  temp_struct.thresh_frame = 0;
  float thresh = 10.0; 

  for (unsigned d = 0; d < size() - 1; ++d) {
    temp_struct.timecourse[d] = S[d+1] - S[d];
    if (!thresh_exceed && temp_struct.timecourse[d] > thresh) {
      temp_struct.thresh_frame = d;
      thresh_exceed = true;
    }
  }
  debug::log (std::format (
      "found gradient timecourse for {} slices", size()));

  debug::log (std::format (
      "idnetified time frame of contrast arrival at frame {}",
      temp_struct.thresh_frame));

  return temp_struct;
}