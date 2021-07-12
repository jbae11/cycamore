#include "general_facility.h"

using cyclus::Material;
using cyclus::Composition;
using cyclus::toolkit::ResBuf;
using cyclus::toolkit::MatVec;
using cyclus::KeyError;
using cyclus::ValueError;
using cyclus::Request;

namespace cycamore {

GeneralFacility::GeneralFacility(cyclus::Context* ctx)
    : cyclus::Facility(ctx),
      power_name("power"),
      latitude(0.0),
      longitude(0.0),
      coordinates(latitude, longitude) {}


#pragma cyclus def clone cycamore::GeneralFacility

#pragma cyclus def schema cycamore::GeneralFacility

#pragma cyclus def annotations cycamore::GeneralFacility

#pragma cyclus def infiletodb cycamore::GeneralFacility

#pragma cyclus def snapshot cycamore::GeneralFacility

#pragma cyclus def snapshotinv cycamore::GeneralFacility

#pragma cyclus def initinv cycamore::GeneralFacility

void GeneralFacility::InitFrom(GeneralFacility* m) {
  #pragma cyclus impl initfromcopy cycamore::GeneralFacility
  cyclus::toolkit::CommodityProducer::Copy(m);
}

void GeneralFacility::InitFrom(cyclus::QueryableBackend* b) {
  #pragma cyclus impl initfromdb cycamore::GeneralFacility

  namespace tk = cyclus::toolkit;
  tk::CommodityProducer::Add(tk::Commodity(power_name),
                             tk::CommodInfo(capacity, capacity));

}

void GeneralFacility::EnterNotify() {
  cyclus::Facility::EnterNotify();

  std::cout << "ENTER\n";

  // read data
  check_data();
  scale_data();

  hours_per_dt = context()->dt()/3600;
  throughput_per_timestep = capacity * capacity_factor * hours_per_dt;

  RecordPosition();
}

void GeneralFacility::check_data(){

}

void GeneralFacility::scale_data(){

}


void GeneralFacility::Tick() {
  std::cout << "TICK " << context()->time() << "\n";
}

std::set<cyclus::RequestPortfolio<Material>::Ptr> GeneralFacility::GetMatlRequests() {
  using cyclus::RequestPortfolio;

  std::set<RequestPortfolio<Material>::Ptr> ports;
  Material::Ptr m;

  return ports;
}

void GeneralFacility::GetMatlTrades(
    const std::vector<cyclus::Trade<Material> >& trades,
    std::vector<std::pair<cyclus::Trade<Material>, Material::Ptr> >&
        responses) {
  using cyclus::Trade;

  
  for (int i = 0; i < trades.size(); i++) {
    double qty = trades[i].amt;
    //Material::Ptr m = spent.Pop(qty);
    //responses.push_back(std::make_pair(trades[i], m));
    //res_indexes.erase(m->obj_id());
  }
}

void GeneralFacility::AcceptMatlTrades(const std::vector<
    std::pair<cyclus::Trade<Material>, Material::Ptr> >& responses) {
  std::vector<std::pair<cyclus::Trade<cyclus::Material>,
                        cyclus::Material::Ptr> >::const_iterator trade;
  
  
  std::stringstream ss;

  for (trade = responses.begin(); trade != responses.end(); ++trade) {
    std::string commod = trade->first.request->commodity();
    //Material::Ptr m = trade->second;
    //index_res(m, commod);

    //if (core.quantity() < core_mass) {
    //  core.Push(m);
    //} else {
    //  fresh.Push(m);
    //}
  }
}

std::set<cyclus::BidPortfolio<Material>::Ptr> GeneralFacility::GetMatlBids(
    cyclus::CommodMap<Material>::type& commod_requests) {
    using cyclus::BidPortfolio;
  std::set<BidPortfolio<Material>::Ptr> ports;
  

  return ports;
}


void GeneralFacility::Tock() {
  std::cout << "TOCK " << context()->time() << "\n";
}


void GeneralFacility::Record(std::string name, std::string val) {
  context()
      ->NewDatum("ReactorEvents")
      ->AddVal("AgentId", id())
      ->AddVal("Time", context()->time())
      ->AddVal("Event", name)
      ->AddVal("Value", val)
      ->Record();
}

void GeneralFacility::RecordPosition() {
  std::string specification = this->spec();
  context()
      ->NewDatum("AgentPosition")
      ->AddVal("Spec", specification)
      ->AddVal("Prototype", this->prototype())
      ->AddVal("AgentId", id())
      ->AddVal("Latitude", latitude)
      ->AddVal("Longitude", longitude)
      ->Record();
}

extern "C" cyclus::Agent* ConstructGeneralFacility(cyclus::Context* ctx) {
  return new GeneralFacility(ctx);
}

}  // namespace cycamore
