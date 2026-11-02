#ifndef vehicul_hpp
#define vehicul_hpp

#include <iostream>
#include <string>

class vehicul {
private:
    std::string marca;
    std::string model;
    int an_fabricatie;
    int kilometraj;
    int km_ultima_revizie;
    double tarif_pe_zi;
    double consum_mediu;
    int stare;

    void valideaza_date();
    void actualizeaza_necesitate_service();

public:
    vehicul();
    vehicul(const std::string& marca, const std::string& model, int an_fabricatie,
            int kilometraj, double tarif_pe_zi, double consum_mediu);

    const std::string& get_marca() const;
    const std::string& get_model() const;
    int get_an_fabricatie() const;
    int get_kilometraj() const;
    int get_km_ultima_revizie() const;
    double get_tarif_pe_zi() const;
    double get_consum_mediu() const;
    int get_stare() const;

    void set_tarif_pe_zi(double tarif_nou);
    void set_consum_mediu(double consum_nou);

    bool este_disponibil() const;
    bool necesita_revizie() const;
    int calculeaza_km_pana_la_revizie() const;

    bool marcheaza_inchiriat();
    void elibereaza_vehicul();
    void adauga_kilometri_parcursi(int km_noi);
    double efectueaza_revizie_tehnica();

    double calculeaza_indice_uzura() const;
    double estimeaza_cost_carburant(int distanta_km, double pret_litru) const;
    double calculeaza_tarif_adaptat(int nr_zile) const;

    friend std::ostream& operator<<(std::ostream& os, const vehicul& v);
    friend std::istream& operator>>(std::istream& is, vehicul& v);
};

#endif
