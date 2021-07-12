#ifndef CYCAMORE_SRC_GENERAL_FACILITY_H_
#define CYCAMORE_SRC_GENERAL_FACILITY_H_

#include "cyclus.h"
#include "cycamore_version.h"



namespace cycamore {


class GeneralFacility : public cyclus::Facility,
  public cyclus::toolkit::CommodityProducer,
  public cyclus::toolkit::Position {


 public:
  GeneralFacility(cyclus::Context* ctx);
  virtual ~GeneralFacility(){};

  virtual std::string version() { return CYCAMORE_VERSION; }

  virtual void Tick();
  virtual void Tock();
  virtual void EnterNotify();

  virtual void AcceptMatlTrades(const std::vector<std::pair<
      cyclus::Trade<cyclus::Material>, cyclus::Material::Ptr> >& responses);

  virtual std::set<cyclus::RequestPortfolio<cyclus::Material>::Ptr>
  GetMatlRequests();

  virtual std::set<cyclus::BidPortfolio<cyclus::Material>::Ptr> GetMatlBids(
      cyclus::CommodMap<cyclus::Material>::type& commod_requests);

  virtual void GetMatlTrades(
      const std::vector<cyclus::Trade<cyclus::Material> >& trades,
      std::vector<std::pair<cyclus::Trade<cyclus::Material>,
                            cyclus::Material::Ptr> >& responses);

  #pragma cyclus decl

 private:

  // booleans for status
  bool is_operating = false;
  bool is_built = false;
  bool is_decom = false;
  int been_building_for = -1;
  bool has_fuel = false;
  bool back_waste_generated = false;
  double throughput_per_timestep;
  double hours_per_dt;


  std::vector<std::string> data_keys = {"front_waste", "front_need",
                                        "back_waste", "back_need",
                                        "opt_waste", "opt_fuel"};
  std::vector<std::string> entry_keys = {"val", "unit", "bib"};

  struct mat_stream {
    double val;
    std::string unit;
    double freq;
    cyclus::toolkit::ResBuf<cyclus::Material> buf;
  };
  std::map<std::string, mat_stream> data;


  void check_data();
  void scale_data();
  /// Records a reactor event to the output db with the given name and note val.
  void Record(std::string name, std::string val);
  
  #pragma cyclus var { \
    "uilabel": "Capacity", \
    "uitype": "range", \
    "range": [1,3], \
    "doc": "Capacity", \
  }
  double capacity;
  #pragma cyclus var { \
    "uilabel": "Capacity Factor", \
    "uitype": "range", \
    "range": [1,3], \
    "doc": "Capacity Factor", \
  }
  double capacity_factor;
  ///////// cycle params ///////////
  #pragma cyclus var { \
    "doc": "build_time", \
    "uilabel": "Build time", \
    "units": "time steps", \
  }
  int build_time;

  #pragma cyclus var { \
    "uilabel": "Path to JSON file", \
    "doc": "Path to JSON file" \
  }
  std::string json_path;

  #pragma cyclus var { \
    "uilabel": "Name of Key", \
    "doc": "Name of key" \
  }
  std::string key_name;

  #pragma cyclus var { \
    "default": "power", \
    "uilabel": "Name of Power", \
    "doc": "Name of power" \
  }
  std::string power_name;

  #pragma cyclus var { \
    "default": 0.0, \
    "uilabel": "Geographical latitude in degrees as a double", \
    "doc": "Latitude of the agent's geographical position. The value should " \
           "be expressed in degrees as a double." \
  }
  double latitude;

  #pragma cyclus var { \
    "default": 0.0, \
    "uilabel": "Geographical longitude in degrees as a double", \
    "doc": "Longitude of the agent's geographical position. The value should " \
           "be expressed in degrees as a double." \
  }
  double longitude;

  cyclus::toolkit::Position coordinates;

  /// Records an agent's latitude and longitude to the output db
  void RecordPosition();
};

} // namespace cycamore

#endif  // CYCAMORE_SRC_REACTOR_H_
