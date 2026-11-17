#include <iostream>
#include <vector>
#include "vehicul.hpp"
#include "client.hpp"
#include "contract.hpp"
#include "agentie.hpp"

int main() {
    std::string nume_agentie;
    double capital_initial = 0.0;

    std::cout << "Introdu numele agentiei si capitalul initial (ex: autorent 25000): ";
    if (!(std::cin >> nume_agentie >> capital_initial)) {
        nume_agentie = "rentcar_oficial";
        capital_initial = 20000.0;
    }

    agentie sediu(nume_agentie, capital_initial);
    sediu.incarca_flota_din_fisier("data/vehicule.txt");
    std::vector<client> clienti = agentie::incarca_clienti_din_fisier("data/clienti.txt");

    if (clienti.empty()) {
        clienti.push_back(client());
    }

    std::cout << "\nBine ai venit la " << sediu.get_denumire() << "!\n";
    std::cout << "Flota incarcata: " << sediu.get_nr_vehicule_parc()
              << " vehicule | Clienti inregistrati: " << clienti.size() << "\n";

    int urmatorul_id_contract = 101;
    int optiune = -1;

    while (optiune != 0) {
        std::cout << "\n================ MENIU AGENTIE AUTO ================\n";
        std::cout << "1. Afiseaza masinile disponibile\n";
        std::cout << "2. Alege o masina si vezi oferta completa (pret, garantie, carburant)\n";
        std::cout << "3. Inchiriaza o masina (emite contract)\n";
        std::cout << "4. Returneaza o masina (inchide contract si calculeaza penalizari)\n";
        std::cout << "5. Verifica si efectueaza revizia tehnica in service\n";
        std::cout << "6. Vezi contractele active, istoricul si bilantul financiar\n";
        std::cout << "7. Cauta masini dupa buget si vezi raportul flotei\n";
        std::cout << "8. Aplica cod promotional pe un contract activ (WEEKEND10 / FIDEL15)\n";
        std::cout << "0. Iesire\n";
        std::cout << "Alege optiunea: ";

        if (!(std::cin >> optiune)) {
            break;
        }

        if (optiune == 1) {
            sediu.afiseaza_vehicule_disponibile();
        } else if (optiune == 2) {
            std::string model;
            int zile = 1;
            int distanta_km = 100;
            double pret_litru = 7.5;

            std::cout << "Introdu modelul dorit (ex: golf, logan), nr. zile, distanta estimata (km) si pretul/litru:\n";
            if (std::cin >> model >> zile >> distanta_km >> pret_litru) {
                sediu.afiseaza_detalii_oferta(clienti[0], model, zile, distanta_km, pret_litru);
            }
        } else if (optiune == 3) {
            std::cout << "\nClienti disponibili:\n";
            for (size_t i = 0; i < clienti.size(); ++i) {
                std::cout << "[" << i << "] " << clienti[i] << "\n";
            }

            size_t idx_client = 0;
            std::string model;
            int zile = 1;
            std::cout << "Introdu indexul clientului, modelul masinii si numarul de zile:\n";
            if (std::cin >> idx_client >> model >> zile) {
                if (idx_client >= clienti.size()) {
                    idx_client = 0;
                }
                bool succes = sediu.incheie_contract(clienti[idx_client], model, zile, urmatorul_id_contract);
                if (succes) {
                    std::cout << "Contract #" << urmatorul_id_contract << " emis cu succes pentru "
                              << clienti[idx_client].get_nume() << "!\n";
                    urmatorul_id_contract++;
                } else {
                    std::cout << "Inchirierea a fost respinsa (client neeligibil sau model indisponibil).\n";
                }
            }
        } else if (optiune == 4) {
            int id_ctr = 0;
            int km_final = 0;
            double comb_lipsa = 0.0;
            int nota = 10;
            std::cout << "Introdu ID contract, kilometrajul la predare, litri combustibil lipsa si nota client (1-10):\n";
            if (std::cin >> id_ctr >> km_final >> comb_lipsa >> nota) {
                if (sediu.finalizeaza_contract(id_ctr, km_final, comb_lipsa, nota)) {
                    std::cout << "Contract #" << id_ctr << " finalizat cu succes!\n";
                } else {
                    std::cout << "Contractul #" << id_ctr << " nu a fost gasit printre cele active.\n";
                }
            }
        } else if (optiune == 5) {
            sediu.afiseaza_vehicule_in_service();
            double cost = sediu.efectueaza_mentenanta_generala();
            std::cout << "Cost total achitat pentru revizii: " << cost << " ron\n";
        } else if (optiune == 6) {
            sediu.afiseaza_contracte_active();
            sediu.afiseaza_istoric_contracte();
            std::cout << sediu;
            std::cout << "Garantii blocate in contracte active: " << sediu.calculeaza_valoare_garantii_blocate() << " ron\n";
            std::cout << "Venituri totale inregistrate: " << sediu.calculeaza_venituri_totale() << " ron\n";
        } else if (optiune == 7) {
            double buget = 200.0;
            std::cout << "Introdu bugetul maxim pe zi (ron): ";
            if (std::cin >> buget) {
                sediu.cauta_vehicule_dupa_buget(buget);
                sediu.afiseaza_raport_statistic_flota();
            }
        } else if (optiune == 8) {
            int id_ctr = 0;
            std::string cod;
            std::cout << "Introdu ID contract si codul promotional (WEEKEND10 sau FIDEL15): ";
            if (std::cin >> id_ctr >> cod) {
                if (sediu.aplica_promo_pe_contract(id_ctr, cod)) {
                    std::cout << "Cod promotional aplicat cu succes pe contractul #" << id_ctr << "!\n";
                } else {
                    std::cout << "Cod invalid sau contractul nu este eligibil.\n";
                }
            }
        } else if (optiune != 0) {
            std::cout << "Optiune invalida. Incearca din nou.\n";
        }
    }

    client c_copie(clienti[0]);
    c_copie = clienti[0];
    c_copie.adauga_calificativ(10);
    c_copie.set_nume(c_copie.get_nume());
    c_copie.set_ani_permis(c_copie.get_ani_permis());
    c_copie.afiseaza_profil_detaliat();

    vehicul v_nou("dacia", "sandero", 2021, 50000, 150.0, 6.0);
    v_nou.set_tarif_pe_zi(160.0);
    v_nou.set_consum_mediu(6.2);
    v_nou.afiseaza_fisa_tehnica();
    std::cout << "Eligibil drum lung (800 km): " << (v_nou.este_eligibil_drum_lung(800) ? "da" : "nu")
              << " | CASCO/zi: " << v_nou.calculeaza_cost_asigurare_casco(c_copie.get_ani_permis()) << " ron\n";
    std::cout << "Eligibil vehicul premium: "
              << (c_copie.poate_inchiria_vehicul_premium(v_nou.calculeaza_valoare_estimata_piata()) ? "da" : "nu") << "\n";
    sediu.adauga_vehicul_in_parc(v_nou);

    contract ctr_demo(999, c_copie, v_nou, 2);
    ctr_demo.adauga_penalizare(15.0);
    std::cout << "Simulare intarziere 2 zile: " << ctr_demo.simuleaza_cost_intarziere(2) << " ron\n";

    std::cout << "\n--- rezumat final sistem ---\n";
    std::cout << "Client verificat: " << c_copie.get_nume() << " (CNP: " << c_copie.get_cnp()
              << ", note: " << c_copie.get_nr_calificative()
              << ", prima nota: " << c_copie.get_calificativ_la_index(0) << ")\n";
    std::cout << "Vehicul adaugat: " << v_nou.get_marca() << " " << v_nou.get_model()
              << " (" << v_nou.get_an_fabricatie() << ", " << v_nou.get_kilometraj()
              << " km, revizie la: " << v_nou.get_km_ultima_revizie()
              << ", tarif: " << v_nou.get_tarif_pe_zi()
              << ", consum: " << v_nou.get_consum_mediu()
              << ", stare: " << v_nou.get_stare() << ")\n";
    std::cout << "Contract demo #" << ctr_demo.get_id_contract()
              << " | client: " << ctr_demo.get_client().get_nume()
              << " | vehicul: " << ctr_demo.get_vehicul().get_marca()
              << " | zile: " << ctr_demo.get_nr_zile()
              << " | km: " << ctr_demo.get_km_la_predare() << "-" << ctr_demo.get_km_la_restituire()
              << " | garantie: " << ctr_demo.get_garantie_blocata()
              << " | activ: " << ctr_demo.este_activ() << "\n";
    std::cout << "Agentie: " << sediu.get_denumire()
              << " | capital: " << sediu.get_capital_financiar()
              << " | active: " << sediu.get_nr_contracte_active()
              << " | istoric: " << sediu.get_nr_istoric_contracte() << "\n";

    return 0;
}