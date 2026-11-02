#ifndef agentie_hpp
#define agentie_hpp

#include <iostream>
#include <string>
#include <vector>
#include "vehicul.hpp"
#include "client.hpp"
#include "contract.hpp"

class agentie {
private:
    std::string denumire;
    double capital_financiar;
    std::vector<vehicul> parc_auto;
    std::vector<contract> contracte_active;
    std::vector<contract> istoric_contracte;

    int cauta_index_vehicul_dupa_model(const std::string& model) const;
    int cauta_index_contract(int id_contract) const;

public:
    agentie();
    agentie(const std::string& denumire, double capital_financiar);

    const std::string& get_denumire() const;
    double get_capital_financiar() const;
    int get_nr_vehicule_parc() const;
    int get_nr_contracte_active() const;
    int get_nr_istoric_contracte() const;

    void adauga_vehicul_in_parc(const vehicul& v);
    bool incarca_flota_din_fisier(const std::string& cale_fisier);
    static std::vector<client> incarca_clienti_din_fisier(const std::string& cale_fisier);

    bool incheie_contract(client& client_curent, const std::string& model_vehicul, int zile, int id_nou);
    bool finalizeaza_contract(int id_contract, int km_la_final, double combustibil_lipsa, int nota_client);

    double efectueaza_mentenanta_generala();
    void afiseaza_vehicule_disponibile() const;
    void afiseaza_vehicule_in_service() const;
    double calculeaza_venituri_totale() const;

    friend std::ostream& operator<<(std::ostream& os, const agentie& a);
};

#endif