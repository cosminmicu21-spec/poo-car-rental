#include <iostream>
#include <vector>
#include "vehicul.hpp"
#include "client.hpp"
#include "contract.hpp"
#include "agentie.hpp"

void apeleaza_toate_metodele() {
    vehicul v_demo;
    (void)v_demo.get_marca();
    (void)v_demo.get_km_ultima_revizie();
    (void)v_demo.get_tarif_pe_zi();
    (void)v_demo.get_consum_mediu();
    (void)v_demo.get_stare();

    client c_demo("Ion Popescu", "1990101123456", 5, nullptr, 0);
    (void)c_demo.getCnp();
    (void)c_demo.get_cnp();
    (void)c_demo.get_ani_permis();
    (void)c_demo.get_nr_calificative();

    contract ct_demo;
    (void)ct_demo.get_client();
    (void)ct_demo.get_nr_zile();
    (void)ct_demo.get_km_la_predare();
    (void)ct_demo.get_km_la_restituire();
    (void)ct_demo.get_garantie_blocata();
    (void)ct_demo.este_activ();
    ct_demo.adauga_penalizare(50.0);
    
    agentie ag_demo;
    (void)ag_demo.get_denumire();
    (void)ag_demo.get_capital_financiar();
    (void)ag_demo.get_nr_contracte_active();
    (void)ag_demo.get_nr_istoric_contracte();
}

int main() {
    apeleaza_toate_metodele();
    
    std::string nume_agentie;
    double capital_initial = 0.0;

    if (!(std::cin >> nume_agentie >> capital_initial)) {
        nume_agentie = "rentcar_oficial";
        capital_initial = 20000.0;
    }

    agentie sediu(nume_agentie, capital_initial);
    std::cout << "initializare: " << sediu << "\n";

    sediu.incarca_flota_din_fisier("data/vehicule.txt");
    std::vector<client> clienti = agentie::incarca_clienti_din_fisier("data/clienti.txt");

    std::cout << "s-au incarcat " << sediu.get_nr_vehicule_parc() << " vehicule si " 
              << clienti.size() << " clienti din fisiere.\n";

    if (!clienti.empty()) {
        std::cout << "\n--- demonstrare regula celor 3 (client) ---\n";
        client c_original = clienti[0];
        client c_copiat(c_original);
        client c_atribuit;
        c_atribuit = c_original;

        c_copiat.adauga_calificativ(10);
        c_atribuit.adauga_calificativ(2);

        std::cout << "original : " << c_original << "\n";
        std::cout << "copie    : " << c_copiat << "\n";
        std::cout << "atribuit : " << c_atribuit << "\n";
    }

    sediu.afiseaza_vehicule_disponibile();

    if (clienti.size() >= 2) {
        std::cout << "\n--- emitere contracte ---\n";
        int zile1 = 5;
        int distanta1 = 650;
        double comb1 = 8.0;
        std::cin >> zile1 >> distanta1 >> comb1;

        bool ok1 = sediu.incheie_contract(clienti[0], "golf", zile1, 101);
        std::cout << "inchiriere golf catre " << clienti[0].get_nume() 
                  << ": " << (ok1 ? "succes" : "respins") << "\n";

        int zile2 = 3;
        int distanta2 = 950;
        double comb2 = 22.5;
        std::cin >> zile2 >> distanta2 >> comb2;

        bool ok2 = sediu.incheie_contract(clienti[1], "logan", zile2, 102);
        std::cout << "inchiriere logan catre " << clienti[1].get_nume() 
                  << ": " << (ok2 ? "succes" : "respins") << "\n";

        std::cout << "\nstare agentie dupa contractare: " << sediu << "\n";

        std::cout << "\n--- procesare returnare si penalizari ---\n";
        sediu.finalizeaza_contract(101, 89200 + distanta1, comb1, 9);
        sediu.finalizeaza_contract(102, 48500 + distanta2, comb2, 6);
    }

    sediu.afiseaza_vehicule_in_service();
    double cost_service = sediu.efectueaza_mentenanta_generala();
    std::cout << "cost achitat pentru revizii: " << cost_service << " ron\n";

    vehicul v_test("dacia", "sandero", 2021, 50000, 150.0, 6.0);
    v_test.set_tarif_pe_zi(160.0);
    v_test.set_consum_mediu(6.2);
    std::cout << "cost carburant estimat: " << v_test.estimeaza_cost_carburant(500, 7.5) << " ron\n";
    std::cout << "km pana la revizie: " << v_test.calculeaza_km_pana_la_revizie() << "\n";
    sediu.adauga_vehicul_in_parc(v_test);

    if (!clienti.empty()) {
        clienti[0].set_nume("popescu andrei modificat");
        clienti[0].set_ani_permis(7);
        std::cout << "calificativ index 0: " << clienti[0].get_calificativ_la_index(0) << "\n";
    }

    std::cout << "\n--- bilant financiar final ---\n";
    std::cout << "total incasari inregistrate: " << sediu.calculeaza_venituri_totale() << " ron\n";
    std::cout << "situatie finala: " << sediu << "\n";

    return 0;
}