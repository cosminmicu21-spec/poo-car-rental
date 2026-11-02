#include "client.hpp"

bool client::verifica_cnp_valid(const std::string& cnp_verificat) {
    if (cnp_verificat.length() != 13) {
        return false;
    }
    for (size_t i = 0; i < cnp_verificat.length(); ++i) {
        if (cnp_verificat[i] < '0' || cnp_verificat[i] > '9') {
            return false;
        }
    }
    return true;
}

void client::copiaza_calificative(const int* sursa, int dimensiune) {
    if (dimensiune > 0 && sursa != nullptr) {
        nr_calificative = dimensiune;
        calificative = new int[static_cast<size_t>(nr_calificative)];
        for (int i = 0; i < nr_calificative; ++i) {
            if (sursa[i] < 1) {
                calificative[i] = 1;
            } else if (sursa[i] > 10) {
                calificative[i] = 10;
            } else {
                calificative[i] = sursa[i];
            }
        }
    } else {
        nr_calificative = 0;
        calificative = nullptr;
    }
}

client::client()
    : nume("anonim"),
      cnp("1000000000000"),
      ani_permis(1),
      nr_calificative(0),
      calificative(nullptr) {
}

client::client(const std::string& nume, const std::string& cnp, int ani_permis,
               const int* note_initiale, int nr_note)
    : nume(nume),
      cnp(cnp),
      ani_permis(ani_permis),
      nr_calificative(0),
      calificative(nullptr) {
    if (this->nume.empty()) {
        this->nume = "anonim";
    }
    if (!verifica_cnp_valid(this->cnp)) {
        this->cnp = "1000000000000";
    }
    if (this->ani_permis < 0) {
        this->ani_permis = 0;
    }
    copiaza_calificative(note_initiale, nr_note);
}

client::client(const client& other)
    : nume(other.nume),
      cnp(other.cnp),
      ani_permis(other.ani_permis),
      nr_calificative(0),
      calificative(nullptr) {
    copiaza_calificative(other.calificative, other.nr_calificative);
}

client& client::operator=(const client& other) {
    if (this != &other) {
        delete[] calificative;
        calificative = nullptr;
        nr_calificative = 0;

        nume = other.nume;
        cnp = other.cnp;
        ani_permis = other.ani_permis;
        copiaza_calificative(other.calificative, other.nr_calificative);
    }
    return *this;
}

client::~client() {
    delete[] calificative;
    calificative = nullptr;
}

const std::string& client::get_nume() const {
    return nume;
}

const std::string& client::getCnp() const {
    return cnp;
}

const std::string& client::get_cnp() const {
    return cnp;
}

int client::get_ani_permis() const {
    return ani_permis;
}

int client::get_nr_calificative() const {
    return nr_calificative;
}

int client::get_calificativ_la_index(int index) const {
    if (index >= 0 && index < nr_calificative && calificative != nullptr) {
        return calificative[index];
    }
    return 0;
}

void client::set_nume(const std::string& nume_nou) {
    if (!nume_nou.empty()) {
        nume = nume_nou;
    }
}

void client::set_ani_permis(int ani_noi) {
    if (ani_noi >= 0 && ani_noi <= 70) {
        ani_permis = ani_noi;
    }
}

void client::adauga_calificativ(int nota_noua) {
    if (nota_noua < 1) {
        nota_noua = 1;
    }
    if (nota_noua > 10) {
        nota_noua = 10;
    }

    int* tablou_nou = new int[static_cast<size_t>(nr_calificative + 1)];
    for (int i = 0; i < nr_calificative; ++i) {
        tablou_nou[i] = calificative[i];
    }
    tablou_nou[nr_calificative] = nota_noua;

    delete[] calificative;
    calificative = tablou_nou;
    nr_calificative++;
}

double client::calculeaza_medie_calificative() const {
    if (nr_calificative == 0 || calificative == nullptr) {
        return 8.0;
    }
    int suma = 0;
    for (int i = 0; i < nr_calificative; ++i) {
        suma += calificative[i];
    }
    return static_cast<double>(suma) / nr_calificative;
}

double client::calculeaza_coeficient_risc() const {
    double risc = 1.0;

    if (ani_permis < 2) {
        risc += 0.45;
    } else if (ani_permis < 5) {
        risc += 0.15;
    } else if (ani_permis >= 10) {
        risc -= 0.15;
    }

    double medie = calculeaza_medie_calificative();
    if (medie < 6.0) {
        risc += 0.50;
    } else if (medie < 7.5) {
        risc += 0.25;
    } else if (medie >= 9.0) {
        risc -= 0.15;
    }

    int penalizari = 0;
    for (int i = 0; i < nr_calificative; ++i) {
        if (calificative[i] <= 5) {
            penalizari++;
        }
    }
    risc += penalizari * 0.10;

    if (risc < 0.70) {
        risc = 0.70;
    }
    return risc;
}

double client::calculeaza_procent_reducere() const {
    double medie = calculeaza_medie_calificative();
    double reducere = 0.0;

    if (medie >= 9.0 && nr_calificative >= 3) {
        reducere += 10.0;
    } else if (medie >= 8.0 && nr_calificative >= 2) {
        reducere += 5.0;
    }

    if (ani_permis >= 5) {
        reducere += 5.0;
    }
    return reducere;
}

bool client::este_client_eligibil() const {
    if (ani_permis < 1) {
        return false;
    }
    if (nr_calificative >= 2 && calculeaza_medie_calificative() < 4.5) {
        return false;
    }
    return true;
}

std::ostream& operator<<(std::ostream& os, const client& c) {
    os << "client: " << c.nume << " | cnp: " << c.cnp
       << " | experienta: " << c.ani_permis << " ani"
       << " | medie: " << c.calculeaza_medie_calificative()
       << " | risc: " << c.calculeaza_coeficient_risc() << " | note: [";
    for (int i = 0; i < c.nr_calificative; ++i) {
        os << c.calificative[i];
        if (i + 1 < c.nr_calificative) {
            os << ", ";
        }
    }
    os << "]";
    return os;
}

std::istream& operator>>(std::istream& is, client& c) {
    std::string prenume_citit;
    std::string nume_familie_citit;
    std::string cnp_citit;
    int ani_cititi = 0;
    int nr_note_citite = 0;

    if (!(is >> prenume_citit >> nume_familie_citit >> cnp_citit >> ani_cititi >> nr_note_citite)) {
        return is;
    }

    c.nume = prenume_citit + " " + nume_familie_citit;
    if (c.verifica_cnp_valid(cnp_citit)) {
        c.cnp = cnp_citit;
    } else {
        c.cnp = "1000000000000";
    }
    c.ani_permis = (ani_cititi >= 0) ? ani_cititi : 0;

    delete[] c.calificative;
    c.calificative = nullptr;
    c.nr_calificative = 0;

    if (nr_note_citite > 0) {
        c.nr_calificative = nr_note_citite;
        c.calificative = new int[static_cast<size_t>(nr_note_citite)];
        for (int i = 0; i < nr_note_citite; ++i) {
            int nota = 8;
            is >> nota;
            if (nota < 1) {
                nota = 1;
            }
            if (nota > 10) {
                nota = 10;
            }
            c.calificative[i] = nota;
        }
    }
    return is;
}