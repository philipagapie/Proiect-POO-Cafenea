#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <memory>
#include <stdexcept>
#include <sstream>
#include <map>
#include <algorithm>

using namespace std;

// Imparte un sir de caractere dupa un delimitator
vector<string> split(const string& str, char delimiter) {
    vector<string> tokens;
    string token;
    istringstream tokenStream(str);
    while (getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

// Verifica daca un sir este un numar intreg
bool esteIntreg(const string& str) {
    return !str.empty() && all_of(str.begin(), str.end(), ::isdigit);
}

// Verifica daca un sir este un numar real
bool esteReal(const string& str) {
    istringstream iss(str);
    double d;
    return iss >> d && iss.eof();
}

// Extrage ora din formatul HH:MM
int extrageOra(const string& timp) {
    vector<string> parti = split(timp, ':');
    if (parti.size() == 2 && esteIntreg(parti[0]) && esteIntreg(parti[1])) {
        return stoi(parti[0]);
    }
    throw invalid_argument("Formatul orei este invalid");
}

// Elimina spatiile dintr-un sir
string eliminaSpatii(const string& str) {
    string result = str;
    result.erase(remove(result.begin(), result.end(), ' '), result.end());
    return result;
}

// Clasa abstracta Angajat
class Angajat {
protected:
    string nume;
    string functie;
    int ora_inceput;
    int ora_sfarsit;
    int ora_pauza;

public:
    Angajat(const string& nume, const string& functie, int ora_inceput, int ora_sfarsit, int ora_pauza)
        : nume(nume), functie(functie), ora_inceput(ora_inceput), ora_sfarsit(ora_sfarsit), ora_pauza(ora_pauza) {}

    virtual ~Angajat() = default;
    virtual void afisareDetalii() const = 0;

    string getNume() const { return nume; }
    string getFunctie() const { return functie; }
    int getOraInceput() const { return ora_inceput; }
    int getOraSfarsit() const { return ora_sfarsit; }
};

// Clase derivate din Angajat
class Barista : public Angajat {
public:
    Barista(const string& nume, int ora_inceput, int ora_sfarsit, int ora_pauza)
        : Angajat(nume, "Barista", ora_inceput, ora_sfarsit, ora_pauza) {}

    void afisareDetalii() const override {
        cout << "Barista: " << nume << ", Ora inceput: " << ora_inceput << ", Ora sfarsit: " << ora_sfarsit << ", Ora pauza: " << ora_pauza << endl;
    }
};

class Manager : public Angajat {
public:
    Manager(const string& nume, int ora_inceput, int ora_sfarsit, int ora_pauza)
        : Angajat(nume, "Manager", ora_inceput, ora_sfarsit, ora_pauza) {}

    void afisareDetalii() const override {
        cout << "Manager: " << nume << ", Ora inceput: " << ora_inceput << ", Ora sfarsit: " << ora_sfarsit << ", Ora pauza: " << ora_pauza << endl;
    }
};

class Ospatar : public Angajat {
public:
    Ospatar(const string& nume, int ora_inceput, int ora_sfarsit, int ora_pauza)
        : Angajat(nume, "Ospatar", ora_inceput, ora_sfarsit, ora_pauza) {}

    void afisareDetalii() const override {
        cout << "Ospatar: " << nume << ", Ora inceput: " << ora_inceput << ", Ora sfarsit: " << ora_sfarsit << ", Ora pauza: " << ora_pauza << endl;
    }
};

vector<shared_ptr<Angajat>> angajati;

void adaugaAngajat(shared_ptr<Angajat> angajat) {
    angajati.push_back(angajat);
}

void stergeAngajat(const string& nume) {
    angajati.erase(remove_if(angajati.begin(), angajati.end(),
                             [&nume](const shared_ptr<Angajat>& angajat) { return angajat->getNume() == nume; }),
                   angajati.end());
}

void gestioneazaProgramAngajati() {
    for (const auto& angajat : angajati) {
        angajat->afisareDetalii();
    }
}

class Produs {
public:
    string nume;
    string categorie;
    double pret;
    int stoc;

    Produs(string nume, string categorie, double pret, int stoc)
        : nume(nume), categorie(categorie), pret(pret), stoc(stoc) {}
};

vector<Produs> produse;

void adaugaProdus(const Produs& produs) {
    produse.push_back(produs);
}

void eliminaProdus(const string& nume) {
    produse.erase(remove_if(produse.begin(), produse.end(),
                            [&nume](const Produs& produs) { return produs.nume == nume; }),
                  produse.end());
}

class Comanda {
public:
    int id;
    string client;
    vector<string> produse;
    double total;
    string tipClient;

    Comanda(int id, string client, vector<string> produse, double total, string tipClient)
        : id(id), client(client), produse(produse), total(total), tipClient(tipClient) {}
};

vector<Comanda> comenzi;

double calculeazaPretTotal(const vector<string>& produse, const map<string, double>& preturiProduse, const string& tipClient) {
    double total = 0.0;
    for (const auto& produs : produse) {
        string produsTrim = eliminaSpatii(produs);
        if (preturiProduse.find(produsTrim) != preturiProduse.end()) {
            total += preturiProduse.at(produsTrim);
        } else {
            cerr << "Produsul " << produs << " nu exista in lista de preturi." << endl;
            return -1; // Indica o eroare
        }
    }
    if (tipClient == "Fidel") {
        total *= 0.9; // 10% reducere pentru clienti fideli
    }
    return total;
}

vector<Comanda> creeazaComenzi(const vector<vector<string>>& date, const map<string, double>& preturiProduse) {
    vector<Comanda> comenzi;
    for (const auto& rand : date) {
        if (rand.size() >= 5 && esteIntreg(rand[0]) && !rand[1].empty() && !rand[2].empty() && esteReal(rand[3]) && !rand[4].empty()) {
            int id = stoi(rand[0]);
            string client = rand[1];
            string produseStr = rand[2];
            string tipClient = rand[4];
            vector<string> produse = split(produseStr, ',');
            for (auto& produs : produse) {
                produs = eliminaSpatii(produs);
            }
            double total = calculeazaPretTotal(produse, preturiProduse, tipClient);

            if (total != -1) {
                comenzi.emplace_back(id, client, produse, total, tipClient);
            } else {
                cerr << "Eroare la conversia datelor pentru comanda " << client << endl;
            }
        } else {
            cerr << "Eroare la conversia datelor pentru comanda " << rand[1] << endl;
        }
    }
    return comenzi;
}

class Eveniment {
public:
    int id;
    string tip;
    string data;
    double costTotal;
    double venitEstimat;

    Eveniment(int id, string tip, string data, double costTotal, double venitEstimat)
        : id(id), tip(tip), data(data), costTotal(costTotal), venitEstimat(venitEstimat) {}
};

vector<Eveniment> evenimente;

void adaugaEveniment(const Eveniment& eveniment) {
    evenimente.push_back(eveniment);
}

double calculeazaCostEvenimente() {
    double costTotal = 0.0;
    for (const auto& eveniment : evenimente) {
        costTotal += eveniment.costTotal;
    }
    return costTotal;
}

void genereazaRaportZilnic() {
    double incasari = 0.0;

    for (const auto& comanda : comenzi) {
        incasari += comanda.total;
    }

    cout << "Raport Zilnic:" << endl;
    cout << "Incasari: " << incasari << endl;
}

vector<shared_ptr<Angajat>> creeazaAngajati(const vector<vector<string>>& date) {
    vector<shared_ptr<Angajat>> angajati;
    for (const auto& rand : date) {
        if (rand.size() >= 6) {
            try {
                string nume = rand[1];
                string functie = rand[2];
                int ora_inceput = extrageOra(rand[3]);
                int ora_sfarsit = extrageOra(rand[4]);
                int ora_pauza = extrageOra(rand[5]);

                if (functie == "Barista") {
                    angajati.push_back(make_shared<Barista>(nume, ora_inceput, ora_sfarsit, ora_pauza));
                } else if (functie == "Manager") {
                    angajati.push_back(make_shared<Manager>(nume, ora_inceput, ora_sfarsit, ora_pauza));
                } else if (functie == "Ospatar") {
                    angajati.push_back(make_shared<Ospatar>(nume, ora_inceput, ora_sfarsit, ora_pauza));
                }
            } catch (const invalid_argument& e) {
                cerr << "Eroare la conversia orelor pentru angajatul " << rand[1] << ": " << e.what() << endl;
            }
        } else {
            cerr << "Eroare la conversia orelor pentru angajatul " << rand[1] << endl;
        }
    }
    return angajati;
}

vector<Produs> creeazaProduse(const vector<vector<string>>& date) {
    vector<Produs> produse;
    for (const auto& rand : date) {
        if (rand.size() >= 5 && !rand[2].empty() && !rand[1].empty() && esteReal(rand[3]) && esteIntreg(rand[4])) {
            string nume = eliminaSpatii(rand[2]);
            string categorie = rand[1];
            double pret = stod(rand[3]);
            int stoc = stoi(rand[4]);

            produse.emplace_back(nume, categorie, pret, stoc);
        } else {
            cerr << "Eroare la conversia pretului sau stocului pentru produsul " << rand[2] << endl;
        }
    }
    return produse;
}

vector<vector<string>> citesteDateDinFisier(const string& numeFisier) {
    vector<vector<string>> date;
    ifstream fisier(numeFisier);
    if (!fisier.is_open()) {
        throw runtime_error("Nu s-a putut deschide fisierul: " + numeFisier);
    }

    string linie;
    bool primaLinie = true;
    while (getline(fisier, linie)) {
        if (primaLinie) {
            primaLinie = false;
            continue;
        }
        stringstream ss(linie);
        string celula;
        vector<string> rand;
        while (getline(ss, celula, ',')) {
            rand.push_back(celula);
        }
        date.push_back(rand);
    }

    return date;
}

int main() {
    try {
        // Citire date din fisiere
        vector<vector<string>> dateAngajati = citesteDateDinFisier("angajati.csv");
        vector<vector<string>> dateProduse = citesteDateDinFisier("produse.csv");
        vector<vector<string>> dateComenzi = citesteDateDinFisier("comenzi.csv");
        vector<vector<string>> dateEvenimente = citesteDateDinFisier("evenimente.csv");

        // Creare obiecte
        angajati = creeazaAngajati(dateAngajati);
        produse = creeazaProduse(dateProduse);
        map<string, double> preturiProduse;
        for (const auto& produs : produse) {
            string produsTrim = eliminaSpatii(produs.nume);
            preturiProduse[produsTrim] = produs.pret;
        }

        // Afisare produse si preturi
        cout << "Produse si preturi:" << endl;
        for (const auto& pair : preturiProduse) {
            cout << pair.first << ": " << pair.second << endl;
        }

        // Afisare comenzi citite
        cout << endl;
        cout << "Comenzi citite:" << endl;
        for (const auto& rand : dateComenzi) {
            for (const auto& celula : rand) {
                cout << celula << " | ";
            }
            cout << endl;
        }

        comenzi = creeazaComenzi(dateComenzi, preturiProduse);

        // Afisare comenzi procesate
        cout << endl;
        cout << "Comenzi procesate:" << endl;
        for (const auto& comanda : comenzi) {
            cout << "ID: " << comanda.id << ", Client: " << comanda.client << ", Total: " << comanda.total << endl;
        }

        for (const auto& rand : dateEvenimente) {
            if (rand.size() >= 5 && esteIntreg(rand[0]) && !rand[1].empty() && !rand[2].empty() && esteReal(rand[3]) && esteReal(rand[4])) {
                int id = stoi(rand[0]);
                string tip = rand[1];
                string data = rand[2];
                double costTotal = stod(rand[3]);
                double venitEstimat = stod(rand[4]);
                evenimente.emplace_back(id, tip, data, costTotal, venitEstimat);
            } else {
                cerr << "Eroare la conversia datelor pentru evenimentul " << rand[1] << endl;
            }
        }

        // Gestionare angajati
        cout << endl;
        cout << "Adaugare angajat 'Ion Popescu':" << endl;
        adaugaAngajat(make_shared<Ospatar>("Ion Popescu", 8, 16, 12));
        gestioneazaProgramAngajati();
        cout << endl;
        cout << "Stergere angajat 'Ion Popescu':" << endl;
        stergeAngajat("Ion Popescu");
        gestioneazaProgramAngajati();
        cout << endl;

        // Gestionare produse
        cout << "Adaugare produs 'Long Black':" << endl;
        adaugaProdus(Produs("Long Black", "Cafea", 10.0, 100));
        for (const auto& produs : produse) {
            cout << produs.nume << " | " << produs.categorie << " | " << produs.pret << " | " << produs.stoc << endl;
        }
        cout << endl;

        cout << "Stergere produs 'Long Black':" << endl;
        eliminaProdus("Long Black");
        for (const auto& produs : produse) {
            cout << produs.nume << " | " << produs.categorie << " | " << produs.pret << " | " << produs.stoc << endl;
        }
        cout << endl;

        // Generare raport zilnic
        genereazaRaportZilnic();

    } catch (const exception& e) {
        cerr << "Eroare: " << e.what() << endl;
    }

    return 0;
}