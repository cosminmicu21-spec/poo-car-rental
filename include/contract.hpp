#ifndef contract_hpp
#define contract_hpp

#include <iostream>
#include <string>
#include "client.hpp"
#include "vehicul.hpp"

class contract {
private:
    int id_contract;
    client client_inchiriere;
    vehicul vehicul_inchiriat;
    int nr_zile;
    int km_la_predare;
    int km_la_restituire;
    double cost_baza;
    double penalizari;
    double garantie_blocata;
    bool activ;

    void calculeaza_tarif_initial();
    double calculeaza_garantie_recomandata() const;

public:
    contract();
    contract(int id_contract, const client& client_inchiriere, const vehicul& vehicul_inchiriat, int nr_zile);

    int get_id_contract() const;
    const client& get_client() const;
    const vehicul& get_vehicul() const;
    int get_nr_zile() const;
    int get_km_la_predare() const;
    int get_km_la_restituire() const;
    double get_cost_baza() const;
    double get_penalizari() const;
    double get_garantie_blocata() const;
    bool este_activ() const;

    void adauga_penalizare(double suma);
    double proceseaza_restituire(int km_efectuati, double combustibil_lipsa_litru, int calificativ_client);
    double calculeaza_cost_total_final() const;

    friend std::ostream& operator<<(std::ostream& os, const contract& c);
};

#include "../src/contract.cpp"
#endif