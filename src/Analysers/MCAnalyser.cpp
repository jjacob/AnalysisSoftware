/*
 * MCAnalyser.cpp
 *
 *  Created on: Jul 5, 2011
 *      Author: phzss
 */

#include "../../interface/Analysers/MCAnalyser.h"

namespace BAT {

void MCAnalyser::analyse(const TopPairEventCandidate& ttbarEvent) {
    histMan->setCurrentCollection("MCStudy");
	MCParticlePointer top, antitop, b_from_top, b_from_antitop, W_plus, W_minus, electron, neutrino, quark_from_W, antiquark_from_W;
	JetCollection genJets = ttbarEvent.GenJets();
	JetPointer topBjet, antitopBjet, jet1fromW, jet2fromW;
	TtbarHypothesis MCttbarEvent;
	bool ejets_event = false;
	bool leptonic_Wplus_found = false, leptonic_Wminus_found = false;
	bool hadronic_Wplus_found = false, hadronic_Wminus_found = false;
	bool fully_hadronic_event = false, fully_leptonic_event = false;
	bool non_electron_leptonic_channel = false;
	int index = 0;
	int top_index = -100, antitop_index = -100, W_plus_index = -100, W_minus_index = -100, electron_index = -100, neutrino_index = -100,
			b_from_top_index = -100, b_from_antitop_index = -100, quark_from_W_index = -100, antiquark_from_W_index = -100;

	// MC ttbar reconstruction
	for (MCParticleCollection::const_iterator mc_particle = ttbarEvent.GenParticles().begin(); mc_particle != ttbarEvent.GenParticles().end(); ++mc_particle, ++index) {

		if ((*mc_particle)->status() != 3) continue;
		//top quark
		if ((*mc_particle)->pdgId() == 6) {
			top = *mc_particle;
			top_index = index;
			continue;
		}

		//anti-top quark
		if ((*mc_particle)->pdgId() == -6) {
			antitop = *mc_particle;
			antitop_index = index;
			continue;
		}

		//W+/W- bosons
		if (((*mc_particle)->pdgId() == 24) && ((*mc_particle)->motherIndex() == top_index)) {
			W_plus = *mc_particle;
			W_plus_index = index;
			continue;
		}

		if (((*mc_particle)->pdgId() == -24) && ((*mc_particle)->motherIndex() == antitop_index)) {
			W_minus = *mc_particle;
			W_minus_index = index;
			continue;
		}

		//b-quarks
		if (((*mc_particle)->pdgId() == 5) && ((*mc_particle)->motherIndex() == top_index)) {
			b_from_top = *mc_particle;
			b_from_top_index = index;
			continue;
		}
		if (((*mc_particle)->pdgId() == -5) && ((*mc_particle)->motherIndex() == antitop_index)) {
			b_from_antitop = *mc_particle;
			b_from_antitop_index = index;
			continue;
		}

		//W+ decay products
		if ((*mc_particle)->motherIndex()==W_plus_index) {
			if ((*mc_particle)->pdgId() == -11) {
				electron = *mc_particle;
				electron_index = index;
				leptonic_Wplus_found = true;
			}

			else if ((*mc_particle)->pdgId() == 12) {
				neutrino = *mc_particle;
				neutrino_index = index;
				leptonic_Wplus_found = true;
			}

			else if ((*mc_particle)->isLepton()) {
				non_electron_leptonic_channel = true;
				leptonic_Wplus_found = true;
			}

			else if ((*mc_particle)->isQuark()  && ((*mc_particle)->pdgId()>0)) {
				quark_from_W = *mc_particle;
				quark_from_W_index = index;
				hadronic_Wplus_found = true;
			}

			else if ((*mc_particle)->isQuark() && ((*mc_particle)->pdgId()<0)) {
				antiquark_from_W = *mc_particle;
				antiquark_from_W_index = index;
				hadronic_Wplus_found = true;
			}

			else {
				cout << "Something went wrong: W+ has unusual decay products." << endl;
			}

		}

		//W- decay products
		if ((*mc_particle)->motherIndex()==W_minus_index) {
			if ((*mc_particle)->pdgId() == 11) {
				electron = *mc_particle;
				electron_index = index;
				leptonic_Wminus_found = true;
			}

			else if ((*mc_particle)->pdgId() == -12) {
				neutrino = *mc_particle;
				neutrino_index = index;
				leptonic_Wminus_found = true;
			}

			else if ((*mc_particle)->isLepton()) {
				leptonic_Wminus_found = true;
				non_electron_leptonic_channel = true;
			}

			else if ((*mc_particle)->isQuark()  && ((*mc_particle)->pdgId()>0)) {
				quark_from_W = *mc_particle;
				quark_from_W_index = index;
				hadronic_Wminus_found = true;
			}

			else if ((*mc_particle)->isQuark() && ((*mc_particle)->pdgId()<0)) {
				antiquark_from_W = *mc_particle;
				antiquark_from_W_index = index;
				hadronic_Wminus_found = true;
			}

			else {
				cout << "Something went wrong: W- has unusual decay products." << endl;
			}
		}
	}

	//classify the event
	if (((leptonic_Wplus_found) || (leptonic_Wminus_found)) && ((hadronic_Wplus_found) || (hadronic_Wminus_found))
			&& (!non_electron_leptonic_channel)) { ejets_event = true; }
	if (((leptonic_Wplus_found) || (leptonic_Wminus_found)) && (!hadronic_Wplus_found)
			&& (!hadronic_Wminus_found)) { fully_leptonic_event = true; }
	if (((hadronic_Wplus_found) || (hadronic_Wminus_found)) && (!leptonic_Wplus_found)
			&& (!leptonic_Wminus_found)) { fully_hadronic_event = true; }

	if (ejets_event) {
		//matching genJets and partons
		if (genJets.size()>0) {
			int closestJetQuarkFromWIndex = quark_from_W->getClosestJetIndex(genJets);
			float minDR_quarkW = quark_from_W->deltaR(genJets.at(closestJetQuarkFromWIndex));
			jet1fromW = genJets.at(closestJetQuarkFromWIndex);

			int closestJetAntiQuarkFromWIndex = antiquark_from_W->getClosestJetIndex(genJets);
			float minDR_antiquarkW = antiquark_from_W->deltaR(genJets.at(closestJetAntiQuarkFromWIndex));
			jet2fromW = genJets.at(closestJetAntiQuarkFromWIndex);

			int closestJetBfromTopIndex = b_from_top->getClosestJetIndex(genJets);
			float minDR_BfromTop = b_from_top->deltaR(genJets.at(closestJetBfromTopIndex));
			topBjet = genJets.at(closestJetBfromTopIndex);

			int closestJetBfromAntiTopIndex = b_from_antitop->getClosestJetIndex(genJets);
			float minDR_BfromAntiTop = b_from_antitop->deltaR(genJets.at(closestJetBfromAntiTopIndex));
			antitopBjet = genJets.at(closestJetBfromAntiTopIndex);

			//delta R between genJets and partons histograms
			histMan->H1D("deltaRjet1")->Fill(minDR_quarkW);
			histMan->H1D("deltaRjet2")->Fill(minDR_antiquarkW);
			histMan->H1D("deltaRjet3")->Fill(minDR_BfromTop);
			histMan->H1D("deltaRjet4")->Fill(minDR_BfromAntiTop);

			histMan->H1D("deltaRjet_sum")->Fill(minDR_quarkW);
			histMan->H1D("deltaRjet_sum")->Fill(minDR_antiquarkW);
			histMan->H1D("deltaRjet_sum")->Fill(minDR_BfromTop);
			histMan->H1D("deltaRjet_sum")->Fill(minDR_BfromAntiTop);
		}

		if (leptonic_Wplus_found) {
			MCttbarEvent.leptonicTop = (ParticlePointer) top;
			MCttbarEvent.hadronicTop = (ParticlePointer) antitop;
			MCttbarEvent.leptonicW = (ParticlePointer) W_plus;
			MCttbarEvent.hadronicW = (ParticlePointer) W_minus;
			MCttbarEvent.leptonicBjet = topBjet;
			MCttbarEvent.hadronicBJet = antitopBjet;
			MCttbarEvent.jet1FromW = jet1fromW;
			MCttbarEvent.jet2FromW = jet2fromW;
			MCttbarEvent.neutrinoFromW = (ParticlePointer) neutrino;
			ElectronPointer e(new Electron(electron->energy(), electron->px(), electron->py(), electron->pz()));
			MCttbarEvent.leptonFromW = e;
		}
		else if (hadronic_Wplus_found) {
			MCttbarEvent.leptonicTop = (ParticlePointer) antitop;
			MCttbarEvent.hadronicTop = (ParticlePointer) top;
			MCttbarEvent.leptonicW = (ParticlePointer) W_minus;
			MCttbarEvent.hadronicW = (ParticlePointer) W_plus;
			MCttbarEvent.leptonicBjet = antitopBjet;
			MCttbarEvent.hadronicBJet = topBjet;
			MCttbarEvent.jet1FromW = jet1fromW;
			MCttbarEvent.jet2FromW = jet2fromW;
			MCttbarEvent.neutrinoFromW = (ParticlePointer) neutrino;
			ElectronPointer e(new Electron(electron->energy(), electron->px(), electron->py(), electron->pz()));
			MCttbarEvent.leptonFromW = e;
		}
		else cout << "ERROR: no hadronic or leptonic W's in semileptonic event (nonsense).\n";

		//comparing deltaR between genJets from W and closest partons
		histMan->H2D("deltaR_genJets_partons")->Fill(MCttbarEvent.jet1FromW->deltaR(MCttbarEvent.jet2FromW),quark_from_W->deltaR(antiquark_from_W));

		//invariant mass histograms
		histMan->H1D("W_inv_mass_from_truth_partons")->Fill(quark_from_W->invariantMass(antiquark_from_W));
		histMan->H1D("W_inv_mass_from_genJets")->Fill(MCttbarEvent.jet1FromW->invariantMass(MCttbarEvent.jet2FromW));
		histMan->H1D("top_leptonic_inv_mass_from_truth")->Fill(MCttbarEvent.leptonicW->invariantMass(MCttbarEvent.leptonicBjet));
		histMan->H1D("top_hadronic_inv_mass_from_truth")->Fill(MCttbarEvent.hadronicW->invariantMass(MCttbarEvent.hadronicBJet));

		histMan->H1D("m3_mc")->Fill(MCttbarEvent.M3());

		// comparing truth and reco objects
		if (ttbarEvent.passesFullTTbarEPlusJetSelection()) {
			histMan->H1D("m3_diff")->Fill(fabs(MCttbarEvent.M3()-ttbarEvent.M3()));

			double deltaRElectron = MCttbarEvent.leptonFromW->deltaR(ttbarEvent.getElectronFromWDecay());
			double deltaRLeptonicBjet = MCttbarEvent.leptonicBjet->deltaR(ttbarEvent.getLeptonicBJet());
			double deltaRHadronicBjet = MCttbarEvent.hadronicBJet->deltaR(ttbarEvent.getHadronicBJet());
			double deltaRjet11fromW = MCttbarEvent.jet1FromW->deltaR(ttbarEvent.getJet1FromHadronicW());
			double deltaRjet12fromW = MCttbarEvent.jet1FromW->deltaR(ttbarEvent.getJet2FromHadronicW());
			double deltaRjet21fromW = MCttbarEvent.jet2FromW->deltaR(ttbarEvent.getJet1FromHadronicW());
			double deltaRjet22fromW = MCttbarEvent.jet2FromW->deltaR(ttbarEvent.getJet2FromHadronicW());

			double deltaRjet1fromW = 0;
			double deltaRjet2fromW = 0;

			if (deltaRjet11fromW < deltaRjet12fromW) deltaRjet1fromW = deltaRjet11fromW; else deltaRjet1fromW = deltaRjet12fromW;
			if (deltaRjet21fromW < deltaRjet22fromW) deltaRjet2fromW = deltaRjet21fromW; else deltaRjet2fromW = deltaRjet22fromW;

			double deltaEtaNeutrino = MCttbarEvent.neutrinoFromW->deltaEta(ttbarEvent.getNeutrinoFromWDecay());
			double deltaPhiNeutrino = MCttbarEvent.neutrinoFromW->deltaPhi(ttbarEvent.getNeutrinoFromWDecay());
			histMan->H1D("deltaEtaNeutrino")->Fill(deltaEtaNeutrino);
			histMan->H1D("deltaPhiNeutrino")->Fill(deltaPhiNeutrino);

			histMan->H1D("deltaRElectron")->Fill(deltaRElectron);
			histMan->H1D("deltaRLeptonicBjet")->Fill(deltaRLeptonicBjet);
			histMan->H1D("deltaRHadronicBjet")->Fill(deltaRHadronicBjet);
			histMan->H1D("deltaRjet1fromW")->Fill(deltaRjet1fromW);
			histMan->H1D("deltaRjet2fromW")->Fill(deltaRjet2fromW);
			histMan->H1D("deltaRsum")->Fill(deltaRElectron+deltaRLeptonicBjet+deltaRHadronicBjet+deltaRjet1fromW+deltaRjet2fromW);

		}
	}
}

MCAnalyser::MCAnalyser(boost::shared_ptr<HistogramManager> histMan) :
    BasicAnalyser(histMan) {

}

MCAnalyser::~MCAnalyser() {
}

void MCAnalyser::createHistograms() {
    histMan->setCurrentCollection("MCStudy");
    histMan->addH1D("deltaRElectron", "delta R between truth and reco electron", 100, 0, 0.3);
    histMan->addH1D("deltaRLeptonicBjet", "delta R between truth and reco b-jet on leptonic side", 100, 0, 5.0);
    histMan->addH1D("deltaRHadronicBjet", "delta R between truth and reco b-jet on hadronic side", 100, 0, 5.0);
    histMan->addH1D("deltaRjet1fromW", "delta R between truth and reco jet1 from W decay", 100, 0, 5.0);
    histMan->addH1D("deltaRjet2fromW", "delta R between truth and reco jet2 from W decay", 100, 0, 5.0);
    histMan->addH1D("deltaRsum", "cumulative delta R between truth and reco", 100, 0, 15.0);

    histMan->addH1D("deltaRjet1", "delta R between quark from W and closest genJet", 100, 0, 5.0);
    histMan->addH1D("deltaRjet2", "delta R between antiquark from W and closest genJet", 100, 0, 5.0);
    histMan->addH1D("deltaRjet3", "delta R between b quark from top and closest genJet", 100, 0, 5.0);
    histMan->addH1D("deltaRjet4", "delta R between b quark from antitop and closest genJet", 100, 0, 5.0);
    histMan->addH1D("deltaRjet_sum", "summarized delta R between partons and genJets", 100, 0, 5.0);

    histMan->addH1D("deltaEtaNeutrino", "delta Eta between truth and reco neutrino", 100, -4, 4);
    histMan->addH1D("deltaPhiNeutrino", "delta Phi between truth and reco neutrino", 100, -4, 4);

    histMan->addH2D("deltaR_genJets_partons", "delta R between genJets from W as opposed to partons", 100, 0, 5, 100, 0, 5);

    histMan->addH1D("W_inv_mass_from_truth_partons", "W inv. mass from truth partons", 100, 0, 120);
    histMan->addH1D("W_inv_mass_from_genJets", "W inv. mass from genJets", 100, 0, 120);
    histMan->addH1D("top_leptonic_inv_mass_from_truth", "Leptonic top inv. mass from truth partons", 100, 100, 220);
    histMan->addH1D("top_hadronic_inv_mass_from_truth", "Haronic top inv. mass from truth partons", 100, 100, 220);
    histMan->addH1D("m3_mc", "M3 for truth event", 500, 0, 500);
    histMan->addH1D("m3_diff", "M3 difference between truth and reco", 500, 0, 500);
}

}
