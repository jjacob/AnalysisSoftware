/*
 * Enumerators.h
 *
 *  Created on: Jun 30, 2010
 *      Author: lkreczko
 */

#ifndef ENUMERATORS_H_
#define ENUMERATORS_H_
#include <string>
#include <boost/array.hpp>

namespace BAT {
namespace DataType {
enum value {
    DATA,
    ttbar,
    Wjets,
    Zjets,
    QCD_BCtoE_Pt20to30,
    QCD_BCtoE_Pt30to80,
    QCD_BCtoE_Pt80to170,
    QCD_EMEnriched_Pt20to30,
    QCD_EMEnriched_Pt30to80,
    QCD_EMEnriched_Pt80to170,//10
    PhotonJets_Pt40to100,
    PhotonJets_Pt100to200,
    PhotonJets_Pt200toInf,
    VQQ,
    singleTop_And_W,
    singleTopTChannel,
    singleTopSChannel,
    Zprime_M500GeV_W5GeV,
    Zprime_M500GeV_W50GeV,
    Zprime_M750GeV_W7500MeV,//20
    Zprime_M1TeV_W10GeV,
    Zprime_M1TeV_W100GeV,
    Zprime_M1250GeV_W12500MeV,
    Zprime_M1500GeV_W15GeV,
    Zprime_M1500GeV_W150GeV,
    Zprime_M2TeV_W20GeV,
    Zprime_M2TeV_W200GeV,
    Zprime_M3TeV_W30GeV,
    Zprime_M3TeV_W300GeV,
    Zprime_M4TeV_W40GeV,//30
    Zprime_M4TeV_W400GeV,
    NUMBER_OF_DATA_TYPES
};

const boost::array<std::string, DataType::NUMBER_OF_DATA_TYPES> names = { {
        "data",
        "ttjet",
        "wj",
        "zj",
        "bce1",
        "bce2",
        "bce3",
        "enri1",
        "enri2",
        "enri3",//10
        "pj1",
        "pj2",
        "pj3",
        "VqqJets",
        "tW",
        "tchan",
        "schan",
        "Zprime_M500GeV_W5GeV",
        "Zprime_M500GeV_W50GeV",
        "Zprime_M750GeV_W7500MeV",//20
        "Zprime_M1000GeV_W10GeV",
        "Zprime_M1000GeV_W100GeV",
        "Zprime_M1250GeV_W12500MeV",
        "Zprime_M1500GeV_W15GeV",
        "Zprime_M1500GeV_W150GeV",
        "Zprime_M2000GeV_W20GeV",
        "Zprime_M2000GeV_W200GeV",
        "Zprime_M3000GeV_W30GeV",
        "Zprime_M3000GeV_W300GeV",
        "Zprime_M4000GeV_W40GeV",//30
        "Zprime_M4000GeV_W400GeV" } };
}

}

#endif /* ENUMERATORS_H_ */