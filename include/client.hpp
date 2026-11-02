#ifndef client_hpp
#define client_hpp

#include <iostream>
#include <string>

class client {
private:
    std::string nume;
    std::string cnp;
    int ani_permis;
    int nr_calificative;
    int* calificative;

   static bool verifica_cnp_valid(const std::string& cnp_verificat);
    void copiaza_calificative(const int* sursa, int dimensiune);

public:
    client();
    client(const std::string& nume, const std::string& cnp, int ani_permis,
           const int* note_initiale, int nr_note);
    client(const client& other);
    client& operator=(const client& other);
    ~client();

    const std::string& get_nume() const;
    const std::string& get_cnp() const;
    const std::string& getCnp() const;
    int get_ani_permis() const;
    int get_nr_calificative() const;
    int get_calificativ_la_index(int index) const;

    void set_nume(const std::string& nume_nou);
    void set_ani_permis(int ani_noi);

    void adauga_calificativ(int nota_noua);
    double calculeaza_medie_calificative() const;
    double calculeaza_coeficient_risc() const;
    double calculeaza_procent_reducere() const;
    bool este_client_eligibil() const;

    friend std::ostream& operator<<(std::ostream& os, const client& c);
    friend std::istream& operator>>(std::istream& is, client& c);
};

#endif