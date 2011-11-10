/*
 * Jet.h
 *
 *  Created on: Jun 25, 2010
 *      Author: lkreczko
 */

#ifndef JET_H_
#define JET_H_
#include "Particle.h"
#include "TF1.h"
#include <vector>
#include <string>
#include <boost/array.hpp>

namespace BAT {

namespace BtagAlgorithm {
enum value {
    GenPartonFlavour,
    JetBProbabilityBTag,
    JetProbabilityBTag,
    SimpleSecondaryVertexHighEffBTag,
    SimpleSecondaryVertexHighPurBTag,
    TrackCountingHighEffBTag,
    TrackCountingHighPurBTag,
    NUMBER_OF_BTAGALGORITHMS
};

enum workingPoint {
    LOOSE, MEDIUM, TIGHT
};
}

namespace JetAlgorithm {
enum value {
    Calo_AntiKT_Cone05,
    CA08PF,
    PF2PAT,
    NUMBER_OF_JETALGORITHMS
};
//for nTuple branch names
const boost::array<std::string, JetAlgorithm::NUMBER_OF_JETALGORITHMS> prefixes = { {
        "goodPatJets",
        "goodPatJetsCA8PF",
        "goodPatJetsPFlow" } };

const boost::array<std::string, JetAlgorithm::NUMBER_OF_JETALGORITHMS> names = { {
        "CaloJets",
        "CA08PFJets",
        "PF2PATJets" } };
}

class Jet: public Particle {
public:

    Jet();
    Jet(const Particle& particle);
    Jet(double energy, double px, double py, double pz);
    virtual ~Jet();
    bool isGood() const;
    bool isBJet(BtagAlgorithm::value type = BtagAlgorithm::SimpleSecondaryVertexHighEffBTag,
            BtagAlgorithm::workingPoint wp = BtagAlgorithm::MEDIUM) const;
    JetAlgorithm::value getUsedAlgorithm() const;
    double emf() const;
    double n90Hits() const;
    double fHPD() const;
    double NOD() const;
    double CEF() const;
    double NHF() const;
    double NEF() const;
    double CHF() const;
    double NCH() const;
    double btagSSVHE() const;
    double getBJetL7EtCorrection() const;
    double getLightJetL7EtCorrection() const;

    void setUsedAlgorithm(JetAlgorithm::value algo);
    void setEMF(double emf);
    void setN90Hits(int n90Hits);
    void setFHPD(double fHPD);
    void setDiscriminatorForBtagType(double discriminator, BtagAlgorithm::value type);
    void setNOD(int nod);
    void setCEF(double cef);
    void setNHF(double nhf);
    void setNEF(double nef);
    void setCHF(double chf);
    void setNCH(double nch);
    void setRho(double rho);

    bool FirstDataLoosePFJetID() const;
    bool FirstDataLooseCaloJetID() const;
private:
    JetAlgorithm::value usedAlgorithm;
    double electromagneticFraction;
    int numberOfRecHitsContaining90PercentOfTheJetEnergy;
    double fractionOfEnergyIntheHottestHPDReadout;
    std::vector<double> btag_discriminators;
    //	double btagInData;
    int numberOfDaughters;
    double chargedEmEnergyFraction, neutralHadronEnergyFraction, neutralEmEnergyFraction;
    double chargedHadronEnergyFraction, chargedMultiplicity;
    int partonFlavour;
};

typedef boost::shared_ptr<Jet> JetPointer;
typedef std::vector<JetPointer> JetCollection;
}

#endif /* JET_H_ */
