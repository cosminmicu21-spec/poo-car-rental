#include "contract.hpp"

void contract::calculeaza_tarif_initial() {
    double tarif_zilnic = vehicul_inchiriat.calculeaza_tarif_adaptat(nr_zile);
    double reducere_client = client_inchiriere.calculeaza_procent_reducere();
    cost_baza = tarif_zilnic * (1.0 - (reducere_client / 100.0));
    garantie_blocata = calculeaza_garantie_recomandata();
}

double contract::calculeaza_garantie_recomandata() const {
    double garantie = 500.0;
    garantie = garantie * client_inchiriere.calculeaza_coeficient_risc();
    if (vehicul_inchiriat.get_an_fabricatie() >= 2022) {
        garantie += 350.0;
    }
    return garantie;
}

contract::contract()
    : id_contract(0),
      client_inchiriere(),
      vehicul_inchiriat(),
      nr_zile(1),
      km_la_predare(0),
      km_la_restituire(0),
      cost_baza(100.0),
      penalizari(0.0),
      garantie_blocata(500.0),
      activ(false) {
}

contract::contract(int id_contract, const client& client_inchiriere, const vehicul& vehicul_inchiriat, int nr_zile)
    : id_contract(id_contract),
      client_inchiriere(client_inchiriere),
      vehicul_inchiriat(vehicul_inchiriat),
      nr_zile((nr_zile > 0) ? nr_zile : 1),
      km_la_predare(vehicul_inchiriat.get_kilometraj()),
      km_la_restituire(vehicul_inchiriat.get_kilometraj()),
      cost_baza(0.0),
      penalizari(0.0),
      garantie_blocata(0.0),
      activ(true) {
    calculeaza_tarif_initial();
}

int contract::get_id_contract() const {
    return id_contract;
}

const client& contract::get_client() const {
    return client_inchiriere;
}

const vehicul& contract::get_vehicul() const {
    return vehicul_inchiriat;
}

int contract::get_nr_zile() const {
    return nr_zile;
}

int contract::get_km_la_predare() const {
    return km_la_predare;
}

int contract::get_km_la_restituire() const {
    return km_la_restituire;
}

double contract::get_cost_baza() const {
    return cost_baza;
}

double contract::get_penalizari() const {
    return penalizari;
}

double contract::get_garantie_blocata() const {
    return garantie_blocata;
}

bool contract::este_activ() const {
    return activ;
}

void contract::adauga_penalizare(double suma) {
    if (suma > 0.0) {
        penalizari += suma;
    }
}

double contract::proceseaza_restituire(int km_efectuati, double combustibil_lipsa_litru, int calificativ_client) {
    if (!activ) {
        return 0.0;
    }

    if (km_efectuati >= km_la_predare) {
        km_la_restituire = km_efectuati;
    } else {
        km_la_restituire = km_la_predare;
    }

    int km_rulati_efectiv = km_la_restituire - km_la_predare;
    vehicul_inchiriat.adauga_kilometri_parcursi(km_rulati_efectiv);
    vehicul_inchiriat.elibereaza_vehicul();

    int limita_km_admis = nr_zile * 250;
    if (km_rulati_efectiv > limita_km_admis) {
        int km_suplimentari = km_rulati_efectiv - limita_km_admis;
        penalizari += km_suplimentari * 0.95;
    }

    if (combustibil_lipsa_litru > 0.0) {
        penalizari += combustibil_lipsa_litru * 8.5;
    }

    client_inchiriere.adauga_calificativ(calificativ_client);

    activ = false;
    return calculeaza_cost_total_final();
}

double contract::calculeaza_cost_total_final() const {
    return cost_baza + penalizari;
}

std::ostream& operator<<(std::ostream& os, const contract& c) {
    os << "=== contract #" << c.id_contract << " | stare: " << (c.activ ? "activ" : "finalizat") << " ===\n";
    os << "date client: " << c.client_inchiriere << "\n";
    os << "date vehicul: " << c.vehicul_inchiriat << "\n";
    os << "durata: " << c.nr_zile << " zile | cost baza: " << c.cost_baza 
       << " ron | penalizari: " << c.penalizari 
       << " ron | total de plata: " << c.calculeaza_cost_total_final() << " ron";
    return os;
}