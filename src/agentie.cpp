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
        return false;
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
        return lista;
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
void agentie::afiseaza_detalii_oferta(const client& client_curent, const std::string& model_vehicul,
                                      int zile, int distanta_km, double pret_litru) const {
    int idx = cauta_index_vehicul_dupa_model(model_vehicul);
    if (idx == -1) {
        std::cout << "Modelul '" << model_vehicul << "' nu este disponibil momentan in parc.\n";
        return;
    }

    const vehicul& v = parc_auto[static_cast<size_t>(idx)];
    double tarif_adaptat = v.calculeaza_tarif_adaptat(zile);
    double reducere = client_curent.calculeaza_procent_reducere();
    double cost_inchiriere = tarif_adaptat * (1.0 - (reducere / 100.0));
    double cost_carburant = v.estimeaza_cost_carburant(distanta_km, pret_litru);

    contract previzualizare(0, client_curent, v, zile);

    std::cout << "\n=== DETALII OFERTA PENTRU MODELUL: " << v.get_marca() << " " << v.get_model() << " ===\n";
    std::cout << "Specificatii: an " << v.get_an_fabricatie()
              << " | rulaj: " << v.get_kilometraj() << " km"
              << " | uzura: " << v.calculeaza_indice_uzura() << "%"
              << " | consum: " << v.get_consum_mediu() << " l/100km\n";
    std::cout << "Client: " << client_curent.get_nume()
              << " | reducere fidelitate: " << reducere << "%\n";
    std::cout << "Cost inchiriere (" << zile << " zile): " << cost_inchiriere << " ron\n";
    std::cout << "Garantie blocata (rambursabila): " << previzualizare.get_garantie_blocata() << " ron\n";
    std::cout << "Cost carburant estimat (" << distanta_km << " km): " << cost_carburant << " ron\n";
    std::cout << "Total estimat (inchiriere + carburant): " << (cost_inchiriere + cost_carburant) << " ron\n";
}

void agentie::afiseaza_contracte_active() const {
    std::cout << "\n--- contracte active in derulare ---\n";
    if (contracte_active.empty()) {
        std::cout << "nu exista contracte active.\n";
        return;
    }
    for (size_t i = 0; i < contracte_active.size(); ++i) {
        std::cout << contracte_active[i] << "\n\n";
    }
}

void agentie::cauta_vehicule_dupa_buget(double buget_maxim_zi) const {
    std::cout << "\n--- vehicule disponibile sub " << buget_maxim_zi << " ron/zi ---\n";
    int gasite = 0;
    for (size_t i = 0; i < parc_auto.size(); ++i) {
        if (parc_auto[i].este_disponibil() && parc_auto[i].get_tarif_pe_zi() <= buget_maxim_zi) {
            std::cout << "- " << parc_auto[i] << "\n";
            gasite++;
        }
    }
    if (gasite == 0) {
        std::cout << "niciun vehicul disponibil in acest buget.\n";
    }
}

void agentie::afiseaza_raport_statistic_flota() const {
    double valoare_totala = 0.0;
    double suma_uzura = 0.0;
    int disponibile = 0;
    int inchiriate = 0;
    int in_service = 0;

    for (size_t i = 0; i < parc_auto.size(); ++i) {
        valoare_totala += parc_auto[i].calculeaza_valoare_estimata_piata();
        suma_uzura += parc_auto[i].calculeaza_indice_uzura();
        if (parc_auto[i].get_stare() == 0 && !parc_auto[i].necesita_revizie()) {
            disponibile++;
        } else if (parc_auto[i].get_stare() == 1) {
            inchiriate++;
        } else {
            in_service++;
        }
    }

    double uzura_medie = parc_auto.empty() ? 0.0 : (suma_uzura / static_cast<double>(parc_auto.size()));
    std::cout << "\n=== RAPORT STATISTIC FLOTA (" << denumire << ") ===\n";
    std::cout << "Valoare estimata flota: " << valoare_totala << " ron\n";
    std::cout << "Uzura medie parc auto: " << uzura_medie << "%\n";
    std::cout << "Distributie: " << disponibile << " disponibile | "
              << inchiriate << " inchiriate | " << in_service << " in service\n";
}

bool agentie::aplica_promo_pe_contract(int id_contract, const std::string& cod_promo) {
    int idx = cauta_index_contract(id_contract);
    if (idx == -1) {
        return false;
    }
    size_t pos = static_cast<size_t>(idx);
    double cost_vechi = contracte_active[pos].get_cost_baza();
    if (contracte_active[pos].aplica_discount_promotional(cod_promo)) {
        double diferenta = cost_vechi - contracte_active[pos].get_cost_baza();
        capital_financiar -= diferenta;
        return true;
    }
    return false;
}

void agentie::afiseaza_istoric_contracte() const {
    std::cout << "\n--- istoric contracte arhivate ---\n";
    if (istoric_contracte.empty()) {
        std::cout << "nu exista contracte arhivate.\n";
        return;
    }
    for (size_t i = 0; i < istoric_contracte.size(); ++i) {
        std::cout << istoric_contracte[i].genereaza_rezumat_scurt()
                  << " | total achitat: " << istoric_contracte[i].calculeaza_cost_total_final() << " ron\n";
    }
}

double agentie::calculeaza_valoare_garantii_blocate() const {
    double suma_garantii = 0.0;
    for (size_t i = 0; i < contracte_active.size(); ++i) {
        suma_garantii += contracte_active[i].get_garantie_blocata();
    }
    return suma_garantii;
}
std::ostream& operator<<(std::ostream& os, const agentie& a) {
    os << "agentie auto: " << a.denumire << " | capital: " << a.capital_financiar << " ron\n";
    os << "flota totala: " << a.parc_auto.size() << " masini | contracte active: " 
       << a.contracte_active.size() << " | contracte arhivate: " << a.istoric_contracte.size() << "\n";
    return os;
}
