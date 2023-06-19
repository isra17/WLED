#pragma once

#include "wled.h"
#include "segments_mapping.h"

class UsermodSegmentsMapping : public Usermod {
private:

  static const char _name_str[];
  static const char _segments_str[];
  static const char _from_str[];
  static const char _to_str[];
  static const char _radius_str[];
  static const char _theta_str[];

  void _addSegmentMappingToJson(JsonObject json, SegmentMapping segment) {
      _addPolarCoordToJson(json.createNestedObject(FPSTR(_from_str)), segment.polar_from);
      _addPolarCoordToJson(json.createNestedObject(FPSTR(_to_str)), segment.polar_to);
  }

  void _addPolarCoordToJson(JsonObject obj, PolarCoord radial) {
      obj[FPSTR(_radius_str)] = radial.radius;
      obj[FPSTR(_theta_str)] = radial.theta;
  }

  SegmentMapping _jsonToSegmentMapping(JsonObjectConst obj) {
    return {
      .polar_from = _jsonToPolarCoord(obj[FPSTR(_from_str)].as<JsonObjectConst>()),
      .polar_to = _jsonToPolarCoord(obj[FPSTR(_to_str)].as<JsonObjectConst>()),
    };
  }

  PolarCoord _jsonToPolarCoord(JsonObjectConst obj) {
    return {
      .radius = obj[FPSTR(_radius_str)],
      .theta = obj[FPSTR(_theta_str)],
    };
  }

public:

  void addToConfig(JsonObject& root)
  {
    JsonObject top = root.createNestedObject(FPSTR(_name_str));
    JsonArray segments_json = top.createNestedArray(FPSTR(_segments_str));
    for (SegmentMapping segment : SegmentsMapping._mappings) {
      JsonObject segment_json = segments_json.createNestedObject();
      _addSegmentMappingToJson(segment_json, segment);
    }
  }

  bool readFromConfig(JsonObject& root)
  {
    SegmentsMapping._mappings.clear();

    JsonObject top = root[FPSTR(_name_str)];
    if (top.isNull()) {
      return true;
    }

    JsonArray segments_json = top[FPSTR(_segments_str)].as<JsonArray>();
    SegmentsMapping._mappings.reserve(segments_json.size());

    for (JsonVariant obj : segments_json) {
      SegmentsMapping._mappings.push_back(_jsonToSegmentMapping(obj.as<JsonObject>()));
    }
    return true;
  }

  uint16_t getId()
  {
    return USERMOD_ID_SEGMENTS_MAPPING;
  }

};

SegmentsMappingManager SegmentsMapping = SegmentsMappingManager();

// strings to reduce flash memory usage (used more than twice)
const char UsermodSegmentsMapping::_name_str[] PROGMEM = "segmap";
const char UsermodSegmentsMapping::_segments_str[] PROGMEM = "seg";
const char UsermodSegmentsMapping::_from_str[] PROGMEM = "from";
const char UsermodSegmentsMapping::_to_str[] PROGMEM = "to";
const char UsermodSegmentsMapping::_radius_str[] PROGMEM = "radius";
const char UsermodSegmentsMapping::_theta_str[] PROGMEM = "theta";
