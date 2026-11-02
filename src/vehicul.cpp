#include "vehicul.hpp"

void vehicul::valideaza_date() {
    if (marca.empty()) {
        marca = "necunoscuta";
    }
    if (model.empty()) {
        model = "standard";
    }
    if (an_fabricatie < 1995 || an_fabricatie > 2026) {
        an_fabricatie = 2020;
    }
    if (kilometraj < 0) {
        kilometraj = 0;
    }
    if (km_ultima_revizie < 0 || km_ultima_revizie > kilometraj) {
        km_ultima_revizie = kilometraj;
    }
    if (tarif_pe_zi <= 10.0) {
        tarif_pe_zi = 100.0;
    }
    if (consum_mediu <= 1.0 || consum_mediu > 30.0) {
        consum_mediu = 6.5;
    }
    if (stare < 0 || stare > 2) {
        stare = 0;
    }
}

void vehicul::actualizeaza_necesitate_service() {
    int km_de_la_revizie = kilometraj - km_ultima_revizie;
    if (km_de_la_revizie >= 10000 && stare != 1) {
        stare = 2;
    }
}

vehicul::vehicul()
    : marca("necunoscuta"),
      model("standard"),
      an_fabricatie(2020),
      kilometraj(0),
      km_ultima_revizie(0),
      tarif_pe_zi(100.0),
      consum_mediu(6.0),
      stare(0) {
}

vehicul::vehicul(const std::string& marca, const std::string& model, int an_fabricatie,
                 int kilometraj, double tarif_pe_zi, double consum_mediu)
    : marca(marca),
      model(model),
      an_fabricatie(an_fabricatie),
      kilometraj(kilometraj),
      km_ultima_revizie((kilometraj / 10000) * 10000),
      tarif_pe_zi(tarif_pe_zi),
      consum_mediu(consum_mediu),
      stare(0) {
    valideaza_date();
    actualizeaza_necesitate_service();
}

const std::string& vehicul::get_marca() const {
    return marca;
}

const std::string& vehicul::get_model() const {
    return model;
}

int vehicul::get_an_fabricatie() const {
    return an_fabricatie;
}

int vehicul::get_kilometraj() const {
    return kilometraj;
}

int vehicul::get_km_ultima_revizie() const {
    return km_ultima_revizie;
}

double vehicul::get_tarif_pe_zi() const {
    return tarif_pe_zi;
}

double vehicul::get_consum_mediu() const {
    return consum_mediu;
}

int vehicul::get_stare() const {
    return stare;
}

void vehicul::set_tarif_pe_zi(double tarif_nou) {
    if (tarif_nou > 20.0) {
        tarif_pe_zi = tarif_nou;
    }
}

void vehicul::set_consum_mediu(double consum_nou) {
    if (consum_nou >= 2.0 && consum_nou <= 25.0) {
        consum_mediu = consum_nou;
    }
}

bool vehicul::este_disponibil() const {
    if (stare == 0 && !necesita_revizie()) {
        return true;
    }
    return false;
}

bool vehicul::necesita_revizie() const {
    int parcurs = kilometraj - km_ultima_revizie;
    if (parcurs >= 10000 || stare == 2) {
        return true;
    }
    return false;
}

int vehicul::calculeaza_km_pana_la_revizie() const {
    int parcurs = kilometraj - km_ultima_revizie;
    int ramas = 10000 - parcurs;
    if (ramas < 0) {
        return 0;
    }
    return ramas;
}

bool vehicul::marcheaza_inchiriat() {
    if (!este_disponibil()) {
        return false;
    }
    stare = 1;
    return true;
}

void vehicul::elibereaza_vehicul() {
    if (stare == 1) {
        stare = 0;
        actualizeaza_necesitate_service();
    }
}

void vehicul::adauga_kilometri_parcursi(int km_noi) {
    if (km_noi > 0) {
        kilometraj += km_noi;
    }
    if (stare != 1) {
        actualizeaza_necesitate_service();
    }
}

double vehicul::efectueaza_revizie_tehnica() {
    int km_depasiti = kilometraj - km_ultima_revizie;
    double cost_baza = 450.0;
    int vechime = 2026 - an_fabricatie;
    if (vechime > 0) {
        cost_baza += vechime * 65.0;
    }
    if (km_depasiti > 10000) {
        int suplimentar = km_depasiti - 10000;
        cost_baza += suplimentar * 0.25;
    }
    if (kilometraj > 100000) {
        cost_baza += 300.0;
    }
    km_ultima_revizie = kilometraj;
    stare = 0;
    return cost_baza;
}

double vehicul::calculeaza_indice_uzura() const {
    int vechime = 2026 - an_fabricatie;
    if (vechime < 0) {
        vechime = 0;
    }
    double uzura_km = (kilometraj / 200000.0) * 60.0;
    double uzura_an = (vechime / 15.0) * 40.0;
    double total = uzura_km + uzura_an;
    if (total > 100.0) {
        total = 100.0;
    }
    if (total < 0.0) {
        total = 0.0;
    }
    return total;
}

double vehicul::estimeaza_cost_carburant(int distanta_km, double pret_litru) const {
    if (distanta_km <= 0 || pret_litru <= 0.0) {
        return 0.0;
    }
    double litri_necesari = (distanta_km / 100.0) * consum_mediu;
    return litri_necesari * pret_litru;
}

double vehicul::calculeaza_tarif_adaptat(int nr_zile) const {
    if (nr_zile <= 0) {
        return 0.0;
    }
    double tarif_curent = tarif_pe_zi;
    double uzura = calculeaza_indice_uzura();
    if (uzura > 70.0) {
        tarif_curent = tarif_curent * 0.85;
    } else if (uzura > 45.0) {
        tarif_curent = tarif_curent * 0.92;
    }
    double cost_total = tarif_curent * nr_zile;
    if (nr_zile >= 7) {
        cost_total = cost_total * 0.90;
    } else if (nr_zile >= 4) {
        cost_total = cost_total * 0.95;
    }
    return cost_total;
}

std::ostream& operator<<(std::ostream& os, const vehicul& v) {
    os << v.marca << " " << v.model << " (" << v.an_fabricatie << ") | "
       << v.kilometraj << " km | tarif: " << v.tarif_pe_zi << " ron/zi | stare: ";
    if (v.stare == 0) {
        os << "disponibil";
    } else if (v.stare == 1) {
        os << "inchiriat";
    } else {
        os << "necesita service";
    }
    os << " (km pana la revizie: " << v.calculeaza_km_pana_la_revizie() << ")";
    return os;
}

std::istream& operator>>(std::istream& is, vehicul& v) {
    is >> v.marca >> v.model >> v.an_fabricatie >> v.kilometraj >> v.tarif_pe_zi >> v.consum_mediu;
    v.km_ultima_revizie = (v.kilometraj / 10000) * 10000;
    v.stare = 0;
    v.valideaza_date();
    v.actualizeaza_necesitate_service();
    return is;
}