#include "agentie.hpp"
#include <fstream>

int agentie::cauta_index_vehicul_dupa_model(const std::string& model) const {
    for (size_t i = 0; i < parc_auto.size(); ++i) {
        if (parc_auto[i].get_model() == model && parc_auto[i].este_disponibil()) {
            return static_cast<int>(i);
        }
    }
    return -1;
}

int agentie::cauta_index_contract(int id_contract) const {
    for (size_t i = 0; i < contracte_active.size(); ++i) {
        if (contracte_active[i].get_id_contract() == id_contract) {
            return static_cast<int>(i);
        }
    }
    return -1;
}

agentie::agentie()
    : denumire("agentie_auto"),
      capital_financiar(10000.0) {
}

agentie::agentie(const std::string& denumire, double capital_financiar)
    : denumire(denumire),
      capital_financiar((capital_financiar >= 0.0) ? capital_financiar : 10000.0) {
}

const std::string& agentie::get_denumire() const {
    return denumire;
}

double agentie::get_capital_financiar() const {
    return capital_financiar;
}

int agentie::get_nr_vehicule_parc() const {
    return static_cast<int>(parc_auto.size());
}

int agentie::get_nr_contracte_active() const {
    return static_cast<int>(contracte_active.size());
}

int agentie::get_nr_istoric_contracte() const {
    return static_cast<int>(istoric_contracte.size());
}

void agentie::adauga_vehicul_in_parc(const vehicul& v) {
    parc_auto.push_back(v);
}

bool agentie::incarca_flota_din_fisier(const std::string& cale_fisier) {
    std::ifstream fisier(cale_fisier);
    if (!fisier.is_open()) {
        std::string rezerva = "../" + cale_fisier;
        fisier.open(rezerva);
        if (!fisier.is_open()) {
            return false;
        }
    }
    vehicul v;
    while (fisier >> v) {
        parc_auto.push_back(v);
    }
    fisier.close();
    return true;
}

std::vector<client> agentie::incarca_clienti_din_fisier(const std::string& cale_fisier) {
    std::vector<client> lista;
    std::ifstream fisier(cale_fisier);
    if (!fisier.is_open()) {
        std::string rezerva = "../" + cale_fisier;
        fisier.open(rezerva);
        if (!fisier.is_open()) {
            return lista;
        }
    }
    client c;
    while (fisier >> c) {
        lista.push_back(c);
    }
    fisier.close();
    return lista;
}

bool agentie::incheie_contract(const client& client_curent, const std::string& model_vehicul, int zile, int id_nou) {
    if (!client_curent.este_client_eligibil()) {
        return false;
    }
    int index_masina = cauta_index_vehicul_dupa_model(model_vehicul);
    if (index_masina == -1) {
        return false;
    }

    if (!parc_auto[static_cast<size_t>(index_masina)].marcheaza_inchiriat()) {
        return false;
    }

    contract nou_contract(id_nou, client_curent, parc_auto[static_cast<size_t>(index_masina)], zile);
    contracte_active.push_back(nou_contract);
    capital_financiar += nou_contract.get_cost_baza();
    return true;
}

bool agentie::finalizeaza_contract(int id_contract, int km_la_final, double combustibil_lipsa, int nota_client) {
    int index = cauta_index_contract(id_contract);
    if (index == -1) {
        return false;
    }

    contract contract_finalizat = contracte_active[static_cast<size_t>(index)];
    double suma_incasata = contract_finalizat.proceseaza_restituire(km_la_final, combustibil_lipsa, nota_client);

    for (size_t i = 0; i < parc_auto.size(); ++i) {
        if (parc_auto[i].get_model() == contract_finalizat.get_vehicul().get_model()) {
            parc_auto[i] = contract_finalizat.get_vehicul();
            break;
        }
    }

    capital_financiar += contract_finalizat.get_penalizari();
    istoric_contracte.push_back(contract_finalizat);
    contracte_active.erase(contracte_active.begin() + index);

    if (suma_incasata > 0.0) {
        return true;
    }
    return false;
}

double agentie::efectueaza_mentenanta_generala() {
    double cost_total_reparatii = 0.0;
    for (size_t i = 0; i < parc_auto.size(); ++i) {
        if (parc_auto[i].necesita_revizie()) {
            double cost = parc_auto[i].efectueaza_revizie_tehnica();
            cost_total_reparatii += cost;
        }
    }
    capital_financiar -= cost_total_reparatii;
    return cost_total_reparatii;
}

void agentie::afiseaza_vehicule_disponibile() const {
    std::cout << "\n--- vehicule disponibile la " << denumire << " ---\n";
    int gasite = 0;
    for (size_t i = 0; i < parc_auto.size(); ++i) {
        if (parc_auto[i].este_disponibil()) {
            std::cout << "[" << i + 1 << "] " << parc_auto[i] << "\n";
            gasite++;
        }
    }
    if (gasite == 0) {
        std::cout << "niciun vehicul disponibil in acest moment.\n";
    }
}

void agentie::afiseaza_vehicule_in_service() const {
    std::cout << "\n--- flota aflata in mentenanta/service ---\n";
    int gasite = 0;
    for (size_t i = 0; i < parc_auto.size(); ++i) {
        if (parc_auto[i].necesita_revizie()) {
            std::cout << "[" << i + 1 << "] " << parc_auto[i] 
                      << " (indice uzura: " << parc_auto[i].calculeaza_indice_uzura() << "%)\n";
            gasite++;
        }
    }
    if (gasite == 0) {
        std::cout << "nu exista vehicule care necesita revizie.\n";
    }
}

double agentie::calculeaza_venituri_totale() const {
    double total = 0.0;
    for (size_t i = 0; i < istoric_contracte.size(); ++i) {
        total += istoric_contracte[i].calculeaza_cost_total_final();
    }
    for (size_t i = 0; i < contracte_active.size(); ++i) {
        total += contracte_active[i].get_cost_baza();
    }
    return total;
}

std::ostream& operator<<(std::ostream& os, const agentie& a) {
    os << "agentie auto: " << a.denumire << " | capital: " << a.capital_financiar << " ron\n";
    os << "flota totala: " << a.parc_auto.size() << " masini | contracte active: " 
       << a.contracte_active.size() << " | contracte arhivate: " << a.istoric_contracte.size() << "\n";
    return os;
}