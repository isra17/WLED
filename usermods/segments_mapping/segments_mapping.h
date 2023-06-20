#pragma once

#include "wled.h"
#include <vector>


typedef struct polar_t {
  uint16_t radius;
  uint16_t theta;
} PolarCoord;

typedef struct segment_mapping_t {
  PolarCoord polar_from;
  PolarCoord polar_to;
} SegmentMapping;

class SegmentsMappingManager {
private:
  std::vector<SegmentMapping> _mappings;
  uint16_t _radius;

public:

  PolarCoord getPixelPolarCoord(uint8_t segment, uint16_t pixel, uint16_t segment_length) {
    if (segment >= _mappings.size()) {
      return {.radius = 0, .theta = 0};
    }

    SegmentMapping mapping = _mappings[segment];
    PolarCoord coord = mapping.polar_from;
    if (mapping.polar_from.radius != mapping.polar_to.radius) {
      coord.radius = map(
        pixel,
        0, segment_length,
        mapping.polar_from.radius, mapping.polar_to.radius
      );
    }

    if (mapping.polar_from.theta != mapping.polar_to.theta) {
      uint32_t from = mapping.polar_from.theta;
      uint32_t to = mapping.polar_to.theta;

      // Deal with wraparound, Segment arc is always the shortest.
      if ((max(from, to) - min(from, to)) > INT16_MAX) {
        if (from > to) {
          to += UINT16_MAX;
        } else {
          from += UINT16_MAX;
        }
      }

      coord.theta = map(
        pixel,
        0, segment_length,
        from, to
      ) % UINT16_MAX;
    }

    return coord;
  }

  void pushMapping(SegmentMapping mapping) {
    _radius = max(max(mapping.polar_to.radius, mapping.polar_from.radius), _radius);
    _mappings.push_back(mapping);
  }

  uint16_t getRadius() {
    return _radius;
  }

  friend class UsermodSegmentsMapping;
};

extern SegmentsMappingManager SegmentsMapping;
